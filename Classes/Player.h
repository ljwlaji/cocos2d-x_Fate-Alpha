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

#define sPlayer Player::GetInstance()

class ActionMgr;
class Creature;
class TalkClass
{
public:
	void AddItem(uint8 ImageID, uint32 sender, uint32 action, std::string TalkString)
	{
		if (Menus.size() >= 4)
			return;
		Menus.push_back(GossipMenu(ImageID, sender, action, TalkString));
	}
	void ClearMenu()				{ Menus.clear(); }
	std::vector<GossipMenu> Menus;
	Creature* TalkingCreature = nullptr;
};

class Player : public Unit
{
public:
	explicit Player(SkeletonAnimation* _SkeletonAnimation, CharacterEnumInfo& _info);
	~Player();

	static Player* GetInstance();
	bool CreatePlayer();
	bool HasSpell(uint32 spellid);
	bool LoadPlayerSpells();
	void CheckPlayerSpellActive();
	void DealVirtualRoker(VirtualRockerOrginType _VirtualRockerOrginType);
	void ResetMoveKeyForRoker();
	bool CanCancelActionForMove();
	void SaveToDB();
	virtual bool LoadFromDB();
	bool UpdatePlayerValues();
	void SendGossipMenu(std::string MainString, Creature* pCreature);
	void CloseGossipMenu();
	ActionType GetDoingAction()									{ return m_Action; }
	ActionMgr* PlayerActionMgr()								{ return _ActionMgr; }
	uint32 GetMoney()											{ return m_Money; }
	uint32 GetExp()												{ return m_Exp; }
	uint32 GetMapid()											{ return m_Mapid; }
	PlayerSpells GetSpells()									{ return m_Spells; }
	Unit* GetPlayerTarget()										{ return m_Player_Target; }
	void SetMapid(uint32 _var)									{ m_Mapid = _var; }
	void SetExp(uint32 _var)									{ m_Exp = _var; }
	void SetMoveKeyEnable(MoveKeyValue _key, bool enable)		{ MoveKeyStatus[_key] = enable; }
	void ResetKeyTimer()										{ KeyVectorClearTimer = Base_Clear_Key_Time; }
	virtual void DestorySelf()									{ removeFromParentAndCleanup(true); }
	void SetMoney(uint32 _var)									{ m_Money = _var; }
	void SetPlayerTarget(Unit* pUnit);
	void ReSetPlayerTarget();
	TalkClass* PlayerTalkClass;
private:
	ActionType m_Action;
	virtual void update(float diff);
	ActionMgr* _ActionMgr;
	std::map<MoveKeyValue, bool> MoveKeyStatus;
	virtual void UpdateMoveStatus();
	float KeyVectorClearTimer;
	PlayerSpells m_Spells;
	uint32 m_Money;
	uint32 m_Exp;
	uint32 m_Mapid;
	Unit* m_Player_Target;
};

#endif