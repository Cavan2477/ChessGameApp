//
//  GameClockNode.cpp
//  HLDouDiZhu
//
//  Created by zhong on 2/24/16.
//
//

#include "GameClockNode.h"
#include "GameClockNode.h"

USING_NS_CC;
USING_BJL_NAMESPACE;

GameClockNode::GameClockNode():
m_spClock(nullptr),
m_atlasLeft(nullptr),
m_nLeft(0),
m_enCountDownTag(bjl_kDefaultCountDown)
{
    
}

GameClockNode::~GameClockNode()
{
    
}

bool GameClockNode::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        
        m_spClock = Sprite::createWithSpriteFrameName("bg_clock.png");
        if (nullptr != m_spClock)
        {
            this->addChild(m_spClock);
        }
        
        
        m_atlasLeft = cocos2d::ui::TextAtlas::create("", "game/game_timenum.png", 18, 24, "0");
        this->addChild(m_atlasLeft);
        
        this->stopCountDown();
        bRes = true;
    } while (false);
    return bRes;
}

void GameClockNode::startCountDown(const int &nLeft,const CLOCKNODE_CALLBACK &fun,const bjl_enCountDownTag &countTag)
{
    this->setVisible(true);
    m_nLeft = nLeft;
    m_funCallBack = fun;
    m_enCountDownTag = countTag;
    char buf[64];
    if (m_nLeft < 10)
    {
        sprintf(buf,"0%d",m_nLeft);
    }
    else
    {
        sprintf(buf,"%d",m_nLeft);
    }
    
    m_atlasLeft->setString(buf);
    
    if (!this->isScheduled(SEL_SCHEDULE(&GameClockNode::countDown)))
    {
        this->schedule(SEL_SCHEDULE(&GameClockNode::countDown),1.0f);
    }
}

void GameClockNode::stopCountDown()
{
    this->setVisible(false);
    m_nLeft = 0;
    m_funCallBack = nullptr;
    m_enCountDownTag = bjl_kDefaultCountDown;
    
    if (this->isScheduled(SEL_SCHEDULE(&GameClockNode::countDown)))
    {
        this->unschedule(SEL_SCHEDULE(&GameClockNode::countDown));
    }
    m_atlasLeft->setString("");
}

void GameClockNode::countDown(float dt)
{
    --m_nLeft;
    char buf[64] = "";
    if (m_nLeft < 10)
    {
        sprintf(buf,"0%d",m_nLeft);
    }
    else
    {
        sprintf(buf,"%d",m_nLeft);
    }
    m_atlasLeft->setString(buf);
    
    if (3 >= m_nLeft && m_funCallBack)
    {
        m_funCallBack(this,m_enCountDownTag);
        m_funCallBack = nullptr;
    }
    if (0 >= m_nLeft )
    {
        stopCountDown();
    }
}