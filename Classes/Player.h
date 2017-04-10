#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Unit.h"
class Player : public Unit
{
public:
	explicit Player(SkeletonAnimation* _SkeletonAnimation);
	~Player();
private:
	virtual void update(float diff);
};

#endif