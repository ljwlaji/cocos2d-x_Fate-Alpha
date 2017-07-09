#include "MainUILayerNews.h"
#include "Common.h"
#include "SocketClient.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#pragma once
#pragma execution_character_set("utf-8")

static MainNewsSprite* _MainNewsSprite = nullptr;

MainNewsSprite::MainNewsSprite() : CurrentNew(0)
{
	initWithFile(MainUILayer_Info_Frame);
	autorelease();
	SetTouchType(Main_UISprite_News_Sprite);
}

MainNewsSprite::~MainNewsSprite()
{
	_MainNewsSprite = nullptr;
}

MainNewsSprite* MainNewsSprite::GetInstance()
{
	if (!_MainNewsSprite)
		_MainNewsSprite = new MainNewsSprite();
	return _MainNewsSprite;
}

void MainNewsSprite::SendNewsRequire()
{
	WorldPacket packet(CMSG_NEWS_REQUIRE);
	packet << (uint16)CurrentNew;
	sSocket->SendPacket(&packet);
}

void MainNewsSprite::InitSprite()
{
	Button = Sprite::create(MainUILayer_Info_Button);
	Button->setPosition(getContentSize().width / 2, getContentSize().height * 0.1f);
	addChild(Button);

	NewsTitle = LabelTTF::create("等待读取信息...", "Airal", 38);
	NewsTitle->setPosition(getContentSize().width / 2, getContentSize().height * 0.9f);
	addChild(NewsTitle);

	TimeTitle = LabelTTF::create("等待读取信息...", "Airal", 28);
	TimeTitle->setPosition(getContentSize().width / 2, NewsTitle->getBoundingBox().origin.y - getContentSize().height * 0.15f);
	addChild(TimeTitle);

	NewsDetail = LabelTTF::create("等待读取信息...", "Airal", 32);
	NewsDetail->setAnchorPoint(Vec2(0.5, 1.0f));
	NewsDetail->setPosition(getContentSize().width / 2, TimeTitle->getBoundingBox().origin.y - getContentSize().height * 0.1f);
	addChild(NewsDetail);

	SendNewsRequire();
}

bool MainNewsSprite::OnUITouchBegin(Touch* pTouch)
{
	if (Button->IsContectPoint(pTouch->getLocation()))
	{
		SendNewsRequire();
		return true;
	}
	return true;
}

void MainNewsSprite::OnUITouchEnded(Touch* pTouch)
{

}

void MainNewsSprite::NewsInput(const uint32& atime, const std::string& title, std::string& message)
{
	CurrentNew++;
	NewsTitle->setString(title.empty() ? "None" : title.c_str());
	char msg[255];
	uint32 LastTime = time(NULL) - atime;

	if (LastTime < 3600)		snprintf(msg, 255, "%d 分钟以前", LastTime / 60);
	else if (LastTime < 86400)	snprintf(msg, 255, "%d 小时以前", LastTime / 3600);
	else						snprintf(msg, 255, "%d 天以前", LastTime / 86400);

	TimeTitle->setString(msg);

	std::list<std::string> TextList;
	while (message.size())
	{
		std::string text = "";
		for (int i = 0; i < 47; i++)
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
				continue;
			}
			text += message.at(0);
			message.erase(0, 1);
		}
		text += "\n";
		TextList.push_back(text);
	}
	std::string OutPut = "";
	while (TextList.size())
	{
		OutPut += (*TextList.begin());
		TextList.pop_front();
	}
	NewsDetail->setString(OutPut.c_str());
}