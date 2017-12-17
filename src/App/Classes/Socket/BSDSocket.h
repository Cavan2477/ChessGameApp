/************************************************************************************
 * file: 		BSDSocket.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:37:56
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __BSDSOCKET_H__
#define __BSDSOCKET_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include <ifaddrs.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "cocos2d.h"
#include "errno.h"

//typedef int				SOCKET;

//#pragma region define win32 const variable in linux
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
//#pragma endregion

class CBSDSocket
{
public:
	CBSDSocket(SOCKET sock = INVALID_SOCKET);
	~CBSDSocket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* domain, unsigned short port);
    bool Connect(struct sockaddr_in svraddr);

    //#region server
	// Bind socket
	bool Bind(unsigned short port);

	// Listen socket
	bool Listen(int backlog = 5); 

	// Accept socket
	bool Accept(CBSDSocket& s, char* fromip = NULL);
	//#endregion
	bool select();
	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);
	
	// Close socket
	int Close();

	// Get errno
	int GetError();
	
	//#pragma region just for win32
	// Init winsock DLL 
	static int Init();
    
	// Clean winsock DLL
	static int Clean();
    
	//#pragma endregion
    bool isLocalWIFI();
    
	// Domain parse
	static bool DnsParse(const char* domain, char* ip);
    
	CBSDSocket& operator = (SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;
};

int check_conn_is_ok(SOCKET sock);

#endif
