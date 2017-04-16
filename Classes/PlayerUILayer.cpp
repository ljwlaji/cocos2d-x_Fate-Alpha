#include "PlayerUILayer.h"

PlayerUILayer::PlayerUILayer()
{
	visiablesize = Director::getInstance()->getVisibleSize();
	m_VirtualRokerLayer = nullptr;
	m_VirtualRoker_BackGround = nullptr;
	m_VirtualRoker_Roker = nullptr;
	RockerLastPostion = Vec2(0, 0);
}

PlayerUILayer::~PlayerUILayer()
{
}

bool PlayerUILayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());


//#ifndef WIN32
		CreateVirtualRoker();
//#endif
		bRef = true;
	} while (0);

	return bRef;
}

void PlayerUILayer::CreateVirtualRoker()
{
	m_VirtualRokerLayer = Layer::create();
	addChild(m_VirtualRokerLayer);

	m_VirtualRoker_BackGround = Sprite::create("Virtual_Roker_Back.png");
	m_VirtualRoker_BackGround->SetRealPosition(m_VirtualRoker_BackGround->getBoundingBox().size.width * 0.6f, m_VirtualRoker_BackGround->getBoundingBox().size.height * 0.6f);
	m_VirtualRokerLayer->addChild(m_VirtualRoker_BackGround);
	m_VirtualRoker_BackGround->setOpacity(80.0f);
	m_VirtualRoker_Roker = Sprite::create("Virtual_Roker_Center.png");
	m_VirtualRoker_Roker->setPosition(m_VirtualRoker_BackGround->getContentSize().width / 2, m_VirtualRoker_BackGround->getContentSize().height / 2);
	m_VirtualRoker_Roker->setOpacity(80.0f);
	m_VirtualRoker_BackGround->addChild(m_VirtualRoker_Roker);

	//意见:给确认是摇杆的touch创建唯一标识符
	auto RokerListener = EventListenerTouchOneByOne::create();
	RokerListener->onTouchBegan = CC_CALLBACK_2(PlayerUILayer::onTouchRockerBegan, this);
	RokerListener->onTouchMoved = CC_CALLBACK_2(PlayerUILayer::onTouchRockerMoved, this);
	RokerListener->onTouchEnded = CC_CALLBACK_2(PlayerUILayer::onTouchRockerEnded, this);
	//RokerListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(RokerListener, m_VirtualRokerLayer);
}

bool PlayerUILayer::onTouchRockerBegan(Touch* touches, Event *event)
{
	if (m_VirtualRoker_Roker && m_VirtualRoker_Roker->getBoundingBox().containsPoint(touches->getLocation()))
	{
		RockerLastPostion = touches->getLocation();
		m_VirtualRoker_Roker->setPosition(touches->getLocation());
		return true;
	}
	return false;
}

void PlayerUILayer::onTouchRockerMoved(Touch* touches, Event *event)
{
	Vec2 NowLoc = touches->getLocation();
	float X_move = NowLoc.x - RockerLastPostion.x;
	float Y_move = NowLoc.y - RockerLastPostion.y;
	if (ccpDistance(Vec2(m_VirtualRoker_BackGround->getPositionY() - X_move, m_VirtualRoker_BackGround->getPositionY() - Y_move), m_VirtualRoker_Roker->getPosition()) < 140.0f)
		m_VirtualRoker_Roker->setPosition(m_VirtualRoker_Roker->getPositionX() + X_move, m_VirtualRoker_Roker->getPositionY() + Y_move);

	uint16 Orgin = GetVirtualRokerOrgin(m_VirtualRoker_BackGround->getPosition(), NowLoc);
	RockerLastPostion = touches->getLocation();
}

void PlayerUILayer::onTouchRockerEnded(Touch* touches, Event *event)
{
	if (m_VirtualRoker_Roker)
		m_VirtualRoker_Roker->setPosition(m_VirtualRoker_BackGround->getContentSize().width / 2, m_VirtualRoker_BackGround->getContentSize().height / 2);

	RockerLastPostion = Vec2(0, 0);
}

uint16 PlayerUILayer::GetVirtualRokerOrgin(Vec2 CenterPoint, Vec2 RokerPoint)
{
	float Correct_Orgin = atan(abs(RokerPoint.y - CenterPoint.y) / abs(RokerPoint.x - CenterPoint.x)) * (180.0f / 3.1415926f);
	if (RokerPoint.x >= CenterPoint.x)
	{
		if (RokerPoint.y >= CenterPoint.y)
			Correct_Orgin += 90.0f;
		else
			Correct_Orgin = 90.0f - Correct_Orgin;
		Correct_Orgin += 180.0f;
	}
	else if (RokerPoint.y > CenterPoint.y)
	{
		Correct_Orgin = 90.0f - Correct_Orgin;
	}
	else
		Correct_Orgin += 90.0f;

	return (uint16)Correct_Orgin;
}