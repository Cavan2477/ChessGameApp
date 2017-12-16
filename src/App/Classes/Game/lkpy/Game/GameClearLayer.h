//
//  GameClearLayer.h
//  MyGame
//
//  Created by wh on 15/6/24.
//
//游戏结算界面


#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
LKPY_NAMESPACE_BEGIN
class GameClearLayer : public cocos2d::Layer,public cocostudio::WidgetCallBackHandlerProtocol
{
  
public:
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    Widget::ccWidgetTouchCallback onLocateTouchCallback(const string &callBackName);
    Widget::ccWidgetClickCallback onLocateClickCallback(const string &callBackName);
    Widget::ccWidgetEventCallback onLocateEventCallback(const string &callBackName);
    
    void onTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onClick(cocos2d::Ref* sender);
    void onEvent(cocos2d::Ref* sender, int eventType);
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void exitCallBack(CCObject *pSender, ui::TouchEventType type);
    
    void backCallBack(CCObject *pSender, ui::TouchEventType type);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    CREATE_FUNC(GameClearLayer)
private:
    std::vector<std::string> _touchTypes;
    std::string _click;
    std::vector<std::string> _eventTypes;
    Layout* m_layout;
    
};
LKPY_NAMESPACE_END
