#include "Unit.h"
#include "HelloWorldScene.h"
#include "MainMapLayer.h"
#include "MovementMgr.h"
#include "SpellMgr.h"
#include "NotifyMgr.h"
#include "Spell.h"
#include "Creature.h"
#include "Loot.h"
#include "LootingSprite.h"

Unit::Unit(SkeletonAnimation* _vision, uint32 entry, uint32 guid)
{
	initWithFile("1.png");
	autorelease();
	m_Castting_Spell = nullptr;
	m_PlayerTarget_Sign = nullptr;
	m_DeathStatus = Alive;
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
	if (m_Castting_Spell)
		delete m_Castting_Spell;

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

bool Unit::IsInCombatWith(Unit* pUnit)
{
	if (ToCreature() && ToCreature()->IsAlive() && ToCreature()->IsInThreatList(pUnit))
		return true;
	else if (ToPlayer() && ToPlayer()->IsUnitInCombatList(pUnit))
		return true;
	return false;
}

void Unit::JustDead(Unit* pKiller)
{
	m_DeathStatus = Dead;
	GetVision()->addAnimation(0, "just_dead", false);
	SetUnitInt32Value(Curr_HP, 0);

	if (ToPlayer())
		ToPlayer()->JustDead();

	if (ToCreature() && pKiller->ToPlayer())
	{
		Loot* TempLoot = Loot::CreateLoot(ToCreature());
		Player* pPlayer = pKiller->ToPlayer();
		uint32 exp = GetLevel() * 10;
		pPlayer->AddExp(exp);
	}
}

void Unit::DealSpellDamage(Unit* pTarget, SpellEffectType type, int32& damage)
{
	if (pTarget->ToCreature() && !pTarget->IsInCombatWith(this))
		pTarget->ToCreature()->CombatStart(this);
	damage += ToPlayer() ? ToPlayer()->GetPlayerTotalInt32Value(Base_Att) + ToPlayer()->GetItemTotalAttack() : GetUnitInt32Value(Base_Att);
	damage -= pTarget->GetUnitInt32Value(Base_Def);

	if (damage < 0)
		damage = 0;

	pTarget->ShowDamageImage(damage);
	if (damage >= pTarget->GetUnitInt32Value(Curr_HP))
	{
		pTarget->JustDead(this);
		return;
	}
	pTarget->SetUnitInt32Value(Curr_HP, pTarget->GetUnitInt32Value(Curr_HP) - damage);
	pTarget->GetVision()->setAnimation(0, "take_damage", false);
}

void Unit::ShowDamageImage(int32 DamageNumber ,bool IsDamage)
{
	std::list<int> numberVector;
	std::list<Sprite*> SpriteVector;
	if (!DamageNumber)
	{
		Sprite* TempSprite = Sprite::create("number_0.png");
		SpriteVector.push_back(TempSprite);
	}
	while (DamageNumber)
	{
		int temp = DamageNumber % 10;
		numberVector.push_back(temp);
		DamageNumber = DamageNumber / 10;
	}

	while (numberVector.size())
	{
		std::list<int>::iterator itr = numberVector.begin();
		char msg[255];
		snprintf(msg, 255, IsDamage ? DamageTextUrl : HealingTextUrl, *itr);
		Sprite* TempSprite = Sprite::create(msg);
		SpriteVector.push_front(TempSprite);
		numberVector.pop_front();
	}
	_ShowDamage(SpriteVector);
}

void Unit::_ShowDamage(std::list<Sprite*>& SpriteList)
{
	float FirstPosX = 0;
	float SingleWidth = (*SpriteList.begin())->getBoundingBox().size.width;
	float TotalWidth = SingleWidth + ((SpriteList.size() - 1) * (SingleWidth / 2));

	int k = 0;
	for (std::list<Sprite*>::iterator itr = SpriteList.begin(); itr != SpriteList.end(); itr++, k++)
	{
		float posx = getContentSize().width / 2 - (TotalWidth / 2) + (k * SingleWidth / 2);
		(*itr)->setAnchorPoint(Vec2(0, 0.5f));
		(*itr)->setPosition(posx, getContentSize().height / 2);
		addChild(*itr);
		(*itr)->runAction(MoveTo::create(1.0f, Vec2((*itr)->getPositionX(), (*itr)->getPositionY() + (*itr)->getBoundingBox().size.height * 5)));
		Sequence* Sq = Sequence::create(FadeOut::create(1.0f), DelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(Unit::DestoryNumberCallBack, this, (*itr))), NULL);
		(*itr)->runAction(Sq);
	}
}

void Unit::DestoryNumberCallBack(Sprite* pSprite)
{
	pSprite->removeFromParentAndCleanup(true);
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

uint32 Unit::GetBaseAttack()
{
	/*
	Max_HP,
	Curr_HP,
	Max_Mana,
	Curr_Mana,
	Base_Def,
	Base_Str,
	Base_Dex,
	Base_Int,
	Faction,
	UnitValue_Level,
	Base_Att,
	UnitInt32_Value_End,
	*/
	uint32 _attack = 0;
	switch (GetClass())
	{
	case Saber:
		_attack = m_UnitInt32Value[Base_Str] + m_UnitInt32Value[Base_Dex] / 4;
		break;
	case Archer:
		_attack = m_UnitInt32Value[Base_Str] / 4 + m_UnitInt32Value[Base_Dex];
		break;
	case Caster:
		_attack = m_UnitInt32Value[Base_Dex] / 4 + m_UnitInt32Value[Base_Int];
		break;
	case Lancer:
		_attack = m_UnitInt32Value[Base_Str] / 2 + m_UnitInt32Value[Base_Dex] / 2;
		break;
	case Assasin:
		_attack = m_UnitInt32Value[Base_Int] / 4 + m_UnitInt32Value[Base_Dex];
		break;
	case Rider:
		_attack = m_UnitInt32Value[Base_Str] / 3 + m_UnitInt32Value[Base_Dex] + m_UnitInt32Value[Base_Int] / 3;
		break;
	case Avenger:
		_attack = m_UnitInt32Value[Base_Dex] / 4 + m_UnitInt32Value[Base_Str] / 4 + m_UnitInt32Value[Base_Dex] / 4 + m_UnitInt32Value[Base_Int] / 4;
		break;
	case Berserker:
		_attack = m_UnitInt32Value[Base_Str] * 2;
		break;
	}

	return _attack;
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
				CurrentNumber = GetBaseAttack();
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
	if (abs(getPositionX() - pTarget->getPositionX()) < 100 && abs(getPositionY() - pTarget->getPositionY()) < sMainMap->GetVisableSize().y * 0.02f)
		return true;
	return false;
}

void Unit::SetInCombat(bool _var)
{
	if (ToPlayer())
		sNotifyMgr->ShowNotify(_var ? "Combat Start!" : "Combat Ended!");
	m_IsInCombat = _var; 
}

void Unit::CheckMoveFall()
{
	//if (sMoveMgr->CanMoveTo(this, Move_To_Down, Base_Falling_Speed))
	//	setPositionY(getPositionY() - Base_Falling_Speed);
}

void Unit::CastSpell(uint32 spellid, Unit* pTarget)
{
	if (m_Castting_Spell)
	{
		sNotifyMgr->ShowNotify("An Other Spell Is Casting!");
		return;
	}
	SpellInfo _info = sSpellMgr->GetSpellInfo(spellid);
	if (!_info.ID)
		return;

	//目标判断
	if (_info.Spell_Target_Type != SpellTargetType_Empty && !pTarget)
	{
		sNotifyMgr->ShowNotify("You Need A Target!");
		return;
	}

	if (pTarget)
	{
		switch (_info.Spell_Target_Type)
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

		if (_info.Spell_Target_Type != SpellTargetType_Self && pTarget != this)
		if (getPosition().getDistance(pTarget->getPosition()) > _info.SpellCastRange)
		{
			if (ToPlayer())
				sNotifyMgr->ShowNotify("Out Of Range");
			return;
		}
	}
	//Missing 移动判断

	Spell* CasttingSpell = new Spell(this, pTarget, _info);
	m_Castting_Spell = CasttingSpell;
}