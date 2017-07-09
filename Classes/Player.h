#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Common.h"


#define sPlayer Player::GetInstance()

class Player
{
public:
	Player(std::string name, uint32 rase, uint32 _class, uint32 gender, uint32 level, uint32 money, uint32 guildid);
	static Player* GetInstance();
	void SetName(std::string& name) { Name = name; }
	void SetRace(uint32 var)		{ Race = var; }
	void SetClass(uint32 var)		{ Class = var; }
	void SetGender(uint32 var)		{ Gender = var; }
	void SetLevel(uint32 var)		{ Level = var; }
	void SetMoney(uint32 var)		{ Money = var; }
	void SetGuildId(uint32 var)		{ guildid = var; }
	std::string GetName()			{ return Name; }
private:
	~Player();


private:
	std::string Name = "";
	int Race = 0;
	int Class = 0;
	int Gender = 0;
	int Level = 0;
	int Money = 0;
	int guildid = 0;
};

#endif