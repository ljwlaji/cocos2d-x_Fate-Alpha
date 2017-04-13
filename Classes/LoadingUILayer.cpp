#include "LoadingUILayer.h"
#include "Player.h"
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"
using namespace spine;


LoadingUILayer::LoadingUILayer()
{
	visibleSize	= Director::getInstance()->getVisibleSize();
	origin		= Director::getInstance()->getVisibleOrigin();
	m_TimeBarPrescent = 0;
	m_TimeBar = nullptr;
	setTag(Loading_Layer_Tag);
}

LoadingUILayer::~LoadingUILayer()
{
}

bool LoadingUILayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		BackGroundSprite = Sprite::create("main_back_0.jpg");
		BackGroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		addChild(BackGroundSprite);

		InitTimeBar();
		bRef = true;
	} while (0);

	return bRef;
}

void LoadingUILayer::ShowLoadingLayer(LoadAddress _LoadAddress)
{
	setVisible(true);
	scheduleUpdate();
	setLocalZOrder(Loading_Layer_Zorder);
}

void LoadingUILayer::DisAppear()
{
	m_TimeBar->setPercentage(0);
	unscheduleUpdate();
	setVisible(false);
	setLocalZOrder(0 - Loading_Layer_Zorder);
}

void LoadingUILayer::InitTimeBar()
{
	if (m_TimeBar == nullptr)
	{
		Sprite* Back = Sprite::create("LoadingLayer_TimeBarImage_Back.png");
		Back->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.1f);
		addChild(Back);

		m_TimeBar = ProgressTimer::create(Sprite::create("LoadingLayer_TimeBarImage.png"));
		m_TimeBar->setPosition(Back->getContentSize().width / 2, Back->getContentSize().height / 2);
		m_TimeBar->setPercentage(0);
		m_TimeBar->setBarChangeRate(Vec2(1, 0));
		m_TimeBar->setMidpoint(Vec2(0, 0));
		m_TimeBar->setType(ProgressTimer::Type::BAR);
		Back->addChild(m_TimeBar);
	}
}

void LoadingUILayer::update(float diff)
{
	float p_now		= m_TimeBar->getPercentage();
	float change	= (float)m_TimeBarPrescent - p_now;
	if ((uint8)change)
	{
		change > 0 ? m_TimeBar->setPercentage(p_now + 1) : m_TimeBar->setPercentage(p_now - 1);
	}
}