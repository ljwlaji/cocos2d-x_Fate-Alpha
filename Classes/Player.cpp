#include "Player.h"
#include "ActionManager.h"
#include "MovementMgr.h"
#include "HelloWorldScene.h"
#include "PlayerTalkLayer.h"
#include "SpellMgr.h"

static Player* _player = nullptr;

Player::Player(SkeletonAnimation* _SkeletonAnimation, CharacterEnumInfo& _info) : Unit(_SkeletonAnimation)
{
	_player = this;
	m_Player_Target = nullptr;
	SetTypeId(TYPEID_PLAYER);
	SetGuid(_info.guid);
	SetName(_info.name.c_str());
	SetClass(_info.Class);
	SetMoney(_info.Money);
	SetExp(_info.Exp);
	SetLevel(_info.Level);
	SetMapid(_info.Mapid);
	SetFaction(_info.Faction);
	SetSpeed(100);
	SetRealPosition(_info.Pos_X, _info.Pos_Y);
	setLocalZOrder(999999);
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

void Player::SetPlayerTarget(Unit* pUnit)
{
	if (!pUnit || m_Player_Target == pUnit)
		return;

	if (m_Player_Target && m_Player_Target->GetPlayerTargetSign())
		m_Player_Target->GetPlayerTargetSign()->removeFromParentAndCleanup(true);
	Sprite* TempSign = Sprite::create("Player_Target_Sign.png");
	TempSign->setPosition(pUnit->getContentSize().width / 2, 0);
	pUnit->addChild(TempSign);
	m_Player_Target = pUnit;
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

void Player::UpdateMoveStatus()
{
	if (GetCastingSpell())
		return;
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
		GetVision()->setAnimation(Track_Idle, "idle_normal", true);
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

bool Player::LoadPlayerSpells()
{
	m_Spells.clear();
	Result _Result;
	char msg[255];//			0	1
	snprintf(msg, 255, "SELECT spell,active FROM player_spells WHERE guid = %u", GetGuid());
	if (!sDataMgr->selectUnitDataList(msg, _Result))
		return false;
	else
	{
		if (_Result.empty())
			return true;
		std::vector<RowInfo> row;
		for (Result::iterator itr = _Result.begin(); itr != _Result.end(); itr++)
		{
			row = itr->second;
			PlayerSpellTemplate _template;
			_template.ID					= row.at(0).GetInt();
			_template.Active				= row.at(1).GetBool();
			m_Spells[row.at(0).GetInt()]	= _template;
		}
	}
	return true;
}

bool Player::CreatePlayer()
{
	if (LoadFromDB() && UpdateUnitValues())
		return true;

	_player = nullptr;
	removeFromParentAndCleanup(true);
	return false;
}

bool Player::HasSpell(uint32 spellid)
{
	if (m_Spells.find(spellid) != m_Spells.end())
		return true;
	return false;
}

void Player::CheckPlayerSpellActive()
{
	for (PlayerSpells::iterator itr = m_Spells.begin(); itr != m_Spells.end(); itr++)
	{
		SpellInfo info = sSpellMgr->GetSpellInfo(itr->first);
		if (!info.ID)
			continue;
		uint32 NextLevelSpellID = sSpellMgr->GetSpellNextLevelID(info.ID, info.BaseLevelSpellID);

		if (!NextLevelSpellID || !HasSpell(NextLevelSpellID))
			itr->second.Active = true;
		else
			itr->second.Active = false;
	}
}

bool Player::LoadFromDB()
{
	if (!LoadPlayerSpells())
		return false;
	CheckPlayerSpellActive();
	return true;
}

void Player::SaveToDB()
{

}