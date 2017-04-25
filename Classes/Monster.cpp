#include "Monster.h"
#include "Player.h"

Monster::Monster(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Creature(_SkeletonAnimation, entry, guid)
{
	if (!InitWithLoadedData())
		DestorySelf();
	SetTypeId(TYPEID_MONSTER);
}

Monster::~Monster()
{
}

bool Monster::InitWithLoadedData()
{

	return true;
}

void Monster::update(float diff)
{
	Creature::update(diff);
}