#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "EnterGameLayer.h"
#include "LoadingUILayer.h"
USING_NS_CC;

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
	void LoadEverything();
	Sprite* ShowDiffcuteImage();
	Sprite* ShowCombatSignImage();
	Sprite* ShowSoundImage();
	void DisTorySprite(Sprite* pSprite);
	CREATE_FUNC(MainScene);
	void SetDiffCute(uint8 _diff)		{ m_Diffcute = (GameDiffcute)_diff; }
	void SetCombatSign(bool var)		{ CombatSign = var; }
	bool GetCombatSign()				{ return CombatSign; }
	GameDiffcute GetDiffCute()			{ return m_Diffcute; }
	void SetCanPlaySound(bool var)		{ CanPlaySound = var; }
	bool GetCanPlaySound()				{ return CanPlaySound; }
	void SwapLayer(Layer* instead, int removetag);
private:
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
	Player* _player;
protected:
	GameDiffcute m_Diffcute;
};

#endif
