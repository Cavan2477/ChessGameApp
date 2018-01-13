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

typedef int RESULT_TYPE;            //�������

#define RESULT_DEFAULT      0       //Ĭ�Ͻ��

#define RESULT_SUCC			1       //����ɹ�
#define RESULT_FAILURE      2       //����ʧ��

#define RESULT_BEGAN        3       //����ʼ
#define RESULT_INGED        4       //������
#define RESULT_FINISH       5       //�������

#define RESULT_OTHER        10      //��������

//�û���Ҹı�֪ͨ
const std::string STR_EVENT_USER_DATA_CHANGE	= "whEvent_User_Data_Change";
const std::string STR_EVENT_DESK_STATUS			= "whEvent_Desk_Status";				//����״̬�仯���Ƿ����
const std::string STR_ND_ROOM_START_GAME		= "whNd_Room_StartGame";				//������Ϸ
const std::string STR_ND_ROOM_LEAVE_GAME		= "whNd_Room_LeaveGame";				//�뿪��Ϸ
const std::string STR_ND_HORN_MSG				= "whNd_Horn_Message";					//������Ϣ
const std::string STR_ND_SYS_MSG				= "whNd_Sys_Chat";						//ϵͳ��Ϣ
const std::string STR_ND_SOCKET_CONNECT_FAILURE	= "whNd_Socket_Connect_Failure";		//socket����ʧ��
const std::string STR_ND_ENTER_FOREGROUND		= "whNd_EnterForeground";				//��̨�л�����
const std::string STR_ND_SOCKET_DISCONNECT		= "whNd_Socket_Disconnect";				//����Ͽ�
const std::string STR_ND_ANDROID_PHOTO_IMAGE	= "whNd_Android_PhotoImage";			//android���ͼƬѡ�����
const std::string STR_ND_DOWNLOAD_UPDATE		= "whNd_Download_Update";				//��Դ����ˢ��

#pragma pack()

#endif
















 
 
