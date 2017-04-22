#include "ScriptMgr.h"
#include "Creature.h"
struct TestAI_One : public ScriptAI
{
	TestAI_One(Creature* pCreature) : ScriptAI(pCreature) {}

	uint32 testtimer = 0;

	void ReSet()
	{
		testtimer = 6000;
	}


	void UpdateAI(const uint32 diff)
	{
		if (testtimer <= diff)
		{
			testtimer = 6000;
			log("One");
		}
		else
			testtimer -= diff;
	}
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