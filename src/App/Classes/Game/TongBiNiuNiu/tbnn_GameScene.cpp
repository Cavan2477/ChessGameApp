//
//  tbnn_GameScene.cpp
//  GameProject
//
//  Created by zhong on 4/18/16.
//
//

#include "tbnn_GameScene.h"
#include "GameDataMgr.h"
#include "../CustomUtilsSrc/AnimationMgr/AnimationMgr.h"
static bool s_bReplace = false;

USING_TBNN_NAMESPACE;
USING_NS_CC;

GameScene::GameScene():
m_gameLayer(nullptr)
{
    
}

GameScene::~GameScene()
{
    INSTANCE(CGameLogic)->destroy();
    
    INSTANCE(AnimationMgr)->removeCachedAnimation(tbnn_CAIJIN_ANIMATION);
}

bool GameScene::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Scene::init());
        s_bReplace = false;
        
        registerNetMessage();
        
        m_gameLayer = GameLayer::createLayer(this);
        CC_BREAK_IF(nullptr == m_gameLayer);
        this->addChild(m_gameLayer);
        
        m_bClickExit = false;
        
        //加载彩金动画
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/caijin_ani.plist");
        INSTANCE(AnimationMgr)->loadAnimationFromFrame("caijin_%d.png", 1, 26, tbnn_CAIJIN_ANIMATION, cocos2d::ui::Widget::TextureResType::PLIST);
        
        bRes = true;
    } while (false);
    return bRes;
}

void GameScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    
    //网络
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
    
    Scene::onExit();
}

bool GameScene::uiMsg(const int &main, const int &sub, void *pData, const int &nLen)
{
    if (UI_MSG_BASE::UI_MSG_GAME == main)
    {
        return onGetGameUIMsg(sub, pData, nLen);
    }
    return false;
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
    param.resPath = "game/niuniu_tex2.png";
    param.texType = ui::Widget::TextureResType::LOCAL;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "game/brow.png";
    param.plistFile = "game/brow.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    return vec;
}

std::vector<tagUIResourceParam> GameScene::uiAsynLoadFinished()
{
    return GameScene::uiAsynLoadResource();
}

std::vector<tagUIResourceParam> GameScene::uiUnloadResource()
{
    return GameScene::uiUnloadResource();
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

bool GameScene::onGetGameUIMsg(const int &sub, void *pData, const int &nLen)
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
    if (nullptr == m_gameLayer)
    {
        return;
    }
    
    log("GameScene State:%d",HallDataMgr::getInstance()->m_cbGameStatus);
    switch (HallDataMgr::getInstance()->m_cbGameStatus)
    {
        case tbnn_GS_TK_FREE:
        {
            CMD_S_StatusFree *cmd = (CMD_S_StatusFree*)pData;
            log("游戏空闲:%lld",cmd->lCellScore);
            m_gameLayer->setGameStatus(en_TBNN_GameState::TBNN_kGameFree);
            
            m_gameLayer->updateOnGameStateFree();
        }
            break;
        case tbnn_GS_TK_PLAYING:
        {
            CMD_S_StatusPlay *cmd = (CMD_S_StatusPlay*)pData;
            log("游戏进行:是否动态加入---%d",cmd->cbDynamicJoin);
            m_gameLayer->setGameStatus(en_TBNN_GameState::TBNN_kGamePlay);
            
            //是否动态加入
            m_gameLayer->setDynamicJoin(cmd->cbDynamicJoin == 1);
            
            //庄家
            m_gameLayer->updateGameBanker(cmd->wBankerUser);
            
            //用户扑克
            tagCards cards = tagCards();
            for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
            {
                if (1 == cmd->cbPlayStatus[i])
                {
                    cards = tagCards();
                    cards.m_cardCount = tbnn_MAX_COUNT;
                    memcpy(&cards.m_cardData, cmd->cbHandCardData[i], sizeof(BYTE) * tbnn_MAX_COUNT);
                    
                    m_gameLayer->reEnterUpdateUserCards(i, cards);
                }
            }
        }
            break;
        default:
            break;
    }
}

void GameScene::onGameMessage(WORD wSubCmdID, void *pData, WORD wSize)
{
    if (nullptr == m_gameLayer)
    {
        return;
    }
    
    switch (wSubCmdID)
    {
        case tbnn_SUB_S_GAME_START:
        {
            m_gameLayer->setGameStatus(en_TBNN_GameState::TBNN_kGameStart);
            this->onGetGameStart(pData, wSize);
        }
            break;
        case tbnn_SUB_S_PLAYER_EXIT:
        {
            this->onGetUserForceQuit(pData, wSize);
        }
            break;
        case tbnn_SUB_S_SEND_CARD:
        {
            m_gameLayer->setGameStatus(en_TBNN_GameState::TBNN_kGamePlay);
            this->onGetDispatchCard(pData, wSize);
        }
            break;
        case tbnn_SUB_S_GAME_END:
        {
            m_gameLayer->setGameStatus(en_TBNN_GameState::TBNN_kGameEnd);
            this->onGetGameEnd(pData, wSize);
        }
            break;
        case tbnn_SUB_S_OPEN_CARD:
        {
            m_gameLayer->setGameStatus(en_TBNN_GameState::TBNN_kGamePlay);
            this->onGetUserOpenCard(pData, wSize);
        }
            break;
        case tbnn_SUB_S_AMDIN_COMMAND:
        {
            
        }
            break;
        case tbnn_SUB_S_ADMIN_REFRESH:
        {
            
        }
            break;
        default:
            break;
    }
}

void GameScene::onGetUserStatus(BYTE *pBuffer, const int &size)
{
    if (nullptr == m_gameLayer)
        return;
    
    CMD_GR_UserStatus *cmd = (CMD_GR_UserStatus*)pBuffer;
    //用户状态
    bool bLookOn = cmd->UserStatus.cbUserStatus == US_LOOKON;
    
    //旁观状态不处理
    if (bLookOn) return;
    
    UserData *pItem = HallDataMgr::getInstance()->getUserData(cmd->dwUserID);
    if (nullptr == pItem)
    {
        return;
    }
    
    //非自己桌用户离开的情况
    if (pItem->m_date.wTableID != INVALID_TABLE
        && pItem->m_date.wTableID != HallDataMgr::getInstance()->m_wTableID)
    {
        return;
    }
    
    //更新用户状态
    m_gameLayer->updateUserStatus(pItem, cmd->UserStatus.wChairID);
}

void GameScene::onGetUserTextChat(void *pBuffer)
{
    CMD_GR_S_UserChat *cmd = (CMD_GR_S_UserChat*)pBuffer;
    
    UserData *pItem = HallDataMgr::getInstance()->m_UserList.at(cmd->dwSendUserID);
    if (nullptr == pItem)
    {
        return;
    }
    
    if (pItem->m_date.wChairID == HallDataMgr::getInstance()->m_wChairID)
    {
        return; //收到自己的消息不处理
    }
    
    int chatLen = cmd->wChatLength;
    TCHAR szMsg[LEN_USER_CHAT];
    memset(szMsg, 0, LEN_USER_CHAT);
    memcpy(szMsg, cmd->szChatString, chatLen);
    std::string msg = WHConverUnicodeToUtf8WithArray(szMsg);
    m_gameLayer->showGameTextChat(pItem->m_date.wChairID, msg.c_str());
    
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
    
    if (pItem->m_date.wChairID == HallDataMgr::getInstance()->m_wChairID)
    {
        return; //收到自己的消息不处理
    }
    
    m_gameLayer->showGameBrowChat(pItem->m_date.wChairID, cmd->wItemIndex + 1);
    
    //聊天记录
    tagChatRecord record = {0};
    record.dwChatUserId = cmd->dwSendUserID;
    record.bBrow = true;
    record.nIdx = cmd->wItemIndex + 1;
    INSTANCE(GameDataMgr)->addRecord(record);
}

void GameScene::onGetGameStart(void *pData, WORD wSize)
{
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    
    CMD_S_GameStart *cmd = (CMD_S_GameStart*)pData;
    log("游戏开始，庄家用户:%d",cmd->wBankerUser);
    
    INSTANCE(AudioMgr)->playGameStartAudio();
    
    m_gameLayer->updateGameBanker(cmd->wBankerUser);
    
    tagCards cards = tagCards();
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        if (1 == cmd->cbPlayStatus[i])
        {
            cards = tagCards();
            cards.m_cardCount = tbnn_MAX_COUNT;
            memcpy(&cards.m_cardData, cmd->cbCardData[i], sizeof(BYTE) * tbnn_MAX_COUNT);
            
            m_gameLayer->updateUserCards(i, cards);
        }
    }
    
    //扑克发牌结束
    m_gameLayer->updateCardsOver();
}

void GameScene::onGetUserForceQuit(void *pData, WORD wSize)
{
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    
    CMD_S_PlayerExit *cmd = (CMD_S_PlayerExit*)pData;
    log("退出用户:%d",cmd->wPlayerID);
}

void GameScene::onGetDispatchCard(void *pData, WORD wSize)
{
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    log("发牌");
    CMD_S_SendCard *cmd = (CMD_S_SendCard*)pData;
    tagCards cards = tagCards();
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        cards = tagCards();
        cards.m_cardCount = tbnn_MAX_COUNT;
        memcpy(&cards.m_cardData, cmd->cbCardData[i], sizeof(BYTE) * tbnn_MAX_COUNT);
        
        m_gameLayer->updateUserCards(i, cards);
    }
    
    //扑克发牌结束
    m_gameLayer->updateCardsOver();
}

void GameScene::onGetGameEnd(void *pData, WORD wSize)
{
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    log("游戏结束");
    
    CMD_S_GameEnd *cmd = (CMD_S_GameEnd*)pData;
    
    //刷新用户得分
    for (int i = 0; i < tbnn_GAME_PLAYER; ++i)
    {
        //用户扑克
        tagCards cards = tagCards();
        cards.m_cardCount = tbnn_MAX_COUNT;
        if (0 != cmd->cbCardData[i][0])
        {
            memcpy(&cards.m_cardData, cmd->cbCardData[i], tbnn_MAX_COUNT * sizeof(BYTE));
            m_gameLayer->updateUserCards(i, cards);
        }
        
        LONGLONG score = cmd->lGameScore[i];
        if(0 != score)
        {
            m_gameLayer->updateUserScore(i, score);
        }
        
        //五花牛
        m_gameLayer->updateFiveKing(i, cmd->bFiveKing[i]);
        
        if (i == HallDataMgr::getInstance()->m_wChairID)
        {
            if (score > 0)
            {
                INSTANCE(AudioMgr)->playTBNN_GameEndAudio(1);
            }
            else if (score < 0)
            {
                INSTANCE(AudioMgr)->playTBNN_GameEndAudio(2);
            }
            else
            {
                INSTANCE(AudioMgr)->playTBNN_GameEndAudio(0);
            }
        }
    }
    
    m_gameLayer->updateGameOver();
}

void GameScene::onGetUserOpenCard(void *pData, WORD wSize)
{
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    
    CMD_S_Open_Card *cmd = (CMD_S_Open_Card*)pData;
    log("用户 %d 摊牌", cmd->wPlayerID);
    
    m_gameLayer->updateUserOpenCard(cmd->wPlayerID);
}

void GameScene::onGetSystemControl(void *pData, WORD wSize)
{
    
}

void GameScene::onGetRefreshUser(void *pData, WORD wSize)
{
    
}

void GameScene::registerNetMessage()
{
    //场景消息
    NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameScene::onGameScene, this);
    
    NetworkMgr::getInstance()->m_Userdelegate = this;
    
    NetworkMgr::getInstance()->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(GameScene::onGameMessage, this));
}

void GameScene::removeNetMessage()
{
    //网络
    NetworkMgr::getInstance()->m_GameScenecallback = nullptr;
    
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GF_GAME);
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
            //m_gameLayer->reSet();
            //m_gameLayer->reSetNewGame();
            //m_gameLayer->clearUser();
            return;
        }
    }
    INSTANCE(SceneMgr)->transitionScene(TBNN_SCENE(PLAZA_SCENE),false);
    s_bReplace = true;
}