#include "WorldChatSprite.h"
#include "Common.h"
#include "MainUILayer.h"
#include "Player.h"
#include "SocketClient.h"
#include "WorldPacket.h"

#pragma once
#pragma execution_character_set("utf-8")

static WorldChatSprite* _WorldChatSprite = nullptr;

WorldChatSprite::WorldChatSprite() : SelectedChannel(World_Channel)
{
	LoadCacheFrame("World_Chat.plist");
	initWithFile("World_Chat_Frame.png");
	autorelease();
	SetTouchType(Main_UISprite_WorldChat_Sprite);
}

WorldChatSprite::~WorldChatSprite()
{
	_WorldChatSprite = nullptr;
}

void WorldChatSprite::InitSprite()
{
	_TitleFrame = FrameCreate(World_Chat_Title_Frame);
	_TitleFrame->setLocalZOrder(1);
	_TitleFrame->setPosition(getContentSize().width / 2, getContentSize().height - _TitleFrame->getBoundingBox().size.height / 2);
	addChild(_TitleFrame);

	Title = FrameCreate(World_Chat_Title_Text);
	Title->setPosition(_TitleFrame->getContentSize() / 2);
	_TitleFrame->addChild(Title);

	Back_Arrow = FrameCreate(World_Chat_Back_Arrow);
	Back_Arrow->setPosition(_TitleFrame->getContentSize().width * 0.1f, _TitleFrame->getContentSize().height / 2);
	_TitleFrame->addChild(Back_Arrow);

	Buttom_Frame = FrameCreate(World_Chat_Buttom_Frame);
	Buttom_Frame->setPosition(getContentSize().width / 2, Buttom_Frame->getBoundingBox().size.height / 2);
	addChild(Buttom_Frame);

	Select_Channel = FrameCreate(World_Chat_Channel_Frame);
	Select_Channel->setPosition(Buttom_Frame->getContentSize().width * 0.09f, Buttom_Frame->getContentSize().height / 2);
	Buttom_Frame->addChild(Select_Channel);

	Sprite* tem = Sprite::create(World_Chat_Channel_World_Text);
	tem->setPosition(Select_Channel->getContentSize() / 2);
	tem->setTag(1);
	Select_Channel->addChild(tem);

	Channel_World = FrameCreate(World_Chat_Channel_Frame);
	Channel_World->setPosition(Select_Channel->getPositionX(), Buttom_Frame->getContentSize().height + Channel_World->getBoundingBox().size.height * 0.7f);
	Buttom_Frame->addChild(Channel_World);
	_fadeout(Channel_World);

	Sprite* taem = FrameCreate(World_Chat_Channel_World_Text);
	taem->setPosition(Select_Channel->getContentSize() / 2);
	Channel_World->addChild(taem);
	taem->setTag(1);

	Channel_Guild = FrameCreate(World_Chat_Channel_Frame);
	Channel_Guild->setPosition(Select_Channel->getPositionX(), Channel_World->getBoundingBox().size.height + Channel_World->getBoundingBox().origin.y + Channel_World->getBoundingBox().size.height * 0.7f);
	Buttom_Frame->addChild(Channel_Guild);
	_fadeout(Channel_Guild);

	Sprite* tebm = FrameCreate(World_Chat_Channel_Guild_Text);
	tebm->setPosition(Select_Channel->getContentSize() / 2);
	Channel_Guild->addChild(tebm);
	tebm->setTag(1);

	InputFrame = FrameCreate(World_Chat_Text_Input_Frame);
	InputFrame->setPosition(Buttom_Frame->getContentSize() / 2);
	Buttom_Frame->addChild(InputFrame);

	TextTyping = TextFieldTTF::textFieldWithPlaceHolder("战网账号", "arial", 30);
	TextTyping->setHorizontalAlignment(TextHAlignment::LEFT);
	TextTyping->setTag(1);
	TextTyping->setPosition(InputFrame->getContentSize() / 2);
	InputFrame->addChild(TextTyping);

	SendButton = FrameCreate(World_Chat_Send_Message_Icon);
	SendButton->setPosition(Buttom_Frame->getContentSize().width * 0.9f, Buttom_Frame->getContentSize().height / 2);
	Buttom_Frame->addChild(SendButton);


	//scheduleUpdate();
}

WorldChatSprite* WorldChatSprite::GetInstance()
{
	if (!_WorldChatSprite)
		_WorldChatSprite = new WorldChatSprite();
	return _WorldChatSprite;
}

bool WorldChatSprite::OnUITouchBegin(Touch* pTouch)
{
	if (Back_Arrow->IsContectPoint(pTouch->getLocation()))
	{
		runAction(MoveTo::create(0.3f, Vec2(Director::getInstance()->getVisibleSize().width + getContentSize().width / 2, getContentSize().height / 2)));
		sMainUI->SetChatShowing(false);
	}
	else if (InputFrame->IsContectPoint(pTouch->getLocation()))
	{
		TextTyping->attachWithIME();
	}
	else if (Select_Channel->IsContectPoint(pTouch->getLocation()))
	{
		_fadein(Channel_World);
		_fadein(Channel_Guild);
	}
	else if (SendButton->IsContectPoint(pTouch->getLocation()))
	{
		if (!TextTyping->getString().empty())
		{
			PushNewMessage(SelectedChannel, TextTyping->getString(), sPlayer->GetName().c_str(), false);
		}
	}
	else if (Channel_World->isVisible() && Channel_World->IsContectPoint(pTouch->getLocation()))
	{
		SetSelectedChannel(World_Channel);
		_fadeout(Channel_World);
		_fadeout(Channel_Guild);
		((Sprite*)Select_Channel->getChildByTag(1))->setTexture(World_Chat_Channel_World_Text);

	}
	else if (Channel_Guild->isVisible() && Channel_Guild->IsContectPoint(pTouch->getLocation()))
	{
		SetSelectedChannel(Guild_Channel);
		_fadeout(Channel_World);
		_fadeout(Channel_Guild);
		((Sprite*)Select_Channel->getChildByTag(1))->setTexture(World_Chat_Channel_Guild_Text);
	}
	return true;
}

void WorldChatSprite::_fadein(Sprite* pSprte)
{
	pSprte->runAction(FadeIn::create(0.5f));
	if (Sprite* psp = (Sprite*)pSprte->getChildByTag(1))
	{
		psp->runAction(FadeIn::create(0.5f));
	}
}

void WorldChatSprite::_fadeout(Sprite* pSprite)
{
	pSprite->runAction(FadeOut::create(0.5f));
	if (Sprite* psp = (Sprite*)pSprite->getChildByTag(1))
	{
		psp->runAction(FadeOut::create(0.5f));
	}
}

void WorldChatSprite::OnUITouchMoved(Touch* pTouch)
{

}

void WorldChatSprite::OnUITouchEnded(Touch* pTouch)
{

}

void WorldChatSprite::PushNewMessage(uint8 Channel, const std::string& Message, std::string SendBy, bool Input)
{
	ChatInfo _info;
	_info.ChatTime = time(NULL);
	_info.Channel = (BattleNet_Message_Channel)Channel;
	_info.Message = Message;
	_info.IsInputMessage = Input;
	_info.SendBy = SendBy;
	_info.LinkedSprite = nullptr;
	_info.LinkedLabel = nullptr;
	AddNewMessageToList(_info);
	MessageList.push_back(_info);

	if (!Input)
	{
		WorldPacket packet(CMSG_MESSAGE_CHAT);
		packet << (std::string)sPlayer->GetName().c_str();
		packet << (uint32)Channel;
		packet << (std::string)Message;
		sSocket->SendPacket(&packet);
	}
}

void WorldChatSprite::AddNewMessageToList(ChatInfo& info)
{
	std::string message = info.Message;
	std::list<std::string> TextList;
	while (message.size())
	{
		std::string text = "";
		int limitString = 35;
		for (int i = 0; i < limitString; i++)
		{
			if (message.empty())
				break;
			if (message[0] & 0x80)
			{
				text += message.at(0);
				message.erase(0, 1);
				text += message.at(0);
				message.erase(0, 1);
				text += message.at(0);
				message.erase(0, 1);
				i += 2;
				limitString++;
				continue;
			}
			text += message.at(0);
			message.erase(0, 1);
		}
		text += "\n";
		TextList.push_back(text);
	}
	std::string OutPut = "";
	switch (info.Channel)
	{
		case World_Channel: 
			OutPut += "[世界频道][";	
			break;
		case Guild_Channel: 
			OutPut += "[公会频道][";	
			break;
	}
	OutPut += info.SendBy;
	OutPut += "]:\n";
	while (TextList.size())
	{
		OutPut += (*TextList.begin());
		TextList.pop_front();
	}


	LabelTTF* TempTTF = LabelTTF::create(OutPut.c_str(), "Airal", 38, Size::ZERO, TextHAlignment::LEFT);
	addChild(TempTTF);
	info.Channel == World_Channel ? TempTTF->setColor(Color3B(255, 255, 0)) : TempTTF->setColor(Color3B(0, 255, 0));

	float ScaleY = 0;
	float ScaleX = 0;
	Sprite* TempSprite = FrameCreate(info.IsInputMessage ? World_Chat_Chat_Frame_Other : World_Chat_Chat_Frame_Self);
	ScaleY = TempTTF->getBoundingBox().size.height / TempSprite->getBoundingBox().size.height;
	ScaleX = TempTTF->getBoundingBox().size.width / TempSprite->getBoundingBox().size.width;
	TempSprite->setScaleX(ScaleX * 1.1f);
	TempSprite->setScaleY(ScaleY * 1.3f);
	TempSprite->setPositionX(info.IsInputMessage ? TempSprite->getBoundingBox().size.width / 2 + getContentSize().width * 0.05f : getContentSize().width - TempSprite->getBoundingBox().size.width / 2 - +getContentSize().width * 0.05f);
	TempSprite->setPositionY(TempSprite->getBoundingBox().size.height / 2 + Buttom_Frame->getBoundingBox().size.height * 1.2f);
	addChild(TempSprite);

	TempTTF->setAnchorPoint(Vec2(0.5f, 0.55f));
	TempTTF->setPosition(TempSprite->getPosition());

	info.LinkedLabel = TempTTF;
	info.LinkedSprite = TempSprite;

	for (std::list<ChatInfo>::iterator itr = MessageList.begin(); itr != MessageList.end(); itr++)
	{
		(*itr).LinkedLabel->runAction(MoveTo::create(0.8f, Vec2((*itr).LinkedLabel->getPositionX(), (*itr).LinkedLabel->getPositionY() + TempSprite->getBoundingBox().size.height + getContentSize().height * 0.02f)));
		(*itr).LinkedSprite->runAction(MoveTo::create(0.8f, Vec2((*itr).LinkedSprite->getPositionX(), (*itr).LinkedSprite->getPositionY() + TempSprite->getBoundingBox().size.height + getContentSize().height * 0.02f)));
	}
	if (MessageList.size() > 100)
	{
		std::list<ChatInfo>::iterator itr = MessageList.begin();
		(*itr).LinkedLabel->removeFromParentAndCleanup(true);
		(*itr).LinkedSprite->removeFromParentAndCleanup(true);
		MessageList.pop_front();
	}
	//int k = 0;
	//while (++k < 2)
	//{
	//	if (!MessageList.size())
	//		break;
	//	std::list<ChatInfo>::iterator itr = MessageList.begin();
	//	if ((*itr).LinkedLabel->getBoundingBox().size.height + (*itr).LinkedLabel->getBoundingBox().origin.y > _TitleFrame->getBoundingBox().origin.y)
	//	{
	//		(*itr).LinkedLabel->removeFromParentAndCleanup(true);
	//		(*itr).LinkedSprite->removeFromParentAndCleanup(true);
	//		MessageList.pop_front();
	//		continue;
	//	}
	//	else break;
	//}
}

void WorldChatSprite::update(float diff)
{
	if (diffa <= diff)
	{
		PushNewMessage(rand() % 100 > 50 ? 1 : 2, rand() % 100 > 50 ? "撒地方撒地方问问,skafhwoer,eoiwqueoinosdnf,sdklsdf撒地玩儿sdafsafwer 撒地方撒地方撒地方fjowiejrodsjnfjweasc..sdaf54987werdfsafewrsdfasdr,sdfkjhwoer" : "撒地方", "asfdwer", rand() % 100 > 50 ? true : false);
		diffa = 3;
	}
	else diffa -= diff;
}