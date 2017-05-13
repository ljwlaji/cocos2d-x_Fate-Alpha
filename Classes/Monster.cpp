#include "Monster.h"
#include "Player.h"
#include "Spell.h"

Monster::Monster(SkeletonAnimation* _SkeletonAnimation, uint32 entry, uint32 guid) : Creature(_SkeletonAnimation, entry, guid)
{
	if (!InitWithLoadedData())
		DestorySelf();
	SetTypeId(TYPEID_MONSTER);
	InitProccessBar();
}

Monster::~Monster()
{
	DestoryVision();
	removeAllChildrenWithCleanup(true);
}

bool Monster::InitWithLoadedData()
{
	return true;
}

void Monster::update(float diff)
{
	Creature::update(diff);
	UpdatePorccessBar();
}

void Monster::InitProccessBar()
{
	HpProccessBar = ProgressTimer::create(Sprite::create(MonsterHpProccessBar));
	HpProccessBar->setAnchorPoint(Vec2(0.5f, 0.5f));
	HpProccessBar->setPercentage(0);
	HpProccessBar->setBarChangeRate(Vec2(1, 0));
	HpProccessBar->setMidpoint(Vec2(0, 0));
	HpProccessBar->setType(ProgressTimer::Type::BAR);
	HpProccessBar->setPosition(getContentSize().width / 2, 0 - HpProccessBar->getBoundingBox().size.height * 2);
	addChild(HpProccessBar);

	CastingProccessBar = ProgressTimer::create(Sprite::create(MonsterHpProccessBar));
	CastingProccessBar->setAnchorPoint(Vec2(0.5f, 0.5f));
	CastingProccessBar->setPercentage(0);
	CastingProccessBar->setBarChangeRate(Vec2(1, 0));
	CastingProccessBar->setMidpoint(Vec2(0, 0));
	CastingProccessBar->setType(ProgressTimer::Type::BAR);
	CastingProccessBar->setPosition(HpProccessBar->getPositionX(), HpProccessBar->getPositionY() - CastingProccessBar->getBoundingBox().size.height);
	addChild(CastingProccessBar);
}

void Monster::UpdatePorccessBar()
{
	//hp
	float CurrentHp = GetUnitInt32Value(Curr_HP);
	float MaxHp = GetUnitInt32Value(Max_HP);
	float PosCurrent = CurrentHp / MaxHp * 100.0f;
	float PosNow = HpProccessBar->getPercentage();
	if (abs(PosNow - PosCurrent) > 1.0f)
	{
		if (!HpProccessBar->isVisible())
			HpProccessBar->setVisible(true);
		HpProccessBar->setPercentage(PosCurrent > PosNow ? ++PosNow : --PosNow);
	}
	else
	{
		if (HpProccessBar->isVisible())
			HpProccessBar->setVisible(false);
	}


	CastingProccessBar->setVisible(GetCastingSpell() ? true : false);
	if (GetCastingSpell())
	{
		float CurrentCssting = GetCastingSpell()->GetSpellCurrentTimeLeft();
		float MaxCasting = GetCastingSpell()->GetSpellTotalCastTime();
		float CurrPercent = CurrentCssting / MaxCasting * 100.0f;
		float PercentNow = CastingProccessBar->getPercentage();
		if (abs(PercentNow - CurrPercent > 1))
			CastingProccessBar->setPercentage(CurrPercent > PercentNow ? ++PercentNow : --PercentNow);
	}
}