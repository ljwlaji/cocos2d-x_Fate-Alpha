#include "Player.h"

Player::Player(SkeletonAnimation* _SkeletonAnimation) : Unit(_SkeletonAnimation)
{
	
}

Player::~Player()
{
}

void Player::update(float diff)
{
	setPositionY(getPositionY() + 2.0f);
}