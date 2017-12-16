/************************************************************************************
 * file: 		TCPSocket.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:36:56
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "TCPSocket.h"
#include "SocketRecvData.h"
#include "../DataMgr/HallDataMgr.h"
#include "../DataMgr/NetworkMgr.h"
#include "../Common/CMD_Stdafx.h"
#include "ModeLayer.h"
#include "Login.h"

USING_NS_CC;

using namespace std;

static unsigned char g_SendByteMap[256] = {
	0x70, 0x2F, 0x40, 0x5F, 0x44, 0x8E,
	0x6E, 0x45, 0x7E, 0xAB, 0x2C, 0x1F, 0xB4, 0xAC, 0x9D, 0x91, 0x0D,
	0x36, 0x9B, 0x0B, 0xD4, 0xC4, 0x39, 0x74, 0xBF, 0x23, 0x16, 0x14,
	0x06, 0xEB, 0x04, 0x3E, 0x12, 0x5C, 0x8B, 0xBC, 0x61, 0x63, 0xF6,
	0xA5, 0xE1, 0x65, 0xD8, 0xF5, 0x5A, 0x07, 0xF0, 0x13, 0xF2, 0x20,
	0x6B, 0x4A, 0x24, 0x59, 0x89, 0x64, 0xD7, 0x42, 0x6A, 0x5E, 0x3D,
	0x0A, 0x77, 0xE0, 0x80, 0x27, 0xB8, 0xC5, 0x8C, 0x0E, 0xFA, 0x8A,
	0xD5, 0x29, 0x56, 0x57, 0x6C, 0x53, 0x67, 0x41, 0xE8, 0x00, 0x1A,
	0xCE, 0x86, 0x83, 0xB0, 0x22, 0x28, 0x4D, 0x3F, 0x26, 0x46, 0x4F,
	0x6F, 0x2B, 0x72, 0x3A, 0xF1, 0x8D, 0x97, 0x95, 0x49, 0x84, 0xE5,
	0xE3, 0x79, 0x8F, 0x51, 0x10, 0xA8, 0x82, 0xC6, 0xDD, 0xFF, 0xFC,
	0xE4, 0xCF, 0xB3, 0x09, 0x5D, 0xEA, 0x9C, 0x34, 0xF9, 0x17, 0x9F,
	0xDA, 0x87, 0xF8, 0x15, 0x05, 0x3C, 0xD3, 0xA4, 0x85, 0x2E, 0xFB,
	0xEE, 0x47, 0x3B, 0xEF, 0x37, 0x7F, 0x93, 0xAF, 0x69, 0x0C, 0x71,
	0x31, 0xDE, 0x21, 0x75, 0xA0, 0xAA, 0xBA, 0x7C, 0x38, 0x02, 0xB7,
	0x81, 0x01, 0xFD, 0xE7, 0x1D, 0xCC, 0xCD, 0xBD, 0x1B, 0x7A, 0x2A,
	0xAD, 0x66, 0xBE, 0x55, 0x33, 0x03, 0xDB, 0x88, 0xB2, 0x1E, 0x4E,
	0xB9, 0xE6, 0xC2, 0xF7, 0xCB, 0x7D, 0xC9, 0x62, 0xC3, 0xA6, 0xDC,
	0xA7, 0x50, 0xB5, 0x4B, 0x94, 0xC0, 0x92, 0x4C, 0x11, 0x5B, 0x78,
	0xD9, 0xB1, 0xED, 0x19, 0xE9, 0xA1, 0x1C, 0xB6, 0x32, 0x99, 0xA3,
	0x76, 0x9E, 0x7B, 0x6D, 0x9A, 0x30, 0xD6, 0xA9, 0x25, 0xC7, 0xAE,
	0x96, 0x35, 0xD0, 0xBB, 0xD2, 0xC8, 0xA2, 0x08, 0xF3, 0xD1, 0x73,
	0xF4, 0x48, 0x2D, 0x90, 0xCA, 0xE2, 0x58, 0xC1, 0x18, 0x52, 0xFE,
	0xDF, 0x68, 0x98, 0x54, 0xEC, 0x60, 0x43, 0x0F };

static unsigned char g_RecvByteMap[256] = {	0x51, 0xA1, 0x9E, 0xB0, 0x1E, 0x83,
	0x1C, 0x2D, 0xE9, 0x77, 0x3D, 0x13, 0x93, 0x10, 0x45, 0xFF, 0x6D,
	0xC9, 0x20, 0x2F, 0x1B, 0x82, 0x1A, 0x7D, 0xF5, 0xCF, 0x52, 0xA8,
	0xD2, 0xA4, 0xB4, 0x0B, 0x31, 0x97, 0x57, 0x19, 0x34, 0xDF, 0x5B,
	0x41, 0x58, 0x49, 0xAA, 0x5F, 0x0A, 0xEF, 0x88, 0x01, 0xDC, 0x95,
	0xD4, 0xAF, 0x7B, 0xE3, 0x11, 0x8E, 0x9D, 0x16, 0x61, 0x8C, 0x84,
	0x3C, 0x1F, 0x5A, 0x02, 0x4F, 0x39, 0xFE, 0x04, 0x07, 0x5C, 0x8B,
	0xEE, 0x66, 0x33, 0xC4, 0xC8, 0x59, 0xB5, 0x5D, 0xC2, 0x6C, 0xF6,
	0x4D, 0xFB, 0xAE, 0x4A, 0x4B, 0xF3, 0x35, 0x2C, 0xCA, 0x21, 0x78,
	0x3B, 0x03, 0xFD, 0x24, 0xBD, 0x25, 0x37, 0x29, 0xAC, 0x4E, 0xF9,
	0x92, 0x3A, 0x32, 0x4C, 0xDA, 0x06, 0x5E, 0x00, 0x94, 0x60, 0xEC,
	0x17, 0x98, 0xD7, 0x3E, 0xCB, 0x6A, 0xA9, 0xD9, 0x9C, 0xBB, 0x08,
	0x8F, 0x40, 0xA0, 0x6F, 0x55, 0x67, 0x87, 0x54, 0x80, 0xB2, 0x36,
	0x47, 0x22, 0x44, 0x63, 0x05, 0x6B, 0xF0, 0x0F, 0xC7, 0x90, 0xC5,
	0x65, 0xE2, 0x64, 0xFA, 0xD5, 0xDB, 0x12, 0x7A, 0x0E, 0xD8, 0x7E,
	0x99, 0xD1, 0xE8, 0xD6, 0x86, 0x27, 0xBF, 0xC1, 0x6E, 0xDE, 0x9A,
	0x09, 0x0D, 0xAB, 0xE1, 0x91, 0x56, 0xCD, 0xB3, 0x76, 0x0C, 0xC3,
	0xD3, 0x9F, 0x42, 0xB6, 0x9B, 0xE5, 0x23, 0xA7, 0xAD, 0x18, 0xC6,
	0xF4, 0xB8, 0xBE, 0x15, 0x43, 0x70, 0xE0, 0xE7, 0xBC, 0xF1, 0xBA,
	0xA5, 0xA6, 0x53, 0x75, 0xE4, 0xEB, 0xE6, 0x85, 0x14, 0x48, 0xDD,
	0x38, 0x2A, 0xCC, 0x7F, 0xB1, 0xC0, 0x71, 0x96, 0xF8, 0x3F, 0x28,
	0xF2, 0x69, 0x74, 0x68, 0xB7, 0xA3, 0x50, 0xD0, 0x79, 0x1D, 0xFC,
	0xCE, 0x8A, 0x8D, 0x2E, 0x62, 0x30, 0xEA, 0xED, 0x2B, 0x26, 0xB9,
	0x81, 0x7C, 0x46, 0x89, 0x73, 0xA2, 0xF7, 0x72 };


//线程接收函数
void *threadSocketRecv(void* param)
{
    //接收消息
    CTCPSocket* socket=(CTCPSocket*)param;
    
    while(socket->getLoop())
    {
        if (socket->getConnectType() == unConnect) {
            socket->setConnectType(Connecting);
            std::string sdomain(socket->getDomain());
            CCLOG("网络连接地址 %s",socket->getDomain().c_str());
            bool ret = socket->getSocket()->Connect(socket->getDomain().c_str(), socket->getwPort());
            if (ret == true) {
                socket->setHeartTime(0.f);
                socket->setConnectType(Connected);
            }
            else
            {
                CCLOG("连接服务器失败");
                socket->setConnectType(Connect_Faild);
                break;
            }
        }
        else if(socket->getConnectType() == Connected)
        {
            if (!socket->socketRecv()) {
                break;
            }
        }
        sleep(1.0/60.0f);
    }
    

    //socket->socketClose();
    pthread_exit(NULL);
    
    return 0;
}



CTCPSocket* CTCPSocket::create()
{
    CTCPSocket* socket=new CTCPSocket;
    socket->autorelease();
    return socket;
}

CTCPSocket::CTCPSocket()
:m_heartTime(0.f)
,m_connecttype(unConnect),m_NoMessageTime(0.f)
{
    m_hThread = 0;
    m_bLoop = true;
    m_bConnect = false;
    m_pSocket = NULL;
    
    m_bEntry = true;
    m_DataType = EM_DATA_TYPE_LOAD;
    m_wSize = 0;
    m_Recv = nullptr;
    memset(m_pData, 0, sizeof(m_pData));
    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(CTCPSocket::RecvDataUpdate), this, 0, false);
}


CTCPSocket::~CTCPSocket()
{
    //this->socketClose();
    m_recvdataQueue.clear();
    
}

//CTCPSocket * CTCPSocket::sharedsocket()
//{
//    if (m_single == NULL) {
//        m_single = new CTCPSocket();
//    }
//    return  m_single;
//}

void CTCPSocket::setSocketTarget(gameMessageRecv recv)
{
    m_Recv = recv;
}


bool CTCPSocket::socketConnect(const char *domain, WORD wPort, EM_DATA_TYPE type , bool isLoop)
{
    //pthread_mutex_lock(&Connectmutex);
    m_pSocket = new CBSDSocket();
    m_pSocket->Init();
    m_pSocket->Create(AF_INET, SOCK_STREAM);
    
    //连接方式
    m_bLoop=isLoop;
    //pthread_mutex_unlock(&Connectmutex);
    m_DataType = type;
    memset(m_pData, 0, sizeof(m_pData));
    m_domain = domain;
    m_wport = wPort;
    this->threadCreate();
    return true;
}


void CTCPSocket::RecvDataUpdate(float time)
{
    int type = m_connecttype;
    if (type == unConnect) {
        return;
    }
    if (type == Connecting) {
        m_heartTime += time;
        if (m_heartTime > 30.f) {
            this->Disconnettologin("网络连接超时，请重新登录");
            Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(CTCPSocket::RecvDataUpdate), this);
            this->socketClose();
        }
        
        return;
    }
    if (type == Connect_Faild) {
        this->connetfaildDeal();
        return;
    }
    if (type == Connect_Kick_Out) {
        this->Disconnettologin("网络异常，请重新登录");
        Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(CTCPSocket::RecvDataUpdate), this);
        this->socketClose();
        return;
    }
    if (m_NoMessageTime > 60.f) {
        if (m_DataType == !EM_DATA_TYPE_LOAD) {
            this->Disconnettologin("网络异常，请重新登录");
        }
        NetworkMgr::getInstance()->Disconnect(m_DataType);
        return;
    }
    cocos2d::Vector<RecvData *> vectordata;
    RecvData *rdata = NULL;
    recvdatamutex.lock();
    if (!m_recvdataQueue.size()) {
        m_heartTime += time;
        m_NoMessageTime += time;
        recvdatamutex.unlock();
        
        if (m_heartTime > 10) {
            TCP_Buffer	heartBuffer;
            memset(&heartBuffer,0,sizeof(TCP_Buffer));
            heartBuffer.Head.CommandInfo.wMainCmdID	= MDM_KN_COMMAND;
            heartBuffer.Head.CommandInfo.wSubCmdID	= SUB_KN_DETECT_SOCKET;
            this->socketSend((char *)&heartBuffer, sizeof(heartBuffer.Head));
            m_heartTime = 0;
        }
        return;
    }
    else
    {
        vectordata = m_recvdataQueue;
        m_recvdataQueue.clear();
        recvdatamutex.unlock();
        m_heartTime = 0;
        m_NoMessageTime = 0.f;
    }
    while(vectordata.size() != 0) {
         rdata = vectordata.at(0);
        if (rdata) {
            int datasize = rdata->getDatasize();
            if (rdata->getdataType() == Send_Data) {
                this->socketSend(rdata->getRecvData(), datasize);
            }
            else
            {
                if (m_Recv) {
                    m_Recv(this,rdata->getRecvData(),datasize, rdata->getIsInBack());
                }
            }
            vectordata.erase(0);
        }
    }
}

void CTCPSocket::connetfaildDeal()
{
    int code=this->getSocket()->GetError();
    switch (code)
    {
            
        case ECONNREFUSED:
        {
            CCLOG("CTCPSocket: 服务器拒绝，无法响应，请留意系统公告。");
        }
            break;
        case ETIMEDOUT:
        {
            CCLOG("CTCPSocket: 服务器连接超时，请稍后重试");
        }
            break;
        case ECONNRESET:
        {
            CCLOG("CTCPSocket: 连接被强制关闭，请注意操作是否非法。");
        }
            break;
        case ENETUNREACH:
        {
            CCLOG("CTCPSocket: 服务器无法响应，请留意系统公告。");
        }
            break;
        case EHOSTUNREACH:
        {
            CCLOG("CTCPSocket: 无网络连接，请检查网络状况");
        }
            break;
        case 0:
        case EINVAL:
        {
            CCLOG("CTCPSocket: 无网络连接");
        }
            break;
        case ENOENT:
        {
            CCLOG("CTCPSocket: 服务器无响应，请留意系统公告后重试");
        }
            break;
        default:
        {
            
        }
            break;
    }
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(CTCPSocket::RecvDataUpdate), this);
    this->Disconnettologin("无网络连接，请检查网络状况");
    this->socketClose();
}

void CTCPSocket::Disconnettologin(const std::string &str)
{
    auto player = (ModeLayer *)HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    player->setEnsureCallback([=]{
        if (!Director::getInstance()->getRunningScene()->getChildByTag(10)) {
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Login::createScene()));
        }
    });
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_ROOM);
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Socket_Connect_Failure);
}

//关闭socket
void CTCPSocket::socketClose()
{
    if (m_connecttype  == unConnect) {
        return;
    }
    m_bConnect = false;
    if (m_hThread) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        pthread_cancel(m_hThread);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        pthread_kill(m_hThread,0);
#endif
        m_hThread = 0;
    }
    
    m_bLoop = false;
    m_wSize=0;
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(CTCPSocket::RecvDataUpdate), this);
    //关闭socket
    if(m_pSocket)
    {
        m_pSocket->Clean();
        m_pSocket->Close();
        CC_SAFE_DELETE(m_pSocket);
    }
    this->threadClosed();
    m_connecttype = unConnect;
}

//消息接收
bool CTCPSocket::socketRecv()
{
    if (m_pSocket == NULL) {
        return false;
    }
    int nSize=m_pSocket->Recv(m_pData+m_wSize, SOCKET_TCP_BUFFER-m_wSize);
    
    if (nSize == -1)
    {
        if (m_pSocket == NULL) {
            return false;
        }
        if (m_connecttype == Connected) {
            m_connecttype = Connect_Kick_Out;
        }
        return false;
    }
    if (nSize == 0) {
        return false;
    }
    
    if(nSize!=0)
    {
        CCLOG("接收长度%d",nSize);
        
        m_wSize += nSize;
    }
    
    //wh协议映射
    if(m_bEntry)
    {
        while ( m_wSize>=sizeof(TCP_Head) )
        {
            //取出前8字节数据
            char headChar[8];
            memset(headChar,0,sizeof(headChar));
            memcpy(headChar,m_pData,8);
            
            //取的数据长度
            TCP_Head* head = (TCP_Head*)headChar;
            WORD wCurSize = head->TCPInfo.wPacketSize;
            
            //长度效验，小于包头 或者 大于当前数据总长度,说明只接收了一半数据，跳出处理
            if( wCurSize<sizeof(TCP_Head) || wCurSize>m_wSize){
                break;
            }
            
            //取的当前长度 开始处理
            char curBuffer[wCurSize];
            
            memset(curBuffer,0,sizeof(curBuffer));
            memcpy(curBuffer,m_pData,wCurSize);
            
            //数据映射
            if( !unMappedBuffer(curBuffer,wCurSize)){
                CCLOG("数据隐射错误");
                break;
            }
            
            CCLOG("处理长度:%d",wCurSize);
            RecvData *rdata = new RecvData(curBuffer, wCurSize);
            rdata->setIsInBack(HallDataMgr::getInstance()->m_isEnterBack);
            recvdatamutex.lock();
            m_recvdataQueue.pushBack(rdata);
            recvdatamutex.unlock();
            rdata->release();

            //减去已处理长度
            m_wSize -= wCurSize;
            
            //数据移动
            memmove(m_pData, m_pData+wCurSize, m_wSize);
            
            //跳出判断
            if( m_wSize<sizeof(TCP_Head) ){
                CCLOG("处理完一次数据－－－－－－－－－－－－－－－－－－－－－－");
                break;
            }
        }
    }
    
    //其他映射
    else
    {
        //直接处理
        //if(m_pTarget && m_pTarget->SocketDelegateWithRecvData(this, m_pData, m_wSize))
        {
            
            
        }
    }
    return true;
}

bool CTCPSocket::socketSend(char* pData, WORD wSize)
{
    //数据大包
    //assert(m_bConnect != false);
    if (m_pSocket == NULL) {
        return false;
    }
    if (m_connecttype == Connect_Kick_Out) {
        return false;
    }
    if (m_connecttype != Connected) {
        RecvData *rdata = new RecvData(pData, wSize);
        rdata->setdataType(Send_Data);
        recvdatamutex.lock();
        m_recvdataQueue.pushBack(rdata);
        recvdatamutex.unlock();
        rdata->release();
        return false;
    }
    this->mappedBuffer(pData, wSize);
    int count = m_pSocket->Send(pData, wSize);
    if(count == SOCKET_ERROR)
    {
        if (HallDataMgr::getInstance()->m_bStartGame) {
            EventCustom event(whNd_Socket_Disconnect);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
        else
        {
            this->Disconnettologin("网络异常，请重新登录");
        }
    }
    
    return true;
}

//隐射数据
bool CTCPSocket::mappedBuffer(void* pData, WORD wDataSize)
{
	//变量定义
	BYTE *buffer = (BYTE*)pData;
	BYTE cbCheckCode = 0;
	
	//映射数据
	for(WORD i=sizeof(TCP_Info);i<wDataSize;i++)
	{
		cbCheckCode+=buffer[i];
		buffer[i]=g_SendByteMap[buffer[i]];
	}
	
	//设置数据
	TCP_Info *pInfo	 = (TCP_Info*)pData;
	pInfo->cbCheckCode = ~cbCheckCode+1;
	pInfo->wPacketSize = wDataSize;
	pInfo->cbDataKind |= DK_MAPPED;
    
    return true;
}

//隐射数据
bool CTCPSocket::unMappedBuffer(void* pData, WORD wDataSize)
{
    //变量定义
	BYTE* buffer=(BYTE*)pData;
	TCP_Info* pInfo=(TCP_Info*)pData;
	
	//映射
	if( (pInfo->cbDataKind & DK_MAPPED) !=0)
	{
		BYTE cbCheckCode = pInfo->cbCheckCode;
		
		for(WORD i=sizeof(TCP_Info);i<wDataSize;i++)
		{
			cbCheckCode += g_RecvByteMap[buffer[i]];
			buffer[i] = g_RecvByteMap[buffer[i]];
		}
		//效验
		if(cbCheckCode!=0)
			return false;
	}
	return true;

}

//多线程创建
bool CTCPSocket::threadCreate()
{
#ifdef WIN32
#ifdef PTW32_STATIC_LIB
    pthread_win32_process_attach_np();
#endif
#endif
    
    //创建线程
    pthread_attr_t attr;
   
    //1/4 init
    pthread_attr_init(&attr);
    
    //2/4: explicitly specify as joinable or detached
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    //3/4 create
    int code = pthread_create(&m_hThread, 0, threadSocketRecv, this);

    if(code!=0)
        CCLOG("线程创建失败");
    
    //4/4 join
    pthread_detach(m_hThread);

    return false;
}

//detach线程
void CTCPSocket::threadClosed()
{
    m_bConnect=false;


#ifdef WIN32
#ifdef PTW32_STATIC_LIB
    pthread_win32_process_detach_np();
#endif
#endif
    
}

bool CTCPSocket::getConnect()
{
    return this->m_bConnect;
}
















