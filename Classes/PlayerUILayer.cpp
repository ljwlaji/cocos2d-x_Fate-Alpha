#include "PlayerUILayer.h"
#include "Player.h"
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
	if (!sPlayer)
		return;
	Vec2 NowLoc = touches->getLocation();
	float X_move = NowLoc.x - RockerLastPostion.x;
	float Y_move = NowLoc.y - RockerLastPostion.y;
	if (ccpDistance(Vec2(m_VirtualRoker_BackGround->getPositionY() - X_move, m_VirtualRoker_BackGround->getPositionY() - Y_move), m_VirtualRoker_Roker->getPosition()) < 140.0f)
		m_VirtualRoker_Roker->setPosition(m_VirtualRoker_Roker->getPositionX() + X_move, m_VirtualRoker_Roker->getPositionY() + Y_move);

	float Orgin = GetVirtualRokerOrgin(m_VirtualRoker_BackGround->getPosition(), m_VirtualRoker_Roker->getPosition());
	RockerLastPostion = touches->getLocation();
	ResetVirtualRokerOrgin(Orgin);
	log("%f", Orgin);
}

void PlayerUILayer::ResetVirtualRokerOrgin(float _orgin)
{
	if (_orgin > 22.5 && _orgin <= 135)
	{
		if (abs(_orgin - 90) < 20)
			sPlayer->DealVirtualRoker(Roker_Left);
		else
			_orgin > 90 ? sPlayer->DealVirtualRoker(Roker_Left_Down) : sPlayer->DealVirtualRoker(Roker_Up_Left);
	}
	else if (_orgin > 135 && _orgin < 225)
	{
		if (abs(_orgin - 180) < 20)
			sPlayer->DealVirtualRoker(Roker_Down);
		else _orgin > 180 ? sPlayer->DealVirtualRoker(Roker_Down_Right) : sPlayer->DealVirtualRoker(Roker_Left_Down);
	}
	else if (_orgin > 225 && _orgin < 315)
	{
		if (abs(_orgin - 270) < 20)
			sPlayer->DealVirtualRoker(Roker_Right);
		else _orgin > 270 ? sPlayer->DealVirtualRoker(Roker_Up_Right) : sPlayer->DealVirtualRoker(Roker_Down_Right);
	}
	else
	{
		if (_orgin < 15 || _orgin > 345)
			sPlayer->DealVirtualRoker(Roker_Up);
		else if (_orgin < 45)
			sPlayer->DealVirtualRoker(Roker_Up_Left);
		else
			sPlayer->DealVirtualRoker(Roker_Up_Right);
	}
}

void PlayerUILayer::onTouchRockerEnded(Touch* touches, Event *event)
{
	if (!sPlayer)
		return;
	sPlayer->ResetMoveKeyForRoker();
	if (m_VirtualRoker_Roker)
		m_VirtualRoker_Roker->setPosition(m_VirtualRoker_BackGround->getContentSize().width / 2, m_VirtualRoker_BackGround->getContentSize().height / 2);

	RockerLastPostion = Vec2(0, 0);
}

float PlayerUILayer::GetVirtualRokerOrgin(Vec2 CenterPoint, Vec2 RokerPoint)
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

	return Correct_Orgin;
}