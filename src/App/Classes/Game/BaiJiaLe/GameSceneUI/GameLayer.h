//
//  GameLayer.hpp
//  DouDiZhu
//
//  Created by zhong on 1/20/16.
//
//

#ifndef bjl_header_GameLayer_hpp
#define bjl_header_GameLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameRoleNode.h"
#include "CMD_Game.h"
#include "GameLogic.h"
#include "GameDataMgr.h"
#include "GameCard/CardsNode.h"
#include "GameClockNode.h"
#include "UserData.h"
#include "ClipText.h"

#include "GameEffectLayer.h"
#include "Chat/GameChatLayer.h"
#include "AntiCheatLayer.h"
#include "GameRoleInfoLayer.h"
#include "GameResultLayer.h"
#include "GameCardLayer.h"
#include "BankerNode.h"
#include "GameJettonNode.h"
#include "Apply/ApplyListLayer.h"
#include "../BankSceneUI/BankLayer.h"
#include "WallBill/SimpleWallBillLayer.h"
#include "WallBill/WallBillLayer.h"

#include "bjl_SettingLayer.h"
BJL_NAMESPACE_BEGIN

class GameLayer:public UILayer,
public SelectCardsListener
{
public:
    GameLayer();
    ~GameLayer();
    
    CREATE_FUNC(GameLayer);
    virtual bool init() override;
    virtual void onSelectedCards(const tagCards &cards) override;
    virtual void onCardsStateChange(const BYTE &cardData,bool bSelect) override;
    virtual void onCardsOut(CardsNode *sender, const tagCards &outCards, const tagCards &handCards) override;
    //ui 线程消息(不能在非ui线程内调用)
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    
    CC_SYNTHESIZE(UIScene*,m_notify,NotifyScene);
#pragma mark -
#pragma mark 网络消息
public:
    //界面重置
    void reSet();
    
    //界面重置，再来一局的情况
    void reSetNewGame();
    
    //清理玩家叫分状态
    void clearAddScoreState();
    
    //
    void onGameStart();
    
    /*
     * @brief 断线重入开始
     */
    void reEnterStart( LONGLONG lUserJetton);
    
    //保存当局游戏玩家
    void saveCurrentGameUser();
    
    /*
     * @brief 游戏空闲状态
     */
    void updateOnGameStateFree();
    
    /*
     * @brief 更新界面用户显示状态
     * @param[pUser] 用户数据
     * @param[dir] 游戏场景相对于自己的方向
     */
    void updateUserStatus( UserData *pUser,const enGameRoleDir &dir);
    
    /*
     * @brief 更新界面 准备 显示状态
     * @param[dir] 游戏场景相对于自己的方向
     * @param[bReady] 是否准备
     */
    void updateReadyDisplay(const enGameRoleDir &dir, bool bReady);
    
    /*
     * @brief 游戏开始
     * @param[bOnGame] 是否开始
     * @param[bValidStart] 是否明牌开始
     */
    void updateOnGame(bool bOnGame, bool bValidStart);
    
    /*
     * @brief 增加上庄玩家
     * @param[tagInfo] 申请信息
     */
    void addApplyBankerUser(const tagApplyInfo &tagInfo);
    
    void removeApplyBankerUser(const WORD &wChair);
    
    /*
     * @brief 更新用户下注
     * @param[cbArea] 下注区域
     * @param[llScore] 下注数额
     * @param[wUser] 下注玩家
     */
    void updateUserJetton(const WORD &wUser, const BYTE &cbArea,  LONGLONG llScore);
    
    /*
     * @brief 断线重连总下注信息
     * @param[cbArea] 下注区域
     * @param[llScore] 下注数额
     */
    void reEnterGameJetton(const BYTE &cbArea,  LONGLONG llScore);
    
    /*
     * @brief 更新用户下注失败
     * @param[cbArea] 下注区域
     * @param[llScore] 下注数额
     * @param[wUser] 下注玩家
     */
    void updateUserJettonFail(const WORD &wUser, const BYTE &cbArea,  LONGLONG llScore);
    
    /*
     * @brief 断线重连玩家下注信息
     * @param[cbArea] 下注区域
     * @param[llScore] 下注数额
     */
    void reEnterUserJetton(const BYTE &cbArea,  LONGLONG llScore);
    
    /*
     * @brief 更新切换庄家
     */
    void updateChangeBanker(const WORD &wChairId,  LONGLONG llScore, const bool &bSystemBanker);
    
    /*
     * @brief 更新庄家积分(游戏币)
     */
    void updateBankerScore();
    
    void updateGameOver();
    
    /*
     * @brief 刷新路单
     */
    void updateWallBill();
    
    /*
     * @brief 更新结算扑克界面
     * @param[tagRes] 扑克数据
     * @param[bAni] 是否需要播放动画
     * @param[cbTime] 剩余时间
     */
    void updateCardsLayer(const tagCardsResult &tagRes, const bool &bAni, const BYTE &cbTime);
    
    void showGameTextChat(const enGameRoleDir &dir, const char *str);
    
    void showGameBrowChat(const enGameRoleDir &dir, const int &idx);
    
    void startCountDown(const BYTE &cbTime, const bjl_enCountDownTag &enTag);
    
    void showGameTip(const GameEffectLayer::emGameTip &tip);
#pragma mark -
public:
    //倒计时
    void countDownCallBack(cocos2d::Node *sender, const bjl_enCountDownTag &tag);
    
    //游戏状态
    CC_SYNTHESIZE(BYTE, m_cbGameStatus, GameStatus);
    
    //游戏倍数
    CC_SYNTHESIZE(BYTE, m_cbGameTimes, GameTimes);
    
    //玩家最大下注值
    CC_SYNTHESIZE(LONGLONG, m_llMaxJetton, MaxJetton);
#pragma mark -
#pragma mark 界面初始化
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    //下注区域
    void touchJettonArea(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    //下注按钮
    void touchJettonBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    //上庄按钮
    void touchApplyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    //初始化顶部菜单
    void initTopMenu();
    //初始化下注区域布局
    void initJettonAreaInfo();
    //重置下注区域
    void reSetJettonAreaInfo();
    //初始化下注信息
    void initJettonInfo();
    //重置下注信息
    void reSetJettonInfo();
    //初始化下注按钮布局
    void initJettonBtnInfo();
    //重置下注按钮布局
    void reSetJettonBtnInfo();
    //初始化托管布局
    void initRobotLayout();
    //重置托管布局
    void reSetRobotLayout();
    //初始化游戏特效界面
    void initGameEffect();
    //重置游戏特效
    void reSetGameEffect();
    //初始化通用动作
    void initAction();
#pragma mark -
private:
    void quit(bool bCheck = true);
    //按钮状态控制
    void enableBtn(cocos2d::ui::Button *btn,bool bEnable, bool bHide = false);
    
    //清理界面扑克资源
    void clearDirCards(const enGameRoleDir &dir);
    
    //游戏托管
    void gameTrusteeship(bool bTrusteeship);
    
    //调整下注按钮状态
    void adjustJettonBtn();
    
    /*
     * @brief 根据下注数额计算对应的下注区域
     * @param[llscore] 下注数额
     * @param[bAllJetton] 是否全下
     * @return 返回计算结果
     */
    std::vector<tagJettonInfo> calcuteJetton( LONGLONG llscore, bool bAllJetton);
    
    int getJettonIdx( LONGLONG llscore);
    
    //下注
    void randomSetJettonPos(cocos2d::Node *nodeArea, cocos2d::Sprite *jettonSp);
    
    //
    void enableJetton(bool bEnable);
    
    //刷新自己已下注数额
    void refreshJetton();
    
    /*
     * @brief 申请上庄
     * @param[bApply] 是否申请
     */
    void applyBanker(const enApplyState &enState);
    
    /*
     * @brief 更新上庄按钮状态
     */
    void updateApplyBtnState(const enApplyState &enState);
    
    /*
     * @brief 显示游戏结果
     */
    void showGameResult(bool bShow);
private:
#pragma mark -
#pragma mark 顶部菜单
    ////////////////////////////////////////////////////////////////////////// 顶部菜单
    cocos2d::ui::Layout *m_topMenuLayout;
    cocos2d::ui::Button *m_btnQuit;
    GameClockNode *m_pClockNode;
    cocos2d::ui::Button *m_btnApply;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 下注区域
    ////////////////////////////////////////////////////////////////////////// 下注区域
    cocos2d::ui::Layout *m_tagJettonLayout;
    GameJettonNode *m_pJettonNode[AREA_MAX];
    ////////////////////////////////////////////////////////////////////////// 下注区域
    
#pragma mark 下注信息
    ////////////////////////////////////////////////////////////////////////// 下注信息
    cocos2d::ui::Layout *m_userJettonLayout;
    ClipText *m_clipJetton;
    LONGLONG m_lHaveJetton; //已下注数额
    ////////////////////////////////////////////////////////////////////////// 下注信息
    
#pragma mark 筹码按钮
    ////////////////////////////////////////////////////////////////////////// 筹码按钮
    cocos2d::ui::Layout *m_JettonBtnLayout;
    LONGLONG m_pJettonNumber[8];
    LONGLONG m_nJettonSelect; //下注数额
    ////////////////////////////////////////////////////////////////////////// 筹码按钮
    
#pragma mark 游戏玩家
    ////////////////////////////////////////////////////////////////////////// 游戏玩家
    HeaderRequest *m_pUserHead;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 申请列表
    ////////////////////////////////////////////////////////////////////////// 申请列表
    ApplyListLayer *m_applyList;
    std::vector<tagApplyInfo> m_vecApplyInfo;
    enApplyState m_enApplyState;
    BankerNode *m_pBankerNode;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 路单
    BJL_NAMESPACE_FUN(SimpleWallBillLayer) *m_simpleWallBill;
    BJL_NAMESPACE_FUN(WallBillLayer) *m_wallBill;
    
#pragma mark 游戏扑克界面
    ////////////////////////////////////////////////////////////////////////// 游戏扑克界面
    GameCardLayer *m_cardLayer;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 托管布局
    ////////////////////////////////////////////////////////////////////////// 游戏托管
    cocos2d::ui::Layout *m_robotLayout;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 游戏特效布局
    ////////////////////////////////////////////////////////////////////////// 游戏特效
    GameEffectLayer *m_gameEffect;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 游戏聊天层
    ////////////////////////////////////////////////////////////////////////// 聊天
    GameChatLayer *m_gameChat;
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 设置界面
    SettingLayer *m_settingLayer;
    
#pragma mark 银行界面
    BankLayer *m_bankLayer;
    
#pragma mark 防作弊
    AntiCheatLayer *m_antiCheat;
    
#pragma mark 玩家信息
    GameRoleInfoLayer *m_roleInfo;
    
#pragma mark 游戏结算
    GameResultLayer *m_gameResultLayer;
private:
    bool m_bOnGame;
    //启用托管
    bool m_bTrusteeship;
    //玩家游戏币
    SCORE m_scoreUser;
    
    ////////////////////////////////////////////百家乐
    /*
     * @brief 用户下注
     * @param[cbArea] 筹码区域
     * @param[lScore] 加注数目
     */
    void userJetton(const BYTE &cbArea, const LONGLONG &lScore);
    
    //申请庄家
    void applyBanker();
    
    //取消申请
    void cancelApply();
    
    /*
     * @brief 更新库存
     */
    void updateStorage();
    ////////////////////////////////////////////百家乐
private:
    ////////////////////////////////////////////////////////////////////////// 通用动作
    cocos2d::Action *m_actCallTip;
    //cocos2d::Action *m_actScoreShow[GAME_PLAYER];
};

BJL_NAMESPACE_END
#endif /* GameLayer_hpp */
