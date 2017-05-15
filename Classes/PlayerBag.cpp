#include "PlayerBag.h"
#include "PlayerUILayer.h"
#include "Player.h"
#include "Item.h"
#include "PlayerEquipWindow.h"

static PlayerBag* _PlayerBag = nullptr;

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
	SetTouchType(PlayerUITouch_Bag);
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
	char msg[255];//				0		1		2		3		4
	snprintf(msg, 255, "SELECT item_entry,bag_page,bag_slot,count,item_guid FROM player_inventory WHERE guid = %u", sPlayer->GetGuid());
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
					Item* pItem = Item::CreateItem(info.at(0).GetInt(), info.at(1).GetInt(), info.at(2).GetInt(), info.at(4).GetInt());
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

bool PlayerBag::OnUITouchBegin(Touch* touches)
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

void PlayerBag::OnUITouchMoved(Touch* touches)
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
		float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
		setPosition(getPositionX() + X_Modify, getPositionY() + Y_Modify);
		m_Start_Move_Position = touches->getLocation();
	}
}

void PlayerBag::OnUITouchEnded(Touch* touches)
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
		_TempPageSprite->setTag((1 + i) * 10);
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