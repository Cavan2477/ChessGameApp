//
//  tbnn_PlazaScene.cpp
//  GameProject
//
//  Created by zhong on 4/18/16.
//
//

#include "tbnn_PlazaScene.h"
USING_TBNN_NAMESPACE;

PlazaScene::PlazaScene():
m_plazaLayer(nullptr),
m_bBackToPlazz(false)
{
    INSTANCE(HallDataMgr)->toSubSystemScale();
}

PlazaScene::~PlazaScene()
{
    //资源释放
    INSTANCE(SceneMgr)->doUISceneResourceRelease(PlazaScene::uiUnloadResource());
    
    //路径
    if (m_bBackToPlazz)
    {
        INSTANCE(SpriteFrameCache)->removeSpriteFramesFromFile("public_res/public_res.plist");
        INSTANCE(Director)->getTextureCache()->removeTextureForKey("public_res/public_res.plist");
        
        INSTANCE(Director)->getTextureCache()->removeUnusedTextures();
        INSTANCE(SpriteFrameCache)->removeUnusedSpriteFrames();
        
        std::string path = FileUtils::getInstance()->getWritablePath() + "OxSixEx/";
        std::vector<std::string> paths = FileUtils::getInstance()->getSearchPaths();
        for (std::vector<std::string>::iterator ite = paths.begin(); ite != paths.end(); )
        {
            if (0 == strcmp((*ite).c_str(), path.c_str()))
            {
                ite = paths.erase(ite);
            }
            else
            {
                ++ite;
            }
        }
        FileUtils::getInstance()->setSearchPaths(paths);
    }
}

std::vector<tagUIResourceParam> PlazaScene::uiAsynLoadResource()
{
    std::vector<tagUIResourceParam> vec = std::vector<tagUIResourceParam>();
    tagUIResourceParam param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "plaza/plaza.png";
    param.plistFile = "plaza/plaza.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    return vec;
}

std::vector<tagUIResourceParam> PlazaScene::uiAsynLoadFinished()
{
    return PlazaScene::uiAsynLoadResource();
}

std::vector<tagUIResourceParam> PlazaScene::uiUnloadResource()
{
    return PlazaScene::uiAsynLoadResource();
}

bool PlazaScene::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Scene::init());
        
        m_plazaLayer = PlazaLayer::create();
        CC_BREAK_IF(nullptr == m_plazaLayer);
        m_plazaLayer->setNotifyScene(this);
        this->addChild(m_plazaLayer);
        
        bRes = true;
    } while (false);
    return bRes;
}

void PlazaScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    INSTANCE(AudioMgr)->playBackgroudAudio(true,"sound_res/backMusic.mp3");
    
    //桌子信息
    INSTANCE(TableMgr)->m_funTableCallBack = CC_CALLBACK_3(PlazaScene::onTableMsgCallBack, this);
    
    //网络
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(PlazaScene::roomloginResult, this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(PlazaScene::roomlistResult, this));
    
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_PARAMETER, CC_CALLBACK_2(PlazaScene::levelInfo, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_UPGRADE, CC_CALLBACK_2(PlazaScene::levelUpgrade, this));
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(PlazaScene::notifyRefreshScore, this)), 1);
    this->sendPacketWithUserLevelInfo();
}

void PlazaScene::onExit()
{
    if (m_bBackToPlazz)
    {
        INSTANCE(AudioMgr)->playGeneralBackgroudAudio(true);
    }
    removeNetMessage();
    //Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    
    Scene::onExit();
}

void PlazaScene::onTableMsgCallBack(const enTableCode &nCode, void *pData, const int &nSize)
{
    switch (nCode)
    {
        case kUpdateTable:
        {
            //m_plazaLayer->refreshCurrentTableInfo();
            tagTableItem *pItem = (tagTableItem*)pData;
            m_plazaLayer->refreshTableInfo(pItem);
        }
            break;
        case kGetTableInfo:
        {
            m_plazaLayer->refreshTableList();
        }
            break;
        case kUpdateTableUser:
        {
            tagTableUserInfo *info = (tagTableUserInfo*)pData;
            m_plazaLayer->refreshTableUser(*info);
        }
            break;
        default:
            break;
    }
}

bool PlazaScene::uiMsg(const int &main, const int &sub,void *pData, const int &nLen)
{
    return false;
}

void PlazaScene::removeNetMessage()
{
    //桌子信息
    INSTANCE(TableMgr)->m_funTableCallBack = nullptr;
    
    //网络
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_SERVER_LIST);
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_UPGRADE);
}

void PlazaScene::levelInfo(void *pData, WORD wSize)
{
    m_plazaLayer->levelchange(pData, wSize);
}

void PlazaScene::levelUpgrade(void *pData, WORD wSize)
{
    m_plazaLayer->scorechange(pData, wSize);
}

void PlazaScene::notifyRefreshScore(cocos2d::EventCustom *event)
{
    m_plazaLayer->notifyRefreshScore(event);
}

void PlazaScene::roomloginResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_LOGON_SUCCESS:                  //登陆成功 100
        {
            
        }
            break;
        case SUB_GR_LOGON_FAILURE:                  //登陆失败 101
        {
            CMD_GR_LogonFailure *failure = (CMD_GR_LogonFailure *)pData;
            auto str = WHConverUnicodeToUtf8WithArray(failure->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
            NetworkMgr::getInstance()->Disconnect(Data_Room);
        }
            break;
        case SUB_GR_LOGON_FINISH:                   //登陆完成 102
        {
            HallDataMgr::getInstance()->readTablepass();
            if (HallDataMgr::getInstance()->m_Tablepass != "")
            {
                //桌子可以加锁
                if ((HallDataMgr::getInstance()->m_RoomRule&SR_FORFEND_LOCK_TABLE) == 0)
                {
                    NetworkMgr::getInstance()->sendEncrypt(HallDataMgr::getInstance()->m_Tablepass);
                }
            }
            
            auto status = HallDataMgr::getInstance()->m_cbStatus;
            
            //跳到桌子界面
            if (status==US_FREE || status==US_NULL)
            {
                m_plazaLayer->showTableList(true);
            }
            
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
            //游戏状态启动游戏
            if(status==US_PLAYING || status==US_READY)
            {
                INSTANCE(SceneMgr)->preloaderScene(TBNN_SCENE(GAME_SCENE),
                                                   []()
                                                   {
                                                       NetworkMgr::getInstance()->sendGameOption();
                                                   },
                                                   false);
            }
        }
            break;
        case SUB_GR_UPDATE_NOTIFY:
        {
            HallDataMgr::getInstance()->AddpopLayer("系统提示", "当前版本不是最新版本，请升级到最新版本", Type_Ensure);
            auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Room);});
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
        }
            break;
            
        default:
            break;
    }
}

void PlazaScene::roomlistResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    if (wSubCmdID == SUB_MB_SERVER_ONLINE)
    {
        auto ponline = static_cast<CMD_MB_ServerOnline *>(pData);
        for (int index=0; index<ponline->wServerCount; ++index) {
            for (auto iter : HallDataMgr::getInstance()->m_subRoomList)
            {
                if (iter->wServerID == ponline->OnLineInfoServer[index].wServerID)
                {
                    iter->dwOnLineCount = ponline->OnLineInfoServer[index].dwOnLineCount;
                }
            }
        }
        NetworkMgr::getInstance()->Disconnect(Data_Load);
    }
}

void PlazaScene::sendPacketWithUserLevelInfo()
{
    CMD_GP_GrowLevelQueryInfo levelQueryInfo;
    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GrowLevelQueryInfo));
    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, levelQueryInfo.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}