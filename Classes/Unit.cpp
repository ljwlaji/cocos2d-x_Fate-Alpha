#include "Unit.h"

Unit::Unit(SkeletonAnimation* _vision)
{
	initWithFile("1.png");
	autorelease();
	m_UnitVision = _vision;
	_vision->setPosition(getContentSize().width / 2, getContentSize().height / 2);
	m_Facing = Facing_Left;
	m_MoveOrgin = MoveOrgin_None;
	m_MoveType = MoveType_None;
	addChild(_vision);
}

Unit::~Unit()
{
	removeAllChildrenWithCleanup(true);
}

void Unit::SetFacing(Facing _var)
{
	if (m_Facing == _var)
		return;
	m_Facing = _var;
	_var == Facing_Left ? GetVision()->setScaleX(-1.0f) : GetVision()->setScaleX(1.0f);
}