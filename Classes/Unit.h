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
	void SetFacing(Facing _var);
	MoveType GetMoveType()									{ return m_MoveType; }
	uint8 GetSpeed()										{ return m_Speed; }
	MoveOrientation GetMoveOrgin()							{ return m_MoveOrgin; }
	virtual SkeletonAnimation* GetVision() final			{ return m_UnitVision; }
	Facing GetFacing()										{ return m_Facing; }
	void SetMoving(MoveOrientation _var)					{ m_MoveOrgin = _var; }
	void SetSpeed(uint8 _var)								{ m_Speed = _var; }
	void SetMoveType(MoveType _var)							{ m_MoveType = _var; }
private:
	SkeletonAnimation* m_UnitVision;
	Facing m_Facing;
	MoveOrientation m_MoveOrgin;
	uint8 m_Speed;
	virtual void UpdateMoveStatus() = 0;
	MoveType m_MoveType;
};

#endif