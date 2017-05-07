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

enum TouchedType
{
	Touch_None,
	Touch_Npc,
	Touch_Monster,
	Touch_Player,
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
	float pos_x = 0;
	float pos_y = 0;
	uint32 entry = 0;
	uint32 guid = 0;
	float Scale = 1.0f;
	std::string ScriptName = "";
	uint32 faction = 0;
	uint32 Level = 0;
	uint32 Class = 0;
};


#define sMainMap Main_Map_Layer::GetInstance()

class Monster;
class Player;
class Npc;
class Unit;
class Main_Map_Layer : public Layer
{
public:
	Main_Map_Layer(int MapId);
	~Main_Map_Layer();
	static Main_Map_Layer* GetInstance();
	bool SwapMap(int insteadid, bool FirstLoad = true);
	WaitFroLoadingUnitTemplate GetCreatureTemplate(uint32 guid);
	Sprite* GetCurrentGroundSprite(int id)		{ int size = m_MapGroundSpriteVector.size(); if (size + 1 <= id) return m_MapGroundSpriteVector.at(id); return nullptr; }
	std::vector<Sprite*> GetGroundSprites()		{ return m_MapGroundSpriteVector; }
	float GetMapMaxWidth()						{ return m_MaxSize; }
	Unit* GetNearestUnitForUnit(Unit* pUnit, bool SelectForTarget = true, bool CheckAlive = true);
private:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void ReCheckZorder();
	void ClearVectors();
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

	std::map<uint32, WaitFroLoadingUnitTemplate> CreaturesTemplate;
	Vec2 Visablesize;

	Sprite* m_TouchedSprite;
	TouchedType m_TouchedType;

	Sprite* m_Next_Map_Door;
	Sprite* m_Older_Map_Door;

	float m_MaxSize;
	std::list<Unit*> CalcZorderList;
	std::list<Unit*>::iterator Listitr;
	float m_CheckZorderTimer = 1.0f;
	EventListenerTouchOneByOne* listener;
};

#endif