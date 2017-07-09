#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
#include "Creature.h"
USING_NS_CC;

class Npc : public Creature
{
public:
	explicit Npc(SkeletonAnimation* _animation, uint32 entry, uint32 guid);
	~Npc();
	void DestorySelf() { removeFromParentAndCleanup(true); }
private:
	virtual void UpdateMoveStatus(){};
	virtual bool LoadFromDB(){ return true; };
};


#endif