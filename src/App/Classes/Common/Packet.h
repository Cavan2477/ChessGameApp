/************************************************************************************
 * file: 		Packet.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:17:22
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __PACKET_H__
#define __PACKET_H__

#include "MacroDefine.h"

#pragma pack(1)

/////////////////////////////////////////////////////////////////////
//端口定义
#define MAX_CONTENT			512							//并发容量
#define PORT_AUTO_SELECT	((WORD)(0XFFFF))			//自动端口

#define PORT_TEMP           6300
#define PORT_LOGON			8300						//登陆端口
#define PORT_CENTER			8310						//协调端口
#define PORT_8311			8311						//测试端口
#define PORT_MANAGER		8320						//管理端口

/////////////////////////////////////////////////////////////////////
//网络定义

//数据类型
#define DK_MAPPED			0X01				//映射类型
#define DK_ENCRYPT			0X02				//加密类型
#define DK_COMPRESS			0X04				//压缩类型

//长度定义
#define SOCKET_TCP_BUFFER	16384									//网络缓冲
#define SOCKET_TCP_PACKET	(SOCKET_TCP_BUFFER-sizeof(TCP_Head))	//网络缓冲


#pragma mark 内核命令
#define MDM_KN_COMMAND			0				//内核命令
#define SUB_KN_DETECT_SOCKET	1				//检测命令
#define SUB_KN_VALIDATE_SOCKET	2				//验证命令
///////////////////////////////////////////////////////////////////////////
//结构定义
//网络验证
typedef struct
{
	TCHAR		szValidateKey[64];				//验证字符
}TCP_Validate;

//网络内核
struct TCP_Info
{
	BYTE		cbDataKind;						//数据类型
	BYTE		cbCheckCode;					//效验字段
	WORD		wPacketSize;					//数据大小
};

//网络命令
typedef struct
{
	WORD		wMainCmdID;						//主命令码
	WORD		wSubCmdID;						//子命令码
}TCP_Command;

//网络包头
typedef struct
{
	TCP_Info	TCPInfo;						//基础结构
	TCP_Command	CommandInfo;					//命令信息
}TCP_Head;

//网络缓冲
typedef struct
{
	TCP_Head	Head;							//数据包头
	BYTE		cbBuffer[SOCKET_TCP_PACKET];	//数据缓冲
}TCP_Buffer;

//数据定义
#pragma pack()
 
#endif
