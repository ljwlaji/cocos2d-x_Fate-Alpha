#include "SpellBullet.h"
#include "Unit.h"
#include "MainMapLayer.h"

Spell_Builet* Spell_Builet::CreateBullet(Unit* Owner, Unit* pTarget, uint32 Damage)
{
	Spell_Builet* TempBuilet = nullptr;
	if (pTarget && pTarget->IsAlive())
	{
		Spell_Builet* TempBuilet = new Spell_Builet();
		TempBuilet->initWithFile("Arrow.png");
		sMainMap->addChild(TempBuilet);
		TempBuilet->SetDamage(Damage);
		TempBuilet->SetTarget(pTarget);
		TempBuilet->SetOwner(Owner);
		TempBuilet->setPosition(Owner->getPosition());
		TempBuilet->scheduleUpdate();
	}
	return TempBuilet;
}

Spell_Builet::Spell_Builet()
{

}

Spell_Builet::~Spell_Builet()
{
}

void Spell_Builet::update(float diff)
{
	if (IsContectPoint(pTarget->getPosition()))
	{
		pTarget->DealSpellDamage(pTarget, Attack, m_Damage);
		removeFromParentAndCleanup(true);
		return;
	}

	if (pTarget && pTarget->IsAlive())
	{
		int i = 6;
		while (--i)
		{
			if (abs(getPositionY() - pTarget->getPositionY()) > 5.0f)
				setPositionY(getPositionY() + (pTarget->getPositionY() > getPositionY() ? 2.5f : -2.5f));
			if (abs(getPositionX() - pTarget->getPositionX()) > 5.0f)
				setPositionX(getPositionX() + (pTarget->getPositionX() > getPositionX() ? 2.5f : -2.5f));
		}
	}
	else
		removeFromParentAndCleanup(true);
}