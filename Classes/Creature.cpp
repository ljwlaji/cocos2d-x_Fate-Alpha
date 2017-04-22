#include "Creature.h"
#include "MainMapLayer.h"
#include "ScriptMgr.h"
Creature::Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Unit(_SkeletonAnimation, entry, guid)
{
	m_script_ai = nullptr;
	WaitFroLoadingUnitTemplate _template = sMainMap->GetCreatureTemplate(guid);
	if (_template.guid == guid)
	{
		m_script_ai = sScriptMgr->GetScripts(_template.ScriptName, this);
	}
}

Creature::~Creature()
{
}


void Creature::UpdateMoveStatus()
{

}

bool Creature::LoadFromDB()
{
	return false;
}