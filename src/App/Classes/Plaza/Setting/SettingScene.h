//
//  Setting.hpp
//  GameProject
//
//  Created by Tang Miao on 2/29/16.
//
//

#ifndef __SettingScene_h__
#define __SettingScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../Common/MacroDefine.h"

USING_NS_CC;

typedef enum
{
    EM_SLIDER_SOUND = 1,
    EM_SLIDER_MUSIC = 2,
    EM_QUIET		= 3,
    EM_ACCOUNT		= 4
}ENUM_SETTING;

class SettingScene : public Layer
{
public:
    SettingScene();
    virtual ~SettingScene();
    virtual bool init();

    CREATE_FUNC(SettingScene);

public:
    void onEnterTransitionDidFinish();
    void onExit();
    void sliderCallBack(Ref* ref,cocos2d::ui::Slider::EventType type);
    void buttonEventWithClose(Ref *ref , cocos2d::ui::Widget::TouchEventType type);

private:
    float    _fOptionSound;
    float    _fOptionMusic;
};

#endif /* Setting_hpp */
