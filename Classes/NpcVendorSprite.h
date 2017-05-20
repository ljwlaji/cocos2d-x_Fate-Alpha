#ifndef __NPC_VENDOR_SPRITE_H__
#define __NPC_VENDOR_SPRITE_H__

#include "UISprite.h"

#define sVendorSprite NpcVendorSprite::GetInstance()
class NpcVendorSprite : public UISprite
{
public:
	static NpcVendorSprite* GetInstance();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	void InitFrame();
	NpcVendorSprite();
	~NpcVendorSprite();
};

#endif