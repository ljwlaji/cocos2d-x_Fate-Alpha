#include "MiniMap.h"
#include "MainMapLayer.h"
#include "Player.h"
#include "NotifyMgr.h"
#include "Creature.h"

static MiniMap* _MiniMap = nullptr;

MiniMap::MiniMap()
{
	initWithFile(PlayerUIMiniMapFrame);
	autorelease();
	clippingNode = nullptr;
	MiniMapSprite = nullptr;
	ClipFrame = nullptr;
	PlayerMiniMapSingle = nullptr;
	MapOpType = 0;
	IsLoadFinished = false;
}

MiniMap::~MiniMap()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_MiniMap = nullptr;
}

MiniMap* MiniMap::GetInstance()
{
	if (!_MiniMap)
		_MiniMap = new MiniMap();
	return _MiniMap;
}

void MiniMap::SwapOpType()
{
	if (++MapOpType == End)
		MapOpType = One;

	float op = 0;
	switch (MapOpType)
	{
	case MiniMap::One:
		op = 255.0f;
		break;
	case MiniMap::Two:
		op = 255.0f * 0.66f;
		break;
	case MiniMap::Three:
		op = 255.0f * 0.33f;
		break;
	case MiniMap::Four:
		break;
	}
	//Strange Place
	//Parent Opacity Can NOT Effect Childens
	//Waiting To Fix
	setOpacity(op);
	MiniMapSprite->setOpacity(op);
	ClipFrame->setOpacity(op);
	for (Vector<Node*>::iterator itr = MiniMapSprite->getChildren().begin(); itr != MiniMapSprite->getChildren().end(); itr++)
		(*itr)->setOpacity(op);
}

void MiniMap::InitWithMapID(uint32 Mapid)
{
	for (std::map<Creature*, Sprite*>::iterator itr = SingleCreatureSignMap.begin(); itr != SingleCreatureSignMap.end(); itr++)
		itr->second->removeFromParentAndCleanup(true);

	SingleCreatureSignMap.clear();
	if (!clippingNode)
	{
		clippingNode = ClippingNode::create();
		clippingNode->setAlphaThreshold(0);
		clippingNode->setAnchorPoint(Vec2(0, 0.5f));
		clippingNode->setPosition(getContentSize().width * 0.08f, getContentSize().height / 2);
		addChild(clippingNode);

		ClipFrame = Sprite::create(PlayerUIMiniMapSkipFrame);
		ClipFrame->setAnchorPoint(Vec2(0, 0.5f));
		ClipFrame->setPosition(0, clippingNode->getContentSize().height / 2);
		clippingNode->setStencil(ClipFrame);
		clippingNode->addChild(ClipFrame);
	}

	if (MiniMapSprite)
		MiniMapSprite->removeFromParentAndCleanup(true);

	TotalX = 0;
	TotalY = 0;
	MiniMapSprite = sMainMap->GetMiniMapSprite(TotalX);//RealMapSprite Here
	MiniMapSprite->setAnchorPoint(Vec2(0, 0.5f));
	TotalY = MiniMapSprite->getBoundingBox().size.height;
	float Size = ClipFrame->getBoundingBox().size.height / MiniMapSprite->getBoundingBox().size.height;
	MiniMapSprite->setScale(Size);
	CurrentX = TotalX * Size;
	MiniMapSprite->setPosition(0, clippingNode->getContentSize().height / 2);
	clippingNode->addChild(MiniMapSprite, 2);
	PlayerMiniMapSingle = Sprite::create("123.png");//NeedChangeImage
	PlayerMiniMapSingle->setPositionY(MiniMapSprite->getContentSize().height / 2);
	MiniMapSprite->addChild(PlayerMiniMapSingle);

	//MiniMapSprite->runAction(Follow::create(PlayerMiniMapSingle, Rect(0, 0, TotalX, TotalY)));

	IsLoadFinished = true;

	for (int i = 0; i != sMainMap->GetMonsterMap()->size(); i++)
		UpdateSingleUnitSign((Unit*)sMainMap->GetMonsterMap()->at(i));

	for (int i = 0; i != sMainMap->GetNpcMap()->size(); i++)
		UpdateSingleUnitSign((Unit*)sMainMap->GetNpcMap()->at(i));
}

void MiniMap::RemoveCreatureFromMiniMap(Creature* pCreature)
{
	for (std::map<Creature*, Sprite*>::iterator itr = SingleCreatureSignMap.begin(); itr != SingleCreatureSignMap.end(); itr++)
	{
		if (itr->first == pCreature)
		{
			itr->second->removeFromParentAndCleanup(true);
			SingleCreatureSignMap.erase(itr);
			return;
		}
	}
	sNotifyMgr->ShowNotify("Remove Creature From MiniMap Error! Can NOT Find Current Creature From SingleCreatureSignMap");
}


void MiniMap::UpdateSingleUnitSign(Unit* pUnit)
{
	if (!IsLoadFinished)
		return;

	Sprite* SignSprite = nullptr;

	if (pUnit->ToCreature())
	{
		if (SingleCreatureSignMap.find(pUnit->ToCreature()) == SingleCreatureSignMap.end())
		{
			SignSprite = Sprite::create("123.png");//NeedChangeImage
			MiniMapSprite->addChild(SignSprite);
			SingleCreatureSignMap[pUnit->ToCreature()] = SignSprite;
			SignSprite->setPosition(MiniMapSprite->getContentSize() / 2);
		}
		else SignSprite = SingleCreatureSignMap[pUnit->ToCreature()];
	}
	else SignSprite = PlayerMiniMapSingle;

	float X = pUnit->getPositionX() / sMainMap->GetMapMaxWidth();
	float MiniMapPosX = TotalX * X;
	float Move_x = MiniMapPosX - PlayerMiniMapSingle->getPositionX();

	SignSprite->setPosition(pUnit->getPositionX(), pUnit->getPositionY());

	if (pUnit->ToPlayer())
	{
		Vec2 SignX = PlayerMiniMapSingle->getParent()->convertToNodeSpace(PlayerMiniMapSingle->getPosition());
		bool CanMoveBackGround = true;
		if (Move_x > 0)
		{
			float RightX = MiniMapSprite->getBoundingBox().origin.x + CurrentX;
			if (RightX < ClipFrame->getBoundingBox().size.width || PlayerMiniMapSingle->getPositionX() < TotalX * 0.2f)
				CanMoveBackGround = false;
		}
		else
		{
			float LeftX = MiniMapSprite->getBoundingBox().origin.x;
			if (LeftX > ClipFrame->getPositionX() || PlayerMiniMapSingle->getPositionX() > TotalX * 0.9f)
				CanMoveBackGround = false;
		}
		if (CanMoveBackGround)
			MiniMapSprite->setPositionX(MiniMapSprite->getPositionX() - (Move_x * (CurrentX / TotalX)));
	}
}

//void MiniMap::UpdatePlayerPosition()
//{
//	//Need Move To Player::UpdateMovementStatus();
//	//Fixed
//	if (!IsLoadFinished)
//		return;
//
//
//
//	float X = sPlayer->getPositionX() / sMainMap->GetMapMaxWidth();
//	float MiniMapPosX = TotalX * X;
//	float Move_x = MiniMapPosX - PlayerMiniMapSingle->getPositionX();
//	PlayerMiniMapSingle->setPositionX(MiniMapPosX);
//
//	Vec2 SignX = PlayerMiniMapSingle->getParent()->convertToNodeSpace(PlayerMiniMapSingle->getPosition());
//	bool CanMoveBackGround = true;
//	if (Move_x > 0)
//	{
//		float RightX = MiniMapSprite->getBoundingBox().origin.x + CurrentX;
//		if (RightX < ClipFrame->getBoundingBox().size.width || PlayerMiniMapSingle->getPositionX() < TotalX * 0.1f)
//			CanMoveBackGround = false;
//	}
//	else
//	{
//		float LeftX = MiniMapSprite->getBoundingBox().origin.x;
//		if (LeftX > ClipFrame->getPositionX() || PlayerMiniMapSingle->getPositionX() > TotalX * 0.9f)
//			CanMoveBackGround = false;
//	}
//	if (CanMoveBackGround)
//		MiniMapSprite->setPositionX(MiniMapSprite->getPositionX() - (Move_x * (CurrentX / TotalX)));
//}