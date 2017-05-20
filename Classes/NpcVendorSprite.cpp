#include "NpcVendorSprite.h"

static NpcVendorSprite* _NpcVendorSprite = nullptr;

NpcVendorSprite::NpcVendorSprite()
{
	initWithFile("");
	autorelease();
}

NpcVendorSprite::~NpcVendorSprite()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_NpcVendorSprite = nullptr;
}

void NpcVendorSprite::InitFrame()
{

}

NpcVendorSprite* NpcVendorSprite::GetInstance()
{
	if (!_NpcVendorSprite)
		_NpcVendorSprite = new NpcVendorSprite();
	return _NpcVendorSprite;
}

bool NpcVendorSprite::OnUITouchBegin(Touch* pTouch)
{
	return true;
}
void NpcVendorSprite::OnUITouchMoved(Touch* pTouch)
{

}
void NpcVendorSprite::OnUITouchEnded(Touch* pTouch)
{

}