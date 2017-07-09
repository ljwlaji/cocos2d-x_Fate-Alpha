#include "ScriptMgr.h"
#ifdef __APPLE__
#include "Scripts/TestScript_One.cpp"
#include "Scripts/TestScript_Two.cpp"
#endif
void AddSC_TestAI_One();
void AddSC_TestAI_Tow();













void AddScripts()
{
	AddSC_TestAI_One();
	AddSC_TestAI_Tow();
}
