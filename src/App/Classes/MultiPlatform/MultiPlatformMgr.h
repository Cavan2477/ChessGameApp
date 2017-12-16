//
//  MultiPlatformMgr.hpp
//  NiuNiu
//
//  Created by zhong on 1/4/16.
//
//

#ifndef MultiPlatformMgr_hpp
#define MultiPlatformMgr_hpp

#include <stdio.h>
#include "cocos2d.h"

class MultiPlatformMgrDelegate
{
public:
    virtual void didPickImgFinish(cocos2d::Image *pickimg){};
};
class MultiPlatformMgr
{
protected:
    MultiPlatformMgr();
public:
    virtual ~MultiPlatformMgr();
    
    static MultiPlatformMgr* getInstance();
    static void destroy();
    virtual void init();
    CC_SYNTHESIZE(MultiPlatformMgrDelegate*, m_pDelegate, PlatformDelegate);
public:
    virtual void test();
    
    //选择图片
    virtual void triggerPickImg();
    
    //震动设置 nOpen:1开；0:关
    virtual void switchVibratorState(bool bOpen);
    
    //设备震动
    virtual void vibrator();
    
    //程序版本号
    virtual std::string clientVersionInfo();
    
    //更新版本信息
    virtual void updateVersion();
    
    virtual std::string getMachineId();
    
    //更新设备id
    virtual void updateMachineID();
protected:
    static MultiPlatformMgr* s_multiInstance;
    //是否开启震动
    bool m_bOpenVibrator;
    //版本号
    std::string m_strVersion;
    //设备id(未进行md5加密)
    std::string m_strMachineID;
};
#endif /* MultiPlatformMgr_hpp */
