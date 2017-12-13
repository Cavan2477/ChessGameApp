/************************************************************************************
 * file: 		BSDSocket.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:40:12
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "BSDSocket.h"
#include <stdio.h>
#include "poll.h"

#ifdef WIN32
	#pragma comment(lib, "wsock32")
#endif

CBSDSocket::CBSDSocket(SOCKET sock)
{
	m_sock = sock;
}

CBSDSocket::~CBSDSocket()
{
}

int CBSDSocket::Init()
{
#ifdef WIN32
	/*
	http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	typedef struct WSAData { 
		WORD wVersion;								//winsock version
		WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
		char szDescription[WSADESCRIPTION_LEN+1]; 
		char szSystemStatus[WSASYSSTATUS_LEN+1]; 
		unsigned short iMaxSockets; 
		unsigned short iMaxUdpDg; 
		char FAR * lpVendorInfo; 
	}WSADATA, *LPWSADATA; 
	*/
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if ( ret ) {
//		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif
	
	return 0;
}
//this is just for windows
int CBSDSocket::Clean()
{
#ifdef WIN32
		return (WSACleanup());
#endif
		return 0;
}

CBSDSocket& CBSDSocket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

CBSDSocket::operator SOCKET ()
{
	return m_sock;
}
//create a socket object win/lin is the same
// af:
bool CBSDSocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if ( m_sock == INVALID_SOCKET )
    {
		return false;
	}
    
    //fcntl(m_sock, F_SETFL, O_NONBLOCK);
    
	return true;
}

bool CBSDSocket::Connect(const char* domain, unsigned short port)
{
    //连接ip
    char ip[25];
    memset(ip, 0, sizeof(ip));
    
    //域名解析
    if( domain[0]>='0' && domain[0]<='9' )
        strcpy(ip, domain);
    else
        this->DnsParse(domain, ip);
    
    //连接ip
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
    int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
    if ( ret == SOCKET_ERROR )
    {
        if(errno == EINTR) {
            if(check_conn_is_ok(m_sock) < 0) {
                perror("connect");
                return false;
            }
            else {
                printf("connect is success!\n");
                return  true;
            }
        }
        else {
            perror("connect");
            return false;
        }
    }
	return true;
}

bool CBSDSocket::Connect(struct sockaddr_in svraddr)
{
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR )
    {
//        struct pollfd unix_really_sucks;
//        int some_more_junk;
//        socklen_t yet_more_useless_junk;
//        
//        if ( errno != EINTR /* && errno != EINPROGRESS */ )
//        {
//            perror ("connect");
//            exit (EXIT_FAILURE);
//        }
//        unix_really_sucks.fd = m_sock;
//        unix_really_sucks.events = POLLOUT;
//        while ( poll (&unix_really_sucks, 1, -1) == -1 )
//            if ( errno != EINTR )
//            {
//                perror ("poll");
//                exit (EXIT_FAILURE);
//            }
//        yet_more_useless_junk = sizeof(some_more_junk);
//        if ( getsockopt (m_sock, SOL_SOCKET, SO_ERROR,
//                         &some_more_junk,
//                         &yet_more_useless_junk) == -1 )
//        {
//            perror ("getsockopt");
//            exit (EXIT_FAILURE);
//        }
//        if ( some_more_junk != 0 )
//        {
//            fprintf (stderr, "connect: %s\n",
//                     strerror (some_more_junk));
//            exit (EXIT_FAILURE);
//        }
		return false;
	}
	return true;
}


bool CBSDSocket::isLocalWIFI()
{
#ifdef WIN32
    //添加win32检测方法
    
    
    return false;

#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    //ios下检测是否wifi
    struct ifaddrs *addresses;
    struct ifaddrs *cursor;
    bool wiFiAvailable = false;
//    
//    if (getifaddrs(&addresses) != 0) return wiFiAvailable;
//    
//    cursor = addresses;
//    while (cursor != NULL)
//    {
//    	if (cursor -> ifa_addr -> sa_family == AF_INET && !(cursor -> ifa_flags & IFF_LOOPBACK)) // Ignore the loopback address
//    	{
//    		// Check for WiFi adapter
//    		if (strcmp(cursor -> ifa_name, "en0") == 0)
//            {
//    			wiFiAvailable = true;
//    			break;
//    		}
//    	}
//    	cursor = cursor -> ifa_next;
//    }
//    
//    freeifaddrs(addresses);

    return wiFiAvailable;
    
#endif
    
    return false;
}



bool CBSDSocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt =  1;
	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 ) 
		return false;

	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

//for server
bool CBSDSocket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool CBSDSocket::Accept(CBSDSocket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if ( sock == SOCKET_ERROR ) {
		return false;
	}

	s = sock;
	if ( fromip != NULL )
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int CBSDSocket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;

	while( count < len )
    {
		bytes = (int)send(m_sock, buf + count, len - count, flags);
		if ( bytes == -1 || bytes == 0 )
			return -1;
        
		count += bytes;
	} 

	return count;
}

int CBSDSocket::Recv(char* buf, int len, int flags)
{
	return (int)(recv(m_sock, buf, len, flags));
}

int CBSDSocket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
    shutdown(m_sock, 2);
	return (close(m_sock));
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    shutdown(m_sock, 2);
    return (close(m_sock));
#endif
}

int CBSDSocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}

bool CBSDSocket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ( (p = gethostbyname(domain)) == NULL )
		return false;
		
	sprintf(ip,
            "%u.%u.%u.%u",
            (unsigned char)p->h_addr_list[0][0],
            (unsigned char)p->h_addr_list[0][1],
            (unsigned char)p->h_addr_list[0][2],
            (unsigned char)p->h_addr_list[0][3]);
	
	return true;
}

int check_conn_is_ok(SOCKET sock) {
	struct pollfd fd;
	int ret = 0;
	socklen_t len = 0;
    
	fd.fd = sock;
	fd.events = POLLOUT;
    
	while ( poll (&fd, 1, -1) == -1 ) {
		if( errno != EINTR ){
			perror("poll");
			return -1;
		}
	}
    
	len = sizeof(ret);
	if ( getsockopt (sock, SOL_SOCKET, SO_ERROR,
                     &ret,
                     &len) == -1 ) {
        perror("getsockopt");
		return -1;
	}
    
	if(ret != 0) {
		fprintf (stderr, "socket %d connect failed: %s\n",
                 sock, strerror (ret));
		return -1;
	}
    
	return 0;
    
}
