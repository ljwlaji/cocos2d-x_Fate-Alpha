#ifndef __VIRTUAL_ROCKER_H__
#define __VIRTUAL_ROCKER_H__

#include "cocos2d.h"
USING_NS_CC;

#define sRocker VirtualRocker::GetInstance()
class VirtualRocker : public Sprite
{
public:
	static VirtualRocker* GetInstance();
	void TouchBegin(Touch* loc);
	void TouchMoved(Touch* loc);
	void TouchEnded(Touch* loc);
private:
	Vec2 MoveVec;
	bool NeedUpdate;
	virtual void update(float diff);
	Point TouchLastPosition;
	Point CurrentPosition;
	VirtualRocker();
	~VirtualRocker();
	Sprite* RockerController;
	float GetVirtualRokerOrgin(const Vec2& CenterPoint, const Vec2& RokerPoint);
	void ResetVirtualRokerOrgin(const float& Orgin);
};

#endif