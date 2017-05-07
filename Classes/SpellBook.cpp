#include "SpellBook.h"
#include "PlayerUILayer.h"
#include "Player.h"
#pragma execution_character_set("utf-8")
static SpellBook* _SpellBook = nullptr;

SpellSlot::SpellSlot(const char* url)
{
	if (url)
		initWithFile(url);
	else
		initWithFile("Player_UI_Bag_Slot.png");
	autorelease();
	m_SpellFrame = nullptr;
	m_spellid = 0;
}

SpellSlot::~SpellSlot()
{

}

void SpellSlot::SetSlotSpell(const SpellInfo& _SpellInfo)
{
	std::string url = _SpellInfo.SpellIconUrl;
	if (url.empty())
		return;
	if (!m_SpellFrame)
	{
		m_SpellFrame = Sprite::create(url.c_str());
		m_SpellFrame->setPosition(getContentSize().width / 2, getContentSize().height / 2);
		if (getBoundingBox().size.width > 40.0f)
			m_SpellFrame->setScale(getBoundingBox().size.width / m_SpellFrame->getBoundingBox().size.width);
		addChild(m_SpellFrame);
	}
	else
		m_SpellFrame->setTexture(url.c_str());
	m_spellid = _SpellInfo.ID;
}

SpellBook::SpellBook()
{
	initWithFile(PlayerUISpellBookFrame);
	autorelease();
	InitFrame();
	InitSlot();
	setVisible(false);
	TouchedSprite = nullptr;
	LoadPlayerSpells();
	m_TouchType = SpellBookTouchType_None;
}

SpellBook::~SpellBook()
{
	_SpellBook = nullptr;
}

void SpellBook::LoadPlayerSpells()
{
	if (!sPlayer)
		return;
	PlayerSpells spells = sPlayer->GetSpells();

	int k = 0;
	SpellInfo info;
	for (PlayerSpells::iterator itr = spells.begin(); itr != spells.end(); itr++)
	{
		if (!itr->second.Active)
			continue;
		uint32 spellid = itr->first;
		info = sSpellMgr->GetSpellInfo(spellid);
		m_SpellSlotTemplate.at(k)->SetSlotSpell(info);
		uint8 Level = sSpellMgr->GetSpellCurrentLevel(info.ID, info.BaseLevelSpellID);
		//std::string url = info.SpellIconUrl;
		//if (url.empty())
		//	continue;
		//Sprite* SpellFrame = Sprite::create(url.c_str());
		//SpellFrame->setPosition(m_SpellSlotTemplate.at(k)->getContentSize().width / 2, m_SpellSlotTemplate.at(k)->getContentSize().height / 2);
		//m_SpellSlotTemplate.at(k)->SetSpellFrame(SpellFrame);
		//m_SpellSlotTemplate.at(k)->addChild(SpellFrame);
		//m_SpellSlotTemplate.at(k)->SetSpell(spellid);

		char msg[255];
		snprintf(msg, 255, "%s\n技能等级 %d",info.SpellName.c_str(), Level);
		LabelTTF* FrontLabel = LabelTTF::create(msg, "Arial", 25, Size::ZERO, TextHAlignment::LEFT);
		FrontLabel->setPosition(m_SpellSlotTemplate.at(k)->getPositionX() + getContentSize().width * 0.07f, m_SpellSlotTemplate.at(k)->getPositionY());
		FrontLabel->setAnchorPoint(Vec2(0, 0.5f));
		FrontLabel->setColor(Color3B(0, 0, 0));
		addChild(FrontLabel);
		k++;
	}
}

void SpellBook::InitSlot()
{
	float PosX = 0;
	float PosY = 0;
	for (int i = 0; i != MaxSlotPerSpellBookPage; i++)
	{
		i % 2 ? PosX = getContentSize().width * 0.6f : PosX = getContentSize().width * 0.22f;
		uint8 Row = i / 2;
		PosY = getContentSize().height * 0.833f - (Row *  getContentSize().height * 0.108f);
		SpellSlot* TempSpellSlot = new SpellSlot();
		TempSpellSlot->setPosition(PosX, PosY);
		addChild(TempSpellSlot);
		m_SpellSlotTemplate.push_back(TempSpellSlot);
	}
}

bool SpellBook::onTouchBagBegan(Touch* touches)
{
	TouchedSprite = nullptr;
	m_TouchType = SpellBookTouchType_None;
	for (int i = SpellButtonTag_Close; i != SpellButtonTag_End; i++)
	{
		if (Sprite* TempPagTag = (Sprite*)getChildByTag(i))
		{
			if (TempPagTag->IsContectPoint(touches->getLocation()))
			{
				m_TouchType = SpellBookTouchType_Button;
				TouchedSprite = TempPagTag;
				return true;
			}
		}
	}

	for (int i = 0; i != m_SpellSlotTemplate.size(); i++)
	{
		SpellSlot* TempSlot = m_SpellSlotTemplate.at(i);
		if (TempSlot && TempSlot->IsContectPoint(touches->getLocation()))
		{
			m_TouchType = SpellBookTouchType_Slot;
			m_Start_Move_Position = touches->getLocation();
			TouchedSprite = TempSlot;
			return true;
		}
	}
	m_Start_Move_Position = touches->getLocation();
	TouchedSprite = this;
	return true;
}

void SpellBook::onTouchBagMoved(Touch* touches)
{
	if (TouchedSprite == this)
	{
		float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
		float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
		setPosition(getPositionX() + X_Modify, getPositionY() + Y_Modify);
		m_Start_Move_Position = touches->getLocation();
		sPlayerUi->ReSetSpellFramePosition();
	}
	else if (m_TouchType == SpellBookTouchType_MoveSlot || m_TouchType == SpellBookTouchType_Slot)
	{
		float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
		float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
		((SpellSlot*)TouchedSprite)->GetSpellFrame()->setPosition(((SpellSlot*)TouchedSprite)->GetSpellFrame()->getPositionX() + X_Modify, ((SpellSlot*)TouchedSprite)->GetSpellFrame()->getPositionY() + Y_Modify);
		m_Start_Move_Position = touches->getLocation();
		m_TouchType = SpellBookTouchType_MoveSlot;
	}
}

void SpellBook::onTouchBagEnded(Touch* touches)
{
	if (!isVisible())
		return;
	switch (m_TouchType)
	{
	case SpellBookTouchType_Button:
		HandleTouchButton((SpellBookButtonTag)TouchedSprite->getTag());
		break;
	case SpellBookTouchType_Slot:
		if (SpellSlot* TempSlot = (SpellSlot*)TouchedSprite)
		{
			if (!TempSlot->GetSpellId())
				return;
			sPlayerUi->SwapSpellFrameVisable();
			sPlayerUi->ReSetSpellFrameText(TempSlot->GetSpellId());
		}
		break;
	case SpellBookTouchType_MoveSlot:
		if (SpellSlot* TempTouchedSlot = sPlayerUi->GetContactButtonSlot(touches->getLocation()))
		{
			SpellInfo pInfo = sSpellMgr->GetSpellInfo(((SpellSlot*)TouchedSprite)->GetSpellId());
			if (pInfo.ID)
			{
				TempTouchedSlot->SetSlotSpell(pInfo);
			}
			log("touched");
		}
		((SpellSlot*)TouchedSprite)->GetSpellFrame()->setPosition(TouchedSprite->getContentSize().width / 2, TouchedSprite->getContentSize().height / 2);
		break;
	}
}

void SpellBook::SwapVisiable()		
{
	isVisible() ? setVisible(false) : setVisible(true);
	if (!isVisible())
		sPlayerUi->SwapSpellFrameVisable();
}

void SpellBook::HandleTouchButton(SpellBookButtonTag _tag)
{
	switch (_tag)
	{
	case SpellButtonTag_Close:
		SwapVisiable();
		break;
	case SpellButtonTag_Tag_1:
		break;
	case SpellButtonTag_Tag_2:
		break;
	case SpellButtonTag_Left_Arrow:
		break;
	case SpellButtonTag_Right_Arrow:
		break;
	}
}

void SpellBook::InitFrame()
{
	m_CloseButton = Sprite::create("Player_UI_Spell_Close.png");
	m_CloseButton->setPosition(getContentSize().width * 0.98f, getContentSize().height - m_CloseButton->getBoundingBox().size.height * 0.75f);
	m_CloseButton->setTag(SpellButtonTag_Close);
	addChild(m_CloseButton);

	m_Tag_1 = Sprite::create("Player_UI_Spell_Tab_1.png");
	m_Tag_1->setPosition(getContentSize().width * 1.03f, getContentSize().height * 0.85f);
	m_Tag_1->setTag(SpellButtonTag_Tag_1);
	addChild(m_Tag_1);

	m_Tag_2 = Sprite::create("Player_UI_Spell_Tab_2.png");
	m_Tag_2->setPosition(getContentSize().width * 1.03f, getContentSize().height * 0.75f);
	m_Tag_2->setTag(SpellButtonTag_Tag_2);
	addChild(m_Tag_2);

	m_SpellPage = Sprite::create("Player_UI_Spell_Page.png");
	m_SpellPage->setPosition(getContentSize().width * 0.955f, getContentSize().height * 0.2f);
	addChild(m_SpellPage);


	m_LeftArrow = Sprite::create("Player_UI_Spell_Book_Left.png");
	m_LeftArrow->setPosition(getContentSize().width * 1.025f, getContentSize().height * 0.23f);
	m_LeftArrow->setTag(SpellButtonTag_Left_Arrow);
	addChild(m_LeftArrow);
	m_RightArrow = Sprite::create("Player_UI_Spell_Right.png");
	m_RightArrow->setPosition(getContentSize().width * 1.025f, getContentSize().height * 0.17f);
	m_RightArrow->setTag(SpellButtonTag_Right_Arrow);
	addChild(m_RightArrow);
}

SpellBook* SpellBook::GetInstance()
{
	if (!_SpellBook)
		_SpellBook = new SpellBook();

	return _SpellBook;
}
