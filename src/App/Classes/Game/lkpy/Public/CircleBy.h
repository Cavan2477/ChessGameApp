#ifndef __CIRCLEBY_H__
#define __CIRCLEBY_H__
#include "cocos2d.h"

USING_NS_CC;

class CircleBy:public ActionInterval
{
public:
	static CircleBy* create(Node* pTarget,float d,CCPoint circleCenter,float radius,float increment,float moveTimes,float add = 0.25f);
	bool init(Node* pTarget,float d,CCPoint circleCenter,float radius,float increment,float moveTimes,float add);
	virtual void update(float time);
    //void startWithTarget(Node *target);

private:
    float  glradian;
	CCPoint m_circleCenter;
	float m_increment;
	float m_radius;
	float m_radian;
	float m_moveTimes;
    float m_add;//
    Node *m_pTarget;
};
	

#endif