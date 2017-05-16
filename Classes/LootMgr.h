#ifndef __LOOT_MGR_H__
#define __LOOT_MGR_H__

#include "Types.h"

#define sLootMgr LootMgr::GetInstance()

class LootMgr
{
public:
	static LootMgr* GetInstance();
	const std::list<SingleLootInfo>* GetLootInfo(const uint32& creatureid);
private:
	LootMgr();
	~LootMgr();
	void LoadLootInfoFromDB();



private:
	std::map<uint32, std::list<SingleLootInfo>> LootInfoMap;
};


#endif
