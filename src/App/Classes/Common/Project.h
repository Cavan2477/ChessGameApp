/************************************************************************************
 * file: 		Project.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:12:59
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __project_h__
#define __project_h__

#include "../Public/Help.h"

#define ADDRESS_WH6603      HallDataMgr::getInstance()->m_strGameIpAdress.c_str()/*"122.10.92.221"*/
#define ADDRESS_WHhttp6603  "http://122.10.92.221"

#define ADDRESS_WHhttp      ADDRESS_WHhttp6603
#define LOGON_ADDRESS_YM    ADDRESS_WH6603				//登陆域名
#define LOGON_PORT          8400						//登陆端口
#define LOGON_KIND_ID       508							//游戏id

//#define LOGON_KIND_ID       27						//NIUNIU
#define PAGE_TABLE_COUNT    1							//每页桌子个数

//通用验证 演示平台验证
static const char* szCompilatio = "6F6AEE2F-53CD-4EF2-B876-35D57D81C7AC";

//AES偏移向量

const BYTE WHAESKEY[] = "WHPhone6603                     ";  //32位key

#define HttpAdress  "http://jd.webfox.com/"

#endif


