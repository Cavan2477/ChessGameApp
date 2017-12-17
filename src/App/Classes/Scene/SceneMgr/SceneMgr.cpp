/************************************************************************************
 * file: 		SceneMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/17 21:22:49
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/17			
 *
 ************************************************************************************/

#include "SceneMgr.h"
#include "../SceneHeader.h"
#include "../../Common/PublicDefine.h"

USING_NS_CC;

using namespace std;

static const char* PRELOADER_SCHEDULE_KEY = "scenemgr_preloader";

SceneMgr* SceneMgr::s_pSceneMgrInstance = nullptr;

void SceneMgr::init()
{
    m_nCurrentCount = -1;
    m_nTotalCount = 0;
}

void SceneMgr::clear()
{
    
}

void SceneMgr::transitionScene(const std::string &scenename, bool bAnimation /*= true*/)
{
    m_bAnimation = bAnimation;
    m_bPreloader = false;
    log("tansition scene %s",scenename.c_str());
    
    std::vector<tagUIResourceParam> param = checkScene(scenename);
    m_strNextSceneName = scenename;
    if (!param.empty())
    {
        if (m_nCurrentCount == m_nTotalCount)
        {
            onLoadFinishi();
        }
        else
        {
            m_pCurrentScene = dynamic_cast<UIScene*>(INSTANCE(Director)->getRunningScene());
            
            this->preLoadSceneResource(param);
            
            if (!m_bAnimation)
			{
                //INSTANCE(LoadingMgr)->normalLoading();
                HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_WAIT);
            }
        }
    }
}

void SceneMgr::preloaderScene(const std::string &scenename, const std::function<void ()> &callback, bool bAnimation /*= true*/)
{
    if (m_bPreloader) return;
    log("preloader scene %s",scenename.c_str());
    
    m_bAnimation = bAnimation;
    m_pCallBack = callback;
    m_bPreloader = true;
    
    std::vector<tagUIResourceParam> param = checkScene(scenename);
    m_strNextSceneName = scenename;
    if (!param.empty())
    {
        m_pCurrentScene = dynamic_cast<UIScene*>(INSTANCE(Director)->getRunningScene());
        
        this->preLoadSceneResource(param);
        
        if (!m_bAnimation)
        {
            //INSTANCE(LoadingMgr)->normalLoading();
            HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_WAIT);
        }
    }
}

UIScene* SceneMgr::seekScene(const std::string &scene_name)
{
    //百家乐
    SCENE_CREATE_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(PlazaScene));
    SCENE_CREATE_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(GameScene));
    //SCENE_CREATE_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(TaskScene));
    SCENE_CREATE_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(BankScene));
   // SCENE_CREATE_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(CheckInScene));
    
    //通比牛牛
    SCENE_CREATE_RETURN(scene_name.c_str(), TBNN_NAMESPACE_FUN(PlazaScene));
    SCENE_CREATE_RETURN(scene_name.c_str(), TBNN_NAMESPACE_FUN(GameScene));
    return nullptr;
}

std::vector<tagUIResourceParam> SceneMgr::checkScene(const std::string &scene_name)
{
    //百家乐
    SCENE_CHECK_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(PlazaScene));
    SCENE_CHECK_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(GameScene));
    //SCENE_CHECK_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(TaskScene));
    SCENE_CHECK_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(BankScene));
    //SCENE_CHECK_RETURN(scene_name.c_str(), BJL_NAMESPACE_FUN(CheckInScene));
    
    //通比牛牛
    SCENE_CHECK_RETURN(scene_name.c_str(), TBNN_NAMESPACE_FUN(PlazaScene));
    SCENE_CHECK_RETURN(scene_name.c_str(), TBNN_NAMESPACE_FUN(GameScene));
    return std::vector<tagUIResourceParam>();
}

void SceneMgr::preLoadSceneResource(const std::vector<tagUIResourceParam> &vecRes)
{
    if (m_nCurrentCount == m_nTotalCount)
    {
        return;
    }
    
    m_bHavaLoadRes = false;
    m_nHeartTime = 0.0f;
    m_nCurrentCount = 0;
    m_fTmpProgress = 0.0f;
    m_nTotalCount = vecRes.size();
    if (INSTANCE(Director)->getScheduler()->isScheduled(PRELOADER_SCHEDULE_KEY, this))
    {
        INSTANCE(Director)->getScheduler()->unschedule(PRELOADER_SCHEDULE_KEY, this);
    }
    INSTANCE(Director)->getScheduler()->schedule(CC_CALLBACK_1(SceneMgr::preLoader,this),this,0.0f,CC_REPEAT_FOREVER,0,false,PRELOADER_SCHEDULE_KEY);
    
    if (vecRes.empty())
    {
        return;
    }
    for (size_t i = 0; i < vecRes.size(); ++i)
    {
        tagUIResourceParam param = vecRes[i];
        switch (param.resType)
        {
            case tagUIResourceParam::UIResourceType::kTEXTURE_RES:
            {
                this->preLoadTextureResource(param);
            }
                break;
            case tagUIResourceParam::UIResourceType::kAUDIO_BACKGROUND:
            {
                this->preLoadBackgroudAudioResource(param);
            }
                break;
            case tagUIResourceParam::UIResourceType::kAUDIO_EFFECT:
            {
                this->preLoadEffectAudioResource(param);
            }
                break;
            default:
                break;
        }
    }
}

void SceneMgr::doUISceneResourceRelease(const std::vector<tagUIResourceParam> &vecRes)
{
    std::vector<tagUIResourceParam> tmp = vecRes;
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        tagUIResourceParam param = tmp[i];
        switch (param.resType)
        {
            case tagUIResourceParam::UIResourceType::kTEXTURE_RES:
            {
                this->releaseTextrueResource(param);
            }
                break;
            case tagUIResourceParam::UIResourceType::kAUDIO_EFFECT:
            {
                this->releaseEffectAudioResource(param);
            }
                break;
            default:
                break;
        }
    }
}

cocos2d::Scene *SceneMgr::getTempSceneByKind(const int &kind)
{
    Scene *s = nullptr;
    
    switch (kind)
    {
        case EM_GAME_BAIJIALE:
            s = BJL_NAMESPACE_FUN(TempGameScene)::create();
            break;
        case EM_GAME_NIUNIU:
            s = NN_NAMESPACE_FUN(TempGameScene)::create();
            break;
        case EM_GAME_TBNN:
            s = TBNN_NAMESPACE_FUN(TempGameScene)::create();
            break;
    }
    
    return s;
}

void SceneMgr::releaseEffectAudioResource(const tagUIResourceParam &param)
{
    INSTANCE(AudioMgr)->unloadEffectAudio(param.resPath);
}

void SceneMgr::releaseTextrueResource(const tagUIResourceParam &param)
{
    if (ui::Widget::TextureResType::PLIST == param.texType)
    {
        INSTANCE(SpriteFrameCache)->removeSpriteFramesFromFile(param.plistFile);
    }
    INSTANCE(Director)->getTextureCache()->removeTextureForKey(param.resPath);
}

void SceneMgr::preLoader(float dt)
{
    if ((m_fTmpProgress) < (m_nTotalCount - m_nCurrentCount))
    {
        m_fTmpProgress += 0.005;
    }
    
    if (m_bAnimation)
    {
        if (0 != m_nTotalCount)
        {
            float cur = (float)m_nCurrentCount;
            float per = cur / m_nTotalCount;
            if (per > 0 && per > m_fTmpProgress)
            {
                //INSTANCE(LoadingMgr)->progressLoading(per * 100,nullptr,nullptr,false);
            }
            else
            {
                //INSTANCE(LoadingMgr)->progressLoading(m_fTmpProgress * 100,nullptr,nullptr,false);
            }
            cocos2d::log("progress %.2f", per);
        }
    }
    
    m_nHeartTime += dt;
	if (m_nHeartTime > HEART_TIME)
    {
        //INSTANCE(LoadingMgr)->hideProgressLoading();
        HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
    }
    
    if (m_nCurrentCount >= m_nTotalCount)
    {
        onLoadFinishi();
    }
}

//预加载背景音乐资源
void SceneMgr::preLoadBackgroudAudioResource(const tagUIResourceParam &param)
{
    INSTANCE(AudioMgr)->preloadBackgroundAudio(param.resPath);
    ++m_nCurrentCount;
    log("on load backgroud res:%d",(int)m_nCurrentCount);
}

//预加载音效资源
void SceneMgr::preLoadEffectAudioResource(const tagUIResourceParam &param)
{
    INSTANCE(AudioMgr)->preloadEffectAudio(param.resPath);
    ++m_nCurrentCount;
    log("on load effect res:%d",(int)m_nCurrentCount);
}

//预加载纹理资源
void SceneMgr::preLoadTextureResource(const tagUIResourceParam &param)
{
    switch (param.texType) {
        case ui::Widget::TextureResType::LOCAL:
        {
            this->preLoadLocalTexture(param);
        }
            break;
        case ui::Widget::TextureResType::PLIST:
        {
            this->preLoadPlistTexture(param);
        }
            break;
        default:
            break;
    }
}

//预加载合图
void SceneMgr::preLoadPlistTexture(const tagUIResourceParam &param)
{
    Director::getInstance()->getTextureCache()->addImageAsync(param.resPath, CC_CALLBACK_1(SceneMgr::onImageLoaded, this));
}

//预加载普通图片
void SceneMgr::preLoadLocalTexture(const tagUIResourceParam &param)
{
    Director::getInstance()->getTextureCache()->addImageAsync(param.resPath, CC_CALLBACK_1(SceneMgr::onImageLoaded, this));
}

void SceneMgr::onImageLoaded(cocos2d::Texture2D *texture)
{
    ++m_nCurrentCount;
    log("on load textrue res:%d",(int)m_nCurrentCount);
}

void SceneMgr::onUISceneResourceLoaded(const std::vector<tagUIResourceParam> &vecRes)
{
    for (size_t i = 0; i < vecRes.size(); ++i)
    {
        tagUIResourceParam param = vecRes[i];
        if (ui::Widget::TextureResType::PLIST == param.texType)
        {
            INSTANCE(SpriteFrameCache)->addSpriteFramesWithFile(param.plistFile);
        }
    }
}

void SceneMgr::onLoadFinishi()
{
    if (!m_bHavaLoadRes)
    {
        m_bHavaLoadRes = true;
        onUISceneResourceLoaded(checkScene(m_strNextSceneName));
    }
    
    if (m_bPreloader)
    {
        if (m_pCallBack)
        {
            m_pCallBack();
            m_pCallBack = nullptr;
        }
        return;
    }
    
    INSTANCE(Director)->getScheduler()->unschedule(PRELOADER_SCHEDULE_KEY, this);
    m_nCurrentCount = -1;
    m_nTotalCount = 0;
    m_nHeartTime = 0.0f;
    m_fTmpProgress = 0.0f;
    
    //INSTANCE(LoadingMgr)->hideProgressLoading();
    //INSTANCE(LoadingMgr)->hideNormalLoading();
    HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
    
    m_pNextScene = seekScene(m_strNextSceneName);
    //场景切换
    if (nullptr != m_pNextScene)
    {
        auto scene = TransitionProgressInOut::create(TIME_SCENE_CHANGE, m_pNextScene);
        INSTANCE(Director)->replaceScene(scene);
    }
}