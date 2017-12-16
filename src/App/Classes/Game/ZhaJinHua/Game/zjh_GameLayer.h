
#pragma once

#define GOLDS_TAG 1000
#define BANNER_TAG 2000



#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "zjh_cmd_game.h"
#include "protocol.h"
#include "zjh_CardSprite.h"
#include "GameLogic.h"
#include "UserData.h"
#include <vector>
#include <string>
#include "CocosHeader.h"
#include "zjh_GameLogic.h"
USING_NS_CC;
using namespace ui;
ZJH_NAMESPACE_BEGIN
class GameLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate,public UserItemDelegate
{
public:
    //GameLayer();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void uiLayerinit();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void openScondCountByChairId(BYTE wChairId);
    
    void closeScondCount();
    
    //结算界面
    void clearlayerinit();
    
    void playVsAnimation();
    
    void userInfoEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void slideEvent(cocos2d::Ref *pSender, Slider::EventType type);
    
    void SetBetCount(LONGLONG lMaxBetCount, LONGLONG lMinBetCount);
    
    void eventLeaveGame(cocos2d::EventCustom *event);
     
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    virtual void update(float delta);
    
    void onGameScenePlay(CMD_S_StatusPlay* cmd_ssp);
    
    void onGameSceneFree(CMD_S_StatusFree* cmd_ssf);
    
    void onGameSceneStart(CMD_S_GameStart* cmd_sgs);
    
    void onGameSceneAddScore(CMD_S_AddScore* cmd_sas);
    
    void onGameSceneGiveUp(CMD_S_GiveUp *cmd_gu);
    
    void onGameSceneSendCard(CMD_S_SendCard* cmd_ssc);
    
    void onGameSceneGetWinner(CMD_S_OpenCard *cmd_gw);
    
    void onGameSceneCompareCard(CMD_S_CompareCard *cmd);
    
    void onGameSceneLookCard(CMD_S_LookCard *cmd);
    
    void onGameScenePlayerExit(CMD_S_PlayerExit* cmd);
    
    void onGameSceneEnd(CMD_S_GameEnd *cmd_ge);
    
    //发牌
    CardSprite* addCard(WORD _chairId,BYTE bValue,int cardIndex,float delayTime = 0,bool bChangeValue = true,bool bDoNotMove = false,bool bPlayEffect = true,float fScale = 1.0f);//doNotMove 断线重连牌直接到指定位置
    void addChip(Vec2 pos,int chipIndex,LONGLONG lScore = 0);
    void showTips(const std::string &str);
    //void showTips(CMD_S_AwardTip *ptip);
    void showTips(cocos2d::Node *pNode);
    
    CREATE_FUNC(GameLayer);
    
    void cannonlayerCallback();
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    
    void setUserInfo(UserData* pUserData);
    
    void OnUserStatus(void *pUser);
    
    void OnUserEnter(void* pUser);
    
    void OnUserScore(void* pData);
    
    void OnUserChat(WORD wSubCode, void *pData, WORD wSize);
    //分数变化
    void eventScoreChange(cocos2d::EventCustom *event);    
    //捕获鱼消息
    void messageCatchFish(void *pData, WORD wSize);
    //快速移动
    void messageFastMove();
    //切换场景
    void messageExchangeScene(int index);
    //获得宝箱提示
    void messageSupplyTip(DWORD chairID);

    //分数提示
    void messageAwardTip(void *pData, WORD wSize);
    
    void onGameScene(void *pData, WORD wSize);
    
    void networkGRGame(WORD wSubCmdID, void *pData, WORD wSize);
    //聊天
    void messageChat(void *pData, WORD wSize);
    //表情
    void messageBrow(void *pData, WORD wSize);
    //
    Button* getInfoBtnByUserId(DWORD dwUserId);
    //语音消息
    void messageSpeech(void *pData,WORD wSize);

    Button* getInfoBtnByChairId(WORD _chairId);
    
    Layout* getCardLayoutByChairId(WORD _chairId);
    
    Widget* getWidgetByChairId(WORD _chairId,const char *format);
    
    //玩家无操作倒计时;
    void secondSchedule(float dt);
    
    //void setSecondCount(int count);
    
    //void addSecondTip();
    void updateAddButtons();
    
    void presentButtomBg();
    
    void hideButtomBg();
    
    void presentButtomBtn();//显示底部
    
    void hideButtomBtn();
    
    void presentButtomAddBtn();
    
    void hideButtomAddBtn();
    
    void presentStartBtns();
    
    void hideStartBtns();
    
    void selfInfoInit();
    
    void updateOperatorText(WORD _chairId);
    
    
    vector<string> m_recordUserVe;//聊天消息记录
    vector<int> m_recordTypeVe;//聊天消息记录
    vector<string> m_recordContentVe;//聊天消息记录
private:
    int m_nChipCount;//桌上放了几个筹码
    LONGLONG m_lMaxScore;
    LONGLONG m_lMinScore;
    LONGLONG m_lTurnMaxScore;
    LONGLONG m_lTurnLessScore;
    
    LONGLONG m_lMinTurnScore;//最大加注数 方便拉杆加注计算
    LONGLONG m_lMaxTurnScore;//最小加注数 方便拉杆加注计算
    bool m_bOpenUserInfo;//能否点击显示用户详情
    CMD_S_GameStart m_cmd_GameStart;
    
    CGameLogic * m_gameLogic;
    
    Vector<Sprite*> m_chipVe;//桌上押注的钱币图片
    
    ImageView *m_userInfoBg;
    
    ImageView *m_btBg;
    
    TextAtlas *m_slideGoldNum;
    
    Slider* m_slideLine;
    
    LoadingBar* m_slideProBar;
    
    Button* m_slideAddGoldBtn;
    
    Button *m_leaveBtn;
    
    Button *m_setBtn;
    
    Button *m_chatBtn;
    
    LONGLONG m_lCurrentTimes;//当前倍数

    EditBox* inputField;
    cocos2d::Layer *m_fishLayer;
    
    cocos2d::Layer *m_uiLayer;
    
    cocos2d::Layer *m_CannonLayer;
    
    int m_nSecondCount;
    //倍数选择层
    cocos2d::ui::Layout *m_multiple;
    
    //菜单选项
    cocos2d::ui::Layout *m_menu;
    
    cocos2d::Vector<cocos2d::Node *>m_infolist;
    
    
    Layout *m_layout;
    
    Layout *m_slideLayout;
    
    ImageView *m_topBg;
    
    ImageView* m_cardType;
    
    Text *m_currentTotalNum;//当前总押注数目
    
    ImageView *m_buttomBg;
    
    Text* totalBet;
    
    //bool m_bIsSecondOpenClear;
    Button *m_startBtn;
    
    Button *m_changeDeskBtn;
    
    Button *m_optionBtn;
    int m_nGoldBannerNum;//当前炮台显示的金币数目
    
    int m_sendCardCount;//记录发牌次数
    
    bool m_bIsCompare;////如果在比牌不显示底部按钮
    
    bool m_bOpenScondCount;//是否开启倒计时
    
    bool m_bCardTypeMoveing;//牌型面板是否正在移动
    
    ImageView* m_countBg;//倒计时框
    LONGLONG m_llTotalScore;//当前总押注
    
    LONGLONG m_llStartScore;//押注跟随的分数
    
    LONGLONG m_llLastScore;//上一个玩家的押注
    
    LONGLONG m_lUserMaxScore;
    
    bool m_bHasLookCard;//是否已经看牌;
    
    WORD m_winnerChairId;
    
    BYTE m_handCardArray[GAME_PLAYER][3];
    
    BYTE m_giveUpPlayer[GAME_PLAYER];//没参与游戏或者放弃游戏
    
    WORD m_wLastAddUser;//上一个加注玩家
    
    int m_nPlayerNum;//游戏玩家数
    LONGLONG	m_lUserScore[GAME_PLAYER];			//用户下注
    LONGLONG	m_lTableScore[GAME_PLAYER];			//桌面下注
    LONGLONG	m_lTotalScore[GAME_PLAYER];			//桌面下注
    WORD m_wMyChiarId;
    WORD m_wBankUser;
    bool m_bIsGameBegin;//游戏是否已经开始
};
ZJH_NAMESPACE_END