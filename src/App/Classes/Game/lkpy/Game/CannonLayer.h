//
//  CannonLayer.h
//  MyGame
//
//  Created by wh on 15/6/1.
//
//炮台层

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CMD_Stdafx.h"
#include "Cannon.h"
#include "GameLayer.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
LKPY_NAMESPACE_BEGIN
class CannonLayer : public cocos2d::Layer , public UserItemDelegate
                    ,public cocos2d::ui::EditBoxDelegate
{
public:
    CannonLayer();
    
    ~CannonLayer();
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    CREATE_FUNC(CannonLayer)
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    //用户进入
    virtual void OnUserEnter(void* pUser);
    //用户状态
    virtual void OnUserStatus(void* pUser);
    
    virtual void OnUserChat(WORD wSubCode, void *pData, WORD wSize);
    
    void messageChat(void *pData, WORD wSize);
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    Cannon * getCannonbypos(DWORD chair);
    
    cocos2d::Map<DWORD, Cannon *>m_cannonlist;//炮台列表
    
    
    
private:
    bool m_bIsChatOnOff;//是否显示聊天框
    bool m_bIsChatBoxMoving;//聊天框是否正在移动
    Button *m_trumpet;
    Button * m_bt_send;
    //cocos2d::EventListener* _touchListener;
    EditBox* inputField;

};
LKPY_NAMESPACE_END
