/************************************************************************************
 * file: 		NetworkMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:40:51
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "../Common/Project.h"
#include "../Public/Help.h"
#include "../Plaza/Plaza/LoginScene.h"

//桌子信息管理
#include "TableMgr.h"
#include "CocosHeader.h"
#include "../Scene/ModeLayer.h"
#include "../Scene/SceneHeader.h"
#include "../Scene/SceneMgr/SceneMgr.h"

static NetworkMgr *s_SharedNetwork = NULL;

NetworkMgr::NetworkMgr()
:m_pSocketData(nullptr)
,m_pSocketOnce(nullptr)
,m_Userdelegate(nullptr)
,m_GameScenecallback(nullptr)
{
    this->registerloadfunction(SUB_KN_DETECT_SOCKET, CC_CALLBACK_3(NetworkMgr::networkszCompilatioFalut, this));
    //用户服务
    this->registerloadfunction(MDM_GP_USER_SERVICE, CC_CALLBACK_3(NetworkMgr::networkUserService, this));
    //游戏房间配置
    this->registerroomfunction(MDM_GR_CONFIG, CC_CALLBACK_3(NetworkMgr::roomconfigResult, this));
    //用户处理
    this->registerroomfunction(MDM_GR_USER, CC_CALLBACK_3(NetworkMgr::networkGRUser, this));
    //桌子状态
    this->registerroomfunction(MDM_GR_STATUS, CC_CALLBACK_3(NetworkMgr::networkGRStatus, this));
    //框架消息
    this->registerroomfunction(MDM_GF_FRAME, CC_CALLBACK_3(NetworkMgr::networkGRFrame, this));
    //游戏消息
    //this->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(NetworkMgr::networkGRGame, this));
    //系统消息
    this->registerroomfunction(MDM_CM_SYSTEM, CC_CALLBACK_3(NetworkMgr::networkGRSystem, this));
}

NetworkMgr::~NetworkMgr()
{
    this->unregisterloadfunction(SUB_KN_DETECT_SOCKET);
    this->unregisterloadfunction(MDM_GP_USER_SERVICE);
    
    this->unregisterroomfunction(MDM_GR_CONFIG);
    this->unregisterroomfunction(MDM_GR_USER);
    this->unregisterroomfunction(MDM_GR_STATUS);
    this->unregisterroomfunction(MDM_GF_FRAME);
    //this->unregisterroomfunction(MDM_GF_GAME);
    this->unregisterroomfunction(MDM_CM_SYSTEM);
    
    CC_SAFE_RELEASE_NULL(m_pSocketOnce);
    CC_SAFE_RELEASE_NULL(m_pSocketData);
}

NetworkMgr* NetworkMgr::getInstance()
{
    if (s_SharedNetwork == NULL) {
        s_SharedNetwork = new NetworkMgr();
    }
    return s_SharedNetwork;
}

CTCPSocket * NetworkMgr::getSocketData() const
{
    return m_pSocketData;
}

CTCPSocket * NetworkMgr::getSocketOnce() const
{
    return  m_pSocketOnce;
}

void NetworkMgr::doConnect(const char *domain, WORD wPort, EM_DATA_TYPE type)
{
    CTCPSocket *socket = new CTCPSocket();
    this->Disconnect(type);
    if (type == EM_DATA_TYPE_ROOM) {
        m_pSocketData = socket;
    }
    if (type == EM_DATA_TYPE_LOAD) {
        m_pSocketOnce = socket;
    }
    socket->socketConnect(domain, wPort, type);
    socket->setSocketTarget(CC_CALLBACK_4(NetworkMgr::SocketDelegateWithRecvData, this));
    this->sendPacket_Compilatio(socket);
}

void NetworkMgr::Disconnect(EM_DATA_TYPE type)
{
    if (type == EM_DATA_TYPE_LOAD) {
        if (m_pSocketOnce) {
            m_pSocketOnce->socketClose();
            m_pSocketOnce->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]{
                m_pSocketOnce->autorelease();
            })));
        }
        m_pSocketOnce = nullptr;
    }
    if (type == EM_DATA_TYPE_ROOM) {
        if (m_pSocketData) {
            m_pSocketData->socketClose();
            m_pSocketData->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]{
                m_pSocketData->autorelease();
            })));
        }
        m_pSocketData = nullptr;
    }
}

void NetworkMgr::SocketDelegateWithRecvData(void *socket, void *pData, WORD wSize, bool isinBack)
{
    CTCPSocket* pScoket=(CTCPSocket*)socket;
    
    //数据转换
    _stTcpBuffer tcpBuffer;
    memset(&tcpBuffer,0,sizeof(_stTcpBuffer));
    memcpy(&tcpBuffer,pData,wSize);
    
    //命令码
    uint wMainCmdID = tcpBuffer.Head.stTCPCmd.wMainCmdID;
    uint wSubCmdID	= tcpBuffer.Head.stTCPCmd.wSubCmdID;
    void* buffer = tcpBuffer.cbBuffer;
    uint size = tcpBuffer.Head.stTCPInfo.wPacketSize - sizeof(_stTcpHead);
    
    CCLOG("MainCmdID:%d,SubCmdID:%d",wMainCmdID,wSubCmdID);
    switch (pScoket->getData())
    {
        case EM_DATA_TYPE_DEFAULT:
            CCAssert(false,"无效状态");
            break;
        case EM_DATA_TYPE_LOAD:
        {
            auto iter = m_loadfunction.find(wMainCmdID);
            if (iter != m_loadfunction.end()) {
                iter->second(wSubCmdID,buffer,size);
            }
        }
            break;
        case EM_DATA_TYPE_ROOM:
        {
            auto iter = m_roomfunction.find(wMainCmdID);
            if (iter != m_roomfunction.end()) {
                if (isinBack) {
                    if(wMainCmdID == MDM_GF_GAME)
                    {
                        return;
                    }
                }
                iter->second(wSubCmdID,buffer,size);
            }
        }
            break;
        default:
            //CCAssert(false, "状态分类:未处理的case状态");
            break;
    }
}

void NetworkMgr::sendData(WORD wMainCmdID, WORD wSubCmdID, void *buffer, WORD wSize, CTCPSocket* socket)
{
    CTCPSocket *psocket = socket;
    if (socket == NULL) {
        psocket = m_pSocketData;
    }
    if (psocket) {
        _stTcpBuffer tcp_buffer;
        memset(&tcp_buffer,0,sizeof(_stTcpBuffer));
        tcp_buffer.Head.stTCPCmd.wMainCmdID=wMainCmdID;
        tcp_buffer.Head.stTCPCmd.wSubCmdID=wSubCmdID;
        
        memcpy(&tcp_buffer.cbBuffer,buffer,wSize);
        
        psocket->socketSend((char*)&tcp_buffer, wSize+sizeof(_stTcpHead));
    }
}

void NetworkMgr::registerloadfunction(WORD wMainCmdID, gameMessageCallback callback)
{
    this->unregisterloadfunction(wMainCmdID);
    m_loadfunction.insert(std::make_pair(wMainCmdID,callback));
}

void NetworkMgr::unregisterloadfunction(WORD wMainCmdID)
{
    auto iter = m_loadfunction.find(wMainCmdID);
    if (iter != m_loadfunction.end()) {
        m_loadfunction.erase(iter);
    }
}

void NetworkMgr::registeruserfunction(WORD wSubCmdID, MessageCallback1 callback)
{
    this->unregisteruserfunction(wSubCmdID);
    m_Userfunction.insert(std::make_pair(wSubCmdID,callback));
}

void NetworkMgr::unregisteruserfunction(WORD wSubCmdID)
{
    auto iter = m_Userfunction.find(wSubCmdID);
    if (iter != m_Userfunction.end()) {
        m_Userfunction.erase(iter);
    }
}

void NetworkMgr::registerroomfunction(WORD wMainCmdID, gameMessageCallback callback)
{
    this->unregisterroomfunction(wMainCmdID);
    m_roomfunction.insert(std::make_pair(wMainCmdID, callback));
}

void NetworkMgr::unregisterroomfunction(WORD wMainCmdID)
{
    auto iter = m_roomfunction.find(wMainCmdID);
    if (iter != m_roomfunction.end()) {
        m_roomfunction.erase(iter);
    }
}

void NetworkMgr::networkszCompilatioFalut(WORD wSubCmdID, void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("系统提示", "授权码错误", Type_Ensure);
    this->Disconnect(EM_DATA_TYPE_LOAD);
}

void NetworkMgr::networkUserService(WORD wSubCmdID, void *pData, WORD wSize)
{
    auto iter = m_Userfunction.find(wSubCmdID);
    if (iter != m_Userfunction.end()) {
        iter->second(pData,wSize);
    }
}

void NetworkMgr::roomconfigResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    if (wSubCmdID == SUB_GR_CONFIG_SERVER) {
        if (wSize != sizeof(ST_CMD_GR_CONFIG_GAME_ROOM)) {
            return;
        }
        auto server = (ST_CMD_GR_CONFIG_GAME_ROOM *)pData;
        HallDataMgr::getInstance()->m_RoomRule = server->dwGameRoomRule;
        HallDataMgr::getInstance()->m_Gametype = server->wGameRoomType;
        
        HallDataMgr::getInstance()->m_TableCount = server->wTableCount;
        HallDataMgr::getInstance()->m_ChairCount = server->wChairCount;
    }
}

void NetworkMgr::networkGRUser(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_USER_WAIT_DISTRIBUTE:
        {
            
        }
            break;
        case SUB_GR_USER_ENTER:
        {
            this->OnUserEnter(pData, wSize);
        }
            break;
        case SUB_GR_USER_STATUS:
        {
            this->OnUserStatus(pData, wSize);
        }
            break;
        case SUB_GR_USER_GAME_COIN:
        {
            this->OnUserScore(pData, wSize);
        }
            break;
        case SUB_GR_PROPERTY_TRUMPET:       //喇叭消息  305
        {
            auto result = (ST_CMD_GR_SERVER_SEND_TRUMPET*)pData;
            TrumpetData *phorntext = new TrumpetData();

			phorntext->sendnickname = WHConverUnicodeToUtf8WithArray((WORD*)result->szSendNickName) + ":";
			phorntext->sendtext = WHConverUnicodeToUtf8WithArray((WORD*)result->szTrumpetContent);

            //金币更新
            EventCustom event(whNd_Horn_Message);
            event.setUserData(phorntext);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            phorntext->autorelease();
        }
            break;
        case SUB_GR_PROPERTY_FAILURE://道具失败
        {
            auto presult = (ST_CMD_GR_SERVER_PROPERTY_FAILURE*)pData;
            auto str = WHConverUnicodeToUtf8WithArray((WORD*)presult->szDes);
            HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
        }
            break;
        case SUB_GR_REQ_FAILURE:
        {
            auto result = (ST_CMD_GR_REQ_FAILURE *)pData;
			auto str = WHConverUnicodeToUtf8WithArray((WORD*)result->szDes);
            HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
        }
            break;
            
        default:
            break;
    }
}

void NetworkMgr::OnUserEnter(void *pData, WORD wSize)
{
    if(wSize<sizeof(_stMobileUserHeadInfo)) 
		return;

    auto result = ( _stMobileUserHeadInfo*)pData;

	//旁观暂不处理
    if (result->cbUserStatus == US_LOOKON)
        return;

    UserData *puser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);

    if (puser)
        puser->UpdateData(pData);
    else
    {
        puser = new UserData(pData);
        HallDataMgr::getInstance()->m_UserList.insert(result->dwUserID, puser);
    }
    
    if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID) 
	{
        HallDataMgr::getInstance()->m_cbStatus = result->cbUserStatus;
        HallDataMgr::getInstance()->m_wChairID = result->wChairID;
        HallDataMgr::getInstance()->m_wTableID = result->wTableID;
        HallDataMgr::getInstance()->m_wCustom = result->dwCustomID;
        HallDataMgr::getInstance()->m_wFaceID = result->dwFaceID;

        //进入的房间类型(积分房、练习房不更新玩家实际积分)
        int enterRoom = INSTANCE(GameDataMgr)->getEnterRoom();

        if (enterRoom >= 0 && enterRoom < HallDataMgr::getInstance()->m_subRoomList.size())
        {
            _stGameRoomServer *pServer = HallDataMgr::getInstance()->m_subRoomList.at(INSTANCE(GameDataMgr)->getEnterRoom());

            if (nullptr != pServer)
            {
                if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID && 
                    pServer->wServerType != GAME_GENRE_SCORE && 
                    pServer->wServerType != GAME_GENRE_EDUCATE)
                    HallDataMgr::getInstance()->m_llUserGold = result->llGameCoin;
            }
        }

        HallDataMgr::getInstance()->m_llGold = result->llGold;
    }
    
    //无桌子界面的游戏
    if (INSTANCE(TableMgr)->haveTableList())
    {
        //桌椅信息更新
        tagTableUserInfo tagInfo = tagTableUserInfo();
        tagInfo.wUserId = result->dwUserID;
        tagInfo.tagStatus.cbUserStatus = result->cbUserStatus;
        tagInfo.tagStatus.wTableID = result->wTableID;
        tagInfo.tagStatus.wChairID = result->wChairID;
        INSTANCE(TableMgr)->updateTableItem(tagInfo);
    }
    
    if (m_Userdelegate)
        m_Userdelegate->OnUserEnter(puser);

    cocos2d::log("用户进入%s", puser->m_nickname.c_str());
}

void NetworkMgr::OnUserStatus(void *pData, WORD wSize)
{
    if(wSize!=sizeof(ST_CMD_GR_USER_STATUS)) 
		return;

    auto result = (ST_CMD_GR_USER_STATUS *)pData;

    UserData *puser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);

    if (!puser) 
	{
        this->sendRequestUserInfo(result->dwUserID, result->stUserStatus.wTableID);
        return;
    }

    puser->UpdateData(result);
    
    if (INSTANCE(TableMgr)->haveTableList())
    {
        //桌椅信息更新
        tagTableUserInfo tagInfo = tagTableUserInfo();
        tagInfo.wUserId = result->dwUserID;
        tagInfo.tagStatus = result->stUserStatus;
        INSTANCE(TableMgr)->updateTableItem(tagInfo);
    }
    
    if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID) 
	{
        HallDataMgr::getInstance()->m_cbStatus = result->stUserStatus.cbUserStatus;
        HallDataMgr::getInstance()->m_wChairID = result->stUserStatus.wChairID;
        HallDataMgr::getInstance()->m_wTableID = result->stUserStatus.wTableID;
        
        //离开游戏
        if (result->stUserStatus.cbUserStatus==US_FREE && HallDataMgr::getInstance()->m_bStartGame) 
		{
            HallDataMgr::getInstance()->m_bStartGame = false;

             //场景切换
            if (INSTANCE(GameDataMgr)->getIsChangeDesk())
            {
                INSTANCE(GameDataMgr)->setIsChangeDesk(false);
                Scene *s = INSTANCE(SceneMgr)->getTempSceneByKind(HallDataMgr::getInstance()->m_dwKindID);

                if (nullptr != s)
                    Director::getInstance()->replaceScene(s);
            }
            else if(INSTANCE(GameDataMgr)->getEnterAntiCheat())
            {
                log("防作弊房间");
                Scene *s = INSTANCE(SceneMgr)->getTempSceneByKind(HallDataMgr::getInstance()->m_dwKindID);

                if (nullptr != s)
                    Director::getInstance()->replaceScene(s);
            }
            else
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
        }
        
        if (result->stUserStatus.cbUserStatus>US_FREE && !HallDataMgr::getInstance()->m_bStartGame)
        {
            switch (HallDataMgr::getInstance()->m_dwKindID)
            {
                case EM_GAME_BAIJIALE:
                {
                    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
                    INSTANCE(SceneMgr)->preloaderScene(BJL_SCENE(GAME_SCENE),
                                                       []()
                                                       {
                                                           NetworkMgr::getInstance()->sendGameOption();
                                                       },
                                                       false);
                }
                    break;
                case EM_GAME_LKPY:
                {
//                  GameDataMgr::getInstance();
                    if(!HallDataMgr::getInstance()->m_isFirstSendGameOption)
						this->sendGameOption();

                    HallDataMgr::getInstance()->m_isFirstSendGameOption = true;
                }
                    break;

                case EM_GAME_TBNN:
                {
                    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
                    INSTANCE(SceneMgr)->preloaderScene(TBNN_SCENE(GAME_SCENE),
                                                       []()
                                                       {
                                                           NetworkMgr::getInstance()->sendGameOption();
                                                       },
                                                       false);
                }
                    break;

                default:
                    this->sendGameOption();
                    break;
            }
        }
    }
    
    if (m_Userdelegate) {
        m_Userdelegate->OnUserStatus(puser);
    }
    cocos2d::log("用户状态%s", puser->m_nickname.c_str());
}

void NetworkMgr::OnUserScore(void *pData, WORD wSize)
{
    //比赛类型
    if(wSize==sizeof(ST_CMD_GR_USER_SCORE))
    {
        
    }
    //自由类型
    else if (wSize==sizeof(ST_CMD_GR_MOBILE_USER_GAME_COIN))
    {
        auto result = (ST_CMD_GR_MOBILE_USER_GAME_COIN *)pData;
        
        //进入的房间类型(积分房、练习房不更新玩家实际积分)
        int enterRoom = INSTANCE(GameDataMgr)->getEnterRoom();
        if (enterRoom >= 0 && enterRoom < HallDataMgr::getInstance()->m_subRoomList.size())
        {
            _stGameRoomServer *pServer = HallDataMgr::getInstance()->m_subRoomList.at(INSTANCE(GameDataMgr)->getEnterRoom());
            if (nullptr != pServer)
            {
                if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID
                    && pServer->wServerType != GAME_GENRE_SCORE
                    && pServer->wServerType != GAME_GENRE_EDUCATE)
                {
                    HallDataMgr::getInstance()->m_llUserGold = result->stMobileUserGameCoin.llGameCoin;
                    HallDataMgr::getInstance()->m_dBean = result->stMobileUserGameCoin.dBean;
                }
            }
        }
        
        auto puser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);
        if (puser)
        {
            puser->UpdateData(result);
            
            if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID && nullptr != m_Userdelegate)
            {
                m_Userdelegate->OnUserScore(puser);
            }
        }
    }
}

void NetworkMgr::networkGRStatus(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_TABLE_INFO:
        {
            auto result = (ST_CMD_GR_TABLE_INFO *)pData;
            memcpy(&HallDataMgr::getInstance()->m_Tableinfo, pData, wSize);
            for (int index=0; index<result->wTableCount; ++index) {
                if (result->stTableStatusArray[index].cbTableLock) {
                    auto value = cocos2d::__Integer::create(index);
                    cocos2d::__NotificationCenter::getInstance()->postNotification(whEvent_Desk_Status, value);
                }
            }
            
            ST_CMD_GR_TABLE_INFO *cmd = (ST_CMD_GR_TABLE_INFO*)pData;
            if (nullptr == cmd)
            {
                break;
            }
            INSTANCE(TableMgr)->resetList();
            if (INSTANCE(TableMgr)->haveTableList())
            {
                for (int i = 0; i < cmd->wTableCount; ++i)
                {
                    INSTANCE(TableMgr)->addTableItem(&cmd->stTableStatusArray[i],i);
                }
                //根据游戏类型进行分类
                INSTANCE(TableMgr)->grouping(HallDataMgr::getInstance()->m_dwKindID);
            }
        }
            break;
        case SUB_GR_TABLE_STATUS:
        {
            auto ptable = (ST_CMD_GR_TABLE_STATUS *)pData;
            HallDataMgr::getInstance()->m_Tableinfo.stTableStatusArray[ptable->wTableID] = ptable->stTableStatus;
            
            auto value = cocos2d::__Integer::create(ptable->wTableID);
            cocos2d::__NotificationCenter::getInstance()->postNotification(whEvent_Desk_Status, value);
            
            if (INSTANCE(TableMgr)->haveTableList())
            {
                //桌椅信息更新
                INSTANCE(TableMgr)->updateTableItem(&ptable->stTableStatus,ptable->wTableID);
            } 
        }
            break;
        default:
            break;
    }
}

void NetworkMgr::networkGRFrame(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GF_GAME_STATUS:
        {
            this->OnGameStatus(pData, wSize);
        }
            break;
        case SUB_GF_GAME_SCENE:
        {
            this->OnGameScene(pData, wSize);
        }
            break;
        case SUB_GF_SYSTEM_MESSAGE://系统消息
        {
            
        }
            break;
        case SUB_GF_USER_CHAT://用户聊天
        {
            if (m_Userdelegate)
            {
                m_Userdelegate->OnUserChat(wSubCmdID, pData, wSize);
            }
        }
            break;
        case SUB_GF_USER_EXPRESSION://用户表情
        {
            if (m_Userdelegate)
            {
                m_Userdelegate->OnUserChat(wSubCmdID, pData, wSize);
            }
        }
            break;
            
        default:
            break;
    }
}

void NetworkMgr::OnGameStatus(void *pData, WORD wSize)
{
    if(wSize!=sizeof(ST_CMD_GF_GAME_STATUS)) return;
    
    auto presult = (ST_CMD_GF_GAME_STATUS *)pData;
    HallDataMgr::getInstance()->m_cbGameStatus = presult->cbGameStatus;
    HallDataMgr::getInstance()->m_bOptionLookOn = presult->cbAllowLookon;
    log("游戏场景状态%d",presult->cbGameStatus);
    switch (HallDataMgr::getInstance()->m_dwKindID)
    {
        case EM_GAME_BAIJIALE:
        {
            INSTANCE(SceneMgr)->transitionScene(BJL_SCENE(GAME_SCENE),false);
        }
            break;
        case EM_GAME_NIUNIU:
        {
            auto scene = NN_NAMESPACE_FUN(GameScene)::create();
            Director::getInstance()->replaceScene(scene);
        }
            break;
        case EM_GAME_LKPY:
        {
            auto presult = (ST_CMD_GF_GAME_STATUS *)pData;
            HallDataMgr::getInstance()->m_cbGameStatus = presult->cbGameStatus;
            HallDataMgr::getInstance()->m_bOptionLookOn = presult->cbAllowLookon;
	            
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, lkpy_game::GameLodingLayer::createScene()));
        }
            break;
        case EM_GAME_BRNN:
        {
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, brnn_game::GameLayer::createScene()));
        }
            break;
        case EM_GAME_ZJH:
        {
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, zjh_game::GameLayer::createScene()));
        }
            break;
        case EM_GAME_TBNN:
        {
            INSTANCE(SceneMgr)->transitionScene(TBNN_SCENE(GAME_SCENE),false);
        }
            break;
        default:
            break;
    }
}

void NetworkMgr::OnGameScene(void *pData, WORD wSize)
{
    if (m_GameScenecallback)
    {
        m_GameScenecallback(pData, wSize);
    }
    
//    int nsize = sizeof(GameScene);
//    auto presult = (GameScene *)pData;
//    lkpy_game::GameDataMgr::getInstance()->m_sceneData = *presult;
//    int multipeArray[6] = {1,10,100,500,1000,5000};
//    for (int i = 0; i < 6; i++) {
//        GameDataMgr::getInstance()->m_sceneData.nMultipleValue[i] = multipeArray[i];
//    }
//    lkpy_game::GameScene gs = GameDataMgr::getInstance()->m_sceneData;
//    lkpy_game::GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
//    lkpy_game::GameDataMgr::getInstance()->initgameMessage();
}

void NetworkMgr::networkGRGame(WORD wSubCmdID, void *pData, WORD wSize)
{
    
}

void NetworkMgr::networkGRSystem(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_CM_MSG_SYS://系统消息
        {
            auto presult = (ST_CMD_SYS_MSG *)pData;
            if ((presult->wType&SMT_CLOSE_ROOM) || (presult->wType&SMT_CLOSE_GAME) || (presult->wType&SMT_CLOSE_LINK)) {
				auto pstr = WHConverUnicodeToUtf8WithArray((WORD*)presult->szMsg);
                auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("系统提示", pstr, Type_Ensure));
                player->setEnsureCallback([=]{
                    if (!Director::getInstance()->getRunningScene()->getChildByTag(10)) {
                        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Login::createScene()));
                        this->Disconnect(EM_DATA_TYPE_ROOM);
                        //播放通用大厅音乐
                        INSTANCE(AudioMgr)->playGeneralBackgroudAudio(true);
                    }
                });
            }
            else if (presult->wType&SMT_EJECT)
            {
				auto pstr = WHConverUnicodeToUtf8WithArray((WORD*)presult->szMsg);
                HallDataMgr::getInstance()->AddpopLayer("系统提示", pstr, Type_Ensure);
            }
            else if(presult->wType&SMT_CHAT)
            {
				std::string chatstr = WHConverUnicodeToUtf8WithArray((WORD*)presult->szMsg);
                std::string::iterator new_end = remove_if(chatstr.begin(), chatstr.end(), bind2nd(std::equal_to<char>(), '\r'));
                chatstr.erase(new_end, chatstr.end());
                new_end = remove_if(chatstr.begin(), chatstr.end(), bind2nd(std::equal_to<char>(), '\n'));
                chatstr.erase(new_end, chatstr.end());
                auto pstr = __String::create(chatstr);
                HallDataMgr::getInstance()->m_chatlist.pushBack(pstr);
                if (HallDataMgr::getInstance()->m_chatlist.size()>5) {
                    HallDataMgr::getInstance()->m_chatlist.erase(0);
                }
                EventCustom event(whNd_Sys_Chat);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
        }
            break;
        case SUB_CM_SYS_DOWNLOAD_MODULE://下载消息
            break;
            
        default:
            break;
    }
}

void NetworkMgr::sendPacket_Compilatio(CTCPSocket *socket)
{
    _stTcpValidate validate;
    memset(&validate, 0, sizeof(_stTcpValidate));
	Utf8ToUtf16(szCompilatio, (WORD*)validate.szValidateKey);
    
    this->sendData(MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, &validate, sizeof(_stTcpValidate),socket);
    
    CCLOG("		发送验证");
}

void NetworkMgr::sendAccountLogin()
{
    //变量定义
    ST_CMD_MB_LOGON_ACCOUNTS stCmdMbLogonAccounts;
    memset(&stCmdMbLogonAccounts,0,sizeof(ST_CMD_MB_LOGON_ACCOUNTS));

    stCmdMbLogonAccounts.dwPlazaVersion = VERSION_MOBILE;
    stCmdMbLogonAccounts.wModuleID = INVALID_WORD/*HallDataMgr::getInstance()->m_dwKindID*/;
    stCmdMbLogonAccounts.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strAccounts, (WORD*)stCmdMbLogonAccounts.szLogonAccounts);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPwd, (WORD*)stCmdMbLogonAccounts.szLogonPwd);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strMachineID, (WORD*)stCmdMbLogonAccounts.szMachineID);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPhone, (WORD*)stCmdMbLogonAccounts.szMobilePhone);

    this->sendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &stCmdMbLogonAccounts, sizeof(ST_CMD_MB_LOGON_ACCOUNTS),this->m_pSocketOnce);

    CCLOG("%s","发送帐号登录");
}

void NetworkMgr::sendMethodLogin(int platform)
{
    ST_CMD_MB_LOGON_OTHER_PLATFORM otherPlatform;
    memset(&otherPlatform, 0, sizeof(otherPlatform));
    
    otherPlatform.cbPlatformID = platform;
    otherPlatform.wModuleID = INVALID_WORD/*HallDataMgr::getInstance()->m_dwKindID*/;
    otherPlatform.dwPlazaVersion = VERSION_MOBILE;
    otherPlatform.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    otherPlatform.cbGender = HallDataMgr::getInstance()->m_cbGender;
    
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strAccounts, (WORD*)otherPlatform.szUserUin);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strMachineID, (WORD*)otherPlatform.szMachineID);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strNickName, (WORD*)otherPlatform.szNickName);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_MB_LOGON, SUB_MB_LOGON_OTHERPLATFORM, &otherPlatform, sizeof(otherPlatform), NetworkMgr::getInstance()->getSocketOnce());
}

void NetworkMgr::sendRegister()
{
    //帐号注册
    ST_CMD_MB_REG_ACCOUNTS stCmdMbRegAccounts;
    memset(&stCmdMbRegAccounts,0,sizeof(ST_CMD_MB_REG_ACCOUNTS));

    stCmdMbRegAccounts.wModuleID = INVALID_WORD/*HallDataMgr::getInstance()->m_dwKindID*/;
    stCmdMbRegAccounts.dwPlazaVersion = VERSION_MOBILE;
    stCmdMbRegAccounts.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    stCmdMbRegAccounts.wFaceID = HallDataMgr::getInstance()->m_wFaceID;
    stCmdMbRegAccounts.cbGender = HallDataMgr::getInstance()->m_cbGender;
    
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strAccounts, (WORD*)stCmdMbRegAccounts.szAccounts);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPwd, (WORD*)stCmdMbRegAccounts.szLogonPwd);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strMachineID, (WORD*)stCmdMbRegAccounts.szMachineID);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPhone, (WORD*)stCmdMbRegAccounts.szMobilePhone);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strAccounts, (WORD*)stCmdMbRegAccounts.szNickName);

    this->sendData(MDM_MB_LOGON, SUB_MB_REGISITER_ACCOUNTS, &stCmdMbRegAccounts, sizeof(ST_CMD_MB_REG_ACCOUNTS),this->m_pSocketOnce);
}

void NetworkMgr::sendRoomLogin(const std::string &roompass)
{
    ST_CMD_GR_LOGON_MOBILE stCmdGrLogonMobile;
    memset(&stCmdGrLogonMobile, 0, sizeof(stCmdGrLogonMobile));
    
    stCmdGrLogonMobile.wGameID = HallDataMgr::getInstance()->m_dwKindID;
    stCmdGrLogonMobile.dwProcessVersion = VERSION_MOBILE;
    stCmdGrLogonMobile.wBehaviorFlags = (BEHAVIOR_LOGON_NORMAL|VIEW_MODE_ALL);//HallDataMgr::getInstance()->m_bStartType?(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_ALL):(BEHAVIOR_LOGON_NORMAL|VIEW_MODE_ALL);
    stCmdGrLogonMobile.wPageTableCount = PAGE_TABLE_COUNT;
    stCmdGrLogonMobile.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    stCmdGrLogonMobile.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strMachineID, (WORD*)stCmdGrLogonMobile.szMachineID);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strDynamicPwd, (WORD*)stCmdGrLogonMobile.szDynamicPwd);
	Utf8ToUtf16(roompass, (WORD*)stCmdGrLogonMobile.szGameRoomPwd);
    
    NetworkMgr::getInstance()->sendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &stCmdGrLogonMobile, sizeof(stCmdGrLogonMobile));
    log("%s","房间登录");
}

void NetworkMgr::sendGameOption()
{
	ST_CMD_GF_GAME_CONFIG stCmdGfGameConfig = {0};

    stCmdGfGameConfig.cbAllowLookon = false;
    stCmdGfGameConfig.dwFrameVersion = INVALID_DWORD;
    stCmdGfGameConfig.dwClientVersion = INVALID_DWORD;
    
    this->sendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &stCmdGfGameConfig, sizeof(stCmdGfGameConfig));
}

void NetworkMgr::sendEncrypt(const std::string &strPwd)
{
    int size = sizeof(ST_CMD_GR_USER_RULE);
    
	BYTE byteBuff[SOCKET_TCP_BUFFER] = {0};
    memset(byteBuff, 0, sizeof(byteBuff));
    
    int length = (int)strPwd.size()+1;

	TCHAR szDes[length] = {0};

    Utf8ToUtf16Byte((BYTE*)strPwd.c_str(), szDes);
    
    BYTE *tbuffer = byteBuff+size;

    _stUserDataExt* datadescribe=(_stUserDataExt*)tbuffer;

    datadescribe->wDataDesc=DTP_GR_TABLE_PWD;
    datadescribe->wDataSize=length*sizeof(TCHAR);
    
    size += sizeof(_stUserDataExt);
    memcpy(byteBuff+size, szDes, datadescribe->wDataSize);
    
    size += datadescribe->wDataSize;
    this->sendData(MDM_GR_USER, SUB_GR_USER_RULE, byteBuff, size);
}

void NetworkMgr::sendRequestUserInfo(DWORD userID, WORD tableID)
{
    ST_CMD_GR_USER_INFO_REQ request;
    memset(&request, 0, sizeof(request));
    
    request.dwUserIDReq = userID;
    request.wTablePos = tableID;
    
    this->sendData(MDM_GR_USER, SUB_GR_USER_INFO_REQ, &request, sizeof(request));
}

void NetworkMgr::sendRequestChairUserInfo(WORD tableID, WORD chairID)
{
    ST_CMD_GR_USER_INFO_CHAIR_REQ request;
    memset(&request, 0, sizeof(request));
    
    request.wChairID = chairID;
    request.wTableID = tableID;
    
    this->sendData(MDM_GR_USER, SUB_GR_USER_CHAIR_INFO_REQ, &request, sizeof(request));
}

void NetworkMgr::sendSitDown(WORD tableID, WORD chairID, const std::string &pass)
{
    ST_CMD_GR_USER_SIT_DOWN stCmdGrUserSitDown;
    memset(&stCmdGrUserSitDown, 0, sizeof(stCmdGrUserSitDown));
    
    stCmdGrUserSitDown.wChairID = chairID;
    stCmdGrUserSitDown.wTableID = tableID;

	Utf8ToUtf16(pass, (WORD*)stCmdGrUserSitDown.szTablePwd);
    
    this->sendData(MDM_GR_USER, SUB_GR_USER_SIT_DOWN, &stCmdGrUserSitDown, sizeof(stCmdGrUserSitDown));
}

void NetworkMgr::sendLeaveGame()
{
    ST_CMD_GR_USER_STAND_UP stCmdGrUserStandUp;
    memset(&stCmdGrUserStandUp, 0, sizeof(stCmdGrUserStandUp));
    
    stCmdGrUserStandUp.wTableID = HallDataMgr::getInstance()->m_wTableID;
    stCmdGrUserStandUp.wChairID = HallDataMgr::getInstance()->m_wChairID;

    stCmdGrUserStandUp.cbForceLeave = true;
    
    NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STAND_UP, &stCmdGrUserStandUp, sizeof(stCmdGrUserStandUp));
}

void NetworkMgr::sendCustomFaceInfo(cocos2d::Image *pimage)
{
    auto pdate = pimage->getData();
    int length = (int)pimage->getDataLen();
    
    char byte[length];
    memset(byte, 0, length);

    for(int i=0; i<length/4;i++)
    {
        byte[i*4]=pdate[i*4+2];
        byte[i*4+1]=pdate[i*4+1];
        byte[i*4+2]=pdate[i*4];
        byte[i*4+3]=255;

        log("%c", byte[i*4]);
    }
    
	_stCmdGpCustomFaceInfo stCmdGpCustomFaceInfo = { 0 };

	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPwd, (WORD*)stCmdGpCustomFaceInfo.szPwd);

    stCmdGpCustomFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(stCmdGpCustomFaceInfo.dwCustomFace, byte, length);

    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CUSTOM_FACE_INFO, &stCmdGpCustomFaceInfo, sizeof(_stCmdGpCustomFaceInfo), NetworkMgr::getInstance()->getSocketOnce());
}

void NetworkMgr::bindingMachine(const BYTE &cbBind, const std::string &strpass)
{
    _stCmdGpModifyMachine cmd = {0};

    cmd.cbBind = cbBind;
    cmd.dwUserID = HallDataMgr::getInstance()->m_dwUserID;

	Utf8ToUtf16(MD5Encrypt(strpass), (WORD*)cmd.szPwd);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strMachineID, (WORD*)cmd.szMachineID);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_MACHINE, &cmd, sizeof(_stCmdGpModifyMachine),NetworkMgr::getInstance()->getSocketOnce());
}