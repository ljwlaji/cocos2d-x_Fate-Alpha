#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "cocos2d.h"
#include "ScriptAI.h"
#include "Unit.h"

USING_NS_CC;

struct UnitMover
{
	UnitMover(UnitSide side, float movedelay)
	{
		Side = side;
		MoveDelay = movedelay;
	}
	UnitSide Side;
	float MoveDelay;
};

class Creature : public Unit
{
public:
	explicit Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid);
	virtual ~Creature();
	void OnGossipHello(Player* pPlayer);
	void OnGossipSelect(Player* pPlayer, uint32 sender, uint32 action);
	void CombatStart(Unit* pUnit);
	bool HasScript()								{ bool re; m_script_ai ? re = true : re = false; return re; }
	ScriptAI* CreatureAI()							{ return m_script_ai; }
	CreatureThreadList GetThreatList()				{ return m_Creature_Threat_List; }
	void ThreatUpdate();
	void AddThreat(Unit* pTarget, float Threat);
	virtual void update(float diff);
	void MoveInLineOfSight(Unit* pUnit);
	bool CheckDisTanceForMILS(Unit* pUnit);
private:
	void FindScript();
	virtual void UpdateMove();
	virtual bool LoadFromDB();
	void GetScript(std::string ScriptName);
	ScriptAI* m_script_ai;

	CreatureThreadList m_Creature_Threat_List;


	float m_Creature_Move_CheckTimer;
	UnitMover* m_UnitMover;
};

#endif