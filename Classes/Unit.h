#ifndef __UNIT_H__
#define __UNIT_H__

#include "Types.h"
#include "cocos2d.h"
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
	Max_Rage,
	Curr_Rage,
	Base_Def,
	Base_Str,
	Base_Dex,
	Base_Int,
	Faction,
	Level,
	Base_Att,
	UnitInt32_Value_End,
};
enum UnitDeathStatus
{
	Alive,
	Dead,
};
using namespace spine;


class Unit : public Sprite
{
public:
	Unit(SkeletonAnimation* UnitVision, uint32 entry = 0, uint32 guid = 0);
	~Unit();

	float GetPositionX() { return getBoundingBox().origin.x + (getBoundingBox().size.width / 2); }
	float GetPositionZ() { return getBoundingBox().origin.y; }
	//由Jump原点获取
	bool UpdateUnitValues();
	float GetPositionY();
	void SetFacing(Facing _var);
	uint8 GetLevel()										{ return m_Level; }
	UnitClasses GetClass()									{ return m_Class; }
	MoveType GetMoveType()									{ return m_MoveType; }
	uint8 GetSpeed()										{ return m_Speed; }
	MoveOrientation GetMoveOrgin()							{ return m_MoveOrgin; }
	virtual SkeletonAnimation* GetVision() final			{ return m_UnitVision; }
	Facing GetFacing()										{ return m_Facing; }
	int32 GetUnitInt32Value(UnitInt32Value _UnitInt32Value)	{ return m_UnitInt32Value[_UnitInt32Value]; }
	uint32 GetGuid()										{ return m_Guid; }
	std::string GetName()									{ return m_Name; }
	void SetLevel(uint8 _var)								{ m_Level = _var; }
	void SetName(std::string _var)							{ m_Name = _var; }
	void SetClass(UnitClasses _var)							{ m_Class = _var; }
	void SetGuid(uint32 _var)								{ m_Guid = _var; }
	void SetUnitInt32Value(UnitInt32Value val, int _var)	{ m_UnitInt32Value[val] = _var; }
	void SetMoving(MoveOrientation _var)					{ m_MoveOrgin = _var; }
	void SetSpeed(uint8 _var)								{ m_Speed = _var; }
	void SetMoveType(MoveType _var)							{ m_MoveType = _var; }
	virtual void DestorySelf() = 0;
private:
	SkeletonAnimation* m_UnitVision;
	Facing m_Facing;
	MoveOrientation m_MoveOrgin;
	uint8 m_Speed;
	virtual void UpdateMoveStatus() = 0;
	virtual bool LoadFromDB() = 0;
	MoveType m_MoveType;

	uint8 m_Level;
	UnitClasses m_Class;
	std::map<UnitInt32Value, int32> m_UnitInt32Value;
	uint32 m_Entry;
	uint32 m_Guid;
	std::string m_Name;
};

#endif