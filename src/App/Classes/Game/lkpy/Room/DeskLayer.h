//
//  DeskLayer.h
//  MyGame
//
//  Created by wh on 15/5/5.
//
//

#ifndef __MyGame__DeskLayer__
#define __MyGame__DeskLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CMD_Stdafx.h"

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
    
    void notifyRefreshStatus(cocos2d::Ref *pSender);
    
    void userinfoUpdate(float t);
    
    //UserDelegate
    void OnUserEnter(void* pUser);
    void OnUserStatus(void* pUser);
    
private:
    cocos2d::Map<DWORD, DeskUserData *> m_UserList;
    
    cocos2d::ui::PageView *m_pageView;
    
    int m_pageIndex;//当前第几页
    
    cocos2d::ui::EditBox *m_Encrpytedit;
};

#endif /* defined(__MyGame__DeskLayer__) */
