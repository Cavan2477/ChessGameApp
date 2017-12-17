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
#define SOCKET_TCP_PACKET	(SOCKET_TCP_BUFFER-sizeof(ST_TCP_HEAD))	//网络缓冲

#pragma mark 内核命令

#define MDM_KN_COMMAND			0				//内核命令
#define SUB_KN_DETECT_SOCKET	1				//检测命令
#define SUB_KN_VALIDATE_SOCKET	2				//验证命令

///////////////////////////////////////////////////////////////////////////
//结构定义
//网络验证
typedef struct _stTcpValidate
{
	TCHAR		szValidateKey[64];				//验证字符
}ST_TCP_VALICATE, *PST_TCP_VALICATE;

//网络内核
typedef struct _stTcpInfo
{
	BYTE		cbDataKind;						//数据类型
	BYTE		cbCheckCode;					//效验字段
	WORD		wPacketSize;					//数据大小
}ST_TCP_INFO, *PST_TCP_INFO;

//网络命令
typedef struct _stTcpCmd
{
	WORD		wMainCmdID;						//主命令码
	WORD		wSubCmdID;						//子命令码
}ST_TCP_CMD, *PST_TCP_CMD;

//网络包头
typedef struct _stTcpHead
{
	_stTcpInfo	stTCPInfo;						//基础结构
	_stTcpCmd	stTCPCmd;						//命令信息
}ST_TCP_HEAD, *PST_TCP_HEAD;

//网络缓冲
typedef struct _stTcpBuffer
{
	_stTcpHead	stTcpHead;						//数据包头
	BYTE		cbBuffer[SOCKET_TCP_PACKET];	//数据缓冲
}ST_TCP_BUFFER, *PST_TCP_BUFFER;

//数据定义
#pragma pack()
 
#endif
