#include "Loot.h"
#include "LootMgr.h"
#include "Creature.h"
#include "Player.h"
#include "NotifyMgr.h"
#include "Item.h"
#include "PlayerBag.h"
#include "PlayerUILayer.h"
#include "TopBar.h"
Loot* Loot::CreateLoot(Creature* creature)
{
	if (const std::list<SingleLootInfo>* info = sLootMgr->GetLootInfo(creature->GetEntry()))
	{
		Loot* TempLoot = new Loot();
		TempLoot->SetLinkCreature(creature);
		for (std::list<SingleLootInfo>::const_iterator itr = info->begin(); itr != info->end(); itr++)
		{
			if (rand() % 100 <= itr->Refance)
			{
				uint32 Count = itr->MinCount + rand() % itr->MaxCount;
				TempLoot->SetLoot(itr->ItemID, Count);
			}
		}
		return TempLoot;
	}
	char msg[255];
	snprintf(msg, 255, "Create Loot Failed! No Loot Info With Creature %d", creature->GetEntry());
	sNotifyMgr->ShowNotify(msg);
	return nullptr;
}

Loot::Loot()
{

}

Loot::~Loot()
{
	m_Linker->SetLoot(nullptr);
}

void Loot::SetLoot(ItemID _id, Count _count)
{
	m_Loots[_id] = _count;
}

void Loot::Pick(ItemID _id)
{
	if (!_id)
	{
		sPlayer->SetMoney(sPlayer->GetMoney() + m_Loots[_id]);
		sTopBar->ReSetTopBarString();
	}
	else
	{
		uint16 _Slot = sPlayer->GetEmptyBagSlot();
		if (!_Slot)
		{
			sNotifyMgr->ShowNotify("No Empty Slots!");
			return;
		}

		uint8 Page = _Slot / 100;
		uint8 __Slot = _Slot % 100;
		Slot* pSlot = sPlayerBag->GetSlotByPageTag(Page, __Slot);
		Count _count = m_Loots[_id];
		Item* pItem = Item::CreateItem(_id, Page, __Slot);
		pItem->SaveToDB();
		pItem->SetCount(_count);
		pSlot->SetItem(pItem);
	}

	//**//
	for (std::map<ItemID, Count>::iterator itr = m_Loots.begin(); itr != m_Loots.end(); itr++)
	{
		if (itr->first == _id)
		{
			m_Loots.erase(itr);
			break;
		}
	}
	DeleteLootIfNeed(false);
}

void Loot::DeleteLootIfNeed(bool forcedelete)
{
	if (forcedelete)
		delete this;
	else if (m_Loots.empty())
		delete this;
}

void Loot::SetLinkCreature(Creature* pLinker)
{
	pLinker->SetLoot(this);
	m_Linker = pLinker;
}