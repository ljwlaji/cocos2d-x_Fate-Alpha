#include "DeadTalkClass.h"
#include "Player.h"
#include "MainMapLayer.h"
#include "HelloWorldScene.h"

static DeadTalkClass* _DeadTalkClass = nullptr;

DeadTalkClass::DeadTalkClass()
{
	//DeadTalkClassFrame
	//	DeadTalkClassButtonFrame
	//	DeadTalkClassOkButton
	//	DeadTalkClassCancelButton
	initWithFile(DeadTalkClassFrame);
	autorelease();
	Sprite* ButtonFrame = Sprite::create(DeadTalkClassButtonFrame);
	ButtonFrame->setPosition(getContentSize().width / 2, getContentSize().height * 0.18f - ButtonFrame->getBoundingBox().size.height * 0.3f);
	addChild(ButtonFrame);

	ButtonYes = Sprite::create(DeadTalkClassOkButton);
	ButtonNo = Sprite::create(DeadTalkClassCancelButton);

	ButtonYes->setPosition(ButtonFrame->getContentSize().width * 0.5f - ButtonYes->getBoundingBox().size.width / 2, ButtonFrame->getContentSize().height * 0.5f);
	ButtonNo->setPosition(ButtonFrame->getContentSize().width * 0.5f + ButtonYes->getBoundingBox().size.width / 2, ButtonFrame->getContentSize().height * 0.5f);

	ButtonFrame->addChild(ButtonYes);
	ButtonFrame->addChild(ButtonNo);

	Disappear();
}

DeadTalkClass::~DeadTalkClass()
{
	_DeadTalkClass = nullptr;
}

void DeadTalkClass::Show()
{
	runAction(MoveTo::create(1.0f, Vec2(sMainMap->GetVisableSize().x / 2, sMainMap->GetVisableSize().y * 0.75f)));
}

void DeadTalkClass::Disappear()
{
	runAction(MoveTo::create(1.0f, Vec2(sMainMap->GetVisableSize().x / 2, sMainMap->GetVisableSize().y + getBoundingBox().size.height / 2)));
}

void DeadTalkClass::OnTouchBegin(Touch* pTouch)
{
	TouchedSprite = nullptr;
	ButtonYes->setScale(1.0f);
	ButtonNo->setScale(1.0f);
	if (ButtonYes->IsContectPoint(pTouch->getLocation()))
	{
		TouchedSprite = ButtonYes;
		ButtonYes->setScale(0.8f);
		return;
	}

	if (ButtonNo->IsContectPoint(pTouch->getLocation()))
	{
		TouchedSprite = ButtonNo;
		ButtonNo->setScale(0.8f);
		return;
	}
}

void DeadTalkClass::OnTouchEnded(Touch* pTouch)
{
	if (!TouchedSprite || !TouchedSprite->IsContectPoint(pTouch->getLocation()))
		return;

	TouchedSprite->setScale(1.0f);
	if (TouchedSprite == ButtonNo)
	{
		Disappear();
	}
	else
	{
		if (uint32 mapid = sGame->FindNearestReviveMap(sMainMap->GetMapID()))
		{
			sPlayer->Revive();
			sMainMap->SwapMap(mapid, false);
			Disappear();
		}
	}
}



DeadTalkClass* DeadTalkClass::GetInstance()
{
	if (!_DeadTalkClass)
		_DeadTalkClass = new DeadTalkClass();
	return _DeadTalkClass;
}