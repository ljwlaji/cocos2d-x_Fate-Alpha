#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "EnterGameLayer.h"
#include "LoadingUILayer.h"
#include "extensions/cocos-ext.h"
#include "Types.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define sGame		MainScene::GetInstance()
enum GameDiffcute
{
	Rate_C,
	Rate_B,
	Rate_A,
	Rate_S,
};

class Player;
class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();
	uint32 GetCurrentExpPerLevel(uint8 PlayerLevel);
    virtual bool init();
	static MainScene* GetInstance();
	Sprite* ShowDiffcuteImage();
	Sprite* ShowCombatSignImage();
	Sprite* ShowSoundImage();
	void DisTorySprite(Sprite* pSprite);
	CREATE_FUNC(MainScene);
	void SwapLayer(int instead, int removetag, int mapid = 0);
	SkeletonAnimation* GetAnimationByClass(UnitClasses _class);
	std::vector<Sprite*> GetNumberSpriteByInt(int _var);
	bool GetFactionFriendly(uint32 factionA, uint32 FactionB);
	const ItemTemplate* GetItemTemplate(const uint32& ItemEntry);
	const SingleMapInfo* GetMapInfo(uint32 _mapid);
	bool IsQuestGiver(const uint32& Creatureid);
	const std::list<uint32>* GetCreatureQuests(uint32 creatureid);
	uint32 FindNearestReviveMap(uint32 CurrentMapid);
	std::string GetClassNameByClassID(UnitClasses _var);
	std::string GetUnitValueNameByID(UnitInt32Value _val);
	ClassInfo GetUnitClassInfo(UnitClasses _car)	{ ClassInfo _ClassInfo; if (m_UnitClasses_Class_Info.find(_car) != m_UnitClasses_Class_Info.end()) _ClassInfo = m_UnitClasses_Class_Info[_car]; return _ClassInfo; }
	void SetCanPlaySound(bool var)					{ CanPlaySound = var; }
	void SetKeyBoardEnable(bool _var)				{ KeyBoardListener->setEnabled(_var); }
	void SetDiffCute(uint8 _diff)					{ m_Diffcute = (GameDiffcute)_diff; }
	void SetCombatSign(bool var)					{ CombatSign = var; }
	bool GetCombatSign()							{ return CombatSign; }
	GameDiffcute GetDiffCute()						{ return m_Diffcute; }
	bool GetCanPlaySound()							{ return CanPlaySound; }
	const VendorList* GetCreatureVendorInfo(uint32 creature_id);
private: 
	void LoadVendorTemplate();
	void LoadExpPerLevelTemplate();
	void LoadQuestGiver();
	void LoadMapInfo();
	void LoadUnitClassInfo();
	void LoadFactionInfo();
	void LoadItemTemplate();
	bool IsMoveKey(EventKeyboard::KeyCode keyCode);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	Size visibleSize;
	Vec2 origin;
	bool CanShowDiffCuteImage;
	bool CombatSign;
	bool CanPlaySound;
	EventListenerKeyboard* KeyBoardListener;
	LabelTTF* Temp;
	std::map<UnitClasses, ClassInfo> m_UnitClasses_Class_Info;
	std::map<uint32, uint32> m_Exp_Per_Level;
	std::map<uint32, std::map<uint32, bool>> m_Faction_Friendly_Info;
	std::map<uint32, ItemTemplate> m_ItemTemplate;
	std::map<uint32, SingleMapInfo> m_MapInfo;
	std::map<uint32, std::list<uint32>> m_QuestGivers;
	std::map<uint32, std::list<SingleVendorInfo>> m_VendorTemplate;
	virtual void update(float diff);
	float ClearCacheDelayTime;
protected:
	GameDiffcute m_Diffcute;
};

#endif
