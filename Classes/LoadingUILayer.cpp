#include "LoadingUILayer.h"
#include "Player.h"
#ifdef __APPLE__
#include "spine/spine.h"
#include "cocos/editor-support/spine/SkeletonAnimation.h"
#else
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"
#endif

using namespace spine;

static LoadingUILayer* _LoadingUILayer = nullptr;
LoadingUILayer::LoadingUILayer()
{
	_LoadingUILayer = this;
	visibleSize	= Director::getInstance()->getVisibleSize();
	origin		= Director::getInstance()->getVisibleOrigin();
	m_TimeBarPrescent = 0;
	m_TimeBar = nullptr;
	setTag(Loading_Layer_Tag);
}

LoadingUILayer::~LoadingUILayer()
{
	_LoadingUILayer = nullptr;
}

LoadingUILayer* LoadingUILayer::GetInstance()
{
	if (!_LoadingUILayer)
		_LoadingUILayer = LoadingUILayer::create();
	return _LoadingUILayer;
}

bool LoadingUILayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		BackGroundSprite_c = Sprite::create("Loading_blackground.png");
		addChild(BackGroundSprite_c);
		BackGroundSprite_c->SetRealPosition(visibleSize.width / 2, visibleSize.height / 2);
		BackGroundSprite_c->setScale(5.0f);

		BackGroundSprite = Sprite::create("Loading_Back_1.png");
		addChild(BackGroundSprite);
		BackGroundSprite->SetRealPosition(visibleSize.width / 2, visibleSize.height / 2);
		
		
		BackGroundSprite_a = Sprite::create("Loading_Back_2.png");
		addChild(BackGroundSprite_a);
		BackGroundSprite_a->SetRealPosition(visibleSize.width / 2, visibleSize.height / 2);


		InitTimeBar();
		bRef = true;
	} while (0);

	return bRef;
}

void LoadingUILayer::Reset()
{
	RandomSetBackGroundImage();
	BackGroundSprite->setOpacity(0.0f);
	BackGroundSprite_a->setOpacity(255.0f);
	m_TimeBarPrescent = 0;
	m_TimeBar->setPercentage(0);
}

void LoadingUILayer::Show()
{
	Reset();
	setVisible(true);
	scheduleUpdate();
	setLocalZOrder(Loading_Layer_Zorder);
}

void LoadingUILayer::DisAppear()
{
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

		Sprite* TimeBarSprite = Sprite::create("LoadingLayer_TimeBarImage.png");
		m_TimeBar = ProgressTimer::create(TimeBarSprite);
		m_TimeBar->setPosition(Back->getContentSize().width / 2, Back->getContentSize().height / 2);
		m_TimeBar->setPercentage(0);
		m_TimeBar->setBarChangeRate(Vec2(1, 0));
		m_TimeBar->setMidpoint(Vec2(0, 0));
		m_TimeBar->setType(ProgressTimer::Type::BAR);
		Back->addChild(m_TimeBar);
	}
}

void LoadingUILayer::RandomSetBackGroundImage()
{
	int i = 10 + rand() % 40;
	i = i / 10;
	char url[255];
	snprintf(url, 255, "Loading_Back_%d.png", i);
	BackGroundSprite->setTexture(url);
	if (++i == 5)
		i = 1;
	char uarl[255];
	snprintf(uarl, 255, "Loading_Back_%d.png", i);
	BackGroundSprite_a->setTexture(uarl);
}

void LoadingUILayer::update(float diff)
{
	float p_now		= m_TimeBar->getPercentage();
	if (p_now >= 100.0f)
	{
		DisAppear();
		return;
	}
	float change	= (float)m_TimeBarPrescent - p_now;
	if ((uint8)change)
	{
		change > 0 ? m_TimeBar->setPercentage(p_now + 1) : m_TimeBar->setPercentage(p_now - 1);
		float modify = m_TimeBar->getPercentage();
		float modify_op = modify * 255.0f / 100.0f;
		BackGroundSprite_a->setOpacity(255.0f - modify_op);
		BackGroundSprite->setOpacity(0 + modify_op);
	}
}
