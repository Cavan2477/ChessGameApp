//
//  SettingLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//设置界面

#ifndef __brnn_MyGame__SettingLayer__
#define __brnn_MyGame__SettingLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

namespace brnn_game {
    
class SettingLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(SettingLayer)
    
private:
    
    cocos2d::ui::Slider *m_bgmusic;//背景音乐
    
    cocos2d::ui::Slider *m_effect;//音效
    
    cocos2d::ui::Button *m_sound;//声音开关
};
    
}
#endif /* defined(__MyGame__SettingLayer__) */
