#ifndef __MAIN_UILAYER_TITLE_SPRITE_H__
#define __MAIN_UILAYER_TITLE_SPRITE_H__

#include "UISprite.h"

#define sMainTitleSprite MainTitleSprite::GetInstance()

class MainTitleSprite : public UISprite
{
public:
	static MainTitleSprite* GetInstance();
	void InitSprite();
private:
	MainTitleSprite();
	~MainTitleSprite();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);




	Sprite* MenuIcon;
	Sprite* BattleNetTitle;
	Sprite* ChatIcon;
};

#endif