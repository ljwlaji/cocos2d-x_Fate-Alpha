#include "PlayerEquipWindow.h"
#include "PlayerUILayer.h"
#include "Player.h"
#include "NotifyMgr.h"
#include "PlayerBag.h"
#include "DataMgr.h"
#include "Item.h"
#include "ItemDetailSprite.h"

static PlayerEquipWindow* _PlayerEquipWindow = nullptr;
static PlayerEuqipValueWindow* _PlayerEuqipValueWindow = nullptr;

PlayerEquipWindow::PlayerEquipWindow()
{
	m_TouchedSprite = nullptr;
	m_Character_ValueButton = nullptr;
	initWithFile(PlayerUIEquipFrame);
	autorelease();
	InitWindow();
	IsTouchedDisPlaySprite = false;
	setVisible(false);
	SetTouchType(PlayerUITouch_Equip_Window);
}

Slot* PlayerEquipWindow::GetSlotByTouch(Touch* toouch)
{
	for (int i = SLOT_WEAPON; i != SLOT_END; i++)
	{
		if (Slot* pSlot = (Slot*)getChildByTag(i))
		{
			if (pSlot->IsContectPoint(toouch->getLocation()))
				return pSlot;
		}
	}
	return nullptr;
}

void PlayerEquipWindow::InitWindow()
{
	float TempXPoint = 0;
	float TempYPoint = 0;
	for (int i = SLOT_WEAPON; i != SLOT_END; i++)
	{
		Slot* TempSlot = new Slot("Player_UI_Equip_Slot_Frame.png");
		i % 2 ? TempXPoint = getContentSize().width * 0.17f : TempXPoint = getContentSize().width - getContentSize().width * 0.12f;
		uint8 fieldcount = i / 2;
		TempYPoint = getBoundingBox().size.height * 0.15f + (fieldcount * (TempSlot->getBoundingBox().size.height));
		TempSlot->setPosition(TempXPoint, TempYPoint);
		TempSlot->setTag(i);
		addChild(TempSlot);
	}
	m_Character_ValueButton = Sprite::create("Player_UI_Equip_Character_Value_Title.png");
	m_Character_ValueButton->setPosition(getContentSize().width * 0.5225f, m_Character_ValueButton->getBoundingBox().size.height * 0.75f);
	m_Character_ValueButton->setTag(ValueButtonTag);
	addChild(m_Character_ValueButton);

	m_Character_Name_Frame = Sprite::create("Player_UI_Equip_Character_Name.png");
	m_Character_Name_Frame->setPosition(m_Character_ValueButton->getPositionX(),getContentSize().height - m_Character_Name_Frame->getBoundingBox().size.height * 1.2f);
	m_Character_Name_Frame->setTag(Name_Frame_Tag);
	addChild(m_Character_Name_Frame);
}

void PlayerEquipWindow::LoadEquipSlot()
{
	Result _res;
	if (sDataMgr->selectUnitDataList(_res,"SELECT item_entry,bag_page,bag_slot,count,item_guid FROM player_inventory WHERE guid = %u AND bag_page == 100", sPlayer->GetGuid()))
	{
		if (!_res.empty())
		{
			for (Result::iterator itr = _res.begin(); itr != _res.end(); itr++)
			{
				std::vector<RowInfo> info = itr->second;
				if (Slot* TempSlot = (Slot*)getChildByTag(info.at(2).GetInt()))
				{
					Item* pItem = Item::CreateItem(info.at(0).GetInt(), info.at(1).GetInt(), info.at(2).GetInt(), info.at(4).GetInt());
					if (!pItem)
						continue;
					pItem->SetCount(info.at(3).GetInt());
					TempSlot->SetItem(pItem);
				}
			}
			sPlayer->CalcItemValues();
		}
	}
	else
	{
		log("Load Template Error From PlayerEquipWindow::LoadEquipSlot() Template name = player_inventory");
	}
}

bool PlayerEquipWindow::OnUITouchBegin(Touch* touches)
{
	m_TouchedSprite = nullptr;
	IsTouchedDisPlaySprite = false;

	for (Vector<Node*>::iterator itr = getChildren().begin(); itr != getChildren().end(); itr++)
	{
		if ((*itr)->IsContectPoint(touches->getLocation()))
		{
			m_TouchedSprite = (Sprite*)(*itr);
			if (m_TouchedSprite->getTag() >= SLOT_WEAPON && m_TouchedSprite->getTag() < SLOT_END)
			{
				m_Start_Move_Position = touches->getLocation();
				sItemDetailSprite->ShowWithItem(((Slot*)m_TouchedSprite)->GetItem());
			}
			return true;
		}
	}

	m_TouchedSprite = this;
	m_Start_Move_Position = touches->getLocation();
}

void PlayerEquipWindow::OnUITouchMoved(Touch* touches)
{
	if (m_TouchedSprite == this)
	{
		float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
		float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
		setPosition(getPositionX() + X_Modify, getPositionY() + Y_Modify);
		sPlayerValueWindow->setPosition(sPlayerValueWindow->getPositionX() + X_Modify, sPlayerValueWindow->getPositionY() + Y_Modify);
		m_Start_Move_Position = touches->getLocation();
	}
	else if (m_TouchedSprite->getTag() >= 0 && m_TouchedSprite->getTag() <= 8)
	{
		if (!((Slot*)m_TouchedSprite)->GetItem())
			return;

		if (Sprite* DisPlaySprite = ((Slot*)m_TouchedSprite)->GetDisPlaySprite())
		{
			float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
			float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
			DisPlaySprite->setPosition(DisPlaySprite->getPositionX() + X_Modify, DisPlaySprite->getPositionY() + Y_Modify);
			m_Start_Move_Position = touches->getLocation();
			IsTouchedDisPlaySprite = true;
		}
	}
}

void PlayerEquipWindow::OnUITouchEnded(Touch* touches)
{
	if (!m_TouchedSprite)
		return;

	if (IsTouchedDisPlaySprite)
	{
		Slot* OldSlot = ((Slot*)m_TouchedSprite);
		Slot* NewSlot = GetSlotByTouch(touches);
		if (!NewSlot)
			NewSlot = sPlayerBag->GetSlotByTouch(touches);
		OldSlot->SwapItem(NewSlot);
		return;
	}
	else
	{
		if (!m_TouchedSprite->IsContectPoint(touches->getLocation()))
			return;
		switch (m_TouchedSprite->getTag())
		{
		case SLOT_WEAPON:
		case SLOT_SECOND_WEAPON:
		case SLOT_LEFT_HAND:
		case SLOT_RIGHT_HAND:
		case SLOT_RING_1:
		case SLOT_RING_2:
		case SLOT_HEAD:
		case SLOT_SHOULDER:
			OnClickedItemSlot(m_TouchedSprite->getTag());
			break;
		case ValueButtonTag:
			sPlayerValueWindow->isVisible() ? sPlayerValueWindow->setVisible(false) : sPlayerValueWindow->setVisible(true);
			break;
		case Name_Frame_Tag:
			break;
		}
	}
}

void PlayerEquipWindow::OnClickedItemSlot(const uint32& _tag)
{

}

void PlayerEquipWindow::SwapVisiable()			
{ 
	if (isVisible())
	{
		setVisible(false);
		sPlayerValueWindow->setVisible(false);
	}
	else
	{
		setVisible(true);
		sPlayerValueWindow->setVisible(true);
	}
}

PlayerEquipWindow::~PlayerEquipWindow()
{
	removeAllChildrenWithCleanup(true);
	_PlayerEquipWindow = nullptr;
}

PlayerEquipWindow* PlayerEquipWindow::GetInstance()
{
	if (!_PlayerEquipWindow)
	{
		_PlayerEquipWindow = new PlayerEquipWindow();
		_PlayerEquipWindow->LoadEquipSlot();
	}
	return _PlayerEquipWindow;
}

PlayerEuqipValueWindow::PlayerEuqipValueWindow()
{
	_PlayerEuqipValueWindow = nullptr;
	initWithFile(PlayerUIEquipValueFrame);
	autorelease();
	Init();
	setVisible(false);
}

PlayerEuqipValueWindow::~PlayerEuqipValueWindow()
{
	removeAllChildrenWithCleanup(true);
	_PlayerEuqipValueWindow = nullptr;
}


PlayerEuqipValueWindow* PlayerEuqipValueWindow::GetInstance()
{
	if (!_PlayerEuqipValueWindow)
		_PlayerEuqipValueWindow = new PlayerEuqipValueWindow();
	return _PlayerEuqipValueWindow;
}

void PlayerEuqipValueWindow::Init()
{
	m_TTFs.clear();

	LabelTTF* Title = LabelTTF::create("Character Stats", "Arial", 34, Size::ZERO, TextHAlignment::CENTER);
	Title->setPosition(getContentSize().width * 0.435f, getContentSize().height * 0.79f);
	Title->setColor(ccc3(0, 0, 0));
	addChild(Title);

	for (int i = Attack; i != TTF_END; i++)
	{
		LabelTTF* TempTTF = LabelTTF::create("test", "Arial", 29, Size::ZERO, TextHAlignment::LEFT);
		TempTTF->setAnchorPoint(Vec2(0, 0.5f));
		TempTTF->setPosition(getContentSize().width * 0.2f, getContentSize().height * 0.573f - (i * TempTTF->getBoundingBox().size.height * 1.02f));
		TempTTF->setColor(ccc3(0, 0, 0));
		addChild(TempTTF);
		m_TTFs[(ValueWindowTTF)i] = TempTTF;

		LabelTTF* Plus = LabelTTF::create("test", "Arial", 29, Size::ZERO, TextHAlignment::LEFT);
		Plus->setAnchorPoint(Vec2(0, 0.5f));
		Plus->setPosition(TempTTF->getPositionX() + TempTTF->getBoundingBox().size.width, TempTTF->getPositionY());
		Plus->setColor(ccc3(100, 255, 100));
		addChild(Plus);
		PlusTTF[(ValueWindowTTF)i] = Plus;
	}
	ResetValueDefault();
}

std::string itos(int _int)
{
	std::string return_;
	char msg[255];
	snprintf(msg, 255, "%d", _int);
	return_ = msg;
	return return_;
}

void PlayerEuqipValueWindow::ResetValueDefault()
{
	if (!sPlayer)
		return;

	std::string str = "";
	std::string plus = "";


	for (std::map<ValueWindowTTF, LabelTTF*>::iterator itr = m_TTFs.begin(); itr != m_TTFs.end(); itr++)
	{
		plus = " + ";
		switch (itr->first)
		{
		case Attack:
			str = "Attack:";
			str += itos(sPlayer->GetUnitInt32Value(Base_Att));

			if (uint32 val = sPlayer->GetItemTotalAttack())
				plus += itos(val);
			else
				plus = "";
			break;
		case Def:
			str = "Def:";
			str += itos(sPlayer->GetUnitInt32Value(Base_Def));

			if (uint32 val = sPlayer->GetEquipItemTotalValusForKey(Base_Def))
				plus += itos(val);
			else
				plus = "";
			break;
		case Str:
			str = "Str:";
			str += itos(sPlayer->GetUnitInt32Value(Base_Str));

			if (uint32 val = sPlayer->GetEquipItemTotalValusForKey(Base_Str))
				plus += itos(val);
			else
				plus = "";
			break;
		case Dex:
			str = "Dex:";
			str += itos(sPlayer->GetUnitInt32Value(Base_Dex));

			if (uint32 val = sPlayer->GetEquipItemTotalValusForKey(Base_Dex))
				plus += itos(val);
			else
				plus = "";
			break;
		case Int:
			str = "Int:";
			str += itos(sPlayer->GetUnitInt32Value(Base_Int));

			if (uint32 val = sPlayer->GetEquipItemTotalValusForKey(Base_Int))
				plus += itos(val);
			else
				plus = "";

			break;
		}
		itr->second->setString(str.c_str());
		if (LabelTTF* TempPlus = PlusTTF[itr->first])
		{
			TempPlus->setString(plus.c_str());
			TempPlus->setPosition(itr->second->getPositionX() + itr->second->getBoundingBox().size.width, itr->second->getPositionY());
		}
	}
}