//
//  Fish.h
//  MyGame
//
//  Created by wh on 15/5/27.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "lkpy_cmd_game.h"
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN

class Fish : public cocos2d::Sprite
{
public:
    static Fish *create(CMD_S_FishCreate *fishdata);
    
    bool initwithType(CMD_S_FishCreate *fishdata);
    
    void initAnim();
    
    void initphysicsBody();
    
    void onEnter();
    //判断是否可以加入到场景中
    bool isCanAddtoScene();
    
    void update(float t);
    
    void dealproductime();
    
    void deadDeal();
    
    cocos2d::FiniteTimeAction *createPath(int index);
    
    void PathCallback();
    
    cocos2d::Vec2 PointOnCubicBezier(tagBezierPoint *path,DOUBLE t);
    
    cocos2d::Vec2 RotatePoint(cocos2d::Vec2 &pcircle, double dradian, cocos2d::Vec2 &ptsome);
    
    void setconvertpoint(const cocos2d::Vec2 &vec2);
    
    void setFastMove();
    
    CMD_S_FishCreate * getFishData(){
        return &m_data;
    }
    //停留鱼
    void stay(float _time);
    
    void startUpdate(float dt);
    
    CC_SYNTHESIZE(LONGLONG,m_producttime,ProductTime);
private:
    LONGLONG m_ydtime;  //鱼的游动时间
    
    int m_pathIndex;
    
    cocos2d::Vec2 m_oldPoint;
    
    CMD_S_FishCreate m_data;
    
    cocos2d::Sprite *m_light;
    
    cocos2d::Sprite *m_guan;
    
    bool m_isfastmove;
};
LKPY_NAMESPACE_END

