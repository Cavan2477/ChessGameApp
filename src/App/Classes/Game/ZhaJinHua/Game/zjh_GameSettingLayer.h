//
//设置界面


#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
ZJH_NAMESPACE_BEGIN
class SettingLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    CREATE_FUNC(SettingLayer)
private:
    ImageView *set_bg;
};
ZJH_NAMESPACE_END
