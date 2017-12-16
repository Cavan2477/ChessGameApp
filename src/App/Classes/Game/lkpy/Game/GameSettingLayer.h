//
//  GameSettingLayer.h
//  MyGame
//
//  Created by wh on 15/6/19.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN
class GameSettingLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    CREATE_FUNC(GameSettingLayer)

};
LKPY_NAMESPACE_END

