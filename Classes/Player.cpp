#include "Player.h"
#include "ActionManager.h"
Player::Player(SkeletonAnimation* _SkeletonAnimation) : Unit(_SkeletonAnimation)
{
	m_Class = 1;
	ActionMgr* _mgr = new ActionMgr(this);
	_ActionMgr = _mgr;
	for (int i = MoveKey_Left; i != MoveKey_Endl; i++)
		MoveKeyStatus[(MoveKeyValue)i] = false;

	KeyVectorClearTimer = Base_Clear_Key_Time;
	scheduleUpdate();
}

Player::~Player()
{
}

void Player::DoAction(ActionType _action)
{
	GetVision()->clearTracks();
	switch (_action)
	{
	case Action_None:
		break;
	case Normal_Attack_0:
		break;
	case Normal_Attack_1:
		break;
	case Normal_Attack_2:
		break;
	case Normal_Attack_3:
		break;
	case Sprint_Attack:
		break;
	case Air_Sprint:
		break;
	case Move_Run_Right:
	case Move_Run_Left:
		SetMoveType(MoveType_Run);
		_action == Move_Run_Right ? SetFacing(Facing_Right) : SetFacing(Facing_Left);
		GetVision()->setAnimation(0, "run", true);
		break;
	default:
		break;
	}
}

void Player::UpdateMoveStatus()
{
	bool NeedResetMoveStatus = true;
	for (int i = MoveKey_Left; i < MoveKey_Endl; i++)
	{
		if (MoveKeyStatus[(MoveKeyValue)i])
			NeedResetMoveStatus = false;
	}
	if (NeedResetMoveStatus && GetMoveType() != MoveType_None)
	{
		SetMoveType(MoveType_None);
		GetVision()->clearTracks();
		GetVision()->setAnimation(Track_Idle, "idle", true);
		return;
	}
	//Calc
	float X_Modify = 0;
	float Y_Modify = 0;
	float Modify_Speed = (float)GetSpeed() / 100.0f;
	if (MoveKeyStatus[MoveKey_Left]) 
		X_Modify -= Base_X_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Right])
		X_Modify += Base_X_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Up])
		Y_Modify += Base_Y_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Down])
		Y_Modify -= Base_Y_MovePoint * Modify_Speed;

	if (X_Modify)
		X_Modify > 0 ? SetFacing(Facing_Right) : SetFacing(Facing_Left);
	if (GetMoveType() == MoveType_Run)
	{
		X_Modify = X_Modify * 1.5f;
		Y_Modify = Y_Modify * 1.5f;
	}
	else
	{
		if (X_Modify && GetMoveType() != MoveType_Walk)
		{
			SetMoveType(MoveType_Walk);
			GetVision()->clearTracks();
			GetVision()->setAnimation(0, "walk", true);
		}
	}
	//Result
	setPosition(getPositionX() + X_Modify, getPositionY() + Y_Modify);
}

void Player::update(float diff)
{
	UpdateMoveStatus();


	//Clear Key
	if (KeyVectorClearTimer <= diff)
	{
		PlayerActionMgr()->ClearKey();
		KeyVectorClearTimer = Base_Clear_Key_Time;
	}
	else KeyVectorClearTimer -= diff;
}

bool Player::CanCancelActionForMove()
{
	switch (GetDoingAction())
	{
	case Action_None:
		return true;
	case Normal_Attack_0:
	case Normal_Attack_1:
	case Normal_Attack_2:
	case Normal_Attack_3:
	{
		if (GetLevel() > 5)
			return false;
		return true;
	}
	case Move_Run_Right:
	case Move_Run_Left:
		return true;
	default:
		return false;
	}
}