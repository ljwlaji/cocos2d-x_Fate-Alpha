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
    virtual bool init();
	static MainScene* GetInstance();
	Sprite* ShowDiffcuteImage();
	Sprite* ShowCombatSignImage();
	Sprite* ShowSoundImage();
	void DisTorySprite(Sprite* pSprite);
	CREATE_FUNC(MainScene);
	void SwapLayer(int instead, int removetag, int mapid = 0);
	SkeletonAnimation* GetAnimationByClass(UnitClasses _class);
	Sprite* GetNumberSpriteByInt(int _var);
	bool GetFactionFriendly(uint32 factionA, uint32 FactionB);
	void SetCanPlaySound(bool var)					{ CanPlaySound = var; }
	void SetKeyBoardEnable(bool _var)				{ KeyBoardListener->setEnabled(_var); }
	void SetDiffCute(uint8 _diff)					{ m_Diffcute = (GameDiffcute)_diff; }
	void SetCombatSign(bool var)					{ CombatSign = var; }
	void LoadUnitClassInfo();
	bool GetCombatSign()							{ return CombatSign; }
	GameDiffcute GetDiffCute()						{ return m_Diffcute; }
	bool GetCanPlaySound()							{ return CanPlaySound; }
	ClassInfo GetUnitClassInfo(UnitClasses _car)	{ ClassInfo _ClassInfo; if (m_UnitClasses_Class_Info.find(_car) != m_UnitClasses_Class_Info.end()) _ClassInfo = m_UnitClasses_Class_Info[_car]; return _ClassInfo; }
private: 
	void LoadFactionInfo();
	bool IsMoveKey(EventKeyboard::KeyCode keyCode);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	LoadingUILayer* LoadingLayer;
	EnterGameLayer* EnterLayer;
	Size visibleSize;
	Vec2 origin;
	bool CanShowDiffCuteImage;
	bool CombatSign;
	bool CanPlaySound;
	EventListenerKeyboard* KeyBoardListener;
	LabelTTF* Temp;
	std::map<UnitClasses, ClassInfo> m_UnitClasses_Class_Info;
	std::map<uint32, std::map<uint32, bool>> m_Faction_Friendly_Info;
protected:
	GameDiffcute m_Diffcute;
};

#endif
