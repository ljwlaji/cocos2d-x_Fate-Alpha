#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include <vector>
#include "ScriptAI.h"
#define sScriptMgr ScriptMgr::GetInstance()


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