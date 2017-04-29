#include "PlayerUILayer.h"
#include "Player.h"
#include "Item.h"
#include "HelloWorldScene.h"

static PlayerBag* _PlayerBag = nullptr;
static PlayerUILayer* _PlayerUILayer = nullptr;

Slot::Slot(const std::string& url)
{
	if (url.empty())
	{
		initWithFile(url.c_str());
		autorelease();
	}
	m_Item = nullptr;
}

Slot::~Slot()
{

}

PlayerBag::PlayerBag()
{
	//_PlayerBag = this;
	url = "";
	initWithFile(url.c_str());
	autorelease();
	InitPage();
}

PlayerBag::~PlayerBag()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
}

bool PlayerBag::SwapItem(Slot* slot_one, Slot* slot_two)
{
	return true;
}

void PlayerBag::InitPage()
{
	m_PlayerBagPageSprites.clear();
	for (int i = Page_One; i != End_Of_Player_Bag_Page; i++)
	{
		//首先创建页面
		Sprite* _TempPageSprite = Sprite::create("");
		_TempPageSprite->setPosition(getContentSize().width / 2, _TempPageSprite->getBoundingBox().size.height / 2);
		addChild(_TempPageSprite);
		_TempPageSprite->setTag(i);
		InitEmptySlots(_TempPageSprite);
		m_PlayerBagPageSprites.push_back(_TempPageSprite);
	}
}

Slot* PlayerBag::GetSlot(uint8 _Page, uint8 _Slot)
{
	if (_Page > Page_Nine)
		return nullptr;

	Sprite* Page = nullptr;

	Page = m_PlayerBagPageSprites.at(_Page);

	if (Slot* pSlot = (Slot*)Page->getChildByTag(_Slot))
		return pSlot;

	return nullptr;
}

void PlayerBag::InitEmptySlots(Sprite* SinglePageSprite)
{
	int tag = 1;
	for (int i = 0; i != SlotRowCount; i++)
	for (int k = 0; k != SlotFieldCount; k++)
	{
		Slot* SingleSlot = new Slot("");
		SingleSlot->setPositionX(SinglePageSprite->getContentSize().width / 2 + ((SlotRowCount / 2) * SingleSlot->getBoundingBox().size.width) - (i * SingleSlot->getBoundingBox().size.width));
		SingleSlot->setPositionY(SinglePageSprite->getContentSize().height / 2 + ((SlotFieldCount / 2) * SingleSlot->getBoundingBox().size.height) - (k * SingleSlot->getBoundingBox().size.height));
		SingleSlot->setTag(tag);
		SinglePageSprite->addChild(SingleSlot);
		tag++;
	}
}

PlayerBag* PlayerBag::GetInstance()
{
	if (!_PlayerBag)
		_PlayerBag = new PlayerBag();

	return _PlayerBag;
}

								/////////////////
								//PlayerUILayer//
								/////////////////
PlayerUILayer::PlayerUILayer()
{
	_PlayerUILayer = this;
	visiablesize = Director::getInstance()->getVisibleSize();
	m_VirtualRokerLayer = nullptr;
	m_VirtualRoker_BackGround = nullptr;
	m_VirtualRoker_Roker = nullptr;
	RockerLastPostion = Vec2(0, 0);
}

PlayerUILayer::~PlayerUILayer()
{
}

PlayerUILayer* PlayerUILayer::GetInstance()
{
	if (!_PlayerUILayer)
		_PlayerUILayer = PlayerUILayer::create();

	return _PlayerUILayer;
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

		InitUI();

		addChild(sPlayerBag);
		scheduleUpdate();
		bRef = true;
	} while (0);

	return bRef;
}

void PlayerUILayer::InitUI()
{
	m_Player_Info_UI = Sprite::create("Player_UI_Info.png");
	m_Player_Info_UI->SetRealPosition(m_Player_Info_UI->getBoundingBox().size.width / 2, visiablesize.y - m_Player_Info_UI->getBoundingBox().size.height / 2);
	addChild(m_Player_Info_UI);


	m_Player_Info_UI_Hp = ProgressTimer::create(Sprite::create("Player_UI_Info_HP.png"));
	m_Player_Info_UI_Hp->setPosition(m_Player_Info_UI->getContentSize().width * 0.70f, m_Player_Info_UI->getContentSize().height * 0.4f);
	m_Player_Info_UI_Hp->setPercentage(0);
	m_Player_Info_UI_Hp->setBarChangeRate(Vec2(1, 0));
	m_Player_Info_UI_Hp->setMidpoint(Vec2(0, 0));
	m_Player_Info_UI_Hp->setType(ProgressTimer::Type::BAR);
	m_Player_Info_UI_Hp_Back = Sprite::create("Player_UI_Info_HP_Back.png");
	m_Player_Info_UI_Hp_Back->setPosition(m_Player_Info_UI_Hp->getPosition());
	m_Player_Info_UI->addChild(m_Player_Info_UI_Hp_Back);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Hp);

	if (!sPlayer)
		return;
	m_Player_Info_UI_Level = sGame->GetNumberSpriteByInt(sPlayer->GetLevel());
	m_Player_Info_UI_Level->setPosition(m_Player_Info_UI->getBoundingBox().size.width * 0.285f, m_Player_Info_UI->getBoundingBox().size.height * 0.4f);
	m_Player_Info_UI_Level->setScale(0.5f);
	m_Player_Info_UI->addChild(m_Player_Info_UI_Level);
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
	RokerListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(RokerListener, m_VirtualRokerLayer);
}

bool PlayerUILayer::onTouchRockerBegan(Touch* touches, Event *event)
{
	if (!sPlayer)
		return false;
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
		if (abs(_orgin - 90) < 15)
			sPlayer->DealVirtualRoker(Roker_Left);
		else
			_orgin > 90 ? sPlayer->DealVirtualRoker(Roker_Left_Down) : sPlayer->DealVirtualRoker(Roker_Up_Left);
	}
	else if (_orgin > 135 && _orgin < 225)
	{
		if (abs(_orgin - 180) < 15)
			sPlayer->DealVirtualRoker(Roker_Down);
		else _orgin > 180 ? sPlayer->DealVirtualRoker(Roker_Down_Right) : sPlayer->DealVirtualRoker(Roker_Left_Down);
	}
	else if (_orgin > 225 && _orgin < 315)
	{
		if (abs(_orgin - 270) < 15)
			sPlayer->DealVirtualRoker(Roker_Right);
		else _orgin > 280 ? sPlayer->DealVirtualRoker(Roker_Up_Right) : sPlayer->DealVirtualRoker(Roker_Down_Right);
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

void PlayerUILayer::update(float diff)
{
	if (m_Player_Info_UI_Hp->getPercentage() != 100.0f)
	{
		float posnow = m_Player_Info_UI_Hp->getPercentage();
		m_Player_Info_UI_Hp->setPercentage(posnow + 1);
	}
}