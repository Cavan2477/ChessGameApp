//
//  GameLayer.cpp
//  DouDiZhu
//
//  Created by zhong on 1/20/16.
//
//

#include "GameLayer.h"
#include "GameDataMgr.h"
#include "AnimationMgr.h"
#include "../DataMgr/GameRecordMgr.h"

USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

enum emGameLayer
{
    TAG_BASE = 1000,
    //聊天
    TAG_CHART_MENU,
    //机器人
    TAG_ROBOT_MENU,
    //设置
    TAG_SETTING_MENU,
    //退出
    TAG_QUIT_MENU,
    //换桌按钮
    TAG_CHDESK_MENU,
    //准备按钮
    TAG_START_MENU,
    //不出按钮
    TAG_PASS_MENU,
    //亮牌按钮
    TAG_SHOWCARD_MENU,
    //提示按钮
    TAG_SUGGEST_MENU,
    //上庄列表
    TAG_BANKERLIST_MENU,
    //银行按钮
    TAG_BANK_BTN,
    //路单按钮
    TAG_LUDAN_BTN
};

//场景Z轴顺序
enum emGameZorder
{
    //准备菜单
    GAME_READY_ZORDER,
    //顶部菜单
    GAME_TOPMENU_ZORDER,
    //游戏控制层
    GAME_CONTROL_ZORDER,
    //玩家
    GAME_USER_ZORDER,
    //特效
    GAME_EFFECT_ZORDER,
    //玩家信息
    GAME_USER_INFO,
    //上庄列表
    GAME_BANKERLIST_ZORDER,
    //游戏结算
    GAME_RESULT_ZORDER,
    //扑克
    GAME_CARDS_ZORDER,
    //路单
    GAME_WALLBILL_ZORDER,
    //倒计时
    GAME_CLOCK_ZORDER,
    //银行、设置窗口()
    OTHER_WINDOW_ZORDER,
    //聊天
    GAME_CHAT_ZORDER,
    //托管
    GAME_ROBOT_ZORDER,
};
GameLayer::GameLayer():
m_notify(nullptr),
m_bOnGame(false),
m_settingLayer(nullptr),
m_antiCheat(nullptr),
m_roleInfo(nullptr),
m_bTrusteeship(false),
m_enApplyState(kCancelState)
{
    //延迟加载
    m_gameChat = nullptr;
    m_gameResultLayer = nullptr;
    m_cardLayer = nullptr;
    m_pUserHead = nullptr;
    m_applyList = nullptr;
    m_pBankerNode = nullptr;
    m_bankLayer = nullptr;
    m_simpleWallBill = nullptr;
    m_wallBill = nullptr;
    
    m_nJettonSelect = -1;
    m_lHaveJetton = 0;
    m_llMaxJetton = 0;
}

GameLayer::~GameLayer()
{
    m_actCallTip->release();
    /*for (int i = 0; i < GAME_PLAYER; ++i)
    {
        m_actScoreShow[i]->release();
    }*/
    
    HallDataMgr::getInstance()->m_bStartGame = false;
    
    INSTANCE(GameRecordMgr)->clearRecord();
}

bool GameLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        //
        LOAD_CSB("game/GameLayer.csb");
        
        INSTANCE(GameDataMgr)->setIsExit(false);
        HallDataMgr::getInstance()->m_bStartGame = true;

        //获取玩家携带游戏币
    	m_scoreUser = HallDataMgr::getInstance()->m_UserScore;
        
        //顶部菜单
        this->initTopMenu();
        
        //下注区域
        this->initJettonAreaInfo();
        
        //下注信息
        this->initJettonInfo();
        
        //下注按钮
        this->initJettonBtnInfo();
        
        //托管
        this->initRobotLayout();
        
        //游戏特效
        this->initGameEffect();
        
        //通用动作
        this->initAction();
        
        auto tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/room_single_win.png");
        CC_ASSERT(tex != nullptr);
        SpriteFrame *frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 26, 25));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/room_single_win.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/room_single_win_1.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 26, 25));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/room_single_win_1.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/room_single_lose.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 26, 25));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/room_single_lose.png");
        
        srand(time(nullptr));
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

void GameLayer::onCardsOut(CardsNode *sender, const tagCards &outCards, const tagCards &handCards)
{
    enGameRoleDir dir = sender->getDir();
    if (handCards.m_cardCount <= 2 && nullptr != m_gameEffect)
    {
        //警报动画
        m_gameEffect->showAlarmEffect(dir);
    }
}

bool GameLayer::uiMsg(const int &main, const int &sub, void *pData, const int &nLen)
{
    if (UI_MSG_BASE::UI_MSG_GAME == main)
    {
        switch (sub) {
            case GAME_UI::ROLE_INFO:
            {
                //防作弊模式，不查看用户信息
                if(INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    return true;
                }
                
                if (nullptr == m_roleInfo)
                {
                    m_roleInfo = GameRoleInfoLayer::create();
                    m_root->addChild(m_roleInfo);
                }
                DWORD roleId = *((DWORD*)pData);
                UserData *pUser = HallDataMgr::getInstance()->getUserData(roleId);
                if (nullptr != pUser)
                {
                    m_roleInfo->setLocalZOrder(GAME_USER_INFO);
                    m_roleInfo->refresh(pUser, INSTANCE(GameDataMgr)->getRoleDir(pUser->m_date.wChairID));
                }
            }
                break;
            case GAME_UI::TEXT_CHAT:
            {
                std::string msg = *((std::string*)pData);
                showGameTextChat(kMY_ROLE, msg.c_str());
            }
                break;
            case GAME_UI::BROW_CHAT:
            {
                int idx = *((int*)pData) + 1;
                showGameBrowChat(kMY_ROLE, idx);
            }
                break;
            case GAME_UI::CONTINUE:
            {
                if (nullptr != m_gameResultLayer)
                {
                    m_gameResultLayer->hideGameResult();
                }
                //准备
                INSTANCE(GameDataMgr)->requestReady(true);
                
                //界面资源清理（针对重来）
                this->reSetNewGame();
            }
                break;
            case GAME_UI::EXIT_GAME:
            {
                if (nullptr != m_gameResultLayer)
                {
                    m_gameResultLayer->hideGameResult();
                }
                quit(false);
            }
                break;
            case GAME_UI::APPLY_BANKER:
            {
                enApplyState enState = *((enApplyState*)pData);
                applyBanker(enState);
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
    m_cbGameTimes = 0;
    //重置游戏特效
    reSetGameEffect();
    //重置下注按钮
    reSetJettonBtnInfo();
}

void GameLayer::reSetNewGame()
{
    //重置下注区域
    reSetJettonAreaInfo();
    reSetJettonInfo();
    //游戏卡牌
    if (nullptr != m_cardLayer)
    {
        m_cardLayer->showLayer(false);
    }
    //重置游戏结算
    showGameResult(false);
}

void GameLayer::saveCurrentGameUser()
{
    
}

void GameLayer::clearAddScoreState()
{
    
}

void GameLayer::onGameStart()
{
    if (nullptr != m_gameEffect)
    {
        m_gameEffect->showGameTip();
    }
    m_nJettonSelect = -1;
    m_lHaveJetton = 0;
    //获取玩家携带游戏币
    m_scoreUser = HallDataMgr::getInstance()->m_UserScore;
    
    //开始音效
    INSTANCE(AudioMgr)->playGameStartAudio();
    
    //下注
    enableJetton(true);
    
    //调整下注按钮
    adjustJettonBtn();
}

void GameLayer::reEnterStart( LONGLONG lUserJetton)
{
    m_lHaveJetton = lUserJetton;
    //下注
    enableJetton(true);
    
    //调整下注按钮
    adjustJettonBtn();
}

void GameLayer::updateOnGameStateFree()
{
    INSTANCE(CGameLogic)->setDynamicJoin(false);
    
    reSetNewGame();
    
    this->enableBtn(m_btnApply, HallDataMgr::getInstance()->m_UserScore > 10000000);
}

bool sortFun(tagApplyInfo a, tagApplyInfo b)
{
    return a.m_llIdx > b.m_llIdx;
}

void GameLayer::addApplyBankerUser(const tagApplyInfo &tagInfo)
{
    if (tagInfo.m_wChairId == HallDataMgr::getInstance()->m_wChairID)
    {
        updateApplyBtnState(kApplyState);
    }
    
    m_vecApplyInfo.push_back(tagInfo);
    
    std::sort(m_vecApplyInfo.begin(), m_vecApplyInfo.end(), sortFun);
    size_t len = m_vecApplyInfo.size();
    for (size_t i = 0; i < len; ++i)
    {
        m_vecApplyInfo[i].m_cbApplyNum = len - i;
    }
    
    if (nullptr != m_applyList && m_applyList->isVisible())
    {
        m_applyList->refresh(m_vecApplyInfo, m_enApplyState);
    }
}

void GameLayer::removeApplyBankerUser(const WORD &wChair)
{
    if (wChair == HallDataMgr::getInstance()->m_wChairID)
    {
        updateApplyBtnState(kCancelState);
    }
    std::vector<tagApplyInfo>::iterator ite = m_vecApplyInfo.begin();
    int j = 0;
    for (; ite != m_vecApplyInfo.end(); ++ite, ++j)
    {
        if (ite->m_wChairId == wChair)
        {
            m_vecApplyInfo.erase(ite);
            break;
        }
    }
    
    std::sort(m_vecApplyInfo.begin(), m_vecApplyInfo.end(), sortFun);
    size_t len = m_vecApplyInfo.size();
    for (size_t i = 0; i < len; ++i)
    {
        m_vecApplyInfo[i].m_cbApplyNum = len - i;
    }
    
    if (nullptr != m_applyList && m_applyList->isVisible())
    {
        m_applyList->refresh(m_vecApplyInfo, m_enApplyState);
    }
}

void GameLayer::updateUserStatus( UserData *pUser,const enGameRoleDir &dir)
{
    enGameRoleDir sitDir = dir;
    tagMobileUserInfoHead info = pUser->m_date;
    //用户状态
    bool bLeave = info.cbUserStatus == US_FREE;
    bool bSit = info.cbUserStatus == US_SIT;
    if (!bLeave)
    {
        if (!INSTANCE(GameDataMgr)->isValidDir(dir))
        {
            return;
        }
    }
    
    if (bLeave)
    {
        
    }
    
    //不同桌
    if (info.wTableID != HallDataMgr::getInstance()->m_wTableID)
    {
        return;
    }
    
    if (bSit)
    {
        if (kMY_ROLE != sitDir)
        {
            
        }
    }
}

void GameLayer::updateReadyDisplay(const enGameRoleDir &dir, bool bReady)
{
    if (!INSTANCE(GameDataMgr)->isValidDir(dir))
    {
        return;
    }
}

void GameLayer::updateOnGame(bool bOnGame, bool bValidStart)
{
    m_bOnGame = true;
}

void GameLayer::updateUserJetton(const WORD &wUser, const BYTE &cbArea,  LONGLONG llScore)
{
    int nIdx = getJettonIdx(llScore);
    if (nIdx > 0 && nIdx < 9)
    {
        char buf[64] = "";
        sprintf(buf, "room_chip_%d_0.png",nIdx);
        auto sp = Sprite::createWithSpriteFrameName(buf);
        auto btn = m_tagJettonLayout->getChildByTag(cbArea);
        if (nullptr != sp)
        {
            sp->setScale(0.5f);
            sp->setTag(wUser);
            sprintf(buf, "%lld",llScore);
            sp->setName(buf);
            
            if (nullptr != btn)
            {
                randomSetJettonPos(btn,sp);
                btn->addChild(sp);
            }
        }
        
        if (nullptr != btn)
        {
            if (nullptr == m_pJettonNode[cbArea])
            {
                m_pJettonNode[cbArea] = GameJettonNode::create();
                CC_ASSERT(m_pJettonNode[cbArea] != nullptr);
                m_pJettonNode[cbArea]->setPosition(btn->getPosition());
                m_tagJettonLayout->addChild(m_pJettonNode[cbArea]);
                m_pJettonNode[cbArea]->setTag(-1);
            }
            m_pJettonNode[cbArea]->refreshJetton(llScore, llScore, wUser == HallDataMgr::getInstance()->m_wChairID);
        }
    }
    INSTANCE(AudioMgr)->playBJLAddScoreAudio();
}

void GameLayer::reEnterGameJetton(const BYTE &cbArea,  LONGLONG llScore)
{
    auto btn = m_tagJettonLayout->getChildByTag(cbArea);
    if (nullptr == btn || 0 == llScore)
    {
        return;
    }
    char buf[64] = "";
    std::vector<tagJettonInfo> vec = calcuteJetton(llScore, false);
    
    for (int i = 0; i < vec.size(); ++i)
    {
        tagJettonInfo info = vec[i];
        for (int j = 0; j < info.m_cbCount; ++j)
        {
            sprintf(buf, "room_chip_%d_0.png",info.m_cbIdx);
            auto sp = Sprite::createWithSpriteFrameName(buf);
            if (nullptr != sp)
            {
                sp->setScale(0.5f);
                sp->setTag(-1);
                sprintf(buf, "%lld",info.m_llScore);
                sp->setName(buf);
                
                randomSetJettonPos(btn,sp);
                btn->addChild(sp);
            }
        }
    }
    if (nullptr == m_pJettonNode[cbArea])
    {
        m_pJettonNode[cbArea] = GameJettonNode::create();
        CC_ASSERT(m_pJettonNode[cbArea] != nullptr);
        m_pJettonNode[cbArea]->setPosition(btn->getPosition());
        m_tagJettonLayout->addChild(m_pJettonNode[cbArea]);
        m_pJettonNode[cbArea]->setTag(-1);
    }
    m_pJettonNode[cbArea]->refreshJetton(llScore, llScore, false);
}

void GameLayer::updateUserJettonFail(const WORD &wUser, const BYTE &cbArea,  LONGLONG llScore)
{
    if (wUser == HallDataMgr::getInstance()->m_wChairID)
    {
        char buf[128];
        sprintf(buf, "下注 %lld 失败", llScore);
        //提示
        m_gameEffect->showGameTip(GameEffectLayer::emGameTip::kGAME_TEXT_TIP, buf);
        
        //自己下注失败
        m_scoreUser += llScore;
        
        m_lHaveJetton -= llScore;
        
        adjustJettonBtn();
        
        refreshJetton();
        
        m_pJettonNode[cbArea]->refreshJetton(-llScore, -llScore, true);
        
        //移除界面元素
        auto btn = m_tagJettonLayout->getChildByTag(cbArea);
        if (nullptr != btn)
        {
            sprintf(buf, "%lld",llScore);
            btn->removeChildByName(buf);
        }
    }
}

void GameLayer::reEnterUserJetton(const BYTE &cbArea,  LONGLONG llScore)
{
    auto btn = m_tagJettonLayout->getChildByTag(cbArea);
    if (nullptr == btn || 0 == llScore)
    {
        return;
    }
    
    if (nullptr == m_pJettonNode[cbArea])
    {
        m_pJettonNode[cbArea] = GameJettonNode::create();
        CC_ASSERT(m_pJettonNode[cbArea] != nullptr);
        m_pJettonNode[cbArea]->setPosition(btn->getPosition());
        m_tagJettonLayout->addChild(m_pJettonNode[cbArea]);
        m_pJettonNode[cbArea]->setTag(-1);
    }
    //刷新自己的下注数额，总数额不变
    m_pJettonNode[cbArea]->refreshJetton(llScore, 0, true);
}

void GameLayer::updateChangeBanker(const WORD &wChairId,  LONGLONG llScore, const bool &bSystemBanker)
{
    if (wChairId == HallDataMgr::getInstance()->m_wChairID)
    {
        updateApplyBtnState(kApplyedState);
    }
    
    //将庄家从申请列表中去除
    removeApplyBankerUser(wChairId);
    
    //更新庄家信息
    if (nullptr == m_pBankerNode)
    {
        m_pBankerNode = BankerNode::create();
        CC_ASSERT(m_pBankerNode != nullptr);
        m_pBankerNode->setPosition(Vec2(568, 487));
        m_root->addChild(m_pBankerNode);
    }
    
    if (0 == llScore)
    {
        log("0000");
    }
    m_pBankerNode->refreshBanker(wChairId, llScore, bSystemBanker);
}

void GameLayer::updateBankerScore()
{
    //更新庄家信息
    if (nullptr == m_pBankerNode)
    {
        m_pBankerNode = BankerNode::create();
        CC_ASSERT(m_pBankerNode != nullptr);
        m_pBankerNode->setPosition(Vec2(568, 487));
        m_root->addChild(m_pBankerNode);
    }
    UserData* pUser = HallDataMgr::getInstance()->getUserDataByChair(m_pBankerNode->getBankerChair());
    if (nullptr != pUser)
    {
        LONGLONG ll = pUser->m_date.lScore;
        
        if (0 == ll)
        {
            log("0000");
        }
        m_pBankerNode->refreshBankerScore(ll);
    }
}

void GameLayer::updateGameOver()
{
    m_bOnGame = false;
    m_cbGameStatus = emGameState::kGAME_FREE;
    enableJetton(false);
    
    //游戏结束，重新开始
    reSet();
    //取消托管
    gameTrusteeship(false);
    
    //设置自己是否是地主
    INSTANCE(GameDataMgr)->setIsMineLander(false);
}

void GameLayer::updateWallBill()
{
    if (nullptr == m_simpleWallBill)
    {
        m_simpleWallBill = BJL_NAMESPACE_FUN(SimpleWallBillLayer)::createLayer(this);
        CC_ASSERT(m_simpleWallBill != nullptr);
        m_root->addChild(m_simpleWallBill);
    }
    m_simpleWallBill->refreshRecordList();
    
    if (nullptr != m_wallBill && m_wallBill->isVisible())
    {
        m_wallBill->refreshWallBillList();
    }
}

void GameLayer::updateCardsLayer(const tagCardsResult &tagRes, const bool &bAni, const BYTE &cbTime)
{
    if (nullptr == m_cardLayer)
    {
        m_cardLayer = GameCardLayer::createLayer(this);
        m_root->addChild(m_cardLayer, GAME_CARDS_ZORDER);
    }
    m_cardLayer->showLayer(true);
    m_cardLayer->refresh(tagRes, bAni, cbTime);
}

void GameLayer::showGameTextChat(const enGameRoleDir &dir, const char *str)
{
    //防作弊不聊天
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        return;
    }
    //m_nodeGameRole[dir]->textChat(dir, str);
}

void GameLayer::showGameBrowChat(const enGameRoleDir &dir, const int &idx)
{
    //防作弊不聊天
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        return;
    }
    //m_nodeGameRole[dir]->browChat(dir, idx);
}

void GameLayer::startCountDown(const BYTE &cbTime, const bjl_enCountDownTag &enTag)
{
    m_pClockNode->stopCountDown();
    m_pClockNode->startCountDown(cbTime, CC_CALLBACK_2(GameLayer::countDownCallBack, this), enTag);
}

void GameLayer::showGameTip(const GameEffectLayer::emGameTip &tip)
{
    m_gameEffect->showGameTip(tip);
}

void GameLayer::countDownCallBack(cocos2d::Node *sender, const bjl_enCountDownTag &tag)
{
    if ((int)kMY_ROLE == sender->getTag())
    {
        log("倒计时结束 %d",tag);
        switch (tag)
        {
            case bjl_enCountDownTag::bjl_kREADY_COUNTDOWN:
            {
                //直接退出房间
                //INSTANCE(GameDataMgr)->setEnterNormalRoom(false);
                if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
                }
            }
                break;
            case bjl_enCountDownTag::bjl_kGAMEOVER_COUNTDOWN:
            {
                if (nullptr != m_cardLayer)
                {
                    m_cardLayer->showLayer(false);
                }
                showGameResult(true);
                
                //更新路单列表
                updateWallBill();
            }
                break;
            default:
                break;
        }
    }
}

void GameLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    Button *pBtn = static_cast<Button*>(pSender);
    if (nullptr == pBtn)
    {
        return;
    }
    
    if (Widget::TouchEventType::ENDED == type)
    {
        switch (pBtn->getTag())
        {
            case TAG_QUIT_MENU:
            {
                INSTANCE(AudioMgr)->playClickAudio();
                
                m_bOnGame = true;
                if (m_cbGameStatus == emGameState::kGAME_FREE)
                {
                    m_bOnGame = false;
                }
                
                quit(m_bOnGame);
            }
                break;
            case TAG_SETTING_MENU:
            {
                INSTANCE(AudioMgr)->playClickAudio();
                if (nullptr == m_settingLayer)
                {
                    m_settingLayer = SettingLayer::create();
                    m_root->addChild(m_settingLayer,OTHER_WINDOW_ZORDER);
                }
                m_settingLayer->showLayer(true);
            }
                break;
            case TAG_ROBOT_MENU:
            {
                //游戏托管，本地处理
                gameTrusteeship(!m_bTrusteeship);
            }
                break;
            case TAG_CHART_MENU:
            {
                //防作弊不聊天
                if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    return;
                }
                INSTANCE(AudioMgr)->playClickAudio();
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
            case TAG_START_MENU:
            {
                m_cbGameStatus = emGameState::kGAME_FREE;
                if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    INSTANCE(GameDataMgr)->setIsExit(true);
                }
                
                INSTANCE(AudioMgr)->playClickAudio();
                //准备
                INSTANCE(GameDataMgr)->requestReady(true);
                
                if (INSTANCE(GameDataMgr)->getIsRoundOver())
                {
                    INSTANCE(GameDataMgr)->setIsRoundOver(false);
                    //界面资源清理（针对重来）
                    this->reSetNewGame();
                }
            }
                break;
            case TAG_CHDESK_MENU:
            {
                INSTANCE(AudioMgr)->playClickAudio();
                reSet();
                //换桌
                INSTANCE(GameDataMgr)->requestChangeDesk();
            }
                break;
            case TAG_SHOWCARD_MENU:
            {
                //出牌定时器停止
            }
                break;
            case TAG_SUGGEST_MENU:
            {
                
            }
                break;
            case TAG_PASS_MENU: //不出
            {
                
            }
                break;
            case TAG_BANKERLIST_MENU: //上庄列表
            {
                INSTANCE(AudioMgr)->playClickAudio();
                if (nullptr == m_applyList)
                {
                    m_applyList = ApplyListLayer::createLayer(this);
                    CC_ASSERT(m_applyList != nullptr);
                    m_root->addChild(m_applyList,GAME_BANKERLIST_ZORDER);
                }
                m_applyList->setVisible(true);
                m_applyList->refresh(m_vecApplyInfo, m_enApplyState);
            }
                break;
            case TAG_BANK_BTN: //银行按钮
            {
                INSTANCE(AudioMgr)->playClickAudio();
                if (nullptr == m_bankLayer)
                {
                    m_bankLayer = BankLayer::create();
                    m_bankLayer->setSceneMode(false);
                    m_root->addChild(m_bankLayer, OTHER_WINDOW_ZORDER);
                }
                m_bankLayer->showLayer(true);
            }
                break;
            case TAG_LUDAN_BTN:
            {
                INSTANCE(AudioMgr)->playClickAudio();
                if (nullptr == m_wallBill)
                {
                    m_wallBill = BJL_NAMESPACE_FUN(WallBillLayer)::createLayer(this);
                    CC_ASSERT(m_wallBill != nullptr);
                    m_root->addChild(m_wallBill,GAME_WALLBILL_ZORDER);
                }
                m_wallBill->refreshWallBillList();
            }
                break;
            default:
                break;
        }
    }
}

void GameLayer::touchJettonArea(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == type)
    {
        if (m_nJettonSelect < 0)
        {
            return;
        }
        LONGLONG lnum = m_nJettonSelect;
        int nArea = pWidget->getTag();
        m_lHaveJetton += lnum;
        if (m_lHaveJetton > m_llMaxJetton)
        {
            //提示
            m_gameEffect->showGameTip(GameEffectLayer::emGameTip::kGAME_TEXT_TIP, "已超过最大下注限额");
            
            m_lHaveJetton -= lnum;
            return;
        }
        
        //下注
        userJetton(nArea, lnum);
        updateUserJetton(HallDataMgr::getInstance()->m_wChairID, nArea, lnum);
        
        m_scoreUser -= lnum;
        
        refreshJetton();
        
        adjustJettonBtn();
    }
}

void GameLayer::touchJettonBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == type)
    {
        int nTag = pWidget->getTag();
        if (7 == pWidget->getTag()) //全下
        {
            m_nJettonSelect = HallDataMgr::getInstance()->m_UserScore;
        }
        else if (nTag >= 0 && nTag < 8)
        {
            m_nJettonSelect = m_pJettonNumber[nTag];
        }
    }
}

void GameLayer::touchApplyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == type)
    {
        enApplyState enState = (enApplyState)pWidget->getTag();
        applyBanker(enState);
    }
}

void GameLayer::initTopMenu()
{
    m_topMenuLayout = static_cast<Layout*>(m_root->getChildByName("top_layout"));
    CC_ASSERT( m_topMenuLayout != nullptr);
    Size topSize = m_topMenuLayout->getContentSize();
    
    //聊天按钮
    Button* csbBtn = static_cast<Button*>(m_topMenuLayout->getChildByName("chat_btn"));
    CC_ASSERT( csbBtn != nullptr);
    csbBtn->setTag(TAG_CHART_MENU);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    //机器人按钮
    csbBtn = static_cast<Button*>(m_topMenuLayout->getChildByName("tru_btn"));
    CC_ASSERT( csbBtn != nullptr);
    csbBtn->setTag(TAG_ROBOT_MENU);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    //设置按钮
    csbBtn = static_cast<Button*>(m_topMenuLayout->getChildByName("set_btn"));
    CC_ASSERT( csbBtn != nullptr);
    csbBtn->setTag(TAG_SETTING_MENU);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    //退出按钮
    m_btnQuit = static_cast<Button*>(m_topMenuLayout->getChildByName("back_btn"));
    CC_ASSERT( m_btnQuit != nullptr);
    m_btnQuit->setTag(TAG_QUIT_MENU);
    m_btnQuit->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    //上庄列表按钮
    csbBtn = static_cast<Button*>(m_topMenuLayout->getChildByName("bankerlist_btn"));
    CC_ASSERT( csbBtn != nullptr);
    csbBtn->setTag(TAG_BANKERLIST_MENU);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    //申请上庄按钮
    m_btnApply = static_cast<Button*>(m_topMenuLayout->getChildByName("apply_btn"));
    CC_ASSERT( m_btnApply != nullptr);
    m_btnApply->setTag(m_enApplyState);
    m_btnApply->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchApplyBtn,this));
    this->enableBtn(m_btnApply, false);
    //银行按钮
    csbBtn = static_cast<Button*>(m_topMenuLayout->getChildByName("bank_btn"));
    CC_ASSERT( csbBtn != nullptr);
    csbBtn->setTag(TAG_BANK_BTN);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    //路单按钮
    csbBtn = static_cast<Button*>(m_topMenuLayout->getChildByName("ludan_btn"));
    CC_ASSERT( csbBtn != nullptr);
    csbBtn->setTag(TAG_LUDAN_BTN);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    //头像背景
    auto tmpSp = m_topMenuLayout->getChildByName("head_bg");
    CC_ASSERT(tmpSp != nullptr);
    
    auto circleNode = DrawNode::create();
    const int maxTrangle = 360;
    Vec2 circleVec2[maxTrangle];
    for (int i = 0; i < maxTrangle; i ++)
    {
        float x = cosf( i * (M_PI/180.f)) * 60;
        float y = sinf( i * (M_PI/180.f)) * 60;
        circleVec2[i] = Vec2(x, y);
    }
    auto circleColor = Color4F(0, 1, 0, 1);
    circleNode->drawPolygon(circleVec2, maxTrangle, circleColor, 1, circleColor);
    
    //玩家头像
    auto clipNode = ClippingNode::create(circleNode);
    clipNode->setPosition(tmpSp->getPosition());
    m_topMenuLayout->addChild(clipNode);
    if (HallDataMgr::getInstance()->m_loadtype == Load_Normal || HallDataMgr::getInstance()->m_loadtype == Load_Visitor)
    {
        m_pUserHead = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                      HallDataMgr::getInstance()->m_wCustom,
                                                      HallDataMgr::getInstance()->m_dwUserID,
                                                      HallDataMgr::getInstance()->m_cbGender);
    }
    else if (HallDataMgr::getInstance()->m_loadtype == Load_RenRen || HallDataMgr::getInstance()->m_loadtype == Load_Sina)
    {
        m_pUserHead = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
    }
    m_pUserHead->setHeadSize(120);
    m_pUserHead->enableTouch(true,
                             [this](cocos2d::Touch * touch, cocos2d::Event * event)->bool
                             {
                                 Vec2 vec = touch->getLocation();
                                 vec = m_pUserHead->convertToNodeSpace(vec);
                                 
                                 Size nodeSize = m_pUserHead->getContentSize();
                                 Rect nodeRect = Rect(0,0,nodeSize.width,nodeSize.height);
                                 
                                 return nodeRect.containsPoint(vec);
                             },
                             [this](cocos2d::Touch *, cocos2d::Event *)
                             {
                                 DWORD uid = HallDataMgr::getInstance()->m_dwUserID;
                                 this->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::ROLE_INFO, &uid, sizeof(DWORD));
                             });
    clipNode->addChild(m_pUserHead);
    
    //倒计时
    m_pClockNode = GameClockNode::create();
    CC_ASSERT(m_pClockNode != nullptr);
    m_pClockNode->setTag(kMY_ROLE);
    m_pClockNode->setPosition(topSize.width * 0.92, topSize.height * 0.34);
    m_root->addChild(m_pClockNode, GAME_CLOCK_ZORDER);
}

void GameLayer::initJettonAreaInfo()
{
    m_tagJettonLayout = static_cast<Layout*>(m_root->getChildByName("tag_control"));
    CC_ASSERT(m_tagJettonLayout != nullptr);
    
    char buf[64] = "";
    for (int i = 0; i < AREA_MAX; ++i)
    {
        memset(buf, 0, 64);
        sprintf(buf, "tag%d_btn",i);
        auto btn = static_cast<Button*>(m_tagJettonLayout->getChildByName(buf));
        CC_ASSERT(btn != nullptr);
        btn->setTag(i);
        btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchJettonArea,this));
        
        m_pJettonNode[i] = nullptr;
    }
    
    reSetJettonAreaInfo();
}

void GameLayer::reSetJettonAreaInfo()
{
    for (int i = 0; i < AREA_MAX; ++i)
    {
        auto btn = static_cast<Button*>(m_tagJettonLayout->getChildByTag(i));
        btn->removeAllChildrenWithCleanup(true);
        this->enableBtn(btn, false);
        
        if (nullptr != m_pJettonNode[i])
        {
            m_pJettonNode[i]->reSet();
        }
    }
}

void GameLayer::initJettonInfo()
{
    m_userJettonLayout = static_cast<Layout*>(m_root->getChildByName("jetton_control"));
    CC_ASSERT(m_userJettonLayout != nullptr);
    Size infoSize = m_userJettonLayout->getContentSize();
    auto text = Text::create("本次下注为:", "Arial", 20);
    text->setAnchorPoint(Vec2(1.0f, 0.5f));
    text->setPosition(Vec2(infoSize.width * 0.495, infoSize.height * 0.19f));
    m_userJettonLayout->addChild(text);
    
    m_clipJetton = ClipText::createClipText(Size(120,23), "");
    CC_ASSERT(m_clipJetton != nullptr);
    m_clipJetton->setPosition(Vec2(infoSize.width * 0.5f, infoSize.height * 0.19f));
    m_clipJetton->setAnchorPoint(Vec2(0, 0.5f));
    m_clipJetton->setTextColor(Color4B(255,165,0,255));
    m_userJettonLayout->addChild(m_clipJetton);
    
    reSetJettonInfo();
}

void GameLayer::reSetJettonInfo()
{
    m_userJettonLayout->setVisible(false);
    m_clipJetton->setString("");
}

void GameLayer::initJettonBtnInfo()
{
    m_JettonBtnLayout = static_cast<Layout*>(m_root->getChildByName("chipCell_sp"));
    CC_ASSERT(m_JettonBtnLayout != nullptr);

    memset(m_pJettonNumber, 0, 8);
    LONGLONG tmpScore[8] = {100, 1000, 10000, 100000, 1000000, 5000000, 10000000, 1};
    memcpy(&m_pJettonNumber, tmpScore, sizeof(tmpScore));
    
    char buf[64] = "";
    for (int i = 0; i < 6; ++i)
    {
        memset(buf, 0, 64);
        sprintf(buf, "chip%d_btn",i);
        auto btn = static_cast<Button*>(m_JettonBtnLayout->getChildByName(buf));
        CC_ASSERT(btn != nullptr);
        btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchJettonBtn,this));
        
        btn->setTag(i);
    }
    
    reSetJettonBtnInfo();
}

void GameLayer::reSetJettonBtnInfo()
{
    char name[32] = "";
    /*
    char buf[64] = "";
    if (HallDataMgr::getInstance()->m_UserScore > 5000000)
    {
        for (int i = 0; i < 6; ++i)
        {
            memset(buf, 0, 64);
            sprintf(buf, "room_chip_%d_0.png",i + 3);
            memset(name, 0, 32);
            sprintf(name, "chip%d_btn",i);
            auto btn = static_cast<Button*>(m_JettonBtnLayout->getChildByName(name));
            CC_ASSERT(btn != nullptr);
            btn->loadTextureNormal(buf,Widget::TextureResType::PLIST);
            btn->loadTextureDisabled(buf,Widget::TextureResType::PLIST);
            memset(buf, 0, 64);
            sprintf(buf, "room_chip_%d_1.png",i + 3);
            btn->loadTexturePressed(buf,Widget::TextureResType::PLIST);
            
            btn->setTag(i + 2);
            this->enableBtn(btn, false);
        }
    }
    else
    {
        for (int i = 0; i < 6; ++i)
        {
            memset(buf, 0, 64);
            sprintf(buf, "room_chip_%d_0.png",i + 1);
            memset(name, 0, 32);
            sprintf(name, "chip%d_btn",i);
            auto btn = static_cast<Button*>(m_JettonBtnLayout->getChildByName(name));
            CC_ASSERT(btn != nullptr);
            btn->loadTextureNormal(buf,Widget::TextureResType::PLIST);
            btn->loadTextureDisabled(buf,Widget::TextureResType::PLIST);
            memset(buf, 0, 64);
            sprintf(buf, "room_chip_%d_1.png",i + 1);
            btn->loadTexturePressed(buf,Widget::TextureResType::PLIST);
            
            btn->setTag(i);
            this->enableBtn(btn, false);
        }
    }
    */
    for (int i = 0; i < 6; ++i)
    {
        memset(name, 0, 32);
        sprintf(name, "chip%d_btn",i);
        auto btn = static_cast<Button*>(m_JettonBtnLayout->getChildByName(name));
        CC_ASSERT(btn != nullptr);
        btn->setTag(i);
        this->enableBtn(btn, false);
    }
}

void GameLayer::initRobotLayout()
{
    m_robotLayout = static_cast<Layout*>(m_root->getChildByName("tru_control"));
    CC_ASSERT(m_robotLayout != nullptr);
    m_robotLayout->setLocalZOrder(GAME_ROBOT_ZORDER);
    m_robotLayout->addTouchEventListener([this](cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
                                         {
                                             if (Widget::TouchEventType::ENDED == type)
                                             {
                                                 if (m_robotLayout->isVisible())
                                                 {
                                                     m_bTrusteeship = false;
                                                     m_robotLayout->setVisible(false);
                                                 }
                                             }
                                         });
    
    reSetRobotLayout();
}

void GameLayer::reSetRobotLayout()
{
    m_robotLayout->setVisible(false);
}

void GameLayer::initGameEffect()
{
    m_gameEffect = GameEffectLayer::create();
    CC_ASSERT(m_gameEffect != nullptr);
    m_root->addChild(m_gameEffect,GAME_EFFECT_ZORDER);
    
    reSetGameEffect();
}

void GameLayer::reSetGameEffect()
{
    m_gameEffect->reSet();
}

void GameLayer::initAction()
{
    auto scaleTo = ScaleTo::create(0.1f,1.0f);
    auto show = Show::create();
    m_actCallTip = Spawn::create(scaleTo,show,nullptr);
    m_actCallTip->retain();
    
    /*for (int i = 0; i < GAME_PLAYER; ++i)
    {
        auto fadein = FadeIn::create(0.2f);
        auto moveUp = MoveBy::create(0.5f, Vec2(0, 20.0f));
        auto fadeOut = FadeOut::create(0.2f);
        m_actScoreShow[i] = Sequence::create(Spawn::create(fadein,moveUp, NULL),fadeOut, NULL);
        m_actScoreShow[i]->retain();
    }*/
}

void GameLayer::quit(bool bCheck /*= true*/)
{
    bool bHandle = false;
    if (m_notify)
    {
        bHandle = m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME,GAME_UI::EXIT_GAME,&bCheck,sizeof(bool));
    }
    //m_btnQuit->setEnabled(!bHandle);
}

void GameLayer::enableBtn(cocos2d::ui::Button *btn,bool bEnable,bool bHide /*= false*/)
{
    btn->setEnabled(bEnable);
    
    if (bEnable)
    {
        btn->setVisible(true);
        btn->setOpacity(255);
    }
    else
    {
        if (bHide)
        {
            btn->setVisible(false);
        }
        else
        {
            btn->setOpacity(125);
        }
    }
}

void GameLayer::clearDirCards(const enGameRoleDir &dir)
{
    if (!INSTANCE(GameDataMgr)->isValidDir(dir))
    {
        return;
    }
}

void GameLayer::gameTrusteeship(bool bTrusteeship)
{
    m_bTrusteeship = bTrusteeship;
    m_robotLayout->setVisible(m_bTrusteeship);
    
    if (m_bTrusteeship)
    {
        
    }
}

void GameLayer::adjustJettonBtn()
{
    //还可以下注的数额
    LONGLONG lCanJetton = m_llMaxJetton - m_lHaveJetton;
    LONGLONG lCondition = MIN(m_scoreUser, lCanJetton);
    
    for (int i = 0; i < 8; ++i)
    {
        auto btn = m_JettonBtnLayout->getChildByTag(i);
        if (nullptr != btn)
        {
            this->enableBtn((Button*)btn, lCondition >= m_pJettonNumber[i]);
        }
    }
    
    if (m_nJettonSelect > m_scoreUser)
    {
        m_nJettonSelect = -1;
    }
}

std::vector<tagJettonInfo> GameLayer::calcuteJetton( LONGLONG llscore, bool bAllJetton)
{
    std::vector<tagJettonInfo> vec = std::vector<tagJettonInfo>();
    tagJettonInfo info = {0};
    if (bAllJetton)
    {
        info.m_cbIdx = 8;
        info.m_cbCount = 1;
        info.m_llScore = llscore;
        
        vec.push_back(info);
    }
    else
    {
        LONGLONG tmpScore = llscore;
        //计算千万下注数量
        int nCount = tmpScore / 10000000;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 10000000;
            info.m_cbCount = nCount;
            info.m_cbIdx = 7;
            vec.push_back(info);
            
            tmpScore -= nCount * 10000000;
        }
        
        //计算五百万下注数量
        nCount = tmpScore / 5000000;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 5000000;
            info.m_cbCount = nCount;
            info.m_cbIdx = 6;
            vec.push_back(info);
            
            tmpScore -= nCount * 5000000;
        }
        
        //计算一百万下注数量
        nCount = tmpScore / 1000000;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 1000000;
            info.m_cbCount = nCount;
            info.m_cbIdx = 5;
            vec.push_back(info);
            
            tmpScore -= nCount * 1000000;
        }
        
        //计算十万下注数量
        nCount = tmpScore / 100000;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 100000;
            info.m_cbCount = nCount;
            info.m_cbIdx = 4;
            vec.push_back(info);
            
            tmpScore -= nCount * 100000;
        }
        
        //计算一万下注数量
        nCount = tmpScore / 10000;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 10000;
            info.m_cbCount = nCount;
            info.m_cbIdx = 3;
            vec.push_back(info);
            
            tmpScore -= nCount * 10000;
        }
        
        //计算一千下注数量
        nCount = tmpScore / 1000;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 1000;
            info.m_cbCount = nCount;
            info.m_cbIdx = 2;
            vec.push_back(info);
            
            tmpScore -= nCount * 1000;
        }
        
        //计算一百下注数量
        nCount = tmpScore / 100;
        if (nCount > 0)
        {
            info = {0};
            info.m_llScore = 100;
            info.m_cbCount = nCount;
            info.m_cbIdx = 1;
            vec.push_back(info);
            
            tmpScore -= nCount * 100;
        }
    }
    
    return vec;
}

int GameLayer::getJettonIdx( LONGLONG llscore)
{
    int nIdx = 1;
    switch (llscore)
    {
        case 100:
            nIdx = 1;
            break;
        case 1000:
            nIdx = 2;
            break;
        case 10000:
            nIdx = 3;
            break;
        case 100000:
            nIdx = 4;
            break;
        case 1000000:
            nIdx = 5;
            break;
        case 5000000:
            nIdx = 6;
            break;
        case 10000000:
            nIdx = 7;
            break;
        default:
        {
            if (llscore > 10000000)
            {
                nIdx = 8;
            }
        }
            break;
    }
    return nIdx;
}

void GameLayer::randomSetJettonPos(cocos2d::Node *nodeArea, cocos2d::Sprite *jettonSp)
{
    if (nullptr == nodeArea || nullptr == jettonSp)
    {
        return;
    }
    
    Size nodeSize = Size(nodeArea->getContentSize().width - 80, nodeArea->getContentSize().height - 80);
    float ran = rand() % 100;
    float xOffset = (float)(ran / 100.0f);
    ran = rand() % 100;
    float yOffset = (float)(ran / 100.0f);
    
    Vec2 pos = Vec2(xOffset * nodeSize.width + 40, yOffset * nodeSize.height + 40);
    jettonSp->setPosition(pos);
}

void GameLayer::enableJetton(bool bEnable)
{
    auto children = m_tagJettonLayout->getChildren();
    for (ssize_t i = 0; i < children.size(); ++i)
    {
        auto btn = static_cast<Button*>(children.at(i));
        if (nullptr != btn && btn->getTag() != -1)
        {
            this->enableBtn(btn, bEnable);
        }
    }
    
    children = m_JettonBtnLayout->getChildren();
    for (ssize_t i = 0; i < children.size(); ++i)
    {
        auto btn = static_cast<Button*>(children.at(i));
        if (nullptr != btn)
        {
            this->enableBtn(btn, bEnable);
        }
    }
}

void GameLayer::refreshJetton()
{
    char buf[64] = "";
    sprintf(buf, "%lld",m_lHaveJetton);
    std::string str = buf;
    size_t len = str.length();
    for (int i = len - 3; i > 0; i -= 3)
    {
        str.insert(i, ",");
    }
    m_clipJetton->setString(str);
    
    m_userJettonLayout->setVisible(true);
}

void GameLayer::applyBanker(const enApplyState &enState)
{
    switch (enState)
    {
        case kCancelState:
        {
            applyBanker();
            updateApplyBtnState(kApplyState);
        }
            break;
        case kApplyState:
        {
            cancelApply();
            updateApplyBtnState(kCancelState);
        }
            break;
        case kApplyedState:
        {
            cancelApply();
            updateApplyBtnState(kCancelState);
        }
            break;
        default:
            break;
    }
}

void GameLayer::updateApplyBtnState(const enApplyState &enState)
{
    std::string str = "";
    switch (enState)
    {
        case kCancelState:
        {
            str = "room_button_application.png";
        }
            break;
        case kApplyState:
        {
            str = "room_cancel_apply.png";
        }
            break;
        case kApplyedState:
        {
            str = "room_cancelZhuang.png";
        }
            break;
        default:
            break;
    }
    m_btnApply->loadTextureDisabled(str, Widget::TextureResType::PLIST);
    m_btnApply->loadTextureNormal(str, Widget::TextureResType::PLIST);
    m_btnApply->loadTexturePressed(str, Widget::TextureResType::PLIST);
    m_btnApply->setTag(enState);
    m_enApplyState = enState;
}

void GameLayer::showGameResult(bool bShow)
{
    if (nullptr == m_gameResultLayer)
    {
        m_gameResultLayer = GameResultLayer::createLayer(this);
        CC_ASSERT(m_gameResultLayer != nullptr);
        m_root->addChild(m_gameResultLayer, GAME_RESULT_ZORDER);
    }
    if (bShow && INSTANCE(CGameLogic)->getJoinGame())
    {
        m_gameResultLayer->showGameResult(INSTANCE(CGameLogic)->getGameResult());
    }
    
    if (!bShow)
    {
        m_gameResultLayer->hideGameResult();
    }
    
    //更新庄家积分
    updateBankerScore();
}

void GameLayer::userJetton(const BYTE &cbArea, const LONGLONG &lScore)
{
    CMD_C_PlaceBet cmd = {0};
    cmd.cbBetArea = cbArea;
    cmd.lBetScore = lScore;
    
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_PLACE_JETTON, &cmd, sizeof(CMD_C_PlaceBet));
}

void GameLayer::applyBanker()
{
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_APPLY_BANKER, nullptr, 0);
}

void GameLayer::cancelApply()
{
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_CANCEL_BANKER, nullptr, 0);
}