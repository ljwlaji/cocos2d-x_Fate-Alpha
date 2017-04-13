#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Unit.h"

class Monster : public Unit
{
public:
	explicit Monster(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid);
	~Monster();
private:

	virtual bool LoadFromDB(){ return true; };
	virtual void UpdateMoveStatus(){};
};




#endif