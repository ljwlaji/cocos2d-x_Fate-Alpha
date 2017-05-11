#include "Item.h"
#include "HelloWorldScene.h"
#include "NotifyMgr.h"
#include "PlayerUILayer.h"

Item* Item::CreateItem(const uint32& ItemEntry)
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
	return pItem;
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