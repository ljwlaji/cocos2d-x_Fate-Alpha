#include "PlayerTalkLayer.h"
#include "Types.h"
#include "Player.h"
#include "Creature.h"

static Player_Talk_Layer* _Player_Talk_Layer = nullptr;

Player_Talk_Layer::Player_Talk_Layer()
{
	_Player_Talk_Layer = this;
	VisiableSize = Director::getInstance()->getVisibleSize();
	TalkClassFrame = nullptr;
	m_Showing_Message.clear();
	setTag(Player_Talk_Layer_Tag);
	setVisible(false);
	m_Touched_Frame = -1;
}

Player_Talk_Layer::~Player_Talk_Layer()
{
	removeAllChildrenWithCleanup(true);
}

bool Player_Talk_Layer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		InitTalkClass();
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(Player_Talk_Layer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(Player_Talk_Layer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		bRef = true;
	} while (0);

	return bRef;
}

Player_Talk_Layer* Player_Talk_Layer::GetInstance()
{
	if (!_Player_Talk_Layer)
		return nullptr;
	return _Player_Talk_Layer;
}

bool Player_Talk_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (!isVisible())
		return false;

	m_Touched_Frame = -1;
	for (int i = 0; i != m_Showing_Message.size(); i++)
	{
		Sprite* SingleFrame = m_Showing_Message.at(i).SingleTalkClass;
		if (SingleFrame && SingleFrame->getBoundingBox().containsPoint(touch->getLocation()))
		{
			m_Touched_Frame = i;
			return true;
		}
	}



	return false;
}

void Player_Talk_Layer::CloseMenuWithCleanUp(bool cleanup)
{
	if (cleanup)
	for (int i = 0; i != m_Showing_Message.size(); i++)
	{
		m_Showing_Message.at(i).action = 0;
		m_Showing_Message.at(i).ImageID = 0;
		m_Showing_Message.at(i).sender = 0;
		m_Showing_Message.at(i).talkstring = "";
	}
	sPlayerTalkLayer->setLocalZOrder(Player_Talk_Disappear_Zorder);
	setVisible(false);
}

void Player_Talk_Layer::OnPlayerTouchedFrame(uint32 sender, uint32 action)
{
	if (sPlayer->PlayerTalkClass->TalkingCreature)
	{
		sPlayer->PlayerTalkClass->TalkingCreature->OnGossipSelect(sPlayer, sender, action);
	}
}

void Player_Talk_Layer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (m_Touched_Frame >= 0 && m_Showing_Message.at(m_Touched_Frame).SingleTalkClass->getBoundingBox().containsPoint(touch->getLocation()))
	{
		if (m_Showing_Message.at(m_Touched_Frame).action)
			OnPlayerTouchedFrame(m_Showing_Message.at(m_Touched_Frame).sender, m_Showing_Message.at(m_Touched_Frame).action);
	}
}

void Player_Talk_Layer::InitTalkClass()
{
	TalkClassFrame = Sprite::create("TalkClassFrame.png");
	TalkClassFrame->setPosition(VisiableSize.x / 2, VisiableSize.y / 2);
	addChild(TalkClassFrame);
	for (int i = 0; i != 4; i++)
	{
		Sprite* SpriteFrame = Sprite::create("TalkClass_Single_Frame.png");
		float height = SpriteFrame->getBoundingBox().size.height;
		float PosY = TalkClassFrame->getContentSize().height / 2 - (i * SpriteFrame->getBoundingBox().size.height);
		SpriteFrame->setPosition(TalkClassFrame->getContentSize().width / 2, PosY);
		SpriteFrame->setTag(i);


		LabelTTF* SpriteText = LabelTTF::create("", "Arial", 30);
		SpriteText->setString("123");
		SpriteText->setFontFillColor(Color3B(0, 0, 0));
		SpriteText->setPosition(SpriteFrame->getContentSize().width / 2, SpriteFrame->getContentSize().height / 2);
		SpriteText->setTag(1);
		SpriteFrame->addChild(SpriteText);

		ShowingTalkClass _template;
		_template.SingleTalkClass = SpriteFrame;
		m_Showing_Message.push_back(_template);

		TalkClassFrame->addChild(SpriteFrame);
	}

}

void Player_Talk_Layer::SendMenuToPlayer(std::string MainString)
{
	TalkClass* _class = sPlayer->PlayerTalkClass;
	for (int i = 0; i != _class->Menus.size(); i++)
	{
		ShowingTalkClass& _template = m_Showing_Message.at(i);
		Sprite* FrameSprite = _template.SingleTalkClass;
		if (!FrameSprite)
			return;
		LabelTTF* ttf = (LabelTTF*)FrameSprite->getChildByTag(1);
		if (!ttf)
			return;

		_template.action = _class->Menus.at(i).action;
		_template.sender = _class->Menus.at(i).sender;
		_template.ImageID = _class->Menus.at(i).ImageId;
		_template.talkstring = _class->Menus.at(i).TalkString;
		ttf->setString(_template.talkstring.c_str());
	}
	setVisible(true);
	setLocalZOrder(Player_Talk_Showing_Zorder);
}