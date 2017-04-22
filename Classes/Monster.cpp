#include "Monster.h"
Monster::Monster(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Creature(_SkeletonAnimation, entry, guid)
{
	if (!InitWithLoadedData())
		DestorySelf();
		
}

Monster::~Monster()
{
}

bool Monster::InitWithLoadedData()
{



	return true;
}