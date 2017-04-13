#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Unit.h"
#include "DataMgr.h"
enum MoveKeyValue
{
	MoveKey_Left,
	MoveKey_Right,
	MoveKey_Up,
	MoveKey_Down,
	MoveKey_Endl,
};

class ActionMgr;

#define sPlayer Player::GetInstance()

class Player : public Unit
{
public:
	explicit Player(SkeletonAnimation* _SkeletonAnimation);
	~Player();

	static Player* GetInstance();
	void DoAction(ActionType _action);
	bool CanCancelActionForMove();
	uint8 GetLevel()											{ return m_Level; }
	ActionType GetDoingAction()									{ return m_Action; }
	ActionMgr* PlayerActionMgr()								{ return _ActionMgr; }
	uint8 GetClass()											{ return m_Class; }
	void SetMoveKeyEnable(MoveKeyValue _key, bool enable)		{ MoveKeyStatus[_key] = enable; }
	void ResetKeyTimer()										{ KeyVectorClearTimer = Base_Clear_Key_Time; }
private:
	uint8 m_Level;
	ActionType m_Action;
	virtual void update(float diff);
	ActionMgr* _ActionMgr;
	uint8 m_Class;
	std::map<MoveKeyValue, bool> MoveKeyStatus;
	virtual void UpdateMoveStatus();
	float KeyVectorClearTimer;
};

#endif