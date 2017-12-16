//
//  DeskLayer.h
//  MyGame
//
//  Created by wh on 15/5/5.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "CMD_Stdafx.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
ZJH_NAMESPACE_BEGIN
class DeskUserData : public cocos2d::Ref
{
public:
    DeskUserData(DWORD userID, WORD tableID, WORD chairID)
    {
        m_userID = userID;
        m_tableID = tableID;
        m_chairID = chairID;
    }
    
    DWORD m_userID;
    WORD m_tableID;
    WORD m_chairID;
};

class DeskLayer : public cocos2d::Layer , public UserItemDelegate
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void encryptinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void pageViewEvent(cocos2d::Ref *pSender, cocos2d::ui::PageView::EventType type);
    
    void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(DeskLayer)
    
    void notifyRefreshScore(cocos2d::EventCustom *event);
    
    void notifyRefreshStatus(cocos2d::Ref *pSender);
    
    void levelchange();
    
    void scorechange();
    
    void userinfoUpdate(float t);
    
    
    void initLayout();
    
    void showLevelInfo(bool show);
    //UserDelegate
    void OnUserEnter(void* pUser);
    void OnUserStatus(void* pUser);
    
    void sendPacketWithUserLevelInfo();
    
    void levelInfo(void *pData, WORD wSize);
    
private:
    cocos2d::Map<DWORD, DeskUserData *> m_UserList;
    
    HeaderRequest * m_head;
    
    Text *m_labelNickName;
    
    TextAtlas *m_levelNum;
    
    LoadingBar *m_ProBar;
    
    TextAtlas *m_goldNum;
    
    TextAtlas *m_beanNum;
    
    Layout* m_layout;
    
    cocos2d::ui::PageView *m_pageView;
    
    int m_pageIndex;//当前第几页
    
    cocos2d::ui::EditBox *m_Encrpytedit;
};
ZJH_NAMESPACE_END