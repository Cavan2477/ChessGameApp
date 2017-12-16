//
//  TouchSprite.cpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#include "TouchSprite.h"
USING_NS_CC;
TouchSprite::TouchSprite()
{
    m_delegate = nullptr;
}

TouchSprite::~TouchSprite()
{
    m_delegate = nullptr;
}

TouchSprite* TouchSprite::createWith(const std::string &framName)
{
    TouchSprite *pSprite = new TouchSprite();
    if (nullptr != pSprite && pSprite->initWithSpriteFrameName(framName))
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return nullptr;
}

void TouchSprite::onEnter()
{
    Sprite::onEnter();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(false);
    m_listener->onTouchBegan = CC_CALLBACK_2(TouchSprite::onTouchBegan,this);
    m_listener->onTouchMoved = CC_CALLBACK_2(TouchSprite::onTouchMoved,this);
    m_listener->onTouchEnded = CC_CALLBACK_2(TouchSprite::onTouchEnd,this);
    m_listener->onTouchCancelled = CC_CALLBACK_2(TouchSprite::onTouchCancelled,this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener,this);
}

void TouchSprite::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Sprite::onExit();
}

bool TouchSprite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!this->isVisible())
    {
        return false;
    }
    if (m_delegate && !m_delegate->isTouchable(this))
    {
        return false;
    }
    Vec2 localtion = this->convertToNodeSpace(touch->getLocation());
    Size nodeSize = this->getBoundingBox().size;
    Rect nodeRect = Rect(0,0,nodeSize.width,nodeSize.height);
    
    return nodeRect.containsPoint(localtion);
}

void TouchSprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void TouchSprite::onTouchEnd(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Vec2 localtion = this->convertToNodeSpace(touch->getLocation());
    Size nodeSize = this->getBoundingBox().size;
    Rect nodeRect = Rect(0,0,nodeSize.width,nodeSize.height);
    
    if (nodeRect.containsPoint(localtion))
    {
        if (m_delegate)
        {
            m_delegate->onTouchSpriteEnd(this);
        }
    }
}

void TouchSprite::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}
