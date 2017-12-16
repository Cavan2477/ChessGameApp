#include "GameLayer.h"
#include "GameDataMgr.h"
#include "AnimationMgr.h"

USING_NS_CC;
using namespace ui;
USING_NN_NAMESPACE;

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
    //亮牌按钮
    TAG_SHOWCARD_MENU,
    //提示按钮
    TAG_SUGGEST_MENU,
    //叫庄按钮
    TAG_CALLBAKER_MENU,
    //不叫庄
    TAG_NOCALL_MENU,
    //叫分
    TAG_CALLSCORE_MENU
};

//场景Z轴顺序
enum emGameZorder
{
    //扑克
    GAME_CARDS_ZORDER,
    //准备菜单
    GAME_READY_ZORDER,
    //玩家
    GAME_USER_ZORDER,
    //顶部菜单
    GAME_TOPMENU_ZORDER,
    //游戏控制层
    GAME_CONTROL_ZORDER,
    //聊天
    GAME_CHAT_ZORDER,
    //游戏结算
    GAME_RESULT_ZORDER,
};
GameLayer::GameLayer():
	m_notify(nullptr),
    m_bOnGame(false),
    m_settingLayer(nullptr),
    m_antiCheat(nullptr),
    m_roleInfo(nullptr)
{
    //延迟加载
    m_gameChat = nullptr;
    //m_bDynamicJoin = false;
    m_nPlayerCount = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
}

GameLayer::~GameLayer()
{
	m_actCallTip->release();
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_actScoreShow[i]->release();
    }
    
    HallDataMgr::getInstance()->m_bStartGame = false;
}

bool GameLayer::init()
{
	bool bRes = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		//
		LOAD_CSB("game/GameLayer.csb");
        
        HallDataMgr::getInstance()->m_bStartGame = true;

		//扑克卡牌
		this->initCards();

		//顶部菜单
		this->initTopMenu();

		//初始化准备布局
		this->initPreGameLayout();

		//游戏开始布局
		this->initStartGameLayout();

		//游戏下庄布局
		this->initCallScoreLayout();

		//游戏进行
		this->initOnGameLayout();
        
        //下注结果
        this->initAddScoreResLayout();
        
        //牛牛结算
        this->initNiuNiuNodeLayout();

		//玩家信息
		this->initUserInfo();
        
        //游戏特效
        this->initGameEffect();

		//通用动作
		this->initAction();
        
        if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
        {
            //隐藏换桌按钮，交换开始按钮与换桌按钮的位置
            m_btnReady->setPosition(m_btnChangeDesk->getPosition());
            m_btnChangeDesk->setVisible(false);
            m_btnChangeDesk->setEnabled(false);
        }
//        m_bDynamicJoin = HallDataMgr::getInstance()->m_cbGameStatus != nn_GS_TK_FREE;
        
        //初始进来按钮无效状态
        this->switchToBeReady(false);
        this->enableBtn(m_btnChangeDesk,true);
		bRes = true;
	} while (false);
	return bRes;
}

void GameLayer::onSelectedCards(const tagCards &cards)
{
    m_calculate->setVisible(cards.m_cardCount > 0);
    
    if (cards.m_cardCount == 3)
    {
        BYTE cardsdata[NN_MAXCOUNT] = {};
        memcpy(cardsdata, m_nodeCards[kMY_ROLE]->getHandCards().m_cardData, m_nodeCards[kMY_ROLE]->getHandCards().m_cardCount);
        
        //在有牛的情况下，手动选择非牛牛牌，不做结算
        BYTE i = INSTANCE(GameLogic)->GetCardLogicValue(cards.m_cardData[0]);
        BYTE j = INSTANCE(GameLogic)->GetCardLogicValue(cards.m_cardData[1]);
        BYTE k = INSTANCE(GameLogic)->GetCardLogicValue(cards.m_cardData[2]);
        
        m_calculate->refreshNumber(i,j,k);
        if (INSTANCE(GameLogic)->GetOxCard(cardsdata, NN_MAXCOUNT))
        {
            if ((i + j + k) % 10 == 0)
            {
                BYTE ty = INSTANCE(GameLogic)->GetCardType(cardsdata, NN_MAXCOUNT);
                m_calculate->showResult((emNiuNiuType)ty);
            }
            else //显示无牛
            {
                m_calculate->showResult(OX_VALUE0);
            }
        }
        else
        {
            m_calculate->showResult(OX_VALUE0);
        }
    }
    else
    {
        m_calculate->hideResult();
    }
}

void GameLayer::onCardsStateChange(const BYTE &cardData, bool bSelect)
{
    BYTE logicValue = INSTANCE(GameLogic)->GetCardLogicValue(cardData);
    if (bSelect)
    {       
        m_calculate->addNumber(logicValue);
    }
    else
    {
        m_calculate->cutNumber(logicValue);
    }
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
                    m_root->addChild(m_roleInfo);
                }
                UserData *pUser = HallDataMgr::getInstance()->m_UserList.at(roleId);
                if (nullptr != pUser)
                {
                    m_roleInfo->setLocalZOrder(GAME_CONTROL_ZORDER);
                    m_roleInfo->refresh(pUser);
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
            default:
                break;
        }
    }
    return true;
}

void GameLayer::reSet()
{
    //重置准备布局
    reSetPreGameLayout();
    //重置游戏开始布局
    reSetStartGameLayout();
    //重置游戏下注布局
    reSetCallScoreLayout();
    //重置控制布局
    reSetOnGameLayout();
    //重置游戏特效
    reSetGameEffect();
}

void GameLayer::reSetNewGame()
{
    //清理扑克
    reSetCards();
    //牛牛结算
    reSetNiuNiuNode();
    //下注/结算结果
    reSetAddScoreResLayout();
}

void GameLayer::clearUser(bool bClear /*= true*/)
{
    reSetUserInfo(!bClear);
}

void GameLayer::updateOnGameStateFree()
{
    reSet();
    reSetNewGame();
    clearUser(false);
    
    this->switchToBeReady(true);
    this->enableBtn(m_btnChangeDesk,true);
}

void GameLayer::updateUserStatus( UserData *pUser,const enGameRoleDir &dir)
{
    enGameRoleDir sitDir = dir;
    tagMobileUserInfoHead info = pUser->m_date;
	//用户状态
	bool bLeave = info.cbUserStatus == US_FREE;

	int nTag = info.dwUserID;
	int nChair = info.wChairID;
    SCORE score = info.lScore;

    UILayer *notify = this;
	if (bLeave)
	{
		if (dir != kMY_ROLE)
		{
            GameRoleNode *tmp = static_cast<GameRoleNode*>(m_root->getChildByTag(nTag));
            if (nullptr == tmp)
            {
                return;
            }
            notify = nullptr;
            sitDir = INSTANCE(GameDataMgr)->getRoleDir(tmp->getChairID());
            nTag = -1;
            nChair = -1;
		}
        
        if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
        {
            m_nodeGameRole[sitDir]->updateRoleInfo(info.dwUserID,"玩家",0,bLeave);
        }
        else
        {
            m_nodeGameRole[sitDir]->updateRoleInfo(info.dwUserID,pUser->m_nickname.c_str(),score,bLeave);
        }
        //准备精灵
        m_spReady[sitDir]->setVisible(false);
	}
    if (kMY_ROLE == sitDir && !m_bDynamicJoin)
    {
        m_clock->startCountDown(COUNTDOWN_READY, CC_CALLBACK_2(GameLayer::countDownCallBack, this),kREADY_COUNTDOWN);
    }
    if (!INSTANCE(GameDataMgr)->isValidDir(sitDir))
    {
        return;
    }
    
    m_nodeGameRole[sitDir]->setTag(nTag);
    m_nodeGameRole[sitDir]->setChairID(nChair);
    m_nodeGameRole[sitDir]->setNotify(notify);
    //不同桌
    if (info.wTableID != HallDataMgr::getInstance()->m_wTableID)
    {
        return;
    }
	
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        m_nodeGameRole[sitDir]->updateRoleInfo(info.dwUserID,"玩家",0,bLeave);
    }
    else
    {
        m_nodeGameRole[sitDir]->updateRoleInfo(info.dwUserID,pUser->m_nickname.c_str(),score,bLeave);
    }
}

void GameLayer::updateReadyDisplay(const enGameRoleDir &dir, bool bReady)
{
    m_preGameLayout->setVisible(true);
	//准备精灵
	m_spReady[dir]->setVisible(bReady);

	//准备、换桌按钮
	if (dir == kMY_ROLE && !m_bDynamicJoin)
	{        
		this->enableBtn(m_btnReady,!bReady);
		this->enableBtn(m_btnChangeDesk,!bReady);
        
        if (bReady)
        {
            m_clock->stopCountDown();
        }
	}
}

void GameLayer::updateOnGame(bool bOnGame)
{
    //准备界面隐藏
    this->switchToBeReady(false);
    this->enableBtn(m_btnChangeDesk,false);
    
    //倒计时停止
    m_clock->stopCountDown();
}

void GameLayer::updateCallBanker(const enGameRoleDir &calldir)
{
    m_startGameLayout->setVisible(true);
    
    if (kMY_ROLE == calldir)
    {
        m_spCallTip->setVisible(false);
        //叫庄状态显示
        if (m_cbGameStatus == nn_GS_TK_CALL)
        {
            m_btnCallBanker->setVisible(true);
            m_btnNoCall->setVisible(true);
        }
    }
    else
    {
        m_spCallTip->setVisible(true);
        m_btnCallBanker->setVisible(false);
        m_btnNoCall->setVisible(false);
        
        m_spCallTip->stopAction(m_actCallTip);
        m_spCallTip->setScaleX(0.0001f);
        if (kRIGHT_ROLE == calldir)
        {
            m_spCallTip->setAnchorPoint(Vec2(1.0f,0.5f));
        }
        else
        {
            m_spCallTip->setAnchorPoint(Vec2(0,0.5f));
        }
        m_spCallTip->setPosition(m_tipsPos[calldir]);
        m_spCallTip->runAction(m_actCallTip);
    }
    
    //倒计时
    m_clock->stopCountDown();
    m_clock->startCountDown(COUNTDOWN_CALLBANKER, CC_CALLBACK_2(GameLayer::countDownCallBack, this),kBANKER_COUNTDOWN);
}

void GameLayer::updateGameBanker(const enGameRoleDir &dir, const LONGLONG &maxScore)
{
    //叫庄界面隐藏
    m_startGameLayout->setVisible(false);
    
    //显示庄家
    m_nodeGameRole[dir]->switchToBanker(true);
    //播放庄家音效
    INSTANCE(AudioMgr)->playGetBankerAudio(m_nodeGameRole[dir]->getTag());
    
    m_gameEffect->showGameTip();
    //闲家显示下注
    if (kMY_ROLE != dir && !m_bDynamicJoin)
    {
        //下注状态显示
        if (m_cbGameStatus == nn_GS_TK_SCORE)
        {
            m_callScoreLayout->setVisible(true);
        }
    }
    else
    {
        //显示闲家下注中
        m_gameEffect->showGameTip(GameEffectLayer::emGameTip::kGAME_TIP_ADDSCORE);
    }
    
    //倒计时
    m_clock->stopCountDown();
    m_clock->startCountDown(COUNTDOWN_CALLSCORE, CC_CALLBACK_2(GameLayer::countDownCallBack, this),kSCORE_COUNTDOWN);
    
    //计算下注数量
    this->updateCallScoreBtn(maxScore);
}

void GameLayer::reEnterShowGameBanker(const enGameRoleDir &dir)
{
    //显示庄家
    m_nodeGameRole[dir]->switchToBanker(true);
}

void GameLayer::updateCallScoreBtn(const LONGLONG &maxScore)
{
	int mutiples = 8;
	char buf[64] = "";
	for (int i = 3; i >= 0; --i)
	{
		memset(buf,0,64);
		
        LONGLONG score = maxScore / mutiples;
		float fScore = 0.0f;
		if (score / 100000000 > 99)
		{
			fScore = score / 100000000.0f;
			sprintf(buf,"%lld 亿",(LONGLONG)fScore);
		}
		else if (score > 100000000)
		{
			fScore = score / 100000000.0f;
			sprintf(buf,"%.2f 亿",fScore);
		}
		else if (score > 1000000)
		{
			fScore = score / 10000;
			sprintf(buf,"%d 万",(int)fScore);
		}
		else if (score > 10000)
		{
			fScore = score / 10000.0f;
			sprintf(buf,"%.2f 万",fScore);
		}
		else
		{
			sprintf(buf,"%lld",score);
		}

		m_callBtns[i]->setTitleText(buf);
        m_llAddScore[i] = score;
        mutiples /= 2;
    
	}
}

void GameLayer::updateCallScoreResult(const enGameRoleDir &dir,
                                      const LONGLONG &lscore,
                                      bool gameover)
{
    LONGLONG llscore = lscore;
    //如果是自己下注结束，则隐藏下注界面
    if (kMY_ROLE == dir)
    {
        //下注界面隐藏
        m_callScoreLayout->setVisible(false);
    }
    
    //下注结果显示
    m_scoreResLayout->setVisible(true);
    
    //处理下注数量
    if (gameover)
    {
        std::string buf = formatScore(llscore);
        char szscore[64] = "";
        
        if (llscore > 0)
        {
            sprintf(szscore, "/%lld",llscore);
            buf = "+" + buf;            
            m_atlasScore[dir]->setProperty(szscore, "game/addnum.png", 19, 23, "/");
        }
        else if (llscore < 0)
        {
            sprintf(szscore, "/%lld",-llscore);
            buf = "-" + buf;
            m_atlasScore[dir]->setProperty(szscore, "game/subnum.png", 19, 23, "/");
        }
        m_textCallScore[dir]->setString(buf);
        m_atlasScore[dir]->stopAction(m_actScoreShow[dir]);
        m_atlasScore[dir]->runAction(m_actScoreShow[dir]);
    }
    else
    {
        m_textCallScore[dir]->setString(formatScore(llscore));
        if (kMY_ROLE != dir)
        {
            INSTANCE(AudioMgr)->playNiuNiuAddScoreAudio(llscore);
        }
    }
    
    m_spCallScore[dir]->setVisible(0 != llscore);
    m_textCallScore[dir]->setVisible(0 != llscore);
}

std::string GameLayer::formatScore(const LONGLONG &llscore)
{
    //处理下注数量
    char buf[64];
    LONGLONG score = llscore > 0 ? llscore : -llscore;
    float fScore = 0.0f;
    if (score / 100000000 > 99)
    {
        fScore = score / 100000000.0f;
        sprintf(buf,"%lld 亿",(LONGLONG)fScore);
    }
    else if (score > 100000000)
    {
        fScore = score / 100000000.0f;
        sprintf(buf,"%.2f 亿",fScore);
    }
    else if (score > 1000000)
    {
        fScore = score / 10000;
        sprintf(buf,"%d 万",(int)fScore);
        
    }
    else if (score > 10000)
    {
        fScore = score / 10000.0f;
        sprintf(buf,"%.2f 万",fScore);
    }
    else
    {
        sprintf(buf,"%lld",score);
    }
    
    return std::string(buf);
}

void GameLayer::updateUserCards(const enGameRoleDir &dir, const tagCards &cards)
{
    m_nodeCards[dir]->updateCardsNode(dir,cards,dir == kMY_ROLE);
    
    if (kMY_ROLE == dir  && 0 != cards.m_cardCount)
    {
        //显示亮牌界面
        m_onGameLayout->setVisible(true);
        
        //倒计时
        m_clock->stopCountDown();
        m_clock->startCountDown(COUNTDOWN_OUTCARD, CC_CALLBACK_2(GameLayer::countDownCallBack, this), kOUTCARD_COUNTDOWN);
    }
}

void GameLayer::updateCardsOver()
{
    //下注界面隐藏
    m_callScoreLayout->setVisible(false);
    //tips隐藏
    m_gameEffect->showGameTip();
    
    m_bSuggestBtn = false;
}

void GameLayer::updateUserOpenCard(const enGameRoleDir &dir, bool bOpenCard)
{
    m_nodeGameRole[dir]->switchToOpenCard(bOpenCard,dir);
}

void GameLayer::updateUserNiuNiuRes(const enGameRoleDir &dir, const LONGLONG &lScore)
{
    tagCards cards = m_nodeCards[dir]->getHandCards();
    if (kMY_ROLE != dir && 0 != cards.m_cardCount)
    {
        BYTE ty = INSTANCE(GameLogic)->GetCardType(cards.m_cardData, cards.m_cardCount);
        m_niuniu[dir]->refreshNiuNiu((emNiuNiuType)ty);
        
        //播放牛牛音效
        INSTANCE(AudioMgr)->playNiuNiuAudio(m_nodeGameRole[dir]->getTag(), ty);
    }
    if (0 != cards.m_cardCount)
    {
        m_nodeCards[dir]->setLocalZOrder(GAME_USER_ZORDER);
        m_nodeCards[dir]->showCardsNiuNiu();
        //结算
        updateCallScoreResult(dir,lScore,true);
        
        //根据分数判断胜负
        if (kMY_ROLE == dir)
        {
            INSTANCE(AudioMgr)->playGameEndAudio(lScore > 0);
        }
    }
}

void GameLayer::updateGameOver()
{
    m_bDynamicJoin = false;
    m_bSuggestBtn = false;
    //游戏结束，重新开始
    reSet();
    bool bHideUser = !INSTANCE(GameDataMgr)->getEnterAntiCheat();
    //清理界面玩家
    reSetUserInfo(bHideUser);
    
    //倒计时
    m_clock->stopCountDown();
    m_clock->startCountDown(COUNTDOWN_READY, CC_CALLBACK_2(GameLayer::countDownCallBack, this), kREADY_COUNTDOWN);
    
    //显示准备按钮
    this->switchToBeReady(true);
    this->enableBtn(m_btnChangeDesk,true);
}

void GameLayer::updateFiveKing(const enGameRoleDir &dir, const bool &bFiveKing)
{
    if (!INSTANCE(GameDataMgr)->isValidDir(dir))
    {
        return;
    }
    
    if (bFiveKing)
    {
        m_nodeGameRole[dir]->showCaiJinAnimation();
    }
}

void GameLayer::showGameTextChat(const enGameRoleDir &dir, const char *str)
{
    //防作弊不聊天
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        return;
    }
    m_nodeGameRole[dir]->textChat(dir, str);
}

void GameLayer::showGameBrowChat(const enGameRoleDir &dir, const int &idx)
{
    //防作弊不聊天
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        return;
    }
    m_nodeGameRole[dir]->browChat(dir, idx);
}

void GameLayer::countDownCallBack(cocos2d::Node *sender, const enCountDownTag &tag)
{
    log("倒计时结束 %d",tag);
    switch (tag)
    {
        case enCountDownTag::kREADY_COUNTDOWN:
        {
            //直接退出房间
            //INSTANCE(GameDataMgr)->setEnterNormalRoom(false);
            if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
            {
                INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
            }
        }
            break;
        case enCountDownTag::kOUTCARD_COUNTDOWN:
        {            
            outCard(m_bSuggestBtn);
        }
            break;
        default:
            break;
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
                if (m_cbGameStatus == nn_GS_TK_FREE || m_cbGameStatus == nn_GS_TK_END)
                {
                    m_bOnGame = false;
                }
                if (m_bDynamicJoin)
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
                    m_root->addChild(m_settingLayer,GAME_CONTROL_ZORDER);
                }
                m_settingLayer->showLayer(true);
            }
			break;
		case TAG_ROBOT_MENU:
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
                INSTANCE(AudioMgr)->playClickAudio();
				//准备
				INSTANCE(GameDataMgr)->requestReady(true);
				//按钮disable
				this->enableBtn(m_btnReady,false);
                //
                this->enableBtn(m_btnChangeDesk, false);
                
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
                this->enableBtn(m_btnChangeDesk, false);
                this->enableBtn(m_btnReady, false);
			}
			break;
		case TAG_CALLBAKER_MENU:
			{
				//叫庄
				userCallBanker(1);
                INSTANCE(AudioMgr)->playCallBankerAudio(HallDataMgr::getInstance()->m_dwUserID, true);
			}
			break;
		case TAG_NOCALL_MENU:
			{
				//不叫庄
				userCallBanker(0);
                INSTANCE(AudioMgr)->playCallBankerAudio(HallDataMgr::getInstance()->m_dwUserID, false);
			}
			break;
		case TAG_SHOWCARD_MENU:
			{
                outCard(true);
			}
			break;
		case TAG_SUGGEST_MENU:
            {
                INSTANCE(AudioMgr)->playClickAudio();
                m_bSuggestBtn = !m_bSuggestBtn;
                
                tagCards hand = m_nodeCards[kMY_ROLE]->getHandCards();
                if (INSTANCE(GameLogic)->GetOxCard(hand.m_cardData, m_nodeCards[kMY_ROLE]->getHandCards().m_cardCount))
                {
                    tagCards suggest = tagCards();
                    memcpy(suggest.m_cardData, hand.m_cardData, 3);
                    suggest.m_cardCount = 3;
                    m_nodeCards[kMY_ROLE]->suggestShootCards(suggest);
                }
                else //显示无牛
                {
                    m_calculate->setVisible(m_bSuggestBtn);
                    m_calculate->showResult(OX_VALUE0);
                }
            }
			break;
		default:
			break;
		}
	}
}

void GameLayer::touchCallScore(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Button *pBtn = static_cast<Button*>(pSender);
	if (nullptr == pBtn)
	{
		return;
	}

	if (Widget::TouchEventType::ENDED == type)
	{
		int nTag = pBtn->getTag();
        if (nTag > 4)
        {
            return;
        }
        
        LONGLONG score = m_llAddScore[nTag];
        //WORD user = HallDataMgr::getInstance()->m_wChairID;

		//加注
        userAddScore(score);
        INSTANCE(AudioMgr)->playNiuNiuAddScoreAudio(score);
	}
}

void GameLayer::initCards()
{
	Size rootSize = m_root->getContentSize();

	Vec2 pos[4];
	pos[kMY_ROLE] = Vec2(rootSize.width * 0.5,100);
	pos[kLEFT_ROLE] = Vec2(rootSize.width * 0.2,rootSize.height * 0.6);
	pos[kRIGHT_ROLE] = Vec2(rootSize.width * 0.8,rootSize.height * 0.6);
	pos[kTOP_ROLE] = Vec2(rootSize.width * 0.35,rootSize.height - 100);
	for (int i = 0; i < m_nPlayerCount; ++i)
	{
		m_nodeCards[i] = nullptr;
		m_nodeCards[i] = CardsNode::createEmptyCardsNode();
		CC_ASSERT( m_nodeCards[i] != nullptr);
		m_nodeCards[i]->setPosition(pos[i]);
		m_root->addChild(m_nodeCards[i]);
	}
    m_nodeCards[kMY_ROLE]->setSelectCardsListen(this);
    
    reSetCards();
}

void GameLayer::reSetCards()
{
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_nodeCards[i]->removeAllCards();
        m_nodeCards[i]->setLocalZOrder(GAME_CARDS_ZORDER);
    }
}

void GameLayer::initTopMenu()
{
	m_topMenuLayout = static_cast<Layout*>(m_root->getChildByName("btn_layout"));
	CC_ASSERT( m_topMenuLayout != nullptr);

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
}

void GameLayer::initPreGameLayout()
{
	m_preGameLayout = static_cast<Layout*>(m_root->getChildByName("pregame_control"));
	CC_ASSERT( m_preGameLayout != nullptr);
	m_preGameLayout->setZOrder(GAME_READY_ZORDER);

	//开始按钮
	m_btnReady = static_cast<Button*>(m_preGameLayout->getChildByName("ready_btn"));
	CC_ASSERT( m_btnReady != nullptr);
	m_btnReady->setTag(TAG_START_MENU);
	m_btnReady->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));

	//换桌按钮
	m_btnChangeDesk = static_cast<Button*>(m_preGameLayout->getChildByName("chtable_btn"));
	CC_ASSERT( m_btnChangeDesk != nullptr);
	m_btnChangeDesk->setTag(TAG_CHDESK_MENU);
	m_btnChangeDesk->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));

	//准备精灵
	char buf[64] = "";
	for (int i = 0; i <m_nPlayerCount; ++i)
	{
		m_spReady[i] = nullptr;
		sprintf(buf,"ready%d",i);
		m_spReady[i] = static_cast<Sprite*>(m_preGameLayout->getChildByName(buf));
		CC_ASSERT(m_spReady[i] != nullptr);
	}
    
    reSetPreGameLayout();
}

void GameLayer::reSetPreGameLayout()
{
    m_preGameLayout->setVisible(false);
    this->enableBtn(m_btnReady, true);
    this->enableBtn(m_btnChangeDesk, true);
}

void GameLayer::initStartGameLayout()
{
	m_startGameLayout = static_cast<Layout*>(m_root->getChildByName("startgame_control"));
	CC_ASSERT(m_startGameLayout != nullptr);

	//叫庄
	m_btnCallBanker = static_cast<Button*>(m_startGameLayout->getChildByName("callbanker_btn"));
	CC_ASSERT(m_btnCallBanker != nullptr);
	m_btnCallBanker->setTag(TAG_CALLBAKER_MENU);
	m_btnCallBanker->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));

	//不叫
	m_btnNoCall = static_cast<Button*>(m_startGameLayout->getChildByName("nocall_btn"));
	CC_ASSERT(m_btnNoCall != nullptr);
	m_btnNoCall->setTag(TAG_NOCALL_MENU);
	m_btnNoCall->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    Size callSize = m_startGameLayout->getContentSize();
    //下注提示
    m_tipsPos[kMY_ROLE] = Vec2(callSize.width * 0.12,callSize.height * 0.2);
    m_tipsPos[kLEFT_ROLE] = Vec2(callSize.width * 0.12, callSize.height * 0.65);
    m_tipsPos[kRIGHT_ROLE] = Vec2(callSize.width * 0.88, callSize.height * 0.65);
    m_tipsPos[kTOP_ROLE] = Vec2(callSize.width * 0.26, callSize.height * 0.9);
    m_spCallTip = static_cast<Sprite*>(m_startGameLayout->getChildByName("game_calling"));
    CC_ASSERT(m_spCallTip != nullptr);
    
    reSetStartGameLayout();
}

void GameLayer::reSetStartGameLayout()
{
    m_startGameLayout->setVisible(false);
    
    this->enableBtn(m_btnCallBanker, true);
    this->enableBtn(m_btnNoCall, true);
}

void GameLayer::initCallScoreLayout()
{
	m_callScoreLayout = static_cast<Layout*>(m_root->getChildByName("callscore_control"));
	CC_ASSERT(m_callScoreLayout != nullptr);
	m_callScoreLayout->setVisible(false);

	char buf[64] = "";
	for (int i = 0; i < 4; ++i)
	{
		sprintf(buf,"score_btn%d",i+1);
		m_callBtns[i] = static_cast<Button*>(m_callScoreLayout->getChildByName(buf));
		CC_ASSERT(m_callBtns[i] != nullptr);
		m_callBtns[i]->setTag(i);
		m_callBtns[i]->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchCallScore,this));
	}
    
    reSetCallScoreLayout();
}

void GameLayer::reSetCallScoreLayout()
{
    for (int i = 0; i < 4; ++i)
    {
        m_callBtns[i]->setTitleText("");
        m_llAddScore[i] = 0;
    }
}

void GameLayer::initOnGameLayout()
{
	m_onGameLayout = static_cast<Layout*>(m_root->getChildByName("ongame_control"));
	CC_ASSERT( m_onGameLayout != nullptr);
	m_onGameLayout->setZOrder(GAME_CONTROL_ZORDER);

	//亮牌按钮
	m_btnShowCards = static_cast<Button*>(m_onGameLayout->getChildByName("showcard_btn"));
	CC_ASSERT( m_btnShowCards != nullptr);
	m_btnShowCards->setTag(TAG_SHOWCARD_MENU);
	m_btnShowCards->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));

	//提示按钮
	m_btnSuggest = static_cast<Button*>(m_onGameLayout->getChildByName("suggest_btn"));
	CC_ASSERT( m_btnSuggest != nullptr);
	m_btnSuggest->setTag(TAG_SUGGEST_MENU);
	m_btnSuggest->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    Size layoutSize = m_onGameLayout->getContentSize();
    //计算节点
    m_calculate = CalculateNiuItem::create();
    CC_ASSERT( m_calculate != nullptr );
    m_calculate->setPosition(Vec2(layoutSize.width * 0.5f, layoutSize.height * 0.5f));
    m_onGameLayout->addChild(m_calculate);
    
    reSetOnGameLayout();
}

void GameLayer::reSetOnGameLayout()
{
    m_onGameLayout->setVisible(false);
    m_calculate->setVisible(false);
    m_calculate->reSet();
}

void GameLayer::initAddScoreResLayout()
{
    m_scoreResLayout = static_cast<Layout*>(m_root->getChildByName("addscore_control"));
    CC_ASSERT(m_scoreResLayout != nullptr);
    
    //下注数目
    char buf[64] = "";
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_spCallScore[i] = nullptr;
        m_textCallScore[i] = nullptr;
        
        memset(buf, 0, 64);
        sprintf(buf,"callscore%d_bg",i);
        m_spCallScore[i] = static_cast<Sprite*>(m_scoreResLayout->getChildByName(buf));
        CC_ASSERT(m_spCallScore[i] != nullptr);
        memset(buf, 0, 64);
        sprintf(buf,"callscore%d",i);
        m_textCallScore[i] = static_cast<Text*>(m_scoreResLayout->getChildByName(buf));
        CC_ASSERT(m_textCallScore[i] != nullptr);
    }
    
    reSetAddScoreResLayout();
}

void GameLayer::reSetAddScoreResLayout()
{
    m_scoreResLayout->setVisible(false);
    //下注数目
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_spCallScore[i]->setVisible(false);
        m_textCallScore[i]->setVisible(false);
    }
}

void GameLayer::initNiuNiuNodeLayout()
{
    m_niuniuLayout = static_cast<Layout*>(m_root->getChildByName("niuniu_control"));
    CC_ASSERT( m_niuniuLayout != nullptr );
    m_niuniuLayout->setZOrder(GAME_CONTROL_ZORDER);
    
    char buf[64] = "";
    Size size = m_niuniuLayout->getContentSize();
    Vec2 pos[4];
    pos[kMY_ROLE] = Vec2(size.width * 0.5, size.height * 0.37);
    pos[kRIGHT_ROLE] = Vec2(size.width * 0.67, size.height * 0.5);
    pos[kLEFT_ROLE] = Vec2(size.width * 0.33, size.height * 0.5);
    pos[kTOP_ROLE] = Vec2(size.width * 0.5,size.height * 0.7);
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_niuniu[i] = NiuNiuNode::create();
        CC_ASSERT( m_niuniu[i] != nullptr );
        m_niuniu[i]->setPosition(pos[i]);
        m_niuniuLayout->addChild(m_niuniu[i]);
        
        m_atlasScore[i] = nullptr;
        sprintf(buf, "res_atlas%d",i);
        m_atlasScore[i] = static_cast<TextAtlas*>(m_niuniuLayout->getChildByName(buf));
        CC_ASSERT(m_atlasScore[i] != nullptr);
    }
    
    reSetNiuNiuNode();
}

void GameLayer::reSetNiuNiuNode()
{
    m_niuniuLayout->setVisible(false);
    
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_niuniu[i]->reSet();
        m_atlasScore[i]->setOpacity(0);
        m_atlasScore[i]->setString("");
    }
    m_atlasScore[kMY_ROLE]->setPositionY(80);
    m_atlasScore[kRIGHT_ROLE]->setPositionY(368);
    m_atlasScore[kLEFT_ROLE]->setPositionY(368);
    m_atlasScore[kTOP_ROLE]->setPositionY(528);
}

void GameLayer::initUserInfo()
{
	for (int i = 0; i < m_nPlayerCount; ++i)
	{
		m_nodeGameRole[i] = nullptr;		
	}

    Size rootSize = m_root->getContentSize();
	m_pos[kMY_ROLE] = Vec2(rootSize.width * 0.06f, rootSize.height * 0.15f);
	m_pos[kLEFT_ROLE] = Vec2(rootSize.width * 0.06f, rootSize.height * 0.6f);
	m_pos[kRIGHT_ROLE] = Vec2(rootSize.width * 0.94f, rootSize.height * 0.6f);
	m_pos[kTOP_ROLE] = Vec2(rootSize.width * 0.2f, rootSize.height * 0.85f);

	for (int i = 0; i < m_nPlayerCount; ++i)
	{
		m_nodeGameRole[i] = GameRoleNode::create();
		CC_ASSERT(m_nodeGameRole[i] != nullptr);
		m_nodeGameRole[i]->setPosition(m_pos[i].x,m_pos[i].y);
		m_root->addChild(m_nodeGameRole[i],GAME_USER_ZORDER);
	}
    
    m_clock = GameClockNode::create();
    CC_ASSERT( m_clock != nullptr );
    m_clock->setPosition(Vec2(rootSize.width * 0.15,rootSize.height * 0.35));
    m_root->addChild(m_clock,GAME_CONTROL_ZORDER);
}

void GameLayer::reSetUserInfo(bool bHideUser /*= true*/)
{
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_nodeGameRole[i]->reSet(bHideUser);
    }
}

void GameLayer::initGameEffect()
{
    m_gameEffect = GameEffectLayer::create();
    CC_ASSERT(m_gameEffect != nullptr);
    m_root->addChild(m_gameEffect);
    
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
    
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        auto fadein = FadeIn::create(0.2f);
        auto moveUp = MoveBy::create(0.5f, Vec2(0, 20.0f));
        auto fadeOut = FadeOut::create(0.2f);
        m_actScoreShow[i] = Sequence::create(Spawn::create(fadein,moveUp, NULL),fadeOut, NULL);
        m_actScoreShow[i]->retain();
    }
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

void GameLayer::enableBtn(cocos2d::ui::Button *btn,bool bEnable)
{
	btn->setEnabled(bEnable);

	if (bEnable)
	{
		btn->setOpacity(255);
	}
	else
	{
		btn->setOpacity(125);
	}
}

void GameLayer::switchToBeReady(bool bReady)
{
    m_preGameLayout->setVisible(bReady);
    this->enableBtn(m_btnReady, bReady);
}

void GameLayer::outCard(bool bTrueCard)
{
    INSTANCE(AudioMgr)->playOpenCardAudio();
    if (bTrueCard)
    {
        m_niuniuLayout->setVisible(true);
        
        tagCards handCards = m_nodeCards[kMY_ROLE]->getHandCards();
        //判断牛牛标志
        BYTE ty = INSTANCE(GameLogic)->GetCardType(handCards.m_cardData, handCards.m_cardCount);
        //摊牌
        m_niuniu[kMY_ROLE]->refreshNiuNiu((emNiuNiuType)ty);
        ty = INSTANCE(GameLogic)->GetOxCard(handCards.m_cardData, handCards.m_cardCount);
        
        userOpenCard(ty);
    }
    else //没有建议牌，按无牛处理
    {
        userOpenCard(OX_VALUE0);
    }
    //显示等待出牌
    if (!INSTANCE(GameDataMgr)->getIsMineLander())
    {
        m_gameEffect->showGameTip(GameEffectLayer::emGameTip::kGAME_TIP_OUTCARD);
    }
    
    //隐藏亮牌控制按钮
    m_onGameLayout->setVisible(false);
    
    //隐藏计算界面
    m_calculate->setVisible(false);
    m_calculate->reSet();
}

void GameLayer::userCallBanker(const BYTE &btBanker)
{
    CMD_C_CallBanker cmd = {0};
    cmd.bBanker = btBanker;
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME,NN_SUB_C_CALL_BANKER,&cmd, sizeof(CMD_C_CallBanker));
}

void GameLayer::userAddScore(const LONGLONG &lAddScore)
{
    CMD_C_AddScore cmd = {0};
    cmd.lScore = lAddScore;
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME,NN_SUB_C_ADD_SCORE, &cmd, sizeof(CMD_C_AddScore));
}

void GameLayer::userOpenCard(const BYTE &bOpen)
{
    CMD_C_OxCard cmd = {0};
    cmd.bOX = bOpen;
    NetworkMgr::getInstance()->sendData(MDM_GF_GAME,NN_SUB_C_OPEN_CARD,&cmd,sizeof(CMD_C_OxCard));
}