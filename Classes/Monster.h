#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Creature.h"

class Monster : public Creature
{
public:
	explicit Monster(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid);
	~Monster();

	virtual void DestorySelf() { removeFromParentAndCleanup(true); }
private:
	virtual void update(float diff);
	virtual bool InitWithLoadedData();
	virtual void UpdateMoveStatus(){};
	void InitProccessBar();
	void UpdatePorccessBar();
	ProgressTimer* HpProccessBar;
	ProgressTimer* CastingProccessBar;
};




#endif