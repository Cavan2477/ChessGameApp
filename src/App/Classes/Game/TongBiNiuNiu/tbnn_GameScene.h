//
//  tbnn_GameScene.hpp
//  GameProject
//
//  Created by zhong on 4/18/16.
//
//

#ifndef tbnn_GameScene_hpp
#define tbnn_GameScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameDefine.h"
#include "TBNNScene.h"
#include "GameSceneUI/GameLayer.h"

TBNN_NAMESPACE_BEGIN

class GameScene: public UIScene,
public UserItemDelegate
{
    UISCENE_DECLAR(GameScene,TBNN_SCENE(GAME_SCENE));
    
    //异步预加载资源
    static std::vector<tagUIResourceParam> uiAsynLoadResource();
    
    //资源加载结束
    static std::vector<tagUIResourceParam> uiAsynLoadFinished();
    
    //释放资源
    static std::vector<tagUIResourceParam> uiUnloadResource();
    
    //用户进入
    virtual void OnUserEnter(void* pUser) override;
    
    //用户状态
    virtual void OnUserStatus(void* pUser) override;
    
    //用户分数
    virtual void OnUserScore(void* pUser) override;
    
    //用户聊天
    virtual void OnUserChat(WORD wSubCode, void* pData, WORD wSize) override;
    
    //点击确认离开游戏
    CC_SYNTHESIZE(bool, m_bClickExit, ClickExit);
private:
    //接收到ui线程消息
    bool onGetGameUIMsg(const int &sub,void *pData,const int &nLen);
private:
    ////////////////////////////////////////////////////////////////////////// 网络消息
    //游戏场景消息
    void onGameScene(void *pData, WORD wSize);
    //游戏消息
    void onGameMessage(WORD wSubCmdID, void *pData, WORD wSize);
    
    /*
     * 处理user消息
     */
    //用户状态变化
    void onGetUserStatus(BYTE *pBuffer, const int &size);
    //用户文本聊天
    void onGetUserTextChat(void *pBuffer);
    //用户表情聊天
    void onGetUserBrowChat(void *pBuffer);
    
    /*
     * 处理游戏消息
     */
    //游戏开始
    void onGetGameStart(void *pData, WORD wSize);
    //用户强退
    void onGetUserForceQuit(void *pData, WORD wSize);
    //发牌消息
    void onGetDispatchCard(void *pData, WORD wSize);
    //游戏结束
    void onGetGameEnd(void *pData, WORD wSize);
    //用户摊牌
    void onGetUserOpenCard(void *pData, WORD wSize);
    //系统控制
    void onGetSystemControl(void *pData, WORD wSize);
    //刷新用户
    void onGetRefreshUser(void *pData, WORD wSize);
    
private:
    void registerNetMessage();
    
    void removeNetMessage();
    
    //离开游戏
    void notifyLeaveGame(cocos2d::EventCustom *event);
    
    void replaceScene();
    
private:
    GameLayer *m_gameLayer;
};

TBNN_NAMESPACE_END
#endif /* tbnn_GameScene_hpp */
