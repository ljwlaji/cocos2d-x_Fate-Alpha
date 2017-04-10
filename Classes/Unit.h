#ifndef __UNIT_H__
#define __UNIT_H__

#include "Types.h"
#include "cocos2d.h"
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"

USING_NS_CC;

using namespace spine;
class Unit : public Sprite
{
public:
	Unit(SkeletonAnimation* UnitVision);
	~Unit();

	float GetPositionX() { return getBoundingBox().origin.x + (getBoundingBox().size.width / 2); }
	float GetPositionZ() { return getBoundingBox().origin.y; }
	//由Jump原点获取
	float GetPositionY();
	virtual SkeletonAnimation* GetVision() final	{ return m_UnitVision; }

private:
	SkeletonAnimation* m_UnitVision;





};

#endif