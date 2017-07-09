#include "SocketClient.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "NotifyMgr.h"
#include "Player.h"
#include "EnterLayer.h"
#include "MainUILayerNews.h"
#include "MainUILayerActionSprite.h"
#include "AuctionUISprite.h"

#pragma once
#pragma execution_character_set("utf-8")
void SocketClient::HandleBattleNetLoginOpcode(WorldPacket& packet)
{
	uint8 _Result = Login_Failed;
	packet >> _Result;
	if (_Result == Login_Failed)
	{
		sNotifyMgr->ShowUpWithNormalNotify("登录失败,请重试");
		return;
	}

	sNotifyMgr->ShowUpWithDisabaledTouching(5.0f, "登录成功,正在与服务器交换数据");
	WorldPacket epacket(CMSG_AUTH_DATA_RESULT, 2);
	epacket << (uint8)1;
	sSocket->SendPacket(&epacket);
}

void SocketClient::HandleServerHeartBeatOpcode(WorldPacket& /*packet*/)
{
	WorldPacket packet(CMSG_HEARTBEAT_RESULT, 2);
	packet << (uint8)1;
	sSocket->SendPacket(&packet);
}

void SocketClient::HandlePlayerLoginOpcode(WorldPacket& packet)
{
	sNotifyMgr->ShowUpWithDisabaledTouching(5.0f, "正在获取战网信息...");
	std::string Name = "";
	uint32 Race = 0;
	uint32 Class = 0;
	uint32 Gender = 0;
	uint32 Level = 0;
	uint32 Money = 0;
	uint32 guildid = 0;
	packet >> Name;
	if (!Name.empty())
	{
		packet >> Race;
		packet >> Class;
		packet >> Gender;
		packet >> Level;
		packet >> Money;
		packet >> guildid;
	}
	if (sPlayer)
	{
		//删除玩家
	}
	Player* _player = new Player(Name, Race, Class, Gender, Level, Money, guildid);
	sEnterLayer->IntoMainPage();
}

void SocketClient::HandleNewsData(WorldPacket& packet)
{
	uint32 Success = 0;
	packet >> Success;
	if (!Success)
	{
		sNotifyMgr->ShowUpWithDisabaledTouching(1.5f, "没有更多的新闻信息了");
		return;
	}
	
	uint32 Time = 0;
	std::string Title = "";
	std::string Message = "";
	std::string test = "";
	packet >> Time;
	packet >> Title;
	packet >> Message;
	sNews->NewsInput(Time, Title, Message);
}

void SocketClient::HandleActionsData(WorldPacket& packet)
{
	for (int i = 0; i < i + 1; i++)
	{
		std::string url = "";
		packet >> url;
		if (url.empty())
			break;
		sActionSprite->PushActionUrl(url);
		sActionSprite->SetCount(i + 1);
	}
	sActionSprite->InitSprite();
}

void SocketClient::HandleAuctionGetItemListOpcode(WorldPacket& packet)
{
	uint32 Success = 0;
	packet >> Success;
	if (!Success)
	{

	}
	uint32 AuctionID = 0;
	std::string ItemOwner = "";
	uint32 AuctionHouseID = 0;
	std::string ItemName = "";
	uint32 BuyOutPrice = 0;
	uint32 Time = 0;
	uint32 LastBid = 0;
	uint32 StartBid = 0;
	while (true)
	{
		AuctionID = 0;
		packet >> AuctionID;
		if (!AuctionID)
			return;

		packet >> ItemOwner;
		packet >> AuctionHouseID;
		packet >> ItemName;
		packet >> BuyOutPrice;
		packet >> Time;
		packet >> LastBid;
		packet >> StartBid;

		SingleAuctionItem _SingleAuctionItem(AuctionID, ItemOwner, AuctionHouseID, ItemName, BuyOutPrice, Time, LastBid, StartBid);
		sAuction->AddNewItem(_SingleAuctionItem);
	}
}