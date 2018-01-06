/************************************************************************************
 * file: 		HallDataMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:15:13
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#include "HallDataMgr.h"

#include "../Plaza/Plaza/FramList.h"
#include "../Common/Project.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

static HallDataMgr *s_SharedHallData = NULL;

HallDataMgr::HallDataMgr()
:m_bRemember(true)
,m_bAutomatic(false)
,m_bStartGame(false)
,m_bStartType(false)
,m_popLayer(nullptr)
,m_cbGameStatus(US_NULL)
,m_RoomType(EM_DATA_TYPE_LOAD)
,m_TableCount(0)
,m_ChairCount(0)
,m_isEnterBack(false)
,m_roomaddress("")
,m_roomPass("")
,m_roomPort(0)
,m_IsLogin(true)
,m_isFirstSendGameOption(false)
,_versionCheckcallback(nullptr)
{

    m_Machine = WHGetMacAddress();
    m_dwKindID = LOGON_KIND_ID;
    memset(&m_Tableinfo, 0, sizeof(m_Tableinfo));
    memset(&m_levelData, 0, sizeof(m_levelData));
    m_levelData.dwNextLevelExp = 1;
    
    m_tagSubParam = tagSubSystemParam();
    
    readSelectServer();
}

HallDataMgr::~HallDataMgr()
{
    m_Headlist.clear();
    m_UserList.clear();
    m_chatlist.clear();
    this->roomlistclear();
}

HallDataMgr* HallDataMgr::getInstance()
{
    if (s_SharedHallData == NULL) {
        s_SharedHallData = new HallDataMgr();
    }
    return s_SharedHallData;
}

cocos2d::Layer* HallDataMgr::AddpopLayer(const std::string &title, const std::string &content, int type,bool close /* = false*/)
{
    if (type == EM_MODE_TYPE_REMOVE)
    {
        if (m_popLayer != NULL)
        {
            if (m_popLayer->getReferenceCount() >= 2)
            {
                m_popLayer->removeFromParent();
                m_popLayer->release();
                m_popLayer = NULL;
                return m_popLayer;
            }
            if (m_popLayer->getReferenceCount() == 1)
            {
                m_popLayer->release();
                m_popLayer = NULL;
                return m_popLayer;
            }
        }
        return NULL;
    }
    else
    {
        if (m_popLayer != NULL)
        {
            if (m_popLayer->getReferenceCount() >= 2)
            {
                m_popLayer->removeFromParent();
                m_popLayer->release();
                m_popLayer = NULL;
            }
            else if (m_popLayer->getReferenceCount() == 1)
            {
                m_popLayer->release();
                m_popLayer = NULL;
            }
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
        m_popLayer = ModeLayer::createBytype(title, content, (EM_MODE_TYPE)type,close);
        Director::getInstance()->getRunningScene()->addChild(m_popLayer,15);
        m_popLayer->retain();
        return m_popLayer;
    }
    return NULL;
}

void HallDataMgr::saveConfig()
{
    UserDefault* userCongfig=UserDefault::getInstance();
    userCongfig->setIntegerForKey("load_run", 1);
    userCongfig->setBoolForKey("load_remember", this->m_bRemember);
    userCongfig->setBoolForKey("load_autoload", this->m_bAutomatic);
    userCongfig->setBoolForKey("load_cancel", this->m_bcancel);
    userCongfig->setBoolForKey("opt_shake", this->m_bOptionShake);
    userCongfig->setBoolForKey("opt_mute", this->m_bOptionMute);
    userCongfig->setIntegerForKey("load_type", this->m_loadtype);
    userCongfig->setIntegerForKey("user_faceid", this->m_wFaceID);
    userCongfig->setIntegerForKey("user_customid", this->m_wCustom);
    
    userCongfig->setFloatForKey("opt_music", this->m_fOptionMusic);
    userCongfig->setFloatForKey("opt_sound", this->m_fOptionSound);
    
    userCongfig->flush();
}

void HallDataMgr::readConfig()
{
    //用户配置
    UserDefault* userCongfig=UserDefault::getInstance();
    
    //读取配置
    int firstRun=userCongfig->getIntegerForKey("load_run");
    if(!firstRun)
    {
        //游戏数据不存在
        return;
    }
    m_bRemember = userCongfig->getBoolForKey("load_remember");
    m_bAutomatic = userCongfig->getBoolForKey("load_autoload");
    m_bcancel = userCongfig->getBoolForKey("load_cancel");
    m_bOptionShake = userCongfig->getBoolForKey("opt_shake");
    m_bOptionMute = userCongfig->getBoolForKey("opt_mute");
    m_loadtype = (EM_LOAD_TYPE)userCongfig->getIntegerForKey("load_type");
    m_wCustom = userCongfig->getIntegerForKey("user_customid");
    m_wFaceID = userCongfig->getIntegerForKey("user_faceid");
    m_fOptionMusic = userCongfig->getFloatForKey("opt_music");
    m_fOptionSound = userCongfig->getFloatForKey("opt_sound");
    if (m_bOptionMute == true)
    {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.f);
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);
    }
    else
    {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_fOptionMusic/100.f);
        SimpleAudioEngine::getInstance()->setEffectsVolume(m_fOptionSound/100.f);
    }
}

void HallDataMgr::readSelectServer()
{
    //用户配置
    UserDefault* userCongfig=UserDefault::getInstance();
    
    m_strGameIpAdress = userCongfig->getStringForKey("login_server_select", "empty_adress");
    if (0 == strcmp("empty_adress", m_strGameIpAdress.c_str()))
    {
        m_strGameIpAdress = "122.10.92.221"/*"172.16.0.21"*/;
        // m_strGameIpAdress = "192.168.3.17"/*"172.16.0.21"*/;
    }
}

void HallDataMgr::saveSelectServer(const std::string &strAdress)
{
    m_strGameIpAdress = strAdress;
    
    //用户配置
    UserDefault* userCongfig=UserDefault::getInstance();
    
    userCongfig->setStringForKey("login_server_select", m_strGameIpAdress);
    
    userCongfig->flush();
}

void HallDataMgr::readTablepass()
{
    //用户配置
    UserDefault* userCongfig=UserDefault::getInstance();
    
    //读取配置
    int firstRun=userCongfig->getIntegerForKey("load_run");
    if(!firstRun)
    {
        //游戏数据不存在
        return;
    }
    m_Tablepass = userCongfig->getStringForKey(__String::createWithFormat("User_%d_enterpass", m_dwUserID)->getCString());
}

void HallDataMgr::saveTablepass(const std::string &pass)
{
    UserDefault* userCongfig=UserDefault::getInstance();
    userCongfig->setIntegerForKey("load_run", 1);
    userCongfig->setStringForKey(__String::createWithFormat("User_%d_enterpass", m_dwUserID)->getCString(), pass);
    userCongfig->flush();
}

void HallDataMgr::roomlistclear()
{
    for(auto iter : m_roomList)
    {
        if (iter != NULL)
        {
            delete iter;
            iter = nullptr;
        }
    }
    m_roomList.clear();
    m_roomList.shrink_to_fit();
}

std::string HallDataMgr::getSignature(LONGLONG times)
{
    LONGLONG timevalue = times;
    log("验证时间%lld", timevalue);
    auto timestr = __String::createWithFormat("%lld", timevalue)->getCString();
    std::string pstr = __String::createWithFormat("%d", m_dwUserID)->getCString();
    pstr += m_dynamicpass;
    pstr += timestr;
    pstr += "JDSyncLoginKey";
    pstr = MD5Encrypt(pstr);
    
    std::string preturnstr = "&time=";
    preturnstr += timestr;
    preturnstr += "&signature=";
    preturnstr += pstr;
    return preturnstr;
}

UserData* HallDataMgr::getUserData(const DWORD &userId)
{
    auto ite = m_UserList.find(userId);
    if (ite != m_UserList.end())
    {
        return ite->second;
    }
    return nullptr;
}

void HallDataMgr::clearUserData()
{
    m_UserList.clear();
}

UserData* HallDataMgr::getUserDataByChair(const DWORD &chairId)
{
    auto ite = m_UserList.begin();
    for (; ite != m_UserList.end(); ++ite)
    {
        if (ite->second->m_date.wChairID == chairId)
        {
            return ite->second;
        }
    }
    return nullptr;
}

void HallDataMgr::toMainSystemScale()
{
    if (m_targetPlatform == cocos2d::ApplicationProtocol::Platform::OS_IPHONE)
    {
        // 设计尺寸width、height
        auto visibleSizeWidth = 960;
        auto visibleSizeHeight = 640;
        
        // 根据屏幕尺寸与设计尺寸的比，重新定义设计尺寸（参考kResolutionNum来适配）
        auto winSize = m_szMainSize;
        //CCSize winSize = CCSizeMake(1136, 640);//CCEGLView::sharedOpenGLView()->getFrameSize();
        if (winSize.width/winSize.height > visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeWidth = visibleSizeHeight * winSize.width/winSize.height;
        }
        else if (winSize.width/winSize.height < visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeHeight = visibleSizeWidth / (winSize.width/winSize.height);
        }
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(visibleSizeWidth, visibleSizeHeight, ResolutionPolicy::NO_BORDER);
    }
    else
    {
        // 设计尺寸width、height
        auto visibleSizeWidth = 1152;
        auto visibleSizeHeight = 648;
        
        // 根据屏幕尺寸与设计尺寸的比，重新定义设计尺寸（参考kResolutionNum来适配）
        auto winSize = m_szMainSize;
        //CCSize winSize = CCSizeMake(1136, 640);//CCEGLView::sharedOpenGLView()->getFrameSize();
        if (winSize.width/winSize.height > visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeWidth = visibleSizeHeight * winSize.width/winSize.height;
        }
        else if (winSize.width/winSize.height < visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeHeight = visibleSizeWidth / (winSize.width/winSize.height);
        }
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(visibleSizeWidth, visibleSizeHeight, ResolutionPolicy::NO_BORDER);
    }
}

void HallDataMgr::toSubSystemScale()
{
    if (m_targetPlatform == cocos2d::ApplicationProtocol::Platform::OS_IPAD)
    {
        auto visibleSizeWidth = 1024;
        auto visibleSizeHeight = 768;
        
        // 根据屏幕尺寸与设计尺寸的比，重新定义设计尺寸（参考kResolutionNum来适配）
        auto winSize = m_szSubSize;
        //CCSize winSize = CCSizeMake(1136, 640);//CCEGLView::sharedOpenGLView()->getFrameSize();
        if (winSize.width/winSize.height > visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeWidth = visibleSizeHeight * winSize.width/winSize.height;
        }
        else if (winSize.width/winSize.height < visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeHeight = visibleSizeWidth / (winSize.width/winSize.height);
        }
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(visibleSizeWidth, visibleSizeHeight, ResolutionPolicy::NO_BORDER);
    }
    else if (m_targetPlatform == cocos2d::ApplicationProtocol::Platform::OS_IPHONE)
    {
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1136, 640, ResolutionPolicy::EXACT_FIT);
    }
    else
    {
        // 设计尺寸width、height
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1136, 640, ResolutionPolicy::EXACT_FIT);
    }
}