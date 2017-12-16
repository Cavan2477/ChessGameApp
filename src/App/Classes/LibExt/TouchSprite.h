//
//  TouchSprite.hpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#ifndef TouchSprite_hpp
#define TouchSprite_hpp

#include <stdio.h>
#include "CocosHeader.h"

class TouchSprite;
class TouchSpriteDelegate
{
public:
    virtual ~TouchSpriteDelegate(){}
    
    virtual bool isTouchable(TouchSprite *sp) = 0;
    virtual void onTouchSpriteEnd(TouchSprite *sp) = 0;
};

class TouchSprite:public cocos2d::Sprite
{
public:
    TouchSprite();
    ~TouchSprite();
    static TouchSprite* createWith(const std::string &framName);
    
    virtual void onEnter();
    virtual void onExit();
    //触摸事件
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchEnd(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event);
    
    CC_SYNTHESIZE(TouchSpriteDelegate*, m_delegate, TouchDelegate);
private:
    cocos2d::EventListenerTouchOneByOne *m_listener;
};
#endif /* TouchSprite_hpp */
