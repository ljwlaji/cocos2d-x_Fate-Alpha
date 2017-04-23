#include "Creature.h"
#include "MainMapLayer.h"
#include "ScriptMgr.h"
#include "Player.h"

Creature::Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Unit(_SkeletonAnimation, entry, guid)
{
	m_script_ai = nullptr;
	WaitFroLoadingUnitTemplate _template = sMainMap->GetCreatureTemplate(guid);
	if (_template.guid == guid)
		m_script_ai = sScriptMgr->GetScripts(_template.ScriptName, this);

	if (m_script_ai)
		m_script_ai->ReSet();
	scheduleUpdate();
}

Creature::~Creature()
{
	if (m_script_ai)
		delete m_script_ai;

	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
}


void Creature::UpdateMoveStatus()
{

}

bool Creature::LoadFromDB()
{
	return false;
}

void Creature::OnGossipHello(Player* pPlayer)
{
	if (!HasScript())
		return;

	GetAI()->OnGossipHello(pPlayer, this);
}

void Creature::OnGossipSelect(Player* pPlayer, uint32 sender, uint32 action)
{
	if (!IsAlive())
		return;
	if (!HasScript())
		return;

	GetAI()->OnGossipSelect(pPlayer, this, sender, action);
}

void Creature::update(float diff)
{
	if (IsAlive())
	{
		if (HasScript())
		{
			m_script_ai->UpdateAI((uint32)(diff * 1000));

			if (!IsInCombat())
			if (sPlayer && sPlayer->IsAlive() && getPosition().getDistance(sPlayer->getPosition()) < 150.0f)
				GetAI()->MoveInLineOfSight(sPlayer);
		}
	}
}