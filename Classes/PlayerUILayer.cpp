#include "PlayerUILayer.h"
#include "Player.h"
#include "Item.h"
#include "HelloWorldScene.h"
#include "SpellBook.h"
#include "DataMgr.h"
#include "PlayerEquipWindow.h"
#include "Spell.h"
#include "PlayerUISettingSprite.h"
#include "QuestBook.h"
#include "NotifyMgr.h"
#include "PlayerEquipWindow.h"
#include "VirtualRocker.h"
#include "DeadTalkClass.h"


#pragma execution_character_set("utf-8")

static PlayerBag* _PlayerBag = nullptr;
static PlayerUILayer* _PlayerUILayer = nullptr;

Slot::Slot(const std::string& url)
{
	if (url.empty())
		initWithFile(PlayerSlotImage);
	else
		initWithFile(url.c_str());

	autorelease();
	m_Item = nullptr;
	m_DisPlaySprite = nullptr;
}

Slot::~Slot()
{
	if (GetItem())
		delete m_Item;
	removeAllChildrenWithCleanup(true);
}

void Slot::SetItem(Item* pItem)	
{
	if (!pItem)
	{
		m_Item = nullptr;
		if (m_DisPlaySprite)
		{
			m_DisPlaySprite->removeFromParentAndCleanup(true);
			m_DisPlaySprite = nullptr;
		}
		return;
	}
	m_Item = pItem; 
	if (!m_DisPlaySprite)
	{
		m_DisPlaySprite = Sprite::create(pItem->GetIconUrl().c_str());
		m_DisPlaySprite->setScale(getBoundingBox().size.width / m_DisPlaySprite->getBoundingBox().size.width);
		m_DisPlaySprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
		addChild(m_DisPlaySprite);
	}
	else
	{
		setTexture(pItem->GetIconUrl().c_str());
	}
	sPlayer->SendUpdateValueRequire();
	return;
}

void Slot::SwapItem(Slot* Instead)
{
	if (!GetItem() || !Instead || Instead == this || !Instead->isVisible())
	{
		if (m_DisPlaySprite)
			m_DisPlaySprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
		return;
	}

	//Equip
	if (Instead->getParent() == sPlayerEquip)
	{
		if (!GetItem()->CanEquipToSlot(Instead) || !sPlayer->CanEquipItem(GetItem()))
		{
			sNotifyMgr->ShowNotify("You Can't Equip This!");
			m_DisPlaySprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
			return;
		}
	}

	Item* NewSlotItem = Instead->GetItem();
	Item* OldSlotItem = GetItem();
	if (NewSlotItem)
		SetItem(NewSlotItem);
	else SetItem(nullptr);
	Instead->SetItem(OldSlotItem);

	sPlayer->CalcItemValues();
	sPlayerValueWindow->ResetValueDefault();
}

PlayerBag::PlayerBag()
{
	//_PlayerBag = this;
	initWithFile(PlayerBagImage);
	autorelease();
	InitPage();
	SwapPage(Page_One);
	TouchedSprite = nullptr;
	LoadInventory();
	setVisible(false);
}

PlayerBag::~PlayerBag()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_PlayerBag = nullptr;
}

void PlayerBag::LoadInventory()
{
	if (!sPlayer)
		return;
	char msg[255];//				0		1		2		3
	snprintf(msg, 255, "SELECT item_entry,bag_page,bag_slot,count FROM player_inventory WHERE guid = %u", sPlayer->GetGuid());
	Result result;
	if (sDataMgr->selectUnitDataList(msg, result))
	{
		if (result.empty()) return;
		else
		{
			for (Result::iterator itr = result.begin(); itr != result.end(); itr++)
			{
				std::vector<RowInfo> info = itr->second;
				if (Slot* TempSlot = GetSlotByPageTag(info.at(1).GetInt(), info.at(2).GetInt()))
				{
					Item* pItem = Item::CreateItem(info.at(0).GetInt());
					if (!pItem)
						continue;
					pItem->SetCount(info.at(3).GetInt());
					TempSlot->SetItem(pItem);
				}
			}
		}
	}
	
}

Slot* PlayerBag::GetSlotByPageTag(const uint8& Page, const uint8& SlotTag)
{
	Slot* TempSlot = nullptr;

	if (Page < m_PlayerBagPageSprites.size() && SlotTag < SingleSlotTagEnded)
	{
		TempSlot = (Slot*)m_PlayerBagPageSprites.at(Page)->getChildByTag(SlotTag);
	}
	return TempSlot;
}

Slot* PlayerBag::GetSlotByTouch(Touch* touches)
{
	if (!m_PlayerBagPageSprites.at(m_CurrentPage)->isVisible())
		return nullptr;
	Sprite* TempPage = m_PlayerBagPageSprites.at(m_CurrentPage);
	for (int i = 0; i != SingleSlotTagEnded; i++)
	{
		if (Slot* TempSlot = (Slot*)TempPage->getChildByTag(i))
		{
			if (!TempSlot->IsContectPoint(touches->getLocation()))
				continue;
			return TempSlot;
		}
	}
	return nullptr;
}

bool PlayerBag::onTouchBagBegan(Touch* touches)
{
	TouchedSprite = nullptr;
	m_BagTouchType = Bag_Type_None;
	for (int i = Page_One; i != End_Of_Player_Bag_Page; i++)
	{
		if (Sprite* TempPagTag = (Sprite*)getChildByTag(i))
		{
			if (TempPagTag->IsContectPoint(touches->getLocation()))
			{
				m_BagTouchType = Bag_Type_SwapPage;
				TouchedSprite = TempPagTag;
				return true;
			}
		}
	}

	for (int i = 0; i != m_PlayerBagPageSprites.size(); i++)
	{
		if (!m_PlayerBagPageSprites.at(i)->isVisible())
			continue;
		Sprite* TempPage = m_PlayerBagPageSprites.at(i);
		for (int j = SingleSlotTagStart; j != SingleSlotTagEnded; j++)
		{
			if (Slot* TempSlot = (Slot*)TempPage->getChildByTag(j))
			{
				if (TempSlot->IsContectPoint(touches->getLocation()))
				{
					m_BagTouchType = Bag_Type_SeleItem;
					m_Start_Move_Position = touches->getLocation();
					TouchedSprite = TempSlot;
					return true;
				}
			}
		}
	}
	m_Start_Move_Position = touches->getLocation();
	TouchedSprite = this;
	return true;
}

void PlayerBag::onTouchBagMoved(Touch* touches)
{
	if (m_BagTouchType == Bag_Type_SeleItem)
	{
		Sprite* DisplaySprite = ((Slot*)TouchedSprite)->GetDisPlaySprite();
		if (DisplaySprite)
		{
			float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
			float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
			DisplaySprite->setPosition(DisplaySprite->getPositionX() + X_Modify, DisplaySprite->getPositionY() + Y_Modify);
			m_Start_Move_Position = touches->getLocation();
			return;
		}
	}
	if (TouchedSprite == this)
	{
		float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
		float Y_Modify = touches->getLocation().y -  m_Start_Move_Position.y;
		setPosition(getPositionX() + X_Modify, getPositionY() + Y_Modify);
		m_Start_Move_Position = touches->getLocation();
	}
}

void PlayerBag::onTouchBagEnded(Touch* touches)
{
	if (!TouchedSprite)
		return;
	switch (m_BagTouchType)
	{
	case Bag_Type_SwapPage:
		if (!TouchedSprite->IsContectPoint(touches->getLocation()))
			return;
		SwapPage((PlayerBagPage)TouchedSprite->getTag(), m_CurrentPage);
		break;
	case Bag_Type_SeleItem:
		Slot* OldSlot = ((Slot*)TouchedSprite);
		Slot* NewSlot = GetSlotByTouch(touches);
		if (!NewSlot)
			NewSlot = sPlayerEquip->GetSlotByTouch(touches);
		OldSlot->SwapItem(NewSlot);
		break;
	}
}

bool PlayerBag::SwapItem(Slot* slot_one, Slot* slot_two)
{
	return true;
}

void PlayerBag::InitPage()
{
	m_PlayerBagPageSprites.clear();
	for (int i = Page_One; i != End_Of_Player_Bag_Page; i++)
	{
		//首先创建页面
		Sprite* _TempPageSprite = Sprite::create(PlayerBagPageImage);
		_TempPageSprite->setPosition(getContentSize().width * 0.53f, getContentSize().height * 0.1f + _TempPageSprite->getBoundingBox().size.height / 2);
		addChild(_TempPageSprite);
		InitEmptySlots(_TempPageSprite);
		m_PlayerBagPageSprites.push_back(_TempPageSprite);
		_TempPageSprite->setVisible(false);

		Sprite* BagPageSelector = Sprite::create(PlayerBagPageSelectorImage);
		BagPageSelector->setPositionX(getContentSize().width * 0.15f + BagPageSelector->getBoundingBox().size.width + (i * BagPageSelector->getBoundingBox().size.width * 1.6f));
		BagPageSelector->setPositionY(getContentSize().height * 0.915f);
		BagPageSelector->setTag(i);
		addChild(BagPageSelector);
		BagPageSelector->setOpacity(120.0f);
	}
}

void PlayerBag::SwapPage(PlayerBagPage enable, PlayerBagPage disable)
{
	if (disable != Page_None)
	{
		if (Sprite* Temp = (Sprite*)getChildByTag(disable))
		{
			Temp->setOpacity(120.0f);
			m_PlayerBagPageSprites.at(disable)->setVisible(false);
		}
	}
	if (Sprite* Show = (Sprite*)getChildByTag(enable))
	{
		Show->setOpacity(255.0f);
		m_PlayerBagPageSprites.at(enable)->setVisible(true);
	}

	m_CurrentPage = enable;
}

Slot* PlayerBag::GetSlot(uint8 _Page, uint8 _Slot)
{
	if (_Page > Page_Nine)
		return nullptr;

	Sprite* Page = nullptr;

	Page = m_PlayerBagPageSprites.at(_Page);

	if (Slot* pSlot = (Slot*)Page->getChildByTag(_Slot))
		return pSlot;

	return nullptr;
}

void PlayerBag::InitEmptySlots(Sprite* SinglePageSprite)
{
	int tag = 1;
	for (int i = 0; i != SlotRowCount; i++)
	for (int k = 0; k != SlotFieldCount; k++)
	{
		Slot* SingleSlot = new Slot();
		SingleSlot->setPositionX(getBoundingBox().size.width * 0.005f + SingleSlot->getBoundingBox().size.width / 2 + (i * SingleSlot->getBoundingBox().size.width * 1.05f));
		SingleSlot->setPositionY(getBoundingBox().size.height * 0.01f + SingleSlot->getBoundingBox().size.height / 2 + (k * SingleSlot->getBoundingBox().size.height * 1.05f));
		SingleSlot->setTag(tag);
		SinglePageSprite->addChild(SingleSlot);
		tag++;
	}
}

PlayerBag* PlayerBag::GetInstance()
{
	if (!_PlayerBag)
		_PlayerBag = new PlayerBag();

	return _PlayerBag;
}

								/////////////////
								//PlayerUILayer//
								/////////////////
PlayerUILayer::PlayerUILayer()
{
	visiablesize = Director::getInstance()->getVisibleSize();
	m_touchtype = PlayerUITouch_None;
	CanTouchButton = true;
	m_TopMenuIsVisable = true;
}

PlayerUILayer::~PlayerUILayer()
{
	removeAllChildrenWithCleanup(true);
	_PlayerUILayer = nullptr;
	_eventDispatcher->removeEventListener(RokerListener);
	RokerListener = nullptr;
}

PlayerUILayer* PlayerUILayer::GetInstance()
{
	if (!_PlayerUILayer)
		_PlayerUILayer = PlayerUILayer::create();

	return _PlayerUILayer;
}

bool PlayerUILayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		InitSpellDefaultFrame();
		InitExpBar();
		InitUI();
		InitButtomMenu();
		InitButtomSpellBar();

		sRocker->setPosition(sRocker->getBoundingBox().size.width / 2, sRocker->getBoundingBox().size.height / 2);
		addChild(sRocker);

		sPlayerBag->setPosition(visiablesize.x * 0.75, visiablesize.y / 2);
		addChild(sPlayerBag);

		sPlayerEquip->setPosition(visiablesize.x * 0.25, visiablesize.y * 0.75);
		addChild(sPlayerEquip);

		sPlayerValueWindow->setPosition(sPlayerEquip->getPositionX() * 1.032f, sPlayerEquip->getBoundingBox().origin.y * 1.04f - sPlayerValueWindow->getBoundingBox().size.height / 2);
		addChild(sPlayerValueWindow);

		sPlayerSpellBook->setPosition(visiablesize.x / 2, visiablesize.y / 2);
		addChild(sPlayerSpellBook);

		sSettingMenu->setPosition(visiablesize.x / 2, visiablesize.y / 2);
		addChild(sSettingMenu);

		sQuestBook->setPosition(visiablesize.x / 2, visiablesize.y / 2);
		addChild(sQuestBook);

		sDeadTalkClass->setPosition(visiablesize.x / 2, visiablesize.y * 0.75f);
		addChild(sDeadTalkClass);

		RokerListener = EventListenerTouchAllAtOnce::create();
		RokerListener->onTouchesBegan = CC_CALLBACK_2(PlayerUILayer::onTouchBegan, this);
		RokerListener->onTouchesMoved = CC_CALLBACK_2(PlayerUILayer::onTouchMoved, this);
		RokerListener->onTouchesEnded = CC_CALLBACK_2(PlayerUILayer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(RokerListener, this);

		scheduleUpdate();
		bRef = true;
	} while (0);

	return bRef;
}

SpellSlot* PlayerUILayer::CheckTouchSpellButton(const Vec2& Loc)
{
	return GetContactButtonSlot(Loc);
}

void PlayerUILayer::InitExpBar()
{
	m_Player_Exp_Bar_Frame = Sprite::create(PlayerUIExpBarFrame);
	m_Player_Exp_Bar_Frame->SetRealPosition(visiablesize.x / 2, m_Player_Exp_Bar_Frame->getBoundingBox().size.height / 2);
	addChild(m_Player_Exp_Bar_Frame);

	m_Player_Exp_Bar_Scroll = ProgressTimer::create(Sprite::create(PlayerUIExpBarScroll));
	m_Player_Exp_Bar_Scroll->setPercentage(0);
	m_Player_Exp_Bar_Scroll->setBarChangeRate(Vec2(1, 0));
	m_Player_Exp_Bar_Scroll->setMidpoint(Vec2(0, 0));
	m_Player_Exp_Bar_Scroll->setType(ProgressTimer::Type::BAR);
	m_Player_Exp_Bar_Scroll->setPosition(m_Player_Exp_Bar_Frame->getContentSize().width / 2, m_Player_Exp_Bar_Frame->getContentSize().height * 0.4f);
	m_Player_Exp_Bar_Frame->addChild(m_Player_Exp_Bar_Scroll);
}

void PlayerUILayer::InitButtomSpellBar()
{
	m_ButtonSpellBar = Sprite::create(PlayerUIButtonSpellBarImage);
	m_ButtonSpellBar->SetRealPosition(visiablesize.x / 2, m_ButtonSpellBar->getBoundingBox().size.height * 0.9f);
	addChild(m_ButtonSpellBar);

	for (int i = 0; i != MaxButtomSpellBarSlot; i++)
	{
		SpellSlot* TempSlot = new SpellSlot();
		TempSlot->setPosition(TempSlot->getBoundingBox().size.width * 0.77f + (i * TempSlot->getBoundingBox().size.width * 1.1f), m_ButtonSpellBar->getContentSize().height / 2);
		m_ButtonSpellBar->addChild(TempSlot);
	}

	m_ButtonSpellItem[0] = new SpellSlot("Player_UI_Button_Big.png");
	m_ButtonSpellItem[0]->setPosition(visiablesize.x - m_ButtonSpellItem[0]->getBoundingBox().size.width * 0.5f, m_ButtonSpellItem[0]->getBoundingBox().size.height * 0.7f);
	addChild(m_ButtonSpellItem[0]);

	float PosX, PosY;
	for (int i = 1, k = 0; i != MaxButtomSpellBarSlot; i++, k++)
	{
		m_ButtonSpellItem[i] = new SpellSlot();
		PosX = m_ButtonSpellItem[0]->getBoundingBox().origin.x - m_ButtonSpellItem[i]->getBoundingBox().size.width / 2 + m_ButtonSpellItem[0]->getBoundingBox().size.width - (k > 3 ? (3 * m_ButtonSpellItem[i]->getBoundingBox().size.width) : k * m_ButtonSpellItem[i]->getBoundingBox().size.width);
		PosY = m_ButtonSpellItem[0]->getBoundingBox().origin.y + m_ButtonSpellItem[0]->getBoundingBox().size.height + m_ButtonSpellItem[i]->getBoundingBox().size.height / 2 - (k <= 3 ? 0 : ((k - 3) * m_ButtonSpellItem[i]->getBoundingBox().size.height));
		m_ButtonSpellItem[i]->setPosition(PosX, PosY);
		addChild(m_ButtonSpellItem[i]);
	}
}

SpellSlot* PlayerUILayer::GetContactButtonSlot(const Vec2& Loc)
{
	for (int i = 0; i != MaxButtomSpellBarSlot; i++)
	{
		for (Vector<Node*>::iterator itr = m_ButtonSpellBar->getChildren().begin(); itr != m_ButtonSpellBar->getChildren().end(); itr++)
		{
			if ((*itr)->IsContectPoint(Loc))
				return (SpellSlot*)(*itr);
		}
	}
	for (int i = 0; i < MaxButtomSpellBarSlot; i++)
	{
		if (m_ButtonSpellItem[i]->IsContectPoint(Loc))
			return (SpellSlot*)m_ButtonSpellItem[i];
	}
	return nullptr;
}

void PlayerUILayer::InitSpellDefaultFrame()
{
	DefaultFrame = Sprite::create(PlayerUISpellDefaultFrame);
	DefaultFrame->setPosition(sPlayerSpellBook->getBoundingBox().origin.x - DefaultFrame->getBoundingBox().size.width / 2, sPlayerSpellBook->getBoundingBox().origin.y +  sPlayerSpellBook->getBoundingBox().size.height - DefaultFrame->getBoundingBox().size.height / 2);
	DefaultFrame->setVisible(false);
	SpellText = LabelTTF::create("...", "Arial", 25, Size::ZERO, TextHAlignment::LEFT);
	SpellText->setPosition(DefaultFrame->getContentSize().width * 0.05f, DefaultFrame->getContentSize().height * 0.9f);
	SpellText->setAnchorPoint(Vec2(0, 1.0f));
	DefaultFrame->addChild(SpellText);
	addChild(DefaultFrame);
}

void PlayerUILayer::ReSetSpellFramePosition()
{
	DefaultFrame->setPosition(sPlayerSpellBook->getBoundingBox().origin.x - DefaultFrame->getBoundingBox().size.width / 2, sPlayerSpellBook->getBoundingBox().origin.y + sPlayerSpellBook->getBoundingBox().size.height - DefaultFrame->getBoundingBox().size.height / 2);
}

void PlayerUILayer::SwapSpellFrameVisable()
{
	sPlayerSpellBook->isVisible() ? DefaultFrame->setVisible(true) : DefaultFrame->setVisible(false);
}

void PlayerUILayer::ReSetSpellFrameText(uint32 SpellID)
{
	SpellInfo info = sSpellMgr->GetSpellInfo(SpellID);
	char msg[255];
	snprintf(msg, 255, "%s\n冷却时间: %u秒\n技能伤害: %d/次\n持续时间: %d秒\n技能范围: %d码\n施法时间: %d秒", info.SpellName.c_str(), (uint32)info.SpellColdDownTime, (uint32)info.SpellValue, (uint8)info.AruaTotalDuration, (uint32)info.SpellCastRange, (uint32)info.SpellCastTime);
	SpellText->setString(msg);
}

void PlayerUILayer::SwapButtomMenuType()
{
	if (!CanTouchButton)
		return;
	CanTouchButton = false;
	Sequence* sq = nullptr;
	if (m_TopMenuIsVisable)
	{
		m_TopMenuIsVisable = false;
		m_Player_UI_TopButton_Swap_Button->runAction(MoveTo::create(1.0f, Vec2(m_Player_UI_TopButton_Swap_Button->getPositionX(), visiablesize.y - m_Player_UI_TopButton_Swap_Button->getBoundingBox().size.height / 2)));
		m_Player_Info_UI->runAction(MoveTo::create(1.0f, Vec2(m_Player_Info_UI->getPositionX(), visiablesize.y - m_Player_Info_UI->getBoundingBox().size.height * 0.45f)));
		sq = Sequence::create(MoveTo::create(1.0f, Vec2(m_ButtomMenu->getPositionX(), m_ButtomMenu->getPositionY() + m_ButtomMenu->getBoundingBox().size.height)), CallFunc::create(CC_CALLBACK_0(PlayerUILayer::ButtonMenuCallBack, this)), NULL);
	}
	else
	{
		m_TopMenuIsVisable = true;
		m_Player_UI_TopButton_Swap_Button->runAction(MoveTo::create(1.0f, Vec2(m_Player_UI_TopButton_Swap_Button->getPositionX(), visiablesize.y + m_Player_UI_TopButton_Swap_Button->getBoundingBox().size.height / 2)));
		m_Player_Info_UI->runAction(MoveTo::create(1.0f, Vec2(m_Player_Info_UI->getPositionX(), visiablesize.y * 0.9f - m_Player_Info_UI->getBoundingBox().size.height / 2)));
		sq = Sequence::create(MoveTo::create(1.0f, Vec2(m_ButtomMenu->getPositionX(), visiablesize.y - m_ButtomMenu->getBoundingBox().size.height / 2)), CallFunc::create(CC_CALLBACK_0(PlayerUILayer::ButtonMenuCallBack, this)), NULL);
	}
	m_ButtomMenu->runAction(sq);
}

void PlayerUILayer::ButtonMenuCallBack()
{
	CanTouchButton = true;
}

void PlayerUILayer::InitButtomMenu()
{
	m_ButtomMenu = Sprite::create(PlayerUIButtomMenuImage);
	m_ButtomMenu->SetRealPosition(visiablesize.x / 2, visiablesize.y - m_ButtomMenu->getBoundingBox().size.height / 2);
	addChild(m_ButtomMenu);

	//character equip bag quest spell setting
	char msg[255];
	float PosX = 0;
	for (int i = Button_Menu_Setting; i != Button_Menu_End; i++)
	{
		snprintf(msg, 255, "%s%d.png", PlayerUIButtonMenuListImage, i);
		Sprite* TempSprite = Sprite::create(msg);
		TempSprite->setPosition(m_ButtomMenu->getContentSize().width * 0.882f - TempSprite->getBoundingBox().size.width / 2 - (i * TempSprite->getBoundingBox().size.width * 0.9f), m_ButtomMenu->getContentSize().height * 0.65f);
		m_ButtomMenu->addChild(TempSprite);
		m_Buttom_Menus.push_back(TempSprite);
		PosX = TempSprite->getPositionX();
	}
	Sprite* MapButton = Sprite::create("Player_UI_Buttom_Menu_6.png");
	MapButton->setPosition(PosX - MapButton->getBoundingBox().size.width * 0.52f, m_ButtomMenu->getContentSize().height * 0.55f);
	m_ButtomMenu->addChild(MapButton);
	m_Buttom_Menus.push_back(MapButton);

	Sprite* NameFrame = Sprite::create("Player_UI_Buttom_Menu_7.png");
	Sprite* ValueFrame = Sprite::create("Player_UI_Buttom_Menu_8.png");
	Sprite* HeadFrame = Sprite::create("Player_UI_Buttom_Menu_9.png");

	NameFrame->setPosition(MapButton->getBoundingBox().origin.x - NameFrame->getBoundingBox().size.width * 0.4f, m_ButtomMenu->getContentSize().height * 0.55f + NameFrame->getBoundingBox().size.height / 2);
	m_ButtomMenu->addChild(NameFrame);

	ValueFrame->setPosition(NameFrame->getPositionX(), m_ButtomMenu->getContentSize().height * 0.5f);
	m_ButtomMenu->addChild(ValueFrame);

	HeadFrame->setPosition(ValueFrame->getBoundingBox().origin.x - HeadFrame->getBoundingBox().size.width * 0.35f, m_ButtomMenu->getContentSize().height * 0.62f);
	m_ButtomMenu->addChild(HeadFrame);

	float FirstPoint = ValueFrame->getBoundingBox().size.width / 9;
	float SinglePoint = ValueFrame->getBoundingBox().size.width / 3;
	for (int i = MoneyTTF; i != EndOfTopTTF; i++)
	{
		LabelTTF* TempTTF = LabelTTF::create("1234", "Arial", 20, Size::ZERO, i == NameTTF ? TextHAlignment::CENTER : TextHAlignment::LEFT);
		i == NameTTF ? TempTTF->setAnchorPoint(Vec2(0.5f,0.5f)) : TempTTF->setAnchorPoint(Vec2(0, 0.5f));
		i == NameTTF ? TempTTF->setPosition(NameFrame->getContentSize().width / 2, NameFrame->getContentSize().height) : TempTTF->setPosition(FirstPoint + (i * SinglePoint), ValueFrame->getContentSize().height / 2);
		ValueFrame->addChild(TempTTF);
		TopMenuLabel[(TopButtonLabelTTF)i] = TempTTF;
	}
	ResetUpButtonString();

	m_Player_UI_TopButton_Swap_Button = Sprite::create(PlayerUIButtonMenuSwapButton);
	m_Player_UI_TopButton_Swap_Button->setPosition(visiablesize.x / 2, visiablesize.y + m_Player_UI_TopButton_Swap_Button->getBoundingBox().size.height / 2);
	addChild(m_Player_UI_TopButton_Swap_Button);
}

void PlayerUILayer::ResetUpButtonString()
{
	for (std::map<TopButtonLabelTTF, LabelTTF*>::iterator itr = TopMenuLabel.begin(); itr != TopMenuLabel.end(); itr++)
	{
		LabelTTF* TempTTF = itr->second;
		char msg[255];
		switch (itr->first)
		{
		case PlayerUILayer::MoneyTTF:
			snprintf(msg, 255, "%d", sPlayer->GetMoney());
			break;
		case PlayerUILayer::AttackTTF:
			snprintf(msg, 255, "%d", sPlayer->GetUnitInt32Value(Base_Att));
			break;
		case PlayerUILayer::CashTTF:
			snprintf(msg, 255, "%d", 99999);
			break;
		case PlayerUILayer::NameTTF:
			snprintf(msg, 255, "%s", sPlayer->GetName().c_str());
			break;
		}
		TempTTF->setString(msg);
	}
}

void PlayerUILayer::ResetHeadLevel()
{
	for (int i = 0; i < m_Player_Info_UI_Level_Sprite.size(); i++)
		m_Player_Info_UI_Level_Sprite.at(i)->removeFromParentAndCleanup(true);
	m_Player_Info_UI_Level_Sprite.clear();


	m_Player_Info_UI_Level_Sprite = sGame->GetNumberSpriteByInt(sPlayer->GetLevel());
	float FirstPosX = 0;
	float SingleWidth = m_Player_Info_UI_Level_Sprite.at(0)->getBoundingBox().size.width;
	float TotalWidth = SingleWidth + ((m_Player_Info_UI_Level_Sprite.size() - 1) * (SingleWidth / 2));

	for (int i = 0; i != m_Player_Info_UI_Level_Sprite.size(); i++)
	{
		float posx = m_Player_Info_UI_Level_Frame->getContentSize().width / 2 - (TotalWidth / 2) + (i * SingleWidth / 2);
		m_Player_Info_UI_Level_Sprite.at(i)->setAnchorPoint(Vec2(0, 0.5f));
		m_Player_Info_UI_Level_Sprite.at(i)->setPosition(posx, m_Player_Info_UI_Level_Frame->getContentSize().height / 2);
		m_Player_Info_UI_Level_Frame->addChild(m_Player_Info_UI_Level_Sprite.at(i));
	}
}

void PlayerUILayer::ResetAllUIValuesNumber()
{
	ResetHeadLevel();
	sPlayerValueWindow->ResetValueDefault();
	ResetUpButtonString();
}

void PlayerUILayer::InitUI()
{
	//Character Head
	m_Player_Info_UI = Sprite::create(PlayerUICharacterHeadImage);
	m_Player_Info_UI->SetRealPosition(m_Player_Info_UI->getBoundingBox().size.width / 2, visiablesize.y * 0.9f - m_Player_Info_UI->getBoundingBox().size.height / 2);

	//Character Heath
	m_Player_Info_UI_Hp = ProgressTimer::create(Sprite::create(PlayerUICharacterHPProccessImage));
	m_Player_Info_UI_Hp->setPosition(m_Player_Info_UI->getContentSize().width * 0.36f, m_Player_Info_UI->getContentSize().height * 0.64f);
	m_Player_Info_UI_Hp->setAnchorPoint(Vec2(0, 1.0f));
	m_Player_Info_UI_Hp->setPercentage(0);
	m_Player_Info_UI_Hp->setBarChangeRate(Vec2(1, 0));
	m_Player_Info_UI_Hp->setMidpoint(Vec2(0, 0));
	m_Player_Info_UI_Hp->setType(ProgressTimer::Type::BAR);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Hp);

	//Character Mana
	m_Player_Info_UI_Mp = ProgressTimer::create(Sprite::create(PlayerUICharacterMPPorccessImage));
	m_Player_Info_UI_Mp->setPosition(m_Player_Info_UI->getContentSize().width * 0.33f, m_Player_Info_UI->getContentSize().height * 0.658f);
	m_Player_Info_UI_Mp->setAnchorPoint(Vec2(0, 0));
	m_Player_Info_UI_Mp->setPercentage(0);
	m_Player_Info_UI_Mp->setBarChangeRate(Vec2(1, 0));
	m_Player_Info_UI_Mp->setMidpoint(Vec2(0, 0));
	m_Player_Info_UI_Mp->setType(ProgressTimer::Type::BAR);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Mp);


	//Character Level
	m_Player_Info_UI_Level_Frame = Sprite::create(PlayerUICharacterLevelFrame);
	m_Player_Info_UI_Level_Frame->setPosition(m_Player_Info_UI->getContentSize().width * 0.36f, m_Player_Info_UI->getContentSize().height * 0.43f);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Level_Frame);

	//Character Level Set Position
	ResetHeadLevel();

	addChild(m_Player_Info_UI);

	m_Player_Info_Casting_Bar_Icon = Sprite::create(PlayerUICastingBarImage);
	m_Player_Info_Casting_Bar_Icon->SetRealPosition(visiablesize.x * 0.45f, visiablesize.y * 0.13f);

	m_Player_Info_Casting_Bar_Frame = Sprite::create(PlayerUICastingBarFrame);
	m_Player_Info_Casting_Bar_Frame->SetRealPosition(m_Player_Info_Casting_Bar_Icon->getPositionX() - m_Player_Info_Casting_Bar_Icon->getBoundingBox().size.width * 0.2f, m_Player_Info_Casting_Bar_Icon->getPositionY() - m_Player_Info_Casting_Bar_Icon->getBoundingBox().size.height * 0.05f);
	m_Player_Info_Casting_Bar_Frame->setAnchorPoint(Vec2(0, 0.5f));
	addChild(m_Player_Info_Casting_Bar_Frame);

	m_Player_Info_Casting_Bar = ProgressTimer::create(Sprite::create(PlayerUICastingBarPorccessImage));
	m_Player_Info_Casting_Bar->setAnchorPoint(Vec2(0, 0.5f));
	m_Player_Info_Casting_Bar->setPercentage(0);
	m_Player_Info_Casting_Bar->setBarChangeRate(Vec2(1, 0));
	m_Player_Info_Casting_Bar->setMidpoint(Vec2(0, 0));
	m_Player_Info_Casting_Bar->setType(ProgressTimer::Type::BAR);
	m_Player_Info_Casting_Bar->setPosition(m_Player_Info_Casting_Bar_Frame->getBoundingBox().size.width * 0.075f, m_Player_Info_Casting_Bar_Frame->getContentSize().height / 2);
	m_Player_Info_Casting_Bar_Frame->addChild(m_Player_Info_Casting_Bar);

	addChild(m_Player_Info_Casting_Bar_Icon);
}

bool PlayerUILayer::IsSingleTouch(const std::vector<Touch*>& touches, PlayerUITouchType _type)
{
	for (int i = 0; i != touches.size(); i++)
	{
		if (touches.at(i)->GetTouchType() == _type)
			return false;
	}
	return true;
}

void PlayerUILayer::onTouchBegan(const std::vector<Touch*>& touchesVector, Event *event)
{
	TouchedSpellSlot = nullptr;
	m_touchtype = PlayerUITouch_None;
	if (!sPlayer)
		return;

	for (int i = 0; i != touchesVector.size(); i++)
	{
		Touch* touches = touchesVector.at(i);
		if (TouchedSpellSlot = CheckTouchSpellButton(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector,PlayerUITouch_Button_SpellSlot))
				touches->SetTouchType(PlayerUITouch_Button_SpellSlot);
			return;
		}
		for (int i = 0; i != m_Buttom_Menus.size(); i++)
		{
			if (Sprite* TempButton = m_Buttom_Menus.at(i))
			{
				if (TempButton->getOpacity() < 245.0f)
					break;
				if (!IsSingleTouch(touchesVector, PlayerUITouch_UperButton))
					break;
				if (TempButton->IsContectPoint(touches->getLocation()))
				{
					switch (i)
					{
					case Button_Menu_Setting:
						sSettingMenu->OnClickSettingButton();
						break;
					case Button_Menu_Spell:
						sPlayerSpellBook->SwapVisiable();
						break;
					case Button_Menu_Quest:
						sQuestBook->SwapVisable();
						break;
					case Button_Menu_Bag:
						sPlayerBag->SwapVisiable();
						break;
					case Button_Menu_Equip:
						sPlayerEquip->SwapVisiable();
						break;
					case Button_Menu_Character:
						sPlayerValueWindow->SwapVisable();
						break;
					}
					touches->SetTouchType(PlayerUITouch_UperButton);
					return;
				}
			}
		}

		
		if (m_Player_UI_TopButton_Swap_Button->getPositionY() < visiablesize.y && m_Player_UI_TopButton_Swap_Button->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_SwapTopButton))
			{
				touches->SetTouchType(PlayerUITouch_SwapTopButton);
			}
			return;
		}

		if (sDeadTalkClass->isVisible() && sDeadTalkClass->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_DeathTalkClass))
			{
				sDeadTalkClass->OnTouchBegin(touches);
				touches->SetTouchType(PlayerUITouch_DeathTalkClass);
			}
			return;
		}

		if (sSettingMenu->isVisible() && sSettingMenu->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_SettingMenu))
			{
				sSettingMenu->OnTouchBegin(touches);
				touches->SetTouchType(PlayerUITouch_SettingMenu);
			}
			return;
		}
		if (sPlayerEquip->isVisible() && sPlayerEquip->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_Equip_Window))
			{
				sPlayerEquip->onTouchBagBegan(touches);
				touches->SetTouchType(PlayerUITouch_Equip_Window);
			}
			return;
		}
		if (sPlayerBag->isVisible() && sPlayerBag->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_Bag))
			{
				sPlayerBag->onTouchBagBegan(touches);
				touches->SetTouchType(PlayerUITouch_Bag);
			}
			return;
		}

		if (sPlayerSpellBook->isVisible() && sPlayerSpellBook->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_SpellBook))
			{
				sPlayerSpellBook->onTouchBagBegan(touches);
				touches->SetTouchType(PlayerUITouch_SpellBook);
			}
			return;
		}
		if (m_ButtomMenu->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_Buttom_Menu))
				touches->SetTouchType(PlayerUITouch_Buttom_Menu);
			return;
		}
		if (sRocker && sRocker->getBoundingBox().containsPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_Roker))
			{
				touches->SetTouchType(PlayerUITouch_Roker);
				sRocker->TouchBegin(touches);
			}
			return;
		}
		if (sQuestBook->isVisible() && sQuestBook->IsContectPoint(touches->getLocation()))
		{
			if (IsSingleTouch(touchesVector, PlayerUITouch_QuestBook))
			{
				touches->SetTouchType(PlayerUITouch_QuestBook);
				sQuestBook->OnTouchBegin(touches);
			}
			return;
		}
	}
}

void PlayerUILayer::onTouchMoved(const std::vector<Touch*>& touchesVector, Event *event)
{
	if (!sPlayer)
		return;
	for (int i = 0; i != touchesVector.size(); i++)
	{
		if (Touch* touches = touchesVector.at(i))
		{
			switch (touches->GetTouchType())
			{
			case PlayerUITouch_Bag:
				sPlayerBag->onTouchBagMoved(touches);
				return;
			case PlayerUITouch_Roker:
				sRocker->TouchMoved(touches);
				return;
			case PlayerUITouch_SpellBook:
				sPlayerSpellBook->onTouchBagMoved(touches);
				return;
			case PlayerUITouch_Equip_Window:
				sPlayerEquip->onTouchBagMoved(touches);
				return;
			}
		}
	}
}

void PlayerUILayer::onTouchEnded(const std::vector<Touch*>& touchesVector, Event *event)
{
	if (!sPlayer)
		return;
	for (int i = 0; i != touchesVector.size(); i++)
	{
		if (Touch* touches = touchesVector.at(i))
		{
			switch (touches->GetTouchType())
			{
				case PlayerUITouch_Bag:
					sPlayerBag->onTouchBagEnded(touches);
					return;
				case PlayerUITouch_SpellBook:
					sPlayerSpellBook->onTouchBagEnded(touches);
					return;
				case PlayerUITouch_Buttom_Menu:
					SwapButtomMenuType();
					return;
				case PlayerUITouch_Roker:
				{
					sRocker->TouchEnded(touches);
					return;
				}
				case PlayerUITouch_Button_SpellSlot:
				{
					if (TouchedSpellSlot->GetSpellId())
						sPlayer->CastSpell(TouchedSpellSlot->GetSpellId(), sPlayer->GetPlayerTarget());
					break;
				}
				case PlayerUITouch_Equip_Window:
				{
					sPlayerEquip->onTouchBagEnded(touches);
					break;
				}
				case PlayerUITouch_SettingMenu:
					sSettingMenu->OnTouchEnded(touches);
					break;
				case PlayerUITouch_SwapTopButton:
					if (m_Player_UI_TopButton_Swap_Button->IsContectPoint(touches->getLocation()))
						SwapButtomMenuType();
					break;
				case PlayerUITouch_QuestBook:
					sQuestBook->OnTouchEnded(touches);
					break;
				case PlayerUITouch_DeathTalkClass:
					sDeadTalkClass->OnTouchEnded(touches);
					break;
			}
		}
	}
}

void PlayerUILayer::SwapCastingBarVisable()
{
	if (m_Player_Info_Casting_Bar->isVisible())
	{
		m_Player_Info_Casting_Bar_Frame->setVisible(false);
		m_Player_Info_Casting_Bar_Icon->setVisible(false);
		m_Player_Info_Casting_Bar->setVisible(false);
	}
	else
	{
		m_Player_Info_Casting_Bar_Frame->setVisible(true);
		m_Player_Info_Casting_Bar_Icon->setVisible(true);
		m_Player_Info_Casting_Bar->setVisible(true);
	}
}

void PlayerUILayer::AutoUpdateExpBar()
{
	float CurrentExp = sPlayer->GetExp();
	float NextLevelExp = sPlayer->GetNextLevelRequireExp();
	float Proccess = (CurrentExp / NextLevelExp) * 100.0f;
	float ProccessNow = m_Player_Exp_Bar_Scroll->getPercentage();
	if (abs(Proccess - ProccessNow) > 1)
		Proccess > ProccessNow ? m_Player_Exp_Bar_Scroll->setPercentage(++ProccessNow) : m_Player_Exp_Bar_Scroll->setPercentage(--ProccessNow);
}

void PlayerUILayer::AutoUpdateCastingBar()
{
	if (Spell* pSpell = sPlayer->GetCastingSpell())
	{
		if (!m_Player_Info_Casting_Bar->isVisible())
			SwapCastingBarVisable();
		float TotalTime = pSpell->GetSpellTotalCastTime();
		float TimeLeft = pSpell->GetSpellCurrentTimeLeft();
		float Proccess = (TotalTime - TimeLeft) / TotalTime;
		m_Player_Info_Casting_Bar->setPercentage(Proccess * 100.0f);
	}
	else if (m_Player_Info_Casting_Bar->isVisible())
		SwapCastingBarVisable();
}

void PlayerUILayer::AutoUpdateHeadBar()
{
	float HpNow = m_Player_Info_UI_Hp->getPercentage();
	float HpCurrent = sPlayer->GetUnitInt32Value(Curr_HP);
	float HpMax = sPlayer->GetUnitInt32Value(Max_HP) + sPlayer->GetEquipItemTotalValusForKey(Max_HP);
	float ProccessHP = (HpCurrent / HpMax) * 100.0f;
	if (abs(ProccessHP - HpNow) > 1)
		ProccessHP > HpNow ? m_Player_Info_UI_Hp->setPercentage(++HpNow) : m_Player_Info_UI_Hp->setPercentage(--HpNow);

	float ManaNow = m_Player_Info_UI_Mp->getPercentage();
	float ManaCurrent = sPlayer->GetUnitInt32Value(Curr_Mana);
	float ManaMax = sPlayer->GetUnitInt32Value(Max_Mana) + sPlayer->GetEquipItemTotalValusForKey(Max_Mana);
	float ProccessMana = (ManaCurrent / ManaNow) * 100.0f;
	if (abs(ProccessMana - ManaNow) > 1)
		ProccessMana > ManaNow ? m_Player_Info_UI_Mp->setPercentage(++ManaNow) : m_Player_Info_UI_Mp->setPercentage(--ManaNow);
}

void PlayerUILayer::update(float diff)
{
	AutoUpdateCastingBar();
	AutoUpdateExpBar();
	AutoUpdateHeadBar();
}