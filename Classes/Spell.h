#ifndef __SPELL_H__
#define __SPELL_H__

#include "cocos2d.h"
#include "SpellMgr.h"
USING_NS_CC;

class Spell
{
public:
	Spell(Unit* _caster, Unit* pTarget, const SpellInfo& _info);
	~Spell();
	float GetSpellTotalCastTime()	{ return m_Spell_Total_Casting_Time; }
	float GetSpellCurrentTimeLeft()	{ return m_SpellInfo.SpellCastTime; }
	const SpellInfo* GetSpellInfo()	{ return &m_SpellInfo; }
	void update(const float& diff);
	void cancel();
private:
	enum SpellStatus
	{
		STATUS_CASTING,
		STATUS_SPELLGO,
		STATUS_SPELLEND,
	};
	void cast();
	void FillTargetMap();
	SpellInfo m_SpellInfo;
	void finish();
	std::list<Unit*> m_SpellTargets;
	Unit* m_Target;
	Unit* m_caster;
	SpellStatus m_status;
	float m_Spell_Total_Casting_Time;
};

#endif