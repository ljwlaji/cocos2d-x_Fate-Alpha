#include "Player.h"

static Player* _Player = nullptr;

Player::Player(std::string name, uint32 rase, uint32 _class, uint32 gender, uint32 level, uint32 money,uint32 _guildid) :
	Name(name.empty() ? "" : name),
	Race(rase ? rase : -1),
	Class(_class ? _class : -1),
	Gender(gender ? gender : -1),
	Level(level ? level : 0),
	Money(money ? money : 0),
	guildid(_guildid ? _guildid : 0)
{
	_Player = this;
}

Player::~Player()
{
}

Player* Player::GetInstance()
{
	if (_Player)
		return _Player;
	return nullptr;
}