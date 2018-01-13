/************************************************************************************
 * file: 		nn_GameScene.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:33:04
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#include "Header/NiuNiuScene.h"
#include "../../DataMgr/GameDataMgr.h"
#include "../../Scene/ModeLayer.h"
#include "../../Scene/AnimationMgr/AnimationMgr.h"

USING_NN_NAMESPACE;

static bool s_bReplace = false;

GameScene::GameScene():
	m_wGameChair(INVALID_CHAIR),
	m_wGameTable(INVALID_TABLE),
	m_bSitDown(false),
    m_pGameLayer(nullptr)
{
    loadRes();
}

GameScene::~GameScene()
{
    unloadRes();
    INSTANCE(GameDataMgr)->clearRecord();
    m_pGameLayer = nullptr;
    GameLogic::destroy();
}

GameScene* GameScene::createGameScene(const WORD &wTable,const WORD &wChair,bool bSitDown)
{
	GameScene *pGame = new GameScene();
	if (nullptr != pGame && pGame->init())
	{
		pGame->setGameTable(wTable);
		pGame->setGameChair(wChair);
		pGame->setSitDown(bSitDown);

		pGame->autorelease();
		return pGame;
	}
	CC_SAFE_DELETE(pGame);
	return nullptr;
}

GameScene* GameScene::createGameScene(bool bReEnter)
{
    GameScene *pGame = new GameScene();
    if (nullptr != pGame && pGame->init())
    {
        pGame->setSitDown(false);
        pGame->autorelease();
        return pGame;
    }
    CC_SAFE_DELETE(pGame);
    return nullptr;
}

bool GameScene::init()
{
	bool bRes = false;
	do 
	{
        CC_BREAK_IF(!Scene::init());
        s_bReplace = false;
        
        //场景消息
        NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameScene::onGameScene, this);
        NetworkMgr::getInstance()->m_Userdelegate = this;
        
        NetworkMgr::getInstance()->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(GameScene::onGameMessage, this));

		m_pGameLayer = GameLayer::create();
		CC_BREAK_IF(nullptr == m_pGameLayer);
		m_pGameLayer->setNotifyScene(this);
		this->addChild(m_pGameLayer);

        m_bClickExit = false;
		bRes = true;
	} while (false);
	return bRes;
}

void GameScene::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();
    
    //网络
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(GameScene::roomloginResult, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Room_LeaveGame, CC_CALLBACK_1(GameScene::notifyLeaveGame, this)), 1);
    
    INSTANCE(GameDataMgr)->setIsRoundOver(false);
    
    //配桌查询
    WORD wTable = HallDataMgr::getInstance()->m_wTableID;
    INSTANCE(GameDataMgr)->queryChairInfo(wTable, INVALID_CHAIR);
}

void GameScene::onExit()
{    
    removeNetMessage();
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Room_LeaveGame);
    
    //NetworkMgr::getInstance()->Disconnect(Data_Room);
    INSTANCE(GameDataMgr)->setAntiCheatJoin(false);
    
	Scene::onExit();
}

bool GameScene::uiMsg(const int &main, const int &sub,void *pData, const int &nLen)
{
	switch (main)
	{
	case UI_MSG_BASE::UI_MSG_GAME:
		return this->onGetGameUIMsg(sub,pData,nLen);
		break;
	default:
		break;
	}
	return true;
}

bool GameScene::onGetGameUIMsg(const int &sub,void *pData,const int &nLen)
{
	switch (sub)
	{
	case GAME_UI::EXIT_GAME:
		{
            bool bCheck = *((bool*)pData);
            m_bClickExit = true;
            if (bCheck)
            {
                auto player = static_cast<ModeLayer*>(HallDataMgr::getInstance()->AddpopLayer("系统提示", "您正在游戏中，强行退出会被扣分，确实要强退吗？", Type_Ensure_Cancel));
                player->setEnsureCallback([this] ()
                                          {
                                              INSTANCE(GameDataMgr)->setIsExit(true);
                                              if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                                              {
                                                  INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
                                              }
                                              removeNetMessage();
                                              
                                              WORD wTable = HallDataMgr::getInstance()->m_wTableID;
                                              WORD wChair = HallDataMgr::getInstance()->m_wChairID;
                                              INSTANCE(GameDataMgr)->leaveRoom(wTable,wChair,1);
                                              
                                              HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                                              
                                              auto call = CallFunc::create([this]()
                                                                           {
                                                                               if (!s_bReplace)
                                                                               {
                                                                                   log("delay leave");
                                                                                   this->replaceScene();
                                                                               }
                                                                           });
                                              DelayTime *de = DelayTime::create(10.0f);
                                              auto seq = Sequence::create(de,call, NULL);
                                              
                                              auto runScene = Director::getInstance()->getRunningScene();
                                              if (nullptr != runScene)
                                              {
                                                  runScene->runAction(seq);
                                              }
                                          });
                return true;
            }
            else
            {
                INSTANCE(GameDataMgr)->setIsExit(true);
                if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
                {
                    INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
                }
                removeNetMessage();
                
                WORD wTable = HallDataMgr::getInstance()->m_wTableID;
                WORD wChair = HallDataMgr::getInstance()->m_wChairID;
                INSTANCE(GameDataMgr)->leaveRoom(wTable,wChair,0);
                
                auto call = CallFunc::create([this]()
                                             {
                                                 if (!s_bReplace)
                                                 {
                                                     log("delay leave");
                                                     this->replaceScene();
                                                 }
                                             });
                this->runAction(Sequence::createWithTwoActions(DelayTime::create(10.0f), call));
                
                return true;
            }
		}
		break;
	default:
		break;
	}
    return false;
}

void GameScene::onGameScene(void *pData, WORD wSize)
{
    if (nullptr == m_pGameLayer)
    {
        return;
    }
    
    log("GameScene State:%d",HallDataMgr::getInstance()->m_cbGameStatus);
    switch (HallDataMgr::getInstance()->m_cbGameStatus)
    {
        case nn_GS_TK_FREE:    //空闲状态
        {
            log("空闲");
            m_pGameLayer->setDynamicJoin(false);
            m_pGameLayer->setGameStatus(nn_GS_TK_FREE);            
            m_pGameLayer->updateOnGameStateFree();
        }
            break;
        case nn_GS_TK_CALL:    //叫庄状态
        {
            log("游戏场景:叫庄");
            CMD_S_StatusCall* cmd = (CMD_S_StatusCall*)pData;
            
            //叫庄玩家
            enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wCallBanker);
            
            log("玩家:%d 叫庄; dir:%d",cmd->wCallBanker,dir);
            m_pGameLayer->setDynamicJoin(1 == cmd->cbDynamicJoin);
            m_pGameLayer->setGameStatus(nn_GS_TK_CALL);
            m_pGameLayer->updateOnGame(true);
            m_pGameLayer->updateCallBanker(dir);
        }
            break;
        case nn_GS_TK_SCORE:   //下注状态
        {
            log("游戏场景:下注");
            CMD_S_StatusScore* cmd = (CMD_S_StatusScore*)pData;
            
            //下注玩家
            enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wBankerUser);
            
            log("玩家:%d 庄家; dir:%d",cmd->wBankerUser,dir);
            m_pGameLayer->setDynamicJoin(1 == cmd->cbDynamicJoin);
            m_pGameLayer->setGameStatus(nn_GS_TK_SCORE);
            m_pGameLayer->updateOnGame(true);
            LONGLONG score = cmd->lTurnMaxScore;
            m_pGameLayer->updateGameBanker(dir,score);
        }
            break;
        case nn_GS_TK_PLAYING: //进行状态
        {
            log("游戏场景:进行");
            CMD_S_StatusPlay *cmd = (CMD_S_StatusPlay*)pData;
            m_pGameLayer->setDynamicJoin(1 == cmd->cbDynamicJoin);
            m_pGameLayer->setGameStatus(nn_GS_TK_PLAYING);
            m_pGameLayer->updateOnGame(true);
            
            //显示庄家用户
            enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wBankerUser);
            m_pGameLayer->reEnterShowGameBanker(dir);
            
            //扑克卡牌
            tagCards cards = tagCards();
            bool bHaveUser = false;
            int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
            for (int i = 0; i < max; ++i)
            {
                //玩家牌数据，以chair为下标
                dir = INSTANCE(GameDataMgr)->getRoleDir(i);
                
                memset(&cards.m_cardData,0,NN_MAXCOUNT);
                memcpy(&cards.m_cardData,cmd->cbHandCardData[i],NN_MAXCOUNT);
                //无用户
                if (cmd->cbHandCardData[i][0] == 0)
                {
                    cards.m_cardCount = 0;
                    bHaveUser = false;
                }
                else
                {
                    cards.m_cardCount = NN_MAXCOUNT;
                    bHaveUser = true;
                }
                
                m_pGameLayer->updateUserCards(dir,cards);
                
                //下注数目
                if (bHaveUser)
                {
                    LONGLONG score = cmd->lTableScore[i];
                    m_pGameLayer->updateCallScoreResult(dir,score);
                }
            }
            m_pGameLayer->updateCardsOver();
        }
            break;
        case nn_GS_TK_END:
        {
            m_pGameLayer->setDynamicJoin(false);
            m_pGameLayer->setGameStatus(nn_GS_TK_END);
            m_pGameLayer->reSet();
            m_pGameLayer->reSetNewGame();
            m_pGameLayer->clearUser();
        }
            break;
        default:
            break;
    }
    /*
    switch (wSize)
    {
        case sizeof(CMD_S_StatusCall):
        {
            
        }
            break;
        case sizeof(CMD_S_StatusScore):
        {
            
        }
            break;
        case sizeof(CMD_S_StatusPlay):
        {
            
        }
            break;
        default:
            break;
    }
    */
}

void GameScene::OnUserEnter(void *pUser)
{
    auto puser = static_cast<UserData *>(pUser);
    if (nullptr == puser)
    {
        return;
    }
    log("用户进入--昵称:%s;id:%d;桌子:%d",puser->m_nickname.c_str(),puser->m_date.dwUserID,puser->m_date.wTableID);
    
    CMD_GR_UserStatus cmd = {};
    cmd.dwUserID = puser->m_date.dwUserID;
    cmd.UserStatus.wChairID = puser->m_date.wChairID;
    cmd.UserStatus.wTableID = puser->m_date.wTableID;
    cmd.UserStatus.cbUserStatus = puser->m_date.cbUserStatus;
    
    onGetUserStatus((BYTE*)&cmd, sizeof(CMD_GR_UserStatus));
}

void GameScene::OnUserStatus(void *pUser)
{
    auto puser = static_cast<UserData *>(pUser);
    if (nullptr == puser)
    {
        return;
    }
    
    CMD_GR_UserStatus cmd = {};
    cmd.dwUserID = puser->m_date.dwUserID;
    cmd.UserStatus.wChairID = puser->m_date.wChairID;
    cmd.UserStatus.wTableID = puser->m_date.wTableID;
    cmd.UserStatus.cbUserStatus = puser->m_date.cbUserStatus;
    
    onGetUserStatus((BYTE*)&cmd, sizeof(CMD_GR_UserStatus));
}


void GameScene::OnUserScore(void *pUser)
{
    
}

void GameScene::OnUserChat(WORD wSubCode, void *pData, WORD wSize)
{
    switch (wSubCode)
    {
        case SUB_GF_USER_CHAT:
        {
            this->onGetUserTextChat(pData);
        }
            break;
        case SUB_GF_USER_EXPRESSION:
        {
            this->onGetUserBrowChat(pData);
        }
            break;
        default:
            break;
    }
}

void GameScene::onGameMessage(WORD wSubCmdID, void *pData, WORD wSize)
{
    log("******GameScene msg:main:%d, sub:%d",200,wSubCmdID);
    this->onHandleGameMsg(wSubCmdID, (BYTE*)pData, wSize);
}

void GameScene::roomloginResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    this->onHandleGameLoginMsg(wSubCmdID, (BYTE*)pData, wSize);
}

void GameScene::onHandleGameLoginMsg(const int &sub, BYTE *pBuffer, const int &size)
{
    switch (sub)
    {
        case SUB_GR_LOGON_SUCCESS:                  //登陆成功 100
        {
            
        }
            break;
        case SUB_GR_LOGON_FAILURE:                  //登陆失败 101
        {
            CMD_GR_LogonFailure *failure = (CMD_GR_LogonFailure *)pBuffer;
            auto str = WHConverUnicodeToUtf8WithArray(failure->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
            NetworkMgr::getInstance()->Disconnect(Data_Room);
        }
            break;
        case SUB_GR_LOGON_FINISH:                   //登陆完成 102
        {
            auto status = HallDataMgr::getInstance()->m_cbStatus;
            
            //此时是防作弊房间
            if (US_PLAYING == status || US_READY == status || US_OFFLINE == status)
            {
                //请求场景
                log("请求场景");
                INSTANCE(GameDataMgr)->requestFrame();
            }
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
        }
            break;
        default:
            break;
    }
}

void GameScene::onHandleGameConfigMsg(const int &sub, BYTE *pBuffer, const int &size)
{

}

void GameScene::onHandleUserMsg(const int &sub,BYTE *pBuffer, const int &size)
{
	if (nullptr == m_pGameLayer)
	{
		return;
	}

	switch (sub)
	{
	case SUB_GR_USER_ENTER:
		break;
	case SUB_GR_REQUEST_FAILURE:			//配桌请求失败
		break;
	case SUB_GR_USER_STATUS:
		this->onGetUserStatus(pBuffer,size);
		break;
	default:
		break;
	}
}

void GameScene::onGetUserStatus(BYTE *pBuffer, const int &size)
{
	CMD_GR_UserStatus *cmd = (CMD_GR_UserStatus*)pBuffer;
    //用户状态
    bool bReady = cmd->UserStatus.cbUserStatus == US_READY;
    bool bLookOn = cmd->UserStatus.cbUserStatus == US_LOOKON;
    bool bOnGame = cmd->UserStatus.cbUserStatus == US_PLAYING;
    bool bMyDesk = cmd->UserStatus.wTableID == HallDataMgr::getInstance()->m_wTableID;
    
    if (bMyDesk)
    {
        CCLOG("\n\n++++++++++++++++++++++++++++++++++++++++++++++");
        CCLOG("id:%d,table:%d,chair:%d,sta:%d",cmd->dwUserID,cmd->UserStatus.wTableID,cmd->UserStatus.wChairID,cmd->UserStatus.cbUserStatus);
    }
	enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->UserStatus.wChairID);
    if (bMyDesk)
    {
        CCLOG("chair:%d ,switch dir:%d",cmd->UserStatus.wChairID,dir);
        CCLOG("++++++++++++++++++++++++++++++++++++++++++++++ \n\n ");
    }	
	
	if (bLookOn) return;
    
    UserData *pItem = HallDataMgr::getInstance()->m_UserList.at(cmd->dwUserID);
	if (nullptr == pItem)
	{
		return;
	}

	//更新用户状态
	m_pGameLayer->updateUserStatus(pItem,dir);
    
    //非自己桌不处理准备状态
    if (bMyDesk)
    {
        //更新准备标签
        m_pGameLayer->updateReadyDisplay(dir,bReady);
        
        if (bOnGame)
        {
            m_pGameLayer->updateOnGame(true);
        }
    }
}

void GameScene::onGetUserTextChat(void *pBuffer)
{
    CMD_GR_S_UserChat *cmd = (CMD_GR_S_UserChat*)pBuffer;
    
    UserData *pItem = HallDataMgr::getInstance()->m_UserList.at(cmd->dwSendUserID);
    if (nullptr == pItem)
    {
        return;
    }
    
    enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(pItem->m_date.wChairID);
    if (kMY_ROLE == dir)
    {
        return; //收到自己的消息不处理
    }
    
    int chatLen = cmd->wChatLength;
    TCHAR szMsg[LEN_USER_CHAT];
    memset(szMsg, 0, LEN_USER_CHAT);
    memcpy(szMsg, cmd->szChatString, chatLen);
    std::string msg = WHConverUnicodeToUtf8WithArray(szMsg);
    m_pGameLayer->showGameTextChat(dir, msg.c_str());
    
    tagChatRecord record = {0};
    record.dwChatUserId = cmd->dwSendUserID;
    record.strChat = msg;
    record.bBrow = false;
    INSTANCE(GameDataMgr)->addRecord(record);
    
    //对比，播放音效
    size_t i = 0;
    if (INSTANCE(GameDataMgr)->compareWithStaticText(msg, i))
    {
        INSTANCE(AudioMgr)->playTextAudio(cmd->dwSendUserID, (int)i);
    }
}

void GameScene::onGetUserBrowChat(void *pBuffer)
{
    CMD_GR_S_UserExpression *cmd = (CMD_GR_S_UserExpression*)pBuffer;
    
    UserData *pItem = HallDataMgr::getInstance()->m_UserList.at(cmd->dwSendUserID);
    if (nullptr == pItem)
    {
        return;
    }
    
    enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(pItem->m_date.wChairID);
    if (kMY_ROLE == dir)
    {
        return; //收到自己的消息不处理
    }
    
    m_pGameLayer->showGameBrowChat(dir, cmd->wItemIndex + 1);
    
    tagChatRecord record = {0};
    record.dwChatUserId = cmd->dwSendUserID;
    record.bBrow = true;
    record.nIdx = cmd->wItemIndex + 1;
    INSTANCE(GameDataMgr)->addRecord(record);
}

void GameScene::onHandleFrameMsg(const int &sub, BYTE *pBuffer, const int &size)
{

}

void GameScene::onHandleGameMsg(const int &sub, BYTE *pBuffer,const int &size)
{
    if (nullptr == m_pGameLayer)
    {
        return;
    }
	switch (sub)
	{
            //case SUB_S_GAME_F
	case NN_SUB_S_GAME_START:		//游戏开始
        {
            m_pGameLayer->setGameStatus(nn_GS_TK_SCORE);
            this->onGetStartGame(pBuffer,size);
        }
		break;
	case NN_SUB_S_ADD_SCORE:		//加注结果
        this->onGetAddScore(pBuffer, size);
		break;
	case NN_SUB_S_PLAYER_EXIT:		//用户强退
		break;
	case NN_SUB_S_SEND_CARD:		//发牌
        this->onGetSendCard(pBuffer, size);
		break;
	case NN_SUB_S_GAME_END:		//游戏结束
        {
            m_pGameLayer->setGameStatus(nn_GS_TK_END);
            this->onGetGameOver(pBuffer, size);
        }
		break;
	case NN_SUB_S_OPEN_CARD:		//用户摊牌
        this->onGetOpenCard(pBuffer, size);
		break;
	case NN_SUB_S_CALL_BANKER:		//用户叫庄
        {
            m_pGameLayer->setGameStatus(nn_GS_TK_CALL);
            this->onGetCallBanker(pBuffer,size);
        }
		break;
	default:
		break;
	}
}

void GameScene::onGetStartGame(BYTE *pBuffer,const int &size)
{
	if (0 == size || nullptr == pBuffer)
	{
		return;
	}

	CMD_S_GameStart *cmd = (CMD_S_GameStart*)pBuffer;
	enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wBankerUser);

    log("玩家:%d 庄家; dir:%d",cmd->wBankerUser,dir);
	m_pGameLayer->updateGameBanker(dir,cmd->lTurnMaxScore);
}

void GameScene::onGetCallBanker(BYTE *pBuffer,const int &size)
{
	if (0 == size || nullptr == pBuffer)
	{
		return;
	}

	CMD_S_CallBanker *cmd = (CMD_S_CallBanker*)pBuffer;
	enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wCallBanker);

    log("玩家:%d 叫庄; dir:%d",cmd->wCallBanker,dir);
	m_pGameLayer->updateCallBanker(dir);
}

void GameScene::onGetAddScore(BYTE *pBuffer,const int &size)
{
	if (0 == size || nullptr == pBuffer)
	{
		return;
	}

	CMD_S_AddScore *cmd = (CMD_S_AddScore*)pBuffer;
	enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wAddScoreUser);

    log("玩家:%d 下注; dir:%d",cmd->wAddScoreUser,dir);
    LONGLONG count = cmd->lAddScoreCount;
	m_pGameLayer->updateCallScoreResult(dir,count);
}

void GameScene::onGetSendCard(BYTE *pBuffer,const int &size)
{
	if (0 == size || nullptr == pBuffer)
	{
		return;
	}

	CMD_S_SendCard *cmd = (CMD_S_SendCard*)pBuffer;
	tagCards cards = tagCards();
    int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
	for (int i = 0; i < max; ++i)
	{
		//玩家牌数据，以chair为下标
		enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(i);
		
		memset(&cards.m_cardData,0,NN_MAXCOUNT);
		memcpy(&cards.m_cardData,cmd->cbCardData[i],NN_MAXCOUNT);
        //无用户
        if (cmd->cbCardData[i][0] == 0)
        {
            cards.m_cardCount = 0;
        }
        else
        {
            cards.m_cardCount = NN_MAXCOUNT;
        }
		
		m_pGameLayer->updateUserCards(dir,cards);
	}
    m_pGameLayer->updateCardsOver();
}

void GameScene::onGetOpenCard(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    
    CMD_S_Open_Card *cmd = (CMD_S_Open_Card*)pBuffer;
    
    enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->wPlayerID);
    m_pGameLayer->updateUserOpenCard(dir, cmd->bOpen == 1);
}

void GameScene::onGetGameOver(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    
    CMD_S_GameEnd *cmd = (CMD_S_GameEnd*)pBuffer;
    int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
    for (int i = 0; i < max; ++i)
    {
        //玩家牌数据，以chair为下标
        enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(i);
        LONGLONG score = cmd->lGameScore[i];
        m_pGameLayer->updateUserNiuNiuRes(dir,score);
        
        //五花牛
        m_pGameLayer->updateFiveKing(dir, cmd->bfiveking[i]);
    }
    //得分之类
    m_pGameLayer->updateGameOver();
    
    //标记一局结束
    INSTANCE(GameDataMgr)->setIsRoundOver(true);
}

void GameScene::notifyLeaveGame(cocos2d::EventCustom *event)
{
    replaceScene();
}

void GameScene::replaceScene()
{
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        if (!m_bClickExit)
        {
            //界面清理（防作弊换桌）
            m_pGameLayer->Reset();
			m_pGameLayer->ResetNewGame();
            m_pGameLayer->ClearUser();
            return;
        }
    }
    //场景切换
    TransitionScene *reScene = NULL;
    Scene *s = PlazaScene::create();
    reScene = TransitionSlideInL::create(TIME_SCENE_CHANGE, s);
    Director::getInstance()->replaceScene(reScene);
    s_bReplace = true;
}

void GameScene::removeNetMessage()
{
    //网络
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GF_GAME);
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
}

void GameScene::loadRes()
{
    //扑克牌
    Director::getInstance()->getTextureCache()->addImage("game/card.png");
    
    Director::getInstance()->getTextureCache()->addImage("game/game_cardtype.png");
    //无牛
    SpriteFrame* frame = SpriteFrame::create("game/game_cardtype_wuniu.png", Rect(0, 0, 84, 48));
    SpriteFrameCache::getInstance()->addSpriteFrame(frame, "wuniu");
    
    //左边聊天背景
    frame = SpriteFrame::create("game/game_chat_0.9.png", Rect(0, 0, 170, 40));
    SpriteFrameCache::getInstance()->addSpriteFrame(frame, "left_chat");
    
    //右边聊天背景
    frame = SpriteFrame::create("game/game_chat_1.9.png", Rect(0, 0, 170, 40));
    SpriteFrameCache::getInstance()->addSpriteFrame(frame, "right_chat");
    
    //聊天表情
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/brow.plist");
    
    //加载彩金动画
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/caijin_ani.plist");
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("caijin_%d.png", 1, 26, CAIJIN_ANIMATION, cocos2d::ui::Widget::TextureResType::PLIST);
}

void GameScene::unloadRes()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey("game/card.png");
    
    Director::getInstance()->getTextureCache()->removeTextureForKey("game/game_cardtype.png");
    SpriteFrameCache::getInstance()->removeSpriteFrameByName("wuniu");
    SpriteFrameCache::getInstance()->removeSpriteFrameByName("left_chat");
    SpriteFrameCache::getInstance()->removeSpriteFrameByName("right_chat");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game/brow.plist");
    
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game/caijin_ani.plist");
    INSTANCE(AnimationMgr)->removeCachedAnimation(CAIJIN_ANIMATION);
}