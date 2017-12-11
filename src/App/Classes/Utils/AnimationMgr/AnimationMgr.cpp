//
//  AnimationMgr.cpp
//  DouDiZhu
//
//  Created by zhong on 1/21/16.
//
//

#include "AnimationMgr.h"
USING_NS_CC;
AnimationMgr* AnimationMgr::_anInstance = nullptr;
void AnimationMgr::init()
{
    
}

void AnimationMgr::loadAnimationFromFile(const std::string &plist)
{
    AnimationCache::getInstance()->addAnimationsWithFile(plist);
}

void AnimationMgr::loadAnimationFromFrame(const std::string &format,
                                          const int &start,
                                          const int &count,
                                          const std::string &key,
                                          const cocos2d::ui::Widget::TextureResType &texType /*= cocos2d::ui::Widget::TextureResType::LOCAL*/)
{
    Animation *animation = Animation::create();
    if (nullptr == animation)
    {
        return;
    }
    
    int begin = start;
    int end = start + count;
    for (int i = begin; i < end; ++i)
    {
        char buf[256];
        sprintf(buf,format.c_str(),i);
        if (cocos2d::ui::Widget::TextureResType::LOCAL == texType)
        {
            animation->addSpriteFrameWithFile(buf);
        }
        else
        {
            SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(buf);
            if(nullptr != frame)
            {
                animation->addSpriteFrame(frame);
            }
        }
    }
    
    AnimationCache::getInstance()->addAnimation(animation,key);
}

void AnimationMgr::removeCachedAnimation(const std::string &key)
{
    AnimationCache::getInstance()->removeAnimation(key);
}

void AnimationMgr::playAnimation(cocos2d::Node *node, const tagAnimationParam &param, ANIMATION_CALLBACK* fun /*= nullptr*/ )
{
    Animation *animation = AnimationCache::getInstance()->getAnimation(param.m_strName);
    if (nullptr == animation)
    {
        return;
    }
    
    //设置动画参数
    if (param.m_bResetParam)
    {
        animation->setDelayPerUnit(param.m_fDelay);
        animation->setRestoreOriginalFrame(param.m_bRestore);
    }
    
    Animate *act = Animate::create(animation);
    if (nullptr == act)
    {
        return;
    }
    
    if (nullptr != fun)
    {
        node->runAction(Sequence::createWithTwoActions(act,fun));
    }
    else
    {
        node->runAction(act);
    }
}

cocos2d::Animate* AnimationMgr::getAnimate(const tagAnimationParam &param)
{
    Animation *animation = AnimationCache::getInstance()->getAnimation(param.m_strName);
    if (nullptr == animation)
    {
        return nullptr;
    }
    
    //设置动画参数
    if (param.m_bResetParam)
    {
        animation->setDelayPerUnit(param.m_fDelay);
        animation->setRestoreOriginalFrame(param.m_bRestore);
    }
    return Animate::create(animation);
}