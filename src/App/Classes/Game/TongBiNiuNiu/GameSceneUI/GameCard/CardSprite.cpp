//
//  CardSprite.cpp
//  DouDiZhu
//
//  Created by zhong on 1/20/16.
//
//

#include "CardSprite.h"
USING_NS_CC;
USING_TBNN_NAMESPACE;

const int CARD_WIDTH = 145;
const int CARD_HEIGHT = 161;

//背面zorder
const int BACK_Z_ORDER				= 2;
CardSprite::CardSprite():
m_spValue(nullptr),
m_spColor(nullptr),
m_spSmallColor(nullptr),
m_spBack(nullptr),
m_cardValue(0),
m_cardColor(0),
m_cardData(0),
m_bShoot(false),
m_delegate(nullptr),
m_listener(nullptr),
m_bTouched(false),
m_bDragCard(false),
m_bClickable(true),
m_bDragSelect(false),
m_bDispatched(false)
{
    m_strCardFile = "game/card.png";
    /*
     //卡牌数值纹理
     Director::getInstance()->getTextureCache()->addImage(CARD_COLOR_FILE);
     //卡牌花色纹理
     Director::getInstance()->getTextureCache()->addImage(CARD_NUM_FILE);
     */
}

CardSprite::~CardSprite()
{
    m_delegate = nullptr;
    this->removeAllChildrenWithCleanup(true);
}

CardSprite* CardSprite::createCard(const BYTE &cardData, const char* backfile)
{
    CardSprite* sp = new CardSprite();
    auto tex = Director::getInstance()->getTextureCache()->getTextureForKey("game/card.png");
    if (nullptr != sp && sp->initWithTexture(tex))
    {
        sp->autorelease();
        BYTE value = INSTANCE(CGameLogic)->GetCardValue(cardData);
        BYTE color = (CGameLogic::getInstance()->GetCardColor(cardData) >> 4) + 1;
        //TODO color的byte值
        ValueColor valueColor = color & 0x01 ? kRedCard : kBlackCard;
        
        sp->setCardData(cardData);
        sp->updateCard(valueColor,value,color);
        sp->createBack(backfile);
        
        Size content = sp->getContentSize();
        sp->m_pLayerColor = LayerColor::create(Color4B::BLACK, content.width, content.height);
        sp->m_pLayerColor->setPosition(Vec2(0, 0));
        sp->m_pLayerColor->setOpacity(125);
        sp->addChild(sp->m_pLayerColor);
        
        sp->hideCardSelectedEffect();
        sp->m_oriColor = sp->getColor();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

void CardSprite::setCardValue(const BYTE &value, const ValueColor& valueColor)
{
    BYTE cardValue = INSTANCE(CGameLogic)->GetCardValue(value);
    BYTE cardColor = (CGameLogic::getInstance()->GetCardColor(value) >> 4) + 1;
    
    this->setCardData(value);
    this->updateCard(valueColor,cardValue,cardColor);
}

BYTE CardSprite::getCardValue()
{
    return m_cardValue;
}

void CardSprite::setCardColor(const BYTE &color)
{
    BYTE tmp = (color >> 4) + 1;
    this->updateCard(m_valueColor,m_cardValue,tmp);
}

BYTE CardSprite::getCardColor()
{
    //与 GameLogic getCardColor的逆运算一致
    return (m_cardColor - 1) << 4;
}

void CardSprite::setCardData(const BYTE &data)
{
    m_cardData = data;
}

BYTE CardSprite::getCardData()
{
    return m_cardData;
}

void CardSprite::showCardSelectedEffect()
{
    /*
     this->setColor(Color3B::BLACK);
     this->setOpacity(125);
     */
    m_pLayerColor->setVisible(true);
}

void CardSprite::hideCardSelectedEffect()
{
    /*
     this->setColor(m_oriColor);
     this->setOpacity(255);
     */
    m_pLayerColor->setVisible(false);
}

void CardSprite::showCardBack(bool var)
{
    if (nullptr != m_spBack)
    {
        m_spBack->setVisible(var);
    }
}

bool CardSprite::getIsShowCardBack()
{
    if (nullptr != m_spBack)
    {
        return m_spBack->isVisible();
    }
    return false;
}

void CardSprite::onEnter()
{
    Sprite::onEnter();
}

void CardSprite::onExit()
{
    Sprite::onExit();
}

void CardSprite::updateCard(const ValueColor& valueColor,const BYTE &value /* = 0 */, const BYTE &color /* = 0 */)
{
    m_valueColor = valueColor;
    this->setTag(m_cardData);
    
    if ( 0 != m_cardData)
    {
        //纹理区域
        Rect rect = Rect((value - 1) * CARD_WIDTH,((int)color - 1) * CARD_HEIGHT,CARD_WIDTH,CARD_HEIGHT);
        if (0x4F == m_cardData)
        {
            rect = Rect(0,4 * CARD_HEIGHT,CARD_WIDTH,CARD_HEIGHT);
        }
        else if (0X4E == m_cardData)
        {
            rect = Rect(CARD_WIDTH,4 * CARD_HEIGHT,CARD_WIDTH,CARD_HEIGHT);
        }
        
        this->setTextureRect(rect);
    }
    else
    {
        //使用背面纹理区域
        Rect rect = Rect(2 * CARD_WIDTH,4 * CARD_HEIGHT,CARD_WIDTH,CARD_HEIGHT);
        this->setTextureRect(rect);
    }
}

void CardSprite::createBack(const char* maskfile)
{
    if (nullptr == maskfile)
    {
        return;
    }
    auto tex = Director::getInstance()->getTextureCache()->getTextureForKey(m_strCardFile);
    //纹理区域
    Rect rect = Rect(2 * CARD_WIDTH,4 * CARD_HEIGHT,CARD_WIDTH,CARD_HEIGHT);
    
    Size cardSize = this->getContentSize();
    m_spBack = Sprite::createWithTexture(tex, rect);
    m_spBack->setPosition(cardSize.width * 0.5, cardSize.height * 0.5);
    m_spBack->setVisible(false);
    this->addChild(m_spBack,BACK_Z_ORDER);
}