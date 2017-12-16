//
//  GameLayer.h
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#pragma once

#define GOLDS_TAG 1000
#define BANNER_TAG 2000

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cmd_game.h"
#include "CocosHeader.h"
#include "Cannon.h"
USING_NS_CC;
using namespace ui;
USING_LKPY_NAMESPACE;
LKPY_NAMESPACE_BEGIN
class GameLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    //GameLayer();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void uiLayerinit();
    
    void menuinit();
    
    void onGameScene(void *pData, WORD wSize);
    
    void multipleinit();
    
    void helplayerinit();
    
    //结算界面
    void clearlayerinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void menutouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void setAutoShoot(bool b);
    void setAutoLock(bool b);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    virtual void update(float delta);
    
    void showCoin(LONGLONG score, DWORD wchairID, cocos2d::Vec2 vec2, BYTE nfishtype);
    
    void showLight(BYTE fishType, DWORD fishIndex);
    
    void showBall(cocos2d::Vec2 &point);
    
    void showLight(cocos2d::Vec2 &point1, cocos2d::Vec2 &point2);
    
    void showTips(const std::string &str);
    void showTips(CMD_S_AwardTip *ptip);
    void showTips(cocos2d::Node *pNode);
    
    //爆炸显示
    void showBomb(Cannon *pcannon);
    
    CREATE_FUNC(GameLayer);
    
    void cannonlayerCallback();
    
    void eventLeaveGame(cocos2d::EventCustom *event);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    
    //分数变化
    void eventScoreChange(cocos2d::EventCustom *event);
    //自动射击变化
    void eventAutoChange(cocos2d::EventCustom *event);
    
    //开火消息
    void messageFire(CMD_S_Fire *fire);
    //捕获鱼消息
    void messageCatchFish(void *pData, WORD wSize);
    //快速移动
    void messageFastMove();
    //切换场景
    void messageExchangeScene(int index);
    //获得宝箱提示
    void messageSupplyTip(DWORD chairID);
    //获得宝箱🉐
    void messageSupply(void *pData, WORD wSize);
    //分数提示
    void messageAwardTip(void *pData, WORD wSize);
    //聊天
    void messageChat(void *pData, WORD wSize);
    //语音消息
    void messageSpeech(void *pData,WORD wSize);
    //激光准备
    void messageBeginLaster(int charID);
    //激光发射
    void messageLaster(CMD_S_Laser *presult);
    //停留鱼
    void messageStayFish(CMD_S_StayFish *presult);
    
    //玩家切换倍数
    void messageMULTIPLE(void *pData, WORD wSize);
    
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void onContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
    void onContactSeperate(cocos2d::PhysicsContact& contact);
    
    //玩家无操作倒计时;
    void secondSchedule(float dt);
    
    void setSecondCount(int count);
    
    void addSecondTip();
    
private:
    bool m_bBannerColor;//红绿切换
    
    EditBox* inputField;
    cocos2d::Layer *m_fishLayer;
    
    cocos2d::Layer *m_uiLayer;
    
    cocos2d::Layer *m_CannonLayer;
    
    int m_nSecondCount;
    //倍数选择层
    cocos2d::ui::Layout *m_multiple;
    
    //菜单选项
    cocos2d::ui::Layout *m_menu;
    
    cocos2d::EventListenerPhysicsContact *m_contact;
    
    cocos2d::Vector<cocos2d::Node *>m_infolist;
    
    
    //bool m_bIsSecondOpenClear;
    
    int m_nGoldBannerNum;//当前炮台显示的金币数目
};

LKPY_NAMESPACE_END
