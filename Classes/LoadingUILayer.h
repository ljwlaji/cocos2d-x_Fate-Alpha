#ifndef __LOADING_UI_LAYER__
#define __LOADING_UI_LAYER__


#include "cocos2d.h"
#include "Player.h"
#include "Types.h"
USING_NS_CC;

class LoadingUILayer : public Layer
{
public:
	LoadingUILayer();
	~LoadingUILayer();
	virtual bool init();
	CREATE_FUNC(LoadingUILayer);
	void ShowLoadingLayer(LoadAddress _addr);
	void DisAppear();
	Player* GetPlayer()	{ return _Player; }
private:
	virtual void update(float diff);
	void ResetTimeBar();
	void InitTimeBar();

	Player* _Player;
	//时间条百分比
	uint8 m_TimeBarPrescent;

	//时间条
	Sprite* BackGroundSprite;
	ProgressTimer* m_TimeBar;
	Size visibleSize;
	Vec2 origin;
};

#endif