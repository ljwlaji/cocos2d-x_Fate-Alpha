#include "Creature.h"
#include "MainMapLayer.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "MovementMgr.h"
#include "HelloWorldScene.h"
Creature::Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Unit(_SkeletonAnimation, entry, guid)
{
	m_script_ai = nullptr;
	m_UnitMover = nullptr;
	WaitFroLoadingUnitTemplate _template = sMainMap->GetCreatureTemplate(guid);
	SetLevel(_template.Level);
	SetFaction(_template.faction);
	SetClass((UnitClasses)_template.Class);
	m_Creature_Threat_List.clear();
	if (_template.guid == guid)
		m_script_ai = sScriptMgr->GetScripts(_template.ScriptName, this);

	if (m_script_ai)
		m_script_ai->ReSet();

	if (GetLevel())
		UpdateUnitValues();

	m_Creature_Move_CheckTimer = 1.0f;
	scheduleUpdate();
}

Creature::~Creature()
{
	if (m_script_ai)
		delete m_script_ai;

	if (m_UnitMover)
		delete m_UnitMover;
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
}


void Creature::UpdateMove()
{
	if (Unit* pTarget = UpdateVictim())
	{
		Vec2 TargetLoc = (pTarget->getPosition());
		UnitSide side = CheckSideForUnit(pTarget->getPosition());
		m_UnitMover = new UnitMover(side,1.0f);
	}
}

bool Creature::LoadFromDB()
{
	return false;
}

void Creature::CombatStart(Unit* pUnit)
{
	SetInCombat(true);
	SetTarget(pUnit);
	AddThreat(pUnit, 5.0f);
}

void Creature::OnGossipHello(Player* pPlayer)
{
	if (!HasScript() || !ToNpc() || !IsAlive() || !IsFrendlyTo(pPlayer))
		return;

	CreatureAI()->OnGossipHello(pPlayer, this);
}

void Creature::OnGossipSelect(Player* pPlayer, uint32 sender, uint32 action)
{
	if (!HasScript() || !ToNpc() || !IsAlive() || !IsFrendlyTo(pPlayer))
		return;
	CreatureAI()->OnGossipSelect(pPlayer, this, sender, action);
}

bool Creature::CheckDisTanceForMILS(Unit* pUnit)
{
	if (abs(getPositionX() - pUnit->getPositionX()) < 350.0f)
		return true;
	return false;
}

void Creature::update(float diff)
{
	if (IsAlive())
	{
		CheckMoveFall();
		if (!IsInCombat())
		{
			if (Unit* pUnit = SelectNearestUnit(true, true))
			if (CheckDisTanceForMILS(pUnit))
				MoveInLineOfSight(pUnit);
		}
		else
		{
			ThreatUpdate();
			if (UpdateVictim() && !IsInAttackRange(UpdateVictim()))
			{
				if (m_Creature_Move_CheckTimer <= diff)
				{
					UpdateMove();
					m_Creature_Move_CheckTimer = 1.0f;
				}
				else if (m_UnitMover)
				{
					if (m_UnitMover->MoveDelay > diff)
					{
						float X_Modify = 0;
						m_UnitMover->Side ? X_Modify = Base_X_MovePoint : X_Modify = 0 - Base_X_MovePoint;
						m_UnitMover->Side ? SetFacing(Facing_Right) : SetFacing(Facing_Left);
						if (sMoveMgr->CanMoveTo(this, X_Modify < 0 ? Move_To_Left : Move_To_Right, X_Modify))
							setPositionX(getPositionX() + X_Modify);
						m_UnitMover->MoveDelay -= diff;
					}
					else
					{
						m_UnitMover = nullptr;
						delete m_UnitMover;
						m_Creature_Move_CheckTimer = 0.2f;
					}
				}
				else m_Creature_Move_CheckTimer -= diff;
			}
			else
			{
				if (HasScript())
				{
					CreatureAI()->UpdateAI((uint32)(diff * 1000));
				}
				else
				{
					//Normal AI
				}
			}
		}
	}
}

void Creature::AddThreat(Unit* pTarget, float Threat)
{
	if (m_Creature_Threat_List.find(pTarget) != m_Creature_Threat_List.end())
		m_Creature_Threat_List[pTarget] += Threat;
	else
		m_Creature_Threat_List[pTarget] = Threat;
}

void Creature::ThreatUpdate()
{
	if (m_Creature_Threat_List.empty() || !IsInCombat())
		return;

	std::vector<Unit*> WaitForDeleteList;
	float HighestThreat = 0;
	Unit* Target = nullptr;
	for (CreatureThreadList::iterator itr = m_Creature_Threat_List.begin(); itr != m_Creature_Threat_List.end(); itr++)
	{
		if (itr->second <= 0)
		{
			WaitForDeleteList.push_back(itr->first);
			continue;
		}
		float TempThreat = itr->second;
		if (HighestThreat < TempThreat)
		{
			HighestThreat = TempThreat;
			Target = itr->first;
		}
	}
	if (Target != UpdateVictim())
		SetTarget(Target);

	for (int i = 0; i != WaitForDeleteList.size(); i++)
		m_Creature_Threat_List.erase(m_Creature_Threat_List.find(WaitForDeleteList.at(i)));
}

void Creature::MoveInLineOfSight(Unit* pUnit)
{
	if (!IsInCombat() && !IsFrendlyTo(pUnit))
	{
		CombatStart(pUnit);
	}
}
