//
//  GamePoolScore.h
//  
//
//  Created by wh on 15/11/30.
//
//

#ifndef __GameGoldMove_h__
#define __GameGoldMove_h__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class GoldDrop : public Sprite
{
public:
    static GoldDrop *create(const std::string& filename);
    
    //初始化位置
    void setBeginVec(const cocos2d::Vec2 &point);
    
    void update(float t);
    
    virtual void onExit();
    
private:
    cocos2d::Vec2 m_beginvec2;

    float _gModulus ;//重力系数
};

class GameGoldMove : public cocos2d::Sprite
{
public:
    static GameGoldMove *create(const std::string& filename);
    
    //初始化数据
    void initData(const cocos2d::Vec2 &point);
    
    void update(float t);
    
    virtual void onExit();
    
private:
    cocos2d::Vec2 m_beginvec2;
    //向左走向右走
    bool m_direction;
    
    float	m_OffSet;				// 偏移
    float   m_Span;					// 跨度
    float	m_Height;				// 高度
    
    bool    m_bhide;				//开始隐藏
};

#endif /* defined(____GamePoolScore__) */
