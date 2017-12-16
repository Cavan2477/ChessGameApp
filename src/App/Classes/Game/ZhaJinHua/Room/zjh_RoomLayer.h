//
//  RoomLayer.h
//  MyGame
#pragma once
#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "Project.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
ZJH_NAMESPACE_BEGIN
class RoomLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //void initLayout();
    //房间输入密码弹出框
    void roompassinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    cocos2d::ui::Widget * roominitwithinfo(tagGameServer *server);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(RoomLayer)
    
    void sendPacketWithUserLevelInfo();
    
    void levelchange();
    
    //roompass房间密码
    void sendRoomLogin(const std::string &roompass);
    
    void updateNicnameCallBack(Ref* obj);
    /////网络消息模块
    //房间登录结果
    void roomloginResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void levelInfo(void *pData, WORD wSize);
    
    
    
    
    static void loadRes();
    
    static void clearRes();
private:
    HeaderRequest * m_head;
    
    Text *m_labelNickName;
    
    TextAtlas *m_levelNum;
    
    LoadingBar *m_ProBar;
    
    TextAtlas *m_goldNum;
    
    TextAtlas *m_beanNum;
    Layout* m_layout;
};
ZJH_NAMESPACE_END
