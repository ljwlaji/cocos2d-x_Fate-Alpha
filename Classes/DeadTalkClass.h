#ifndef __DEAD_TALK_CLASS_H__
#define __DEAD_TALK_CLASS_H__

#include "Types.h"

#define sDeadTalkClass	DeadTalkClass::GetInstance()

class DeadTalkClass : public Sprite
{
public:
	static DeadTalkClass* GetInstance();
	void OnTouchBegin(Touch* pTouch);
	void OnTouchEnded(Touch* pTouch);
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