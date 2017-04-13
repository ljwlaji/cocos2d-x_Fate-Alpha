#ifndef __MAIN_MAP_LAYER_H__
#define __MAIN_MAP_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

enum MapObjectType
{
	Object_GroundSprite,
	Object_BackGround,
	Object_ForeGround,
	Object_FloatingObject,
	Object_Endl,
};

struct WaitForLoadingObjectTemplate
{
	std::string url = "";
	float pos_x;
	float pos_y;
};

struct WaitFroLoadingUnitTemplate
{
	std::string json = "";
	std::string atlas = "";
	float pos_x;
	float pos_y;
	uint32 entry;
	uint32 guid;
};
#define sMainMap Main_Map_Layer::GetInstance()
class Monster;
class Player;
class Npc;
class Main_Map_Layer : public Layer
{
public:
	Main_Map_Layer(int MapId);
	~Main_Map_Layer();
	static Main_Map_Layer* GetInstance();
	bool SwapMap(int insteadid);
	Sprite* GetCurrentGroundSprite(int id)		{ int size = m_MapGroundSpriteVector.size(); if (size + 1 <= id) return m_MapGroundSpriteVector.at(id); return nullptr; }
	std::vector<Sprite*> GetGroundSprites()		{ return m_MapGroundSpriteVector; }
	void ClearVectors();
private:
	void FillLoadVectors(int mapid);
	void CreateObjects();
	virtual void update(float diff);
	std::vector<Npc*> m_NpcVector;
	std::vector<Monster*> m_MonsterVector;
	std::vector<Sprite*> m_MapGroundSpriteVector;
	std::vector<Sprite*> m_MapFloatingObjectVector;
	std::vector<Sprite*> m_MapBackGroundVector;
	std::vector<Sprite*> m_MapForeGroundVector;
	bool LoadGround();
	bool LoadFloatingObjects();
	bool LoadMonsters();
	bool LoadNpcs();
	bool LoadBackGround();
	bool LoadForeGround();


	int m_Mapid;

	std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>> m_WaitForLoadingObjects;
	std::vector<WaitFroLoadingUnitTemplate> m_WaitForLoadingNpcs;
	std::vector<WaitFroLoadingUnitTemplate> m_WaitForLoadingMonsters;

	bool NeedCreateObjects;

	uint32 TotalLoadingSize;
	uint32 LoadedSize;
};

#endif