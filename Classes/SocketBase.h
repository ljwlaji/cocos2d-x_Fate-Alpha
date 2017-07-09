
#ifndef __SOCKET_BASE_H__
#define __SOCKET_BASE_H__

#include "../cocos2d/cocos/cocos2d.h"
#include <list>
#include <thread>
USING_NS_CC;
// 对于windows平台

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
#endif
// 对于android平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <arpa/inet.h>  
// for inet_**
#include <netdb.h>  
// for gethost**
#include <netinet/in.h> 
// for sockaddr_in
#include <sys/types.h>  
// for socket
#include <sys/socket.h> 
// for socket
#include <unistd.h>
#include <stdio.h>  
// for printf
#include <stdlib.h> 
// for exit
#include <string.h> 
// for bzero
#define HSocket int
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define HSocket int
#endif



class SocketBase : public Ref

{

public:
	SocketBase();
	~SocketBase();
protected:
	void closeConnect(HSocket socket);
	bool error(HSocket socket);
protected:
	std::mutex _mutex;
private:
	bool _bInitSuccess;
};
#endif
