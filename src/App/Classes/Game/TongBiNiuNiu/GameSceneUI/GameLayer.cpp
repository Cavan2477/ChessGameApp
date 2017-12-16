//
//  GameLayer.cpp
//  GameProject
//
//  Created by zhong on 4/19/16.
//
//

#include "GameLayer.h"
#include "GameDataMgr.h"
#include "UtilEx.h"

USING_TBNN_NAMESPACE;
USING_NS_CC;
using namespace ui;
enum enGameLayerTag
{
    kBackBtn_Tag = 100,
    kSetBtn_Tag,
    kTruBtn_Tag,
    kChatBtn_Tag,
    kReadyBtn_Tag,
    
    kSDCP_Tag,
    kZDCP_Tag,
    kTanPai_Tag,
    kTiShi_Tag,
};

enum enGameLayerZOrder
{
    //扑克
    GAME_CARDS_ZORDER,
    //玩家
    GAME_USER_ZORDER,
    //游戏控制层
    GAME_CONTROL_ZORDER,
    //聊天
    GAME_CHAT_ZORDER,
    //用户信息
    GAME_USER_INFO,
    //托管
    GAME_ROBOT_ZORDER
};

GameLayer::GameLayer():
m_bDynamicJoin(false),
m_bAutoShowCards(true),
m_settingLayer(nullptr),
m_gameChat(nullptr),
m_bGameTru(false),
m_gameTru(nullptr),
m_roleInfo(nullptr),
m_bMyOpenCards(false)
{
    
}

GameLayer::~GameLayer()
{
    HallDataMgr::getInstance()->m_bStartGame = false;
}

bool GameLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        LOAD_CSB("game/GameLayer.csb");
        
        INSTANCE(GameDataMgr)->setIsExit(false);
        HallDataMgr::getInstance()->m_bStartGame = true;
        
        //初始化通用按钮
        initBtnLayout();
        
        //游戏玩家
        initUserInfo();
        
        //游戏扑克
        initCardsInfo();
        
        //游戏控制
        initGameControLayout();
        
        //游戏特效
        initGameEffect();
        
        //游戏托管
        m_gameTru = static_cast<Layout*>(m_root->getChildByName("tru_control"));
        CC_ASSERT(m_gameTru != nullptr);
        m_gameTru->setLocalZOrder(GAME_ROBOT_ZORDER);
        m_gameTru->addTouchEventListener([this](cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
                                             {
                                                 if (Widget::TouchEventType::ENDED == type)
                                                 {
                                                     if (m_gameTru->isVisible())
                                                     {
                                                         m_bGameTru = false;
                                                         m_gameTru->setVisible(false);
                                                     }
                                                 }
                                             });
        m_gameTru->setVisible(false);
        bRes = true;
    } while (false);
    return bRes;
}

void GameLayer::onSelectedCards(const tagCards &cards)
{
    
}

void GameLayer::onCardsStateChange(const BYTE &cardData, bool bSelect)
{
    
}

bool GameLayer::uiMsg(const int &main, const int &sub, void *pData, const int &nLen)
{
    if (UI_MSG_BASE::UI_MSG_GAME == main)
    {
        switch (sub) {
            case GAME_UI::ROLE_INFO:
            {
                DWORD roleId = *((DWORD*)pData);
                //防作弊模式，不查看用户信息
                //屏蔽非本人信息查看
                if(INSTANCE(GameDataMgr)->getEnterAntiCheat()
                   || roleId != HallDataMgr::getInstance()->m_dwUserID)
                {
                    return true;
                }
                
                if (nullptr == m_roleInfo)
                {
                    m_roleInfo = GameRoleInfoLayer::create();
                    m_root->addChild(m_roleInfo,GAME_USER_INFO);
                }
                
                UserData *pUser = HallDataMgr::getInstance()->getUserData(roleId);
                if (nullptr != pUser)
                {
                    m_roleInfo->setLocalZOrder(GAME_USER_INFO);
                    m_roleInfo->refresh(pUser);
                }
            }
                break;
            case GAME_UI::TEXT_CHAT:
            {
                std::string msg = *((std::string*)pData);
                showGameTextChat(HallDataMgr::getInstance()->m_wChairID, msg.c_str());
            }
                break;
            case GAME_UI::BROW_CHAT:
            {
                int idx = *((int*)pData) + 1;
                showGameBrowChat(HallDataMgr::getInstance()->m_wChairID, idx);
            }
                break;
            default:
                break;
        }
    }
    return true;
}

void GameLayer::reSet()
{
    CUtilEx::enableBtn(m_btnReady, true, false);
    m_clock->stopCountDown();
    reSetGameEffect();
    m_bMyOpenCards = false;
    
    if (nullptr != m_gameEffect)
    {
        m_gameEffect->showGameTip();
    }
}

void GameLayer::reSetNewGame()
{
    reSetCardsInfo();
    reSetUserInfo();
}

void GameLayer::clearUser(bool bClear)
{
    
}

void GameLayer::countDownCallBack(cocos2d::Node *sender, const tbnn_game::en_tbnn_CountDownTag &tag)
{
    log("倒计时结束 %d", tag);
    switch (tag)
    {
        case en_tbnn_CountDownTag::tbnn_kREADY_COUNTDOWN:
        {
            //直接退出房间
            //INSTANCE(GameDataMgr)->setEnterNormalRoom(false);
            if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
            {
                INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
            }
        }
            break;
        case en_tbnn_CountDownTag::tbnn_kOPENCARD_COUNTDOWN:
        {
            gameTrusteeship(true);
        }
            break;
        default:
            break;
    }
}

//MARK::网络消息
void GameLayer::updateOnGameStateFree()
{
    CUtilEx::enableBtn(m_btnReady, true, false);
}

void GameLayer::updateUserStatus(UserData *pUser, const WORD &wChair)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    tagMobileUserInfoHead info = pUser->m_date;
    //用户状态
    bool bLeave = info.cbUserStatus == US_FREE;
    bool bReady = info.cbUserStatus == US_READY;
    bool bGame = info.cbUserStatus == US_PLAYING;
    
    int nTag = info.dwUserID;
    int nChair = info.wChairID;
    SCORE score = info.lScore;
    
    UILayer *notify = this;
    if (bLeave)
    {
        if (viewChair != tbnn_kGameChair0)
        {
            GameRoleNode *tmp = static_cast<GameRoleNode*>(m_root->getChildByTag(nTag));
            if (nullptr == tmp)
            {
                return;
            }
            notify = nullptr;
            viewChair = INSTANCE(CGameLogic)->switchToGameChair(tmp->getChairID());
            nTag = -1;
            nChair = -1;
        }
        
        if (INSTANCE(CGameLogic)->isValidChair(viewChair))
        {
            if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
            {
                m_nodeGameRole[viewChair]->updateRoleInfo(info.dwUserID,"玩家",0,bLeave);
            }
            else
            {
                m_nodeGameRole[viewChair]->updateRoleInfo(info.dwUserID,pUser->m_nickname.c_str(),score,bLeave);
            }
            m_spReady[viewChair]->setVisible(false);
            m_nodeCards[viewChair]->removeAllCards();
        }
    }
    
    if (viewChair == tbnn_kGameChair0 && !m_bDynamicJoin)
    {
        if (!bReady &&  !bGame)
        {
            //定时器
            m_clock->startCountDown(tbnn_COUNTDOWN_READY, CC_CALLBACK_2(GameLayer::countDownCallBack, this), tbnn_kREADY_COUNTDOWN);
        }
        else
        {
            m_clock->stopCountDown();
        }
    }
    if (!INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        return;
    }
    
    m_nodeGameRole[viewChair]->setTag(nTag);
    m_nodeGameRole[viewChair]->setChairID(nChair);
    m_nodeGameRole[viewChair]->setNotify(notify);
    //不同桌
    if (info.wTableID != HallDataMgr::getInstance()->m_wTableID)
    {
        /*m_nodeGameRole[viewChair]->reSet(false);
        m_nodeGameRole[viewChair]->setTag(-1);
        m_nodeGameRole[viewChair]->setChairID(-1);
        m_nodeGameRole[viewChair]->setNotify(nullptr);*/
        return;
    }
    
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        m_nodeGameRole[viewChair]->updateRoleInfo(info.dwUserID,"玩家",0,bLeave);
    }
    else
    {
        m_nodeGameRole[viewChair]->updateRoleInfo(info.dwUserID,pUser->m_nickname.c_str(),score,bLeave);
    }
    
    //准备
    m_spReady[viewChair]->setVisible(bReady);
}

void GameLayer::updateOnGame(bool bOnGame)
{
    
}

void GameLayer::updateGameBanker(const WORD &wBanker)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wBanker);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        m_nodeGameRole[viewChair]->switchToBanker(true);
    }
}

void GameLayer::updateUserCards(const WORD &wChair, const tagCards &cards)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        tagCards tmpCards = cards;
        INSTANCE(CGameLogic)->SortCardList(tmpCards.m_cardData, tmpCards.m_cardCount);
        
        if (tbnn_kGameChair0 == viewChair)
        {
            m_nodeCards[viewChair]->updateCardsNode(viewChair, tmpCards, m_bAutoShowCards);
        }
        else
        {
            m_nodeCards[viewChair]->updateCardsNode(viewChair, tmpCards, false);
        }
    }
}

void GameLayer::updateUserCardsData(const WORD &wChair, const tagCards &cards)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        tagCards tmpCards = cards;
        INSTANCE(CGameLogic)->SortCardList(tmpCards.m_cardData, tmpCards.m_cardCount);
        
        m_nodeCards[viewChair]->updateCardsData(tmpCards);
    }
}

void GameLayer::reEnterUpdateUserCards(const WORD &wChair, const tagCards &cards)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        tagCards tmpCards = cards;
        INSTANCE(CGameLogic)->SortCardList(tmpCards.m_cardData, tmpCards.m_cardCount);
        
        if (tbnn_kGameChair0 == viewChair)
        {
            m_nodeCards[viewChair]->arrangeCardsNodeWithUpdate(viewChair, tmpCards, m_bAutoShowCards, false);
        }
        else
        {
            m_nodeCards[viewChair]->arrangeCardsNodeWithUpdate(viewChair, tmpCards, false, false);
        }
        
    }
}

void GameLayer::updateCardsOver()
{
    Vector<FiniteTimeAction*> acts;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < tbnn_GAME_PLAYER; ++j)
        {
            auto call = CallFunc::create([=]()
                                         {
                                             m_nodeCards[j]->dispatchCardBySingle();
                                         });
            auto delay = DelayTime::create(0.05f);
            acts.pushBack(Sequence::create(call, delay, NULL));
        }
    }
    auto callFunc = CallFunc::create([=]()
                                     {
                                         CUtilEx::enableBtn(m_btnTp, true, false);
                                         CUtilEx::enableBtn(m_btnTiShi, true, false);
                                         
                                         //倒计时
                                         m_clock->stopCountDown();
                                         m_clock->startCountDown(tbnn_COUNTDOWN_OPENCARD, CC_CALLBACK_2(GameLayer::countDownCallBack, this), tbnn_kOPENCARD_COUNTDOWN);
                                         
                                         m_bMyOpenCards = true;
                                         
                                         for (int j = 0; j < tbnn_GAME_PLAYER; ++j)
                                         {
                                             m_nodeCards[j]->dispatchCardOver();
                                         }
                                     });
    acts.pushBack(callFunc);
    auto seq = Sequence::create(acts);
    this->stopAllActions();
    this->runAction(seq);
}

void GameLayer::updateUserOpenCard(const WORD &wChair)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        m_nodeGameRole[viewChair]->switchToOpenCard(true);
    }
}

void GameLayer::updateUserScore(const WORD &wChair, LONGLONG llscore)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        m_nodeGameRole[viewChair]->refreshScore(llscore);
    }
}

void GameLayer::updateGameOver()
{
    reSet();
    
    for (int j = 0; j < tbnn_GAME_PLAYER; ++j)
    {
        m_nodeCards[j]->showCardsNiuNiu();
    }
    
    m_gameTru->setVisible(false);
    m_bGameTru = false;
    
    INSTANCE(GameDataMgr)->setIsRoundOver(true);
}

void GameLayer::updateFiveKing(const WORD &wChair, const bool &bFiveKing)
{
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        if (bFiveKing)
        {
            m_nodeGameRole[viewChair]->showCaiJinAnimation();
        }
    }
}

void GameLayer::showGameTextChat(const WORD &wChair, const char *str)
{
    //防作弊不聊天
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        return;
    }
    
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        m_nodeGameRole[viewChair]->textChat(viewChair, str);
    }
}

void GameLayer::showGameBrowChat(const WORD &wChair, const int &idx)
{
    //防作弊不聊天
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        return;
    }
    
    enGameChairIndex viewChair = INSTANCE(CGameLogic)->switchToGameChair(wChair);
    if (INSTANCE(CGameLogic)->isValidChair(viewChair))
    {
        m_nodeGameRole[viewChair]->browChat(viewChair, idx);
    }
}

void GameLayer::openCard(const BYTE &cbOx, const tagCards &cards)
{
    CMD_C_OxCard cmd = {};
    cmd.bOX = cbOx;
    memcpy(&cmd.cbOxCardData, cards.m_cardData, tbnn_MAX_COUNT * sizeof(BYTE));
    
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, tbnn_SUB_C_OPEN_CARD, &cmd, sizeof(CMD_C_OxCard));
}

//MARK::网络消息
void GameLayer::quit(bool bCheck /*= true*/)
{
    bool bHandle = false;
    if (m_notify)
    {
        bHandle = m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME,GAME_UI::EXIT_GAME,&bCheck,sizeof(bool));
    }
}

void GameLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == tType)
    {
        INSTANCE(AudioMgr)->playClickAudio();
        switch (pWidget->getTag())
        {
            case kBackBtn_Tag:
            {
                if (m_bDynamicJoin)
                {
                    quit(false);
                }
                else
                {
                    bool bOnGame = m_cbGameStatus == en_TBNN_GameState::TBNN_kGamePlay || m_cbGameStatus == en_TBNN_GameState::TBNN_kGameStart;
                    quit(bOnGame);
                }
            }
                break;
            case kSetBtn_Tag:
            {
                if (nullptr == m_settingLayer)
                {
                    m_settingLayer = SettingLayer::create();
                    m_root->addChild(m_settingLayer,GAME_CONTROL_ZORDER);
                }
                m_settingLayer->showLayer(true);
                //updateFiveKing(HallDataMgr::getInstance()->m_wChairID, true);
                /*
                tagCards cards = tagCards();
                cards.m_cardCount = 5;
                cards.m_cardData[0] = 0x4E;
                cards.m_cardData[1] = 0x4F;
                cards.m_cardData[2] = 0x01;
                cards.m_cardData[3] = 0x02;
                cards.m_cardData[4] = 0x03;
                BYTE ox = INSTANCE(CGameLogic)->GetOxCard(cards.m_cardData, cards.m_cardCount);
                log("ox %d",ox);
                 */
            }
                break;
            case kTruBtn_Tag:
            {
                gameTrusteeship(true);
            }
                break;
            case kChatBtn_Tag:
            {
                //防作弊不聊天
                if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    return;
                }
                if (nullptr == m_gameChat)
                {
                    //聊天界面
                    m_gameChat = GameChatLayer::create();
                    CC_ASSERT( m_gameChat != nullptr );
                    m_gameChat->setNotify(this);
                    m_root->addChild(m_gameChat,GAME_CHAT_ZORDER);
                }
                m_gameChat->showGameChat(true);
            }
                break;
            case kReadyBtn_Tag:
            {
                m_cbGameStatus = en_TBNN_GameState::TBNN_kGameFree;
                if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    INSTANCE(GameDataMgr)->setIsExit(true);
                }
                
                CUtilEx::enableBtn(m_btnReady, false, true);
                
                INSTANCE(GameDataMgr)->requestReady(true);
                if (INSTANCE(GameDataMgr)->getIsRoundOver())
                {
                    INSTANCE(GameDataMgr)->setIsRoundOver(false);
                    //界面资源清理（针对重来）
                    this->reSetNewGame();
                }
            }
                break;
            case kSDCP_Tag:
            {
                m_bAutoShowCards = false;
                
                m_btnSdcp->setOpacity(125);
                m_btnZdcp->setOpacity(255);
            }
                break;
            case kZDCP_Tag:
            {
                m_bAutoShowCards = true;
                
                m_btnZdcp->setOpacity(125);
                m_btnSdcp->setOpacity(255);
            }
                break;
            case kTanPai_Tag:
            {
                m_gameEffect->showGameTip(GameEffectLayer::emGameTip::kGAME_TIP_OUTCARD);
                
                tagCards cards = m_nodeCards[tbnn_kGameChair0]->getHandCards();
                BYTE ox = INSTANCE(CGameLogic)->GetOxCard(cards.m_cardData, cards.m_cardCount);
                openCard(ox, cards);
                
                CUtilEx::enableBtn(m_btnTp, false, true);
                CUtilEx::enableBtn(m_btnTiShi, false, true);
                
                m_clock->stopCountDown();
            }
                break;
            case kTiShi_Tag:
            {
                m_nodeCards[tbnn_kGameChair0]->showNiuNiuNode();
            }
                break;
            default:
                break;
        }
    }
}

void GameLayer::initBtnLayout()
{
    auto btnLayout = m_root->getChildByName("btn_layout");
    CC_ASSERT(nullptr != btnLayout);
    
    //退出按钮
    Button *btn = static_cast<Button*>(btnLayout->getChildByName("back_btn"));
    CC_ASSERT(btn != nullptr);
    btn->setTag(kBackBtn_Tag);
    btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //设置按钮
    btn = static_cast<Button*>(btnLayout->getChildByName("set_btn"));
    CC_ASSERT(btn != nullptr);
    btn->setTag(kSetBtn_Tag);
    btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //托管
    btn = static_cast<Button*>(btnLayout->getChildByName("tru_btn"));
    CC_ASSERT(btn != nullptr);
    btn->setTag(kTruBtn_Tag);
    btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //聊天
    btn = static_cast<Button*>(btnLayout->getChildByName("chat_btn"));
    CC_ASSERT(btn != nullptr);
    btn->setTag(kChatBtn_Tag);
    btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //准备
    m_btnReady = static_cast<Button*>(btnLayout->getChildByName("ready_btn"));
    CC_ASSERT(m_btnReady != nullptr);
    m_btnReady->setTag(kReadyBtn_Tag);
    m_btnReady->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    CUtilEx::enableBtn(m_btnReady, false, true);
}

void GameLayer::initUserInfo()
{
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        m_nodeGameRole[i] = nullptr;
    }
    
    Size rootSize = m_root->getContentSize();
    m_pos[tbnn_kGameChair0] = Vec2(370, 100);
    m_pos[tbnn_kGameChair1] = Vec2(1066, 272);
    m_pos[tbnn_kGameChair2] = Vec2(1066, 462);
    m_pos[tbnn_kGameChair3] = Vec2(418, 545);
    m_pos[tbnn_kGameChair4] = Vec2(70, 462);
    m_pos[tbnn_kGameChair5] = Vec2(70, 272);
    
    auto layout = m_root->getChildByName("userinfo_control");
    char buf[32] = "";
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        m_nodeGameRole[i] = GameRoleNode::create();
        CC_ASSERT(m_nodeGameRole[i] != nullptr);
        m_nodeGameRole[i]->setPosition(m_pos[i].x,m_pos[i].y);
        m_root->addChild(m_nodeGameRole[i],GAME_USER_ZORDER);
        
        sprintf(buf, "ready_%d", i);
        m_spReady[i] = static_cast<Sprite*>(layout->getChildByName(buf));
        CC_ASSERT(m_spReady[i] != nullptr);
    }
    layout->setLocalZOrder(GAME_USER_ZORDER);
    
    m_clock = GameClockNode::create();
    CC_ASSERT( m_clock != nullptr );
    m_clock->setPosition(Vec2(rootSize.width * 0.5,rootSize.height * 0.6));
    m_root->addChild(m_clock,GAME_CONTROL_ZORDER);
    
    reSetUserInfo();
}

void GameLayer::reSetUserInfo()
{
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        m_nodeGameRole[i]->reSet(-1 != m_nodeGameRole[i]->getTag());
        m_spReady[i]->setVisible(false);
    }
}

void GameLayer::initCardsInfo()
{
    Size rootSize = m_root->getContentSize();
    
    Vec2 pos[6];
    pos[tbnn_kGameChair0] = Vec2(630,100);
    pos[tbnn_kGameChair1] = Vec2(890,255);
    pos[tbnn_kGameChair2] = Vec2(890,445);
    pos[tbnn_kGameChair3] = Vec2(610,530);
    pos[tbnn_kGameChair4] = Vec2(246,445);
    pos[tbnn_kGameChair5] = Vec2(246,255);
    
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        m_nodeCards[i] = nullptr;
        m_nodeCards[i] = CardsNode::createEmptyCardsNode();
        CC_ASSERT(m_nodeCards[i] != nullptr);
        m_nodeCards[i]->setPosition(pos[i]);
        m_root->addChild(m_nodeCards[i]);
    }
    m_nodeCards[tbnn_kGameChair0]->setSelectCardsListen(this);
    
    reSetCardsInfo();
}

void GameLayer::reSetCardsInfo()
{
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        m_nodeCards[i]->removeAllCards();
        m_nodeCards[i]->setLocalZOrder(GAME_CARDS_ZORDER);
    }
}

void GameLayer::initGameControLayout()
{
    m_gameControlLayout = static_cast<Layout*>(m_root->getChildByName("game_control"));
    CC_ASSERT(m_gameControlLayout != nullptr);
    
    //手动搓牌
    m_btnSdcp = static_cast<Button*>(m_gameControlLayout->getChildByName("btn_sdcp"));
    CC_ASSERT(m_btnSdcp != nullptr);
    m_btnSdcp->setTag(kSDCP_Tag);
    m_btnSdcp->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //自动搓牌
    m_btnZdcp = static_cast<Button*>(m_gameControlLayout->getChildByName("btn_zdcp"));
    CC_ASSERT(m_btnZdcp != nullptr);
    m_btnZdcp->setTag(kZDCP_Tag);
    m_btnZdcp->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //摊牌
    m_btnTp = static_cast<Button*>(m_gameControlLayout->getChildByName("btn_tp"));
    CC_ASSERT(m_btnTp != nullptr);
    m_btnTp->setTag(kTanPai_Tag);
    m_btnTp->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    //提示
    m_btnTiShi = static_cast<Button*>(m_gameControlLayout->getChildByName("btn_tishi"));
    CC_ASSERT(m_btnTiShi != nullptr);
    m_btnTiShi->setTag(kTiShi_Tag);
    m_btnTiShi->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    reSetGameControlLayout();
}

void GameLayer::reSetGameControlLayout()
{
    CUtilEx::enableBtn(m_btnTp, false, true);
    CUtilEx::enableBtn(m_btnTiShi, false, true);
    
    if (m_bAutoShowCards)
    {
        m_btnZdcp->setOpacity(125);
        m_btnSdcp->setOpacity(255);
    }
    else
    {
        m_btnSdcp->setOpacity(125);
        m_btnZdcp->setOpacity(255);
    }
}

void GameLayer::initGameEffect()
{
    m_gameEffect = GameEffectLayer::create();
    m_root->addChild(m_gameEffect);
    
    reSetGameEffect();
}

void GameLayer::reSetGameEffect()
{
    m_gameEffect->showGameTip();
}

void GameLayer::gameTrusteeship(bool bTrusteeship)
{
    m_bGameTru = bTrusteeship;
    m_gameTru->setVisible(m_bGameTru);
    
    if (m_bGameTru)
    {
        if(m_bMyOpenCards)
        {
            m_bMyOpenCards = false;
            
            tagCards cards = m_nodeCards[tbnn_kGameChair0]->getHandCards();
            BYTE ox = INSTANCE(CGameLogic)->GetOxCard(cards.m_cardData, cards.m_cardCount);
            openCard(ox, cards);
            
            CUtilEx::enableBtn(m_btnTp, false, true);
            CUtilEx::enableBtn(m_btnTiShi, false, true);
            
            m_clock->stopCountDown();
        }
    }
}