//
//  RotateWithAction.h
//  fishdemo
//
//  Created by wh on 13-7-4.
//
//

#pragma once

#include "cocos2d.h"
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN
class RotateWithAction:public cocos2d::BezierTo
{
public:
    
    static RotateWithAction * create(float t, const cocos2d::ccBezierConfig& c);
    
    virtual void update(float time);
    
};
LKPY_NAMESPACE_END

