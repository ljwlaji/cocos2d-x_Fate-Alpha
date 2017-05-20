#include "Creature.h"
#include "MainMapLayer.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "MovementMgr.h"
#include "HelloWorldScene.h"
#include "PlayerTalkLayer.h"
#include "Spell.h"
#include "Loot.h"
Creature::Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Unit(_SkeletonAnimation, entry, guid)
{
	m_script_ai = nullptr;
	m_UnitMover = nullptr;
	m_Loot = nullptr;
	WaitFroLoadingUnitTemplate _template = sMainMap->GetCreatureTemplate(guid);
	SetLevel(_template.Level);
	SetFaction(_template.faction);
	SetClass((UnitClasses)_template.Class);
	CreatureStartPoint = Vec2(_template.pos_x, _template.pos_y);
	m_Flags = _template.Npc_Flags;
	SetEntry(entry);
	SetGuid(guid);
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

	if (m_Loot)
		m_Loot->DeleteLootIfNeed(true);

	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
}

void Creature::EnterEvadeMode()
{
	if (m_UnitMover)
	{
		delete m_UnitMover;
		m_UnitMover = nullptr;
	}
	if (GetCastingSpell())
		GetCastingSpell()->cancel();
	ResetThreatList();
	SetTarget(nullptr);
	SetRealPosition(sMainMap->GetVisableSize().x * CreatureStartPoint.x / 100, sMainMap->GetVisableSize().y * CreatureStartPoint.y / 100);
}

void Creature::Reset()
{
	if (HasScript())
		CreatureAI()->ReSet();

	SetDeathStatus(Alive);
	SetUnitInt32Value(Curr_HP, GetUnitInt32Value(Max_HP));
	EnterEvadeMode();
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
	pUnit->SetInCombat(true);
	if (pUnit->ToPlayer())
		pUnit->ToPlayer()->AddUnitToPlayerCombatList(this);
}

void Creature::OnGossipHello(Player* pPlayer)
{
	if (!IsFrendlyTo(pPlayer) || !IsAlive())
		return;

	if (IsQuestGiver())
	{
		const std::list<uint32>* TempList = sGame->GetCreatureQuests(GetEntry());
		for (std::list<uint32>::const_iterator itr = TempList->begin(); itr != TempList->end(); itr++)
		{
			if (sPlayer->HasQuest(*itr))
				continue;
			//Send Quest
			sPlayerTalkLayer->SendQuestMenuToPlayer(GetEntry());
			sPlayerTalkLayer->SetQuestTalking(true);
			return;
		}
	}

	else if (IsVendor())
	{
		if (IsItemRepairer())
		{

		}

	}
	else if (IsSpellTeacher())
	{

	}

	if (IsGossipTalker())
	{
		sPlayerTalkLayer->SetQuestTalking(false);
		CreatureAI()->OnGossipHello(pPlayer, this);
	}
}

void Creature::OnGossipSelect(Player* pPlayer, uint32 sender, uint32 action)
{
	if (!IsAlive() || !IsFrendlyTo(pPlayer))
		return;

	if (HasScript())
		CreatureAI()->OnGossipSelect(pPlayer, this, sender, action);
}

bool Creature::CheckDisTanceForMILS(Unit* pUnit)
{
	if (abs(getPositionX() - pUnit->getPositionX()) < 350.0f)
		return true;
	return false;
}

void Creature::UpdateAI(const float& diff)
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

void Creature::UpdateMovement(const float& diff)
{
	if (!IsInAttackRange(UpdateVictim()))
	{
		if (!m_UnitMover)
		{
			if (m_Creature_Move_CheckTimer <= diff)
			{
				UpdateMove();
				m_Creature_Move_CheckTimer = 1.0f;
			}
			else m_Creature_Move_CheckTimer -= diff;
		}
		else
		{
			if (m_UnitMover->MoveDelay > diff)
			{
				float X_Modify = 0;
				m_UnitMover->Side ? X_Modify = Base_X_MovePoint : X_Modify = 0 - Base_X_MovePoint;
				m_UnitMover->Side ? SetFacing(Facing_Right) : SetFacing(Facing_Left);
				if (sMoveMgr->CanMoveTo(this, X_Modify < 0 ? Move_To_Left : Move_To_Right, X_Modify))
					setPositionX(getPositionX() + X_Modify);

				float CheckPoint = abs(UpdateVictim()->getPositionY() - getPositionY());
				if (CheckPoint > sMainMap->GetVisableSize().y * 0.02f)
				{
					float Mov_Y = 0;
					UpdateVictim()->getPositionY() > getPositionY() ? Mov_Y += Base_Y_MovePoint : Mov_Y -= Base_Y_MovePoint;
					if (sMoveMgr->CanMoveTo(this, Mov_Y < 0 ? Move_To_Down : Move_To_Up, Mov_Y))
						setPositionY(getPositionY() + Mov_Y);
				}
				m_UnitMover->MoveDelay -= diff;
			}
			else
			{
				m_UnitMover = nullptr;
				delete m_UnitMover;
				m_Creature_Move_CheckTimer = 0.2f;
			}
		}
	}
}

void Creature::update(float diff)
{
	if (IsAlive())
	{
		CheckMoveFall();
		if (GetCastingSpell())
			GetCastingSpell()->update(diff);
		if (!IsInCombat())
		{
			if (Unit* pUnit = SelectNearestUnit(true, true))
			if (CheckDisTanceForMILS(pUnit))
				MoveInLineOfSight(pUnit);
		}
		else
		{
			ThreatUpdate();
			if (UpdateVictim())
			{
				UpdateAI(diff);
				UpdateMovement(diff);
			}
		}
	}
}

bool Creature::IsInThreatList(Unit* pUnit)
{
	if (m_Creature_Threat_List.find(pUnit) != m_Creature_Threat_List.end())
		return true;
	return false;
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
	if (m_Creature_Threat_List.empty())
	{
		if (IsInCombat())
			EnterEvadeMode();
		return;
	}

	std::vector<Unit*> WaitForDeleteList;
	float HighestThreat = 0;
	Unit* Target = nullptr;
	for (CreatureThreadList::iterator itr = m_Creature_Threat_List.begin(); itr != m_Creature_Threat_List.end(); itr++)
	{
		if (itr->second <= 0 || !itr->first->IsAlive())
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
