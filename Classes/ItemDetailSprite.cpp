#include "ItemDetailSprite.h"
#include "HelloWorldScene.h"
#include "Item.h"
static ItemDetailSprite* _ItemDetailSprite = nullptr;

ItemDetailSprite::ItemDetailSprite()
{
	initWithFile(PlayerUIItemDetailFrameTop);
	autorelease();
	InitDefault();
	setVisible(false);
	SetTouchType(PlayerUITouch_ItemDetailSprite);
}

ItemDetailSprite::~ItemDetailSprite()
{
	_ItemDetailSprite = nullptr;
}

ItemDetailSprite* ItemDetailSprite::GetInstance()
{
	if (!_ItemDetailSprite)
		_ItemDetailSprite = new ItemDetailSprite();
	return _ItemDetailSprite;
}

bool ItemDetailSprite::OnUITouchBegin(Touch* pTouch) 
{
	LastPoint = pTouch->getLocation();
	return true;
}
void ItemDetailSprite::OnUITouchMoved(Touch* pTouch) 
{
	if (!isVisible())
		return;
	MovePoint = ccpSub(pTouch->getLocation(), LastPoint);
	setPosition(getPositionX() + MovePoint.x, getPositionY() + MovePoint.y);
	LastPoint = pTouch->getLocation();
}
void ItemDetailSprite::OnUITouchEnded(Touch* pTouch) 
{
}

void ItemDetailSprite::InitDefault()
{
	ItemNameTTF = LabelTTF::create("TestName", "Airal", 34, Size::ZERO, TextHAlignment::LEFT);
	ItemNameTTF->setAnchorPoint(Vec2(0, 0.5f));
	ItemNameTTF->setPosition(getContentSize().width * 0.08f, getContentSize().height * 0.75f);
	addChild(ItemNameTTF);

	ItemSlotTTF = LabelTTF::create("TestSlot", "Airal", 26, Size::ZERO, TextHAlignment::LEFT);
	ItemSlotTTF->setAnchorPoint(Vec2(0, 0.5f));
	ItemSlotTTF->setPosition(ItemNameTTF->getPositionX(), ItemNameTTF->getPositionY() - ItemSlotTTF->getBoundingBox().size.height * 1.3f);
	addChild(ItemSlotTTF);

	ItemDamageTTF = LabelTTF::create("1111 - 2222 Physics Damage", "Airal", 26, Size::ZERO, TextHAlignment::LEFT);
	ItemDamageTTF->setAnchorPoint(Vec2(0, 0.5f));
	ItemDamageTTF->setPosition(ItemSlotTTF->getPositionX(), ItemSlotTTF->getPositionY() - ItemDamageTTF->getBoundingBox().size.height * 1.3f);
	addChild(ItemDamageTTF);



	ButtomFrame = Sprite::create(PlayerUIItemDetailFrameButtom);
	ButtomFrame->setAnchorPoint(Vec2(0.5f, 1.0f));
	ButtomFrame->setPosition(getContentSize().width / 2, 0 - (5 * MiddlePosY));
	addChild(ButtomFrame);


	ItemRequireClassTTF = LabelTTF::create("Class: Saber", "Airal", 26, Size::ZERO, TextHAlignment::LEFT);
	ItemRequireClassTTF->setPosition(ItemDamageTTF->getPositionX(), ButtomFrame->getContentSize().height);
	ItemRequireClassTTF->setAnchorPoint(Vec2(0, 1.0f));
	ButtomFrame->addChild(ItemRequireClassTTF);

	ItemRequireLevelTTF = LabelTTF::create("Level: 12", "Airal", 26, Size::ZERO, TextHAlignment::LEFT);
	ItemRequireLevelTTF->setPosition(ItemDamageTTF->getPositionX(), ItemRequireClassTTF->getPositionY() - ItemRequireClassTTF->getBoundingBox().size.height * 1.3f);
	ItemRequireLevelTTF->setAnchorPoint(Vec2(0, 1.0f));
	ButtomFrame->addChild(ItemRequireLevelTTF);

	float LastWeith = 0;
	for (int i = 0; i != 3; i++)
	{
		LabelTTF* TempItemSellPriceTTF = LabelTTF::create("Price: 999", "Airal", 26, Size::ZERO, TextHAlignment::RIGHT);
		switch (i)
		{
		case 0:
			TempItemSellPriceTTF->setPosition(getContentSize().width * 0.34f, ItemRequireLevelTTF->getPositionY() - ItemRequireLevelTTF->getBoundingBox().size.height * 1.3f);
			TempItemSellPriceTTF->setAnchorPoint(Vec2(1.0f, 1.0f));
			break;
		case 1:
			TempItemSellPriceTTF->setPosition(getContentSize().width / 2, ItemRequireLevelTTF->getPositionY() - ItemRequireLevelTTF->getBoundingBox().size.height * 1.3f);
			TempItemSellPriceTTF->setAnchorPoint(Vec2(1.0f, 1.0f));
			break;
		case 2:
			TempItemSellPriceTTF->setPosition(getContentSize().width * 0.65f, ItemRequireLevelTTF->getPositionY() - ItemRequireLevelTTF->getBoundingBox().size.height * 1.3f);
			TempItemSellPriceTTF->setAnchorPoint(Vec2(1.0f, 1.0f));
			break;
		}
		ButtomFrame->addChild(TempItemSellPriceTTF);
		ItemSellPriceTTF.push_back(TempItemSellPriceTTF);
		LastWeith = TempItemSellPriceTTF->getPositionX() - ItemDamageTTF->getPositionX();
	}
}

void ItemDetailSprite::ResetMiddleFrame(const ItemTemplate* ValueCount)
{
	for (std::list<Sprite*>::iterator itr = MiddleSprites.begin(); itr != MiddleSprites.end(); itr++)
		(*itr)->removeFromParentAndCleanup(true);
	MiddleSprites.clear();

	int i = 0;
	for (ItemValueInfo::const_iterator itr = ValueCount->Values.begin(); itr != ValueCount->Values.end(); itr++)
	{
		Sprite* TestValueFrame = Sprite::create(PlayerUIItemDetailFrameMiddle);
		TestValueFrame->setPosition(getContentSize().width / 2, 0 - (i * TestValueFrame->getBoundingBox().size.height));
		TestValueFrame->setAnchorPoint(Vec2(0.5f, 1.0f));
		addChild(TestValueFrame);
		MiddleSprites.push_back(TestValueFrame);
		MiddlePosY = TestValueFrame->getBoundingBox().origin.y;

		char msg[255];
		snprintf(msg, 255, "+ %d %s", itr->second, sGame->GetUnitValueNameByID((UnitInt32Value)itr->first).c_str());
		LabelTTF* TestValueString = LabelTTF::create(msg, "Airal", 26, Size::ZERO, TextHAlignment::LEFT);
		TestValueString->setAnchorPoint(Vec2(0, 0.5f));
		TestValueString->setPosition(ItemDamageTTF->getPositionX(), TestValueFrame->getContentSize().height / 2);
		TestValueFrame->addChild(TestValueString);
		++i;
	}
}

void ItemDetailSprite::ShowWithItem(Item* pItem)
{
	if (!pItem)
		return;
	if (const ItemTemplate* TempTemplate = pItem->GetTemplate())
	{
		ResetMiddleFrame(TempTemplate);
		ResetDefaultString(TempTemplate);
		ResetButtomFramePosition();
		setVisible(true);
		m_DisappearTime = 5.0f;
		scheduleUpdate();
	}
}

void ItemDetailSprite::ResetButtomFramePosition()
{
	ButtomFrame->setPositionY(MiddlePosY);
}

void ItemDetailSprite::ResetDefaultString(const ItemTemplate* Value)
{
	std::string reset = "";
	reset = Value->Name;
	ItemNameTTF->setString(reset.c_str());
	ItemSlotTTF->setString(GetSlotStringBuyType(Value->SlotType));
	char dmg[255];
	snprintf(dmg, 255, "%d - %d Damage", Value->MinDamage, Value->MaxDamage);
	ItemDamageTTF->setString(dmg);

	std::string Require = "Class: ";
	Require += sGame->GetClassNameByClassID(Value->RequireClass);
	ItemRequireClassTTF->setString(Require.c_str());
	snprintf(dmg, 255, "Require Level: %d", Value->RequireLevel);
	ItemRequireLevelTTF->setString(dmg);

	uint8 Gold = Value->SellPrice / 10000;
	uint8 Sliv = (Value->SellPrice % 10000) / 100;
	uint8 Coin = (Value->SellPrice % 10000) % 100;
	snprintf(dmg, 255, "Price: %d", Gold);
	ItemSellPriceTTF.at(0)->setString(dmg);

	snprintf(dmg, 255, "%d", Sliv);
	ItemSellPriceTTF.at(1)->setString(dmg);

	snprintf(dmg, 255, "%d", Coin);
	ItemSellPriceTTF.at(2)->setString(dmg);
}

std::string ItemDetailSprite::GetSlotStringBuyType(PlayerEquipSlots _slot)
{
	std::string re = "";
	switch (_slot)
	{
	case SLOT_WEAPON:
		re = "Main-Hand";
		break;
	case SLOT_SECOND_WEAPON:
		re = "Off-Hand";
		break;
	case SLOT_LEFT_HAND:
	case SLOT_RIGHT_HAND:
		re = "Hand";
		break;
	case SLOT_RING_1:
	case SLOT_RING_2:
		re = "Ring";
		break;
	case SLOT_HEAD:
		re = "Head";
		break;
	case SLOT_SHOULDER:
		re = "Shoulder";
		break;
	default:
		re = "UnKnow";
		break;
	}
	return re;
}

void ItemDetailSprite::update(float diff)
{
	if (m_DisappearTime <= diff)
	{
		setVisible(false);
		unscheduleUpdate();
	}
	else m_DisappearTime -= diff;
}