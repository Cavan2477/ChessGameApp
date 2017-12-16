#include "GameClockNode.h"

USING_NS_CC;
USING_TBNN_NAMESPACE;

GameClockNode::GameClockNode():
	m_spClock(nullptr),
	m_atlasLeft(nullptr),
	m_nLeft(0),
	m_enCountDownTag(tbnn_kDefaultCountDown)
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
        CC_ASSERT(m_spClock != nullptr);
		this->addChild(m_spClock);
        
        m_atlasLeft = cocos2d::ui::TextAtlas::create("", "game/game_timenum.png", 22, 34, "0");
        this->addChild(m_atlasLeft);

		this->stopCountDown();
		bRes = true;
	} while (false);
	return bRes;
}

void GameClockNode::startCountDown(const int &nLeft,const CLOCKNODE_CALLBACK &fun,const en_tbnn_CountDownTag &countTag)
{
	this->setVisible(true);
	m_nLeft = nLeft;
	m_funCallBack = fun;
	m_enCountDownTag = countTag;

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
	m_enCountDownTag = tbnn_kDefaultCountDown;

	if (this->isScheduled(SEL_SCHEDULE(&GameClockNode::countDown)))
	{
		this->unschedule(SEL_SCHEDULE(&GameClockNode::countDown));
	}
	m_atlasLeft->setString("");
}

void GameClockNode::countDown(float dt)
{
	--m_nLeft;
	char buf[64];
	sprintf(buf,"%d",m_nLeft);
	m_atlasLeft->setString(buf);

    //播放倒计时警告音效
    if (5 > m_nLeft)
    {
        INSTANCE(AudioMgr)->playHL_ClockWarnAudio();
    }
    
	if (0 >= m_nLeft)
	{
        if (m_funCallBack)
        {
            m_funCallBack(this,m_enCountDownTag);
            m_funCallBack = nullptr;
        }
		stopCountDown();
	}
}