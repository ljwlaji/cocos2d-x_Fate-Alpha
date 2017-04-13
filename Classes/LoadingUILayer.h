#ifndef __LOADING_UI_LAYER__
#define __LOADING_UI_LAYER__


#include "cocos2d.h"
#include "Player.h"
#include "Types.h"
USING_NS_CC;

#define sLoadingLayer LoadingUILayer::GetInstance()
class LoadingUILayer : public Layer
{
public:
	LoadingUILayer();
	~LoadingUILayer();
	virtual bool init();
	CREATE_FUNC(LoadingUILayer);
	void Show();
	void Reset();
	void SetPretage(uint8 _var)	{ m_TimeBarPrescent = _var; }
	void DisAppear();
	Player* GetPlayer()	{ return _Player; }
	static LoadingUILayer* GetInstance();
private:
	virtual void update(float diff);
	void ResetTimeBar();
	void InitTimeBar();
	void RandomSetBackGroundImage();
	Player* _Player;
	//时间条百分比
	uint8 m_TimeBarPrescent;

	//时间条

	Sprite* BackGroundSprite;
	Sprite* BackGroundSprite_a;
	Sprite* BackGroundSprite_c;
	ProgressTimer* m_TimeBar;
	Size visibleSize;
	Vec2 origin;
};

#endif