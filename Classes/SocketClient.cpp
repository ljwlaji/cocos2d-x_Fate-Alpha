#include "SocketClient.h"
#include "WorldPacket.h"
#include "NotifyMgr.h"
#pragma once
#pragma execution_character_set("utf-8")
static SocketClient* _SocketClient = nullptr;
static std::mutex PlayerSocketLocker;

SocketClient::SocketClient(void) : onRecv(nullptr), _socektClient(0)
{

}

SocketClient::~SocketClient(void)
{
	if (_socektClient != 0)
	{
		_mutex.lock();
		this->closeConnect(_socektClient);
		_mutex.unlock();
	}
}
SocketClient* SocketClient::GetInstance()
{
	if (_SocketClient == nullptr)
	{
		ThreadLocker Locker(PlayerSocketLocker);
		if (_SocketClient == nullptr)
		{
			_SocketClient = new SocketClient();
		}
	}
	return _SocketClient;
}
bool SocketClient::initClient()
{
	if (_socektClient != 0)
	{
		_mutex.lock();
		this->closeConnect(_socektClient);
		_mutex.unlock();
	}
	_socektClient = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (error(_socketServer))
	{
		log("init client error!");
		_socektClient = 0;
		return false;
	}
	return true;
}

int SocketClient::connectServer(const char* serverIP, unsigned short port)
{
	if (!initClient())
	{
		return false;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);
	int ret = 0;
	ret = connect(_socektClient, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
	if (ret < 0)
	{
		this->closeConnect(_socektClient);
		_socektClient = 0;
		return false;
	}
	std::thread recvThread(&SocketClient::recvMessage, this);
	recvThread.detach();
	return true;
}

void SocketClient::recvMessage()
{
	char recvBuf[4096];
	int ret = 0;
	while (true)
	{
		ret = recv(_socektClient, recvBuf, sizeof(recvBuf), 0);
		if (ret > 0)
		{
			log("%s", recvBuf);
			RecvPacket(recvBuf);
			continue;
		}
	}
	_mutex.lock();
	this->closeConnect(_socektClient);
	if (onDisconnect != nullptr)
	{
		onDisconnect();
	}
	_socektClient = 0;
	_mutex.unlock();
}

void SocketClient::MessageComming(std::string args)
{

}

void SocketClient::sendMessage(const char* data, int count)
{
	if (!count)
		count = sizeof(data);

	if (_socektClient != 0)
	{
		int ret = send(_socektClient, data, count, 0);
		if (ret < 0)
		{
			log("send error!");
		}
	}
}

int SocketClient::SendPacket(const WorldPacket* packet)
{
	char send_buffer[1024];
	int send_len = (2 + 4) + packet->size();
	*((int*)send_buffer) = send_len;
	*((int*)(send_buffer + 4)) = packet->GetOpcode();
	if (!packet->empty())
		memcpy(send_buffer + 6, packet->contents(), packet->size());
	log("Send %d", sizeof(uint16)+sizeof(uint32)+packet->size());
	int ErrorCode = send(_socektClient, send_buffer, send_len, 0);
	if (ErrorCode == -1)
	{
		connectServer(IP_SERVER, PORT_SERVER);
		sNotifyMgr->ShowUpWithNormalNotify("与服务器连接中断,请重新尝试.");
	}
	
	return ErrorCode;
}

void SocketClient::RecvPacket(const char* buff)
{
	uint16 pack_len = *((uint16*)buff) - 8;
	uint16 op = *((uint16*)(buff + 4));
	if (!op || op > 0x424)
		return;
	WorldPacket* pack = new WorldPacket(op, pack_len);
	pack->resize(pack_len);
	pack->put(0, (const unsigned char*)buff + 8, pack_len);

	OpcodeHandler const& opHandle = opcodeTable[pack->GetOpcode()];
	(this->*opHandle.handler)(*pack);

	delete pack;
}