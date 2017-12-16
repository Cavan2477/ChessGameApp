//
//  GameLodingLayer.h
//  MyGame
//
//  Created by wh on 15/5/22.
//
//导入游戏资源

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN
class GameLodingLayer : public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void onGameScene(void *pData, WORD wSize);
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    virtual void update(float delta);
    
    void loadingCallback(cocos2d::Texture2D *ptexture);
    
    void readFishAniam();
    
    //FramForm  序列帧名字格式
    //SaveName  存到CCAnimationCache的名字
    //FishType  鱼的类型
    void readFishAnimation(const char *FramForm, const char *SaveName , int FishType, int animnum=12, float time = 0.1f );
    
    //FramForm  序列帧名字格式
    //SaveName  存到CCAnimationCache的名字
    //framnum   帧动画数目
    void readAnimation(const char *FramForm, const char *SaveName, int framnum , float time = 0.12f);
    
    //从单张图中读取动画
    //filename 图片名称
    //with 单个序列帧宽度
    //height  单个序列帧高度
    //rownum  列数
    //linenum 行数
    //SaveName 保存名称
    void readAnimationbyfilename(const char *filename , int width, int height,int rownum,int linenum, const char *SaveName,float time = 0.03f);
    
    CREATE_FUNC(GameLodingLayer)
private:
    int m_index;
    
    cocos2d::ProgressTimer  *m_progress;//进度条
};
LKPY_NAMESPACE_END

