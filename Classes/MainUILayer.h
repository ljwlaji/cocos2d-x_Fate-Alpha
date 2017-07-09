#ifndef __MAIN_UI_LAYER_H__
#define __MAIN_UI_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

enum LeftButtonTag
{
	MyBattleNet,
	Guild,
	Auction,
	Heros,
	Chat,
	Settings,
	EndButton,
};

struct LoadHanler;
class UISprite;
#define sMainUI	MainUILayer::GetInstance()

class MainUILayer : public Layer
{
public:
	static MainUILayer* GetInstance();
	void SetAccountShowing(bool val)	{ IsAccountShowing = val; }
	void SetChatShowing(bool val)		{ IsWorldChatShowing = val; }
private:
	virtual bool init();
	CREATE_FUNC(MainUILayer);
	MainUILayer();
	~MainUILayer();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	EventListenerTouchOneByOne*Listener;
	Point OldPoint;
	void CheckEdgeAndMoving(const Point& pPoint);
	bool CanMoveBackGroundLeftRight;

	std::list<Sprite*> UperButtons;
	std::list<UISprite*> UIList;

	Sprite* LogoArrow;
	bool IsAccountShowing;
	bool IsWorldChatShowing;
public:
	//Handler
	void HandleInitTitleButtons();
	void HandleInitUperButtons();
	void HandleInitNewsFrame();
	void HandleInitActionsFrame();
	void HandleInitAccountFrame();
	void HandleInitWorldChatFrame();
	void HandleInitAuctionHouse();
};

#endif