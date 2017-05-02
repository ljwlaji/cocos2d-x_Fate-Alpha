#include "Unit.h"
#include "HelloWorldScene.h"
#include "MainMapLayer.h"
#include "MovementMgr.h"
#include "SpellMgr.h"
#include "NotifyMgr.h"
#include "Spell.h"

Unit::Unit(SkeletonAnimation* _vision, uint32 entry, uint32 guid)
{
	initWithFile("1.png");
	autorelease();
	m_PlayerTarget_Sign = nullptr;
	for (int i = Max_HP; i != UnitInt32_Value_End; i++)
		m_UnitInt32Value[(UnitInt32Value)i] = 0;
	setAnchorPoint(Vec2(0.5f, 0));
	if (entry) m_Entry = entry;
	if (guid) m_Guid = guid;
	m_IsInCombat = false;
	m_Target = nullptr;
	m_UnitVision = _vision;
	_vision->setPosition(getContentSize().width / 2, 0);
	m_Facing = Facing_Left;
	m_MoveOrgin = MoveOrgin_None;
	m_MoveType = MoveType_None;
	SetInCombat(false);
	addChild(_vision);
}

Unit::~Unit()
{
	removeAllChildrenWithCleanup(true);
}

bool Unit::IsFrendlyTo(Unit* pUnit)
{
	return sGame->GetFactionFriendly(GetFaction(), pUnit->GetFaction());
}

void Unit::SetFacing(Facing _var)
{
	if (m_Facing == _var)
		return;
	m_Facing = _var;
	_var == Facing_Left ? GetVision()->setScaleX(-1.0f) : GetVision()->setScaleX(1.0f);
}

std::string Unit::GetUnitActionStringForAction(ActionType _Typeid)
{
	std::string returnstring = "";
	switch (_Typeid)
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
		returnstring = "run";
		break;
	}
	return returnstring;
}

void Unit::DoAction(ActionType _action)
{
	std::string actionname = "";
	actionname = GetUnitActionStringForAction(_action);
	if (!actionname.empty())
		GetVision()->clearTracks();

	GetVision()->setAnimation(0, actionname.c_str(), false);

	switch (_action)
	{
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
		break;
	default:
		break;
	}
}

bool Unit::UpdateUnitValues()
{
	const ClassInfo& _info = sGame->GetUnitClassInfo(GetClass());
	if (!_info.ClassName.empty())
	{
		uint8 Level = GetLevel();
		for (int i = Max_HP; i != UnitInt32_Value_End; i++)
		{
			float CurrentNumber = 0;
			switch (i)
			{
			case Max_HP:
				CurrentNumber = _info.BaseHp + (Level * _info.ModifyHpPerLevel);
				SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
				break;
			case Base_Def:
				CurrentNumber = _info.BaseDef + (Level * _info.ModifyDefPerLevel);
				SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
				break;
			case Base_Str:
				CurrentNumber = _info.BaseStr + (Level * _info.ModifyStrPerLevel);
				SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
				break;
			case Base_Dex:
				CurrentNumber = _info.BaseDex + (Level * _info.ModifyDexPerLevel);
				SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
				break;
			case Base_Int:
				CurrentNumber = _info.BaseInt + (Level * _info.ModifyIntPerLevel);
				SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
				break;
			case Base_Att:
				SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
				break;
			}
		}



		return true;
	}
	return false;
}

Unit* Unit::SelectNearestUnit(bool SelectForTarget, bool CheckAlive)
{
	return sMainMap->GetNearestUnitForUnit(this, SelectForTarget, CheckAlive);
}

UnitSide Unit::CheckSideForUnit(const Vec2& Loc)
{
	UnitSide _side;
	getPosition().x > Loc.x ? _side = UnitSide_Left : _side = UnitSide_Right;
	return _side;
}

bool Unit::IsInAttackRange(Unit* pTarget)
{
	if (abs(getPositionX() - pTarget->getPositionX()) < 100 && abs(getPositionY() - pTarget->getPositionY()) < 100)
		return true;
	return false;
}

void Unit::CheckMoveFall()
{
	if (sMoveMgr->CanMoveTo(this, Move_To_Down, Base_Falling_Speed))
		setPositionY(getPositionY() - Base_Falling_Speed);
}

void Unit::CastSpell(uint32 spellid, Unit* pTarget)
{
	SpellInfo _info = sSpellMgr->GetSpellInfo(spellid);
	if (!_info.ID)
		return;

	//目标判断
	if (_info.SpellTargetType != SpellTargetType_Empty && !pTarget)
	{
		sNotifyMgr->ShowNotify("You Need A Target!");
		return;
	}

	if (pTarget)
	{
		switch (_info.SpellTargetType)
		{
		case SpellTargetType_Self:
			if (pTarget != this)
				return;
		case SpellTargetType_Enemy_Single:
		case SpellTargetType_Enemy_Multi:
			if (pTarget->IsFrendlyTo(this) || pTarget == this)
				return;
		case SpellTargetType_Friend_Single:
		case SpellTargetType_Friend_Multi:
			if (!pTarget->IsFrendlyTo(this))
				return;
		}

		if (_info.SpellTargetType != SpellTargetType_Self && pTarget != this)
		if (_info.SpellCastRange > getPosition().getDistance(pTarget->getPosition()))
		{
			if (ToPlayer())
				sNotifyMgr->ShowNotify("Out Of Range");
			return;
		}
	}
	//Missing 移动判断

	Spell* CasttingSpell = new Spell(this, pTarget, _info);
}