#include "Npc.h"

Npc::Npc(SkeletonAnimation* _animation, uint32 entry, uint32 guid) : Creature(_animation, entry, guid)
{
	SetTypeId(TYPEID_NPC);
}

Npc::~Npc()
{
}