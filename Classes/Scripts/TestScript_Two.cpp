#include "ScriptMgr.h"
#include "Creature.h"
struct TestAI_Two : public ScriptAI
{
	TestAI_Two(Creature* pCreature) : ScriptAI(pCreature) {}

	uint32 testtimer = 0;

	void ReSet()
	{
		testtimer = 3000;
	}


	void UpdateAI(const uint32& diff)
	{
		if (testtimer <= diff)
		{
			testtimer = 3000;
			log("Two");
		}
		else
			testtimer -= diff;
	}
};

ScriptAI* GetAI_TestAI_Two(Creature* c)
{
	return new TestAI_Two(c);
}

void AddSC_TestAI_Tow()
{
	Script* _Script = new Script();

	_Script->name = "123123";
	_Script->GetAI = &GetAI_TestAI_Two;
	_Script->RegisterSelf();
}