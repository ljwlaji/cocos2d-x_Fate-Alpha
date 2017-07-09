#include "LearnSpellSprite.h"
#include "DataMgr.h"
#include "HelloWorldScene.h"
#include "SpellMgr.h"

static LearnSpellSprite* _LearnSpellSprite = nullptr;

LearnSpellSprite::LearnSpellSprite()
{
	initWithFile(PlayerUILearnSpellSpriteFrame);
	autorelease();
	SetTouchType(PlayerUITouch_LearnSpellSprite);
	setVisible(false);
	CloseButton = Sprite::create(PlayerUIVendorCloseButton);
	CloseButton->setPosition(getContentSize().width - CloseButton->getBoundingBox().size.width, getContentSize().height - CloseButton->getBoundingBox().size.height);
	addChild(CloseButton);
	m_TouchedSprite = nullptr;
}

LearnSpellSprite::~LearnSpellSprite()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_LearnSpellSprite = nullptr;
}

void LearnSpellSprite::AddNewSpellToList(const SpellInfo& spellinfo, uint32 Cost, uint32 Level)
{
	Sprite* TempFrame = Sprite::create(PlayerUILearnSpellSpriteSingleFrame);
	TempFrame->setPosition(getContentSize().width / 2, getContentSize().height * 0.85f - (SingleFrameList.size() * TempFrame->getBoundingBox().size.height));
	TempFrame->setTag(spellinfo.ID);
	addChild(TempFrame);
	SingleFrameList.push_back(TempFrame);

	Sprite* TempIcon = Sprite::create(spellinfo.SpellIconUrl.c_str());
	TempIcon->setPosition(TempIcon->getBoundingBox().size.width * 0.7f, TempFrame->getContentSize().height / 2);
	TempFrame->addChild(TempIcon);

	LabelTTF* TempSpellName = LabelTTF::create(spellinfo.SpellName.c_str(), "Arial", 28, Size::ZERO, TextHAlignment::LEFT);
	TempSpellName->setAnchorPoint(Vec2(0.0f, 0.5f));
	TempSpellName->setPosition(TempFrame->getContentSize().width * 0.33f, TempFrame->getContentSize().height * 0.5f + TempSpellName->getBoundingBox().size.height / 2);
	TempFrame->addChild(TempSpellName);

	char msg[255];
	snprintf(msg, 255, "%d,Slev:%d", Cost, sSpellMgr->GetSpellCurrentLevel(spellinfo.ID, spellinfo.BaseLevelSpellID));
	LabelTTF* TempSpellPrice = LabelTTF::create(msg, "Arial", 28, Size::ZERO, TextHAlignment::LEFT);
	TempSpellPrice->setAnchorPoint(Vec2(0.0f, 0.5f));
	TempSpellPrice->setPosition(TempFrame->getContentSize().width * 0.33f, TempFrame->getContentSize().height / 2 - TempSpellPrice->getBoundingBox().size.height / 2);
	TempFrame->addChild(TempSpellPrice);

	snprintf(msg, 255, "%d", Level);
	LabelTTF* TempSpellLevel = LabelTTF::create(msg, "Arial", 28, Size::ZERO, TextHAlignment::LEFT);
	TempSpellLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
	TempSpellLevel->setPosition(TempFrame->getContentSize().width * 0.83f, TempFrame->getContentSize().height / 2 - TempSpellLevel->getBoundingBox().size.height / 2);
	TempFrame->addChild(TempSpellLevel);
}

void LearnSpellSprite::InitWithCreature(uint32 creatureid)
{
	for (std::list<Sprite*>::iterator itr = SingleFrameList.begin(); itr != SingleFrameList.end(); itr++)
		(*itr)->removeFromParentAndCleanup(true);
	SingleFrameList.clear();

	if (const std::list<SingleTrainerSpell>* TempList = sGame->GetSingleTrainerList(creatureid))
	{
		for (std::list<SingleTrainerSpell>::const_iterator itr = TempList->begin(); itr != TempList->end(); itr++)
		{
			if (itr->SpellID == 30)
			{
				int i = 30;
			}
			if (sPlayer->HasSpell(itr->SpellID))
				continue;
			SpellInfo _spellinfo = sSpellMgr->GetSpellInfo(itr->SpellID);
			if (_spellinfo.ID)
			{
				uint32 FrontSpellID = sSpellMgr->GetSpellFrontLevelID(itr->SpellID, _spellinfo.BaseLevelSpellID);
				//				存在上级且玩家未获取上级技能的情况
				if (FrontSpellID && !sPlayer->HasSpell(FrontSpellID))
					continue;
				AddNewSpellToList(_spellinfo, itr->NeedMoney, itr->NeedLevel);
			}
		}
	}
	m_CreatureID = creatureid;
	setVisible(true);
}

LearnSpellSprite* LearnSpellSprite::GetInstance()
{
	if (!_LearnSpellSprite)
		_LearnSpellSprite = new LearnSpellSprite();
	return _LearnSpellSprite;
}

bool LearnSpellSprite::OnUITouchBegin(Touch* pTouch)
{
	if (CloseButton->IsContectPoint(pTouch->getLocation()))
	{
		m_TouchedSprite = CloseButton;
		return true;
	}
	for (std::list<Sprite*>::iterator itr = SingleFrameList.begin(); itr != SingleFrameList.end(); itr++)
	{
		if ((*itr)->IsContectPoint(pTouch->getLocation()))
		{
			m_TouchedSprite = (*itr);
			return true;
		}
	}
	return true;
}
void LearnSpellSprite::OnUITouchMoved(Touch* pTouch)
{

}
void LearnSpellSprite::OnUITouchEnded(Touch* pTouch)
{
	if (m_TouchedSprite == CloseButton)
		setVisible(false);
	else
	{
		sPlayer->LearnSpell(m_TouchedSprite->getTag());
		InitWithCreature(m_CreatureID);
	}
}