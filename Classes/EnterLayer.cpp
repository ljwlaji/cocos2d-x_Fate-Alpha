#include "Common.h"
#include "EnterLayer.h"
#include "NotifyMgr.h"
#include "SocketClient.h"
#include "LoginTypeSprite.h"
#include "MainUILayer.h"
#include "HelloWorldScene.h"
#include "WaittingLayer.h"

#pragma once
#pragma execution_character_set("utf-8")
static EnterLayer* _MainLayer = nullptr;

EnterLayer::EnterLayer()
{
	setTag(EnterLayerTag);
}

EnterLayer::~EnterLayer()
{
	_eventDispatcher->removeEventListener(Listener);
	_MainLayer = nullptr;
	
}

EnterLayer* EnterLayer::GetInstance()
{
	if (!_MainLayer)
		_MainLayer = EnterLayer::create();
	return _MainLayer;
}

bool EnterLayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		Listener = EventListenerTouchOneByOne::create();
		Listener->onTouchBegan = CC_CALLBACK_2(EnterLayer::onTouchBegan, this);
		Listener->onTouchMoved = CC_CALLBACK_2(EnterLayer::onTouchMoved, this);
		Listener->onTouchEnded = CC_CALLBACK_2(EnterLayer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(Listener, this);
		setSwallowsTouches(true);

		BackGround = Sprite::create("Login_BackGround_Login.png");
		BackGround->SetRealPosition(getContentSize() / 2);
		addChild(BackGround);

		Logo = Sprite::create("Logo.png");
		Logo->setPosition(getContentSize() / 2);

		addChild(Logo);

		bRef = true;
	} while (0);

	return bRef;
}

bool EnterLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (sNotifyMgr->isVisible())
	{
		if (sNotifyMgr->IsDisabaleTouch())
			return true;
		else
		{
			pTouch->SetTouchType(sNotifyMgr->GetTouchType());
			sNotifyMgr->OnUITouchBegin(pTouch);
		}
	}

	for (std::list<UISprite*>::iterator itr = UISpriteList.begin(); itr != UISpriteList.end(); itr++)
	{
		if ((*itr) && (*itr)->isVisible() && (*itr)->OnUITouchBegin(pTouch))
		{
			pTouch->SetTouchType((*itr)->GetTouchType());
			return true;
		}
	}
}

void EnterLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{

}

void EnterLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	for (std::list<UISprite*>::iterator itr = UISpriteList.begin(); itr != UISpriteList.end(); itr++)
	{
		if ((*itr) && (*itr)->isVisible() && (*itr)->GetTouchType() == pTouch->GetTouchType())
		{
			(*itr)->OnUITouchEnded(pTouch);
			return;
		}
	}
}

void EnterLayer::CreateConnectingSprite(int i)
{
	Sprite* Connecting = Sprite::create("Connecting.png");
	FadeIn* Fadein = FadeIn::create(1.5f);
	FadeOut* FadeOut = FadeOut::create(1.5f);
	Sequence* Seq = Sequence::create(Fadein, FadeOut, NULL);
	Connecting->runAction(RepeatForever::create(Seq));

	Connecting->setPosition(BackGround->getContentSize().width / 2 + (2.5f * Connecting->getBoundingBox().size.width) - (i * Connecting->getBoundingBox().size.width), getContentSize().height * 0.25f);
	BackGround->addChild(Connecting);
	ConnectList.push_back(Connecting);
}

void EnterLayer::OnConnectSuccessed()
{
	ConnectLabel->removeFromParentAndCleanup(true);
	ConnectLabel = nullptr;
	while (ConnectList.size())
	{
		if (Sprite* TempSprite = *ConnectList.begin())
		{
			TempSprite->removeFromParentAndCleanup(true);
			ConnectList.pop_front();
		}
	}
	Sequence* seq = Sequence::create(MoveTo::create(1.0f, Vec2(Logo->getPositionX(), getContentSize().height * 0.75f)), CallFunc::create(CC_CALLBACK_0(EnterLayer::typingshowup, this)), NULL);
	Logo->runAction(seq);
}

void EnterLayer::typingshowup()
{
	sLoginTypeSprite->setPosition(getContentSize() / 2);
	addChild(sLoginTypeSprite);
	sLoginTypeSprite->ShowUp();
	UISpriteList.push_back(sLoginTypeSprite);
}

void EnterLayer::StartConnect()
{
	ConnectLabel = LabelTTF::create("正在连接战网服务器", "arial", 32);
	ConnectLabel->SetRealPosition(getContentSize().width / 2, getContentSize().height * 0.35f);
	ConnectLabel->setFontName("arial");
	addChild(ConnectLabel);

	float TotalDelay = 0;
	for (int i = 0; i < 6; i++)
	{
		TotalDelay = i * 0.5f;
		runAction(Sequence::create(DelayTime::create(TotalDelay), CallFunc::create(CC_CALLBACK_0(EnterLayer::CreateConnectingSprite, this, i)), NULL));
	}
	runAction(Sequence::create(DelayTime::create(TotalDelay + 1.0f), CallFunc::create(CC_CALLBACK_0(EnterLayer::_connect, this)), NULL));
}

void EnterLayer::_connect()
{
	int ErrorCode = sSocket->connectServer(IP_SERVER, PORT_SERVER);
	if (!ErrorCode)
	{

	}
	else if (ErrorCode == -1)
	{

	}
	else
		sEnterLayer->OnConnectSuccessed();
}

void EnterLayer::IntoMainPage()
{
	sLoginTypeSprite->OnFadeOut();
	for (std::list<UISprite*>::iterator itr = UISpriteList.begin(); itr != UISpriteList.end(); itr++)
	{
		(*itr)->stopAllActions();
		(*itr)->runAction(FadeOut::create(1.0f));
	}
	sNotifyMgr->ShowUpWithDisabaledTouching(1.0f, "连接成功");
	Sequence* Seq = Sequence::create(ScaleTo::create(0.5f, 0.8f), ScaleTo::create(0.2f, 1.2f), NULL);
	Logo->runAction(RepeatForever::create(Seq));
	DelayTimer = 4.0f;
	scheduleUpdate();
}

void EnterLayer::update(float diff)
{
	if (DelayTimer >= diff && DelayTimer)
	{
		if (Logo->getPositionY() > getContentSize().height * 0.5f)
			Logo->setPositionY(Logo->getPositionY() - 4.5f);
		DelayTimer -= diff;
		return;
	}
	else
	{
		DelayTimer = 0;
		unscheduleUpdate();
		sGame->SwapLayer(getTag(), sMainUI);
		sWaittingLayer->ShowUp(sMainUI->getTag());
	}


}