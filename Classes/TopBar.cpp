#include "TopBar.h"
#include "PlayerUILayer.h"
#include "MainMapLayer.h"
#include "Player.h"
#include "PlayerUISettingSprite.h"
#include "SpellBook.h"
#include "QuestBook.h"
#include "PlayerBag.h"
#include "PlayerEquipWindow.h"

static TopBar* _TopBar = nullptr;

TopBar::TopBar()
{
	initWithFile(PlayerUIButtomMenuImage);
	autorelease();
	SetRealPosition(sMainMap->GetVisableSize().x / 2, sMainMap->GetVisableSize().y - getBoundingBox().size.height / 2);
	Init();
}

TopBar::~TopBar()
{
	removeFromParentAndCleanup(true);
	_TopBar = nullptr;
}

void TopBar::Init()
{
	//character equip bag quest spell setting
	char msg[255];
	float PosX = 0;
	for (int i = Button_Menu_Setting; i != Button_Menu_End; i++)
	{
		snprintf(msg, 255, "%s%d.png", PlayerUIButtonMenuListImage, i);
		Sprite* TempSprite = Sprite::create(msg);
		TempSprite->setPosition(getContentSize().width * 0.882f - TempSprite->getBoundingBox().size.width / 2 - (i * TempSprite->getBoundingBox().size.width * 0.9f), getContentSize().height * 0.65f);
		addChild(TempSprite);
		m_Buttom_Menus.push_back(TempSprite);
		PosX = TempSprite->getPositionX();
	}
	Sprite* MapButton = Sprite::create("Player_UI_Buttom_Menu_6.png");
	MapButton->setPosition(PosX - MapButton->getBoundingBox().size.width * 0.52f, getContentSize().height * 0.55f);
	addChild(MapButton);
	m_Buttom_Menus.push_back(MapButton);

	Sprite* NameFrame = Sprite::create("Player_UI_Buttom_Menu_7.png");
	Sprite* ValueFrame = Sprite::create("Player_UI_Buttom_Menu_8.png");
	Sprite* HeadFrame = Sprite::create("Player_UI_Buttom_Menu_9.png");

	NameFrame->setPosition(MapButton->getBoundingBox().origin.x - NameFrame->getBoundingBox().size.width * 0.4f, getContentSize().height * 0.55f + NameFrame->getBoundingBox().size.height / 2);
	addChild(NameFrame);

	ValueFrame->setPosition(NameFrame->getPositionX(), getContentSize().height * 0.5f);
	addChild(ValueFrame);

	HeadFrame->setPosition(ValueFrame->getBoundingBox().origin.x - HeadFrame->getBoundingBox().size.width * 0.35f, getContentSize().height * 0.62f);
	addChild(HeadFrame);

	float FirstPoint = ValueFrame->getBoundingBox().size.width / 9;
	float SinglePoint = ValueFrame->getBoundingBox().size.width / 3;
	for (int i = MoneyTTF; i != EndOfTopTTF; i++)
	{
		LabelTTF* TempTTF = LabelTTF::create("1234", "Arial", 20, Size::ZERO, i == NameTTF ? TextHAlignment::CENTER : TextHAlignment::LEFT);
		i == NameTTF ? TempTTF->setAnchorPoint(Vec2(0.5f, 0.5f)) : TempTTF->setAnchorPoint(Vec2(0, 0.5f));
		i == NameTTF ? TempTTF->setPosition(NameFrame->getContentSize().width / 2, NameFrame->getContentSize().height) : TempTTF->setPosition(FirstPoint + (i * SinglePoint), ValueFrame->getContentSize().height / 2);
		ValueFrame->addChild(TempTTF);
		TopMenuLabel[(TopButtonLabelTTF)i] = TempTTF;
	}
	ReSetTopBarString();
}

void TopBar::ReSetTopBarString()
{
	for (std::map<TopButtonLabelTTF, LabelTTF*>::iterator itr = TopMenuLabel.begin(); itr != TopMenuLabel.end(); itr++)
	{
		LabelTTF* TempTTF = itr->second;
		char msg[255];
		switch (itr->first)
		{
		case MoneyTTF:
			snprintf(msg, 255, "%d", sPlayer->GetMoney());
			break;
		case AttackTTF:
			snprintf(msg, 255, "%d", sPlayer->GetUnitInt32Value(Base_Att));
			break;
		case CashTTF:
			snprintf(msg, 255, "%d", 99999);
			break;
		case NameTTF:
			snprintf(msg, 255, "%s", sPlayer->GetName().c_str());
			break;
		}
		TempTTF->setString(msg);
	}
}

TopBar* TopBar::GetInstance()
{
	if (!_TopBar)
		_TopBar = new TopBar();
	return _TopBar;
}

bool TopBar::OnUITouchBegin(Touch* pTouch)
{
	for (int i = 0; i != m_Buttom_Menus.size(); i++)
	{
		if (Sprite* TempButton = m_Buttom_Menus.at(i))
		{
			if (TempButton->IsContectPoint(pTouch->getLocation()))
			{
				UISprite* TempUISprite = nullptr;
				switch (i)
				{
				case Button_Menu_Setting:
					sSettingMenu->OnClickSettingButton();
					TempUISprite = sSettingMenu;
					break;
				case Button_Menu_Spell:
					sPlayerSpellBook->SwapVisiable();
					TempUISprite = sPlayerSpellBook;
					break;
				case Button_Menu_Quest:
					sQuestBook->SwapVisable();
					TempUISprite = sQuestBook;
					break;
				case Button_Menu_Bag:
					sPlayerBag->SwapVisiable();
					TempUISprite = sPlayerBag;
					break;
				case Button_Menu_Equip:
					sPlayerEquip->SwapVisiable();
					TempUISprite = sPlayerEquip;
					break;
				case Button_Menu_Character:
					sPlayerValueWindow->SwapVisable();
					TempUISprite = sPlayerEquip;
					break;
				}
				if (TempUISprite)
					sPlayerUi->PushSprite(TempUISprite);
				return true;
			}
		}
	}
	return false;
}

void TopBar::OnUITouchEnded(Touch* pTouch)
{

}