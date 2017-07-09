#ifndef __LOOT_H__
#define __LOOT_H__
#include "Types.h"
class Creature;
class Loot
{
public:
	static Loot* CreateLoot(Creature* pCreature);
private:
	typedef uint32 ItemID;
	typedef uint32 Count;
	void SetLoot(ItemID _id, Count _count);
	void SetLinkCreature(Creature* pLinker);
	Loot();
	~Loot();
	std::map<ItemID, Count> m_Loots;
	Creature* m_Linker;
public:
	void Pick(ItemID _id);
	void DeleteLootIfNeed(bool forcedelete = false);
	const std::map<ItemID, Count>* GetLoots()	{ return &m_Loots; }
};



#endif