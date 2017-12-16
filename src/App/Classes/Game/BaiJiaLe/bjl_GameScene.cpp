//
//  GameScene.cpp
//  DouDiZhu
//
//  Created by zhong on 1/21/16.
//
//

#include "BaiJiaLeScene.h"
#include "GameDataMgr.h"
#include "ModeLayer.h"
#include "AnimationMgr.h"
#include "GameRecordMgr.h"
#include "BaiJiaLeLogicDefine.h"
static bool s_bReplace = false;

USING_BJL_NAMESPACE;
GameScene::GameScene():
m_wGameChair(INVALID_CHAIR),
m_wGameTable(INVALID_TABLE),
m_bSitDown(false),
m_gameLayer(nullptr)
{
    m_llApplyCount = 0;
}

GameScene::~GameScene()
{
    log("bjl release");
    this->stopAllActions();
    
    m_llApplyCount = 0;
    unloadAnimation();
    //资源释放
    INSTANCE(SceneMgr)->doUISceneResourceRelease(GameScene::uiUnloadResource());
    INSTANCE(GameDataMgr)->clearRecord();
    m_gameLayer = nullptr;
    
    //游戏逻辑释放
    INSTANCE(CGameLogic)->destroy();
}

std::vector<tagUIResourceParam> GameScene::uiAsynLoadResource()
{
    std::vector<tagUIResourceParam> vec = std::vector<tagUIResourceParam>();
    tagUIResourceParam param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/game.png";
    param.plistFile = "game/game.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/card.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/brow.png";
    param.plistFile = "game/brow.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/addnum.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/subnum.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/game_timenum.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/room_single_win.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/room_single_win_1.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/room_single_lose.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    char buf[32] = "";
    for (int i = 0; i < 2; ++i)
    {
        sprintf(buf,"game/game_ludan%d_zhuang.png",i + 1);
        param = {};
        param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
        param.resPath = buf;
        param.texType = ui::Widget::TextureResType::LOCAL;
        vec.push_back(param);
        
        sprintf(buf,"game/game_ludan%d_xian.png",i + 1);
        param = {};
        param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
        param.resPath = buf;
        param.texType = ui::Widget::TextureResType::LOCAL;
        vec.push_back(param);
        
        sprintf(buf,"game/game_ludan%d_ping.png",i + 1);
        param = {};
        param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
        param.resPath = buf;
        param.texType = ui::Widget::TextureResType::LOCAL;
        vec.push_back(param);
    }
    /*
    //动画资源
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "animation/animation.png";
    param.plistFile = "animation/animation.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    */
    return vec;
}

std::vector<tagUIResourceParam> GameScene::uiAsynLoadFinished()
{
    return GameScene::uiAsynLoadResource();
}

std::vector<tagUIResourceParam> GameScene::uiUnloadResource()
{
    return GameScene::uiAsynLoadResource();
}

bool GameScene::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Scene::init());
        s_bReplace = false;
        
        loadAnimation();
        
        //场景消息
        NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameScene::onGameScene, this);
        NetworkMgr::getInstance()->m_Userdelegate = this;
        
        NetworkMgr::getInstance()->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(GameScene::onGameMessage, this));
        
        m_gameLayer = GameLayer::create();
        CC_BREAK_IF(nullptr == m_gameLayer);
        m_gameLayer->setNotifyScene(this);
        this->addChild(m_gameLayer);
        
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
                auto player = static_cast<ModeLayer*>(HallDataMgr::getInstance()->AddpopLayer("系统提示", "您正在游戏中，确定要强退嘛？", Type_Ensure_Cancel));
                
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
                this->runAction(Sequence::createWithTwoActions(DelayTime::create(5.0f), call));
                
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
    if (nullptr == m_gameLayer)
    {
        return;
    }
    log("GameScene State:%d",HallDataMgr::getInstance()->m_cbGameStatus);
    switch (HallDataMgr::getInstance()->m_cbGameStatus)
    {
        case emGameState::kGAME_FREE:
        {
            CMD_S_StatusFree *cmd = (CMD_S_StatusFree*)pData;
            log("游戏空闲状态,剩余时间:%d; 申请条件:%lld",cmd->cbTimeLeave,cmd->lApplyBankerCondition);
            m_gameLayer->setGameStatus(emGameState::kGAME_FREE);
            //倒计时开始
            m_gameLayer->startCountDown(cmd->cbTimeLeave, bjl_kGAMEFREE_COUNTDOWN);
            
            //刷新庄家信息
            m_gameLayer->updateChangeBanker(cmd->wBankerUser, cmd->lBankerScore, cmd->bEnableSysBanker);
        }
            break;
        case emGameState::kGAME_JETTON:
        {
            CMD_S_StatusPlay *cmd = (CMD_S_StatusPlay*)pData;
            log("游戏下注状态,剩余时间:%d",cmd->cbTimeLeave);
            m_gameLayer->setGameStatus(emGameState::kGAME_JETTON);
            //倒计时开始
            m_gameLayer->startCountDown(cmd->cbTimeLeave, bjl_kGAMEPLAY_COUNTDOWN);
            //玩家最大下注
            m_gameLayer->setMaxJetton(cmd->lPlayBetScore);
            
            LONGLONG lScore = 0;
            //刷新下注信息
            LONGLONG ll = 0;
            for (int i = 0; i < AREA_MAX; ++i)
            {
                ll = cmd->lAllBet[i];
                m_gameLayer->reEnterGameJetton(i, ll);
                
                ll = cmd->lPlayBet[i];
                m_gameLayer->reEnterUserJetton(i, ll);
                lScore += ll;
            }
            
            //刷新庄家信息
            m_gameLayer->updateChangeBanker(cmd->wBankerUser, cmd->lBankerScore, cmd->bEnableSysBanker);
            
            //游戏开始信息
            m_gameLayer->reEnterStart(lScore);
        }
            break;
        case emGameState::kGAME_END:
        {
            CMD_S_StatusPlay *cmd = (CMD_S_StatusPlay*)pData;
            log("游戏结束状态,剩余时间:%d",cmd->cbTimeLeave);
            m_gameLayer->setGameStatus(emGameState::kGAME_END);
            //倒计时开始
            m_gameLayer->startCountDown(cmd->cbTimeLeave, bjl_kGAMEOVER_COUNTDOWN);
            INSTANCE(CGameLogic)->setDynamicJoin(true);
            
            //玩家最大下注
            m_gameLayer->setMaxJetton(cmd->lPlayBetScore);
            
            LONGLONG ll = 0;
            for (int i = 0; i < AREA_MAX; ++i)
            {
                ll = cmd->lAllBet[i];
                m_gameLayer->reEnterGameJetton(i, ll);
                
                ll = cmd->lPlayBet[i];
                m_gameLayer->reEnterUserJetton(i, ll);
            }
            
            //刷新庄家信息
            m_gameLayer->updateChangeBanker(cmd->wBankerUser, cmd->lBankerScore, cmd->bEnableSysBanker);
            
            //设置游戏结果
            tagGameResult res = tagGameResult();
            res.m_llTotal = cmd->lPlayAllScore;
            memcpy(&res.m_pAreaScore, cmd->lPlayScore, 8 * sizeof(LONGLONG));
            INSTANCE(CGameLogic)->setGameResult(res);
            bool bJoin = false;
            for (int i = 0; i < AREA_MAX; ++i)
            {
                if (cmd->lPlayScore[i] != 0)
                {
                    bJoin = true;
                    break;
                }
            }
            INSTANCE(CGameLogic)->setJoinGame(bJoin);
            
            //显示扑克界面
            tagCardsResult tagRes = tagCardsResult();
            tagCards tmp = tagCards();
            tmp.m_cardCount = cmd->cbCardCount[0];
            memcpy(&tmp.m_cardData, cmd->cbTableCardArray[0], tmp.m_cardCount);
            tagRes.m_idleCards = tmp;
            tmp = tagCards();
            tmp.m_cardCount = cmd->cbCardCount[1];
            memcpy(&tmp.m_cardData, cmd->cbTableCardArray[1], tmp.m_cardCount);
            tagRes.m_masterCards = tmp;
            m_gameLayer->updateCardsLayer(tagRes, cmd->cbTimeLeave > 13, cmd->cbTimeLeave);
        }
            break;
        default:
            break;
    }
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
    //log("用户状态--昵称:%s;id:%d",puser->m_nickname.c_str(),puser->m_date.dwUserID);
    
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

void GameScene::onGameMessage(WORD wSubCmdID, void *pData, WORD wSize)
{
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
    if (nullptr == m_gameLayer)
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
    /*
    if (bMyDesk)
    {
        CCLOG("\n\n++++++++++++++++++++++++++++++++++++++++++++++");
        CCLOG("id:%d,table:%d,chair:%d,sta:%d",cmd->dwUserID,cmd->UserStatus.wTableID,cmd->UserStatus.wChairID,cmd->UserStatus.cbUserStatus);
    }
    
    if (bMyDesk)
    {
        CCLOG("chair:%d ,switch dir:%d",cmd->UserStatus.wChairID,dir);
        CCLOG("++++++++++++++++++++++++++++++++++++++++++++++ \n\n ");
    }
    */
    enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(cmd->UserStatus.wChairID);
    if (bLookOn) return;
    
    UserData *pItem = HallDataMgr::getInstance()->getUserData(cmd->dwUserID);
    if (nullptr == pItem)
    {
        return;
    }
    
    //更新用户状态
    m_gameLayer->updateUserStatus(pItem,dir);
    
    //非自己桌不处理准备状态
    if (bMyDesk)
    {
        //更新准备标签
        m_gameLayer->updateReadyDisplay(dir,bReady);
    }
}

void GameScene::onGetUserTextChat(void *pBuffer)
{
    CMD_GR_S_UserChat *cmd = (CMD_GR_S_UserChat*)pBuffer;
    
    UserData *pItem = HallDataMgr::getInstance()->getUserData(cmd->dwSendUserID);
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
    m_gameLayer->showGameTextChat(dir, msg.c_str());
    
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
    
    UserData *pItem = HallDataMgr::getInstance()->getUserData(cmd->dwSendUserID);
    if (nullptr == pItem)
    {
        return;
    }
    
    enGameRoleDir dir = INSTANCE(GameDataMgr)->getRoleDir(pItem->m_date.wChairID);
    if (kMY_ROLE == dir)
    {
        return; //收到自己的消息不处理
    }
    
    m_gameLayer->showGameBrowChat(dir, cmd->wItemIndex + 1);
    
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
    if (nullptr == m_gameLayer)
    {
        return;
    }
    switch (sub)
    {
        case SUB_S_GAME_FREE:       //游戏空闲
        {
            m_gameLayer->setGameStatus(emGameState::kGAME_FREE);
            this->onGetGameFree(pBuffer, size);
        }
            break;
        case SUB_S_GAME_START:      //游戏开始
        {
            m_gameLayer->setGameStatus(emGameState::kGAME_JETTON);
            this->onGetStartGame(pBuffer, size);
        }
            break;
        case SUB_S_PLACE_JETTON:    //用户下注
        {
            m_gameLayer->setGameStatus(emGameState::kGAME_JETTON);
            this->onGetUserJetton(pBuffer, size);
        }
            break;
        case 102:        //游戏结束
        {
            m_gameLayer->setGameStatus(emGameState::kGAME_END);
            this->onGetGameOver(pBuffer, size);
        }
            break;
        case SUB_S_APPLY_BANKER:    //申请庄家
        {
            this->onGetApplyBanker(pBuffer, size);
        }
            break;
        case SUB_S_CHANGE_BANKER:   //切换庄家
        {
            this->onGetChangeBanker(pBuffer,size);
        }
            break;
        case SUB_S_CHANGE_USER_SCORE://更新积分
        {
            this->onGetChangeScore(pBuffer,size);
        }
            break;
        case SUB_S_SEND_RECORD:     //游戏记录
        {
            this->onGetGameRecord(pBuffer,size);
        }
            break;
        case SUB_S_PLACE_JETTON_FAIL: //下注失败
        {
            this->onGetJettonFail(pBuffer,size);
        }
            break;
        case SUB_S_CANCEL_BANKER:   //取消申请
        {
            this->onGetCancelApply(pBuffer,size);
        }
            break;
        case SUB_S_AMDIN_COMMAND:   //管理员命令
        {
            this->onGetAdminCMD(pBuffer, size);
        }
            break;
        case SUB_S_UPDATE_STORAGE:  //更新库存
        {
            this->onGetUpdateStorage(pBuffer, size);
        }
            break;
        default:
            break;
    }
}

void GameScene::onGetGameFree(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_GameFree *cmd = (CMD_S_GameFree*)pBuffer;
    log("游戏空闲:空闲时间--%d",cmd->cbTimeLeave);
    //倒计时开始
    m_gameLayer->startCountDown(cmd->cbTimeLeave, bjl_kGAMEFREE_COUNTDOWN);
    //游戏提示
    m_gameLayer->showGameTip(GameEffectLayer::emGameTip::kGAME_FREE_TIP);
    
    m_gameLayer->updateOnGameStateFree();
}

void GameScene::onGetStartGame(BYTE *pBuffer,const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_GameStart *cmd = (CMD_S_GameStart*)pBuffer;
    log("游戏开始:剩余时间--%d",cmd->cbTimeLeave);
    //玩家最大下注
    m_gameLayer->setMaxJetton(cmd->lPlayBetScore);
    //倒计时开始
    m_gameLayer->startCountDown(cmd->cbTimeLeave, bjl_kGAMEPLAY_COUNTDOWN);
    //游戏提示
    m_gameLayer->showGameTip(GameEffectLayer::emGameTip::kGAME_JETTON_TIP);
    
    m_gameLayer->onGameStart();
}

void GameScene::onGetUserJetton(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_PlaceBet *cmd = (CMD_S_PlaceBet*)pBuffer;
    if (cmd->wChairID == HallDataMgr::getInstance()->m_wChairID)
    {
        return;
    }
    log("玩家下注:玩家--%d",cmd->wChairID);
    m_gameLayer->updateUserJetton(cmd->wChairID, cmd->cbBetArea, cmd->lBetScore);
}

void GameScene::onGetGameOver(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_GameEnd *cmd = (CMD_S_GameEnd*)pBuffer;
    log("游戏结束:剩余时间--%d",cmd->cbTimeLeave);
    //倒计时开始
    m_gameLayer->startCountDown(cmd->cbTimeLeave, bjl_kGAMEOVER_COUNTDOWN);
    
    //设置游戏结果
    tagGameResult res = tagGameResult();
    res.m_llTotal = cmd->lPlayAllScore;
    log("玩家得分:%lld",cmd->lPlayAllScore);
    memcpy(&res.m_pAreaScore, cmd->lPlayScore, 8 * sizeof(LONGLONG));
    INSTANCE(CGameLogic)->setGameResult(res);
    bool bJoin = false;
    
    int nWinCount = 0;
    int nLoseCount = 0;
    for (int i = 0; i < AREA_MAX; ++i)
    {
        if (cmd->lPlayScore[i] > 0)
        {
            bJoin = true;
            ++nWinCount;
        }
        else if (cmd->lPlayScore[i] < 0)
        {
            bJoin = true;
            ++nLoseCount;
        }
    }
    //成绩
    INSTANCE(CGameLogic)->m_llTotalScore += cmd->lPlayAllScore;
    INSTANCE(CGameLogic)->calcuteRate(nWinCount, nLoseCount);
    
    INSTANCE(CGameLogic)->setJoinGame(bJoin);
    
    //设置结算扑克
    tagCardsResult tagRes = tagCardsResult();
    tagCards tmp = tagCards();
    tmp.m_cardCount = cmd->cbCardCount[0];
    memcpy(&tmp.m_cardData, cmd->cbTableCardArray[0], tmp.m_cardCount);
    tagRes.m_idleCards = tmp;
    tmp = tagCards();
    tmp.m_cardCount = cmd->cbCardCount[1];
    memcpy(&tmp.m_cardData, cmd->cbTableCardArray[1], tmp.m_cardCount);
    tagRes.m_masterCards = tmp;
    m_gameLayer->updateCardsLayer(tagRes, true, cmd->cbTimeLeave);
    
    m_gameLayer->updateGameOver();
    
    //标记一局结束
    INSTANCE(GameDataMgr)->setIsRoundOver(true);
}

void GameScene::onGetApplyBanker(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_ApplyBanker *cmd = (CMD_S_ApplyBanker*)pBuffer;
    log("申请庄家:申请--%d",cmd->wApplyUser);
    
    UserData *pUser = HallDataMgr::getInstance()->getUserDataByChair(cmd->wApplyUser);
    if (nullptr == pUser)
    {
        return;
    }
    tagApplyInfo info = {0};
    info.m_wChairId = cmd->wApplyUser;
    info.m_wUserId = pUser->m_date.dwUserID;
    info.m_strUserName = pUser->m_nickname;
    info.m_bCurrent = pUser->m_date.dwUserID == HallDataMgr::getInstance()->m_dwUserID;
    info.m_llIdx = m_llApplyCount++;
    
    m_gameLayer->addApplyBankerUser(info);
}

void GameScene::onGetChangeBanker(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_ChangeBanker *cmd = (CMD_S_ChangeBanker*)pBuffer;
    log("切换庄家:当前庄家--%d,庄家分数--%lld",cmd->wBankerUser,cmd->lBankerScore);
    //需要清空上庄列表
    m_gameLayer->updateChangeBanker(cmd->wBankerUser, cmd->lBankerScore, false);
}

void GameScene::onGetChangeScore(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
}

void GameScene::onGetGameRecord(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    int singsize = sizeof(tagServerGameRecord);
    int count = size/singsize;
    log("游戏记录:%d",count);
    
    tagGameRecord rec = tagGameRecord();
    for (int i = 0; i < count; ++i)
    {
        rec = tagGameRecord();
        tagServerGameRecord *record = new tagServerGameRecord();
        char *pbegin = (char *)pBuffer + i*singsize;
        memcpy(record, pbegin, singsize);
        rec.m_pServerRecord = record;
        
        if (record->cbPlayerCount > record->cbBankerCount)
        {
            rec.m_cbGameResult = AREA_XIAN;
        }
        else if (record->cbPlayerCount < record->cbBankerCount)
        {
            rec.m_cbGameResult = AREA_ZHUANG;
        }
        else if (record->cbPlayerCount == record->cbBankerCount)
        {
            rec.m_cbGameResult = AREA_PING;
        }
        INSTANCE(GameRecordMgr)->addGameRecord(rec);
    }
     /*
    tagGameRecord rec = tagGameRecord();
    rec = tagGameRecord();
    tagServerGameRecord *record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_PING;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_PING;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_PING;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_ZHUANG;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_PING;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_ZHUANG;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_ZHUANG;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_ZHUANG;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_ZHUANG;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    record = new tagServerGameRecord();
    rec.m_cbGameResult = AREA_XIAN;
    rec.m_pServerRecord = record;
    INSTANCE(GameRecordMgr)->addGameRecord(rec);
    */
    //刷新路单
    m_gameLayer->updateWallBill();
}

void GameScene::onGetJettonFail(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_PlaceBetFail *cmd = (CMD_S_PlaceBetFail*)pBuffer;
    log("下注失败:下注玩家-%d,区域-%d,当前下注-%lld",cmd->wPlaceUser,cmd->lBetArea,cmd->lPlaceScore);
    m_gameLayer->updateUserJettonFail(cmd->wPlaceUser, cmd->lBetArea, cmd->lPlaceScore);
}

void GameScene::onGetCancelApply(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_CancelBanker *cmd = (CMD_S_CancelBanker*)pBuffer;
    log("取消申请:取消--%d",cmd->wCancelUser);
    m_gameLayer->removeApplyBankerUser(cmd->wCancelUser);
}

void GameScene::onGetAdminCMD(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
}

void GameScene::onGetUpdateStorage(BYTE *pBuffer, const int &size)
{
    if (0 == size || nullptr == pBuffer)
    {
        return;
    }
    CMD_S_UpdateStorage *cmd = (CMD_S_UpdateStorage*)pBuffer;
    log("更新库存:新-%lld,衰减-%lld",cmd->lStorage,cmd->lStorageDeduct);
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
            m_gameLayer->reSet();
            m_gameLayer->reSetNewGame();
            return;
        }
    }
    INSTANCE(SceneMgr)->transitionScene(BJL_SCENE(PLAZA_SCENE),false);
    s_bReplace = true;
}

void GameScene::removeNetMessage()
{
    //网络
    NetworkMgr::getInstance()->m_GameScenecallback = nullptr;
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GF_GAME);
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    
}

void GameScene::loadAnimation()
{/*
    //加载叫分动画
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("call_point_0%d.png", 0, 5, CALLSCORE_ANIMATION_KEY, Widget::TextureResType::PLIST);
    //一分
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("call_point_0%d.png", 5, 3, CALLONE_ANIMATION_KEY, Widget::TextureResType::PLIST);
    //两分
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("call_point_1%d.png", 5, 3, CALLTWO_ANIMATION_KEY, Widget::TextureResType::PLIST);
    //三分
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("call_point_2%d.png", 5, 3, CALLTHREE_ANIMATION_KEY, Widget::TextureResType::PLIST);
    
    //加载飞机动画
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("plane_%d.png", 0, 5, AIRSHIP_ANIMATION_KEY, Widget::TextureResType::PLIST);
    
    //加载火箭动画
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("rocket_%d.png", 0, 5, ROCKET_ANIMATION_KEY, Widget::TextureResType::PLIST);
    
    //加载报警动画
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("game_alarm_0%d.png", 1, 4, ALARM_ANIMATION_KEY, Widget::TextureResType::PLIST);
    
    //加载炸弹动画
    INSTANCE(AnimationMgr)->loadAnimationFromFrame("game_bomb_0%d.png", 1, 5, BOMB_ANIMATION_KEY, Widget::TextureResType::PLIST);*/
}

void GameScene::unloadAnimation()
{/*
    INSTANCE(AnimationMgr)->removeCachedAnimation(CALLSCORE_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(CALLONE_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(CALLTWO_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(CALLTHREE_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(AIRSHIP_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(ROCKET_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(ALARM_ANIMATION_KEY);
    INSTANCE(AnimationMgr)->removeCachedAnimation(BOMB_ANIMATION_KEY);*/
}