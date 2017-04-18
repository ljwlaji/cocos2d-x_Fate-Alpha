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

enum VirtualRockerOrginType
{
	Roker_Up,
	Roker_Up_Left,
	Roker_Up_Right,
	Roker_Left,
	Roker_Left_Down,
	Roker_Down,
	Roker_Down_Right,
	Roker_Right,
};
class ActionMgr;

#define sPlayer Player::GetInstance()

class Player : public Unit
{
public:
	explicit Player(SkeletonAnimation* _SkeletonAnimation, uint32 guid);
	~Player();

	static Player* GetInstance();
	void DoAction(ActionType _action);
	void DealVirtualRoker(VirtualRockerOrginType _VirtualRockerOrginType);
	void ResetMoveKeyForRoker();
	bool CanCancelActionForMove();
	void SaveToDB();
	bool UpdatePlayerValues();
	ActionType GetDoingAction()									{ return m_Action; }
	ActionMgr* PlayerActionMgr()								{ return _ActionMgr; }
	void SetMoveKeyEnable(MoveKeyValue _key, bool enable)		{ MoveKeyStatus[_key] = enable; }
	void ResetKeyTimer()										{ KeyVectorClearTimer = Base_Clear_Key_Time; }
	virtual void DestorySelf()									{ removeFromParentAndCleanup(true); }
private:
	ActionType m_Action;
	virtual void update(float diff);
	ActionMgr* _ActionMgr;
	std::map<MoveKeyValue, bool> MoveKeyStatus;
	virtual void UpdateMoveStatus();
	float KeyVectorClearTimer;
	virtual bool LoadFromDB();
};

#endif