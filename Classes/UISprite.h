#ifndef __UI_SPRITE_H__
#define __UI_SPRITE_H__

#include "Types.h"

class UISprite : public Sprite
{
public:
	UISprite();
	~UISprite();

	virtual bool OnUITouchBegin(Touch* pTouch) = 0;
	virtual void OnUITouchMoved(Touch* pTouch) {};
	virtual void OnUITouchEnded(Touch* pTouch) = 0;
	virtual void SetTouchType(PlayerUITouchType _var)	{ _PlayerUITouchType = _var; }
	virtual PlayerUITouchType GetTouchType()			{ return _PlayerUITouchType; }
private:
	PlayerUITouchType _PlayerUITouchType;
};


#endif