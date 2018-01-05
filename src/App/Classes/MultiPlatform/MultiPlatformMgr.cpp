/************************************************************************************
 * file: 		MultiPlatformMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/05 23:10:42
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/05			
 *
 ************************************************************************************/

#include "MultiPlatformMgr.h"
#include "HallDataMgr.h"

MultiPlatformMgr* MultiPlatformMgr::s_multiInstance = nullptr;

MultiPlatformMgr::MultiPlatformMgr()
{
    m_strMachineID = "";
    m_strVersion = "";
}

MultiPlatformMgr::~MultiPlatformMgr()
{
    
}

void MultiPlatformMgr::destroy()
{
    CC_SAFE_DELETE(s_multiInstance);
}

void MultiPlatformMgr::init()
{
    m_bOpenVibrator = HallDataMgr::getInstance()->m_bOptionShake;
    m_pDelegate = nullptr;
}

void MultiPlatformMgr::test()
{
    cocos2d::log("base test");
}

void MultiPlatformMgr::triggerPickImg()
{
    cocos2d::log("base trigger");
}

void MultiPlatformMgr::switchVibratorState(bool bOpen)
{
    cocos2d::log("base switch");
}

void MultiPlatformMgr::vibrator()
{
    cocos2d::log("base vibrator");
}

std::string MultiPlatformMgr::clientVersionInfo()
{
    return m_strVersion;
}

void MultiPlatformMgr::updateVersion()
{
    cocos2d::log("base updateVersion");
}

std::string MultiPlatformMgr::getMachineId()
{
    return m_strMachineID;
}

void MultiPlatformMgr::updateMachineID()
{
    cocos2d::log("base updateMachineID");
}