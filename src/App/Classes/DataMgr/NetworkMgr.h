/************************************************************************************
 * file: 		NetworkMgr.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:34:36
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

//网络管理模块
#ifndef __NETWORK_MGR_H__
#define __NETWORK_MGR_H__

#include <stdio.h>
#include "cocos2d.h"
#include "../Common/CMD_Stdafx.h"
#include "../Socket/TCPSocket.h"

class NetworkMgr : public cocos2d::Ref
{
public:
    NetworkMgr();
    ~NetworkMgr();
    
    static NetworkMgr *getInstance();
    
    //网络连接
    void doConnect(const char* domain, WORD wPort, EM_DATA_TYPE type);
    
    void Disconnect(EM_DATA_TYPE type);
    
    //消息接收
    void SocketDelegateWithRecvData(void* socket, void* pData, WORD wSize, bool isinBack);    //消息处理
    
    void sendData(WORD wMainCmdID, WORD wSubCmdID, void* buffer, WORD wSize, CTCPSocket* socket=NULL);

    //登录服务器消息
    void registerloadfunction(WORD wMainCmdID,gameMessageCallback callback);
    
    void unregisterloadfunction(WORD wMainCmdID);
    
    //用户服务消息
    void registeruserfunction(WORD wSubCmdID, MessageCallback1 callback);
    
    void unregisteruserfunction(WORD wSubCmdID);
    
    //游戏服务器消息
    void registerroomfunction(WORD wMainCmdID, gameMessageCallback callback);
    
    void unregisterroomfunction(WORD wMainCmdID);
    
public:
    //SUB_KN_DETECT_SOCKET  授权码错误
    void networkszCompilatioFalut(WORD  wSubCmdID, void* pData, WORD wSize);

    //用户服务
    void networkUserService(WORD wSubCmdID, void *pData, WORD wSize);

    //房间配置
    void roomconfigResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    //用户消息
    void networkGRUser(WORD  wSubCmdID, void* pData, WORD wSize);

    //桌子状态
    void networkGRStatus(WORD  wSubCmdID, void* pData, WORD wSize);

    //框架消息
    void networkGRFrame(WORD  wSubCmdID, void* pData, WORD wSize);

    //游戏消息
    void networkGRGame(WORD  wSubCmdID, void* pData, WORD wSize);

    //系统消息
    void networkGRSystem(WORD  wSubCmdID, void* pData, WORD wSize);
    
    //用户消息处理
    void OnUserEnter(void* pData, WORD wSize);
    void OnUserStatus(void* pData, WORD wSize);
    void OnUserScore(void* pData, WORD wSize);
    
    //框架处理
    //游戏状态
    void OnGameStatus(void *pData, WORD wSize);

    //游戏场景
    void OnGameScene(void *pData, WORD wSize);
    
//发送数据部分
public:
    void sendPacket_Compilatio(CTCPSocket *socket);
    
    //发送帐号登录
    void sendAccountLogin();
    
    //发送第三方登录
    void sendMethodLogin(int platform);
    
    //发送注册
    void sendRegister();
    
    //roompass房间密码
    void sendRoomLogin(const std::string &roompass);
    
    //发送游戏规则
    void sendGameOption();
    
    //发送加密
    void sendEncrypt(const std::string &pass);
    
    //请求用户
    void sendRequestUserInfo(DWORD userID, WORD tableID);

    //请求椅子用户
    void sendRequestChairUserInfo(WORD tableID, WORD chairID);

    //发送坐下
    void sendSitDown(WORD tableID, WORD chairID, const std::string &pass);
    
    //发送离开游戏
    void sendLeaveGame();
    
    //发送上传头像
    void sendCustomFaceInfo(cocos2d::Image *pimage);
    
    //绑定机器
    void bindingMachine(const BYTE &cbBind, const std::string &strpass);
        
    //网络变量
    CC_PROPERTY_READONLY(CTCPSocket*, m_pSocketData, SocketData)//长时连接
    CC_PROPERTY_READONLY(CTCPSocket*, m_pSocketOnce, SocketOnce)//一次连接

public:
    UserItemDelegate*		m_pUseItemDelegate;				//用户消息监听
    MessageCallback1		m_msgCallbackGameScene;
    
private:
    std::map<int, gameMessageCallback>	m_loadfunction;		//登陆回调列表
    std::map<int, MessageCallback1>		m_Userfunction;		//用户服务消息
    std::map<int, gameMessageCallback>	m_roomfunction;		//房间回调
};

#endif /* defined(__MyGame__NetworkMgr__) */
