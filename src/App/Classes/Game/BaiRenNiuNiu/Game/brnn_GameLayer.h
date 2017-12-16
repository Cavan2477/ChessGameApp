//
//  GameLayer.h
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#ifndef __brnn_MyGame__GameLayer__
#define __brnn_MyGame__GameLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Header/cmd_game.h"
#include "CMD_Stdafx.h"
    

class UserData;
namespace brnn_game {
    class CGameLogic;
    
class GameLayer : public cocos2d::Layer, public UserItemDelegate, public cocos2d::extension::TableViewDataSource
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    //点击筹码响应处理
    void scoretouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    //点击下注区域处理
    void areatouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void update(float delta);
    
    //喇叭定时刷新内容
    void hornUpdate(float t);
    
    //发牌动作
    void sendCard();
    void sendCard(int pos);
    //直接发牌，无动画
    void sendCardNoAnim();
    
    void clearCardList();
    
    //产生牌
    cocos2d::Sprite *getCardByValue(BYTE value);
    
    //房间登录结果
    void roomloginResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    //游戏场景消息
    void onGameScene(void *pData, WORD wSize);
    //游戏空闲
    void onGameSceneFree(void *pData, WORD wSize);
    //游戏中
    void onGameScenePlaying(void *pData, WORD wSize);
    
    //游戏消息
    void onGameMessage(WORD wSubCmdID, void *pData, WORD wSize);
    //游戏空闲
    void onGameMessageFree(void *pData, WORD wSize);
    //游戏开始，开始下注
    void onGameMessageStart(void *pData, WORD wSize);
    //游戏结束，开始发牌，亮牌
    void onGameMessageEnd(void *pData, WORD wSize);
    //用户下注
    void onGameMessagePlaceJetton(void *pData, WORD wSize);
    //申请上庄
    void onGameMessageApplyBanker(void *pData, WORD wSize);
    //切换庄家
    void onGameMessageChangeBanker(void *pData, WORD wSize);
    //取消申请上庄
    void onGameMessageCancelBanker(void *pData, WORD wSize);
    //更新积分
    void onGameMessageChangeScore(void *pData, WORD wSize);
    //游戏记录
    void onGameMessageRecord(void *pData, WORD wSize);
    //下注失败
    void onGameMessagePlaceJettonFailure(void *pData, WORD wSize);
    

    //userdelegate
    //用户进入
    void OnUserEnter(void* pUser);
    //用户状态
    void OnUserStatus(void* pUser);
    //用户分数
    void OnUserScore(void* pUser);
    
    //发送上庄
    //发送取消申请
    //发送下庄
    
    CREATE_FUNC(GameLayer);
    //清空游戏数据
    void clearData();
    //清空金币
    void clearGold();
    //随机生成游戏记录
    void produceRecordList();
    //清空记录
    void clearRecord();
    
    //离开游戏
    void notifyLeaveGame(cocos2d::EventCustom *event);
    //自己金币变化
    void notifyScoreChange(cocos2d::EventCustom *event);
    //喇叭消息
    void notifyHorn(cocos2d::EventCustom *event);
    //系统消息
    void notifySysChat(cocos2d::EventCustom *event);
    //切回后台
    void notifyEnterForeground(cocos2d::EventCustom *event);
    //网络断开
    void notifySocketDisconnect(cocos2d::EventCustom *event);
    
    //游戏重连
    void gameConect();
    
    //获取筹码对应金币个数
    int getgoldnumbyScore(LONGLONG score);
    //获取用户数据通过椅子号
    UserData *getUserByChairID(WORD chairID);
    
    //显示函数
    //导入按钮发亮动画
    void loadlightAnim();
    //显示庄家
    void showBanker();
    //更新自己的分数显示
    void updateselfscore();
    //更新加注按钮
    void updateScoreButton(bool enable);
    //超过庄家上限更新加注按钮
    void updateScoreButton(SCORE socre);
    //更新上庄按钮显示
    void updateApplyBankerButton();
    //倒计时显示
    void showCountDown(bool show);
    //区域加注显示
    void updateAreaScore(bool show);
    //区域个人加注显示
    void updateAreaSelfScore(bool show);
    //牛的点数显示
    void showniudot(bool show);
    //显示牌
    void showCard(int pos);
    //根据类型获取图片
    cocos2d::ui::Layout *getspriteByType(const std::string &imagefile, int type);
    //下注状态进入游戏，下注区增加金币显示
    void addgoldtoArea();
    //金币移动到下注区域动画
    void showgoldmove(LONGLONG score, int areaID, bool isself = false);
    //金币移动到庄家
    void showgoldmoveToZ(int pos);
    //金币从庄家移动到区域
    void showgoldmoveToArea();
    //金币从区域移到玩家
    void showgoldmoveToUser(int areaID);
    //游戏成绩显示
    void showGameEndScore();
    //游戏记录显示
    void showGameRecord();
    //显示玩家列表
    void showUserList();
    //显示上庄列表
    void showApplyBankerList();
    //喇叭内容输入框
    void showEditHorn();
    //显示个人信息
    void showUserInfo(UserData *puser);
public:
    virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView *table) {
        return cocos2d::Size(650, 62);
    }
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)
    {
        return m_UserList.size();
    }

private:
    //游戏逻辑
    CGameLogic *m_gamelogic;
    
    cocos2d::Map<int, UserData *> m_UserList;// 用户列表
    
    //牌列表
    cocos2d::Vector<cocos2d::Sprite *>m_cardlist[5];
    
    //金币列表
    cocos2d::Vector<cocos2d::Sprite *>m_goldlist[5];
    
    //喇叭消息列表
    cocos2d::Vector<TrumpetData *> m_hornlist;
    int m_chatindex;
    
    //牌的位置
    cocos2d::Vec2 m_cardVec2[5];
    
    int m_sendIndex;
    
    int m_scoreIndex;//加注按钮选中，－1为不可加注
    
    //0为未申请，也不是庄家
    //1为申请状态
    //2为当庄状态
    int m_ApplyBankerStatus;//申请上庄状态
    
    LONGLONG m_lUserScore;//每局成绩值
    LONGLONG m_lBankerScore;//庄家成绩值
    
    //游戏走势记录
    std::vector<tagServerGameRecord *> m_recordlist;
    
    std::map<int, int> m_bankwaitlist;//申请上庄用户列表
    
    //游戏数据
    //全局下注
    LONGLONG						m_lAllJettonScore[AREA_COUNT+1];		//全体总注
    
    //玩家下注
    LONGLONG						m_lUserJettonScore[AREA_COUNT+1];		//个人总注
    
    //玩家积分
    LONGLONG						m_lUserMaxScore;						//最大下注
    
    //控制信息
    LONGLONG						m_lApplyBankerCondition;				//申请条件
    LONGLONG						m_lAreaLimitScore;                      //区域限制
    
    //扑克信息
    BYTE							m_cbTableCardArray[5][5];				//桌面扑克
    bool                            m_bUserOxCard[5];                       //区域输赢数据
    int                             m_Multiple[4];                          //区域输赢倍数
    
    
    //庄家信息
    WORD							m_wBankerUser;                          //当前庄家
    bool							m_bEnableSysBanker;                     //系统做庄
    
    //全局信息
    int                             m_cbTimeLeave;                          //剩余时间
    BYTE							m_cbGameStatus;                         //游戏状态
    
    SCORE                           m_showscore;                            //显示分数
};

}
#endif /* defined(__MyGame__GameLayer__) */
