//
//  MultiPlatformMgr-ios.hpp
//  NiuNiu
//
//  Created by zhong on 1/4/16.
//
//

#ifndef MultiPlatformMgr_ios_hpp
#define MultiPlatformMgr_ios_hpp

#include <stdio.h>
#include "MultiPlatformMgr.h"
class MultiPlatformMgr_ios:public MultiPlatformMgr
{
public:
    MultiPlatformMgr_ios();
    ~MultiPlatformMgr_ios();
    
    virtual void test() override;
    
    //选择图片
    virtual void triggerPickImg() override;
    
    //震动设置 nOpen:1开；0:关
    virtual void switchVibratorState(bool bOpen) override;
    
    //设备震动
    virtual void vibrator() override;
    
    //程序版本号
    virtual void updateVersion() override;
    
    //设备id
    virtual void updateMachineID() override;
};
#endif /* MultiPlatformMgr_ios_hpp */
