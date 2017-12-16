//
//  CardsNode.cpp
//  DouDiZhu
//
//  Created by zhong on 1/13/16.
//
//

#include "CardsNode.h"
USING_NS_CC;
#define ANI_BEGIN (0.1f)
#define ANI_RATE(i) (i * 0.1f)
using namespace std;
USING_BJL_NAMESPACE;

//卡牌背面
const char* CARD_BACK_FILE	= "card_back.png";
//卡牌弹出动画时间
const float CARD_SHOOT_TIME = 0.2f;
//卡牌回位动画时间
const float CARD_BACK_TIME	= 0.2f;
//卡牌弹出距离
const float CARD_SHOOT_DIS	= 20.0f;
//卡牌X轴坐标
const float CARD_X_POS      = 0.0f;
//卡牌Y轴坐标
const float CARD_Y_POS		= 0.0f;

//未知卡牌尺寸
const float CARD_HIDE_SCALE = 0.4f;

//自己卡牌尺寸
const float CARD_SHOW_SCALE = 0.75f;

//最低叠放层级
const int MIN_DRAW_ORDER = 0;
//最高叠放层级
const int MAX_DRAW_ORDER = 54;

const int BJL_MAX_COUNT = MAX_DRAW_ORDER;

const int BJL_CARD_X_POS = 40.0f;

const int BJL_CARD_Y_POS = 25.0f;

//排序函数：根据卡牌显示层级
bool sortByDrawOrder(const CardSprite *a,const CardSprite *b)
{
    return a->getLocalZOrder() < b->getLocalZOrder();
}

//排序函数：根据卡牌数据
bool sortByCardData(CardSprite *a,CardSprite *b)
{
    return a->getCardData() < b->getCardData();
}

CardsNode::CardsNode():
m_cardsHolder(nullptr),
m_actShootCard(nullptr),
m_actBackCard(nullptr),
m_bSuggested(false),
m_leftCardsLabel(nullptr),
m_bClickable(false),
m_bAnimation(false),
m_pSelectedListener(nullptr),
m_nHandCardCount(17),
m_beginSelectCard(nullptr),
m_endSelectCard(nullptr),
m_bTrueCardData(true),
m_atlasLeftLabel(nullptr),
m_bShowCard(false),
m_bDispatching(false)
{
    m_bTouched = false;
    removeAllCards();
    
    //TODO 卡牌弹出后的回调动作（如确定按钮、逻辑验证）
    auto moveBy = MoveBy::create(CARD_SHOOT_TIME,Vec2(0,CARD_SHOOT_DIS));
    auto call = CallFunc::create([this](){});
    m_actShootCard = Sequence::create(moveBy,call,nullptr);
    CC_SAFE_RETAIN(m_actShootCard);
    
    moveBy = moveBy->reverse();
    call = CallFunc::create([this](){});
    m_actBackCard = Sequence::create(moveBy,call,nullptr);
    CC_SAFE_RETAIN(m_actBackCard);
}

CardsNode::~CardsNode()
{
    m_pSelectedListener = nullptr;
    
    removeAllCards();
    this->removeAllChildrenWithCleanup(true);
    
    //资源释放
    CC_SAFE_RELEASE(m_actShootCard);
    CC_SAFE_RELEASE(m_actBackCard);
}

CardsNode* CardsNode::createCardsNode(const enGameRoleDir &dir, const tagCards &cards, bool isShowCard /*= false*/)
{
    CardsNode* cn = new CardsNode();
    if (nullptr != cn && cn->init())
    {
        cn->autorelease();
        cn->addCardsHolder();
        cn->updateCardsNode(dir,cards,isShowCard);
        cn->m_bClickable = dir == kMY_ROLE;
        return cn;
    }
    CC_SAFE_DELETE(cn);
    return nullptr;
}

CardsNode* CardsNode::createEmptyCardsNode()
{
    CardsNode* cn = new CardsNode();
    if (nullptr != cn && cn->init())
    {
        cn->autorelease();
        cn->addCardsHolder();
        return cn;
    }
    CC_SAFE_DELETE(cn);
    return nullptr;
}

void CardsNode::onEnter()
{
    Node::onEnter();
    
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(false);
    m_listener->onTouchBegan = CC_CALLBACK_2(CardsNode::onTouchBegan,this);
    m_listener->onTouchMoved = CC_CALLBACK_2(CardsNode::onTouchMoved,this);
    m_listener->onTouchEnded = CC_CALLBACK_2(CardsNode::onTouchEnd,this);
    m_listener->onTouchCancelled = CC_CALLBACK_2(CardsNode::onTouchCancelled,this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener,this);
}

void CardsNode::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Node::onExit();
}

bool CardsNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
{
    if (!isVisible())
    {
        return false;
    }
    if (!m_bClickable)
    {
        return false;
    }
    if (m_bDispatching)
    {
        return false;
    }
    
    m_beginSelectCard = nullptr;
    m_endSelectCard = nullptr;
    m_bDragCard = false;
    m_beginTouchPoint = this->convertToNodeSpace(touch->getLocation());
    /*
     Point locationInNode = this->convertToNodeSpace(touch->getLocation());
     Size s = this->getContentSize();
     Rect rect = Rect(0, 0, s.width, s.height);
     m_bTouched = rect.containsPoint(locationInNode); //判断触摸点是否在目标的范围内
     */
    m_beginSelectCard = this->filterCard(kHIGHEST,touch->getLocation());
    if (nullptr != m_beginSelectCard)
    {
        m_beginSelectCard->showCardSelectedEffect();
        m_mapSelectedCards.insert(make_pair(m_beginSelectCard->getCardData(),m_beginSelectCard));
    }
    m_bTouched = m_beginSelectCard != nullptr;
    
    return true;
}

void CardsNode::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event)
{
    if (m_bTouched)
    {
        m_endSelectCard = this->filterCard(kHIGHEST,touch->getLocation());
        m_bDragCard = true;
        Rect touchRect = makeTouchRect(this->convertToNodeSpace(touch->getLocation()));
        
        //筛选在触摸区域内的卡牌
        vector<CardSprite*> inTouchCards = inTouchAreaCards(touchRect);
        
        //过滤有效卡牌，选择叠放最高的
        if (inTouchCards.empty())
        {
            return;
        }
        
        //CardSprite* tmp = filterCard(kHIGHEST,touch->getLocation());
        if (nullptr != m_endSelectCard)
        {
            if ( !m_endSelectCard->getCardDragSelect() )
            {
                m_endSelectCard->showCardSelectedEffect();
                m_endSelectCard->setCardDragSelect(true);
                if (nullptr != m_beginSelectCard &&
                    m_beginSelectCard->getCardData() != m_endSelectCard->getCardData())
                {
                    m_mapDragSelectCards.insert(make_pair(m_endSelectCard->getCardData(),m_endSelectCard));
                }
            }
        }
        
        //剔除不在触摸区域内，但已选择的卡牌
        map<BYTE,CardSprite*> tmpMap;
        for (size_t i = 0; i < inTouchCards.size(); ++i)
        {
            CardSprite* tmp = inTouchCards[i];
            tmpMap.insert(make_pair(tmp->getCardData(),tmp));
        }
        map<BYTE,CardSprite*>::iterator ite = m_mapDragSelectCards.begin();
        for (; ite != m_mapDragSelectCards.end();)
        {
            map<BYTE,CardSprite*>::iterator tmpIte = tmpMap.find(ite->first);
            if (tmpIte != tmpMap.end())
            {
                ++ite;
            }
            else
            {
                ite->second->setCardDragSelect(false);
                ite->second->hideCardSelectedEffect();
                ite = m_mapDragSelectCards.erase(ite);
            }
        }
    }
}

void CardsNode::onTouchEnd(cocos2d::Touch *touch, cocos2d::Event * event)
{
    if (m_bTouched)
    {
        m_endSelectCard = this->filterCard(kHIGHEST,touch->getLocation());
        if (!m_bDragCard)
        {
            if (nullptr != m_endSelectCard /*&& !m_endSelectCard->getCardDragSelect()*/)
            {
                m_endSelectCard->setCardDragSelect(true);
                
                if (nullptr != m_beginSelectCard &&
                    m_beginSelectCard->getCardData() != m_endSelectCard->getCardData())
                {
                    m_mapSelectedCards.insert(make_pair(m_endSelectCard->getCardData(),m_endSelectCard));
                }
            }
            //选牌音效
            INSTANCE(AudioMgr)->playSelectCardAudio();
        }
        
        if (nullptr != m_beginSelectCard)
        {
            m_beginSelectCard->hideCardSelectedEffect();
        }
    }
    m_vecSelectCard = filterDragSelectCards(m_bTouched);
    dragCards();
    if (m_bSuggested)
    {
        m_bSuggested = !m_mapSelectedCards.empty();
    }
    m_beginSelectCard = nullptr;
    m_endSelectCard = nullptr;
    m_bDragCard = false;
    m_bTouched = false;
}

void CardsNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event)
{
    CCLOG("cancel");
}

void CardsNode::updateCardsNode(const enGameRoleDir &dir,
                                const tagCards &cards,
                                bool isShowCard /*= false*/,
                                bool bAnimation /*= true*/,
                                cocos2d::Action *pCallBack /*= nullptr*/)
{
    if (0 == cards.m_cardCount)
    {
        return;
    }
    m_bAddCards = false;
    m_bDispatching = true;
    
    m_bClickable = false;
    this->removeAllCards();
    this->readyForCards(cards.m_cardCount);
    
    m_cardsData = cards;
    m_layerDir = dir;
    m_bShowCard = isShowCard;
    
    //转换为相对于自己的中间位置
    Size winSize = INSTANCE(Director)->getWinSize();
    Vec2 centerPos = Vec2(winSize.width * 0.5f, winSize.height * 0.55);
    centerPos = this->convertToNodeSpace(centerPos);
    Vec2 toPos = centerPos;
    if(kLEFT_ROLE == m_layerDir)
    {
        toPos = this->convertToNodeSpace(Vec2(winSize.width * 0.2f, winSize.height * 0.55));
    }
    else if (kRIGHT_ROLE == m_layerDir)
    {
        toPos = this->convertToNodeSpace(Vec2(winSize.width * 0.8f, winSize.height * 0.55));
    }
    
    if ((kRIGHT_ROLE == m_layerDir || kLEFT_ROLE == m_layerDir || kMY_ROLE == m_layerDir) && bAnimation)
    {
        float scale = m_layerDir == kBANKER_ROLE ? 1.0f : 0.6f;
        int mapKey = 0;
        for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
        {
            CardSprite *tmpSp = CardSprite::createCard(m_cardsData.m_cardData[i],CARD_BACK_FILE);
            tmpSp->setPosition(centerPos);
            tmpSp->setDispatched(false);
            tmpSp->setCardSpriteDelegate(this);
            tmpSp->showCardBack(true);
            tmpSp->setScale(scale);
            m_cardsHolder->addChild(tmpSp);
            if (0 == m_cardsData.m_cardData[i])
            {
                mapKey = i;
            }
            else
            {
                mapKey = m_cardsData.m_cardData[i];
            }
            m_mapCard.insert(make_pair(mapKey,tmpSp));
            m_vecCard.push_back(tmpSp);
            
            auto moveTo = MoveTo::create(0.3f + i / 16.0f, toPos);
            auto backTo = MoveTo::create(0.3f, centerPos);
            Sequence *seq = nullptr;
            if (m_cardsData.m_cardCount -1 == i)
            {
                seq = Sequence::create(moveTo, backTo, CallFunc::create([this,bAnimation,cards,pCallBack]()
                                                                        {
                                                                            if (!m_mapCard.empty())
                                                                            {
                                                                                m_cardSize = m_mapCard.begin()->second->getContentSize();
                                                                            }
                                                                            
                                                                            this->arrangeAllCards(bAnimation,pCallBack);
                                                                            
                                                                            //计算手中剩余卡牌
                                                                            m_nHandCardCount = cards.m_cardCount;
                                                                            this->refreshLeftCards(m_layerDir, cards.m_cardCount);
                                                                        }),NULL);
            }
            else
            {
                seq = Sequence::create(moveTo, backTo, NULL);
            }
            tmpSp->stopAllActions();
            tmpSp->runAction(seq);
        }
    }
    else
    {
        float scale = m_layerDir == kBANKER_ROLE ? 1.0f : 0.6f;
        int mapKey = 0;
        for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
        {
            CardSprite *tmpSp = CardSprite::createCard(m_cardsData.m_cardData[i],CARD_BACK_FILE);
            tmpSp->setPosition(centerPos);
            tmpSp->setDispatched(false);
            tmpSp->setCardSpriteDelegate(this);
            tmpSp->showCardBack(true);
            tmpSp->setScale(scale);
            m_cardsHolder->addChild(tmpSp);
            if (0 == m_cardsData.m_cardData[i])
            {
                mapKey = i;
            }
            else
            {
                mapKey = m_cardsData.m_cardData[i];
            }
            m_mapCard.insert(make_pair(mapKey,tmpSp));
            m_vecCard.push_back(tmpSp);
            
        }
        
        if (!m_mapCard.empty())
        {
            m_cardSize = m_mapCard.begin()->second->getContentSize();
        }
        
        this->arrangeAllCards(bAnimation,pCallBack);
        
        //计算手中剩余卡牌
        m_nHandCardCount = cards.m_cardCount;
        this->refreshLeftCards(m_layerDir, cards.m_cardCount);
    }
}

void CardsNode::updateCardsData(const tagCards &cards)
{
    m_cardsData = cards;
    //排序
    //INSTANCE(CGameLogic)->SortCardList(m_cardsData.m_cardData,m_cardsData.m_cardCount,ST_ORDER);
    
    Vector<Node*> children = m_cardsHolder->getChildren();
    m_mapCard.clear();
    
    for (ssize_t i = 0; i < children.size(); ++i)
    {
        CardSprite* tmp = static_cast<CardSprite*>(children.at(i));
        if (nullptr == tmp)
        {
            continue;
        }
        BYTE color = (CGameLogic::getInstance()->GetCardColor(m_cardsData.m_cardData[i]) >> 4) + 1;
        CardSprite::ValueColor valueColor = color & 0x01 ? CardSprite::ValueColor::kRedCard : CardSprite::ValueColor::kBlackCard;
        tmp->setCardValue(m_cardsData.m_cardData[i], valueColor);
        m_mapCard.insert(make_pair(m_cardsData.m_cardData[i], tmp));
    }
}

tagCards CardsNode::getSelectCards()
{
    /*
    tagCards tmp = tagCards();
    
    tmp.m_cardCount = m_mapSelectedCards.size();
    int i = 0;
    map<BYTE,CardSprite*>::iterator ite = m_mapSelectedCards.begin();
    for (; ite != m_mapSelectedCards.end(); ++ite)
    {
        tmp.m_cardData[i] = ite->second->getCardData();
        ++i;
    }
    
    //排序
    INSTANCE(CGameLogic)->SortCardList(tmp.m_cardData, tmp.m_cardCount, ST_ORDER);
     */
    //清除已选择的卡牌
    //m_mapSelectedCards.clear();
    
    return m_tSelectedCards;
}

void CardsNode::addCards(const tagCards &cards, bool isShowCard /*= false*/, cocos2d::Action *pCallBack /*= nullptr*/)
{
    if(cards.m_cardCount + m_cardsData.m_cardCount > BJL_MAX_COUNT) return;
    m_bAddCards = false; //针对百家乐修改
    
    BYTE total = cards.m_cardCount + m_cardsData.m_cardCount;
    if (total > BJL_MAX_COUNT) return;
    
    bool bShowBack = m_layerDir != kMY_ROLE;
    //转换为相对于自己的中间位置
    //百家乐，顶部位置
    Size winSize = INSTANCE(Director)->getWinSize();
    Vec2 centerPos = Vec2(winSize.width * 0.5f, winSize.height);
    centerPos = this->convertToNodeSpace(centerPos);
    BYTE begin = m_cardsData.m_cardCount != 0 ? m_cardsData.m_cardCount : 0;
    BYTE index = 0;
    CardSprite *tmpSp = nullptr;
    for (BYTE i = begin; i < total; ++i)
    {
        if(m_bTrueCardData)
        {
            tmpSp = CardSprite::createCard(cards.m_cardData[index],CARD_BACK_FILE);
            m_cardsData.m_cardData[i] = cards.m_cardData[index];
            ++index;
            m_mapCard.insert(make_pair(m_cardsData.m_cardData[i],tmpSp));
        }
        else
        {
            tmpSp = CardSprite::createCard(0,CARD_BACK_FILE);
            m_mapCard.insert(make_pair(i,tmpSp));
        }
        m_vecCard.push_back(tmpSp);
        tmpSp->setDispatched(false);
        tmpSp->setCardSpriteDelegate(this);
        tmpSp->setPosition(centerPos);
        tmpSp->showCardBack(true);
        tmpSp->setLocalZOrder(i);
        m_cardsHolder->addChild(tmpSp);
    }
    m_cardsData.m_cardCount = total;
    m_bShowCard = isShowCard;
    m_nHandCardCount = total;
    
    //排序
    //INSTANCE(CGameLogic)->SortCardList(m_cardsData.m_cardData,m_cardsData.m_cardCount,ST_ORDER);
    //this->reSortCards(m_cardsData,isShowCard);
    this->arrangeAllCards(true, pCallBack);
    
    //计算手中剩余卡牌
    m_nHandCardCount = total;
    this->refreshLeftCards(m_layerDir, total);
}

void CardsNode::addCardsCount(const BYTE &count)
{
    BYTE total = count + m_cardsData.m_cardCount;
    if (total > BJL_MAX_COUNT) return;
    m_cardsData.m_cardCount = total;
    
    //计算手中剩余卡牌
    this->refreshLeftCards(m_layerDir, m_cardsData.m_cardCount);
}

std::vector<CardSprite*> CardsNode::outCards(const tagCards &oCards,
                                             bool isShowCard /*= false*/,
                                             bool bSubCards /*= true*/)
{
    m_bShowCard = isShowCard;
    tagCards cards = oCards;
    
    std::vector<CardSprite*> vecOut = std::vector<CardSprite*>();
    
    if (!bSubCards)
    {
        for (BYTE i = 0; i < cards.m_cardCount; ++i)
        {
            CardSprite *tmpSp = CardSprite::createCard(cards.m_cardData[i],CARD_BACK_FILE);
            tmpSp->setPosition(BJL_CARD_X_POS,CARD_Y_POS);
            tmpSp->showCardBack(true);
            m_cardsHolder->addChild(tmpSp);
            vecOut.push_back(tmpSp);
        }
    }
    else
    {
        if (0 != m_cardsData.m_cardCount && m_bTrueCardData)
        {
            //清理扑克数据
            tagCards tmpData = m_cardsData;
            BYTE counts = 0;
            
            for (BYTE i = 0; i < cards.m_cardCount; ++i)
            {
                BYTE tag = cards.m_cardData[i];
                log("-------------------------出牌:%d",(int)INSTANCE(CGameLogic)->GetCardValue(tag));
                CardSprite* tmp = static_cast<CardSprite*>(m_cardsHolder->getChildByTag(tag));
                if (nullptr != tmp)
                {
                    vecOut.push_back(tmp);
                }
                
                map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(tag);
                if (ite != m_mapCard.end())
                {
                    m_mapCard.erase(ite);
                }
                
                for (BYTE j = 0; j < m_cardsData.m_cardCount; ++j)
                {
                    if (tag == m_cardsData.m_cardData[j])
                    {
                        tmpData.m_cardData[j] = 0;
                    }
                }
            }
            m_cardsData = tagCards();
            for (BYTE i = 0; i < tmpData.m_cardCount; ++i)
            {
                if (tmpData.m_cardData[i] != 0)
                {
                    m_cardsData.m_cardData[counts] = tmpData.m_cardData[i];
                    ++counts;
                }
            }
            
            m_cardsData.m_cardCount = counts;
            this->reSortCards(m_cardsData,isShowCard);
        }
        else
        {
            Vector<Node*> children = m_cardsHolder->getChildren();
            if (!children.empty())
            {
                int childrenSize = (int)children.size();
                int outCount = cards.m_cardCount < childrenSize ? cards.m_cardCount : childrenSize;
                
                tagCards tmpData = m_cardsData;
                BYTE counts = 0;
                for (BYTE i = 0; i < outCount; ++i)
                {
                    BYTE tag = cards.m_cardData[i];
                    log("**********************出牌:%d",(int)INSTANCE(CGameLogic)->GetCardValue(tag));
                    CardSprite* tmp = static_cast<CardSprite*>(children.at(i));
                    if (nullptr != tmp)
                    {
                        if (tmp->getTag() != tag)
                        {
                            BYTE color = (CGameLogic::getInstance()->GetCardColor(tag) >> 4) + 1;
                            CardSprite::ValueColor valueColor = color & 0x01 ? CardSprite::ValueColor::kRedCard : CardSprite::ValueColor::kBlackCard;
                            tmp->setCardValue(tag, valueColor);
                        }
                        vecOut.push_back(tmp);
                    }
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(tag);
                    if (ite != m_mapCard.end())
                    {
                        m_mapCard.erase(ite);
                    }
                    
                    //清理扑克数据
                    for (BYTE j = 0; j < m_cardsData.m_cardCount; ++j)
                    {
                        if (tag == m_cardsData.m_cardData[j])
                        {
                            tmpData.m_cardData[j] = 0;
                        }
                    }
                }
                
                m_cardsData = tagCards();
                for (BYTE i = 0; i < tmpData.m_cardCount; ++i)
                {
                    if (tmpData.m_cardData[i] != 0)
                    {
                        m_cardsData.m_cardData[counts] = tmpData.m_cardData[i];
                        ++counts;
                    }
                }
                m_cardsData.m_cardCount = counts;
            }
            else
            {
                for (BYTE i = 0; i < cards.m_cardCount; ++i)
                {
                    CardSprite *tmpSp = CardSprite::createCard(cards.m_cardData[i],CARD_BACK_FILE);
                    tmpSp->setPosition(BJL_CARD_X_POS,CARD_Y_POS);
                    tmpSp->showCardBack(true);
                    m_cardsHolder->addChild(tmpSp);
                    vecOut.push_back(tmpSp);
                }
            }
        }
        
        //计算手上剩余卡牌
        m_nHandCardCount -= cards.m_cardCount;
        m_cardsData.m_cardCount = m_nHandCardCount;
        this->refreshLeftCards(m_layerDir,m_nHandCardCount);
    }
    
    //出牌通知
    if (nullptr != m_pSelectedListener)
    {
        m_pSelectedListener->onCardsOut(this,cards, m_cardsData);
    }
    
    //清除选中
    std::map<BYTE,CardSprite*> tmpMap = std::map<BYTE,CardSprite*>();
    tmpMap.insert(m_mapSelectedCards.begin(),m_mapSelectedCards.end());
    tmpMap.insert(m_mapDragSelectCards.begin(),m_mapDragSelectCards.end());
    for (std::map<BYTE,CardSprite*>::iterator ite = tmpMap.begin(); ite != tmpMap.end(); ++ite)
    {
        ite->second->hideCardSelectedEffect();
        ite->second->setCardDragSelect(false);
        ite->second->setPositionY(CARD_Y_POS);
    }
    m_mapSelectedCards.clear();
    m_mapDragSelectCards.clear();
    m_tSelectedCards = tagCards();
    m_bSuggested = false;
    return vecOut;
}

void CardsNode::refreshLeftCards(const enGameRoleDir &dir,const BYTE &left)
{
    //更新剩余扑克数量
    /*
    if (nullptr == m_leftCardsLabel)
    {
        m_leftCardsLabel = Label::createWithSystemFont("","Arial",40);
        m_leftCardsLabel->setVisible(false);
        this->addChild(m_leftCardsLabel);
        
        if (dir == kLEFT_ROLE)
        {
            m_leftCardsLabel->setVisible(true);
            //m_leftCardsLabel->setAnchorPoint(Vec2(1,0.0f));
            //m_leftCardsLabel->setPosition(m_cardSize.width * 0.5 , 0);
        }
        else if (dir == kRIGHT_ROLE)
        {
            m_leftCardsLabel->setVisible(true);
            //m_leftCardsLabel->setAnchorPoint(Vec2(0,0.0f));
            //m_leftCardsLabel->setPosition(m_cardSize.width * 0.5 , 0);
        }
    }
     */
    if (nullptr == m_atlasLeftLabel)
    {
        m_atlasLeftLabel = ui::TextAtlas::create("", "game/game_cardnum.png", 22, 34, "0");
        m_atlasLeftLabel->setVisible(false);
        this->addChild(m_atlasLeftLabel);
        if (dir == kLEFT_ROLE)
        {
            m_atlasLeftLabel->setVisible(true);
            //m_atlasLeftLabel->setAnchorPoint(Vec2(1,0.0f));
            //m_atlasLeftLabel->setPosition(m_cardSize.width * 0.5 , 0);
        }
        else if (dir == kRIGHT_ROLE)
        {
            m_atlasLeftLabel->setVisible(true);
            //m_atlasLeftLabel->setAnchorPoint(Vec2(0,0.0f));
            //m_atlasLeftLabel->setPosition(m_cardSize.width * 0.5 , 0);
        }
    }
    
    char buf[64];
    sprintf(buf,"%d",left);
    m_atlasLeftLabel->setString(buf);
}

void CardsNode::reSortCards(const tagCards &cards, bool isShowCard /* = false */)
{
    for (BYTE i = 0; i < cards.m_cardCount; ++i)
    {
        BYTE cardData = cards.m_cardData[i];
        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
        if (ite == m_mapCard.end())
        {
            CardSprite *tmpSp = CardSprite::createCard(cardData,CARD_BACK_FILE);
            tmpSp->setCardSpriteDelegate(this);
            tmpSp->showCardBack(true);
            tmpSp->setLocalZOrder(i);
            m_cardsHolder->addChild(tmpSp);
            m_mapCard.insert(make_pair(m_cardsData.m_cardData[i],tmpSp));
        }
    }
    
    //扑克布局
    switch (m_layerDir)
    {
        case kMY_ROLE:      //自己牌
        {
            //针对百家乐的最多三张
            float center = m_cardsData.m_cardCount * 0.5f;
            if ( 1 == m_cardsData.m_cardCount)
            {
                center = 0;
            }
            else if ( 2 == m_cardsData.m_cardCount)
            {
                center = 0.5;
            }
            else if (3 == m_cardsData.m_cardCount)
            {
                center = 1.0f;
            }
            
            for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
            {
                /*
                BYTE cardData = m_cardsData.m_cardData[i];
                map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                if (ite == m_mapCard.end())
                {
                    continue;
                }
                 */
                CardSprite *tmp = m_vecCard[i]; //针对百家乐多幅牌
                tmp->setScale(CARD_SHOW_SCALE);
                tmp->stopAllActions();
                tmp->setDispatched(true);
                tmp->hideCardSelectedEffect();
                
                tmp->showCardBack(false);
                Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS, CARD_Y_POS);
                tmp->setPosition(pos);
                if (i == m_cardsData.m_cardCount - 1)
                {
                    m_bDispatching = false;
                }
            }
        }
            break;
        case kLEFT_ROLE:	//左边牌
        {
            //niuniu
            //float center = 2.0f;
            //明牌
            if (isShowCard)
            {
                int center = 0;
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE mapKey = m_cardsData.m_cardData[i];
                    mapKey = mapKey != 0 ? mapKey : i;
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->setScale(CARD_HIDE_SCALE);
                    tmp->showCardBack(false);
                    tmp->setDispatched(true);
                    tmp->hideCardSelectedEffect();
                    tmp->stopAllActions();
                    Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
                    tmp->setPosition(pos);
                    if (i == m_cardsData.m_cardCount - 1)
                    {
                        m_bDispatching = false;
                    }
                }
            }
            else //卡牌叠放，显示剩余卡牌数值
            {
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE mapKey = m_cardsData.m_cardData[i];
                    mapKey = mapKey != 0 ? mapKey : i;
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->setScale(CARD_HIDE_SCALE);
                    tmp->showCardBack(true);
                    tmp->setDispatched(true);
                    tmp->hideCardSelectedEffect();
                    tmp->stopAllActions();
                    tmp->setPosition(CARD_X_POS,CARD_Y_POS);
                    if (i == m_cardsData.m_cardCount - 1)
                    {
                        m_bDispatching = false;
                    }
                }
            }
        }
            break;
        case kRIGHT_ROLE:       //右边牌
        {
            if (isShowCard)
            {
                int center = m_cardsData.m_cardCount;
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE mapKey = m_cardsData.m_cardData[i];
                    mapKey = mapKey != 0 ? mapKey : i;
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->setScale(CARD_HIDE_SCALE);
                    tmp->showCardBack(false);
                    tmp->setDispatched(true);
                    tmp->hideCardSelectedEffect();
                    tmp->stopAllActions();
                    Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
                    tmp->setPosition(pos);
                    if (i == m_cardsData.m_cardCount - 1)
                    {
                        m_bDispatching = false;
                    }
                }
            }
            else //卡牌叠放，显示剩余卡牌数值
            {
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE mapKey = m_cardsData.m_cardData[i];
                    mapKey = mapKey != 0 ? mapKey : i;
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->setScale(CARD_HIDE_SCALE);
                    tmp->showCardBack(true);
                    tmp->setDispatched(true);
                    tmp->hideCardSelectedEffect();
                    tmp->stopAllActions();
                    tmp->setPosition(CARD_X_POS,CARD_Y_POS);
                    if (i == m_cardsData.m_cardCount - 1)
                    {
                        m_bDispatching = false;
                    }
                }
            }
        }
            break;
        case kTOP_ROLE:     //顶部牌
        {
            //float center = (m_cardsData.m_cardCount * 0.5);
            //niuniu
            //float center = 2.0f;
            if (isShowCard)
            {
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE mapKey = m_cardsData.m_cardData[i];
                    mapKey = mapKey != 0 ? mapKey : i;
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->showCardBack(false);
                    tmp->stopAllActions();
                    tmp->hideCardSelectedEffect();
                    Vec2 pos = Vec2(0, -i * BJL_CARD_Y_POS);
                    tmp->setPosition(pos);
                }
            }
            else //卡牌叠放，显示剩余卡牌数值
            {
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE mapKey = m_cardsData.m_cardData[i];
                    mapKey = mapKey != 0 ? mapKey : i;
                    
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->showCardBack(true);
                    tmp->setPosition(CARD_X_POS,CARD_Y_POS);
                    tmp->hideCardSelectedEffect();
                }
            }
        }
            break;
        case enGameRoleDir::kBANKER_ROLE: //地主3张牌
        {
            float center = 1.0f;
            Vector<Node*> children = m_cardsHolder->getChildren();
            for (ssize_t i = 0; i < children.size(); ++i)
            {
                CardSprite *tmp = static_cast<CardSprite*>(children.at(i));
                if (nullptr == tmp)
                {
                    continue;
                }
                Vec2 pos = Vec2(( i - center) * 150, CARD_Y_POS);
                tmp->showCardBack(true);
                tmp->setPosition(pos);
                tmp->hideCardSelectedEffect();
            }
        }
            break;
        default:
            break;
    }
}

void CardsNode::removeAllCards(bool showAnimation /* = true */)
{
    m_cardsData = tagCards();
    m_mapCard.clear();
    m_vecCard.clear();
    vector<CardSprite*>().swap(m_vecSelectCard);
    m_mapSelectedCards.clear();
    m_mapDragSelectCards.clear();
    m_nHandCardCount = 0;
    
    if (nullptr != m_cardsHolder)
    {
        m_cardsHolder->removeAllChildrenWithCleanup(true);
    }
    m_bShowCard = false;
    
    /*
    if (nullptr != m_leftCardsLabel)
    {
        m_leftCardsLabel->setString("");
    }
     */
    if (nullptr != m_atlasLeftLabel)
    {
        m_atlasLeftLabel->setString("");
    }
}

void CardsNode::showCards()
{
    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
    {
        BYTE cardData = m_cardsData.m_cardData[i];
        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
        if (ite == m_mapCard.end())
        {
            continue;
        }
        CardSprite* tmp = ite->second;
        tmp->showCardBack(false);
    }
}

void CardsNode::showCardsNiuNiu()
{
    
}

void CardsNode::showLeftCards(const tagCards &cards)
{
    //自己的牌不需要处理
    if (kMY_ROLE == m_layerDir)
    {
        return;
    }
    Vector<Node*> children = m_cardsHolder->getChildren();
    
    int center = 0;
    if (kRIGHT_ROLE == m_layerDir)
    {
        center = (int)children.size();
    }
    
    if (cards.m_cardCount <= children.size())
    {
        for (int i = 0; i < cards.m_cardCount; ++i)
        {
            CardSprite* tmp = static_cast<CardSprite*>(children.at(i));
            if (nullptr == tmp)
            {
                continue;
            }
            BYTE color = (CGameLogic::getInstance()->GetCardColor(cards.m_cardData[i]) >> 4) + 1;
            CardSprite::ValueColor valueColor = color & 0x01 ? CardSprite::ValueColor::kRedCard : CardSprite::ValueColor::kBlackCard;
            tmp->setCardValue(cards.m_cardData[i], valueColor);
            
            Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
            MoveTo *moveTo = MoveTo::create(0.5f + i / 16.0f, pos);
            CallFunc *call = CallFunc::create([tmp]
                                              {
                                                  tmp->showCardBack(false);
                                              });
            Spawn *spa = Spawn::createWithTwoActions(moveTo, call);
            tmp->stopAllActions();
            tmp->runAction(spa);
        }
    }
}

void CardsNode::openCards(bool bAnimation)
{
    m_bShowCard = true;
    //自己的牌不需要处理
    if (kMY_ROLE == m_layerDir)
    {
        return;
    }
    Vector<Node*> children = m_cardsHolder->getChildren();
    
    int center = 0;
    if (kRIGHT_ROLE == m_layerDir)
    {
        center = (int)children.size();
    }
    
    if (bAnimation)
    {
        for (int i = 0; i < m_cardsData.m_cardCount; ++i)
        {
            BYTE mapKey = m_cardsData.m_cardData[i];
            mapKey = mapKey != 0 ? mapKey : i;
            
            map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
            if (ite == m_mapCard.end())
            {
                continue;
            }
            CardSprite* tmp = ite->second;
            
            Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
            MoveTo *moveTo = MoveTo::create(0.5f + abs(i - center) / 16.0f, pos);
            CallFunc *call = CallFunc::create([tmp]
                                              {
                                                  tmp->showCardBack(false);
                                              });
            Spawn *spa = Spawn::createWithTwoActions(moveTo, call);
            tmp->stopAllActions();
            tmp->runAction(spa);
        }
    }
    else
    {
        for (int i = 0; i < m_cardsData.m_cardCount; ++i)
        {
            BYTE mapKey = m_cardsData.m_cardData[i];
            mapKey = mapKey != 0 ? mapKey : i;
            
            map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
            if (ite == m_mapCard.end())
            {
                continue;
            }
            CardSprite* tmp = ite->second;
            
            Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
            tmp->showCardBack(false);
            tmp->stopAllActions();
            tmp->setPosition(pos);
        }
    }
}

void CardsNode::reSetCards()
{
    m_beginSelectCard = nullptr;
    m_endSelectCard = nullptr;
    
    m_vecSelectCard = filterDragSelectCards(false);
    dragCards();
    m_mapSelectedCards.clear();
    m_mapDragSelectCards.clear();
    
    m_bSuggested = false;
}

void CardsNode::suggestShootCards(const tagCards &cards)
{
    if (!m_bTouched)
    {
        m_beginSelectCard = nullptr;
        m_endSelectCard = nullptr;
    }
    //将已经选择的扑克状态更新
    m_vecSelectCard = filterDragSelectCards(false);
    dragCards();
    m_mapSelectedCards.clear();
    m_mapDragSelectCards.clear();
    
    if (!m_bSuggested)
    {
        for (BYTE i = 0; i < cards.m_cardCount; ++i)
        {
            CardSprite *tmp = static_cast<CardSprite*>(m_cardsHolder->getChildByTag(cards.m_cardData[i]));
            if (nullptr != tmp)
            {
                tmp->setCardDragSelect(true);
                m_mapSelectedCards.insert(make_pair(tmp->getCardData(),tmp));
            }
        }
        m_vecSelectCard = filterDragSelectCards(false);
        dragCards();
    }
    m_bSuggested = !m_bSuggested;
}

void CardsNode::addCardsHolder()
{
    if (nullptr == m_cardsHolder)
    {
        m_cardsHolder = Node::create();
        this->addChild(m_cardsHolder);
    }
}

void CardsNode::readyForCards(const BYTE &counts)
{
    m_vecSelectCard.reserve((size_t)counts);
}

void CardsNode::arrangeAllCards(bool showAnimation /* = true */,
                                cocos2d::Action *pCallBack /*= nullptr*/)
{
    int idx = 0;
    //动画效果
    if (showAnimation)
    {
        switch (m_layerDir)
        {
            case enGameRoleDir::kMY_ROLE:
            {
                //针对百家乐的最多三张
                float center = m_cardsData.m_cardCount * 0.5f;
                if ( 1 == m_cardsData.m_cardCount)
                {
                    center = 0;
                }
                else if ( 2 == m_cardsData.m_cardCount)
                {
                    center = 0.5;
                }
                else if (3 == m_cardsData.m_cardCount)
                {
                    center = 1.0f;
                }
                
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    /*
                    BYTE cardData = m_cardsData.m_cardData[i];
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                     */
                    CardSprite *tmp = m_vecCard[i]; //针对百家乐多幅牌
                    tmp->stopAllActions();
                    Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS, CARD_Y_POS);
                    if (tmp->getDispatched())
                    {
                        tmp->setPosition(pos);
                        continue;
                    }
                    
                    tmp->setDispatched(true);
                    
                    //转换为相对于自己的中间位置
                    Size winSize = INSTANCE(Director)->getWinSize();
                    Vec2 centerPos = Vec2(winSize.width * 0.5f, winSize.height * 0.55);
                    centerPos = this->convertToNodeSpace(centerPos);
                    auto moveTo = MoveTo::create(ANI_BEGIN / 2, Vec2(centerPos.x, CARD_Y_POS));
                    auto moveTo2 = MoveTo::create(ANI_BEGIN / 2, pos);
                    auto delay = DelayTime::create(ANI_BEGIN);
                    auto hideBack = CallFunc::create([=]
                                                     {
                                                         if(!m_bAddCards)
                                                         {
                                                             INSTANCE(AudioMgr)->playSendCardAudio();
                                                         }
                                                         
                                                     });
                    auto seq = Sequence::create(delay,hideBack,nullptr);
                    auto spawn = Spawn::create(moveTo,ScaleTo::create(ANI_BEGIN, CARD_SHOW_SCALE), CallFunc::create([=]
                                                                                                                    {
                                                                                                                        if (i == m_cardsData.m_cardCount - 1 && nullptr != pCallBack)
                                                                                                                        {
                                                                                                                            tmp->runAction(pCallBack);
                                                                                                                            pCallBack->release();
                                                                                                                            
                                                                                                                            m_bDispatching = false;
                                                                                                                        }
                                                                                                                    }),seq,nullptr);
                    auto runSeq = Sequence::create(DelayTime::create(ANI_RATE(idx)), spawn, DelayTime::create(0.05f), moveTo2, CallFunc::create([tmp]()
                                                                                                                                                {
                                                                                            tmp->showCardBack(false);
                                                                                                                                                }), NULL);
                    
                    tmp->runAction(runSeq);
                    ++idx;
                }
            }
                break;
            case enGameRoleDir::kLEFT_ROLE:
            {
                //niuniu
                //float center = 2.0f;
                if (m_bShowCard)
                {
                    int center = 0;
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE mapKey = m_cardsData.m_cardData[i];
                        mapKey = mapKey != 0 ? mapKey : i;
                        
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
                        tmp->stopAllActions();
                        if (tmp->getDispatched())
                        {
                            tmp->setPosition(pos);
                            continue;
                        }
                        tmp->setDispatched(true);
                        
                        auto moveTo = MoveTo::create(ANI_BEGIN, pos);
                        auto delay = DelayTime::create(ANI_BEGIN);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             if(!m_bAddCards)
                                                             {
                                                                 INSTANCE(AudioMgr)->playSendCardAudio();
                                                             }
                                                             
                                                             tmp->showCardBack(false);
                                                             
                                                             if (i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispatching = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,ScaleTo::create(ANI_BEGIN, CARD_HIDE_SCALE),seq,nullptr);
                        auto runSeq = Sequence::create(DelayTime::create(ANI_RATE(idx)), spawn, NULL);
                        
                        tmp->runAction(runSeq);
                        ++idx;
                    }
                }
                else
                {
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE mapKey = m_cardsData.m_cardData[i];
                        mapKey = mapKey != 0 ? mapKey : i;
                        
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(CARD_X_POS, CARD_Y_POS);
                        tmp->stopAllActions();
                        if (tmp->getDispatched())
                        {
                            tmp->setPosition(pos);
                            continue;
                        }
                        tmp->setDispatched(true);
                        
                        auto moveTo = MoveTo::create(ANI_BEGIN, pos);
                        auto delay = DelayTime::create(ANI_BEGIN);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             if(!m_bAddCards)
                                                             {
                                                                 INSTANCE(AudioMgr)->playSendCardAudio();
                                                             }
                                                             
                                                             tmp->showCardBack(true);
                                                             
                                                             if (i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispatching = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,ScaleTo::create(ANI_BEGIN, CARD_HIDE_SCALE),seq,nullptr);
                        auto runSeq = Sequence::create(DelayTime::create(ANI_RATE(idx)), spawn, NULL);
                        
                        tmp->runAction(runSeq);
                        ++idx;
                    }
                }
            }
                break;
            case enGameRoleDir::kRIGHT_ROLE:
            {
                //niuniu
                //float center = 2.0f;
                if (m_bShowCard)
                {
                    int center = m_cardsHolder->getChildren().size();
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE mapKey = m_cardsData.m_cardData[i];
                        mapKey = mapKey != 0 ? mapKey : i;
                        
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(( i - center) * BJL_CARD_X_POS * CARD_HIDE_SCALE, CARD_Y_POS);
                        tmp->stopAllActions();
                        if (tmp->getDispatched())
                        {
                            tmp->setPosition(pos);
                            continue;
                        }
                        tmp->setDispatched(true);
                        
                        auto moveTo = MoveTo::create(ANI_BEGIN, pos);
                        auto delay = DelayTime::create(ANI_BEGIN);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             if(!m_bAddCards)
                                                             {
                                                                 INSTANCE(AudioMgr)->playSendCardAudio();
                                                             }
                                                             
                                                             tmp->showCardBack(false);
                                                             
                                                             if (i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispatching = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,ScaleTo::create(ANI_BEGIN, CARD_HIDE_SCALE),seq,nullptr);
                        auto runSeq = Sequence::create(DelayTime::create(ANI_RATE(idx)), spawn, NULL);
                        
                        tmp->runAction(runSeq);
                        ++idx;
                    }
                }
                else
                {
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE mapKey = m_cardsData.m_cardData[i];
                        mapKey = mapKey != 0 ? mapKey : i;
                        
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(CARD_X_POS, CARD_Y_POS);
                        tmp->stopAllActions();
                        if (tmp->getDispatched())
                        {
                            tmp->setPosition(pos);
                            continue;
                        }
                        tmp->setDispatched(true);
                        
                        auto moveTo = MoveTo::create(ANI_BEGIN, pos);
                        auto delay = DelayTime::create(ANI_BEGIN);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             if(!m_bAddCards)
                                                             {
                                                                 INSTANCE(AudioMgr)->playSendCardAudio();
                                                             }
                                                             
                                                             tmp->showCardBack(true);
                                                             
                                                             if (i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispatching = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,ScaleTo::create(ANI_BEGIN, CARD_HIDE_SCALE),seq,nullptr);
                        auto runSeq = Sequence::create(DelayTime::create(ANI_RATE(idx)), spawn, NULL);
                        
                        tmp->runAction(runSeq);
                        ++idx;
                    }
                }
            }
                break;
            case enGameRoleDir::kTOP_ROLE:
            {
                //niuniu
                //float center = 2.0f;
                if (m_bShowCard)
                {
                    
                }
                else
                {
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE mapKey = m_cardsData.m_cardData[i];
                        mapKey = mapKey != 0 ? mapKey : i;
                        
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(CARD_X_POS, CARD_Y_POS);
                        tmp->stopAllActions();
                        if (tmp->getDispatched())
                        {
                            tmp->setPosition(pos);
                            continue;
                        }
                        tmp->setDispatched(true);
                        
                        auto moveTo = MoveTo::create(ANI_BEGIN, pos);
                        auto delay = DelayTime::create(ANI_BEGIN + ANI_RATE(i));
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             if(!m_bAddCards)
                                                             {
                                                                 INSTANCE(AudioMgr)->playSendCardAudio();
                                                             }
                                                             
                                                             tmp->showCardBack(true);
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,ScaleTo::create(ANI_BEGIN, CARD_HIDE_SCALE),seq,nullptr);
                        auto runSeq = Sequence::create(DelayTime::create(ANI_RATE(idx)), spawn, NULL);
                        
                        tmp->runAction(runSeq);
                        ++idx;
                    }
                }
            }
                break;
            default:
            {
                if (nullptr != pCallBack)
                {
                    pCallBack->release();
                }
            }
                break;
        }
    }
    else
    {
        //整理卡牌位置
        this->reSortCards(m_cardsData,m_bShowCard);
    }
}

void CardsNode::switchCardState(CardSprite *card)
{
    if (nullptr == card)
    {
        return;
    }
    card->stopAllActions();
    bool tmpBShoot = card->getCardShoot();
    if (tmpBShoot)
    {
        card->runAction(m_actBackCard);
        card->setCardShoot(false);
    }
    else
    {
        card->runAction(m_actShootCard);
        card->setCardShoot(true);
    }
    card->hideCardSelectedEffect();
}

void CardsNode::dragCards()
{
    for (size_t i = 0; i < m_vecSelectCard.size(); ++i)
    {
        CardSprite *card = m_vecSelectCard[i];
        Vec2 pos = card->getPosition();
        card->stopAllActions();
        if (!card->getCardShoot())
        {
            auto shoot = MoveTo::create(CARD_SHOOT_TIME,Vec2(pos.x,CARD_Y_POS + CARD_SHOOT_DIS));
            card->runAction(shoot);
            card->setCardShoot(true);
            if (nullptr != m_pSelectedListener && m_bClickable)
            {
                m_pSelectedListener->onCardsStateChange(card->getCardData(), true);
            }
            
            m_mapSelectedCards.insert(make_pair(card->getCardData(),card));
        }
        else
        {
            auto shoot = MoveTo::create(CARD_SHOOT_TIME,Vec2(pos.x,CARD_Y_POS));
            card->runAction(shoot);
            card->setCardShoot(false);
            if (nullptr != m_pSelectedListener && m_bClickable)
            {
                m_pSelectedListener->onCardsStateChange(card->getCardData(), false);
            }
            
            map<BYTE,CardSprite*>::iterator ite = m_mapSelectedCards.find(card->getCardData());
            if (ite != m_mapSelectedCards.end())
            {
                m_mapSelectedCards.erase(ite);
            }
        }
    }
    
    bool tmpShow = m_layerDir == kMY_ROLE ? false : !m_bShowCard;
    auto children = m_cardsHolder->getChildren();
    for (ssize_t i = 0; i < children.size(); ++i)
    {
        CardSprite *sp = (CardSprite*)children.at(i);
        if (nullptr != sp)
        {
            sp->hideCardSelectedEffect();
            sp->setCardDragSelect(false);
            sp->showCardBack(tmpShow);
        }
    }
    
    m_tSelectedCards = tagCards();
    m_tSelectedCards.m_cardCount = m_mapSelectedCards.size();
    int i = 0;
    map<BYTE,CardSprite*>::iterator ite = m_mapSelectedCards.begin();
    for (; ite != m_mapSelectedCards.end(); ++ite)
    {
        m_tSelectedCards.m_cardData[i] = ite->second->getCardData();
        ++i;
    }
    if (nullptr != m_pSelectedListener && m_bClickable)
    {
        //INSTANCE(CGameLogic)->SortCardList(m_tSelectedCards.m_cardData, m_tSelectedCards.m_cardCount, ST_ORDER);
        
        m_pSelectedListener->onSelectedCards(m_tSelectedCards);
    }
    m_mapDragSelectCards.clear();
    vector<CardSprite*>().swap(m_vecSelectCard);
}

CardSprite* CardsNode::filterCard(const enFilterFlag &flag, const Point& touchPoint)
{
    vector<CardSprite*> tmpSel = vector<CardSprite*>();
    map<BYTE,CardSprite*>::iterator ite = m_mapCard.begin();
    for (; ite != m_mapCard.end(); ++ite)
    {
        CardSprite* tmp = ite->second;
        Point locationInNode = tmp->convertToNodeSpace(touchPoint);;
        Rect rect = Rect(0,0,tmp->getContentSize().width,tmp->getContentSize().height);
        if (rect.containsPoint(locationInNode))
        {
            tmpSel.push_back(tmp);
        }
    }
    
    if (tmpSel.empty())
    {
        return nullptr;
    }
    std::sort(tmpSel.begin(),tmpSel.end(),sortByDrawOrder);
    if (CardsNode::enFilterFlag::kHIGHEST == flag)
    {
        return tmpSel[tmpSel.size() - 1];
    }
    else
    {
        return tmpSel[0];
    }
}

std::vector<CardSprite*> CardsNode::inTouchAreaCards(const Rect &touchRect)
{
    map<BYTE,CardSprite*> tmpMap = map<BYTE,CardSprite*>();
    map<BYTE,CardSprite*>::iterator ite = m_mapCard.begin();
    for (; ite != m_mapCard.end(); ++ite)
    {
        CardSprite* tmp = ite->second;
        Point locationInNode = tmp->getPosition();
        Size tmpSize =  tmp->getContentSize();
        Vec2 anchor = tmp->getAnchorPoint();
        
        Vec2 ori = Vec2(locationInNode.x - tmpSize.width * anchor.x,locationInNode.y - tmpSize.height * anchor.y);
        Rect rect = Rect(ori.x,ori.y,tmpSize.width,tmpSize.height);
        
        if (rect.intersectsRect(touchRect))
        {
            tmpMap.insert(make_pair(tmp->getCardData(),tmp));
        }		
    }
    return filterDragSelectCards(true,tmpMap);
}

Rect CardsNode::makeTouchRect(const Vec2 &endTouch)
{
    Vec2 movePoint = endTouch;	
    //判断拖动方向（左右）
    m_dragMoveDir = (m_beginTouchPoint.x) < (movePoint.x) ? enMoveDir::kMoveToRight : enMoveDir::kMoveToLeft;
    bool toRight = (m_beginTouchPoint.x) < (movePoint.x) ? true : false;
    //判断拖动方向（上下）
    bool toTop = (m_beginTouchPoint.y) < (movePoint.y) ? true : false;
    
    Rect touchRect = Rect::ZERO;
    
    if (toRight && toTop) //右上
    {
        touchRect = Rect(m_beginTouchPoint.x,m_beginTouchPoint.y,movePoint.x - m_beginTouchPoint.x,movePoint.y - m_beginTouchPoint.y);
    }
    else if (toRight && !toTop) //右下
    {
        touchRect = Rect(m_beginTouchPoint.x,movePoint.y,movePoint.x - m_beginTouchPoint.x,m_beginTouchPoint.y - movePoint.y);
    }
    else if (!toRight && toTop) //左上
    {
        touchRect = Rect(movePoint.x,m_beginTouchPoint.y,m_beginTouchPoint.x - movePoint.x, movePoint.y - m_beginTouchPoint.y);
    }
    else if (!toRight && !toTop) //左下
    {
        touchRect = Rect(movePoint.x,movePoint.y,m_beginTouchPoint.x - movePoint.x, m_beginTouchPoint.y - movePoint.y);
    }
    
    return touchRect;
}

vector<CardSprite*> CardsNode::filterDragSelectCards(bool bFilter /*= true*/,const map<BYTE,CardSprite*> &cards /*= map<BYTE,CardSprite*>()*/)
{
    int lowOrder = getLowOrder();
    int hightOrder = getHightOrder();
    
    //过滤对象
    map<BYTE,CardSprite*>tmpMap = map<BYTE,CardSprite*>();	
    if (cards.empty())
    {
        //合并
        tmpMap.insert(m_mapSelectedCards.begin(),m_mapSelectedCards.end());
        tmpMap.insert(m_mapDragSelectCards.begin(),m_mapDragSelectCards.end());
    }
    else
    {
        tmpMap = cards;
    }
    
    vector<CardSprite*> tmp = vector<CardSprite*>();
    map<BYTE,CardSprite*>::iterator ite = tmpMap.begin();
    for (; ite != tmpMap.end(); ++ite)
    {		
        CardSprite* tmpCard = ite->second;
        if (bFilter)
        {
            if (tmpCard->getLocalZOrder() >= lowOrder && tmpCard->getLocalZOrder() <= hightOrder)
            {
                tmp.push_back(tmpCard);
            }
        }
        else
        {
            tmp.push_back(tmpCard);
        }		
    }
    
    return tmp;
}

int CardsNode::getLowOrder()
{
    int beginOrder = m_beginSelectCard != nullptr ? m_beginSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
    int endOrder = MAX_DRAW_ORDER;
    if (kMoveToLeft == m_dragMoveDir)
    {
        endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
    }
    else
    {
        endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MAX_DRAW_ORDER;
    }
    
    return MIN(beginOrder, endOrder);
}

int CardsNode::getHightOrder()
{
    int beginOrder = m_beginSelectCard != nullptr ? m_beginSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
    int endOrder = MAX_DRAW_ORDER;
    
    if (kMoveToLeft == m_dragMoveDir)
    {
        endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
    }
    else
    {
        endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MAX_DRAW_ORDER;
    }
    return MAX(beginOrder, endOrder);
}

void CardsNode::reOrderCards()
{
    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
    {
        BYTE mapKey = m_cardsData.m_cardData[i];
        mapKey = mapKey != 0 ? mapKey : i;
        
        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(mapKey);
        if (ite == m_mapCard.end())
        {
            continue;
        }
        CardSprite* tmp = ite->second;
        tmp->setLocalZOrder(i);
    }
}