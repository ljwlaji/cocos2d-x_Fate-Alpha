#include "EnterGameLayer.h"
#include "HelloWorldScene.h"
#include "TypingLayer.h"
#include "MainMapLayer.h"
#include "Player.h"
EnterGameLayer::EnterGameLayer()
{
	Visablesize = Director::getInstance()->getVisibleSize();
	OrginSize = Director::getInstance()->getVisibleOrigin();
	setTag(EnterGame_Layer_Tag);
	m_TouchedButton = -1;
	SettingMenu = nullptr;
	DiffcuteMenu = nullptr;
}

EnterGameLayer::~EnterGameLayer()
{

}

bool EnterGameLayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		float SingleSize = Visablesize.y * 0.1f;
		for (int i = button_entergame; i != menu_settings; i++)
		{
			char url[255];
			snprintf(url, 255, "EnterGame_%u.png", i);
			Sprite* TempButton = Sprite::create(url);
			TempButton->SetRealPosition(Visablesize.x / 2, Visablesize.y * 0.4 - (i * SingleSize));
			TempButton->setOpacity(0.0f);
			TempButton->setTag(i);
			this->addChild(TempButton);
		}
		EveryThingFadeIn();
		InitListener();
		InitSettingMenu();
		InitDiffcuteMenu();
		bRef = true;
	} while (0);

	return bRef;
}


void EnterGameLayer::EveryThingFadeIn()
{
	float DelayTime = 0;
	for (int i = button_entergame; i < button_exit + 1; i++)
	{
		if (Sprite* Temp = (Sprite*)getChildByTag(i))
		{
			Temp->runAction(CCSequence::create(CCDelayTime::create(DelayTime), CCFadeIn::create(1.0f), NULL));
		}
		DelayTime += 1;
	}
}

void EnterGameLayer::EveryThingFadeOut()
{
	for (int i = button_entergame; i < button_exit + 1; i++)
	{
		if (Sprite* Temp = (Sprite*)getChildByTag(i))
		{
			Temp->runAction(CCFadeOut::create(2.0f));
		}
	}
}

bool EnterGameLayer::RunActionWithButton(int _tag)
{
	switch (_tag)
	{
	case button_entergame:
		if (Main_Map_Layer* layer = new Main_Map_Layer(1))
		{
			layer->init();
			layer->autorelease();
			sGame->SwapLayer(layer, getTag());
			return false;
		}
		break;
	case button_settings:
		SettingMenu->setVisible(true);
		MainMenuDisAppear();
		break;
	case button_diffcute:
		DiffcuteMenu->setVisible(true);
		MainMenuDisAppear();
		break;
	case button_exit:
		Director::getInstance()->end();
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
		break;
	case setting_button_cancel:
		SettingMenu->setVisible(false);
		MainMenuShow();
		break;
	case setting_button_one://难度设置
		sGame->GetDiffCute() != Rate_S ? sGame->SetDiffCute(sGame->GetDiffCute() + 1) : sGame->SetDiffCute(Rate_C);
		if (Sprite* temp = sGame->ShowDiffcuteImage())
		{
			temp->setPosition(Visablesize.x / 2, Visablesize.y / 2);
			addChild(temp);
		}
		break;
	case setting_button_two://音量开关
		sGame->GetCanPlaySound() ? sGame->SetCanPlaySound(false) : sGame->SetCanPlaySound(true);
		if (Sprite* Temp = sGame->ShowSoundImage())
		{
			Temp->setPosition(Visablesize.x / 2, Visablesize.y / 2);
			addChild(Temp);
		}
		break;
	case setting_button_three://连招提示
		sGame->GetCombatSign() ? sGame->SetCombatSign(false) : sGame->SetCombatSign(true);
		if (Sprite* Temp = sGame->ShowCombatSignImage())
		{
			Temp->setPosition(Visablesize.x / 2, Visablesize.y / 2);
			addChild(Temp);
		}
		break;
	case setting_button_four:
		break;
	case setting_button_five:
		break;
	case setting_button_end:
		break;
	case Diffcute_button_cancel:
		DiffcuteMenu->setVisible(false);
		MainMenuShow();
		break;
	case Diffcute_button_one:
		break;
	case Diffcute_button_two:
		break;
	case Diffcute_button_three:
		break;
	case Diffcute_button_four:
		break;
	case Diffcute_button_five:
		break;
	case Diffcute_button_end:
		break;
	case end_of_button_tag:
		break;
	default:
		break;
	}
	return true;
}

void EnterGameLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	Sprite* Temp = nullptr;
	if (m_TouchedButton <= button_exit)
	{
		Temp = (Sprite*)getChildByTag(m_TouchedButton);
	}
	else if (m_TouchedButton <= setting_button_end)
	{
		Temp = (Sprite*)SettingMenu->getChildByTag(m_TouchedButton);
	}
	else
	{
		Temp = (Sprite*)DiffcuteMenu->getChildByTag(m_TouchedButton);
	}
	if (Temp)
	{
		Temp->setScale(1.0f);
		if (Temp->IsContectPoint(touch->getLocation()))
		{
			if (Temp->isVisible())
			if (!RunActionWithButton(Temp->getTag()))
				return;
		}
	}
	m_TouchedButton = -1;
}

bool EnterGameLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	for (int i = button_entergame; i < button_exit + 1; i++)
	{
		if (Sprite* Temp = (Sprite*)getChildByTag(i))
		{
			if (Temp->isVisible() && Temp->IsContectPoint(touch->getLocation()))
			{
				log("%d", Temp->getTag());
				Temp->setScale(0.8f);
				m_TouchedButton = i;
				return true;
			}
		}
	}
	for (int i = menu_settings; i < menu_diffcute + 1; i++)
	{
		if (Sprite* aTemp = (Sprite*)getChildByTag(i))
		{
			if (aTemp->IsContectPoint(touch->getLocation()) && aTemp->isVisible())
			{
				for (int j = setting_button_cancel; j < Diffcute_button_end; j++)
				{
					if (Sprite* bTemp = (Sprite*)aTemp->getChildByTag(j))
					{
						if (bTemp->IsContectPoint(touch->getLocation()) && bTemp->isVisible())
						{
							bTemp->setScale(0.8f);
							m_TouchedButton = j;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void EnterGameLayer::InitSettingMenu()
{
	SettingMenu = Sprite::create("EnterGame_Menu_BackGround.png");
	SettingMenu->SetRealPosition(Visablesize.x / 2, Visablesize.y / 2);
	SettingMenu->setVisible(false);
	SettingMenu->setTag(menu_settings);
	Sprite* Cancel = Sprite::create("EnterGame_Menu_CaccelButton.png");
	Cancel->setPosition(SettingMenu->getContentSize().width - Cancel->getBoundingBox().size.width / 2, SettingMenu->getContentSize().height - Cancel->getBoundingBox().size.height / 2);
	Cancel->setTag(setting_button_cancel);
	SettingMenu->addChild(Cancel);

	for (int i = setting_button_one; i < setting_button_four; i++)
	{
		char msg[255];
		snprintf(msg, 255, "Setting_Button_%d.png", i - 7);
		Sprite* Temp = Sprite::create(msg);
		Temp->setPosition(SettingMenu->getContentSize().width / 2, SettingMenu->getContentSize().height / 3 + ((i - 7) * SettingMenu->getContentSize().height / 6));
		Temp->setTag(i);
		SettingMenu->addChild(Temp);
	}
	addChild(SettingMenu);

}

void EnterGameLayer::InitDiffcuteMenu()
{
	DiffcuteMenu = Sprite::create("EnterGame_Menu_BackGround.png");
	DiffcuteMenu->SetRealPosition(Visablesize.x / 2, Visablesize.y / 2);
	DiffcuteMenu->setVisible(false);
	DiffcuteMenu->setTag(menu_diffcute);
	addChild(DiffcuteMenu);
	Sprite* Cancel = Sprite::create("EnterGame_Menu_CaccelButton.png");
	Cancel->setPosition(DiffcuteMenu->getContentSize().width - Cancel->getBoundingBox().size.width / 2, DiffcuteMenu->getContentSize().height - Cancel->getBoundingBox().size.height / 2);
	Cancel->setTag(Diffcute_button_cancel);
	DiffcuteMenu->addChild(Cancel);
}

void EnterGameLayer::InitListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(EnterGameLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(EnterGameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void EnterGameLayer::MainMenuShow()
{
	for (int i = button_entergame; i < button_exit + 1; i++)
	{
		if (Sprite* Temp = (Sprite*)getChildByTag(i))
		{
			Temp->setVisible(true);
		}
	}
}

void EnterGameLayer::MainMenuDisAppear()
{
	for (int i = button_entergame; i < button_exit + 1; i++)
	{
		if (Sprite* Temp = (Sprite*)getChildByTag(i))
		{
			Temp->setVisible(false);
		}
	}
}