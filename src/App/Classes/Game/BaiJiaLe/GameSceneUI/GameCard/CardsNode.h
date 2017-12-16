//
//  CardsNode.hpp
//  DouDiZhu
//
//  Created by zhong on 1/13/16.
//
//

#ifndef bjl_header_CardsNode_hpp
#define bjl_header_CardsNode_hpp

//扑克卡牌组节点
#include "CocosHeader.h"
#include "CardSprite.h"
#include "GameDefine.h"
#include <vector>
#include <map>

BJL_NAMESPACE_BEGIN

class CardsNode;
class SelectCardsListener
{
public:
    SelectCardsListener(){};
    virtual ~SelectCardsListener(){};
    
    virtual void onSelectedCards(const tagCards &cards) = 0;
    virtual void onCardsStateChange(const BYTE &cardData,bool bSelect){}
    //出牌
    virtual void onCardsOut(CardsNode *sender, const tagCards &outCards, const tagCards &handCards){}
};

class CardsNode:public cocos2d::Node,
public CardSpriteDelegate
{
public:
    CardsNode();
    ~CardsNode();
    
    //卡牌曾布局（水平、左竖、右竖（涉及到明牌））
    enum LayerDirection
    {
        kHorizontal,
        kLeftVertical,
        kRightVertical,
    };
    
    enum enMoveDir
    {
        kMoveToLeft,
        kMoveToRight,
    };
    
    enum enFilterFlag
    {
        kHIGHEST,
        kLOWEST,
    };
    
    /*
     * @brief 创建一个扑克牌组节点，负责管理一个玩家的扑克图片
     * @param[dir] 扑克布局（根据玩家方向）
     * @param[cards] 扑克数据
     * @param[isShowCard] 是否明牌
     * @return CardsNode
     */
    static CardsNode* createCardsNode(const enGameRoleDir &dir, const tagCards &cards, bool isShowCard = false);
    
    static CardsNode* createEmptyCardsNode();
    
    void setDir(const enGameRoleDir &dir) { m_layerDir = dir; };
    
    //选牌监听
    CC_SYNTHESIZE(SelectCardsListener*,m_pSelectedListener,SelectCardsListen);
public:
    // 点击事件
    virtual bool onTouchCardBegan(CardSprite *card){ return true; }
    virtual void onTouchCardMoved(CardSprite *card){}
    virtual void onTouchCardEnd(CardSprite *card,bool bDrag){}
    virtual void onTouchCardCanceled(CardSprite *card){}
    
    virtual void onEnter();
    virtual void onExit();
    
    //触摸事件
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchEnd(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event);
public:
    enGameRoleDir getDir() const { return m_layerDir;}
    /*
     * @brief 更新卡牌资源
     * @param[dir] 扑克布局（水平，左竖，右竖）
     * @param[cards] 扑克数据
     * @param[isShowCard] 是否明牌
     * @param[bAnimation] 是否播放动画
     * @param[pCallBack] 发牌动画运行完后执行动作
     */
    void updateCardsNode(const enGameRoleDir &dir,
                         const tagCards &cards,
                         bool isShowCard = false,
                         bool bAnimation = true,
                         cocos2d::Action *pCallBack = nullptr);
    
    /*
     * @brief 更新卡牌数据
     * @param[cards] 扑克数据
     */
    void updateCardsData(const tagCards &cards);
    
    tagCards getSelectCards();
    
    /*
     * @brief 添加扑克
     * @param[cards] 扑克数据
     * @param[isShowCard] 是否明牌
     * @param[pCallBack] 发牌动画运行完后执行动作
     */
    void addCards(const tagCards &cards, bool isShowCard = false, cocos2d::Action *pCallBack = nullptr);
    
    /*
     * @brief 添加扑克数量(用于对方玩家)
     * @param[count] 待添加的数量
     */
    void addCardsCount(const BYTE &count);
    
    /*
     * @brief 控制出牌
     * @param[cards] 扑克数据
     * @param[isShowCard] 是否明牌
     * @param[bSubCards] 是否减少手中牌(针对断线重入)
     * @return 返回出牌的扑克精灵
     */
    std::vector<CardSprite*> outCards(const tagCards &cards, bool isShowCard = false, bool bSubCards = true);
    
    void refreshLeftCards(const enGameRoleDir &dir,const BYTE &left);
    
    /*
     * @brief 重新排列卡牌，对扑克精灵的位置进行调整
     * @param[cards] 扑克数据
     * @param[isShowCard] 是否明牌
     */
    void reSortCards(const tagCards &cards, bool isShowCard = false);
    
    /*
     * @brief 提示：弹出扑克
     * @param[cards] 需要弹出的扑克数据
     */
    void suggestShootCards(const tagCards &cards);
    
    bool getIsShowCards(){ return m_bShowCard; }
    
    //手上的扑克
    tagCards getHandCards(){ return m_cardsData; }
    
    //卡牌动画
    CC_SYNTHESIZE(bool, m_bAnimation, CardsAnimation);
    
    //手上扑克数量（不带扑克数据，用于计算手上剩余扑克数量）
    CC_SYNTHESIZE(BYTE,m_nHandCardCount,HandCardCount);
    
    //是否有实际的扑克数据
    CC_SYNTHESIZE(bool, m_bTrueCardData, TrueCardData);
    
    /*
     * @brief 移除所有卡牌资源
     * @param[showAnimation] 是否使用动画
     */
    void removeAllCards(bool showAnimation = true);
    
    /*
     * @brief 亮牌
     */
    void showCards();
    
    /*
     * @brief 亮牌(牛牛游戏)
     */
    void showCardsNiuNiu();
    
    /*
     * @brief 亮牌(手中剩余牌)
     * @param[cards] 亮牌扑克
     */
    void showLeftCards(const tagCards &cards);
    
    /*
     * @brief 明牌
     * param[bAnimation] 是否开启明牌动画
     */
    void openCards(bool bAnimation);
    
    //获取是否已经有提示牌
    bool getIsSuggested() const { return m_bSuggested; }
    
    //扑克回位(已选择的扑克至为未选择状态)
    void reSetCards();
private:
    void addCardsHolder();
    
    void readyForCards(const BYTE &counts);
    
    /*
     * @brief 排列所有卡牌资源
     * @param[showAnimation] 是否使用动画
     * @param[pCallBack] 发牌动画运行完后执行动作
     */
    void arrangeAllCards(bool showAnimation = true,
                         cocos2d::Action *pCallBack = nullptr);
    
    //切换扑克状态(弹出/回位)
    void switchCardState(CardSprite *card);
    
    //滑动出牌
    void dragCards();
    
    /*
     * @brief 根据扑克叠放层级确定选中的卡牌
     * @param[flag] 过滤方式
     * @param[touchPoint] 触摸点
     * @return 返回有效扑克
     */
    CardSprite* filterCard(const enFilterFlag &flag, const cocos2d::Point& touchPoint);
    
    /*
     * @brief 筛选在触摸拖动区域中的有效扑克
     * @param[touchRect] 触摸拖动区域
     * @return 返回有效扑克
     */
    std::vector<CardSprite*> inTouchAreaCards(const cocos2d::Rect &touchRect);
    
    cocos2d::Rect makeTouchRect(const cocos2d::Vec2 &endTouch);
    
    /*
     * @brief 拖动过程中移除
     */
    
    /*
     * @brief 确定滑动选择的卡牌
     * @param[bFilter] 是否根据叠放层级进行过滤
     * @param[cards] 可选参数，待过滤
     * @return 返回选择的扑克
     */
    std::vector<CardSprite*> filterDragSelectCards(bool bFilter = true,const std::map<BYTE,CardSprite*> &cards = std::map<BYTE,CardSprite*>());
    
    int getLowOrder();
    
    int getHightOrder();
    
    void reOrderCards();
private:
    //卡牌管理
    cocos2d::Node *m_cardsHolder;
    //布局
    enGameRoleDir m_layerDir;
    //卡牌数据
    tagCards m_cardsData;
    //选择的卡牌
    tagCards m_tSelectedCards;
    //是否明牌
    bool m_bShowCard;
    //卡牌资源
    std::map<BYTE,CardSprite*> m_mapCard;
    //卡牌资源
    std::vector<CardSprite*> m_vecCard;
    //卡牌背面
    std::vector<cocos2d::Sprite*> m_vecBack;
    //管理选择的扑克
    std::vector<CardSprite*> m_vecSelectCard;
    //管理选中的扑克
    std::map<BYTE,CardSprite*> m_mapSelectedCards;
    //管理拖动选择的扑克
    std::map<BYTE,CardSprite*> m_mapDragSelectCards;
    //触摸监听
    cocos2d::EventListenerTouchOneByOne *m_listener;
    //是否可以点击
    bool m_bClickable;
    //是否拖拽
    bool m_bDragCard;
    //是否触摸
    bool m_bTouched;
    //开始触摸点
    cocos2d::Vec2 m_beginTouchPoint;
    enMoveDir m_dragMoveDir;
    //选择的卡牌
    CardSprite *m_beginSelectCard, *m_endSelectCard;
    //是否已经提示
    bool m_bSuggested;
    //扑克剩余数量
    cocos2d::Label *m_leftCardsLabel;
    //
    cocos2d::ui::TextAtlas *m_atlasLeftLabel;
    //扑克精灵尺寸
    cocos2d::Size m_cardSize;
    //
    bool m_bAddCards;
    //
    bool m_bDispatching;
private://通用的动画、动作
    //卡牌弹出、卡牌回位
    cocos2d::Sequence *m_actShootCard, *m_actBackCard;
};

BJL_NAMESPACE_END
#endif /* CardsNode_hpp */
