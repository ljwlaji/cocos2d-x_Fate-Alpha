﻿#include "PlayerUILayer.h"
#include "Player.h"
#include "Item.h"
#include "HelloWorldScene.h"
#include "SpellBook.h"
#include "PlayerEquipWindow.h"
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
	if (m_DisPlaySprite)
		m_DisPlaySprite->removeFromParentAndCleanup(true);
}

void Slot::SetItem(Item* pItem)	
{
	if (!pItem)
	{
		m_Item = nullptr;
		if (m_DisPlaySprite)
			m_DisPlaySprite->removeFromParentAndCleanup(true);
		return;
	}
	m_Item = pItem; 
	if (!m_DisPlaySprite)
	{
		m_DisPlaySprite = Sprite::create(pItem->GetIconUrl().c_str());
		if (getBoundingBox().size.width > 39.0f)
			m_DisPlaySprite->setScale(getBoundingBox().size.width / 39.0f);
		m_DisPlaySprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
		addChild(m_DisPlaySprite);
	}
	else
	{
		setTexture(pItem->GetIconUrl().c_str());
	}
	return;
}

void Slot::SwapItem(Slot* Instead)
{
	if (!GetItem() || !Instead)
		return;
	Item* NewSlotItem = Instead->GetItem();
	Item* OldSlotItem = GetItem();
	if (NewSlotItem)
		SetItem(NewSlotItem);
	Instead->SetItem(OldSlotItem);
}

PlayerBag::PlayerBag()
{
	//_PlayerBag = this;
	initWithFile(PlayerBagImage);
	autorelease();
	InitPage();
	SwapPage(Page_One);
	TouchedSprite = nullptr;
	setVisible(false);
}

PlayerBag::~PlayerBag()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
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

	for (int i = 0; i < m_PlayerBagPageSprites.size(); i++)
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
	if (!TouchedSprite->IsContectPoint(touches->getLocation()))
		return;

	switch (m_BagTouchType)
	{
	case Bag_Type_SwapPage:
		SwapPage((PlayerBagPage)TouchedSprite->getTag(), m_CurrentPage);
		break;
	case Bag_Type_SeleItem:
		if (Slot* TempSlot = (Slot*)TouchedSprite)
		{
			if (Item* pItem = TempSlot->GetItem())
			{
				//Do Sth;
			}
		}
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
	_PlayerUILayer = this;
	visiablesize = Director::getInstance()->getVisibleSize();
	m_VirtualRokerLayer = nullptr;
	m_VirtualRoker_BackGround = nullptr;
	m_VirtualRoker_Roker = nullptr;
	RockerLastPostion = Vec2(0, 0);
	m_touchtype = PlayerUITouch_None;
	CanTouchButton = true;
}

PlayerUILayer::~PlayerUILayer()
{
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


//#ifndef WIN32
		CreateVirtualRoker();
//#endif

		InitUI();
		InitButtomMenu();
		InitButtomSpellBar();
		sPlayerBag->setPosition(visiablesize.x * 0.75, visiablesize.y / 2);
		addChild(sPlayerBag);

		sPlayerEquip->setPosition(visiablesize.x * 0.25, visiablesize.y * 0.75);
		addChild(sPlayerEquip);

		sPlayerValueWindow->setPosition(sPlayerEquip->getPositionX() * 1.032f, sPlayerEquip->getBoundingBox().origin.y * 1.04f - sPlayerValueWindow->getBoundingBox().size.height / 2);
		addChild(sPlayerValueWindow);

		sPlayerSpellBook->setPosition(visiablesize.x / 2, visiablesize.y / 2);
		addChild(sPlayerSpellBook);

		InitSpellDefaultFrame();

		scheduleUpdate();
		bRef = true;
	} while (0);

	return bRef;
}

SpellSlot* PlayerUILayer::CheckTouchSpellButton(const Vec2& Loc)
{
	return GetContactButtonSlot(Loc);
}

void PlayerUILayer::InitButtomSpellBar()
{
	m_ButtonSpellBar = Sprite::create(PlayerUIButtonSpellBarImage);
	m_ButtonSpellBar->SetRealPosition(visiablesize.x / 2, m_ButtonSpellBar->getBoundingBox().size.height * 0.7f);
	addChild(m_ButtonSpellBar);

	for (int i = 0; i != MaxButtomSpellBarSlot; i++)
	{
		SpellSlot* TempSlot = new SpellSlot();
		TempSlot->setPosition(TempSlot->getBoundingBox().size.width * 0.77f + (i * TempSlot->getBoundingBox().size.width * 1.1f), m_ButtonSpellBar->getContentSize().height / 2);
		m_ButtonSpellBar->addChild(TempSlot);
	}

	m_ButtonSpellItem[0] = new SpellSlot("Player_UI_Button_Big.png");
	m_ButtonSpellItem[0]->setPosition(visiablesize.x - m_ButtonSpellItem[0]->getBoundingBox().size.width / 2, m_ButtonSpellItem[0]->getBoundingBox().size.height / 2);
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
	if (m_Buttom_Menus.at(0)._sprite->getPositionX() == m_ButtomMenu->getPositionX())
	{
		for (int i = 0; i < m_Buttom_Menus.size(); i++)
		{
			Sprite* TempSprite = m_Buttom_Menus.at(i)._sprite;
			TempSprite->runAction(MoveTo::create(0.3f, m_Buttom_Menus.at(i).Loc));
			TempSprite->runAction(FadeIn::create(0.3f));
		}
	}
	else
	{
		for (int i = 0; i < m_Buttom_Menus.size(); i++)
		{
			Sprite* TempSprite = m_Buttom_Menus.at(i)._sprite;
			TempSprite->runAction(MoveTo::create(0.3f, m_ButtomMenu->getPosition()));
			TempSprite->runAction(FadeOut::create(0.3f));
		}
	}
	Sequence* sq = Sequence::create(DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(PlayerUILayer::ButtonMenuCallBack, this)), NULL);
	runAction(sq);
}

void PlayerUILayer::ButtonMenuCallBack()
{
	CanTouchButton = true;
}

void PlayerUILayer::InitButtomMenu()
{
	m_ButtomMenu = Sprite::create(PlayerUIButtomMenuImage);
	m_ButtomMenu->SetRealPosition(visiablesize.x - m_ButtomMenu->getBoundingBox().size.width / 2, visiablesize.y - m_ButtomMenu->getBoundingBox().size.height / 2);
	addChild(m_ButtomMenu);

	//character equip bag quest spell setting
	char msg[255];
	for (int i = Button_Menu_Setting; i != Button_Menu_End; i++)
	{
		snprintf(msg, 255, "%s%d.png", PlayerUIButtonMenuListImage, i);
		Sprite* TempSprite = Sprite::create(msg);
		Vec2 TempPos = Vec2(m_ButtomMenu->getBoundingBox().origin.x - TempSprite->getBoundingBox().size.width * 0.55f - (i * TempSprite->getBoundingBox().size.width * 1.05f), m_ButtomMenu->getPositionY());
		ButtonMenuInfo _info;
		_info._sprite = TempSprite;
		_info.Loc = TempPos;
		TempSprite->SetRealPosition(m_ButtomMenu->getPosition());
		addChild(TempSprite);
		TempSprite->setOpacity(0.0f);
		m_Buttom_Menus.push_back(_info);
	}
}

void PlayerUILayer::InitUI()
{
	m_Player_Info_UI = Sprite::create("Player_UI_Info.png");
	m_Player_Info_UI->SetRealPosition(m_Player_Info_UI->getBoundingBox().size.width / 2, visiablesize.y - m_Player_Info_UI->getBoundingBox().size.height / 2);
	addChild(m_Player_Info_UI);


	m_Player_Info_UI_Hp = ProgressTimer::create(Sprite::create("Player_UI_Info_HP.png"));
	m_Player_Info_UI_Hp->setPosition(m_Player_Info_UI->getContentSize().width * 0.70f, m_Player_Info_UI->getContentSize().height * 0.4f);
	m_Player_Info_UI_Hp->setPercentage(0);
	m_Player_Info_UI_Hp->setBarChangeRate(Vec2(1, 0));
	m_Player_Info_UI_Hp->setMidpoint(Vec2(0, 0));
	m_Player_Info_UI_Hp->setType(ProgressTimer::Type::BAR);
	m_Player_Info_UI_Hp_Back = Sprite::create("Player_UI_Info_HP_Back.png");
	m_Player_Info_UI_Hp_Back->setPosition(m_Player_Info_UI_Hp->getPosition());
	m_Player_Info_UI->addChild(m_Player_Info_UI_Hp_Back);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Hp);

	if (!sPlayer)
		return;
	m_Player_Info_UI_Level = sGame->GetNumberSpriteByInt(sPlayer->GetLevel());
	m_Player_Info_UI_Level->setPosition(m_Player_Info_UI->getBoundingBox().size.width * 0.285f, m_Player_Info_UI->getBoundingBox().size.height * 0.4f);
	m_Player_Info_UI_Level->setScale(0.5f);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Level);
}

void PlayerUILayer::CreateVirtualRoker()
{
	m_VirtualRokerLayer = Layer::create();
	addChild(m_VirtualRokerLayer);

	m_VirtualRoker_BackGround = Sprite::create("Virtual_Roker_Back.png");
	m_VirtualRoker_BackGround->SetRealPosition(m_VirtualRoker_BackGround->getBoundingBox().size.width * 0.6f, m_VirtualRoker_BackGround->getBoundingBox().size.height * 0.6f);
	m_VirtualRokerLayer->addChild(m_VirtualRoker_BackGround);
	m_VirtualRoker_BackGround->setOpacity(80.0f);
	m_VirtualRoker_Roker = Sprite::create("Virtual_Roker_Center.png");
	m_VirtualRoker_Roker->setPosition(m_VirtualRoker_BackGround->getContentSize().width / 2, m_VirtualRoker_BackGround->getContentSize().height / 2);
	m_VirtualRoker_Roker->setOpacity(80.0f);
	m_VirtualRoker_BackGround->addChild(m_VirtualRoker_Roker);

	//意见:给确认是摇杆的touch创建唯一标识符
	auto RokerListener = EventListenerTouchOneByOne::create();
	RokerListener->onTouchBegan = CC_CALLBACK_2(PlayerUILayer::onTouchBegan, this);
	RokerListener->onTouchMoved = CC_CALLBACK_2(PlayerUILayer::onTouchMoved, this);
	RokerListener->onTouchEnded = CC_CALLBACK_2(PlayerUILayer::onTouchEnded, this);
	RokerListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(RokerListener, this);
}

bool PlayerUILayer::onTouchBegan(Touch* touches, Event *event)
{
	TouchedSpellSlot = nullptr;
	m_touchtype = PlayerUITouch_None;
	if (!sPlayer)
		return false;

	if (TouchedSpellSlot = CheckTouchSpellButton(touches->getLocation()))
	{
		m_touchtype = PlayerUITouch_Button_SpellSlot;
		return true;
	}
	for (int i = 0; i < m_Buttom_Menus.size(); i++)
	{
		if (Sprite* TempButton = m_Buttom_Menus.at(i)._sprite)
		{
			if (TempButton->getOpacity() < 245.0f)
				break;
			if (TempButton->IsContectPoint(touches->getLocation()))
			{
				switch (i)
				{
				case Button_Menu_Setting:
					break;
				case Button_Menu_Spell:
					sPlayerSpellBook->SwapVisiable();
					break;
				case Button_Menu_Quest:
					break;
				case Button_Menu_Bag:
					sPlayerBag->SwapVisiable();
					break;
				case Button_Menu_Equip:
					sPlayerEquip->SwapVisiable();
					break;
				case Button_Menu_Character:
					break;
				}
				m_touchtype = PlayerUITouch_None;
				return true;
			}
		}
	}

	if (sPlayerEquip->isVisible() && sPlayerEquip->IsContectPoint(touches->getLocation()))
	{
		sPlayerEquip->onTouchBagBegan(touches);
		m_touchtype = PlayerUITouch_Equip_Window;
		return true;
	}
	if (sPlayerBag->isVisible() && sPlayerBag->IsContectPoint(touches->getLocation()))
	{
		sPlayerBag->onTouchBagBegan(touches);
		m_touchtype = PlayerUITouch_Bag;
		return true;
	}

	if (sPlayerSpellBook->isVisible() && sPlayerSpellBook->IsContectPoint(touches->getLocation()))
	{
		sPlayerSpellBook->onTouchBagBegan(touches);
		m_touchtype = PlayerUITouch_SpellBook;
		return true;
	}
	if (m_ButtomMenu->IsContectPoint(touches->getLocation()))
	{
		m_touchtype = PlayerUITouch_Buttom_Menu;
		return true;
	}
	if (m_VirtualRoker_Roker && m_VirtualRoker_Roker->getBoundingBox().containsPoint(touches->getLocation()))
	{
		m_touchtype = PlayerUITouch_Roker;
		RockerLastPostion = touches->getLocation();
		m_VirtualRoker_Roker->setPosition(touches->getLocation());
		return true;
	}
	return false;
}

void PlayerUILayer::onTouchMoved(Touch* touches, Event *event)
{
	if (!sPlayer)
		return;
	switch (m_touchtype)
	{
	case PlayerUITouch_Bag:
		sPlayerBag->onTouchBagMoved(touches);
		return;
	case PlayerUITouch_Roker:
		{
			Vec2 NowLoc = touches->getLocation();
			float X_move = NowLoc.x - RockerLastPostion.x;
			float Y_move = NowLoc.y - RockerLastPostion.y;
			if (ccpDistance(Vec2(m_VirtualRoker_BackGround->getPositionY() - X_move, m_VirtualRoker_BackGround->getPositionY() - Y_move), m_VirtualRoker_Roker->getPosition()) < 140.0f)
				m_VirtualRoker_Roker->setPosition(m_VirtualRoker_Roker->getPositionX() + X_move, m_VirtualRoker_Roker->getPositionY() + Y_move);

			float Orgin = GetVirtualRokerOrgin(m_VirtualRoker_BackGround->getPosition(), m_VirtualRoker_Roker->getPosition());
			RockerLastPostion = touches->getLocation();
			ResetVirtualRokerOrgin(Orgin);
			break;
		}
	case PlayerUITouch_SpellBook:
		sPlayerSpellBook->onTouchBagMoved(touches);
		return;
	case PlayerUITouch_Equip_Window:
		sPlayerEquip->onTouchBagMoved(touches);
		return;
	}
}

void PlayerUILayer::onTouchEnded(Touch* touches, Event *event)
{
	if (!sPlayer)
		return;

	switch (m_touchtype)
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
			sPlayer->ResetMoveKeyForRoker();
			if (m_VirtualRoker_Roker)
				m_VirtualRoker_Roker->setPosition(m_VirtualRoker_BackGround->getContentSize().width / 2, m_VirtualRoker_BackGround->getContentSize().height / 2);

			RockerLastPostion = Vec2(0, 0);
			break;
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
	}
}

void PlayerUILayer::ResetVirtualRokerOrgin(float _orgin)
{
	if (_orgin > 22.5 && _orgin <= 135)
	{
		if (abs(_orgin - 90) < 15)
			sPlayer->DealVirtualRoker(Roker_Left);
		else
			_orgin > 90 ? sPlayer->DealVirtualRoker(Roker_Left_Down) : sPlayer->DealVirtualRoker(Roker_Up_Left);
	}
	else if (_orgin > 135 && _orgin < 225)
	{
		if (abs(_orgin - 180) < 15)
			sPlayer->DealVirtualRoker(Roker_Down);
		else _orgin > 180 ? sPlayer->DealVirtualRoker(Roker_Down_Right) : sPlayer->DealVirtualRoker(Roker_Left_Down);
	}
	else if (_orgin > 225 && _orgin < 315)
	{
		if (abs(_orgin - 270) < 15)
			sPlayer->DealVirtualRoker(Roker_Right);
		else _orgin > 280 ? sPlayer->DealVirtualRoker(Roker_Up_Right) : sPlayer->DealVirtualRoker(Roker_Down_Right);
	}
	else
	{
		if (_orgin < 15 || _orgin > 345)
			sPlayer->DealVirtualRoker(Roker_Up);
		else if (_orgin < 45)
			sPlayer->DealVirtualRoker(Roker_Up_Left);
		else
			sPlayer->DealVirtualRoker(Roker_Up_Right);
	}
}

float PlayerUILayer::GetVirtualRokerOrgin(Vec2 CenterPoint, Vec2 RokerPoint)
{
	float Correct_Orgin = atan(abs(RokerPoint.y - CenterPoint.y) / abs(RokerPoint.x - CenterPoint.x)) * (180.0f / 3.1415926f);
	if (RokerPoint.x >= CenterPoint.x)
	{
		if (RokerPoint.y >= CenterPoint.y)
			Correct_Orgin += 90.0f;
		else
			Correct_Orgin = 90.0f - Correct_Orgin;
		Correct_Orgin += 180.0f;
	}
	else if (RokerPoint.y > CenterPoint.y)
	{
		Correct_Orgin = 90.0f - Correct_Orgin;
	}
	else
		Correct_Orgin += 90.0f;

	return Correct_Orgin;
}

void PlayerUILayer::update(float diff)
{
	if (m_Player_Info_UI_Hp->getPercentage() != 100.0f)
	{
		float posnow = m_Player_Info_UI_Hp->getPercentage();
		m_Player_Info_UI_Hp->setPercentage(posnow + 1);
	}
}