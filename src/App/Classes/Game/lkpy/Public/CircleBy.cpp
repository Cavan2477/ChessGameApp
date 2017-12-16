#include "CircleBy.h"
#include "cocos2d.h"
USING_NS_CC;
CircleBy* CircleBy::create(Node* pTarget,float d,CCPoint circleCenter,float radius,float increment,float moveTimes,float add)
{
	CircleBy* pcircle=new CircleBy();
    pcircle->m_pTarget = pTarget;
	pcircle->init(pTarget,d,circleCenter,radius,increment,moveTimes,add);
	pcircle->autorelease();
	return pcircle;
}

bool CircleBy::init(Node* pTarget,float d,CCPoint circleCenter,float radius,float increment,float moveTimes,float add)
{
	if(CCActionInterval::initWithDuration(d))
	{
        m_pTarget = pTarget;
		glradian=0;
		m_circleCenter=circleCenter;
		m_increment=increment;
		m_radius=radius;
		m_radian=2.0*M_PI/(0.3f/(CCDirector::sharedDirector()->getAnimationInterval()));
		m_moveTimes=moveTimes;
		glradian=m_moveTimes*0.1f;
        m_add = add;
		return true;
	}
	return false;
}

//void CircleBy::startWithTarget(Node *target)
//{
//    CCActionInterval::startWithTarget(m_pTarget);
//}

void CircleBy::update(float time)
{
	float radian=m_radian*m_moveTimes;
	float x=m_radius*sin(radian);
	float y=m_radius*cos(radian);
	CCPoint pos=ccp(x+m_circleCenter.x,y+m_circleCenter.y);
    
	//m_pTarget->runAction(CCRotateTo::create(2.0f,360.0f));
	//m_pTarget->setRotation(glradian);
	if(m_increment==1)
	glradian+=0.1f;
	else if(m_increment==-1)
	{
		glradian-=0.1f;
	}
	//m_pTarget->runAction(CCRotateBy::create(1.0/60,1));
	m_pTarget->setPosition(pos);
	m_moveTimes+=m_increment*m_add;
	if(glradian>=360.0f)
	{
		//CCLOG("right gui yi");
		m_moveTimes=1;
		glradian=1;
	}
	if(glradian<=-360.0f)
	{
		//CCLOG("left gui yi");
		m_moveTimes=-1;
		glradian=-1;
	}
}