#include "ScriptMgr.h"
#include "Creature.h"
#include "Player.h"
struct TestAI_One : public ScriptAI
{
	TestAI_One(Creature* pCreature) : ScriptAI(pCreature) {}

	uint32 testtimer = 0;

	void ReSet()
	{
		testtimer = 6000;
	}

	void OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		pPlayer->PlayerTalkClass->ClearMenu();
		pPlayer->ADD_GOSSIP_ITEM(1, 2, 3, "321");
		pPlayer->ADD_GOSSIP_ITEM(1, 2, 3, "333");
		pPlayer->ADD_GOSSIP_ITEM(1, 2, 3, "444");
		pPlayer->ADD_GOSSIP_ITEM(1, 2, 3, "555");

		pPlayer->SEND_GOSSIP_MENU("Hello Stanger!\nThis is A Test Title.", pCreature);
	}

	void OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action) 
	{
		pPlayer->PlayerTalkClass->ClearMenu();
		pPlayer->ADD_GOSSIP_ITEM(1, 2, 3, "97");
		pPlayer->ADD_GOSSIP_ITEM(1, 2, 3, "98");
		pPlayer->SEND_GOSSIP_MENU("This is Page 2", pCreature);
	}

	void UpdateAI(const uint32& diff)
	{
		if (!me()->UpdateVictim())
			return;

		if (testtimer <= diff)
		{
			me()->CastSpell(10, me()->UpdateVictim());
			testtimer = 6000;
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