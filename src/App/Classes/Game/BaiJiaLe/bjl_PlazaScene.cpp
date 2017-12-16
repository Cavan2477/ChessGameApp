#include "BaiJiaLeScene.h"

USING_BJL_NAMESPACE;
PlazaScene::PlazaScene():
	m_plazaLayer(nullptr),
    m_bIsReEnter(false)
{
    m_bBackToPlazz = false;
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
        
        std::string path = FileUtils::getInstance()->getWritablePath() + "JDBaiJiaLe/hd/";
        FileUtils::getInstance()->addSearchPath(path);
        std::string path2 = FileUtils::getInstance()->getWritablePath() + "JDBaiJiaLe/";
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

std::vector<tagUIResourceParam> PlazaScene::uiAsynLoadResource()
{
    std::vector<tagUIResourceParam> vec = std::vector<tagUIResourceParam>();
    tagUIResourceParam param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "plaza/plaza.png";
    param.plistFile = "plaza/plaza.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    /*
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "exchange/exchange.png";
    param.plistFile = "exchange/exchange.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "person/person.png";
    param.plistFile = "person/person.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "setting/setting.png";
    param.plistFile = "setting/setting.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
     */
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
    
    //网络
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(PlazaScene::roomloginResult, this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(PlazaScene::roomlistResult, this));
    
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_PARAMETER, CC_CALLBACK_2(PlazaScene::levelInfo, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_UPGRADE, CC_CALLBACK_2(PlazaScene::levelUpgrade, this));
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(PlazaScene::notifyRefreshScore, this)), 1);
    /*
    if (HallDataMgr::getInstance()->m_loadtype != Load_Visitor)
    {
        this->sendPacketWithUserLevelInfo();
    }
     */
    this->sendPacketWithUserLevelInfo();
    
    //INSTANCE(SpriteFrameCache)->addSpriteFramesWithFile("user_face.plist","user_face.png");
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

void PlazaScene::removeNetMessage()
{
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
    switch (wSubCmdID)
    {
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
                /*
                auto call = CallFunc::create([this]()
                                             {
                                                 m_plazaLayer->showTableList(true);
                                                 INSTANCE(LoadingMgr)->hideProgressLoading();
                                             });
                INSTANCE(LoadingMgr)->progressLoading(100,nullptr,call);
                 */
                //m_plazaLayer->showTableList(true);
                HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                INSTANCE(GameDataMgr)->requestSitdown(INVALID_TABLE,INVALID_CHAIR);
            }
            
            //游戏状态启动游戏
            if(status==US_PLAYING || status==US_READY)
            {
                INSTANCE(SceneMgr)->preloaderScene(BJL_SCENE(GAME_SCENE), []()
                                                   {
                                                       NetworkMgr::getInstance()->sendGameOption();
                                                   },
                                                   false);
                HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
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
		//m_plazaLayer->refreshTableList();
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
            /*
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, GameScene::create()));
             */
            INSTANCE(SceneMgr)->transitionScene(BJL_SCENE(GAME_SCENE),false);
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
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), levelQueryInfo.szMachineID);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}