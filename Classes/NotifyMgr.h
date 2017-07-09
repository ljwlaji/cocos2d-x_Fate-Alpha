#ifndef __NOTIFY_MGR_H__
#define __NOTIFY_MGR_H__

#include "UISprite.h"
#include "Common.h"

#define sNotifyMgr	NotifyMgr::GetInstance()
class NotifyMgr : public UISprite
{
public:
	static NotifyMgr* GetInstance();
	void ShowUpWithDisabaledTouching(float WaitSecond,const char* format, ...);
	void ShowUpWithNormalNotify(const char* formal, ...);
	bool IsDisabaleTouch()		{ return DisabaleTouching; }
	virtual bool OnUITouchBegin(Touch* pTouch);
private:
	NotifyMgr();
	~NotifyMgr();
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	virtual void update(float diff);
private:
	bool DisabaleTouching;
	LabelTTF* NotiTTF;
	Sprite* CancelButton;
	Sprite* BackGround;
	Sprite* Frame;
	float DelayTimer;
};

#endif