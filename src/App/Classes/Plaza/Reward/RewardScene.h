//
//  Reward.hpp
//  GameProject
//
//  Created by Tang Miao on 1/29/16.
//
//

#ifndef __RewardScene_h__
#define __RewardScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../../Common/MacroDefine.h"
#include "../../Public/GameGoldMove.h"

#define AnimCoin   "coinAnim"
#define WEEK        7

USING_NS_CC;

class RewardScene : public Layer
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    RewardScene();
    virtual ~RewardScene();
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(RewardScene);
    
    bool onTouchBegan(Touch *touch, Event *event);
public:
    void initReward();
    void initBaseen();
    
    
public:
    void buttonEventWithClose(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithSign(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithTakeBaseen(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithShowReward(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithShowBaseen(Ref* target,cocos2d::ui::Widget::TouchEventType type);
public:
    enum RewardType
    {
        type_reward ,
        type_baseen
        
    };
    
    void updateReward(void *data);
    void updateBaseen(void *data);
    
    //查询签到
    void sendCheckinQueryInfo();
    //发送签到
    void sendCheckinDone();
    //发送领取
    void sendEnsureTake();
    
    //签到信息
    void checkininfo(void* pData, WORD wSize);
    
    //签到结果
    void checkinresult(void* pData, WORD wSize);
    
    //低保参数
    void baseensureparamter(void* pData, WORD wSize);
    
    //低保结果
    void baseensureresult(void* pData, WORD wSize);
    
    
private:
    
    DWORD m_wSeriesDate;
    
    SCORE m_lowbase;//低保领取标准
    
private:
    EventListenerTouchOneByOne* _touchListener;
    cocos2d::ui::Layout *_mainLayout;
    
    cocos2d::ui::Layout  *_RewardLayout;
    
    cocos2d::ui::Layout  *_BaseenLayout;
    
    RewardType           m_eRewardType;
    
    
};



#endif /* Reward_hpp */
