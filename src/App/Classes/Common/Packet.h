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
//�˿ڶ���
#define MAX_CONTENT			512							//��������
#define PORT_AUTO_SELECT	((WORD)(0XFFFF))			//�Զ��˿�

#define PORT_TEMP           6300
#define PORT_LOGON			8300						//��½�˿�
#define PORT_CENTER			8310						//Э���˿�
#define PORT_8311			8311						//���Զ˿�
#define PORT_MANAGER		8320						//����˿�

/////////////////////////////////////////////////////////////////////
//���綨��

//��������
#define DK_MAPPED			0X01				//ӳ������
#define DK_ENCRYPT			0X02				//��������
#define DK_COMPRESS			0X04				//ѹ������

//���ȶ���
#define SOCKET_TCP_BUFFER	16384									//���绺��
#define SOCKET_TCP_PACKET	(SOCKET_TCP_BUFFER-sizeof(ST_TCP_HEAD))	//���绺��

#pragma mark �ں�����

#define MDM_KN_COMMAND			0				//�ں�����
#define SUB_KN_DETECT_SOCKET	1				//�������
#define SUB_KN_VALIDATE_SOCKET	2				//��֤����

///////////////////////////////////////////////////////////////////////////
//�ṹ����
//������֤
typedef struct _stTcpValidate
{
	TCHAR		szValidateKey[64];				//��֤�ַ�
}ST_TCP_VALICATE, *PST_TCP_VALICATE;

//�����ں�
typedef struct _stTcpInfo
{
	BYTE		cbDataKind;						//��������
	BYTE		cbCheckCode;					//Ч���ֶ�
	WORD		wPacketSize;					//���ݴ�С
}ST_TCP_INFO, *PST_TCP_INFO;

//��������
typedef struct _stTcpCmd
{
	WORD		wMainCmdID;						//��������
	WORD		wSubCmdID;						//��������
}ST_TCP_CMD, *PST_TCP_CMD;

//�����ͷ
typedef struct _stTcpHead
{
	_stTcpInfo	stTCPInfo;						//�����ṹ
	_stTcpCmd	stTCPCmd;						//������Ϣ
}ST_TCP_HEAD, *PST_TCP_HEAD;

//���绺��
typedef struct _stTcpBuffer
{
	_stTcpHead	stTcpHead;						//���ݰ�ͷ
	BYTE		cbBuffer[SOCKET_TCP_PACKET];	//���ݻ���
}ST_TCP_BUFFER, *PST_TCP_BUFFER;

//���ݶ���
#pragma pack()
 
#endif
