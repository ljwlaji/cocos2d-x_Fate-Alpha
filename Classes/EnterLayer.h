#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

#define sEnterLayer EnterLayer::GetInstance()
class UISprite;
class EnterLayer : public Layer
{
public:
	static EnterLayer* GetInstance();
	virtual bool init();
	CREATE_FUNC(EnterLayer);
	void OnConnectSuccessed();
	void StartConnect();
	void _connect();
	void IntoMainPage();
	void DestorySprite(Sprite* pSprite);
private:
	EnterLayer();
	~EnterLayer();
	void typingshowup();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	virtual void update(float diff);
	EventListenerTouchOneByOne* Listener;
	Sprite* Logo;
	Sprite* BackGround;
	void CreateConnectingSprite(int i);
	LabelTTF* ConnectLabel;
	std::list<Sprite*> ConnectList;
	std::list<UISprite*> UISpriteList;
	float DelayTimer;
};

#endif