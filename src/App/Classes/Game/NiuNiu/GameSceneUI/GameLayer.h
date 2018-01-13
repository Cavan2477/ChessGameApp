/************************************************************************************
 * file: 		GameLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/11 22:41:06
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/11			
 *
 ************************************************************************************/

#ifndef __NN_GAME_LAYER_H__
#define __NN_GAME_LAYER_H__

#include "CocosHeader.h"
#include "../../../DataMgr/GameDataMgr.h"
#include "../../../DataMgr/UserData.h"
#include "../Header/CMD_Game.h"
#include "../DataMgr/GameLogic.h"
#include "../nn_SettingLayer.h"
#include "GameCard/CardsNode.h"
#include "GameRoleNode.h"
#include "NiuNiuNode.h"
#include "CalculateNiuItem.h"
#include "GameClockNode.h"
#include "GameEffectLayer.h"
#include "Chat/GameChatLayer.h"
#include "AntiCheatLayer.h"
#include "GameRoleInfoLayer.h"

NN_NAMESPACE_BEGIN
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
    //ui 线程消息(不能在非ui线程内调用)
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    
	CC_SYNTHESIZE(UIScene*,m_notify,NotifyScene);
public:
	//界面重置
	void Reset();
    
    //界面重置，再来一局的情况
    void ResetNewGame();
    
    //清理界面玩家
    void ClearUser(bool bClear = true);
    
    //是否动态加入
    void SetDynamicJoin(bool bDynamicJoin){ m_bDynamicJoin = bDynamicJoin; }
    
    /*
     * @brief 游戏空闲状态
     */
    void UpdateOnGameStateFree();
    
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
    
    void updateOnGame(bool bOnGame);

	/*
	 * @brief 更新界面叫庄信息
	 * @param[calldir] 游戏场景相对于自己的方向的叫庄用户
	 */
	void updateCallBanker(const enGameRoleDir &calldir);

	/*
	 * @brief 更新界面庄家信息
	 * @parma[dir] 游戏场景相对于自己的方向
	 * @param[maxScore] 最大下注
	 */
	void updateGameBanker(const enGameRoleDir &dir, const LONGLONG &maxScore);
    
    void reEnterShowGameBanker(const enGameRoleDir &dir);

	/*
	 * @brie 更新界面叫分按钮数额
	 * @param[maxScore] 最大数额
	 */
	void updateCallScoreBtn(const LONGLONG &maxScore);

	/*
	 * @brief 更新界面下注结果
	 * @param[dir] 下注方
	 * @param[score] 下注数目
     * @param[gameover] 是否是游戏结算
	 */
	void updateCallScoreResult(const enGameRoleDir &dir,
                               const LONGLONG &score,
                               bool gameover = false);
    
    std::string formatScore(const LONGLONG &score);

	/*
	 * @brief 更新用户扑克数据
	 * @param[dir] 游戏场景相对于自己的方向
	 * @param[cards] 扑克数据
	 */
	void updateUserCards(const enGameRoleDir &dir, const tagCards &cards);
    
    void updateCardsOver();
    
    /*
     * @brief 更新用户摊牌
     * @param[dir] 游戏场景相对于自己的方向
     * @param[bOpenCard] 是否摊牌
     */
    void updateUserOpenCard(const enGameRoleDir &dir, bool bOpenCard);
    
    /*
     * @brief 更新用户牛牛结算
     * @param[dir] 游戏场景相对于自己的方向
     * @param[lScore] 分数
     */
    void updateUserNiuNiuRes(const enGameRoleDir &dir, const LONGLONG &lScore);
    
    /*
     * @brief 更新游戏结束
     * @param
     */
    void updateGameOver();
    
    /*
     * @brief 五花牛信息
     */
    void updateFiveKing(const enGameRoleDir &dir, const bool &bFiveKing);
    
    void showGameTextChat(const enGameRoleDir &dir, const char *str);
    
    void showGameBrowChat(const enGameRoleDir &dir, const int &idx);
    
    //倒计时
    void countDownCallBack(cocos2d::Node *sender, const enCountDownTag &tag);
    
    //游戏状态
    CC_SYNTHESIZE(BYTE, m_cbGameStatus, GameStatus);
private:
	//button 点击回调
	void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//叫分
	void touchCallScore(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//初始化扑克牌
	void initCards();
    //重置扑克
    void reSetCards();
	//初始化顶部菜单
	void initTopMenu();
	//初始化游戏准备阶段布局
	void initPreGameLayout();
    //重置准备布局
    void reSetPreGameLayout();
	//初始化游戏开始布局
	void initStartGameLayout();
    //重置开始布局
    void reSetStartGameLayout();
	//初始化下注布局
	void initCallScoreLayout();
    //重置下注布局
    void reSetCallScoreLayout();
	//初始化游戏进行控制布局
	void initOnGameLayout();
    //重置控制布局
    void reSetOnGameLayout();
    //初始化下注结果布局
    void initAddScoreResLayout();
    //重置下注结果布局
    void reSetAddScoreResLayout();
    //初始化牛牛结算布局
    void initNiuNiuNodeLayout();
    //重置牛牛结算布局
    void reSetNiuNiuNode();
	//初始化玩家信息
	void initUserInfo();
    //重置玩家信息
    void reSetUserInfo(bool bHideUser = true);
    //初始化游戏特效界面
    void initGameEffect();
    //重置游戏特效
    void reSetGameEffect();
	//初始化通用动作
	void initAction();

	void quit(bool bCheck = true);
	//按钮状态控制
	void enableBtn(cocos2d::ui::Button *btn,bool bEnable);
    //切换到准备模式
    void switchToBeReady(bool bReady);
    /*
     * @brief 出牌
     * @param[bTrueCard] 是否使用真实牌
     */
    void outCard(bool bTrueCard);
private:
	CardsNode *m_nodeCards[4]; //扑克牌
	////////////////////////////////////////////////////////////////////////// 顶部菜单
	cocos2d::ui::Layout *m_topMenuLayout;
    cocos2d::ui::Button *m_btnQuit;
	//////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// 游戏准备阶段布局信息
	cocos2d::ui::Layout *m_preGameLayout;
	cocos2d::ui::Button *m_btnReady;
	cocos2d::ui::Button *m_btnChangeDesk;
	//////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// 游戏开始布局
	cocos2d::ui::Layout *m_startGameLayout;
	cocos2d::ui::Button *m_btnCallBanker;
	cocos2d::ui::Button *m_btnNoCall;
	cocos2d::Sprite *m_spReady[4];
    cocos2d::Vec2 m_tipsPos[4]; //下注提示tip位置
    cocos2d::Sprite *m_spCallTip; //下注提示
	//////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// 游戏下注布局
	cocos2d::ui::Layout *m_callScoreLayout;
	cocos2d::ui::Button *m_callBtns[4];
    LONGLONG m_llAddScore[4]; //下注分数
	//////////////////////////////////////////////////////////////////////////

#pragma mark 游戏进行阶段布局
	////////////////////////////////////////////////////////////////////////// 游戏进行阶段布局
	cocos2d::ui::Layout *m_onGameLayout;
	cocos2d::ui::Button *m_btnShowCards;
	cocos2d::ui::Button *m_btnSuggest;
    CalculateNiuItem *m_calculate;
	//////////////////////////////////////////////////////////////////////////
    
#pragma mark 下注结果布局
    ////////////////////////////////////////////////////////////////////////// 游戏下注结果布局
    cocos2d::ui::Layout *m_scoreResLayout;
    cocos2d::Sprite *m_spCallScore[4]; //下注背景
    cocos2d::ui::Text *m_textCallScore[4]; //下注数目
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 牛牛结算布局
    ////////////////////////////////////////////////////////////////////////// 牛牛结算布局
    cocos2d::ui::Layout *m_niuniuLayout;
    NiuNiuNode *m_niuniu[4];
    cocos2d::ui::TextAtlas *m_atlasScore[4];
    //////////////////////////////////////////////////////////////////////////
    
#pragma mark 游戏玩家
	////////////////////////////////////////////////////////////////////////// 游戏玩家
	GameRoleNode* m_nodeGameRole[4];
	cocos2d::Vec2 m_pos[4];
    GameClockNode *m_clock;
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
    
#pragma mark 防作弊
    AntiCheatLayer *m_antiCheat;
    
#pragma mark 玩家信息
    GameRoleInfoLayer *m_roleInfo;
    
private:
    ////////////////////////////////////////////////////////////////////////// 牛牛
    //用户叫庄
    void userCallBanker(const BYTE &btBanker);
    //用户加注
    void userAddScore(const LONGLONG &lAddScore);
    //用户摊牌
    void userOpenCard(const BYTE &bOpen);
    //////////////////////////////////////////////////////////////////////////
private:
    bool m_bOnGame;
    //动态加入
    bool m_bDynamicJoin;
    //点击了提示按钮
    bool m_bSuggestBtn;
    //游戏玩家数量
    int m_nPlayerCount;
private:
	////////////////////////////////////////////////////////////////////////// 通用动作
	cocos2d::Action *m_actCallTip;
    cocos2d::Action *m_actScoreShow[4];
};

NN_NAMESPACE_END
#endif