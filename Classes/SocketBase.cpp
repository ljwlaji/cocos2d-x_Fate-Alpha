#include "SocketBase.h"
SocketBase::SocketBase()
{
	_bInitSuccess = false;

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(2, 0);
		WSADATA wsaData;
		int nRet = WSAStartup(wVersionRequested, &wsaData);
		if (nRet != 0)
		{
			fprintf(stderr, "Initilize Error!\n");
			return;
		}
		_bInitSuccess = true;
	}
	#endif
}

SocketBase::~SocketBase()
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	if (_bInitSuccess)
	{
		WSACleanup();
	}
	#endif   
}

void SocketBase::closeConnect(HSocket socket)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	close(socket);
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	closesocket(socket);
	#endif  
}

bool SocketBase::error(HSocket socket)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	return socket == SOCKET_ERROR;
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	return socket < 0;
	#endif  
}