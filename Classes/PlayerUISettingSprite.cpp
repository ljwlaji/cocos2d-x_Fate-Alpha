#include "PlayerUISettingSprite.h"
#include "PlayerUILayer.h"
#include "HelloWorldScene.h"

static SettingMenu* _SettingMenu = nullptr;

SettingMenu::SettingMenu()
{
	initWithFile(PlayerUISettingMenuFrame);
	autorelease();
	setVisible(false);
	Init();
}

SettingMenu::~SettingMenu()
{
	_SettingMenu = nullptr;
	removeAllChildrenWithCleanup(true);
}

void SettingMenu::Init()
{
	char msg[255];
	for (int i = TAG_RESUME_GAME; i != MaxSettingButton; i++)
	{
		Sprite* ButtonNormal = Sprite::create(PlayerUISettingMenuButtonNormal);
		ButtonNormal->setPosition(getContentSize().width / 2, getContentSize().height * 0.05f + ButtonNormal->getBoundingBox().size.height / 2 + ((i - 1) * ButtonNormal->getBoundingBox().size.height));
		ButtonNormal->setTag(i);
		snprintf(msg, 255, "Player_Setting_UI_Button_Text_%d.png", i - 1);
		Sprite* Text = Sprite::create(msg);
		Text->setPosition(ButtonNormal->getContentSize().width / 2, ButtonNormal->getContentSize().height / 2);
		ButtonNormal->addChild(Text);
		addChild(ButtonNormal);
	}
}

SettingMenu* SettingMenu::GetInstance()
{
	if (!_SettingMenu)
		_SettingMenu = new SettingMenu();
	return _SettingMenu;
}

void SettingMenu::OnClickSettingButton()
{
	if (!isVisible())
	{
		Director::getInstance()->pause();
		setVisible(true);
	}
}

void SettingMenu::OnTouchBegin(Touch* pTouch)
{
	m_TouchedButton = nullptr;
	for (int i = TAG_RESUME_GAME; i != MaxSettingButton; i++)
	{
		if (Sprite* Button = (Sprite*)this->getChildByTag(i))
		{
			Vec2 loc = pTouch->getLocation();
			log("%f,%f", loc.x, loc.y);
			if (Button->IsContectPoint(pTouch->getLocation()))
			{
				m_TouchedButton = Button;
				break;
			}
		}
	}
}

void SettingMenu::OnTouchEnded(Touch* pTouch)
{
	if (!m_TouchedButton)
		return;
	if (m_TouchedButton->IsContectPoint(pTouch->getLocation()))
	{
		switch (m_TouchedButton->getTag())
		{
		case TAG_RESUME_GAME:
			Director::getInstance()->resume();
			setVisible(false);
			break;
		case TAG_EXIT_GAME:
			Director::getInstance()->resume();
			exit(0);
			break;
		case TAG_LOG_OUT:
			Director::getInstance()->resume();
			sPlayer->removeFromParentAndCleanup(true);
			sPlayerUi->removeFromParentAndCleanup(true);
			sGame->SwapLayer(EnterGame_Layer_Tag, Main_Map_Layer_Tag);
			break;
		case TAG_GAME_SETTING:
			break;
		case TAG_SYS_SETTING:
			break;
		}
	}
}
