/************************************************************************************
 * file: 		Platform.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:18:22
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#pragma pack(1)

#define	BUILD_VER	0	//��Ȩ�汾
#define PRODUCT_VER 6	//��Ʒ�汾
#define GAME_VER    7

//��ڰ汾
#define INTERFACE_VERSION(cbMainVer,cbSubVer)			\
	(((BYTE)(PRODUCT_VER)) << 24) +						\
	(((BYTE)(cbMainVer)) << 16) +						\
	((BYTE)(cbSubVer) << 8) +							\
	(BYTE)(BUILD_VER))

//ģ��汾
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)  \
	(DWORD)(											\
	(((BYTE)(PRODUCT_VER)) << 24) +						\
	(((BYTE)(cbMainVer)) << 16) +						\
	((BYTE)(cbSubVer) << 8) +							\
	(BYTE)(cbBuildVer))

//��Ϸ�汾
#define ROOM_VER(cbMainVer,cbSubVer,cbBuildVer)			\
	(DWORD)(											\
	(((BYTE)(GAME_VER)) << 24) +						\
	(((BYTE)(cbMainVer)) << 16) +						\
	((BYTE)(cbSubVer) << 8) +							\
	(BYTE)(cbBuildVer))

//����汾
#define VERSION_FRAME			PROCESS_VERSION(7,0,1)				//��ܰ汾
#define VERSION_PLAZA			ROOM_VER(7,0,1)                     //�����汾
#define VERSION_MOBILE			PROCESS_VERSION(7,0,1)				//�ֻ��汾
#define VERSION_RENYU           PROCESS_VERSION(6,0,3)              //����
#define VERSION_LKPY			PROCESS_VERSION(6,0,3)              //����
#define VERSION_GSWZ			PROCESS_VERSION(7,0,1)              //��ʽ����

//�汾����
#define VERSION_EFFICACY		0									//Ч��汾
#define VERSION_FRAME_SDK		INTERFACE_VERSION(7,1)				//��ܰ汾

#define VERSION_APP             @"1.0.0"                            //�ͻ��˰汾

#pragma pack()

#endif
