//
//  SceneMgr.hpp
//  DouDiZhu
//
//  Created by zhong on 1/11/16.
//
//

#ifndef SceneMgr_hpp
#define SceneMgr_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "../../LibExt/UIInterface.h"

#define SCENE_CREATE_RETURN(SCENE_NAME, CLASS) \
    if (0 == strcmp(SCENE_NAME, CLASS::getSceneName().c_str())) \
        return CLASS::create(); \

#define SCENE_CHECK_RETURN(SCENE_NAME,CLASS) \
    if (0 == strcmp(SCENE_NAME, CLASS::getSceneName().c_str())) \
        return CLASS::uiAsynLoadResource(); \

class SceneMgr
{
    CREATE_SINGLETON_MUTEX_CLEAR(SceneMgr, s_sceneInstance, nullptr);

    void init();
    void clear();
    
public:
    void transitionScene(const std::string &scenename,bool bAnimation = true);
    
    void preloaderScene(const std::string &scenename, const std::function<void()> &callback, bool bAnimation = true);
    
    UIScene* seekScene(const std::string &scene_name);
    
    std::vector<tagUIResourceParam> checkScene(const std::string &scene_name);
    
    //资源释放
    void doUISceneResourceRelease(const std::vector<tagUIResourceParam> &vecRes);
    
    cocos2d::Scene *getTempSceneByKind(const int &kind);
private:
    //释放音效资源
    void releaseEffectAudioResource(const tagUIResourceParam &param);
    //释放纹理资源
    void releaseTextrueResource(const tagUIResourceParam &param);
    
    //加载定时器
    void preLoader(float dt);
    //预加载场景资源
    void preLoadSceneResource(const std::vector<tagUIResourceParam> &vecRes);
    //预加载背景音乐资源
    void preLoadBackgroudAudioResource(const tagUIResourceParam &param);
    //预加载音效资源
    void preLoadEffectAudioResource(const tagUIResourceParam &param);
    //预加载纹理资源
    void preLoadTextureResource(const tagUIResourceParam &param);
    //预加载合图
    void preLoadPlistTexture(const tagUIResourceParam &param);
    //预加载普通图片
    void preLoadLocalTexture(const tagUIResourceParam &param);
    //异步加载图片结束
    void onImageLoaded(cocos2d::Texture2D *texture);
    
    //资源加载完毕
    void onUISceneResourceLoaded(const std::vector<tagUIResourceParam> &vecRes);
    
    void onLoadFinishi();
private:
    static SceneMgr* s_sceneInstance;
    
    //总加载数量
    size_t m_nTotalCount;
    //当前加载数量
    size_t m_nCurrentCount;
    
    //当前场景名
    std::string m_strNextSceneName;
    //当前场景
    UIScene* m_pCurrentScene;
    //待切换场景
    UIScene* m_pNextScene;
    
    //是否显示切换加载动画
    bool m_bAnimation;
    
    //加载回调
    std::function<void()> m_pCallBack;
    
    bool m_bPreloader;
    
    float m_nHeartTime;
    
    //是否已加载资源
    bool m_bHavaLoadRes;
    
    //
    float m_fTmpProgress;
};
#endif /* SceneMgr_hpp */
