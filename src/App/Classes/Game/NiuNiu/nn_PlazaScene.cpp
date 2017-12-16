#include "NiuNiuScene.h"
#include "PlazaSceneUI/PlazaLayer.h"

USING_NN_NAMESPACE;
PlazaScene::PlazaScene():
    m_bIsReEnter(false)
{
    INSTANCE(HallDataMgr)->toSubSystemScale();
    m_bBackToPlazz = false;
}

PlazaScene::~PlazaScene()
{
    if (m_bBackToPlazz)
    {
        INSTANCE(SpriteFrameCache)->removeSpriteFrameByName("blank.png");
        INSTANCE(Director)->getTextureCache()->removeUnusedTextures();
        INSTANCE(SpriteFrameCache)->removeUnusedSpriteFrames();        
        
        std::string path = FileUtils::getInstance()->getWritablePath() + "JDNiuNiu/hd/";
        std::string path2 = FileUtils::getInstance()->getWritablePath() + "JDNiuNiu/";
        std::vector<std::string> paths = FileUtils::getInstance()->getSearchPaths();
        for (std::vector<std::string>::iterator ite = paths.begin(); ite != paths.end(); )
        {
            if (0 == strcmp((*ite).c_str(), path.c_str()))
            {
                ite = paths.erase(ite);
            }
            else if (0 == strcmp((*ite).c_str(), path2.c_str()))
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

bool PlazaScene::init()
{
	bool bRes = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		PlazaLayer* m_plazaLayer = PlazaLayer::create();
		CC_BREAK_IF(nullptr == m_plazaLayer);
        m_plazaLayer->setName("plaza_layer");
		m_plazaLayer->setNotifyScene(this);
		this->addChild(m_plazaLayer);
		
		bRes = true;
	} while (false);
	return bRes;
}

void PlazaScene::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();
    INSTANCE(AudioMgr)->playBackgroudAudio(true,"sound_res/backaudio/backMusic.mp3");
    
    //桌子信息
    INSTANCE(TableMgr)->m_funTableCallBack = CC_CALLBACK_3(PlazaScene::onTableMsgCallBack, this);
    
    //网络
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(PlazaScene::roomloginResult, this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(PlazaScene::roomlistResult, this));
    
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_PARAMETER, CC_CALLBACK_2(PlazaScene::levelInfo, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_UPGRADE, CC_CALLBACK_2(PlazaScene::levelUpgrade, this));
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(PlazaScene::notifyRefreshScore, this)), 1);
    if (HallDataMgr::getInstance()->m_loadtype != Load_Visitor)
    {
        this->sendPacketWithUserLevelInfo();
    }
    
    
    //INSTANCE(SpriteFrameCache)->addSpriteFramesWithFile("user_face.plist","user_face.png");
}

void PlazaScene::onExit()
{
    if (m_bBackToPlazz)
    {
        INSTANCE(AudioMgr)->playGeneralBackgroudAudio(true);
    }
    
    //桌子信息
    INSTANCE(TableMgr)->m_funTableCallBack = nullptr;
    
    //网络
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_SERVER_LIST);
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_UPGRADE);
    //Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
	Scene::onExit();
}

void PlazaScene::HanderMessage(int main, int sub, BYTE *pBuffer, int size)
{
	CCLOG("PlazaSceneHandle,mian:%d,sub:%d",main,sub);
	switch (main) 
	{
	case MDM_GR_STATUS:
		onGetPlazaStatusMsg(sub,pBuffer,size);
		break;
	default:
		break;
	}
}

void PlazaScene::onTableMsgCallBack(const enTableCode &nCode, void *pData, const int &nSize)
{
    switch (nCode)
    {
        case kUpdateTable:
            if (!m_bIsReEnter)
            {
                PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
                if (nullptr != mLayer)
                {
                    tagTableItem *pItem = (tagTableItem*)pData;
                    mLayer->refreshTableInfo(pItem);
                }
            }
            break;
        case kGetTableInfo:
            if (!m_bIsReEnter)
            {
                PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
                if (nullptr != mLayer)
                {
                    mLayer->refreshTableList();
                }
            }
            break;
        case kUpdateTableUser:
        {
            if (!m_bIsReEnter)
            {
                tagTableUserInfo *info = (tagTableUserInfo*)pData;
                PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
                if (nullptr != mLayer)
                {
                    mLayer->refreshTableUser(*info);
                }
            }
        }
            break;
        default:
            break;
    }
}

bool PlazaScene::uiMsg(const int &main, const int &sub,void *pData, const int &nLen)
{
	switch (main)
	{
	case UI_MSG_BASE::UI_MSG_PLAZA:
		onGetPlazaUIMsg(sub,pData,nLen);
		break;
	default:
		break;
	}
	return true;
}

void PlazaScene::levelInfo(void *pData, WORD wSize)
{
    PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
    if (nullptr != mLayer)
    {
        mLayer->levelchange(pData, wSize);
    }
}

void PlazaScene::levelUpgrade(void *pData, WORD wSize)
{
    PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
    if (nullptr != mLayer)
    {
        mLayer->scorechange(pData, wSize);
    }
}

void PlazaScene::notifyRefreshScore(cocos2d::EventCustom *event)
{
    PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
    if (nullptr != mLayer)
    {
        mLayer->notifyRefreshScore(event);
    }
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
                PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
                if (nullptr != mLayer)
                {
                    mLayer->showTableList(true);
                }
            }
            
            //游戏状态启动游戏
            if(status==US_PLAYING || status==US_READY)
            {
                //INSTANCE(LoadingMgr)->hideProgressLoading();
                NetworkMgr::getInstance()->sendGameOption();
            }
            log("登陆完成");
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
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
            for (auto iter : HallDataMgr::getInstance()->m_roomList) {
                if (iter->wServerID == ponline->OnLineInfoServer[index].wServerID)
                {
                    iter->dwOnLineCount = ponline->OnLineInfoServer[index].dwOnLineCount;
                }
            }
        }
        //this->updateOnlineShow();
        NetworkMgr::getInstance()->Disconnect(Data_Load);
    }
}

void PlazaScene::onGetPlazaStatusMsg(const int &sub,BYTE *pData,const int &nLen)
{
	switch (sub)
	{
	case SUB_GR_TABLE_INFO:						//接收完桌子消息
        {
            PlazaLayer *mLayer = static_cast<PlazaLayer*>(this->getChildByName("plaza_layer"));
            if (nullptr != mLayer)
            {
                mLayer->refreshTableList();
            }
        }
		break;
	case SUB_GR_TABLE_STATUS:					//接受完桌子状态消息
		break;
	default:
		break;
	}
}

void PlazaScene::onGetPlazaUIMsg(const int &sub,void *pData,const int &nLen)
{
    switch (sub) {
        case PLAZA_UI::ENTER_GAME:
        {
            //cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, GameScene::create()));
        }
            break;
            
        default:
            break;
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