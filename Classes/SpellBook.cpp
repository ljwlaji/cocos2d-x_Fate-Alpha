#include "SpellBook.h"

static SpellBook* _SpellBook = nullptr;

SpellSlot::SpellSlot()
{
	initWithFile("Player_UI_Bag_Slot.png");
	autorelease();
}

SpellSlot::~SpellSlot()
{
}

SpellBook::SpellBook()
{
	initWithFile(PlayerUISpellBookFrame);
	autorelease();
	InitFrame();
	InitSlot();
	setVisible(false);
	TouchedSprite = nullptr;
	m_TouchType = SpellBookTouchType_None;
}

SpellBook::~SpellBook()
{
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
	}
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

void SpellBook::onTouchBagEnded(Touch* touches)
{
	switch (m_TouchType)
	{
	case SpellBook::SpellBookTouchType_Button:
		HandleTouchButton((SpellBookButtonTag)TouchedSprite->getTag());
		break;
	case SpellBook::SpellBookTouchType_Slot:
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
