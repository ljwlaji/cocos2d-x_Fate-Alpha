#include "Creature.h"
#include "MainMapLayer.h"
#include "ScriptMgr.h"
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

void Creature::update(float diff)
{
	if (HasScript())
		m_script_ai->UpdateAI((uint32)(diff * 1000));
}