#ifndef __SCRIPT_AI_H__
#define __SCRIPT_AI_H__
#include "Types.h"

class Creature;
class ScriptAI
{
public:
	ScriptAI(Creature* pCreature);
	~ScriptAI();


	virtual void UpdateAI(const uint32 diff) {};
	virtual void ReSet(){};
	virtual void OnGossipHello() {};
	virtual void OnGossipSelect() {};
	Creature* me;
private:

};


#endif