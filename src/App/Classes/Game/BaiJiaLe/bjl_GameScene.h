//
//  GameScene.hpp
//  DouDiZhu
//
//  Created by zhong on 1/21/16.
//
//

#ifndef bjl_header_GameScene_hpp
#define bjl_header_GameScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameSceneUI/GameLayer.h"
#include "BaiJiaLeScene.h"
#include "GameDefine.h"

BJL_NAMESPACE_BEGIN

class GameScene:public UIScene,
public UserItemDelegate
{
    UISCENE_DECLAR(GameScene,BJL_SCENE(GAME_SCENE));
    
    //异步预加载资源
    static std::vector<tagUIResourceParam> uiAsynLoadResource();
    
    //资源加载结束
    static std::vector<tagUIResourceParam> uiAsynLoadFinished();
    
    //释放资源
    static std::vector<tagUIResourceParam> uiUnloadResource();
    
    CC_SYNTHESIZE(WORD,m_wGameTable,GameTable);
    CC_SYNTHESIZE(WORD,m_wGameChair,GameChair);
    CC_SYNTHESIZE(bool,m_bSitDown,SitDown);
    //点击确认离开游戏
    CC_SYNTHESIZE(bool, m_bClickExit, ClickExit);
private:
    //接收到ui线程消息
    bool onGetGameUIMsg(const int &sub,void *pData,const int &nLen);
private:
    ////////////////////////////////////////////////////////////////////////// 网络消息
    //游戏场景消息
    void onGameScene(void *pData, WORD wSize);
    //用户进入
    void OnUserEnter(void* pUser) override;
    //用户状态
    void OnUserStatus(void* pUser) override;
    //用户分数
    void OnUserScore(void* pUser) override;
    
    //游戏消息
    void onGameMessage(WORD wSubCmdID, void *pData, WORD wSize);
    //房间登陆结果
    void roomloginResult(WORD wSubCmdID, void *pData, WORD wSize);
    
    //处理登陆消息
    void onHandleGameLoginMsg(const int &sub, BYTE *pBuffer, const int &size);
    //处理配置消息
    void onHandleGameConfigMsg(const int &sub, BYTE *pBuffer, const int &size);
    
    //处理user消息
    void onHandleUserMsg(const int &sub,BYTE *pBuffer, const int &size);
    void onGetUserStatus(BYTE *pBuffer, const int &size);
    void onGetUserTextChat(void *pBuffer);
    void onGetUserBrowChat(void *pBuffer);
    
    //处理场景消息
    void onHandleFrameMsg(const int &sub, BYTE *pBuffer, const int &size);
    
    /*
     * 处理游戏消息
     */
    void onHandleGameMsg(const int &sub, BYTE *pBuffer,const int &size);
    //接收到游戏空闲
    void onGetGameFree(BYTE *pBuffer,const int &size);
    //接收到游戏开始
    void onGetStartGame(BYTE *pBuffer,const int &size);
    //接收到用户下注
    void onGetUserJetton(BYTE *pBuffer,const int &size);
    //接收到游戏结束
    void onGetGameOver(BYTE *pBuffer, const int &size);
    //接收到申请庄家
    void onGetApplyBanker(BYTE *pBuffer,const int &size);
    //接收到切换庄家
    void onGetChangeBanker(BYTE *pBuffer,const int &size);
    //接收到更新积分
    void onGetChangeScore(BYTE *pBuffer,const int &size);
    //接收到游戏记录
    void onGetGameRecord(BYTE *pBuffer,const int &size);
    //接收到下注失败
    void onGetJettonFail(BYTE *pBuffer,const int &size);
    //接收到取消申请
    void onGetCancelApply(BYTE *pBuffer,const int &size);
    //接收到管理员命令
    void onGetAdminCMD(BYTE *pBuffer,const int &size);
    //接收到更新库存
    void onGetUpdateStorage(BYTE *pBuffer,const int &size);
    //////////////////////////////////////////////////////////////////////////
private:
    //离开游戏
    void notifyLeaveGame(cocos2d::EventCustom *event);
    
    void replaceScene();
    
    void removeNetMessage();
    
    void loadAnimation();
    
    void unloadAnimation();
private:
    GameLayer *m_gameLayer;
    LONGLONG m_llApplyCount;
};

BJL_NAMESPACE_END
#endif /* GameScene_hpp */
