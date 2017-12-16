//
//  RotateWithAction.cpp
//  fishdemo
//
//  Created by wh on 13-7-4.
//
//

#include "RotateWithAction.h"
USING_NS_CC;
USING_LKPY_NAMESPACE;

RotateWithAction * RotateWithAction::create(float t, const ccBezierConfig& c)
{
    RotateWithAction *action = new RotateWithAction();
    if (action && action->initWithDuration(t,c)) {
        action->autorelease();
        return  action;
    }
    CC_SAFE_DELETE(action);
    return  NULL;
}

void RotateWithAction::update(float time)
{
    auto prePos = _target->getPosition();
    BezierTo::update(time);
    auto curPos = _target->getPosition();
    
//    auto angle = prePos.getAngle(curPos);
//    float rotation = angle*180/M_PI;
//    _target->setRotation(rotation);
    
    double len_y = curPos.y - prePos.y;
    double len_x = curPos.x - prePos.x;
    if (len_x == 0.f) {
        if (len_y < 0) {
            _target->setRotation(180);
        }
        return;
    }
    if (len_y == 0) {
        if (len_x > 0) {
            _target->setRotation(90);
            return;
        }
        else if(len_x < 0)
        {
            _target->setRotation(-90);
            return;
        }
    }
    double tan_yx =  fabs(len_y)/fabs(len_x);
    float angle = 0;
    if(len_y > 0 && len_x < 0) {
        angle = atan(tan_yx)*180/M_PI - 90;
    } else if (len_y > 0 && len_x > 0) {
        angle = 90 - atan(tan_yx)*180/M_PI;
    } else if(len_y < 0 && len_x < 0) {
        angle = -atan(tan_yx)*180/M_PI - 90;
    } else if(len_y < 0 && len_x > 0) {
        angle = atan(tan_yx)*180/M_PI + 90;
    }
    _target->setRotation(angle);
}


