/************************************************************************************
 * file: 		CMD_NotifyDefine.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:34:39
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef	__CMD_NOTIFY_DEFINE_H__
#define	__CMD_NOTIFY_DEFINE_H__

#pragma pack(1)

#include "cocos2d.h"

typedef int RESULT_TYPE;            //结果处理

#define RESULT_DEFAULT      0       //默认结果

#define RESULT_SUCCESS      1       //处理成功
#define RESULT_FAILURE      2       //处理失败

#define RESULT_BEGAN        3       //处理开始
#define RESULT_INGED        4       //处理中
#define RESULT_FINISH       5       //处理完成

#define RESULT_OTHER        10      //其他处理

//用户金币改变通知
const std::string whEvent_User_Data_Change		= "whEvent_User_Data_Change";
const std::string whEvent_Desk_Status			= "whEvent_Desk_Status";				//桌子状态变化，是否加锁
const std::string whEvent_Room_StartGame		= "whNd_Room_StartGame";				//启动游戏
const std::string whNd_Room_LeaveGame			= "whNd_Room_LeaveGame";				//离开游戏
const std::string whNd_Horn_Message				= "whNd_Horn_Message";					//喇叭消息
const std::string whNd_Sys_Chat					= "whNd_Sys_Chat";						//系统消息
const std::string whNd_Socket_Connect_Failure	= "whNd_Socket_Connect_Failure";		//socket连接失败
const std::string whNd_EnterForeground			= "whNd_EnterForeground";				//后台切换回来
const std::string whNd_Socket_Disconnect		= "whNd_Socket_Disconnect";				//网络断开
const std::string whNd_Android_PhotoImage		= "whNd_Android_PhotoImage";			//android相册图片选择完成
const std::string whNd_Download_Update			= "whNd_Download_Update";				//资源下载刷新

#pragma pack()

#endif
















 
 
