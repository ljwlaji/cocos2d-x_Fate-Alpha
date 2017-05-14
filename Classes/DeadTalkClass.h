#ifndef __DEAD_TALK_CLASS_H__
#define __DEAD_TALK_CLASS_H__

#include "Types.h"
#include "UISprite.h"

#define sDeadTalkClass	DeadTalkClass::GetInstance()

class DeadTalkClass : public UISprite
{
public:
	static DeadTalkClass* GetInstance();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void Show();
	void Disappear();
private:
	DeadTalkClass();
	~DeadTalkClass();
	Sprite* ButtonYes;
	Sprite* ButtonNo;
	Sprite* TouchedSprite;
};

#endif