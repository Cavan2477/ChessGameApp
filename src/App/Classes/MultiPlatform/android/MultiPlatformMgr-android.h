/************************************************************************************
 * file: 		MultiPlatformMgr-android.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/28 21:33:05
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/28			
 *
 ************************************************************************************/

#ifndef __MULTI_PLATFORM_MGR_ANDROID_H__
#define __MULTI_PLATFORM_MGR_ANDROID_H__

#include <stdio.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "../../MultiPlatform/MultiPlatformMgr.h"

class MultiPlatformMgr_android:public MultiPlatformMgr
{
    friend class MultiPlatformMgr;

public:
    MultiPlatformMgr_android();
    ~MultiPlatformMgr_android();
    
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

#endif

#endif /* MultiPlatformMgr_android_hpp */
