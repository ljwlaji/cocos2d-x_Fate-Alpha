#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include "SocketBase.h"

#define sSocket				SocketClient::GetInstance()
class WorldPacket;
class SocketClient : public SocketBase
{
public:
	int connectServer(const char* serverIP, unsigned short port);
	void sendMessage(const char* data, int count = 0);
	std::function<void(const char* data, int count)> onRecv;
	std::function<void()> onDisconnect;
	static SocketClient* GetInstance();
	int SendPacket(const WorldPacket* pck);
private:
	SocketClient(void);
	~SocketClient(void);
	bool initClient();
	void recvMessage();
	void MessageComming(std::string args);
	void RecvPacket(const char* buff);
private:
	HSocket _socketServer;
	HSocket _socektClient;
#define ThreadLocker		std::lock_guard<std::mutex>

public:
	void Handle_NULL(WorldPacket& /*packet*/) {}
	void HandleServerHeartBeatOpcode(WorldPacket& packet);
	void HandleBattleNetLoginOpcode(WorldPacket& packet);
	void HandlePlayerLoginOpcode(WorldPacket& packet);
	void HandleNewsData(WorldPacket& packet);
	void HandleActionsData(WorldPacket& packet);
	void HandleAuctionGetItemListOpcode(WorldPacket& packet);
};



#endif