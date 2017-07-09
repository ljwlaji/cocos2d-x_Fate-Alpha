#include "NotifyMgr.h"
#pragma once
#pragma execution_character_set("utf-8")

static NotifyMgr* _NotifyMgr = nullptr;

NotifyMgr::NotifyMgr() : DisabaleTouching(false)
{
	initWithFile(NotifyMgr_BackGround);
	autorelease();

	SetTouchType(PlayerUITouch_Notify_Sprite);

	Frame = Sprite::create(NotifyMgr_Frame);
	Frame->setPosition(0,getContentSize().height);
	Frame->setAnchorPoint(Vec2(0.0f, 1.0f));
	addChild(Frame);

	CancelButton = Sprite::create(NotifyMgr_Button);
	CancelButton->setAnchorPoint(Vec2(0.5f, 0));
	CancelButton->setPosition(getContentSize().width / 2, 0);
	addChild(CancelButton);

	NotiTTF = LabelTTF::create("123", "Airal", 35);
	NotiTTF->setPosition(getContentSize().width / 2, getContentSize().height / 2 + NotiTTF->getBoundingBox().size.height);
	addChild(NotiTTF);

	setLocalZOrder(9000);
}


NotifyMgr::~NotifyMgr()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_NotifyMgr = nullptr;
}

NotifyMgr* NotifyMgr::GetInstance()
{
	if (!_NotifyMgr)
		_NotifyMgr = new NotifyMgr();
	return _NotifyMgr;
}

void NotifyMgr::ShowUpWithNormalNotify(const char* format, ...)
{
	va_list ap;
	char szQuery[1024];
	va_start(ap, format);
	int res = vsnprintf(szQuery, 1024, format, ap);
	va_end(ap);

	if (res == -1)
		return;

	NotiTTF->setString(szQuery);

	setVisible(true);
	DisabaleTouching = false;
	CancelButton->setVisible(true);
}

void NotifyMgr::ShowUpWithDisabaledTouching(float WaitSecond,const char* format, ...)
{
	va_list ap;
	char szQuery[1024];
	va_start(ap, format);
	int res = vsnprintf(szQuery, 1024, format, ap);
	va_end(ap);

	if (res == -1)
		return;

	unscheduleUpdate();
	NotiTTF->setString(szQuery);
	DelayTimer = (float)WaitSecond;
	setVisible(true);
	DisabaleTouching = true;
	CancelButton->setVisible(false);
	scheduleUpdate();
}



bool NotifyMgr::OnUITouchBegin(Touch* pTouch)
{
	if (CancelButton->isVisible() && CancelButton->IsContectPoint(pTouch->getLocation()))
		setVisible(false);
	return true;
}

void NotifyMgr::OnUITouchMoved(Touch* pTouch)
{

}

void NotifyMgr::OnUITouchEnded(Touch* pTouch)
{

}

void NotifyMgr::update(float diff)
{
	if (DelayTimer <= diff)
	{
		unscheduleUpdate();
		DelayTimer = 0;
		setVisible(false);
	}
	else DelayTimer -= diff;
}