#include "MiniMap.h"
#include "MainMapLayer.h"

static MiniMap* _MiniMap = nullptr;

MiniMap::MiniMap()
{
	initWithFile(PlayerUIMiniMapFrame);
	autorelease();
	clippingNode = nullptr;
	MiniMapSprite = nullptr;
	ClipFrame = nullptr;
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

void MiniMap::InitWithMapID(uint32 Mapid)
{
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
	}

	if (MiniMapSprite)
		MiniMapSprite->removeFromParentAndCleanup(true);

	TotalX = 0;
	MiniMapSprite = sMainMap->GetMiniMapSprite(TotalX);//RealMap
	MiniMapSprite->setAnchorPoint(Vec2(0, 0.5f));
	float Size = ClipFrame->getBoundingBox().size.height / MiniMapSprite->getBoundingBox().size.height;
	MiniMapSprite->setScale(Size);
	MiniMapSprite->setPosition(0, clippingNode->getContentSize().height / 2);
	clippingNode->addChild(MiniMapSprite, 2);

	MiniMapSprite->runAction(MoveTo::create(1.0f, Vec2(MiniMapSprite->getPositionX() - 500, MiniMapSprite->getPositionY())));
}