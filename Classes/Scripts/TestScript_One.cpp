#include "ScriptMgr.h"

struct TestAI_One : public ScriptAI
{
	TestAI_One(Creature* pCreature) : ScriptAI(pCreature) {}

};

ScriptAI* GetAI_TestAI_One(Creature* c)
{
	return new TestAI_One(c);
}

void AddSC_TestAI_One()
{
	Script* _Script = new Script();

	_Script->name = "12312";
	_Script->GetAI = &GetAI_TestAI_One;
	_Script->RegisterSelf();
}