//
//  RoomLayer.h
//  MyGame
//
//  Created by wh on 15/5/5.
//
//

#ifndef __brnn_MyGame__RoomLayer__
#define __brnn_MyGame__RoomLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Project.h"
#include "CommonPlazaLayer.h"

namespace brnn_game{
    

class RoomLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //房间输入密码弹出框
    void roompassinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    cocos2d::ui::Widget * roominitwithinfo(tagGameServer *server);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    void updateOnline(float t);
    
    //更新在线人数显示
    void updateOnlineShow();
    
    CREATE_FUNC(RoomLayer)
    
    void sendPacketWithUserLevelInfo();
    
    //在线人数查询
    void sendOnline();
    
    /////网络消息模块
    //房间登录结果
    void roomloginResult(WORD  wSubCmdID, void* pData, WORD wSize);
    //在线人数查询
    void roomlistResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    CommonPlazaLayer *m_commonPlaza;
};

}
#endif /* defined(__MyGame__RoomLayer__) */
