//
//  GameGoldMove.cpp
//  
//
//  Created by wh on 15/11/30.
//
//

#include "GameGoldMove.h"
USING_NS_CC;
//MARK::DROP
GoldDrop *GoldDrop::create(const std::string &filename)
{
    
    auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (!pframe)
    {
        return nullptr;
    }
    GoldDrop *pgold = new (std::nothrow) GoldDrop();
    if (pgold && pgold->initWithSpriteFrameName(filename))
    {
        pgold->autorelease();
        return pgold;
    }
    CC_SAFE_DELETE(pgold);
    return nullptr;
}

void GoldDrop::setBeginVec(const cocos2d::Vec2 &point)
{
    
    this->schedule(CC_CALLBACK_1(GoldDrop::update, this), 0.03f, "drop");
    this->setPosition(Vec2(point.x + std::rand()%(1136 - (int)point.x*2), point.y ));
    m_beginvec2 = this->getPosition();
    this->setScale(((std::rand()%100 + 800)/1000.f));
    this->_gModulus = 1.0;
}

void GoldDrop::update(float t)
{
    
    m_beginvec2.y -= 40*_gModulus*1.9;
    this->setPosition(Vec2(m_beginvec2.x, m_beginvec2.y));
    
    if (m_beginvec2.y <= -30)
    {
        this->unscheduleUpdate();
        this->removeFromParent();
    }
}


void GoldDrop::onExit()
{
    this->unschedule("drop");
    Sprite::onExit();
}


//MARK::MOVE
GameGoldMove *GameGoldMove::create(const std::string &filename)
{
    auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (!pframe)
    {
        return nullptr;
    }
    GameGoldMove *pgold = new (std::nothrow) GameGoldMove();
    if (pgold && pgold->initWithSpriteFrameName(filename))
    {
        pgold->autorelease();
        return pgold;
    }
    CC_SAFE_DELETE(pgold);
    return nullptr;
}

void GameGoldMove::initData(const cocos2d::Vec2 &point)
{
    this->schedule(CC_CALLBACK_1(GameGoldMove::update, this), 0.013f, "update");
    this->setPosition(Vec2(point.x-170+std::rand()%340, point.y+50-std::rand()%200));
    m_beginvec2 = this->getPosition();
    this->setScale(((std::rand()%500 + 500.f)/1000.f));
    m_direction = std::rand()%2;
    m_OffSet = m_direction?10.f:-10.f;
    m_Span = ( std::rand()%150 + 1 ) / 10.f;
    m_Height = ( std::rand()%150 + 100 ) / 100.f;
    m_bhide = false;
    
    auto paction = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("animgold")));
    this->runAction(paction);
}

void GameGoldMove::update(float t)
{
    // 基准比较
    int nBegin = m_direction?10:-10;
    
    // 计算位置
    auto posx = (m_OffSet*m_Span) - (nBegin*m_Span) + m_beginvec2.x;
    auto posy = m_beginvec2.y + (m_Height * nBegin * nBegin) - (m_Height * m_OffSet * m_OffSet);
    this->setPosition(Vec2(posx, posy));
    // 修改偏移
    m_OffSet += m_direction?-0.5f:0.5f;
    
    if (std::abs(m_OffSet) > std::abs(nBegin) && m_bhide == false) {
        m_bhide = true;
        this->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.34f, 0.1f), CallFunc::create([=]{
            this->removeFromParent();
        })));
        this->runAction(FadeTo::create(0.34f, 0));
    }
}

void GameGoldMove::onExit()
{
    this->unschedule("update");
    Sprite::onExit();
}

