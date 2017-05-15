#include "Item.h"
#include "HelloWorldScene.h"
#include "NotifyMgr.h"
#include "PlayerUILayer.h"
#include "DataMgr.h"
#include "Player.h"
Item* Item::CreateItem(const uint32& ItemEntry, const uint32& Page, const uint32& Slot,const uint32& ItemGuid)
{
	const ItemTemplate* TemCheck = sGame->GetItemTemplate(ItemEntry);
	if (!TemCheck)
	{
		char msg[255];
		snprintf(msg, 255, "Try Create Item Entry: %u Failed! Reason: Cannot Find Template In ItemTemplate Map!", ItemEntry);
		sNotifyMgr->ShowNotify(msg);
		return nullptr;
	}
	Item* pItem = new Item(TemCheck);
	pItem->SetSlot(Page, Slot);
	if (!ItemGuid)
	{
		pItem->SetGuid(pItem->CreateNewItemGuid() + 1);
		pItem->InsertToDB();
	}
	else
	{
		pItem->SetGuid(ItemGuid);
	}
	return pItem;
}

void Item::SetSlot(const uint32& Page, const uint32& Slot)
{
	m_Page = Page;
	m_Slot = Slot;
}

uint32 Item::CreateNewItemGuid()
{
	Result _res;
	if (sDataMgr->selectUnitDataList("SELECT MAX(item_guid) FROM player_inventory", _res))
	{
		if (_res.empty())
		{

		}
		else
		{
			return (*_res.begin()).second.at(0).GetInt();
		}
	}
}

Item::Item(const ItemTemplate* pTemplate)
{
	m_Template = pTemplate;
}

Item::~Item()
{

}

bool Item::CanEquipToSlot(const Slot* pSlot)
{
	if (pSlot->getTag() == m_Template->SlotType)
		return true;
	return false;
}

void Item::DeleteFromDB()
{
	char deletemsg[255];
	snprintf(deletemsg, 255, "DELETE FROM player_inventory WHERE guid = %d", m_Guid);
	sDataMgr->PExcute(deletemsg);
}

void Item::InsertToDB()
{
	char insertmsg[255];//												0			1
	snprintf(insertmsg, 255, "INSERT INTO player_inventory(item_guid,guid,bag_page,bag_slot,item_entry,count) VALUES(%d,%d,%d,%d,%d,%d);", m_Guid, sPlayer->GetGuid(), m_Page, m_Slot, m_Template->Entry, m_ItemCount);
	sDataMgr->PExcute(insertmsg);
}

void Item::SaveToDB()
{
	char insertmsg[255];//												0			1
	snprintf(insertmsg, 255, "UPDATE player_inventory SET guid = %d, item_entry = %d, bag_page = %d, bag_slot = %d, count = %d WHERE item_guid = %d;", sPlayer->GetGuid(), m_Template->Entry, m_Page, m_Slot, m_ItemCount, m_Guid);
	sDataMgr->PExcute(insertmsg);
}