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
class Loot;
class Creature : public Unit
{
public:
	explicit Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid);
	~Creature();
	void OnGossipHello(Player* pPlayer);
	void OnGossipSelect(Player* pPlayer, uint32 sender, uint32 action);
	bool IsQuestGiver()								{ return m_Flags & NPC_Flag_QuestGiver; }
	bool IsGossipTalker()							{ return m_Flags & NPC_Flag_GossipTalker; }
	bool IsVendor()									{ return m_Flags & NPC_Flag_Vendor; }
	bool IsItemRepairer()							{ return m_Flags & NPC_Flag_Reqairer; }
	bool IsSpellTeacher()							{ return m_Flags & NPC_Flag_SpellTeacher; }
	void CombatStart(Unit* pUnit);
	bool HasScript()								{ bool re; m_script_ai ? re = true : re = false; return re; }
	ScriptAI* CreatureAI()							{ return m_script_ai; }
	uint32 GetEntry()								{ return m_Entry; }
	CreatureThreadList GetThreatList()				{ return m_Creature_Threat_List; }
	bool IsInThreatList(Unit* pUnit);
	void SetLoot(Loot* pLoot)						{ m_Loot = pLoot; }
	Loot* GetLoot()									{ return m_Loot; }
	void SetEntry(uint32 entry)						{ m_Entry = entry; }
	void ThreatUpdate();
	void ResetThreatList()							{ m_Creature_Threat_List.clear(); }
	void AddThreat(Unit* pTarget, float Threat);
	virtual void update(float diff);
	void UpdateAI(const float& diff);
	void UpdateMovement(const float& diff);
	void MoveInLineOfSight(Unit* pUnit);
	bool CheckDisTanceForMILS(Unit* pUnit);
	void Reset();
	void EnterEvadeMode();
private:
	Vec2 CreatureStartPoint;
	void FindScript();
	virtual void UpdateMove();
	virtual bool LoadFromDB();
	void GetScript(std::string ScriptName);
	ScriptAI* m_script_ai;

	CreatureThreadList m_Creature_Threat_List;

	float m_Creature_Move_CheckTimer;
	UnitMover* m_UnitMover;

	uint32 m_Entry;
	Loot* m_Loot;
	uint32 m_Flags;
};

#endif