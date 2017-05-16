#ifndef __UNIT_H__
#define __UNIT_H__

#include "Types.h"
#include "cocos2d.h"
#include "SpellMgr.h"
#ifdef WIN32
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"
#else
#include "../cocos2d/cocos/editor-support/spine/spine.h"
#include "../cocos2d/cocos/editor-support/spine/SkeletonAnimation.h"
#endif

USING_NS_CC;

enum UnitInt32Value
{
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
};
enum UnitDeathStatus
{
	Dead,
	Alive,
};

using namespace spine;

class Creature;
class Monster;
class Player;
class Npc;
class Spell;
class Unit : public Sprite
{
public:
	Unit(SkeletonAnimation* UnitVision, uint32 entry = 0, uint32 guid = 0);
	~Unit();

	Player* ToPlayer()										{ if (GetTypeId() == TYPEID_PLAYER) return reinterpret_cast<Player*>(this); else return nullptr; }
	Creature* ToCreature()									{ if (GetTypeId() == TYPEID_NPC || GetTypeId() == TYPEID_MONSTER) return reinterpret_cast<Creature*>(this); else return nullptr; }
	Monster* ToMonster()									{ if (GetTypeId() == TYPEID_MONSTER) return reinterpret_cast<Monster*>(this); else return nullptr; }
	Npc* ToNpc()											{ if (GetTypeId() == TYPEID_NPC) return reinterpret_cast<Npc*>(this); else return nullptr; }
	float GetPositionX()									{ return getBoundingBox().origin.x + (getBoundingBox().size.width / 2); }
	float GetPositionZ()									{ return getBoundingBox().origin.y; }
	//由Jump原点获取
	//Combat
public:
	Unit* SelectNearestUnit(bool SelectForTarget = true, bool CheckAlive = true);
	void CheckMoveFall();
	bool IsInCombatWith(Unit* pUnit);
private:
	std::string GetUnitActionStringForAction(ActionType _Typeid);
	void DoAction(ActionType _action);
	void ShowDamageImage(int32 DamageNumber, bool IsDamage = true);
	void _ShowDamage(std::list<Sprite*>& SpriteList);
	void DestoryNumberCallBack(Sprite* pSprite);
	//End Of Combat

public:
	void CastSpell(uint32 _SpellID, Unit* pTarget = nullptr);
	bool UpdateUnitValues();
	float GetPositionY();
	void SetFacing(Facing _var);
	bool IsFrendlyTo(Unit* pUnit);
	uint32 GetBaseAttack();
	UnitDeathStatus IsAlive()								{ return m_DeathStatus; }
	void SetDeathStatus(UnitDeathStatus _status)			{ m_DeathStatus = _status; }
	int32 GetLevel()										{ return GetUnitInt32Value(UnitValue_Level); }
	UnitClasses GetClass()									{ return m_Class; }
	MoveType GetMoveType()									{ return m_MoveType; }
	uint8 GetSpeed()										{ return m_Speed; }
	MoveOrientation GetMoveOrgin()							{ return m_MoveOrgin; }
	virtual SkeletonAnimation* GetVision() final			{ return m_UnitVision; }
	Facing GetFacing()										{ return m_Facing; }
	int32 GetUnitInt32Value(UnitInt32Value _UnitInt32Value)	{ return m_UnitInt32Value[_UnitInt32Value]; }
	uint32 GetGuid()										{ return m_Guid; }
	std::string GetName()									{ return m_Name; }
	bool IsInCombat()										{ return m_IsInCombat; }
	int32 GetFaction()										{ return GetUnitInt32Value(Faction); }
	TypeID GetTypeId()										{ return m_TypeId; }
	Unit* UpdateVictim()									{ return m_Target; }
	Sprite* GetPlayerTargetSign()							{ return m_PlayerTarget_Sign; }
	Spell* GetCastingSpell()								{ return m_Castting_Spell; }
	void DestoryVision()									{ if (m_UnitVision) m_UnitVision->removeFromParentAndCleanup(true); m_UnitVision = nullptr; }
	void ResetPlayerTargetSign()							{ m_PlayerTarget_Sign = nullptr; }
	void SetPlayerTargetSign(Sprite* sign)					{ m_PlayerTarget_Sign = sign; }
	void SetCastingSpell(Spell* pSpell)						{ m_Castting_Spell = pSpell; }
	void SetTypeId(TypeID _var)								{ m_TypeId = _var; }
	void SetFaction(uint32 faction)							{ SetUnitInt32Value(Faction, faction); }
	void SetInCombat(bool _var);
	void SetLevel(uint8 _var)								{ SetUnitInt32Value(UnitValue_Level, _var); }
	void SetName(std::string _var)							{ m_Name = _var; }
	void SetClass(UnitClasses _var)							{ m_Class = _var; }
	void SetGuid(uint32 _var)								{ m_Guid = _var; }
	void SetUnitInt32Value(UnitInt32Value val, int _var)	{ m_UnitInt32Value[val] = _var; }
	void SetMoving(MoveOrientation _var)					{ m_MoveOrgin = _var; }
	void SetSpeed(uint8 _var)								{ m_Speed = _var; }
	void SetMoveType(MoveType _var)							{ m_MoveType = _var; }
	void SetTarget(Unit* pTarget)							{ m_Target = pTarget; }
	bool IsInAttackRange(Unit* pTarget);
	UnitSide CheckSideForUnit(const Vec2& Loc);
	virtual void DestorySelf() = 0;
	void DealSpellDamage(Unit* pTarget, SpellEffectType type, int32& damage);
	void JustDead(Unit* pKiller);
private:
	SkeletonAnimation* m_UnitVision;
	Facing m_Facing;
	MoveOrientation m_MoveOrgin;
	uint8 m_Speed;
	virtual void UpdateMoveStatus() = 0;
	virtual bool LoadFromDB() = 0;
	MoveType m_MoveType;
	UnitDeathStatus m_DeathStatus;
	UnitClasses m_Class;
	std::map<UnitInt32Value, int32> m_UnitInt32Value;
	uint32 m_Entry;
	uint32 m_Guid;
	std::string m_Name;
	bool m_IsInCombat;
	TypeID m_TypeId;
	Sprite* m_PlayerTarget_Sign;
	Unit* m_Target;
	Spell* m_Castting_Spell;
};

#endif