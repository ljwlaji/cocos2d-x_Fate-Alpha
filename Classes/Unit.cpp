#include "Unit.h"

Unit::Unit(SkeletonAnimation* _vision)
{
	initWithFile("1.png");
	autorelease();
	m_UnitVision = _vision;
	_vision->setPosition(getContentSize().width / 2, getContentSize().height / 2);
	addChild(_vision);
}

Unit::~Unit()
{
	removeAllChildrenWithCleanup(true);
}