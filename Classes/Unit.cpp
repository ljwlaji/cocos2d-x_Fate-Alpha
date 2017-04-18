#include "Unit.h"
#include "HelloWorldScene.h"
Unit::Unit(SkeletonAnimation* _vision, uint32 entry, uint32 guid)
{
	initWithFile("1.png");
	autorelease();
	if (entry) m_Entry = entry;
	if (guid) m_Guid = guid;

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

bool Unit::UpdateUnitValues()
{
	ClassInfo _info = sGame->GetUnitClassInfo(GetClass());
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
				break;
			case Base_Att:
				break;
			case Base_Def:
				CurrentNumber = _info.BaseDef + (Level * _info.ModifyDefPerLevel);
				break;
			case Base_Str:
				CurrentNumber = _info.BaseStr + (Level * _info.ModifyStrPerLevel);
				break;
			case Base_Dex:
				CurrentNumber = _info.BaseDex + (Level * _info.ModifyDexPerLevel);
				break;
			case Base_Int:
				CurrentNumber = _info.BaseInt + (Level * _info.ModifyIntPerLevel);
				break;
			}
			SetUnitInt32Value((UnitInt32Value)i, CurrentNumber);
		}



		return true;
	}
	return false;
}