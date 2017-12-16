//
//  GameRoleNode.hpp
//  DouDiZhu
//
//  Created by zhong on 1/21/16.
//
//

#ifndef bjl_header_GameRoleNode_hpp
#define bjl_header_GameRoleNode_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "define.h"
#include "GameDefine.h"
#include "HeaderRequest.h"
#include "ClipText.h"

BJL_NAMESPACE_BEGIN

class GameRoleNode:public UINode
{
public:
    GameRoleNode();
    ~GameRoleNode();
    
    CREATE_FUNC(GameRoleNode);
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    
    void reSet(bool bShowSelf = true);
    
    /*
     * @brief 更新角色信息
     * @param[nRoleId] 玩家id
     * @param[szName] 玩家昵称
     * @param[lCoin] 金币
     * @param[bReEnter] 是否断线重连
     * @param[bLeave] 是否离开
     */
    void updateRoleInfo(const DWORD &nRoleId,const char *szName,  SCORE lCoin, bool bReEnter,bool bLeave = false);
    
    /*
     * @brief 更新角色状态
     */
    void updateRoleState(const BYTE &cbState);
    
    /*
     * @brief 切换到庄家角色
     * @param[bBanker] 是否是庄家
     * @param[dir] 玩家方向
     */
    void switchToBanker(bool bBanker,const enGameRoleDir &dir);
    
    /*
     * @brief 切换到摊牌状态
     * @param[bOpenCard] 是否摊牌
     * @param[dir] 玩家方向
     */
    void switchToOpenCard(bool bOpenCard, const enGameRoleDir &dir = kMY_ROLE);
    
    /*
     * @brief 切换到游戏形象
     */
    void switchToGameRole();
    
    /*
     * @brief 用户文本聊天
     * @param[dir] 玩家方向
     * @param[str] 聊天内容
     */
    void textChat(const enGameRoleDir &dir,const char *str);
    
    /*
     * @brief 用户表情聊天
     * @param[dir] 玩家方向
     * @param[idx] 表情索引
     */
    void browChat(const enGameRoleDir &dir, const int &idx);
    
    void changeChatPos(const enGameRoleDir &dir);
    
    std::string getRoleName();
    
    //椅子编号
    CC_SYNTHESIZE(int, m_nChairID,ChairID);
    
    CC_SYNTHESIZE(UILayer*, m_notify, Notify);
private:
    void chatTipUpdater(float dt);
private:
    //背景
    cocos2d::Sprite *m_spHeadBg;
    //头像框
    cocos2d::Sprite *m_spHeadFrame;
    //头像
    HeaderRequest *m_pHead;
    /*//名字
     cocos2d::ui::Text *m_textName;*/
    //
    ClipText *m_clipNickName;
    //元宝
    cocos2d::ui::Text *m_textCoin;
    //庄家
    cocos2d::Sprite *m_spBanker;
    /*
     
     //摊牌
     cocos2d::Sprite *m_spOpenCard;
     */
    
    //聊天
    cocos2d::ui::ImageView *m_spChatBg;
    cocos2d::Label *m_labChat;
    cocos2d::Sprite *m_spBrow;
    
    //
    DWORD m_dwUserId;
    bool m_beginInTouch;
    cocos2d::EventListenerTouchOneByOne *m_listener;
    
    bool m_bConvertBanker;
    bool m_bConvertHead;
private:
    ////////////////////////////////////////////////////////////////////////// 通用动作
    cocos2d::Action *m_actTip;
};

BJL_NAMESPACE_END

#endif /* GameRoleNode_hpp */
