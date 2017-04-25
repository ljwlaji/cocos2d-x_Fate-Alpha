#include "Unit.h"
#include "HelloWorldScene.h"
#include "MainMapLayer.h"
Unit::Unit(SkeletonAnimation* _vision, uint32 entry, uint32 guid)
{
	initWithFile("1.png");
	autorelease();
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
	if (getPositionX() - pTarget->getPositionX() < 100 && abs(getPositionY() - pTarget->getPositionY()) < 100)
		return true;
	return false;
}