#ifndef __SCRIPT_AI_H__
#define __SCRIPT_AI_H__

class Creature;
class ScriptAI
{
public:
	ScriptAI(Creature* pCreature);
	~ScriptAI();

private:


	Creature* me;
};


#endif