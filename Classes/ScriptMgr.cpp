#include "ScriptMgr.h"
#include "ScriptLoader.h"

static ScriptMgr* _ScriptMgr = nullptr;

ScriptMgr::ScriptMgr()
{
	_ScriptMgr = this;
}

ScriptMgr::~ScriptMgr()
{
	_ScriptMgr = nullptr;
}

ScriptMgr* ScriptMgr::GetInstance()
{
	if (!_ScriptMgr)
		_ScriptMgr = new ScriptMgr();
	return _ScriptMgr;
}

void ScriptMgr::InitScripts()
{
	Scripts.clear();
	AddScripts();
}

ScriptAI* ScriptMgr::GetScripts(std::string& name, Creature* pCreature)
{
	if (name.empty())
		return nullptr;
	for (int i = 0; i != Scripts.size(); i++)
	{
		if (Scripts.at(i)->name == name)
		{
			return (Scripts.at(i)->GetAI)(pCreature);
		}
	}
	return nullptr;
}

void Script::RegisterSelf()
{
	for (int i = 0; i != sScriptMgr->GetScriptVector().size(); i++)
	{
		if (sScriptMgr->GetScriptVector().at(i) == this)
		{
			return;
		}
	}
	sScriptMgr->GetScriptVector().push_back(this);
}