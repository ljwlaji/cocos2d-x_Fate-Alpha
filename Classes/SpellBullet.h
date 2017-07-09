#ifndef __SPELL_BUILET_H__
#define __SPELL_BUILET_H__

#include "Types.h"


class Spell_Builet : public Sprite
{
public:
	static Spell_Builet* CreateBullet(Unit* pOwner, Unit* pTarget, uint32 Damage);
	void SetDamage(uint32 var)	{ m_Damage = var; }
	void SetTarget(Unit* Target)	{ pTarget = Target; }
	void SetOwner(Unit* pOnwer)		{ Owner = pOnwer; }
private:
	Spell_Builet();
	~Spell_Builet();
	virtual void update(float diff);
	int m_Damage;
	Unit* pTarget;
	Unit* Owner;
};

#endif