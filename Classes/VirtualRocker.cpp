#include "VirtualRocker.h"
#include "Player.h"

static VirtualRocker* _VirtualRocker = nullptr;
VirtualRocker::VirtualRocker()
{
	initWithFile("Virtual_Roker_Back.png");
	autorelease();

	RockerController = Sprite::create("Virtual_Roker_Center.png");
	RockerController->setPosition(getContentSize().width / 2, getContentSize().height / 2);
	addChild(RockerController);
	CurrentPosition = RockerController->getPosition();
	NeedUpdate = false;
	SetTouchType(PlayerUITouch_Roker);
	scheduleUpdate();
}

VirtualRocker::~VirtualRocker()
{
	_VirtualRocker = nullptr;
	removeAllChildrenWithCleanup(true);
}
VirtualRocker* VirtualRocker::GetInstance()
{
	if (!_VirtualRocker)
		_VirtualRocker = new VirtualRocker();

	return _VirtualRocker;
}

bool VirtualRocker::OnUITouchBegin(Touch* loc)
{
	if (IsContectPoint(loc->getLocation()))
	{
		TouchLastPosition = loc->getLocation();
		return true;
	}
	return false;
}

void VirtualRocker::OnUITouchMoved(Touch* loc)
{
	if (!NeedUpdate)
	{
		MoveVec = TouchLastPosition - loc->getLocation();
		NeedUpdate = true;
	}
	TouchLastPosition = loc->getLocation();
}

void VirtualRocker::OnUITouchEnded(Touch* /*loc*/)
{
	sPlayer->ResetMoveKeyForRoker();
	RockerController->setPosition(getContentSize().width / 2, getContentSize().height / 2);
	CurrentPosition = getPosition();
	TouchLastPosition = Vec2(0, 0);
	NeedUpdate = false;
}

float VirtualRocker::GetVirtualRokerOrgin(const Vec2& CenterPoint, const Vec2& RokerPoint)
{
	float Correct_Orgin = 0;
	Correct_Orgin = atan2(abs(RokerPoint.y - CenterPoint.y),abs(RokerPoint.x - CenterPoint.x)) * (180.0f / 3.14f);
	if (RokerPoint.x >= CenterPoint.x)
	{
		RokerPoint.y >= CenterPoint.y ? Correct_Orgin += 90.0f : Correct_Orgin = 90.0f - Correct_Orgin;
		Correct_Orgin += 180.0f;
	}
	else if (RokerPoint.y > CenterPoint.y)
	{
		Correct_Orgin = 90.0f - Correct_Orgin;
	}
	else
		Correct_Orgin += 90.0f;

	return Correct_Orgin;
}


void VirtualRocker::ResetVirtualRokerOrgin(const float& _orgin)
{
	if (_orgin > 22.5 && _orgin <= 135)
	{
		if (abs(_orgin - 90) < 15)
			sPlayer->DealVirtualRoker(Roker_Left);
		else
			_orgin > 90 ? sPlayer->DealVirtualRoker(Roker_Left_Down) : sPlayer->DealVirtualRoker(Roker_Up_Left);
	}
	else if (_orgin > 135 && _orgin < 225)
	{
		if (abs(_orgin - 180) < 15)
			sPlayer->DealVirtualRoker(Roker_Down);
		else _orgin > 180 ? sPlayer->DealVirtualRoker(Roker_Down_Right) : sPlayer->DealVirtualRoker(Roker_Left_Down);
	}
	else if (_orgin > 225 && _orgin < 315)
	{
		if (abs(_orgin - 270) < 15)
			sPlayer->DealVirtualRoker(Roker_Right);
		else _orgin > 280 ? sPlayer->DealVirtualRoker(Roker_Up_Right) : sPlayer->DealVirtualRoker(Roker_Down_Right);
	}
	else
	{
		if (_orgin < 15 || _orgin > 345)
			sPlayer->DealVirtualRoker(Roker_Up);
		else if (_orgin < 45)
			sPlayer->DealVirtualRoker(Roker_Up_Left);
		else
			sPlayer->DealVirtualRoker(Roker_Up_Right);
	}
}

void VirtualRocker::update(float /*diff*/)
{
	if (!NeedUpdate)
		return;
	if ((RockerController->getPosition() - MoveVec).getDistance(getPosition()) < 140.0f)
	{
		RockerController->setPosition(RockerController->getPosition() - MoveVec);
		ResetVirtualRokerOrgin(GetVirtualRokerOrgin(getPosition(), RockerController->getPosition()));
	}
	NeedUpdate = false;
}