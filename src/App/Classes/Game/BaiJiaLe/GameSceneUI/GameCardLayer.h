//
//  GameCardLayer.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/1/16.
//
//

#ifndef bjl_header_GameCardLayer_hpp
#define bjl_header_GameCardLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameCard/CardsNode.h"
#include "LCScrollViewType2.h"

BJL_NAMESPACE_BEGIN

struct tagCardsResult
{
    tagCards m_idleCards;
    tagCards m_masterCards;
    
    tagCardsResult()
    {
        m_idleCards = tagCards();
        m_masterCards = tagCards();
    }
};

struct tagDispathCard
{
    enGameRoleDir m_dir;
    BYTE m_cbCardData;
};
class GameCardLayer:public UILayer
{
public:
    UILAYER_WITHNOTIFY(GameCardLayer);
    
public:
    enum enPointResult
    {
        kPointDefault,
        kDraw,          //平局
        kIdleWin,       //闲赢
        kMasterWin      //庄赢
    };
    void showLayer(bool bShow);
    
    void refresh(const tagCardsResult &tagRes, const bool &bAni, const BYTE &cbTime);
    
    /*
     * @brief 增加扑克
     * @param[cards] 扑克数据
     * @param[dir] 玩家方向( kLeftRole:代表闲;kRightRole:代表庄)
     * @param[pCallBack] 回调
     */
    void addCards(const tagCards &cards, const enGameRoleDir &dir, cocos2d::Action *pCallBack);
    
    /*
     * @brief 刷新结算点数
     * @param[dir] 玩家方向( kLeftRole:代表闲;kRightRole:代表庄)
     */
    void refreshPoint(const enGameRoleDir &dir);
    
    /*
     * @brief 刷新结算结果
     */
    void refreshResult(const enPointResult& enResult);
    
    /*
     * @brief 重置
     */
    void reSet();
private:
    void dispatchUpdate(float dt);
    
    void autoHide(float dt);
    
    void calResult();
    
    void noticeTips();
private:
    //闲、庄胜负
    cocos2d::Sprite *m_spIdleStatus, *m_spMasterStatus;
    //闲、庄点数
    cocos2d::Sprite *m_spPoint[2];
    //平局
    cocos2d::Sprite *m_spDraw;
    //提示
    LCScrollViewType2 *m_tipsView;
    cocos2d::Sprite *m_spBg;
    
    CardsNode *m_nodeCards[2];
    std::vector<tagDispathCard> m_vecDispatchCards;
    enPointResult m_enPointResult;
    
    //发牌总数
    int m_nTotalCount;
    //已发牌数
    int m_nDispatchedCount;
    
    cocos2d::Action *m_actTransition;
    cocos2d::Action *m_actTransitionBack;
};

BJL_NAMESPACE_END
#endif /* GameCardLayer_hpp */
