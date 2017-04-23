#include "Player.h"
#include "ActionManager.h"
#include "MovementMgr.h"
#include "HelloWorldScene.h"
#include "PlayerTalkLayer.h"
static Player* _player = nullptr;

Player::Player(SkeletonAnimation* _SkeletonAnimation, CharacterEnumInfo& _info) : Unit(_SkeletonAnimation)
{
	_player = this;
	SetName(_info.name.c_str());
	SetClass(_info.Class);
	SetMoney(_info.Money);
	SetExp(_info.Exp);
	SetLevel(_info.Level);
	SetMapid(_info.Mapid);
	SetSpeed(100);
	SetRealPosition(_info.Pos_X, _info.Pos_Y);
	setZOrder(999999);
	ActionMgr* _mgr = new ActionMgr(this);
	_ActionMgr = _mgr;
	for (int i = MoveKey_Left; i != MoveKey_Endl; i++)
		MoveKeyStatus[(MoveKeyValue)i] = false;
	KeyVectorClearTimer = Base_Clear_Key_Time;
	PlayerTalkClass = new TalkClass();
	PlayerTalkClass->ClearMenu();
	scheduleUpdate();
}

Player::~Player()
{
	if (_ActionMgr)
		delete _ActionMgr;
}

Player* Player::GetInstance()
{
	if (!_player)
		return nullptr;

	return _player;
}

void Player::CloseGossipMenu()
{
	PlayerTalkClass->ClearMenu();
	if (sPlayerTalkLayer)
	{
		sPlayerTalkLayer->CloseMenuWithCleanUp(true);
	}
}

void Player::SendGossipMenu(std::string Main_String, Creature* pCreature)
{
	if (sPlayerTalkLayer)
	{
		sPlayerTalkLayer->SendMenuToPlayer(Main_String);
		PlayerTalkClass->TalkingCreature = pCreature;
	}
}

void Player::ResetMoveKeyForRoker()
{
	for (int i = 0; i != MoveKey_Endl; i++)
		MoveKeyStatus[(MoveKeyValue)i] = false;
}
void Player::DealVirtualRoker(VirtualRockerOrginType _VirtualRockerOrginType)
{
	switch (_VirtualRockerOrginType)
	{
	case Roker_Up:
		for (int i = 0; i != MoveKey_Endl; i++)
			i == MoveKey_Up ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Up_Left:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Up || i == MoveKey_Left) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Up_Right:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Up || i == MoveKey_Right) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Left:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Left) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Left_Down:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Down || i == MoveKey_Left) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Down:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Down) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Down_Right:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Down || i == MoveKey_Right) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Right:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Right) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	}

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
	//log("speed %f", GetSpeed());
	if (MoveKeyStatus[MoveKey_Left]) 
		X_Modify -= Base_X_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Right])
		X_Modify += Base_X_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Up])
		Y_Modify += Base_Y_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Down])
		Y_Modify -= Base_Y_MovePoint * Modify_Speed;

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

	CheckMoveTo X_MoveFront;
	CheckMoveTo Y_MoveFront;
	float CheckPosX = 0;
	float CheckPosY = 0;
	if (X_Modify)
	{
		if (X_Modify > 0)
		{
			SetFacing(Facing_Right);
			X_MoveFront = Move_To_Right;
		}
		else
		{
			SetFacing(Facing_Left);
			X_MoveFront = Move_To_Left;
		}
		if (sMoveMgr->CanMoveTo(this,X_MoveFront, abs(X_Modify)))
			setPositionX(getPositionX() + X_Modify);
	}

	if (Y_Modify)
	{
		Y_Modify > 0 ? Y_MoveFront = Move_To_Up : Y_MoveFront = Move_To_Down;
		if (sMoveMgr->CanMoveTo(this,Y_MoveFront, abs(Y_Modify)))
			setPositionY(getPositionY() + Y_Modify);
	}
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

bool Player::CreatePlayer()
{
	if (LoadFromDB() && UpdateUnitValues())
		return true;

	_player = nullptr;
	removeFromParentAndCleanup(true);
	return false;
}

bool Player::LoadFromDB()
{
	return true;
	//Result _Result;
	//char msg[255];//			0	1		2	3	  4		5		6	7
	//snprintf(msg, 255, "SELECT name,Class,Money,Exp,Level,Mapid,Pos_X,Pos_Y FROM characters WHERE guid = %u", GetGuid());
	//if (!sDataMgr->selectUnitDataList(msg, _Result))
	//	return false;
	//else
	//{
	//	if (_Result.empty())
	//		return false;
	//
	//	std::vector<RowInfo> r_inf = _Result.begin()->second;
	//	SetName(r_inf.at(0).GetString().c_str());
	//	SetClass((UnitClasses)r_inf.at(1).GetInt());
	//	SetMoney(r_inf.at(2).GetInt());
	//	SetExp(r_inf.at(3).GetInt());
	//	SetLevel(r_inf.at(4).GetInt());
	//	SetMapid(r_inf.at(5).GetInt());
	//	SetRealPosition(r_inf.at(6).GetFloat(), r_inf.at(7).GetFloat());
	//	return true;
	//}
}

void Player::SaveToDB()
{

}