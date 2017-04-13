#include "MainMapLayer.h"
#include "Player.h"
#include "Npc.h"
#include "Monster.h"
#include "DataMgr.h"

Main_Map_Layer* _Main_Map_Layer = nullptr;

Main_Map_Layer::Main_Map_Layer(int MapId)
{
	_Main_Map_Layer = this;
	m_Mapid = MapId;
	m_WaitForLoadingNpcs.clear();
	m_WaitForLoadingMonsters.clear();
	for (int i = Object_GroundSprite; i != Object_Endl; i++)
	{
		std::vector<WaitForLoadingObjectTemplate> TempSprite;
		TempSprite.clear();
		m_WaitForLoadingObjects[(MapObjectType)i] = TempSprite;
	}
	SwapMap(m_Mapid);
}

Main_Map_Layer::~Main_Map_Layer()
{
}

Main_Map_Layer* Main_Map_Layer::GetInstance()
{
	if (!_Main_Map_Layer)
		return nullptr;

	return _Main_Map_Layer;
}

bool Main_Map_Layer::SwapMap(int insteadid)
{
	unscheduleUpdate();

	removeAllChildrenWithCleanup(true);
	ClearVectors();
	LoadMapInfo(insteadid);


	scheduleUpdate();
	return true;
}

bool Main_Map_Layer::LoadMapInfo(int mapid)
{
	if (!mapid)
		mapid = m_Mapid;

	FillLoadVectors(mapid);

	return true;
}

void Main_Map_Layer::FillLoadVectors(int id)
{
	//读取地图信息添加到待载入列表
	std::string LoadTemplateName = "";
	for (std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>>::iterator itr = m_WaitForLoadingObjects.begin(); itr != m_WaitForLoadingObjects.end(); itr++)
	{
		switch (itr->first)
		{
			case Object_GroundSprite:	LoadTemplateName = "map_ground_template";			break;
			case Object_BackGround:		LoadTemplateName = "map_back_ground_template";		break;
			case Object_ForeGround:		LoadTemplateName = "map_fore_ground_template";		break;
			case Object_FloatingObject:	LoadTemplateName = "map_floating_object_template";	break;
		}
		char msg[255];
		snprintf(msg, 255, "SELECT url,pos_x,pos_y FROM %s WHERE map_id = %d", LoadTemplateName.c_str(), id);
		Result _Result;
		if (sDataMgr->selectUnitDataList(msg, _Result))
		{
			if (_Result.empty())
			{
				log("ErrDB: LoadMapInfo: Empty Template %s For Loading Map %d", LoadTemplateName.c_str(), id);	continue;
			}
			else
			{
				for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
				{
					std::vector<RowInfo> row = ito->second;
					if (row.empty()) continue;
					WaitForLoadingObjectTemplate _SingleTemplate;
					_SingleTemplate.url			= row.at(0).GetString();
					_SingleTemplate.pos_x		= row.at(1).GetFloat();
					_SingleTemplate.pos_y		= row.at(2).GetFloat();
					itr->second.push_back(_SingleTemplate);
				}
			}
		}
		else { log("ErrDB: LoadMapInfo: Error Loading %s For Loading Map %d", LoadTemplateName.c_str(), id);	continue; }
	}

	for (int i = 0; i != 2; i++)
	{
		i ? LoadTemplateName = "map_npc_template" : LoadTemplateName = "map_monster_template";
		char get[255];//			0	 1		2	 3		4	5
		snprintf(get, 255, "SELECT json,atlas,pos_x,pos_y,guid,entry FROM %s WHERE map_id = %d", LoadTemplateName.c_str(), id);
		Result _Result;
		if (sDataMgr->selectUnitDataList(get, _Result))
		{
			if (_Result.empty())
			{
				log("ErrDB: LoadMapInfo: Empty Template %s For Loading Map %d", LoadTemplateName.c_str(), id);	continue;
			}
			else
			{
				for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
				{
					std::vector<RowInfo> row = ito->second;
					if (row.empty()) continue;
					WaitFroLoadingUnitTemplate _SingleTemplate;
					_SingleTemplate.json	= row.at(0).GetString();
					_SingleTemplate.atlas	= row.at(1).GetString();
					_SingleTemplate.pos_x	= row.at(2).GetFloat();
					_SingleTemplate.pos_y	= row.at(3).GetFloat();
					_SingleTemplate.guid	= row.at(4).GetInt();
					_SingleTemplate.entry	= row.at(5).GetInt();
					i ? m_WaitForLoadingNpcs.push_back(_SingleTemplate) : m_WaitForLoadingMonsters.push_back(_SingleTemplate);
				}
			}
		}
		else { log("ErrDB: LoadMapInfo: Error Loading %s For Loading Map %d", LoadTemplateName.c_str(), id);	continue; }
	}
	NeedCreateObjects = true;
}

void Main_Map_Layer::ClearVectors()
{
	m_NpcVector.clear();
	m_MonsterVector.clear();
	m_MapGroundSpriteVector.clear();
	m_MapFloatingObjectVector.clear();
	m_MapBackGroundVector.clear();
	m_MapForeGroundVector.clear();
	m_WaitForLoadingNpcs.clear();
	m_WaitForLoadingMonsters.clear();
	for (std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>>::iterator itr = m_WaitForLoadingObjects.begin(); itr != m_WaitForLoadingObjects.end(); itr++)
		itr->second.clear();
}

void Main_Map_Layer::CreateObjects()
{
	for (std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>>::iterator itr = m_WaitForLoadingObjects.begin(); itr != m_WaitForLoadingObjects.end(); itr++)
	{
		if (!itr->second.empty())
		{
			WaitForLoadingObjectTemplate _template = itr->second.at(itr->second.size() - 1);
			Sprite* Temp = Sprite::create(_template.url.c_str());
			Temp->SetRealPosition(_template.pos_x, _template.pos_y);
			addChild(Temp);
			switch (itr->first)
			{
				case Object_GroundSprite:	m_MapGroundSpriteVector.push_back(Temp);	break;
				case Object_BackGround:		m_MapBackGroundVector.push_back(Temp);		break;
				case Object_ForeGround:		m_MapForeGroundVector.push_back(Temp);		break;
				case Object_FloatingObject: m_MapFloatingObjectVector.push_back(Temp);	break;
			}
			itr->second.pop_back();
			return;
		}
	}
	if (!m_WaitForLoadingNpcs.empty())
	{
		WaitFroLoadingUnitTemplate _template = m_WaitForLoadingNpcs.at(m_WaitForLoadingNpcs.size() - 1);
		SkeletonAnimation* sk = SkeletonAnimation::createWithFile(_template.json.c_str(), _template.atlas.c_str(), 1.0f);
		Npc* Temp = new Npc(sk, _template.entry, _template.guid);
		Temp->SetRealPosition(_template.pos_x, _template.pos_y);
		m_WaitForLoadingNpcs.pop_back();
		return;
	}
	if (!m_WaitForLoadingMonsters.empty())
	{

		WaitFroLoadingUnitTemplate _template = m_WaitForLoadingMonsters.at(m_WaitForLoadingMonsters.size() - 1);
		SkeletonAnimation* sk = SkeletonAnimation::createWithFile(_template.json.c_str(), _template.atlas.c_str(), 1.0f);
		Monster* Temp = new Monster(sk, _template.entry, _template.guid);
		Temp->SetRealPosition(_template.pos_x, _template.pos_y);
		m_WaitForLoadingMonsters.pop_back();
		return;
	}

	NeedCreateObjects = false;
}

void Main_Map_Layer::update(float diff)
{
	if (NeedCreateObjects)
	{
		CreateObjects();
		return;
	}
}