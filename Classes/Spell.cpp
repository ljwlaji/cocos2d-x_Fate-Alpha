#include "Spell.h"
#include "Unit.h"
#include "NotifyMgr.h"
Spell::Spell(Unit* caster, Unit* pTarget, const SpellInfo& _info)
{
	initWithFile("1.png");
	autorelease();
	caster->addChild(this);
	m_SpellInfo = _info;
	m_caster = caster;
	m_Target = pTarget;
	m_status = STATUS_CASTING;

	m_caster->SetMoveType(MoveType_Actioning);
	m_caster->GetVision()->clearTracks();
	m_caster->GetVision()->setAnimation(0, _info.SpellActionName.c_str(), false);
	scheduleUpdate();
}

Spell::~Spell()
{

}

void Spell::cast()
{
	FillTargetMap();
	switch (m_SpellInfo.SpellType)
	{
	case SpellType_Melee:
		for (std::list<Unit*>::iterator itr = m_SpellTargets.begin(); itr != m_SpellTargets.end(); itr++)
		{
			m_caster->DealSpellDamage(m_caster, *itr, m_SpellInfo.EffectType, m_SpellInfo.SpellValue);
		}
		finish();
		break;
	case SpellType_Magic:
		m_status = STATUS_SPELLGO;
		break;
	case SpellType_Arua:
		break;
	default:
		cancel();
		break;
	}
}

void Spell::FillTargetMap()
{

}

void Spell::cancel()
{
	m_caster->SetCastingSpell(nullptr);
	removeFromParentAndCleanup(true);
	//仅内存清理 重置CD时间
}

void Spell::finish()
{
	m_caster->SetCastingSpell(nullptr);
	removeFromParentAndCleanup(true);
	//做降低使用XX动作 及内存清理 不重置CD时间
}

void Spell::update(float diff)
{
	switch (m_status)
	{
	case STATUS_CASTING:
		if (m_Target && m_caster->getPosition().getDistance(m_Target->getPosition()) > m_SpellInfo.SpellCastRange)
		{
			if (m_caster->ToPlayer())
			{
				sNotifyMgr->ShowNotify("Out Of Range");
				m_status = STATUS_SPELLEND;
				cancel();
			}
			return;
		}
		if (m_SpellInfo.SpellCastTime <= diff)
		{
			cast();
			return;
		}
		else m_SpellInfo.SpellCastTime -= diff;
		break;
	case STATUS_SPELLGO:
		//创建Spell攻击物体
		break;
	case STATUS_SPELLEND:
		break;
	}
}