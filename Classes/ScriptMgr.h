#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include "ScriptAI.h"
#include "Types.h"
#include <vector>
#include <string>

#define sScriptMgr ScriptMgr::GetInstance()

#define ADD_GOSSIP_ITEM(a,b,c,d)	PlayerTalkClass->AddItem(a,b,c,d)
#define SEND_GOSSIP_MENU(a,b)		SendGossipMenu(a,b)
#define CLOSE_GOSSIP_MENU			CloseGossipMenu
class Script;
class ScriptMgr
{
public:
	ScriptMgr();
	~ScriptMgr();
	static ScriptMgr* GetInstance();
	void InitScripts();
	ScriptAI* GetScripts(std::string& name, Creature* pCreature);
	std::vector<Script*>& GetScriptVector() { return Scripts; };
private:
	std::vector<Script*> Scripts;

};

struct Script
{
	std::string name = "";
	ScriptAI* (*GetAI)(Creature*);
	void RegisterSelf();
};

#endif