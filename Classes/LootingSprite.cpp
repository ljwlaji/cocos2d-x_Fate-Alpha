#include "LootingSprite.h"
#include "Loot.h"
#include "HelloWorldScene.h"

static LootingSprite* _LootingSprite = nullptr;

/*
PlayerUILootingFrame
PlayerUILootingCloseButton
PlayerUILootingSingleIconFrame
PlayerUILootingSingleFrame
PlayerUILootingScoreBarFrame
PlayerUILootingScoreBarController
PlayerUILootingScoreBarUpButton
PlayerUILootingScoreBarButtomButton
*/
LootingSprite::LootingSprite()
{
	m_TouchedSprite = nullptr;
	initWithFile(PlayerUILootingFrame);
	autorelease();
	SetTouchType(PlayerUITouch_LootingSprite);
	Init();
	CanLootItem = false;
	setVisible(false);
}

LootingSprite::~LootingSprite()
{
	_LootingSprite = nullptr;
	removeAllChildrenWithCleanup(true);
}

LootingSprite* LootingSprite::GetInstance()
{
	if (!_LootingSprite)
		_LootingSprite = new LootingSprite();
	return _LootingSprite;
}

void LootingSprite::ScorllUpdate()
{
	if (LootList.empty())
		return;
	if (MovePoint.y < 0)
	{
		if (Sprite* psp = *LootList.begin())
		{
			if (psp->getBoundingBox().origin.y + psp->getBoundingBox().size.height < getContentSize().height * 0.9f)
				return;
		}
	}
	else
	{
		std::list<Sprite*>::iterator itr = LootList.end();
		--itr;
		if ((*itr)->getBoundingBox().origin.y > 0)
			return;
	}
	for (std::list<Sprite*>::iterator itr = LootList.begin(); itr != LootList.end(); itr++)
	{
		Sprite* TempSp = (*itr);
		TempSp->setPositionY(TempSp->getPositionY() + MovePoint.y);
		if (IsSpriteOutOfRange(TempSp) && TempSp->isVisible())
			TempSp->setVisible(false);
		else if (!IsSpriteOutOfRange(TempSp) && !TempSp->isVisible())
			TempSp->setVisible(true);
	}
}

bool LootingSprite::OnUITouchBegin(Touch* pTouch)
{
	m_TouchedSprite = nullptr;
	CanLootItem = true;
	LastPoint = pTouch->getLocation();
	MovePoint = Vec2(0, 0);
	if (CloseButton->IsContectPoint(pTouch->getLocation()))
	{
		m_TouchedSprite = CloseButton;
		return true;
	}

	for (std::list<Sprite*>::iterator itr = LootList.begin(); itr != LootList.end(); itr++)
	{
		if ((*itr)->IsContectPoint(pTouch->getLocation()))
		{
			m_TouchedSprite = *itr;
			return true;
		}
	}
	m_TouchedSprite = this;
	CanLootItem = false;
	return true;
}

void LootingSprite::OnUITouchMoved(Touch* pTouch)
{
	if (m_TouchedSprite == this)
	{
		MovePoint = ccpSub(pTouch->getLocation(), LastPoint);
		setPosition(getPositionX() + MovePoint.x, getPositionY() + MovePoint.y);
		LastPoint = pTouch->getLocation();
	}
	else
	{
		MovePoint.y = pTouch->getLocation().y - LastPoint.y;
		ScorllUpdate();
		LastPoint.y = pTouch->getLocation().y;
		CanLootItem = false;
	}
}

void LootingSprite::OnUITouchEnded(Touch* pTouch)
{
	if (!m_TouchedSprite || m_TouchedSprite == this || !m_TouchedSprite->IsContectPoint(pTouch->getLocation()))
		return;

	if (m_TouchedSprite == CloseButton)
		setVisible(false);
	else if (CanLootItem)
	{
		for (std::list<Sprite*>::iterator itr = LootList.begin(); itr != LootList.end(); itr++)
		{
			if (*itr == m_TouchedSprite)
			{
				m_Linked_Loot->Pick(m_TouchedSprite->getTag());
				m_TouchedSprite->removeFromParentAndCleanup(true);
				LootList.erase(itr);
				break;
			}
		}
		if (!LootList.size())
		{
			setVisible(false);
			return;
		}
		int k = 0;
		for (std::list<Sprite*>::iterator itr = LootList.begin(); itr != LootList.end(); itr++)
		{
			(*itr)->setPosition(getContentSize().width * 0.515f, getContentSize().height * 0.8f - (k * (*itr)->getBoundingBox().size.height));
			k++;
			if (IsSpriteOutOfRange(*itr) && (*itr)->isVisible())
				(*itr)->setVisible(false);
			else if (!IsSpriteOutOfRange(*itr) && !(*itr)->isVisible())
				(*itr)->setVisible(true);
		}
	}
}

void LootingSprite::AddNewLootVisual(uint32 Item_ID, uint32 Count)
{
	const ItemTemplate* _template = sGame->GetItemTemplate(Item_ID);
	if (_template || !Item_ID)
	{
		Sprite* SingleFrame = Sprite::create(PlayerUILootingSingleFrame);
		SingleFrame->setPosition(getContentSize().width * 0.515f, getContentSize().height * 0.8f - LootList.size() * SingleFrame->getBoundingBox().size.height);
		addChild(SingleFrame);
		SingleFrame->setTag(Item_ID);

		Sprite* ImageFrame = Sprite::create(PlayerUILootingSingleIconFrame);
		ImageFrame->setPosition(SingleFrame->getContentSize().width * 0.15f, SingleFrame->getContentSize().height / 2);
		SingleFrame->addChild(ImageFrame);

		Sprite* ImageVisual = Sprite::create(Item_ID ? _template->Url.c_str() : "Loot_Coin_Image.png");
		ImageVisual->setPosition(ImageFrame->getContentSize().width / 2, ImageFrame->getContentSize().height / 2);
		ImageFrame->addChild(ImageVisual);

		LabelTTF* TempLabel = LabelTTF::create(Item_ID ? _template->Name.c_str() : "Gold", "Arial", 30, Size::ZERO, TextHAlignment::LEFT);
		TempLabel->setAnchorPoint(Vec2(0, 0.5f));
		TempLabel->setPosition(SingleFrame->getContentSize().width * 0.3f, SingleFrame->getContentSize().height / 2);
		SingleFrame->addChild(TempLabel);


		if (LootList.size() >= 5)
			SingleFrame->setVisible(false);

		LootList.push_back(SingleFrame);
	}
}

void LootingSprite::Init()
{
	CloseButton = Sprite::create(PlayerUILootingCloseButton);

	CloseButton->setPosition(getContentSize().width - CloseButton->getBoundingBox().size.width, getContentSize().height - CloseButton->getBoundingBox().size.height);
	addChild(CloseButton);
}

void LootingSprite::ResetWithLoot(Loot* pLoot)
{
	if (pLoot)
	{
		setVisible(true);
		for (std::list<Sprite*>::iterator itr = LootList.begin(); itr != LootList.end(); itr++)
			(*itr)->removeFromParentAndCleanup(true);

		LootList.clear();
		const std::map<uint32, uint32>* SingleLootTemplate = pLoot->GetLoots();
		for (std::map<uint32, uint32>::const_iterator itr = SingleLootTemplate->begin(); itr != SingleLootTemplate->end(); itr++)
			AddNewLootVisual(itr->first, itr->second);
	}
	m_Linked_Loot = pLoot;
}