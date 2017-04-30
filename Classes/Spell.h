#ifndef __SPELL_H__
#define __SPELL_H__

#include "cocos2d.h"
#include "SpellMgr.h"
USING_NS_CC;

class Spell : public Sprite
{
public:
	Spell(Unit* _caster, Unit* pTarget, const SpellInfo& _info);
	~Spell();
private:
	enum SpellStatus
	{
		STATUS_CASTING,
		STATUS_SPELLGO,
		STATUS_SPELLEND,
	};
	void cast();
	void FillTargetMap();
	virtual void update(float diff);
	SpellInfo m_SpellInfo;
	void cancel();
	void finish();
	std::list<Unit*> m_SpellTargets;
	Unit* m_Target;
	Unit* m_caster;
	SpellStatus m_status;
};

#endif