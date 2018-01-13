/************************************************************************************
 * file: 		CMD_GameServer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:26:12
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "MacroDefine.h"
#include "Packet.h"
#include "Struct.h"
#include "Property.h"

#pragma pack(1)

///////////////////////////////////////////////////////////////////////////
//��¼����
#define MDM_GR_LOGON				1									//��¼��Ϣ

#pragma mark -
#pragma mark ��¼ģʽ
#define SUB_GR_LOGON_USERID			1									//id ��¼
#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS		3									//�ʻ���¼

#pragma mark -
#pragma mark ��¼���
#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���

#pragma mark -
#pragma mark ������ʾ
#define SUB_GR_UPDATE_NOTIFY		200									//������ʾ

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark �豸����
#define DEVICE_TYPE_ITOUCH			0x20								//iTouch
#define DEVICE_TYPE_IPHONE			0x40								//iPhone
#define DEVICE_TYPE_IPAD			0x80								//iPad
#define DEVICE_TYPE_DEFAULT         DEVICE_TYPE_IPHONE

#pragma mark -
#pragma mark ��ͼģʽ
#define	VIEW_MODE_ALL				0x0001								//ȫ������
#define	VIEW_MODE_PART				0x0002								//���ֿ���

#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040							//������Ϣ

#pragma mark -
#pragma mark ��Ϣģʽ
#define VIEW_INFO_LEVEL_1			0x0010								//������Ϣ
#define VIEW_INFO_LEVEL_2			0x0020								//������Ϣ
#define VIEW_INFO_LEVEL_3			0x0040								//������Ϣ
#define VIEW_INFO_LEVEL_4			0x0080								//������Ϣ

#pragma mark -
#pragma mark ��������
#define RECVICE_GAME_CHAT			0x0100								//��������
#define RECVICE_ROOM_CHAT			0x0200								//��������
#define RECVICE_ROOM_WHISPER		0x0400								//����˽��

#pragma mark -
#pragma mark ��Ϊ��ʶ
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //��ͨ��¼
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //������¼

//id ��¼
typedef struct _stCmdGrLogonUserID
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾
	
	//��¼��Ϣ
	DWORD							dwUserID;							//�û�id
	TCHAR							szLogonPwd[LEN_MD5];				//��¼����
    TCHAR                           szGameRoomPwd[LEN_PWD];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    WORD                            wGameTypeID;                        //��Ϸ��������
}ST_CMD_GR_LOGON_USER_ID, *PST_CMD_GR_LOGON_USER_ID;

//�ֻ���¼
typedef struct _stCmdGrLogonMobile
{
	//�汾��Ϣ
	WORD							wGameID;							//��Ϸ��ʶ
	DWORD							dwProcessVersion;					//���̰汾
	
	//��������
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����
	
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDynamicPwd[LEN_MD5];				//��¼����
    TCHAR                           szGameRoomPwd[LEN_PWD];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
}ST_CMD_GR_LOGON_MOBILE, *PST_CMD_GR_LOGON_MOBILE;

//�ʺŵ�¼
typedef struct _stCmdGrLogonAccounts
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾
	
	//��¼��Ϣ
	TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//��¼�ʺ�
	TCHAR							szLogonPwd[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GR_LOGON_ACCOUNTS, *PST_CMD_GR_LOGON_ACCOUNTS;

//��¼�ɹ�
typedef struct _stCmdGrLogonSucc
{
	DWORD							dwUserPriview;						//�û�Ȩ��
	DWORD							dwMasterPriview;					//����Ȩ��
}ST_CMD_GR_LOGON_SUCC, *PST_CMD_GR_LOGON_SUCC;

//��¼ʧ��
typedef struct _stCmdGrLogonFailure
{
	INT								nErrorCode;							//�������
	TCHAR							szDes[128];							//��������
}ST_CMD_GR_LOGON_FAILURE, *PST_CMD_GR_LOGON_FAILURE;

//��¼���
typedef struct _stCmdGrLogonFinish
{
    bool                            bGuideTask;                        //��������
}ST_CMD_GR_LOGON_FINISH, *PST_CMD_GR_LOGON_FINISH;

//������ʾ
typedef struct _stCmdUpdateNotify
{	
	//������־
	BYTE							cbMustUpdatePlaza;					//ǿ������
	BYTE							cbMustUpdateClient;					//ǿ������
	BYTE							cbAdviceUpdateClient;				//��������
	
	//��ǰ�汾
	DWORD							dwCurrPlazaVersion;					//��ǰ�汾
	DWORD							dwCurrFrameVersion;					//��ǰ�汾
	DWORD							dwCurrClientVersion;				//��ǰ�汾
}ST_CMD_GR_UPDATE_NOTIFY, *PST_CMD_GR_UPDATE_NOTIFY;

////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark ��������

#define	MDM_GR_CONFIG				2									//������Ϣ

#define	SUB_GR_CONFIG_COLUMN		100									//�б�����
#define SUB_GR_CONFIG_SERVER		101									//��������
#define SUB_GR_CONFIG_PROPERTY		102									//��������

#define SUB_GR_CONFIG_FINISH		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT    104                                 //���Ȩ��

/////////////////////////////////////////////////////////////////////////////////

//�б�����
typedef struct _stCmdGrConfigColumn
{
	BYTE							cbColumnCount;						//�б���Ŀ
	_stColumnItem					stColumnItemArray[MAX_COLUMN];		//�б�����
}ST_CMD_GR_CONFIG_COLUMN, *PST_CMD_GR_CONFIG_COLUMN;

//��������
typedef struct _stCmdGrConfigGameRoom
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ
	
	//��������
	WORD							wGameRoomType;						//��������
	DWORD							dwGameRoomRule;						//�������
}ST_CMD_GR_CONFIG_GAME_ROOM, *PST_CMD_GR_CONFIG_GAME_ROOM;

//��������
typedef struct _stCmdGrConfigProperty
{
	BYTE							cbPropertyCount;					//������Ŀ
	ST_PROPERTY_INFO				stPropertyInfoArray[MAX_PROPORTY];	//��������
}ST_CMD_GR_CONFIG_PROPERTY, *PST_CMD_GR_CONFIG_PROPERTY;

//���Ȩ��
typedef struct _stCmdGrConfigUserPriview
{
    DWORD                           dwUserPriview;                      //���Ȩ��
}ST_CMD_GR_CONFIG_USER_PRIVIEW, *PST_CMD_GR_CONFIG_USER_PRIVIEW;

/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark �û�����

#define MDM_GR_USER					3									//�û���Ϣ

//�û�����
#define SUB_GR_USER_RULE			1									//�û�����
#define	SUB_GR_USER_LOOK_ON			2									//�Թ�����
#define	SUB_GR_USER_SIT_DOWN		3									//��������
#define	SUB_GR_USER_STAND_UP		4									//��������
#define SUB_GR_USER_INVITE			5									//�û�����
#define SUB_GR_USER_INVITE_REQ		6									//��������
#define SUB_GR_USER_REPULSE_SIT  	7									//�ܾ��������
#define SUB_GR_USER_KICK_USER       8                                   //�߳��û�
#define SUB_GR_USER_INFO_REQ        9                                   //�����û���Ϣ
#define SUB_GR_USER_CHAIR_REQ       10                                  //�������λ��
#define SUB_GR_USER_CHAIR_INFO_REQ	11									//���������û���Ϣ
#define SUB_GR_USER_WAIT_DISTRIBUTE 12                                  //�ȴ�����

//�û�״̬
#define	SUB_GR_USER_ENTER			100									//�û�����
#define	SUB_GR_USER_GAME_COIN		101									//�û���Ϸ��
#define SUB_GR_USER_STATUS			102									//�û�״̬

#define	SUB_GR_REQ_FAILURE			103									//����ʧ��

//��������
#define	SUB_GR_USER_CHAT			201									//������Ϣ
#define	SUB_GR_USER_EXPRESSION		202									//������Ϣ
#define SUB_GR_WISPER_CHAT			203									//˽����Ϣ
#define	SUB_GR_WISPER_EXPRESSION	204									//˽�ı���
#define SUB_GR_COLLOQUY_CHAT		205									//�Ự��Ϣ
#define	SUB_GR_COLLOQUY_ERPRESSION	206									//�Ự����

//��������
#define SUB_GR_PROPERTY_BUY			300									//�������
#define SUB_GR_PROPERTY_SUCCESS		301									//���߳ɹ�
#define SUB_GR_PROPERTY_FAILURE		302									//����ʧ��
#define SUB_GR_PROPERTY_MESSAGE     303                                 //������Ϣ
#define SUB_GR_PROPERTY_EFFECT      304                                 //����ЧӦ
#define SUB_GR_PROPERTY_TRUMPET     305                                 //�û�����

//��������
#define SUB_GR_PLAYTIME_PRESENT     400                                 //����֪ͨ
#define SUB_GR_PRESENT_DRAW         401                                 //������ȡ

#define SR_FORFEND_LOCK_TABLE		0x00200000							//��ֹ����

/////////////////////////////////////////////////////////////////////////////////

//�Թ�����
typedef struct _stCmdGrUserLookon
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
}ST_CMD_GR_USER_LOOK_ON, *PST_CMD_GR_USER_LOOK_ON;

//��������
typedef struct _stCmdGrUserSitDown
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	TCHAR							szTablePwd[LEN_PWD];				//��������
}ST_CMD_GR_USER_SIT_DOWN, *PST_CMD_GR_USER_SIT_DOWN;

//��������
typedef struct _stCmdGrUserStandUp
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbForceLeave;						//ǿ���뿪
}ST_CMD_GR_USER_STAND_UP, *PST_CMD_GR_USER_STAND_UP;

//�����û�
typedef struct _stCmdGrUserInvite
{
    WORD                            wTableID;                           //������
    DWORD                           dwUserID;                           //�û�id
}ST_CMD_GR_USER_INVITE, *PST_CMD_GR_USER_INVITE;

//�����û�����
typedef struct _stCmdGrUserInviteReq
{
    WORD                            wTableID;                           //������
    DWORD                           dwUserID;                           //�û�id
}ST_CMD_GR_USER_INVITE_REQ, *PST_CMD_GR_USER_INVITE_REQ;

//�����û���Ϣ
typedef struct _stCmdGrUserInfoReq
{
    DWORD							dwUserIDReq;						//�����û�
    WORD							wTablePos;							//����λ��
}ST_CMD_GR_USER_INFO_REQ, *PST_CMD_GR_USER_INFO_REQ;

//����������Ϣ
typedef struct _stCmdGrUserInfoChairReq
{
    WORD							wTableID;							//���Ӻ���
    WORD							wChairID;							//���Ӻ���
}ST_CMD_GR_USER_INFO_CHAIR_REQ, *PST_CMD_GR_USER_INFO_CHAIR_REQ;

//�û�����
typedef struct _stCmdGrUserScore
{
	DWORD							dwUserID;							//�û���ʶ
	_stUserScore					stUserScore;						//������Ϣ
}ST_CMD_GR_USER_SCORE, *PST_CMD_GR_USER_SCORE;

#pragma mark -
#pragma mark �û���Ϸ��
typedef struct _stCmdGrMobileUserGameCoin
{
	DWORD							dwUserID;							//�û���ʶ
	_stMobileUserGameCoin			stMobileUserGameCoin;				//�ֻ��û���Ϸ����Ϣ
}ST_CMD_GR_MOBILE_USER_GAME_COIN, *PST_CMD_GR_MOBILE_USER_GAME_COIN;

//�û�״̬
typedef struct _stCmdGrUserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	_stUserStatus					stUserStatus;						//�û�״̬
}ST_CMD_GR_USER_STATUS, *PST_CMD_GR_USER_STATUS;

//����ʧ��
typedef struct _stCmdGrReqFailure
{
	INT								nErrorCode;							//�������
	TCHAR							szDes[256];							//������Ϣ
}ST_CMD_GR_REQ_FAILURE, *PST_CMD_GR_REQ_FAILURE;

//�û�����
typedef struct _stCmdGrUserClientChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMsg[LEN_USER_CHAT_MSG];		//������Ϣ
}ST_CMD_GR_USER_CLIENT_CHAT, *PST_CMD_GR_USER_CLIENT_CHAT;

//�û�����
typedef struct _stCmdGrUserServerChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMsg[LEN_USER_CHAT_MSG];		//������Ϣ
}ST_CMD_GR_USER_SERVER_CHAT, *PST_CMD_GR_USER_SERVER_CHAT;

//�û�����
typedef struct _stCmdGrUserClientExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
}ST_CMD_GR_USER_CLIENT_EXPRESSION, *PST_CMD_GR_USER_CLIENT_EXPRESSION;

//�û�����
typedef struct _stCmdGrUserServerExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
}ST_CMD_GR_USER_SERVER_EXPRESSION, *PST_CMD_GR_USER_SERVER_EXPRESSION;

//�û�˽��
typedef struct _stCmdGrUserClientWisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
    DWORD                           dwSendUserID;                       //�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMsg[LEN_USER_CHAT_MSG];		//������Ϣ
}ST_CMD_GR_USER_CLIENT_WISPER_CHAT, *PST_CMD_GR_USER_CLIENT_WISPER_CHAT;

//�û�˽��
typedef struct _stCmdGrUserServerWisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMsg[LEN_USER_CHAT_MSG];		//������Ϣ
}ST_CMD_GR_USER_SERVER_WISPER_CHAT, *PST_CMD_GR_USER_SERVER_WISPER_CHAT;

//˽�ı���
typedef struct _stCmdGrUserClientWisperExpression
{
	WORD							wItemIndex;							//��������
    DWORD                           dwSendUserID;                       //�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
}ST_CMD_GR_USER_CLIENT_WISPER_EXPRESSION, *PST_CMD_GR_USER_CLIENT_WISPER_EXPRESSION;

//˽�ı���
typedef struct _stCmdGrUserServerWisperExpression
{
	DWORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
}ST_CMD_GR_USER_SERVER_WISPER_EXPRESSION, *PST_CMD_GR_USER_SERVER_WISPER_EXPRESSION;

//�û��Ự
typedef struct _stCmdGrGrColloquyChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwConversationID;					//�Ự��ʶ
	DWORD							dwTargetUserID[16];					//Ŀ���û�
	TCHAR							szChatMsg[LEN_USER_CHAT_MSG];		//������Ϣ
}ST_CMD_GR_GR_COLLOQUY_CHAT, *PST_CMD_GR_GR_COLLOQUY_CHAT;

//�����û�
typedef struct _stCmdGrUserClientInviteUser
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwSendUserID;						//�����û�
}ST_CMD_GR_USER_CLIENT_INVITE_USER, *PST_CMD_GR_USER_CLIENT_INVITE_USER;

//�����û�
typedef struct _stCmdGrUserServerInviteUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
}ST_CMD_GR_USER_SERVER_INVITE_USER, *PST_CMD_GR_USER_SERVER_INVITE_USER;

//�������
typedef struct _stCmdGrClientBuyProperty
{
    BYTE                            cbRequestArea;                      //����Χ
    BYTE                            cbConsumScore;                      //��������
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwTargetUserID;						//ʹ�ö���
}ST_CMD_GR_CLIENT_BUY_PROPERTY, *PST_CMD_GR_CLIENT_BUY_PROPERTY;

//���߳ɹ�
typedef struct _stCmdGrServerPropertySucc
{
    BYTE							cbRequestArea;						//ʹ�û���
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//Ŀ�����
    DWORD							dwTargetUserID;						//ʹ�ö���
}ST_CMD_GR_SERVER_PROPERTY_SUCC, *PST_CMD_GR_SERVER_PROPERTY_SUCC;

//����ʧ��
typedef struct _stCmdGrServerPropertyFailure
{
    WORD							wReqArea;							//��������
	INT								nErrorCode;							//�������
	TCHAR               			szDes[256];							//������Ϣ
}ST_CMD_GR_SERVER_PROPERTY_FAILURE, *PST_CMD_GR_SERVER_PROPERTY_FAILURE;

//������Ϣ
typedef struct _stCmdGrServerPropertyMsg
{
	WORD                			wPropertyIndex;						//��������
	WORD                			wPropertyCount;						//������Ŀ
	DWORD               			dwSourceUserID;						//Ŀ�����
	DWORD               			dwTargetUserID;						//ʹ�ö���
}ST_CMD_GR_SERVER_PROPERTY_MSG, *PST_CMD_GR_SERVER_PROPERTY_MSG;

//����ЧӦ
typedef struct _stCmdGrServerPropertyEffect
{
	DWORD               			wUserID;							//�û�I D
	BYTE                			cbVipLevel;							//��Ա�ȼ�
}ST_CMD_GR_SERVER_PROPERTY_EFFECT, *PST_CMD_GR_SERVER_PROPERTY_EFFECT;

//��������
typedef struct _stCmdGrClientSendTrumpet
{
    BYTE							cbRequestArea;
    WORD							wPropertyIndex;
    DWORD							TrumpetColor;
    TCHAR							szTrumpetContent[TRUMPET_MAX_CHAR];
}ST_CMD_GR_CLIENT_SEND_TRUMPET, *PST_CMD_GR_CLIENT_SEND_TRUMPET;

//��������
typedef struct _stCmdGrServerSendTrumpet
{
	WORD							wPropertyIndex;
	DWORD							dwSendUserID;
	DWORD							dwTrumpetColor;
	TCHAR							szSendNickName[32];
	TCHAR							szTrumpetContent[TRUMPET_MAX_CHAR];
}ST_CMD_GR_SERVER_SEND_TRUMPET, *PST_CMD_GR_SERVER_SEND_TRUMPET;

//�û��ܾ�����������
typedef struct _stCmdGrUserRepulseSit
{
    WORD							wTableID;
    WORD							wChairID;
	DWORD							dwUserID;							//�û�id
	DWORD							dwrepulseUserID;					//�û�id
}ST_CMD_GR_USER_REPULSE_SIT, *PST_CMD_GR_USER_REPULSE_SIT;

//����֪ͨ
typedef struct _stCmdGrPlayTimePresentNotify
{
	DWORD							dwPlayTimeCount;					//��Ϸʱ��
	DWORD							dwPresentPlayTime;					//����ʱ��
}ST_CMD_GR_PLAY_TIME_PRESENT_NOTIFY, *PST_CMD_GR_PLAY_TIME_PRESENT_NOTIFY;

//������ȡ
typedef struct _stCmdGrPlayTimePresentDraw
{
    BYTE							cbActionGame;						//��Ϸ����-0:������ȡ | 1:��Ϸ��ȡ
}ST_CMD_GR_PLAY_TIME_PRESENT_DRAW, *PST_CMD_GR_PLAY_TIME_PRESENT_DRAW;

/////////////////////////////////////////////////////////////////////////////////

//�����־
#define UR_LIMIT_SAME_IP			0x01								//���Ƶ�ַ
#define UR_LIMIT_WIN_RATE			0x02								//����ʤ��
#define UR_LIMIT_FLEE_RATE			0x04								//��������
#define UR_LIMIT_GAME_SCORE			0x08								//���ƻ���

//�û�����
typedef struct _stCmdGrUserRule
{
	BYTE							cbRuleMask;							//��������
	WORD							wMinWinRate;						//���ʤ��
	WORD							wMaxFleeRate;						//�������
	INT								nMaxGameGameCoin;					//��߷���
	INT								nMinGameGameCoin;					//��ͷ���
}ST_CMD_GR_USER_RULE, *PST_CMD_GR_USER_RULE;

/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark ״̬����

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define	SUB_GR_TABLE_STATUS			101									//����״̬

//������Ϣ
typedef struct _stCmdGrTableInfo
{
	WORD							wTableCount;						//������Ŀ
	_stTableStatus					stTableStatusArray[512];			//����״̬
}ST_CMD_GR_TABLE_INFO, *PST_CMD_GR_TABLE_INFO;

//����״̬
typedef struct _stCmdGrTableStatus
{
	WORD							wTableID;							//���Ӻ���
	_stTableStatus					stTableStatus;						//����״̬
}ST_CMD_GR_TABLE_STATUS, *PST_CMD_GR_TABLE_STATUS;

/////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_BANK								5						//�û���Ϣ

//��������
#define SUB_GR_REQ_BANK_ENABLE					1						//��ͨ����
#define SUB_GR_REQ_BANK_QUERY					2						//��ѯ����
#define SUB_GR_REQ_BANK_GOLD_SAVE				3						//��Ҵ������
#define SUB_GR_REQ_BANK_GOLD_TAKE_OUT			4						//���ȡ������
#define SUB_GR_REQ_BANK_GOLD_TRANSFER			5						//ת�˲���
#define SUB_GR_REQ_BANK_QUERY_USER_INFO			6						//��ѯ�û�

#define SUB_GR_USER_BANK_INFO					100						//��������
#define SUB_GR_USER_BANK_SUCC					101						//���гɹ�
#define SUB_GR_USER_BANK_FAILURE				102						//����ʧ��
#define SUB_GR_USER_BANK_TRANSFER_USER_INFO     103						//�û�����
#define SUB_GR_USER_BANK_ENABLE_RESULT			104						//��ͨ���

//������ʽ
#define BANK_SAVE								0						//��ȡ��ʽ
#define BANK_TRANSFER							1						//ת�ʷ�ʽ

//��ͨ����
typedef struct _stCmdGrClientReqBankEnable
{
    BYTE							cbActivityGame;						//��Ϸ����
    DWORD							dwUserID;							//�û�id
    TCHAR							szLogonPwd[LEN_PWD];				//��¼����
    TCHAR							szBankPwd[LEN_PWD];					//��������
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GR_CLIENT_REQ_BANK_ENABLE, *PST_CMD_GR_CLIENT_REQ_BANK_ENABLE;

#pragma mark -
#pragma mark �ͻ��˽ṹ��
//��ѯ����
typedef struct _stCmdGrClientReqBankQuery
{
	BYTE							cbActivityGame;						//��Ϸ����
	TCHAR							szBankPwd[LEN_PWD];					//��������
}ST_CMD_GR_CLIENT_REQ_BANK_QUERY, *PST_CMD_GR_CLIENT_REQ_BANK_QUERY;

//�������
typedef struct _stCmdGrClientReqBankGoldSave
{
	BYTE							cbActivityGame;						//��Ϸ����
	LONGLONG						llGoldSave;							//�����Ŀ
}ST_CMD_GR_CLIENT_REQ_BANK_GOLD_SAVE, *PST_CMD_GR_CLIENT_REQ_BANK_GOLD_SAVE;

//ȡ������
typedef struct _stCmdGrClientReqBankGoldTakeOut
{
	BYTE							cbAvtivityGame;						//��Ϸ����
	LONGLONG						llGoldTakeOut;						//ȡ����Ŀ
	TCHAR							szBankPwd[LEN_PWD];					//��������
}ST_CMD_GR_CLIENT_REQ_BANK_GOLD_TAKE_OUT, *PST_CMD_GR_CLIENT_REQ_BANK_GOLD_TAKE_OUT;

//��ѯ�û�
typedef struct _stCmdGrClientReqBankQueryUserInfo
{
    BYTE							cbActivityGame;                     //��Ϸ����
    BYTE							cbByNickName;                       //�ǳ�����
    TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//Ŀ���û�
}ST_CMD_GR_CLIENT_REQ_BANK_QUERY_USER_INFO, *PST_CMD_GR_CLIENT_REQ_BANK_QUERY_USER_INFO;

//ת�ʽ��
typedef struct _stCmdGrClientReqBankGoldTransfer
{
	BYTE							cbActivityGame;						//��Ϸ����
	LONGLONG						lTransferGold;						//ת�ʽ��
	TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//Ŀ���û�
	TCHAR							szInsurePwd[LEN_PWD];				//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];    //ת�ʱ�ע
}ST_CMD_GR_CLIENT_REQ_BANK_GOLD_TRANSFER, *PST_CMD_GR_CLIENT_REQ_BANK_GOLD_TRANSFER;

#pragma mark -
#pragma mark ���������ͽṹ��
//��������
typedef struct _stCmdGrServerUserBankInfo
{
	BYTE							cbActivityGame;						//��Ϸ����
	BYTE							cbEnjoinTransfer;                   //ת�ʿ���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferVip;				//˰�ձ���
	WORD							wGameRoomID;						//�����ʶ
	LONGLONG						llUserGold;							//�û����
	LONGLONG						llUserBankGold;						//���н��
	LONGLONG						llTransferCondition;				//ת������
}ST_CMD_GR_SERVER_USER_BANK_INFO, *PST_CMD_GR_SERVER_USER_BANK_INFO;

//���гɹ�
typedef struct _stCmdGrServerUserBankSucc
{
	BYTE							cbActivityGame;						//��Ϸ����
    BYTE							cbOperateType;                      //��������
    DWORD							dwUserMedal;                        //�û�����
    DWORD							dwRecordNo;							//��¼���
	LONGLONG						llUserGold;							//�û����
	LONGLONG						llUserBankGold;						//���н��
	TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GR_SERVER_USER_BANK_SUCC, *PST_CMD_GR_SERVER_USER_BANK_SUCC;

//����ʧ��
typedef struct _stCmdGrServerUserBankFailure
{
	BYTE							cbActivityGame;						//��Ϸ����
	INT								nErrorCode;							//�������
	TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GR_SERVER_USER_BANK_FAILURE, *PST_CMD_GR_SERVER_USER_BANK_FAILURE;

//�û���Ϣ
typedef struct _stCmdGrServerUserTransferUserInfo
{
	BYTE							cbActivityGame;						//��Ϸ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//Ŀ���û�
}ST_CMD_GR_SERVER_USER_TRANSFER_USRE_INFO, *PST_CMD_GR_SERVER_USER_TRANSFER_USRE_INFO;

//��ͨ���
typedef struct _stCmdGrServerUserBankEnableResult
{
	BYTE							cbActivityGame;						//��Ϸ����
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GR_SERVER_USER_BANK_ENABLE_RESULT, *PST_CMD_GR_SERVER_USER_BANK_ENABLE_RESULT;

/////////////////////////////////////////////////////////////////////////////////

#define	MDM_GR_TASK                 6									//��������

#define SUB_GR_TASK_LOAD_INFO       1                                   //��������
#define SUB_GR_TASK_TAKE            2                                   //��ȡ����
#define SUB_GR_TASK_REWARD          3                                   //������
#define SUB_GR_TASK_INFO            4                                   //������Ϣ
#define SUB_GR_TASK_FINISH          5                                   //�������
#define SUB_GR_TASK_LIST            6                                   //�����б�
#define SUB_GR_TASK_RESULT          7                                   //������

///////////////////////////////////////////////////////////////////////////////////

//��������
typedef struct _stCmdGrClientLoadTaskInfo
{
    DWORD							dwUserID;
    TCHAR							szLogonPwd[LEN_PWD];				//��¼����
}ST_CMD_GR_CLIENT_LOAD_TASK_INFO, *PST_CMD_GR_CLIENT_LOAD_TASK_INFO;

//��ȡ����
typedef struct _stCmdGrClientTaskGet
{
	WORD							wTaskID;                            //�����ʶ
	DWORD							dwUserID;                           //�û���ʶ
    TCHAR							szLogonPwd[LEN_PWD];				//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];        //��������
}ST_CMD_GR_CLIENT_TASK_GET, *PST_CMD_GR_CLIENT_TASK_GET;

//��ȡ����
typedef struct _stCmdGrClientTaskReward
{
	WORD							wTaskID;                            //�����ʶ
	DWORD							dwUserID;                           //�û���ʶ
	TCHAR							szLogonPwd[LEN_PWD];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];        //��������
}ST_CMD_GR_CLIENT_TASK_REWARD, *PST_CMD_GR_CLIENT_TASK_REWARD;

//������Ϣ
typedef struct _stCmdGrServerTaskInfo
{
    WORD							wTaskCount;							//��������
    ST_TASK_STATUS					stTaskStatusArray[TASK_MAX_COUNT];	//����״̬
}ST_CMD_GR_SERVER_TASK_INFO, *PST_CMD_GR_SERVER_TASK_INFO;

//�������
typedef struct _stCmdGrServerTaskFinish
{
	WORD							wTaskID;							//�����ʶ
	TCHAR							szTaskName[LEN_TASK_NAME];          //��������
}ST_CMD_GR_SERVER_TASK_FINISH, *PST_CMD_GR_SERVER_TASK_FINISH;

//������
typedef struct _stCmdGrServerTaskResult
{
    //�����Ϣ
    bool							bSucc;								//�ɹ���ʶ
    WORD							wCommandID;                         //�����ʶ
    
    //�Ƹ���Ϣ
    LONGLONG						llCurrGameCoin;						//��ǰ��Ϸ��
    LONGLONG						llCurGold;							//��ǰ���
    
    //��ʾ��Ϣ
    TCHAR							szNotifyContent[128];               //��ʾ����
}ST_CMD_GR_SERVER_TASK_RESULT, *PST_CMD_GR_SERVER_TASK_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////

//�һ�����
#define MDM_GR_EXCHANGE						7                           //�һ�����

#define  SUB_GR_EXCHANGE_LOAD_INFO          400                         //������Ϣ
#define  SUB_GR_EXCHANGE_PARAM_INFO         401                         //�һ�����
#define  SUB_GR_PURCHASE_MEMBER             402                         //�����Ա
#define  SUB_GR_PURCHASE_RESULT             403                         //������
#define  SUB_GR_EXCHANGE_GAME_COIN_BY_GOLD  404                         //��Ҷһ���Ϸ��
#define  SUB_GR_EXCHANGE_GAME_COIN_BY_BEAN  405							//��Ϸ���һ���Ϸ��
#define  SUB_GR_EXCHANGE_RESULT             406                         //�һ����

///////////////////////////////////////////////////////////////////////////////////////////////
//��ѯ����
typedef struct _stCmdGrExchangeParam
{
    DWORD							dwGoldExchangeGameCoinRate;			//��ҡ���Ϸ�Ҷһ�����
    DWORD							dwPresentExchangeGameCoinRate;		//��������Ϸ�Ҷһ���
    DWORD							dwRateBeanExchangeGameCoin;			//��Ϸ������Ϸ�Ҷһ���
    WORD							wVipCount;							//��Ա��Ŀ
    _stVipParam						stVipParamArray[10];				//��Ա����
}ST_CMD_GR_EXCHANGE_PARAM, *PST_CMD_GR_EXCHANGE_PARAM;

//�һ���Ϸ��
typedef struct _stCmdGrExchangeGameCoin
{
    DWORD                           dwUserID;                           //�û���ʶ
    LONGLONG						llExchangedGold;                    //���һ����
    TCHAR                           szMachineID[LEN_MACHINE_ID];        //������ʶ
}ST_CMD_GR_EXCHANGE_GAME_GOIN, *PST_CMD_GR_EXCHANGE_GAME_GOIN;

//�һ���Ϸ��
typedef struct _stCmdGrExchangeGameCoinByBean
{
    DWORD							dwUserID;							//�û���ʶ
    double							dExchangedBean;						//���һ���Ϸ��
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
}ST_CMD_GR_EXCHANGE_GAME_COIN_BY_BEAN, *PST_CMD_GR_EXCHANGE_GAME_COIN_BY_BEAN;

//�һ����
typedef struct _stCmdGrExchangeResult
{
    bool                            bSucc;								//�ɹ���ʶ
    LONGLONG						llCurrGameCoin;                     //��ǰ��Ϸ��
    LONGLONG						llCurGold;							//��ǰ���
    TCHAR                           szNotifyMsg[128];					//��ʾ��Ϣ
}ST_CMD_GR_EXCHANGE_RESULT, *PST_CMD_GR_EXCHANGE_RESULT;

//�����Ա
typedef struct _stCmdGrPurchaseVip
{
	DWORD                           dwUserID;                           //�û���ʶ
	BYTE                            cbVipFlag;							//��Ա��ʶ
	WORD                            wPurchaseTime;                      //����ʱ��
	TCHAR                           szMachineID[LEN_MACHINE_ID];        //������ʶ
}ST_CMD_GR_PURCHASE_VIP, *PST_CMD_GR_PURCHASE_VIP;

//������
typedef struct _stCmdGrPurchaseResult
{
	bool                            bSucc;								//�ɹ���ʶ
	BYTE                            cbVipOrder;							//��Աϵ��
	LONGLONG						llCurrGameCoin;                     //��ǰ��Ϸ��
	DWORD                           dwCurrBean;							//��ǰ��Ϸ��
	TCHAR                           szNotifyMsg[128];					//��ʾ��Ϣ
}ST_CMD_GR_PURCHASE_RESULT, *PST_CMD_GR_PURCHASE_RESULT;

/////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#define      MDM_GR_MANAGE          8                                   //��������

#define      SUB_GR_SEND_WARNING    1                                   //���;���
#define      SUB_GR_SEND_MESSAGE    2                                   //������Ϣ
#define      SUB_GR_LOOK_USER_IP    3                                   //�鿴��ַ
#define      SUB_GR_KILL_USER       4                                   //�޳��û�
#define      SUB_GR_LIMIT_ACCOUNTS  5                                   //��ֹ�ʻ�
#define      SUB_GR_SET_USER_RIGHT  6                                   //Ȩ������

//��������
#define      SUB_GR_QUERY_OPTION    7                                   //��ѯ����
#define      SUB_GR_OPTION_SERVER   8                                   //��������
#define      SUB_GR_OPTION_CURRENT  9                                   //��ǰ����
#define      SUB_GR_LIMIT_USER_CHAT 10                                  //��������
#define      SUB_GR_KICK_ALL_USER   11                                  //�޳��û�
#define      SUB_GR_DISMISSGAME     12                                  //������Ϸ

//...
//���;���
typedef struct _stCmdGrSendWarning
{
    WORD                            wLen;								//��Ϣ����
    DWORD                           dwUserID;							//Ŀ���û�
	TCHAR                           szWarningMsg[LEN_MSG];				//������Ϣ
}ST_CMD_GR_SEND_WARNING, *PST_CMD_GR_SEND_WARNING;

//ϵͳ��Ϣ
typedef struct _stCmdGrSendMsg
{
	BYTE                            cbGame;                             //��Ϸ��Ϣ
	BYTE                            cbRoom;                             //��Ϸ��Ϣ
	BYTE                            cbAllRoom;                          //��Ϸ��Ϣ
	BYTE                            cbLoop;                             //ѭ����ʶ
	DWORD                           dwTimeRate;                         //ѭ�����
	INT								nEndTime;							//����ʱ��
	WORD                            wLen;								//��Ϣ����
	TCHAR                           szSysMsg[LEN_MSG];					//ϵͳ��Ϣ
}ST_CMD_GR_SEND_MSG, *PST_CMD_GR_SEND_MSG;

//�鿴��ַ
typedef struct _stCmdGrUserLookIP
{
    DWORD                           dwUserID;
}ST_CMD_GR_USER_LOOK_IP, *PST_CMD_GR_USER_LOOK_IP;

//�߳��û�
typedef struct _stCmdGrUserKickOut
{
    DWORD                           dwUserID;
}ST_CMD_GR_USER_KICK_OUT, *PST_CMD_GR_USER_KICK_OUT;

//�����ʻ�
typedef struct _stCmdGrUserDisableAccounts
{
    DWORD                           dwUserID;
}ST_CMD_GR_USER_DISABLE_ACCOUNTS, *PST_CMD_GR_USER_DISABLE_ACCOUNTS;

//Ȩ������
typedef struct _stCmdGrUserGetPriview
{
    //Ŀ���û�
    WORD                            dwUserID;
    
    //�󶨱���
    BYTE                            cbGamePriview;                      //�ʺ�Ȩ��
    BYTE                            cbAccountsPriview;                  //�ʺ�Ȩ��
    
    //Ȩ�ޱ仯
    BYTE                            cbLimitRoomChat;                    //��������
    BYTE                            cbLimitGameChat;                    //��Ϸ����
    BYTE                            cbLimitPlayGame;                    //��ϷȨ��
    BYTE                            cbLimitSendWisper;                  //������Ϣ
    BYTE                            cbLimitLookonGame;                  //�Թ�Ȩ��
}ST_CMD_GR_USER_SET_PRIVIEW, *PST_CMD_GR_USER_SET_PRIVIEW;

//��������
typedef struct _stCmdGrGameRoomSetting
{
    DWORD                           dwRuleMask;                         //��������
    _stGameRoomConfig				stGameRoomConfig;                   //��������
}ST_CMD_GR_GAME_ROOM_SETTING, *PST_CMD_GR_GAME_ROOM_SETTING;

//��Ϸ��������
typedef struct _stCmdGrGameRoomConfig
{
    _stGameRoomConfig				stGameRoomConfig;                   //��������
}ST_CMD_GR_GAME_ROOM_CONFIG;

//�޳������û�
typedef struct _stCmdGrUserKickOutAll
{
	TCHAR                           szMsg[LEN_MSG];						//�߳���ʾ
}ST_CMD_GR_USER_KICK_OUT_ALL, *PST_CMD_GR_USER_KICK_OUT_ALL;

typedef struct _stCmdGrDissmissGame
{
    WORD                            wTableNum;							//��ɢ����
}ST_CMD_GR_DISSMISS_GAME, *PST_CMD_GR_DISSMISS_GAME;

//���ñ�ʶ
#define  OSF_ROOM_CHAT              1                                   //��������
#define  OSF_GAME_CHAT              2                                   //��Ϸ����
#define  OSF_ROOM_WISPER            3                                   //����˽��
#define  OSF_ENTER_TABLE            4                                   //������Ϸ
#define  OSF_ENTER_SERVER           5                                   //���뷿��
#define  OSF_SEND_BUGLE             12                                  //��������

//��������
typedef struct _stCmdGrServerGameRoomConfig
{
	BYTE                            cbFlags;							//���ñ�ʶ
	BYTE                            cbValue;							//����ֵ
}ST_CMD_GR_SERVER_GAME_ROOM_CONFIG, *PST_CMD_GR_SERVER_GAME_ROOM_CONFIG;

//��������
typedef struct _stCmdGrUserLimitChat
{
	DWORD                           dwUserID;
	BYTE                            cbFlags;							//���Ʊ�־
	BYTE                            cbValue;							//�������
}ST_CMD_GR_USER_LIMIT_CHAT, *PST_CMD_GR_USER_LIMIT_CHAT;

////////////////////////////////////////////////////////////////////////////////////////////
//��������
#define MDM_GR_MATCH				9									//��������

#define SUB_GR_MATCH_FEE			400									//��������
#define SUB_GR_MATCH_NUM			401									//�ȴ�����
#define SUB_GR_LEAVE_MATCH			402									//�˳�����
#define SUB_GR_MATCH_INFO			403									//������Ϣ
#define SUB_GR_MATCH_WAIT_TIP		404									//�ȴ���ʾ
#define SUB_GR_MATCH_RESULT			405									//�������
#define SUB_GR_MATCH_STATUS			406									//����״̬
#define SUB_GR_MATCH_DESC			408									//��������
#define SUB_GR_MATCH_GOLD_UPDATE    409									//��Ҹ���
#define SUB_GR_MATCH_ELIMINATE      410                                 //������̭

//��������
typedef struct _stCmdGrMatchFee
{
    LONGLONG                       llSignUpFee;                         //��������
	TCHAR                          szNotifyMsg[LEN_MSG];				//��ʾ����
}ST_CMD_GR_MATCH_FEE, *PST_CMD_GR_MATCH_FEE;

//��������
typedef struct _stCmdGrMatchUserNum
{
	DWORD							dwWaittingNum;						//�ȴ�����
	DWORD							dwTotalNum;							//��������
}ST_CMD_GR_MATCH_USER_NUM, *PST_CMD_GR_MATCH_USER_NUM;

//������Ϣ
typedef struct _stCmdGrMatchInfo
{
	TCHAR							szTitleArray[4][LEN_TITLE_MSG];		//��Ϣ����
    WORD							wGameOfNum;							//��Ϸ����
}ST_CMD_GR_MATCH_INFO, *PST_CMD_GR_MATCH_INFO;

//��ʾ��Ϣ
typedef struct _stCmdGrMatchWaitTip
{
	LONGLONG						llCurrGameCoin;						//��ǰ��Ϸ��
	WORD							wCurrRank;							//��ǰ����
	WORD							wCurrTableRank;						//��������
	WORD							wUserOfNum;							//��ǰ����
    WORD                            wCurrGameOfNum;                     //��ǰ����
    WORD                            wGameTotal;                         //�ܹ�����
	WORD							wPlayingTable;						//��Ϸ����
    TCHAR							szMatchName[LEN_GAME_ROOM];			//��������
}ST_CMD_GR_MATCH_WAIT_TIP, *PST_CMD_GR_MATCH_WAIT_TIP;

//�������
typedef struct _stCmdGrMatchResult
{
    LONGLONG						llGameCoin;							//��Ϸ�ҽ���
    DWORD							dwGold;								//��ҽ���
    DWORD							dwExp;								//���齱��
	TCHAR							szDes[LEN_DES];						//�ý�����
}ST_CMD_GR_MATCH_RESULT, *PST_CMD_GR_MATCH_RESULT;

#define MAX_MATCH_DES				4									//�������

//��������
typedef struct _stCmdGrMatchDes
{
    TCHAR							szTitleArray[MAX_MATCH_DES][16];	//��Ϣ����
	TCHAR							szDes[MAX_MATCH_DES][LEN_MATCH_DES];//��������
    DWORD                           dwTitleColor;						//������ɫ
    DWORD                           dwDesColor;							//������ɫ
}ST_CMD_GR_MATCH_DES, *PST_CMD_GR_MATCH_DES;

//��Ҹ���
typedef struct _stCmdGrMatchGoldUpdate
{
    LONGLONG                       llCurrGameCoin;						//��ǰ��Ϸ��
    LONGLONG                       llCurrGold;							//��ǰ���
    DWORD                          dwCurrExp;							//��ǰ����
}ST_CMD_GR_MATCH_GOLD_UPDATE, *PST_CMD_GR_MATCH_GOLD_UPDATE;

////////////////////////////////////////////////////////////////////////////////
//...

///////////////////////////////////////////////////////////////////////////////
//�������

#define	MDM_GF_FRAME				100									//�������

//�û�����
#define SUB_GF_GAME_OPTION			1									//��Ϸ����
#define SUB_GF_USER_READY			2									//�û�׼��
#define SUB_GF_LOOKON_CONFIG		3									//�Թ�����
#define SUB_GF_DISTRIBUTE_AFFIRM	4									//����ȷ��

//��������
#define SUB_GF_USER_CHAT			10									//�û�����
#define SUB_GF_USER_EXPRESSION		11									//�û�����

//��Ϸ��Ϣ
#define SUB_GF_GAME_STATUS			100									//��Ϸ״̬
#define SUB_GF_GAME_SCENE			101									//�Թ۳���
#define SUB_GF_LOOKON_STATUS		102									//�Թ�״̬

//ϵͳ��Ϣ
#define SUB_GF_SYSTEM_MESSAGE		200									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE		201									//������Ϣ

/////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
typedef struct _stCmdGfGameConfig
{
	BYTE							cbAllowLookon;						//�Թ۱�־
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwClientVersion;					//��Ϸ�汾
}ST_CMD_GF_GAME_CONFIG, *PST_CMD_GF_GAME_CONFIG;

//�Թ�����
typedef struct _stCmdGfLookOnConfig
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbAllowLookon;						//�����Թ�
}ST_CMD_GF_LOOK_ON_CONFIG, *PST_CMD_GF_LOOK_ON_CONFIG;

//�Թ�״̬
typedef struct _stCmdGfLookOnStatus
{
	BYTE							cbAllowLookon;						//�����Թ�
}ST_CMD_GF_LOOK_ON_STATUS, *PST_CMD_GF_LOOK_ON_STATUS;

//��Ϸ����
typedef struct _stCmdGfGameStatus
{
	BYTE							cbGameStatus;						//��Ϸ״̬
	BYTE							cbAllowLookon;						//�Թ۱�־
}ST_CMD_GF_GAME_STATUS, *PST_CMD_GF_GAME_STATUS;

//�û�����
typedef struct _stCmdGfUserClientChat
{
	DWORD                           dwUserID;							//�û�ID
    WORD                            wLen;
    DWORD                           dwChatColor;						//���챳��
    TCHAR                           szChatMsg[LEN_USER_CHAT_MSG];		//������Ϣ
}ST_CMD_GF_USER_CLIENT_CHAT, *PST_CMD_GF_USER_CLIENT_CHAT;

typedef struct _stCmdGfUserServerChat
{
    WORD                            wLen;
    DWORD                           dwChatColor;
    DWORD                           dwSendUserID;
	DWORD                           dwRecvUserID;
    TCHAR                           szChatMsg[LEN_USER_CHAT_MSG];
}ST_CMD_GF_USER_SERVER_CHAT, *PST_CMD_GF_USER_SERVER_CHAT;

//�û�����
typedef struct _stCmdGfUserClientExpression
{
    WORD                            wItemIndex;
    DWORD                           dwUserID;
}ST_CMD_GF_USER_CLIENT_EXPRESSION, *PST_CMD_GF_USER_CLIENT_EXPRESSION;

typedef struct _stCmdGfUserServerExpression
{
    WORD                            wItemIndex;
    DWORD                           dwSendUserID;
    DWORD                           dwUserID;
}ST_CMD_GF_USER_SERVER_EXPRESSION, *PST_CMD_GF_USER_SERVER_EXPRESSION;

/////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
#define MDM_GF_GAME					200									//��Ϸ����

//������Ϣ
#define DTP_GR_TABLE_PWD			1									//��������

//�û�����
#define DTP_GR_NICK_NAME			10									//�û��ǳ�
#define DTP_GR_GROUP_NAME			11									//��������
#define DTP_GR_UNDER_WRITE			12									//����ǩ��

//������Ϣ
#define DTP_GR_USER_NOTE			20									//�û���ע
#define DTP_GR_CUSTOM_FACE			21									//�Զ�ͷ��

/////////////////////////////////////////////////////////////////////////////////
//�������
#define REQ_FAILURE_NORMAL			0									//����ԭ��
#define REQ_FAILURE_NO_GOLD			1									//��Ҳ���
#define REQ_FAILURE_NO_GAME_COIN	2									//��Ϸ�Ҳ���
#define REQ_FAILURE_PWD				3									//�������
#define REQ_FAILURE_ACCOUNTS		7									//�ǳƴ���

#pragma pack()

#endif