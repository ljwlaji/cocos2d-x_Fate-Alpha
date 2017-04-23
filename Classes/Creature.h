#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "cocos2d.h"
#include "ScriptAI.h"
#include "Unit.h"

USING_NS_CC;

class Creature : public Unit
{
public:
	explicit Creature(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid);
	virtual ~Creature();
	void OnGossipHello(Player* pPlayer);
	void OnGossipSelect(Player* pPlayer, uint32 sender, uint32 action);
	ScriptAI* GetAI()	{ return m_script_ai; }
private:
	bool HasScript() { bool re; m_script_ai ? re = true : re = false; return re; }
	virtual void update(float diff);
	void FindScript();
	virtual void UpdateMoveStatus();
	virtual bool LoadFromDB();
	void GetScript(std::string ScriptName);
	ScriptAI* m_script_ai;



};

#endif