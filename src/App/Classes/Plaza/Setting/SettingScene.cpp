//
//  Setting.cpp
//  GameProject
//
//  Created by Tang Miao on 2/29/16.
//
//

#include "SettingScene.h"
#include "cocostudio/CocoStudio.h"

#include "../../DataMgr/HallDataMgr.h"
#include "SimpleAudioEngine.h"

#include "Login.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "UMSocial.h"
#import "AppController.h"
#import "RootViewController.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

using namespace cocos2d::ui;

SettingScene::SettingScene()
{
    
}

SettingScene::~SettingScene()
{

}

bool SettingScene::init()
{
    _fOptionMusic = HallDataMgr::getInstance()->m_fOptionMusic;
    _fOptionSound = HallDataMgr::getInstance()->m_fOptionSound;
    
    if (!Layer::init())
    {
        return false;
    }
    
    HallDataMgr::getInstance()->readConfig();
    
    auto layout  = Layout::create();
    layout->setTag(1);
    layout->setContentSize(cocos2d::Size(1136, 960));
    
    auto rootNode = CSLoader::createNode("setting_res/SettingScene.csb");
    rootNode->setTag(1);
    layout->addChild(rootNode);
    
    //滑杆
    auto sliderSound = static_cast<Slider *>(rootNode->getChildByName("Slider_sound"));
    sliderSound->addEventListener(CC_CALLBACK_2(SettingScene::sliderCallBack, this));
    sliderSound->setTag(EM_SLIDER_SOUND);
    sliderSound->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
    
    auto sliderMusic = static_cast<Slider *>(rootNode->getChildByName("Slider_music"));
    sliderMusic->addEventListener(CC_CALLBACK_2(SettingScene::sliderCallBack, this));
    sliderMusic->setTag(EM_SLIDER_MUSIC);
    sliderMusic->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);

    //静音切换
    auto quiet = static_cast<Button *>(rootNode->getChildByName("Button_quite"));
    if (nullptr != quiet)
    {
        quiet->setTag(EM_QUIET);
        if (HallDataMgr::getInstance()->m_bOptionMute)
        {
            
            quiet->loadTextures("setting_res/set_sliper_open.png", "setting_res/set_sliper_close.png");
        }else
        {
             quiet->loadTextures("setting_res/set_sliper_close.png", "setting_res/set_sliper_open.png");
            
        }
        
        quiet->addTouchEventListener([=](cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type){
            
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                HallDataMgr::getInstance()->m_bOptionMute = !HallDataMgr::getInstance()->m_bOptionMute;
                
                if (HallDataMgr::getInstance()->m_bOptionMute)
                {
                    quiet->loadTextures("setting_res/set_sliper_open.png", "setting_res/set_sliper_close.png");
                    sliderSound->setPercent(0);
                    sliderMusic->setPercent(0);
                    
                    HallDataMgr::getInstance()->m_fOptionSound = 0;
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
             
                    HallDataMgr::getInstance()->m_fOptionMusic = 0;
                    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
                    
                }else
                {
                    quiet->loadTextures("setting_res/set_sliper_close.png", "setting_res/set_sliper_open.png");
                    
                    HallDataMgr::getInstance()->m_fOptionSound = _fOptionSound;
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(_fOptionSound);
                    
                    HallDataMgr::getInstance()->m_fOptionMusic = _fOptionMusic;
                    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(_fOptionMusic);
                    
                    sliderSound->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
                    sliderMusic->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
                }
            }
        });
    }
    
    //分享
    auto share = static_cast<Button *>(rootNode->getChildByName("Button_share"));
    if (nullptr != share)
    {
        share->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED)
            {
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)   
                AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
                [UMSocialSnsService presentSnsIconSheetView:pApp.viewController
                                                     appKey:UMKey
                                                  shareText:@"游戏测试"
                                                 shareImage:[UIImage imageNamed:@"Icon-72.png"]
                                            shareToSnsNames:[NSArray arrayWithObjects:UMShareToSina,UMShareToWechatTimeline,UMShareToWechatSession,nil]
                                                   delegate:pApp];
#endif
                
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                JniMethodInfo minfo;
                jobject jobj;
                bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity","share","()V");
                if (isHave)
                {
                    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
                    minfo.env->DeleteLocalRef(minfo.classID);
                }
#endif
                
                
//                
//                
//                auto pbg = Button::create();
//                pbg->setScale9Enabled(true);
//                pbg->setContentSize(cocos2d::Size(1136, 640));
//                pbg->setPosition(Vec2(568, 320));
//                pbg->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
//                    
//                    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
//                    {
//                        pbg->removeFromParent();
//                    }
//                });
//                layout->addChild(pbg);
//                
//                
//                auto shareRoot = CSLoader::createNode("share_res/share.csb");
//                pbg->addChild(shareRoot);
//                
//                //新浪
//                auto sina = static_cast<Button *>(shareRoot->getChildByName("Button_sina"));
//                if (nullptr != sina)
//                {
//                    
//                    sina->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
//                        
//                        if (type == Widget::TouchEventType::ENDED)
//                        {
//                            
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//                      
//                            AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
//                            [[UMSocialControllerService defaultControllerService] setShareText:@"分享内嵌文字" shareImage:[UIImage imageNamed:@"Icon-72.png"] socialUIDelegate:pApp];
//                            [UMSocialSnsPlatformManager getSocialPlatformWithName:UMShareToSina].snsClickHandler(pApp.viewController,[UMSocialControllerService defaultControllerService],YES);
//#endif
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//                            JniMethodInfo minfo;
//                            jobject jobj;
//                            bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity","sharetoSina","()V");
//                            if (isHave)
//                            {
//                                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
//                                minfo.env->DeleteLocalRef(minfo.classID);
//                            }
//#endif
//                        }
//                        
//                    });
//                }
//                
//                //微信
//                auto weixin = static_cast<Button *>(shareRoot->getChildByName("Button_weixin"));
//                if (nullptr != weixin)
//                {
//                    
//                    weixin->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
//                        
//                        if (type == Widget::TouchEventType::ENDED)
//                        {
//                            
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//                            
//                            AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
//                            [[UMSocialControllerService defaultControllerService] setShareText:@"分享内嵌文字" shareImage:[UIImage imageNamed:@"Icon-72.png"] socialUIDelegate:pApp];
//                            [UMSocialSnsPlatformManager getSocialPlatformWithName:UMShareToWechatTimeline].snsClickHandler(pApp.viewController,[UMSocialControllerService defaultControllerService],YES);
//#endif
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//                            JniMethodInfo minfo;
//                            jobject jobj;
//                            bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity","sharetoWinxin","()V");
//                            if (isHave)
//                            {
//                                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
//                                minfo.env->DeleteLocalRef(minfo.classID);
//                            }
//#endif
//                            
//                        }
//                        
//                    });
//                    
//                }
            }
        });
        
    }
    
    //切换按钮
    auto changeAccount = static_cast<Button *>(rootNode->getChildByName("Button_changeAccount"));
    if (nullptr != changeAccount)
    {
        
        changeAccount->addTouchEventListener([](cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type){
        
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                HallDataMgr::getInstance()->m_bcancel = true;
                HallDataMgr::getInstance()->saveConfig();
                TransFadeInLogin;
            }
            
        });
        
    }
    
    //关闭按钮
    auto close = static_cast<Button *>(rootNode->getChildByName("Button_close"));
    if (nullptr != close)
    {
        
        close->addTouchEventListener(CC_CALLBACK_2(SettingScene::buttonEventWithClose, this));
    }
    
    layout->setScaleX(JudgeScale);
    this->addChild(layout);

    return true;
}

void SettingScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void SettingScene::onExit()
{
    HallDataMgr::getInstance()->saveConfig();
    Layer::onExit();
}

void SettingScene::sliderCallBack(cocos2d::Ref *ref, cocos2d::ui::Slider::EventType type)
{
    
    auto slider = (Slider *)ref;
    int percent = slider->getPercent();
    float _percent = percent / (float)100;
    
    
    if (slider->getTag() == EM_SLIDER_SOUND)
    {
        
        HallDataMgr::getInstance()->m_fOptionSound = percent;
        _fOptionSound = percent;
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(_percent);
    }else
    {
        HallDataMgr::getInstance()->m_fOptionMusic = percent;
        _fOptionMusic = percent;
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(_percent);
        
    }
    
    if (percent > 0)
    {
        HallDataMgr::getInstance()->m_bOptionMute = false;
        auto layout = static_cast<Layout *>(this->getChildByTag(1));
        auto rootNode = static_cast<Node *>(layout->getChildByTag(1));
        auto quiet = static_cast<Button *>(rootNode->getChildByTag(EM_QUIET));
        quiet->loadTextures("setting_res/set_sliper_close.png", "setting_res/set_sliper_open.png");
    }
}


void SettingScene::buttonEventWithClose(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        this->removeFromParent();
        
    }
}
    
    
