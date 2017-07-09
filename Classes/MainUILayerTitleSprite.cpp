#include "MainUILayerTitleSprite.h"
#include "Common.h"
#include "MainUILayer.h"
#include "MainUIAccountSprite.h"
#include "WorldChatSprite.h"
static MainTitleSprite* _MainTitleSprite = nullptr;

MainTitleSprite::MainTitleSprite()
{
	initWithFile(TitleFrame);
	autorelease();
	SetTouchType(Main_UISprite_Title_Sprite);
}

MainTitleSprite::~MainTitleSprite()
{
	_MainTitleSprite = nullptr;
}

MainTitleSprite* MainTitleSprite::GetInstance()
{
	if (!_MainTitleSprite)
		_MainTitleSprite = new MainTitleSprite();
	return _MainTitleSprite;
}

bool MainTitleSprite::OnUITouchBegin(Touch* pTouch)
{
	if (MenuIcon->IsContectPoint(pTouch->getLocation()))
	{
		sAccount->runAction(MoveTo::create(0.3f, Vec2(getBoundingBox().origin.x + sAccount->getBoundingBox().size.width / 2, sAccount->getPositionY())));
		sMainUI->SetAccountShowing(true);
	}
	else if (ChatIcon->IsContectPoint(pTouch->getLocation()))
	{
		sWorldChat->runAction(MoveTo::create(0.3f, Vec2(getPositionX(), sWorldChat->getPositionY())));
		sMainUI->SetChatShowing(true);
	}
	return true;
}

void MainTitleSprite::OnUITouchEnded(Touch* pTouch)
{

}

void MainTitleSprite::InitSprite()
{
	MenuIcon = Sprite::create(MainUILayer_Title_Menu_Icon);
	MenuIcon->setPosition(MenuIcon->getBoundingBox().size.width * 1.5f, getContentSize().height / 2);
	addChild(MenuIcon);

	BattleNetTitle = Sprite::create(MainUILayer_Title_Title_Text);
	BattleNetTitle->setPosition(getContentSize() / 2);
	addChild(BattleNetTitle);

	ChatIcon = Sprite::create(MainUILayer_Title_Chat_Icon);
	ChatIcon->setPosition(getContentSize().width - MenuIcon->getBoundingBox().size.width - ChatIcon->getBoundingBox().size.width / 2, BattleNetTitle->getPositionY());
	addChild(ChatIcon);
}