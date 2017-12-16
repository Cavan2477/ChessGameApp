//
//  GameLayer.hpp
//  GameProject
//
//  Created by zhong on 4/19/16.
//
//

#ifndef tbnn_header_GameLayer_hpp
#define tbnn_header_GameLayer_hpp

#include "CocosHeader.h"
#include "GameDefine.h"
#include "tbnn_SettingLayer.h"
#include "GameCard/CardsNode.h"
#include "GameRoleNode.h"
#include "GameClockNode.h"
#include "GameEffectLayer.h"
#include "Chat/GameChatLayer.h"
#include "GameRoleInfoLayer.h"

TBNN_NAMESPACE_BEGIN
class GameLayer:public UILayer,
public SelectCardsListener
{
    UILAYER_DECLAR(GameLayer);
    
    virtual void onSelectedCards(const tagCards &cards) override;
    virtual void onCardsStateChange(const BYTE &cardData,bool bSelect) override;
    //ui 线程消息(不能在非ui线程内调用)
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    
public:
    //游戏状态
    CC_SYNTHESIZE(BYTE, m_cbGameStatus, GameStatus);
    
    CC_SYNTHESIZE(bool, m_bDynamicJoin, DynamicJoin);
    
    //界面重置
    void reSet();
    
    //界面重置，再来一局的情况
    void reSetNewGame();
    
    //清理界面玩家
    void clearUser(bool bClear = true);
    
    //倒计时
    void countDownCallBack(cocos2d::Node *sender, const en_tbnn_CountDownTag &tag);
public:
    //MARK::网络消息
    /*
     * @brief 游戏空闲状态
     */
    void updateOnGameStateFree();
    
    /*
     * @brief 更新界面用户显示状态
     * @param[pUser] 用户数据
     * @param[wChair] 椅子编号
     */
    void updateUserStatus( UserData *pUser,const WORD &wChair);
    
    void updateOnGame(bool bOnGame);
    
    void updateGameBanker(const WORD &wBanker);
    
    /*
     * @brief 更新用户扑克数据
     * @param[wChair] 椅子编号
     * @param[cards] 扑克数据
     */
    void updateUserCards(const WORD &wChair, const tagCards &cards);
    
    void updateUserCardsData(const WORD &wChair, const tagCards &cards);
    
    void reEnterUpdateUserCards(const WORD &wChair, const tagCards &cards);
    
    void updateCardsOver();
    
    /*
     * @brief 用户摊牌
     */
    void updateUserOpenCard(const WORD &wChair);
    
    /*
     * @brief 用户分数
     */
    void updateUserScore(const WORD &wChair, LONGLONG llscore);
    
    void updateGameOver();
    
    /*
     * @brief 五花牛信息
     */
    void updateFiveKing(const WORD &wChair, const bool &bFiveKing);
    
    void showGameTextChat(const WORD &wChair, const char *str);
    
    void showGameBrowChat(const WORD &wChair, const int &idx);
    
    void openCard(const BYTE& cbOx, const tagCards &cards);
    //MARK::网络消息
private:
    void quit(bool bCheck = true);
    
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
    
    //初始化通用按钮
    void initBtnLayout();
    
    //初始化玩家信息
    void initUserInfo();
    void reSetUserInfo();
    
    //初始化游戏扑克
    void initCardsInfo();
    void reSetCardsInfo();
    
    //游戏控制
    void initGameControLayout();
    void reSetGameControlLayout();
    
    //游戏特效
    void initGameEffect();
    void reSetGameEffect();
    
    void gameTrusteeship(bool bTrusteeship);
private:
    //MARK::通用按钮
    /////////////////////////////////////////////////////////////
    cocos2d::ui::Button *m_btnReady;
    /////////////////////////////////////////////////////////////
    
    //MARK::游戏玩家
    /////////////////////////////////////////////////////////////
    GameRoleNode *m_nodeGameRole[tbnn_GAME_PLAYER];
    cocos2d::Vec2 m_pos[tbnn_GAME_PLAYER];
    cocos2d::Sprite *m_spReady[tbnn_GAME_PLAYER];
    GameClockNode *m_clock;
    /////////////////////////////////////////////////////////////
    
    //MARK::游戏扑克
    /////////////////////////////////////////////////////////////
    CardsNode *m_nodeCards[tbnn_GAME_PLAYER];
    /////////////////////////////////////////////////////////////
    
    //MARK::游戏控制
    /////////////////////////////////////////////////////////////
    cocos2d::ui::Layout *m_gameControlLayout;
    cocos2d::ui::Button *m_btnSdcp;             //手动搓牌按钮
    cocos2d::ui::Button *m_btnZdcp;             //自动搓牌按钮
    cocos2d::ui::Button *m_btnTp;               //摊牌按钮
    cocos2d::ui::Button *m_btnTiShi;            //提示按钮
    /////////////////////////////////////////////////////////////
    
    //MARK::游戏托管
    cocos2d::ui::Layout *m_gameTru;
    
    //MARK::游戏特效
    GameEffectLayer *m_gameEffect;
    
    //MARK::设置
    SettingLayer *m_settingLayer;
    
    //MARK::聊天
    GameChatLayer *m_gameChat;
    
    //MARK::用户信息
    GameRoleInfoLayer *m_roleInfo;
private:
    bool m_bAutoShowCards;
    
    bool m_bGameTru; //托管
    
    bool m_bMyOpenCards; //轮到摊牌
};
TBNN_NAMESPACE_END
#endif /* GameLayer_hpp */
