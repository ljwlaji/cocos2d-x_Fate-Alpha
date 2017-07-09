#include "MainUILayer.h"
#include "EnterLayer.h"
#include "Common.h"
#include "MainUILayerTitleSprite.h"
#include "MainUILayerUperSprite.h"
#include "MainUILayerNews.h"
#include "MainUILayerActionSprite.h"
#include "MainUIAccountSprite.h"
#include "WorldChatSprite.h"
#include "AuctionUISprite.h"
#include "UISprite.h"
#include "NotifyMgr.h"
#pragma once
#pragma execution_character_set("utf-8")

static MainUILayer* _MainUILayer = nullptr;

MainUILayer::MainUILayer() : IsAccountShowing(false), IsWorldChatShowing(false)
{
	setTag(MainUILayerTag);
	CCSpriteFrameCache * _CCSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	_CCSpriteFrameCache->addSpriteFramesWithFile("MainUILayer.plist");
}

MainUILayer::~MainUILayer()
{
	_MainUILayer = nullptr;
}

MainUILayer* MainUILayer::GetInstance()
{
	if (!_MainUILayer)
		_MainUILayer = MainUILayer::create();
	return _MainUILayer;
}

bool MainUILayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		Listener = EventListenerTouchOneByOne::create();
		Listener->onTouchBegan = CC_CALLBACK_2(MainUILayer::onTouchBegan, this);
		Listener->onTouchMoved = CC_CALLBACK_2(MainUILayer::onTouchMoved, this);
		Listener->onTouchEnded = CC_CALLBACK_2(MainUILayer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(Listener, this);
		setSwallowsTouches(true);
		Sprite* BackGround = Sprite::create("mainuilayer_background.png");
		BackGround->setPosition(getContentSize() / 2);
		addChild(BackGround);
		bRef = true;
	} while (0);
	return bRef;
}

bool MainUILayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (sNotifyMgr->isVisible() && sNotifyMgr->IsContectPoint(pTouch->getLocation()))
	{
		sNotifyMgr->OnUITouchBegin(pTouch);
		return true;
	}
	if (sAuction->IsInShow())
	{
		sAuction->OnUITouchBegin(pTouch);
		pTouch->SetTouchType(sAuction->GetTouchType());
		return true;
	}

	if (IsAccountShowing && !sAccount->IsContectPoint(pTouch->getLocation()))
	{
		sAccount->runAction(MoveTo::create(0.3f, Vec2(0 - sAccount->getBoundingBox().size.width / 2, getContentSize().height / 2)));
		SetAccountShowing(false);
		return true;
	}

	if (IsWorldChatShowing)
	{
		sWorldChat->OnUITouchBegin(pTouch);
		pTouch->SetTouchType(sWorldChat->GetTouchType());
		return true;
	}
	for (std::list<UISprite*>::iterator itr = UIList.begin(); itr != UIList.end(); itr++)
	{
		if ((*itr)->IsContectPoint(pTouch->getLocation()) && (*itr)->OnUITouchBegin(pTouch))
		{
			pTouch->SetTouchType((*itr)->GetTouchType());
			return true;
		}
	}
	return true;
}
void MainUILayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	for (std::list<UISprite*>::iterator itr = UIList.begin(); itr != UIList.end(); itr++)
	{
		if ((*itr)->GetTouchType() == pTouch->GetTouchType())
		{
			(*itr)->OnUITouchMoved(pTouch);
			return;
		}
	}
}
void MainUILayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	for (std::list<UISprite*>::iterator itr = UIList.begin(); itr != UIList.end(); itr++)
	{
		if ((*itr)->GetTouchType() == pTouch->GetTouchType())
		{
			(*itr)->OnUITouchEnded(pTouch);
			return;
		}
	}
}

void MainUILayer::CheckEdgeAndMoving(const Point& pPoint)
{

}

void MainUILayer::HandleInitTitleButtons()
{
	addChild(sMainTitleSprite);
	sMainTitleSprite->InitSprite();
	sMainTitleSprite->setPosition(getContentSize().width / 2, getContentSize().height - sMainTitleSprite->getBoundingBox().size.height / 2);
	UIList.push_back(sMainTitleSprite);
}

void MainUILayer::HandleInitUperButtons()
{
	addChild(sMainUperSprite);
	sMainUperSprite->InitSprite();
	sMainUperSprite->setPosition(getContentSize().width / 2, sMainTitleSprite->getBoundingBox().origin.y - sMainTitleSprite->getBoundingBox().size.height / 2);
	UIList.push_back(sMainUperSprite);
}

void MainUILayer::HandleInitNewsFrame()
{
	addChild(sNews);
	sNews->InitSprite();
	sNews->setPosition(sActionSprite->getPositionX(), sActionSprite->getBoundingBox().origin.y - getContentSize().height * 0.03f - sNews->getBoundingBox().size.height / 2);
	UIList.push_back(sNews);
}

void MainUILayer::HandleInitActionsFrame()
{
	addChild(sActionSprite);
	sActionSprite->setPosition(getContentSize().width / 2, getContentSize().height * 0.62f);
	UIList.push_back(sActionSprite);
}

void MainUILayer::HandleInitAccountFrame()
{
	addChild(sAccount);
	sAccount->InitSprite();
	sAccount->setPosition(0 - sAccount->getContentSize().width / 2, getContentSize().height / 2);
	UIList.push_front(sAccount);
}

void MainUILayer::HandleInitWorldChatFrame()
{
	addChild(sWorldChat);
	sWorldChat->InitSprite();
	sWorldChat->setPosition(getContentSize().width + sWorldChat->getBoundingBox().size.width / 2, getContentSize().height / 2);
	UIList.push_front(sWorldChat);
}

void MainUILayer::HandleInitAuctionHouse()
{
	addChild(sAuction);
	sAuction->InitSprite();
	//sAuction->setPosition(getContentSize() / 2);
	sAuction->setPosition(getContentSize().width / 2, getContentSize().height + sAuction->getBoundingBox().size.height / 2);
	UIList.push_front(sAuction);
}