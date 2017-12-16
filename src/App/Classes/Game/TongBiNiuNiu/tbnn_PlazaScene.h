//
//  tbnn_PlazaScene.hpp
//  GameProject
//
//  Created by zhong on 4/18/16.
//
//

#ifndef tbnn_PlazaScene_hpp
#define tbnn_PlazaScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "TBNNScene.h"
#include "TableMgr.h"
#include "PlazaSceneUI/PlazaLayer.h"

TBNN_NAMESPACE_BEGIN

class PlazaScene: public UIScene
{
    UISCENE_DECLAR(PlazaScene, TBNN_SCENE(PLAZA_SCENE));
    
    //异步预加载资源
    static std::vector<tagUIResourceParam> uiAsynLoadResource();
    
    //资源加载结束
    static std::vector<tagUIResourceParam> uiAsynLoadFinished();
    
    //释放资源
    static std::vector<tagUIResourceParam> uiUnloadResource();
    
    CC_SYNTHESIZE(bool, m_bBackToPlazz, BackToPlazz);
private:
    //桌子消息
    void onTableMsgCallBack(const enTableCode& nCode, void *pData, const int& nSize);
    
    ///////////////////////// 网络
    //等级升级
    void levelUpgrade(void* pData, WORD wSize);
    //等级信息
    void levelInfo(void* pData, WORD wSize);
    //刷新用户金币显示，头像，昵称,等级刷新
    void notifyRefreshScore(cocos2d::EventCustom *event);
    //房间登陆消息
    void roomloginResult(WORD wSubCmdID, void *pData, WORD wSize);
    //房间列表消息
    void roomlistResult(WORD wSubCmdID, void *pData, WORD wSize);
    /////////////////////////
    
private:
    void sendPacketWithUserLevelInfo();
    
    void removeNetMessage();
    
private:
    PlazaLayer *m_plazaLayer;
};

TBNN_NAMESPACE_END
#endif /* tbnn_PlazaScene_hpp */
