#include "PlayerTalkLayer.h"
#include "Types.h"
#include "Player.h"
#include "Creature.h"
#include "HelloWorldScene.h"
#include "QuestMgr.h"
#include "NotifyMgr.h"

static Player_Talk_Layer* _Player_Talk_Layer = nullptr;

Player_Talk_Layer::Player_Talk_Layer()
{
	_Player_Talk_Layer = this;
	VisiableSize = Director::getInstance()->getVisibleSize();
	TalkClassFrame = nullptr;
	m_Showing_Message.clear();
	setTag(Player_Talk_Layer_Tag);
	setLocalZOrder(Player_Talk_Showing_Zorder);
	setVisible(false);
	m_Touched_Frame = nullptr;
	m_QuestID = 0;
	IsQuestTalking = false;
}

Player_Talk_Layer::~Player_Talk_Layer()
{
	_Player_Talk_Layer = nullptr;
	removeAllChildrenWithCleanup(true);
	_eventDispatcher->removeEventListener(listener);
	listener = nullptr;
}

bool Player_Talk_Layer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		InitTalkClass();
		listener = EventListenerTouchOneByOne::create();
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
		_Player_Talk_Layer = new Player_Talk_Layer::create();
	return _Player_Talk_Layer;
}

bool Player_Talk_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (!isVisible())
		return false;
	m_Touched_Frame = nullptr;

	if (IsQuestTalking)
	{
		for (int i = 0; i != 4; i++)
		{
			if (SpriteFrame[i]->IsContectPoint(touch->getLocation()) && SpriteFrame[i]->isVisible())
			{
				m_Touched_Frame = SpriteFrame[i];
				return true;
			}
		}
	}

	for (int i = 0; i != m_Showing_Message.size(); i++)
	{
		Sprite* SingleFrame = m_Showing_Message.at(i).SingleTalkClass;
		if (SingleFrame && SingleFrame->IsContectPoint(touch->getLocation()) && SingleFrame->isVisible())
		{
			m_Touched_Frame = SingleFrame;
			return true;
		}
	}

	
	if (CancelButton->IsContectPoint(touch->getLocation()))
	{
		CancelButton->setScale(0.8f);
		m_Touched_Frame = CancelButton;
		return true;
	}

	else if (ComfirmButton->IsContectPoint(touch->getLocation()))
	{
		ComfirmButton->setScale(0.8f);
		m_Touched_Frame = ComfirmButton;
		return true;
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
	if (!m_Touched_Frame)
		return;

	if (IsQuestTalking)
	{
		for (int i = 0; i != 4; i++)
		{
			if (SpriteFrame[i] == m_Touched_Frame)
			if (m_Touched_Frame->IsContectPoint(touch->getLocation()) && m_Touched_Frame->isVisible())
			{
				sPlayer->PlayerTalkClass->ClearMenu();
				SendMenuToPlayer(sQuestMgr->GetQuestTemplate(m_Touched_Frame->getTag())->QuestText.c_str());
				for (int i = 0; i != 4; i++)
					SpriteFrame[i]->setVisible(false);
				return;
			}
		}
	}

	for (int i = 0; i != m_Showing_Message.size(); i++)
	{
		if (m_Showing_Message.at(i).SingleTalkClass == m_Touched_Frame && m_Touched_Frame->IsContectPoint(touch->getLocation()))
		{
			OnPlayerTouchedFrame(m_Showing_Message.at(i).sender, m_Showing_Message.at(i).action);
			return;
		}
	}

	if (m_Touched_Frame == CancelButton && CancelButton->IsContectPoint(touch->getLocation()))
	{
		setVisible(false);
	}

	else if (m_Touched_Frame == ComfirmButton && ComfirmButton->IsContectPoint(touch->getLocation()))
	{
		//DoComifrm
		if (!m_QuestID)
			setVisible(false);
		else
		{
			sPlayer->AcceptQuest(m_QuestID);
			setVisible(false);
		}
	}

	CancelButton->setScale(1.0f);
	ComfirmButton->setScale(1.0f);
}

void Player_Talk_Layer::InitTalkClass()
{
	TalkClassFrame = Sprite::create("TalkClassFrame.png");
	TalkClassFrame->setPosition(VisiableSize.x / 2, VisiableSize.y / 2);
	addChild(TalkClassFrame);

	TalkClassString = LabelTTF::create("", "Arial", 33, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
	TalkClassString->setAnchorPoint(Vec2(0, 1.0f));
	TalkClassString->setPosition(TalkClassFrame->getContentSize().width * 0.08f, TalkClassFrame->getContentSize().height * 0.85f);
	TalkClassFrame->addChild(TalkClassString);


	for (int i = 0; i != 4; i++)
	{
		SpriteFrame[i] = Sprite::create("TalkClass_Single_Frame.png");
		float PosY = TalkClassFrame->getContentSize().height * 0.57f - (i * SpriteFrame[i]->getBoundingBox().size.height);
		SpriteFrame[i]->setPosition(TalkClassFrame->getContentSize().width / 2, PosY);
		SpriteFrame[i]->setTag(i);


		LabelTTF* SpriteText = LabelTTF::create("", "Arial", 30, Size::ZERO, TextHAlignment::LEFT);
		SpriteText->setString("123");
		SpriteText->setAnchorPoint(Vec2(0, 0.5f));
		SpriteText->setPosition(SpriteFrame[i]->getContentSize().width * 0.2f, SpriteFrame[i]->getContentSize().height / 2);
		SpriteText->setTag(1);
		SpriteFrame[i]->addChild(SpriteText);

		Sprite* ButtonImage = Sprite::create("TalkClass_Single_Frame_Button_0.png");
		ButtonImage->setPosition(SpriteFrame[i]->getContentSize().width * 0.15f, SpriteFrame[i]->getContentSize().height / 2);
		ButtonImage->setTag(2);
		SpriteFrame[i]->addChild(ButtonImage);


		ShowingTalkClass _template;
		_template.SingleTalkClass = SpriteFrame[i];
		m_Showing_Message.push_back(_template);

		SpriteFrame[i]->setVisible(false);
		TalkClassFrame->addChild(SpriteFrame[i]);
	}

	ButtonFrame = Sprite::create("TalkClassFrame_Button_Frame.png");
	ButtonFrame->setPosition(TalkClassFrame->getContentSize().width / 2, 0);
	TalkClassFrame->addChild(ButtonFrame);

	CancelButton = Sprite::create("TalkClassFrame_Button_1.png");
	CancelButton->setPosition(ButtonFrame->getContentSize().width / 2 - CancelButton->getBoundingBox().size.width / 2, ButtonFrame->getContentSize().height / 2);
	ButtonFrame->addChild(CancelButton);
	ComfirmButton = Sprite::create("TalkClassFrame_Button_0.png");
	ComfirmButton->setPosition(ButtonFrame->getContentSize().width / 2 + CancelButton->getBoundingBox().size.width / 2, ButtonFrame->getContentSize().height / 2);
	ButtonFrame->addChild(ComfirmButton);

}

void Player_Talk_Layer::SendQuestMenuToPlayer(uint32 Creatureid)
{
	IsQuestTalking = true;
	TalkClassString->setString("Can You Help Me ?");
	for (int i = 0; i != 4; i++)
		SpriteFrame[i]->setVisible(false);
	if (const std::list<uint32>* QuestList = sGame->GetCreatureQuests(Creatureid))
	{
		int k = 0;
		for (std::list<uint32>::const_iterator itr = QuestList->begin(); itr != QuestList->end(); itr++)
		{

			LabelTTF* ttf = (LabelTTF*)SpriteFrame[k]->getChildByTag(1);
			Sprite* ButtonImage = (Sprite*)SpriteFrame[k]->getChildByTag(2);

			if (!ttf || !ButtonImage)
				continue;

			if (sPlayer->HasQuest(*itr))
				continue;

			const QuestTemplate* _template = sQuestMgr->GetQuestTemplate(*itr);
			if (!_template)
			{
				char msg[255];
				snprintf(msg, 255, "Can Not Find Quest Template %d", (*itr));
				sNotifyMgr->ShowNotify(msg);
				continue;
			}

			ttf->setString(_template->Title.c_str());
			//需要增加ButtonImage变更
			SpriteFrame[k]->setVisible(true);
			SpriteFrame[k]->setTag(_template->ID);
			if (++k == 4)
				break;
		}
	}


	setVisible(true);
}

void Player_Talk_Layer::SendMenuToPlayer(const std::string& MainString)
{
	if (IsQuestTalking)
	{
		TalkClassString->setString(MainString);
		m_QuestID = m_Touched_Frame->getTag();
		return;
	}
	TalkClass* _class = sPlayer->PlayerTalkClass;
	for (int i = 0; i != 4; i++)
		SpriteFrame[i]->setVisible(false);

	int i = 0;
	for (; i != _class->Menus.size(); i++)
	{
		ShowingTalkClass& _template = m_Showing_Message.at(i);
		Sprite* FrameSprite = _template.SingleTalkClass;
		if (!FrameSprite)
			return;
		LabelTTF* ttf = (LabelTTF*)FrameSprite->getChildByTag(1);
		if (!ttf)
			return;

		FrameSprite->setVisible(true);
		_template.action = _class->Menus.at(i).action;
		_template.sender = _class->Menus.at(i).sender;
		_template.ImageID = _class->Menus.at(i).ImageId;
		_template.talkstring = _class->Menus.at(i).TalkString;
		ttf->setString(_template.talkstring.c_str());
	}

	TalkClassString->setString(MainString);
	setVisible(true);
	m_QuestID = 0;
}