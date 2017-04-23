#ifndef __SCRIPT_AI_H__
#define __SCRIPT_AI_H__
#include "Types.h"

class Creature;
class Unit;
class Player;
class ScriptAI
{
public:
	ScriptAI(Creature* pCreature);
	~ScriptAI();

	virtual void UpdateAI(const uint32 diff) {};
	virtual void ReSet(){};
	virtual void OnGossipHello(Player* pPlayer, Creature* pCreature) {};
	virtual void OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action) {};
	void MoveInLineOfSight(Unit* pUnit);
	Creature* me;
private:
};


#endif