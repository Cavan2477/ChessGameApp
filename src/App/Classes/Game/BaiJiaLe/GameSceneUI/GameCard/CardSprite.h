//
//  CardSprite.hpp
//  DouDiZhu
//
//  Created by zhong on 1/20/16.
//
//

#ifndef bjl_header_CardSprite_hpp
#define bjl_header_CardSprite_hpp

#include <stdio.h>
//扑克卡片
#include "CocosHeader.h"
#include "../../DataMgr/GameLogic.h"

BJL_NAMESPACE_BEGIN

class CardSprite;
class CardSpriteDelegate
{
public:
    CardSpriteDelegate(){};
    virtual ~CardSpriteDelegate(){};
    
    virtual bool onTouchCardBegan(CardSprite *card) = 0;
    virtual void onTouchCardMoved(CardSprite *card) = 0;
    virtual void onTouchCardEnd(CardSprite *card,bool bDrag) = 0;
    virtual void onTouchCardCanceled(CardSprite *card) = 0;
};

class CardSprite:public cocos2d::Sprite
{
public:
    CardSprite();
    ~CardSprite();
    
    /*
     * @brief 卡牌数值颜色,对应于card_num.png的第几行
     */
    enum ValueColor
    {
        kBlackCard = 1,
        kRedCard = 2,
    };
    
    /*
     * @brief 创建卡牌
     * @param[cardData] 卡牌值
     * @param[backfile] 背面图片
     * @return CardSprite
     */
    static CardSprite* createCard(const BYTE &cardData, const char* backfile);
    
    virtual void setLocalZOrder(int localZOrder) override;
    
    //设置、获取卡牌数值
    void setCardValue(const BYTE &value, const ValueColor& valueColor);
    BYTE getCardValue();
    
    //设置、获取卡牌花色
    void setCardColor(const BYTE &color);
    BYTE getCardColor();
    
    //设置、获取卡牌数据（对应 GameLogic中cbCardData）
    void setCardData(const BYTE &data);
    BYTE getCardData();
    
    //显示卡牌被选中的特效
    void showCardSelectedEffect();
    
    //隐藏卡牌被选中的特效
    void hideCardSelectedEffect();
    
    //显示卡牌背面
    void showCardBack(bool var);
    
    //扑克弹起
    CC_SYNTHESIZE(bool,m_bShoot,CardShoot);
    
    //拖动选择
    CC_SYNTHESIZE(bool,m_bDragSelect,CardDragSelect);
    
    //触摸事件代理
    CC_SYNTHESIZE(CardSpriteDelegate*, m_delegate,CardSpriteDelegate);
    
    //是否响应触摸事件
    CC_SYNTHESIZE(bool,m_bClickable,Clickable);
    
    //是否已被发牌
    CC_SYNTHESIZE(bool, m_bDispatched, Dispatched);
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    //触摸事件
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchEnd(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event);
private:
    void updateCard(const ValueColor& valueColor,const BYTE &value = 0, const BYTE &color = 0);
    //创建扑克背面
    void createBack(const char* maskfile);
private:
    //触摸监听
    cocos2d::EventListenerTouchOneByOne *m_listener;
    //卡牌数值精灵、卡牌花色精灵、小花色、背面
    cocos2d::Sprite *m_spValue, *m_spColor, *m_spSmallColor, *m_spBack;
    //卡牌数值、卡牌花色
    BYTE m_cardValue, m_cardColor, m_cardData;
    //数值颜色
    ValueColor m_valueColor;
    //是否拖拽
    bool m_bDragCard;
    //是否触摸
    bool m_bTouched;
    cocos2d::Color3B m_oriColor;
    //
    cocos2d::LayerColor *m_pLayerColor;
    
    std::string m_strCardFile;
};

BJL_NAMESPACE_END
#endif /* CardSprite_hpp */
