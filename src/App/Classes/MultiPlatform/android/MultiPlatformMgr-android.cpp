//
//  MultiPlatformMgr-android.cpp
//  NiuNiu
//
//  Created by zhong on 1/4/16.
//
//

#if  CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MultiPlatformMgr-android.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "HallDataMgr.h"
#include "ModeLayer.h"
#define JAVA_CLASS "com.foxuc.utils.WHHelper"
USING_NS_CC;

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT void JNICALL Java_com_foxuc_utils_WHHelper_nativeDidPickImgFinish(JNIEnv * env, jobject obj, jstring jStr)
    {
        std::string path = JniHelper::jstring2string(jStr);
        
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([path](){
            cocos2d::Image * pcimage = new cocos2d::Image();
            if (pcimage->initWithImageFile(path))
            {
                cocos2d::log("path:%s",path.c_str());
                if (MultiPlatformMgr::getInstance()->getPlatformDelegate())
                {
                    MultiPlatformMgr::getInstance()->getPlatformDelegate()->didPickImgFinish(pcimage);
                }
            }
            pcimage->release();
        });
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_nativeQuitApp(JNIEnv * env, jobject obj)
    {
        auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("提示", "确定要退出有乐棋牌吗?", Type_Ensure_Cancel));
        player->setEnsureCallback([]
                                  {
                                      Director::getInstance()->end();
                                  });
    }
}
#endif
MultiPlatformMgr_android::MultiPlatformMgr_android()
{
    
}

MultiPlatformMgr_android::~MultiPlatformMgr_android()
{
    
}

MultiPlatformMgr* MultiPlatformMgr::getInstance()
{
    if (nullptr == s_multiInstance)
    {
        s_multiInstance = new MultiPlatformMgr_android();
    }
    return s_multiInstance;
}

void MultiPlatformMgr_android::test()
{
    
}

void MultiPlatformMgr_android::triggerPickImg()
{
    JniMethodInfo t;
    bool isHave = JniHelper::getStaticMethodInfo(t,
                                                 JAVA_CLASS,
                                                 "pickImg",
                                                 "()V");
    if (isHave)
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void MultiPlatformMgr_android::switchVibratorState(bool bOpen)
{
    m_bOpenVibrator = bOpen;
    JniMethodInfo t;
    bool isHave = JniHelper::getStaticMethodInfo(t,
                                                 JAVA_CLASS,
                                                 "switchVibratorState",
                                                 "(Z)V");
    if (isHave)
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,bOpen);
        t.env->DeleteLocalRef(t.classID);
    }
}

void MultiPlatformMgr_android::vibrator()
{
    if (!m_bOpenVibrator)
    {
        return;
    }
    JniMethodInfo t;
    bool isHave = JniHelper::getStaticMethodInfo(t,
                                                 JAVA_CLASS,
                                                 "switchVibratorState",
                                                 "(Z)V");
    if (isHave)
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,true);
        t.env->DeleteLocalRef(t.classID);
    }
}

void MultiPlatformMgr_android::updateVersion()
{
    JniMethodInfo t;
    bool isHave = JniHelper::getStaticMethodInfo(t,
                                                 JAVA_CLASS,
                                                 "clientVersion",
                                                 "()Ljava/lang/String;");
    if (isHave)
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        m_strVersion = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
    }
}

void MultiPlatformMgr_android::updateMachineID()
{
    JniMethodInfo t;
    bool isHave = JniHelper::getStaticMethodInfo(t,
                                                 JAVA_CLASS,
                                                 "getAndroidMachineID",
                                                 "()Ljava/lang/String;");
    if (isHave)
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        m_strMachineID = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
    }
}
#endif