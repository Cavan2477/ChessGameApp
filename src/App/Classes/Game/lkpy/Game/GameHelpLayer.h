//
//  GameHelpLayer.h
//  MyGame
//
//  Created by wh on 15/6/30.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
LKPY_NAMESPACE_BEGIN
class GameHelpLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    CREATE_FUNC(GameHelpLayer)
    
    void exitCallBack(Ref *pSender, ui::TouchEventType type);
    
    void openFishCallBack(Ref *pSender, ui::TouchEventType type);
    
    void openHelpCallBack(Ref *pSender, ui::TouchEventType type);
    
    void openSupplyCallBack(Ref *pSender, ui::TouchEventType type);
private:
    cocos2d::Sprite *m_currentbg;
    Layout* m_layout;
    ImageView *supply;
    ImageView *fish;
    ImageView *help;
};
LKPY_NAMESPACE_END

