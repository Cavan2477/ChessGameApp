/************************************************************************************
 * file: 		TCPSocket.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:39:24
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __TCPSocket_h__
#define __TCPSocket_h__

#include "cocos2d.h"
#include "../Common/CMD_Stdafx.h"
#include "BSDSocket.h"
#include <stdio.h>
#include <iostream>
#include <pthread.h>

enum ConnectType
{
    unConnect = 0,//未连接
    Connecting,//正在连接
    Connected,//已连接
    Connect_Faild,//连接失败
    Connect_Kick_Out,//踢出
};


//线程接收函数
void *threadSocketRecv();

class RecvData;
//socket使用
class CTCPSocket : public cocos2d::Node
{
    //构造
public:
    CTCPSocket();
    ~CTCPSocket();
    
    static CTCPSocket* create();
    
    void setSocketTarget(gameMessageRecv recv);
    
    //网络回包数据调用
    void RecvDataUpdate(float time);
    
    void connetfaildDeal();
    
    //网络断开或异常返回登录界面
    //str提示内容
    void Disconnettologin(const std::string &str);
    
    //网络操作
public:
    bool socketConnect(const char *domain, WORD wPort, DataType type , bool isLoop=true);
    void socketClose();
    
    bool socketRecv();
    bool socketSend(char* pData, WORD wSize);
    
    //隐射数据
    bool mappedBuffer(void* pData, WORD wDataSize);
    
    //隐射数据
    bool unMappedBuffer(void* pData, WORD wDataSize);
    
    //多线程操作
public:
    bool threadCreate();
    void threadClosed();
    
public:
    void setLoop(bool loop)     { m_bLoop=loop;}        //设置循环
    void setEntry(bool isEntry) { m_bEntry=isEntry;}    //加密映射
    void setData(DataType dt)   { m_DataType=dt;}       //数据类型
    
    bool getLoop() {return m_bLoop;};
    bool getEntry() {return m_bEntry;};
    DataType getData() {return m_DataType;}
    
    bool getConnect();
    
    CBSDSocket *getSocket(){
        return m_pSocket;
    }
    
private:
    CBSDSocket*             m_pSocket;                  //网络连接
    pthread_t               m_hThread;                  //线程句丙
    bool                    m_bConnect;                 //连接状态 d:false
    bool                    m_bLoop;                    //接收标识 d:true
    
    //数据隐射标识在此添加
    bool                    m_bEntry;                   //隐射标识 d:true
    DataType                m_DataType;                 //数据类型 d:load
    
    //数据缓冲
    char                    m_pData[SOCKET_TCP_BUFFER]; //网络缓冲
    WORD                    m_wSize;                    //缓冲大小
    
    gameMessageRecv m_Recv;
    
    cocos2d::Vector<RecvData *> m_recvdataQueue;//接受到需要调用得数据
    
    std::mutex recvdatamutex;//控制数据调用得互斥锁

    CC_SYNTHESIZE(float, m_heartTime , HeartTime)
    
    CC_SYNTHESIZE(float, m_NoMessageTime , NoMessageTime)
    
    //网络连接状态
    CC_SYNTHESIZE(ConnectType, m_connecttype , ConnectType)
    
    //ip地址
    CC_SYNTHESIZE(std::string, m_domain, Domain)
    
    //端口号
    CC_SYNTHESIZE(WORD, m_wport, wPort)
};




#endif /* defined(__GameProject__TCPSocket__) */
