#ifndef __UI_SPRITE_H__
#define __UI_SPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

enum PlayerUITouchType
{
	PlayerUITouch_Typing_Layer,
	PlayerUITouch_Notify_Sprite,
	Main_UISprite_Title_Sprite,
	Main_UISprite_Auction_Sprite,
	Main_UISprite_News_Sprite,
	Main_UISprite_Uper_Sprite,
	Main_UISprite_Account_Sprite,
	Main_UISprite_WorldChat_Sprite,
	Main_UISprite_AuctionUI_Sprite,
};

class UISprite : public Sprite
{
public:
	UISprite();
	virtual ~UISprite();
	bool IsSpriteOutOfRange(Sprite* pSprite);
	virtual bool OnUITouchBegin(Touch* pTouch) = 0;
	virtual void OnUITouchMoved(Touch* pTouch) {};
	virtual void OnUITouchEnded(Touch* pTouch) = 0;
	virtual void SetTouchType(PlayerUITouchType _var)	{ _PlayerUITouchType = _var; }
	virtual PlayerUITouchType GetTouchType()			{ return _PlayerUITouchType; }
private:
	PlayerUITouchType _PlayerUITouchType;
};


#endif