#include "LootMgr.h"
#include "DataMgr.h"

static LootMgr* _LootMgr = nullptr;

LootMgr::LootMgr()
{
	LoadLootInfoFromDB();
}

LootMgr::~LootMgr()
{
	_LootMgr = nullptr;
}

LootMgr* LootMgr::GetInstance()
{
	if (!_LootMgr)
		_LootMgr = new LootMgr();
	return _LootMgr;
}

void LootMgr::LoadLootInfoFromDB()
{
	LootInfoMap.clear();
	Result _res;//									0		1		2			3		4
	if (sDataMgr->selectUnitDataList("SELECT creature_id,item_id,min_count,max_count,refance FROM loot_template", _res))
	{
		if (_res.empty())
		{
			log("SELECT DB ERROR LootMgr::LoadLootInfoFromDB() From loot_template Error No DB Info");
		}
		else
		{
			int Creatureid = 0;
			std::vector<RowInfo> row;
			for (Result::iterator itr = _res.begin(); itr != _res.end(); itr++)
			{
				row = itr->second;
				if (Creatureid != row.at(0).GetInt())
				{
					std::list<SingleLootInfo> _info;
					LootInfoMap[row.at(0).GetInt()] = _info;
					Creatureid = row.at(0).GetInt();
				}
				SingleLootInfo _tempSingleInfo;
				_tempSingleInfo.ItemID = row.at(1).GetInt();
				_tempSingleInfo.MinCount = row.at(2).GetInt();
				_tempSingleInfo.MaxCount = row.at(3).GetInt();
				_tempSingleInfo.Refance = row.at(4).GetInt();
				LootInfoMap[Creatureid].push_back(_tempSingleInfo);
			}
		}
	}
	else
	{
		log("SELECT DB ERROR LootMgr::LoadLootInfoFromDB() From loot_template Maybe No DataBase Template In DB!");
	}
}

const std::list<SingleLootInfo>* LootMgr::GetLootInfo(const uint32& creatureid)
{
	if (LootInfoMap.find(creatureid) != LootInfoMap.end())
		return &LootInfoMap[creatureid];
	return nullptr;
}