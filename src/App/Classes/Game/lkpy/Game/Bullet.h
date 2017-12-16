//
//  Bullet.h
//  MyGame
//
//  Created by wh on 15/6/2.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "define.h"
#include "Cannon.h"
USING_NS_CC;
LKPY_NAMESPACE_BEGIN
enum BulletType
{
    Normal_Bullet = 0,//正常炮
    Bignet_Bullet,//网变大
    Special_Bullet,//加速炮
};

class Bullet : public cocos2d::Sprite
{
public:
    static Bullet *create(float angle,WORD _wChiarId,int _score,CannonType _cannonType = Normal_Cannon);
    
    bool initwithangle(float angle,WORD _wChiarId,int _score,CannonType _cannonType);
    
    void update(float t);
    
    virtual void onEnter();
    
    void nomalupdate(float t);
    void followFish(float t);
    
    void fallingnet();
    
    void changeDisplayFrame(WORD _wChiarId,int _score);
    
    static int getBulletNum(int _score);//根据分数获取子弹个数
    
    Rect getNetRect(Point centerPos,Point* offsetArray,int len,Size contentSize);//获取3个网子的矩形区域
    
    void sendCatchFish(Rect rect);
    //子弹类型
    CC_SYNTHESIZE_READONLY(BulletType, m_Type, Type)
    void setType(int type);
    //锁定鱼
    CC_SYNTHESIZE_READONLY(DWORD, m_fishindex, FishIndex)
    void setFishIndex(DWORD fishindex);
    
    //索引
    CC_SYNTHESIZE(DWORD, m_index, Index)
    
    //炮台位置ID
    CC_SYNTHESIZE(DWORD , m_pos , Pos)
    
    CC_SYNTHESIZE(Bullet *, m_bbullet, Bbullet)
    
    CC_SYNTHESIZE(bool , m_isturn, IsTurn)
    
    CC_SYNTHESIZE(bool , m_isself, IsSelf)
    
    CC_SYNTHESIZE(cocos2d::Vec2 , m_movedir, MoveDir)
    
    CC_SYNTHESIZE(cocos2d::Color3B , m_netcolor, NetColor)
    
    CC_SYNTHESIZE_READONLY(cocos2d::Vec2 , m_targetpoint, TargetPoint)
    void setTargetPoint(cocos2d::Vec2 target);
    
private:
    
    float m_speed;//移动速度
    int m_nScore;//子弹的分数
};

LKPY_NAMESPACE_END
