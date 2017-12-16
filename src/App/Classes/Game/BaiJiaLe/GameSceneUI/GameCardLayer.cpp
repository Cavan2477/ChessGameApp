//
//  GameCardLayer.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/1/16.
//
//

#include "GameCardLayer.h"
#include "GameLogic.h"
#include "GameDataMgr.h"
#include "GameRecordMgr.h"

USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

GameCardLayer::GameCardLayer():
m_notify(nullptr),
m_spIdleStatus(nullptr),
m_spMasterStatus(nullptr),
m_enPointResult(enPointResult::kPointDefault)
{
    for (int i = 0; i < 2; ++i)
    {
        m_spPoint[i] = nullptr;
        m_nodeCards[i] = nullptr;
    }
    m_nTotalCount = 0;
    m_nDispatchedCount = 0;
    m_actTransition =  nullptr;
    m_actTransitionBack = nullptr;
}

GameCardLayer::~GameCardLayer()
{
    CC_SAFE_RELEASE(m_actTransition);
    CC_SAFE_RELEASE(m_actTransitionBack);
}

bool GameCardLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        //读取csb
        LOAD_CSB("game/GameCardLayer.csb");
        
        auto tmpSp = static_cast<Sprite*>(m_root->getChildByName("clearing_bg"));
        CC_ASSERT(tmpSp != nullptr);
        Size tmpSize = tmpSp->getContentSize();
        m_spBg = tmpSp;
        
        m_spIdleStatus = static_cast<Sprite*>(tmpSp->getChildByName("idle_sp"));
        CC_ASSERT(m_spIdleStatus != nullptr);
        
        m_spMasterStatus = static_cast<Sprite*>(tmpSp->getChildByName("master_sp"));
        CC_ASSERT(m_spMasterStatus != nullptr);
        
        m_spPoint[1] = static_cast<Sprite*>(tmpSp->getChildByName("idle_res_sp"));
        CC_ASSERT(m_spPoint[1] != nullptr);
        
        m_spPoint[0] = static_cast<Sprite*>(tmpSp->getChildByName("master_res_sp"));
        CC_ASSERT(m_spPoint[0] != nullptr);
        
        m_spDraw = static_cast<Sprite*>(tmpSp->getChildByName("draw_sp"));
        CC_ASSERT(m_spDraw != nullptr);
        m_spDraw->setLocalZOrder(1);
        
        m_nodeCards[1] = CardsNode::createEmptyCardsNode();
        CC_ASSERT(m_nodeCards[1] != nullptr);
        m_nodeCards[1]->setDir(kMY_ROLE);
        m_nodeCards[1]->setPosition(Vec2(tmpSize.width * 0.26, tmpSize.height * 0.36));
        tmpSp->addChild(m_nodeCards[1]);
        
        m_nodeCards[0] = CardsNode::createEmptyCardsNode();
        CC_ASSERT(m_nodeCards[0] != nullptr);
        m_nodeCards[0]->setDir(kMY_ROLE);
        m_nodeCards[0]->setPosition(Vec2(tmpSize.width * 0.74, tmpSize.height * 0.36));
        tmpSp->addChild(m_nodeCards[0]);
        
        auto tmp = m_root->getChildByName("view_tmp");
        CC_ASSERT(tmp != nullptr);
        m_tipsView = LCScrollViewType2::create(tmp->getContentSize());
        CC_ASSERT(m_tipsView != nullptr);
        m_tipsView->setPosition(tmp->getPosition());
        m_tipsView->setDirection(extension::ScrollView::Direction::VERTICAL);
        m_tipsView->setCanMove(false);
        m_tipsView->setAutoHide(false);
        m_tipsView->setUpdateOpacity(false);
        m_root->addChild(m_tipsView);
        tmp->removeFromParentAndCleanup(true);
        
        reSet();
        
        Size winSize = INSTANCE(Director)->getWinSize();
        auto call = CallFunc::create([tmpSp, winSize]()
                                     {
                                         tmpSp->setPositionY(winSize.height + tmpSp->getContentSize().height);
                                     });
        auto move = MoveTo::create(0.5f, Vec2(tmpSp->getPositionX(), winSize.height));
        auto call2 = CallFunc::create([this]()
                                      {
                                          if (!this->isScheduled(SEL_SCHEDULE(&GameCardLayer::dispatchUpdate)))
                                          {
                                              this->schedule(SEL_SCHEDULE(&GameCardLayer::dispatchUpdate),1.0f);
                                          }
                                      });
        m_actTransition = Sequence::create(call, move, call2, nullptr);
        m_actTransition->retain();
        
        auto move2 = MoveTo::create(0.5f, Vec2(tmpSp->getPositionX(), winSize.height + tmpSize.height));
        auto call3 = CallFunc::create([this]()
                                      {
                                          this->setVisible(false);
                                          this->reSet();
                                      });
        m_actTransitionBack = Sequence::create(move2, call3,NULL);
        m_actTransitionBack->retain();
        bRes = true;
    } while (false );
    return bRes;
}

void GameCardLayer::showLayer(bool bShow)
{
    if (!bShow)
    {
        m_spBg->stopAllActions();
        m_spBg->runAction(m_actTransitionBack);
        this->unschedule(SEL_SCHEDULE(&GameCardLayer::autoHide));
    }
    else
    {
        this->setVisible(bShow);
    }
}

void GameCardLayer::refresh(const tagCardsResult &tagRes, const bool &bAni, const BYTE &cbTime)
{
    reSet();
    
    std::vector<tagDispathCard>().swap(m_vecDispatchCards);
    m_nTotalCount = tagRes.m_idleCards.m_cardCount + tagRes.m_masterCards.m_cardCount;
    m_vecDispatchCards.reserve(m_nTotalCount);
    
    int idleIdx = 0;
    int masterIdx = 0;
    tagDispathCard dis = {};
    for (int i = 0; i < m_nTotalCount; ++i)
    {
        if (i & 1)
        {
            dis = {};
            if (0 != tagRes.m_masterCards.m_cardData[masterIdx])
            {
                dis.m_dir = enGameRoleDir::kRIGHT_ROLE;
                dis.m_cbCardData = tagRes.m_masterCards.m_cardData[masterIdx];
                ++masterIdx;
            }
            else
            {
                dis.m_dir = enGameRoleDir::kLEFT_ROLE;
                dis.m_cbCardData = tagRes.m_idleCards.m_cardData[idleIdx];
                ++idleIdx;
            }
        }
        else
        {
            dis = {};
            if (0 != tagRes.m_idleCards.m_cardData[idleIdx])
            {
                dis.m_dir = enGameRoleDir::kLEFT_ROLE;
                dis.m_cbCardData = tagRes.m_idleCards.m_cardData[idleIdx];
                ++idleIdx;
            }
            else
            {
                dis.m_dir = enGameRoleDir::kRIGHT_ROLE;
                dis.m_cbCardData = tagRes.m_masterCards.m_cardData[masterIdx];
                ++masterIdx;
            }
        }
        if ( 0 == dis.m_cbCardData )
        {
            log("error");
        }
        m_vecDispatchCards.push_back(dis);
    }
    
    if (bAni)
    {
        m_spBg->stopAllActions();
        m_spBg->runAction(m_actTransition);
    }
    else
    {
        m_spBg->setPositionY(INSTANCE(Director)->getWinSize().height);
        
        m_nodeCards[1]->updateCardsNode(kMY_ROLE,
                                        tagRes.m_idleCards,
                                        true,
                                        false);
        refreshPoint(kLEFT_ROLE);
        
        m_nodeCards[0]->updateCardsNode(kMY_ROLE,
                                        tagRes.m_masterCards,
                                        true,
                                        false);
        refreshPoint(kRIGHT_ROLE);
        
        calResult();
    }
}

void GameCardLayer::addCards(const tagCards &cards, const enGameRoleDir &dir, cocos2d::Action *pCallBack)
{
    int nIdx = dir - 1;
    if (nIdx < 0 || nIdx > 1) return;
    if (nullptr != pCallBack)
    {
        pCallBack->retain();
    }
    m_nodeCards[nIdx]->addCards(cards,true,pCallBack);
}

void GameCardLayer::refreshPoint(const enGameRoleDir &dir)
{
    int nIdx = dir - 1;
    if (nIdx < 0 || nIdx > 1) return;
    
    tagCards hand = m_nodeCards[nIdx]->getHandCards();
    
    //切换动画
    auto sca = ScaleTo::create(0.2f, 0.0001f, 1.0f);
    auto call = CallFunc::create([this,nIdx,hand]()
                                 {
                                     BYTE point = INSTANCE(CGameLogic)->GetCardListPip(hand.m_cardData, hand.m_cardCount);
                                     char buf[64] = "";
                                     sprintf(buf, "clearing_%d.png",point);
                                     SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(buf);
                                     if (nullptr != frame)
                                     {
                                         m_spPoint[nIdx]->setSpriteFrame(frame);
                                     }
                                 });
    auto sca2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
    auto seq = Sequence::create(sca, call, sca2, NULL);
    
    m_spPoint[nIdx]->stopAllActions();
    m_spPoint[nIdx]->runAction(seq);
}

void GameCardLayer::refreshResult(const GameCardLayer::enPointResult &enResult)
{
    if (kDraw == enResult)
    {
        m_spDraw->setVisible(true);
    }
    else if (kIdleWin == enResult)
    {
        auto sca = ScaleTo::create(0.2f, 0.0001f, 1.0f);
        auto call = CallFunc::create([this]()
                                     {
                                         SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("room_clearing_ldlewin.png");
                                         if (nullptr != frame)
                                         {
                                             m_spIdleStatus->setSpriteFrame(frame);
                                         }
                                     });
        auto sca2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
        auto seq = Sequence::create(sca, call, sca2, NULL);
        
        m_spIdleStatus->stopAllActions();
        m_spIdleStatus->runAction(seq);
    }
    else if (kMasterWin == enResult)
    {
        auto sca = ScaleTo::create(0.2f, 0.0001f, 1.0f);
        auto call = CallFunc::create([this]()
                                     {
                                         SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("room_clearing_masterwin.png");
                                         if (nullptr != frame)
                                         {
                                             m_spMasterStatus->setSpriteFrame(frame);
                                         }
                                     });
        auto sca2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
        auto seq = Sequence::create(sca, call, sca2, NULL);
        m_spMasterStatus->stopAllActions();
        m_spMasterStatus->runAction(seq);
    }
    
    /*
    if (!this->isScheduled(SEL_SCHEDULE(&GameCardLayer::autoHide)))
    {
        this->schedule(SEL_SCHEDULE(&GameCardLayer::autoHide),10.0f);
    }
     */
}

void GameCardLayer::reSet()
{
    SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("room_clearing_ldlefail.png");
    if (nullptr == frame)
    {
        frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png");
    }
    m_spIdleStatus->setSpriteFrame(frame);
    
    frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("room_clearing_masterfail.png");
    if (nullptr == frame)
    {
        frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png");
    }
    m_spMasterStatus->setSpriteFrame(frame);
    
    for (int i = 0; i < 2; ++i)
    {
        m_nodeCards[i]->removeAllCards();
        
        frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png");
        m_spPoint[i]->setSpriteFrame(frame);
    }
    m_spDraw->setVisible(false);
    
    m_enPointResult = enPointResult::kPointDefault;
    
    m_tipsView->reSet();
    
    m_nTotalCount = 0;
    m_nDispatchedCount = 0;
    
    m_spBg->setPositionY(INSTANCE(Director)->getWinSize().height + m_spBg->getContentSize().height);
}

void GameCardLayer::dispatchUpdate(float dt)
{
    if (!m_vecDispatchCards.empty())
    {
        ++m_nDispatchedCount;
        tagDispathCard dis = *(m_vecDispatchCards.begin());
        m_vecDispatchCards.erase(m_vecDispatchCards.begin());
        
        tagCards tmp = tagCards();
        tmp.m_cardCount = 1;
        tmp.m_cardData[0] = dis.m_cbCardData;
        this->addCards(tmp, dis.m_dir, CallFunc::create([this,dis]()
                                                        {
                                                            this->refreshPoint(dis.m_dir);
                                                        }));
        this->noticeTips();
    }
    else
    {
        calResult();
        if (this->isScheduled(SEL_SCHEDULE(&GameCardLayer::dispatchUpdate)))
        {
            this->unschedule(SEL_SCHEDULE(&GameCardLayer::dispatchUpdate));
        }        
    }
}

void GameCardLayer::autoHide(float dt)
{
    showLayer(false);
}

void GameCardLayer::calResult()
{
    tagCards idleCards = m_nodeCards[1]->getHandCards();
    INSTANCE(CGameLogic)->SortCardList(idleCards.m_cardData, idleCards.m_cardCount, ST_ORDER);
    BYTE idlePoint = INSTANCE(CGameLogic)->GetCardListPip(idleCards.m_cardData, idleCards.m_cardCount);
    
    tagCards masterCards = m_nodeCards[0]->getHandCards();
    INSTANCE(CGameLogic)->SortCardList(masterCards.m_cardData, masterCards.m_cardCount, ST_ORDER);
    BYTE masterPoint = INSTANCE(CGameLogic)->GetCardListPip(masterCards.m_cardData, masterCards.m_cardCount);
    
    BYTE nowCBWinner = AREA_MAX;
    BYTE nowCBKingWinner = AREA_MAX;
    if (idlePoint > masterPoint)
    {
        m_enPointResult = enPointResult::kIdleWin;
        
        nowCBWinner = AREA_XIAN;
        if (idlePoint == 8 || idlePoint == 9)
        {
            nowCBKingWinner = AREA_XIAN_TIAN;
        }
    }
    else if (idlePoint < masterPoint)
    {
        m_enPointResult = enPointResult::kMasterWin;
        
        nowCBWinner = AREA_ZHUANG;
        if (masterPoint == 8 || masterPoint == 9)
        {
            nowCBKingWinner = AREA_ZHUANG_TIAN;
        }
    }
    else if (idlePoint == masterPoint)
    {
        m_enPointResult = enPointResult::kDraw;
        
        nowCBWinner = AREA_PING;
        //判断是否为同点平
        bool bAllPointSame = false;
        if (idleCards.m_cardCount == masterCards.m_cardCount)
        {
            BYTE cbCardIdx = 0;
            for (cbCardIdx = 0; cbCardIdx < idleCards.m_cardCount; ++cbCardIdx)
            {
                BYTE cbBankerValue = INSTANCE(CGameLogic)->GetCardValue(masterCards.m_cardData[cbCardIdx]);
                BYTE cbIdleValue = INSTANCE(CGameLogic)->GetCardValue(idleCards.m_cardData[cbCardIdx]);
                
                if (cbBankerValue != cbIdleValue)
                {
                    break;
                }
            }
            if (cbCardIdx == masterCards.m_cardCount)
            {
                bAllPointSame = true;
            }
        }
        
        if (bAllPointSame)
        {
            nowCBKingWinner = AREA_TONG_DUI;
        }
    }
    
    //对子判断
    bool nowBIdleTwoPair = false;
    bool nowBMasterTwoPair = false;
    if (INSTANCE(CGameLogic)->GetCardValue(idleCards.m_cardData[0]) == INSTANCE(CGameLogic)->GetCardValue(idleCards.m_cardData[1]))
    {
        nowBIdleTwoPair = true;
    }
    if (INSTANCE(CGameLogic)->GetCardValue(masterCards.m_cardData[0]) == INSTANCE(CGameLogic)->GetCardValue(masterCards.m_cardData[1]))
    {
        nowBMasterTwoPair = true;
    }
    
    if ( !INSTANCE(CGameLogic)->getDynamicJoin())
    {
        //添加路单记录
        tagGameRecord rec = tagGameRecord();
        tagServerGameRecord *pRec = new tagServerGameRecord();
        pRec->bBankerTwoPair = nowBMasterTwoPair;
        pRec->bPlayerTwoPair = nowBIdleTwoPair;
        pRec->cbBankerCount = masterPoint;
        pRec->cbPlayerCount = idlePoint;
        pRec->cbKingWinner = nowCBKingWinner;
        rec.m_pServerRecord = pRec;
        rec.m_cbGameResult = nowCBWinner;
        
        rec.m_tagUserRecord.m_bJoin = INSTANCE(CGameLogic)->getJoinGame();
        //玩家是否参与
        if (INSTANCE(CGameLogic)->getJoinGame())
        {
            tagGameResult res = INSTANCE(CGameLogic)->getGameResult();
            rec.m_tagUserRecord.m_bWin = res.m_llTotal > 0;
        }
        INSTANCE(GameRecordMgr)->addGameRecord(rec);
    }
    
    this->refreshResult(m_enPointResult);
    
    //播放结束音效
    //玩家是否参与
    if (INSTANCE(CGameLogic)->getJoinGame())
    {
        if (INSTANCE(CGameLogic)->getGameResult().m_llTotal > 0)
        {
            INSTANCE(AudioMgr)->playBJLGameResAudio(2);
        }
        else if (INSTANCE(CGameLogic)->getGameResult().m_llTotal < 0)
        {
            INSTANCE(AudioMgr)->playBJLGameResAudio(1);
        }
        else
        {
            INSTANCE(AudioMgr)->playBJLGameResAudio(0);
        }
    }
    else
    {
        INSTANCE(AudioMgr)->playBJLGameResAudio(0);
    }
}

void GameCardLayer::noticeTips()
{
    if (m_nTotalCount > 4)
    {
        if (m_nDispatchedCount >= 4)
        {
            this->unschedule(SEL_SCHEDULE(&GameCardLayer::dispatchUpdate));
            this->schedule(SEL_SCHEDULE(&GameCardLayer::dispatchUpdate), 1.0f, CC_REPEAT_FOREVER, 2.0f);
        }
        
        tagCards idleCards = m_nodeCards[1]->getHandCards();
        INSTANCE(CGameLogic)->SortCardList(idleCards.m_cardData, idleCards.m_cardCount, ST_ORDER);
        BYTE idlePoint = INSTANCE(CGameLogic)->GetCardListPip(idleCards.m_cardData, idleCards.m_cardCount);
        
        tagCards masterCards = m_nodeCards[0]->getHandCards();
        INSTANCE(CGameLogic)->SortCardList(masterCards.m_cardData, masterCards.m_cardCount, ST_ORDER);
        BYTE masterPoint = INSTANCE(CGameLogic)->GetCardListPip(masterCards.m_cardData, masterCards.m_cardCount);
        
        std::string str = "";
        char buf[128] = "";
        if (m_nDispatchedCount == 4)
        {
            if (idleCards.m_cardCount == 2 && (6 == idlePoint || 7 == idlePoint))
            {
                sprintf(buf, "闲前两张 %d 点, 庄 %d 点, 庄继续拿牌", idlePoint, masterPoint);
                str = buf;
            }
            else if (idleCards.m_cardCount == 2 && 6 > idlePoint)
            {
                sprintf(buf, "闲 %d 点, 庄 %d 点, 闲继续拿牌", idlePoint, masterPoint);
                str = buf;
            }
            else if (idleCards.m_cardCount == 2
                     && (3 <= masterPoint && masterPoint <= 5))
            {
                sprintf(buf, "闲不补牌, 庄 %d 点, 庄继续拿牌", masterPoint);
                str = buf;
            }
        }
        else if (5 == m_nDispatchedCount)
        {
            if (idleCards.m_cardCount == 3
                && masterCards.m_cardCount == 2
                && 6 == m_nTotalCount)
            {
                BYTE cbValue = INSTANCE(CGameLogic)->GetCardPip(m_nodeCards[1]->getHandCards().m_cardData[2]);
                sprintf(buf, "闲第三张牌 %d 点, 庄 %d 点, 庄继续拿牌", cbValue, masterPoint);
                str = buf;
            }
        }
        
        if ("" != str)
        {
            auto text = Text::create(str, "Arial", 20);
            text->setTextColor(Color4B::YELLOW);
            text->setPosition(Vec2(m_tipsView->getViewSize().width * 0.5, 0));
            m_tipsView->addScrollViewCell(text);
        }
    }
}