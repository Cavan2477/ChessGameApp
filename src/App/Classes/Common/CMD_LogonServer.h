/************************************************************************************
 * file: 		CMD_LogonServer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:31:18
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __LOGON_SERVER_H__
#define __LOGON_SERVER_H__

#include "MacroDefine.h"
#include "Packet.h"
#include "Struct.h"

#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////
//��¼����
#define MDM_GP_LOGON                1

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID         1                       //I D ��¼
#define SUB_GP_LOGON_ACCOUNTS       2                       //�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS    3                       //ע���ʺ�
#define SUB_GP_LOGON_MANAGETOOL     4                       //������
#define SUB_GP_VERIFY_INDIVIDUAL	5                       //��֤����


//��¼���
#define SUB_GP_LOGON_SUCCESS		100                     //��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101                     //��¼ʧ��
#define SUB_GP_LOGON_FINISH			102                     //��¼���
#define SUB_GP_VALIDATE_MBCARD      103                     //��¼ʧ��
#define SUB_GP_VALIDATE_PASSPORT	104                     //��¼ʧ��
#define SUB_GP_VERIFY_RESULT		105                     //��֤���
#define SUB_GP_MATCH_SIGNUPINFO		106                     //������Ϣ
#define SUB_GP_GROWLEVEL_CONFIG		107                     //�ȼ�����
#define SUB_GP_UPDATE_NOTIFY		200                     //������ʾ
//////////////////////////////////////////////////////////////////////////////////////////

#define MB_VALIDATE_FLAGS           0x01                    //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                    //Ч��Ͱ汾
#define VERIFY_ACCOUNTS				0x01                    //У���ʺ�
#define VERIFY_NICKNAME				0x02                    //У���ǳ�

//ID��¼
typedef struct _stCmdGpLogonGameID
{
    //ϵͳ��Ϣ
    DWORD							dwPlazaVersion;             //�㳡�汾
    TCHAR							szMachineID[LEN_MACHINE_ID];//��������
   
    //��¼��Ϣ
	DWORD							dwGameID;                   //��Ϸ I D
    TCHAR							szPwd[LEN_MD5];				//��¼����
    BYTE							cbValidateFlags;            //У���ʶ
}ST_CMD_GP_LOGON_GAME_ID, *PST_CMD_GP_LOGON_GAME_ID;

//�ʺŵ�¼
typedef struct _stCmdGpLogonAccounts
{
    //ϵͳ��Ϣ
    DWORD							dwPlazaVersion;                 //�㳡�汾
    TCHAR							szMachineID[LEN_MACHINE_ID];    //��������
    
    //��¼��Ϣ
    BYTE							cbValidateFlags;                //У���ʶ
    TCHAR							szPwd[LEN_MD5];					//��¼����
    TCHAR							szAccount[LEN_MAX_ACCOUNT];			//��¼�ʺ�
    TCHAR							szIdentityID[LEN_IDENTITY_ID];	//���֤��
}CMD_GP_LOGON_ACCOUNTS, *PCMD_GP_LOGON_ACCOUNTS;

//ע���ʺ�
typedef struct _stCmdGpRegAccount
{
    //ϵͳ��Ϣ
	DWORD							dwPlazaVersion;                     //�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];        //��������

	//�������
	TCHAR							szLogonPwd[LEN_MD5];				//��¼����

	//ע����Ϣ
	WORD							wFaceID;                            //ͷ���ʶ
	BYTE							cbGender;                           //�û��Ա�
	TCHAR							szAccount[LEN_MAX_ACCOUNT];				//��¼�ʺ�
	TCHAR							szNickName[LEN_MAX_NICKNAME];           //�û��ǳ�
	TCHAR							szSpreader[LEN_MAX_ACCOUNT];			//�Ƽ��ʺ�
	TCHAR							szPassportID[LEN_IDENTITY_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];   //��ʵ����
    BYTE							cbValidateFlags;                    //У���ʶ
}ST_CMD_GP_REG_ACCOUNT, *PST_CMD_GP_REG_ACCOUNT;

//��֤����
typedef struct _stCmdGpVerifyIndividual
{
    //ϵͳ��Ϣ
    DWORD							dwPlazaVersion;                     //�㳡�汾
    
    //��֤��Ϣ
    WORD							wVerifyMask;                        //��֤����
}ST_CMD_GP_VERIFY_INDIVIDUAL, *PST_CMD_GP_VERIFY_INDIVIDUAL;

//��¼�ɹ�
typedef struct _stCmdGpLogonSucc
{
    //��������
    WORD							wFaceID;            			//ͷ���ʶ
    DWORD							dwUserID;           			//�û� I D
    DWORD							dwGameID;           			//��Ϸ I D
    DWORD							dwGroupID;          			//���ű�ʶ
    DWORD							dwCustomID;         			//�Զ���ʶ
    DWORD							dwExp;       					//������ֵ
    DWORD							dwLoveLiness;       			//�û�����
    
    //�û��ɼ�
	LONGLONG						lUserGameCoin;         			//�û���Ϸ��
	LONGLONG						lUserInsure;        			//�û�����
	LONGLONG						lUserGold;						//�û����
    DOUBLE							dUserBean;						//�û���Ϸ��
    
    //�û���Ϣ
    BYTE							cbGender;						//�û��Ա�
    BYTE							cbMoorMachine;					//��������
    TCHAR							szAccounts[LEN_MAX_ACCOUNT];		//��¼�ʺ�
    TCHAR							szNickName[LEN_MAX_ACCOUNT];		//�û��ǳ�
    TCHAR							szDynamicPwd[LEN_PWD];			//��̬����
    TCHAR							szGroupName[LEN_MAX_GROUP_NAME];	//��������
    
    //������Ϣ
	BYTE							cbInsureEnabled;				//����ʹ�ܱ�ʶ
	BYTE                            cbShowServerStatus;				//��ʾ������״̬
}ST_CMD_GP_LOGON_SUCC, *PST_CMD_GP_LOGON_SUCC;

//��¼ʧ��
typedef struct _stCmdGpLogonFailure
{
    INT								nErrCode;						//�������
    TCHAR							szDesc[256];
}ST_CMD_GP_LOGON_FAILURE, *PST_CMD_GP_LOGON_FAILURE;

//��½���
typedef struct _stCmdGpLogonFinish
{
	WORD							wIntermitTime;					//�ж�ʱ��
	WORD							wOnlineCountTime;				//����ʱ��
}ST_CMD_GP_LOGON_FINISH, *PST_CMD_GP_LOGON_FINISH;

//��֤����Ϣ
typedef struct _stCmdGpValidateMBCard
{
    UINT							uMBCardID;						// ��������
}ST_CMD_GP_VALIDATE_MB_CARD, *PST_CMD_GP_VALIDATE_MB_CARD;

//��֤���
typedef struct _stCmdGpVerifyIndividualResult
{
	bool							bVerifyPassage;					//��֤ͨ��
	WORD							wVerifyMask;					//��֤����
	TCHAR							szErrorMsg[128];				//������Ϣ
}ST_CMD_GP_VERIFY_INDIVIDUAL_RESULT, *PST_CMD_GP_VERIFY_INDIVIDUAL_RESULT;

//������ʾ
typedef struct _stCmdGpUpdateNotify
{
    BYTE							cbMustUpdate;					//ǿ������
    BYTE							cbAdviceUpdate;					//��������
    DWORD							dwCurrentVersion;				//��ǰ�汾
}ST_CMD_GP_UPDATE_NOTIFY, *PST_CMD_GP_UPDATE_NOTIFY;

//////////////////////////////////////////////////////////////////////////////////////////

//Я����Ϣ _stCMD_GP_LogonSuccess
#define DTP_GP_GROUP_INFO			1								//������Ϣ
#define DTP_GP_MEMBER_INFO			2								//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3								//����ǩ��
#define DTP_GP_STATION_URL			4								//��ҳ��Ϣ

//������Ϣ
typedef struct _stCmdGpGroupInfo
{
    DWORD							dwGroupID;						//��������
    TCHAR							szGroupName[LEN_MAX_GROUP_NAME];	//��������
}ST_DTP_GP_GROUP_INFO, *PST_DTP_GP_GROUP_INFO;

//��Ա��Ϣ
typedef struct _stCmdGpVipInfo
{
    BYTE							cbVipLevel;						//��Ա�ȼ�
    ST_SYS_TIME						stSysTimeVipEndDate;			//����ʱ��
}ST_DTP_GP_VIP_INFO, *PST_DTP_GP_VIP_INFO;

//�б�����
#define MDM_GP_SERVER_LIST			2								//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1								//��ȡ�б�
#define SUB_GP_GET_SERVER			2								//��ȡ����
#define SUB_GP_GET_MATCH			3								//��ȡ����
#define SUB_GP_GET_ONLINE			4								//��ȡ����
#define SUB_GP_GET_COLLECTION		5								//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100								//�����б�
#define SUB_GP_LIST_KIND			101								//�����б�
#define SUB_GP_LIST_NODE			102								//�ڵ��б�
#define SUB_GP_LIST_PAGE			103								//�����б�
#define SUB_GP_LIST_SERVER			104								//�����б�
#define SUB_GP_LIST_MATCH			105								//�����б�
#define SUB_GP_VIDEO_OPTION			106								//��Ƶ����

//�����Ϣ
#define SUB_GP_LIST_FINISH			200								//�������
#define SUB_GP_SERVER_FINISH		201								//�������
#define SUB_GP_MATCH_FINISH			202								//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300								//��������
#define SUB_GR_SERVER_ONLINE		301								//��������

/////////////////////////////////////////////////////////////////////////////////////////////////

//��ȡ����
typedef struct _stCmdGpGetOnline
{
    WORD							wGameRoomCount;					//������Ŀ
    WORD							wOnLineGameRoomID[MAX_GAME_ROOM];//�����ʶ
}ST_CMD_GP_GET_ONLINE, *PST_CMD_GP_GET_ONLINE;

//��������
typedef struct _stCmdGpTypeOnline
{
    WORD							wTypeCount;							//������Ŀ
    _stOnlineInfoType				stOnlineInfoGameType[MAX_GAME_TYPE];//��������
}ST_CMD_GP_TYPE_ONLINE, *PST_CMD_GP_TYPE_ONLINE;

//��������
typedef struct _stCmdGpServerOnline
{
    WORD							wGameRoomCount;						//������Ŀ
    _stOnlineInfoGameRoom			stOnLineInfoGameRoom[MAX_GAME_ROOM];//��������
}ST_CMD_GP_SERVER_ONLINE, *PST_CMD_GP_SERVER_ONLINE;

////////////////////////////////////////////////////////////////////////////////////////////////
//��������
#define MDM_GP_USER_SERVICE					3							//�û�����

//�ʺŷ���
#define SUB_GP_MODIFY_MACHINE				100							//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS			101							//�޸�����
#define SUB_GP_MODIFY_BANK_PWD			102							//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE			103							//�޸�ǩ��

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO				120							//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO				122							//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO				123							//�Զ�ͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL				140							//��������
#define	SUB_GP_QUERY_INDIVIDUAL				141							//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL			152							//�޸�����

//���з���
#define SUB_GP_USER_ENABLE_INSURE			160							//��ͨ����
#define SUB_GP_USER_SAVE_SCORE				161							//������
#define SUB_GP_USER_TAKE_SCORE				162							//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE			163							//ת�ʲ���
#define SUB_GP_REQ_BANK_QUERY_BANK				164							//��������
#define SUB_GP_QUERY_INSURE_INFO			165							//��ѯ����
#define SUB_GP_REQ_BANK_ENABLE_SUCC			166							//���гɹ�
#define SUB_GP_REQ_BANK_ENABLE_FAILURE			167							//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST		168							//��ѯ�û�
#define SUB_GP_REQ_BANK_QUERY_USER_INFO		169							//�û���Ϣ
#define SUB_GP_REQ_BANK_ENABLE	170							//��ͨ���

//��������
#define SUB_GP_MATCH_SIGNUP					200							//��������
#define SUB_GP_MATCH_UNSIGNUP				201							//ȡ������
#define SUB_GP_MATCH_SIGNUP_RESULT			202							//�������

//ǩ������
#define SUB_GP_CHECKIN_QUERY				220							//��ѯǩ��
#define SUB_GP_CHECKIN_INFO					221							//ǩ����Ϣ
#define SUB_GP_CHECKIN_DONE					222							//ִ��ǩ��
#define SUB_GP_CHECKIN_RESULT				223							//ǩ�����

//�������
#define SUB_GP_TASK_LOAD					240							//��������
#define SUB_GP_TASK_TAKE					241							//��ȡ����
#define SUB_GP_TASK_REWARD					242							//������
#define SUB_GP_TASK_INFO					243							//������Ϣ
#define SUB_GP_TASK_LIST					244							//������Ϣ
#define SUB_GP_TASK_RESULT					245							//������

//�ͱ�����
#define SUB_GP_BASEENSURE_LOAD				260							//���صͱ�
#define SUB_GP_BASEENSURE_TAKE				261							//��ȡ�ͱ�
#define SUB_GP_BASEENSURE_PARAMETER			262							//�ͱ�����
#define SUB_GP_BASEENSURE_RESULT			263							//�ͱ����

//�ƹ����
#define SUB_GP_SPREAD_QUERY					280							//�ƹ㽱��
#define SUB_GP_SPREAD_INFO					281							//��������

//�ȼ�����
#define SUB_GP_GROWLEVEL_QUERY				300							//��ѯ�ȼ�
#define SUB_GP_GROWLEVEL_PARAMETER			301							//�ȼ�����
#define SUB_GP_GROWLEVEL_UPGRADE			302							//�ȼ�����

//�һ�����
#define SUB_GP_EXCHANGE_QUERY				320							//�һ�����
#define SUB_GP_EXCHANGE_PARAMETER			321							//�һ�����
#define SUB_GP_PURCHASE_MEMBER				322							//�����Ա
#define SUB_GP_PURCHASE_RESULT				323							//������
#define SUB_GP_EXCHANGE_GAME_COIN_BY_GOLD	324                         //��Ҷһ���Ϸ��
#define SUB_GP_EXCHANGE_GAME_COIN_BY_BEAN	325                         //��Ϸ���һ���Ϸ��

#define SUB_GP_EXCHANGE_RESULT				326							//�һ����

//�齱����
#define SUB_GP_LOTTERY_CONFIG_REQ			340							//��������
#define SUB_GP_LOTTERY_CONFIG				341							//�齱����
#define SUB_GP_LOTTERY_USER_INFO			342							//�齱��Ϣ
#define SUB_GP_LOTTERY_START				343							//��ʼ�齱
#define SUB_GP_LOTTERY_RESULT				344							//�齱���

//��Ϸ����
#define SUB_GP_QUERY_USER_GAME_DATA			360							//��ѯ����


//�������
#define SUB_GP_OPERATE_SUCCESS				500							//�����ɹ�
#define SUB_GP_OPERATE_FAILURE				501							//����ʧ��

////////////////////////////////////////////////////////////////////////////////////////////////

//�޸�����
typedef struct _stCmdGpModifyLogonPwd
{
    DWORD							dwUserID;							//�û� I D
    TCHAR							szNewPwd[LEN_PWD];					//������
    TCHAR							szOldPwd[LEN_PWD];					//ԭ����
}ST_CMD_GP_MODIFY_LOGON_PWD, *PST_CMD_GP_MODIFY_LOGON_PWD;

//�޸�����
typedef struct _stCmdGpModifyBankPwd
{
    DWORD							dwUserID;							//�û� I D
	TCHAR							szNewPwd[LEN_PWD];					//������
	TCHAR							szOldPwd[LEN_PWD];					//ԭ����
}ST_CMD_GP_MODIFY_BANK_PWD, *PST_CMD_GP_MODIFY_BANK_PWD;

//�޸�ǩ��
typedef struct _stCmdGpModifySignature
{
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPwd[LEN_PWD];						//�û�����
    TCHAR							szSignature[LEN_SIGNATURE];			//����ǩ��
}ST_CMD_GP_MODIFY_SIGNATURE, *PST_CMD_GP_MODIFY_SIGNATURE;

///////////////////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
typedef struct _stCmdGpUserFaceInfo
{
    WORD							wFaceID;							//ͷ���ʶ
    DWORD							dwCustomID;							//�Զ���ʶ
}ST_CMD_GP_USER_FACE_INFO, *PST_CMD_GP_USER_FACE_INFO;

//�޸�ͷ��
typedef struct _stCmdGpSysFaceInfo
{
    WORD							wFaceID;							//ͷ���ʶ
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPwd[LEN_PWD];						//�û�����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_SYS_FACE_INFO, *PST_CMD_GP_SYS_FACE_INFO;

//�޸�ͷ��
typedef struct _stCmdGpCustomFaceInfo
{
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPwd[LEN_PWD];						//�û�����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
}ST_CMD_GP_CUSTOM_FACE_INFO, *PST_CMD_GP_CUSTOM_FACE_INFO;

///////////////////////////////////////////////////////////////////////////////////////////////
//�󶨻���
typedef struct _stCmdGpModifyMachine
{
    BYTE							cbBind;								//�󶨱�־
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szPwd[LEN_PWD];						//�û�����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_MODIFY_MACHINE, *PST_CMD_GP_MODIFY_MACHINE;

///////////////////////////////////////////////////////////////////////////////////////////////
//��������
typedef struct _stCmdGpUserIndividual
{
    DWORD							dwUserID;							//�û� I D
}ST_CMD_GP_USER_INDIVIDUAL, *PST_CMD_GP_USER_INDIVIDUAL;

//��ѯ��Ϣ
typedef struct _stCmdGpQueryIndividual
{
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPwd[LEN_PWD];						//�û�����
}ST_CMD_GP_QUERY_INDIVIDUAL, *PST_CMD_GP_QUERY_INDIVIDUAL;

//�޸�����
typedef struct _stCmdGpModifyIndividual
{
    BYTE							cbGender;							//�û��Ա�
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPwd[LEN_PWD];						//�û�����
}ST_CMD_GP_MODIFY_INDIVIDUAL, *PST_CMD_GP_MODIFY_INDIVIDUAL;

///////////////////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual
#define DTP_GP_UI_ACCOUNTS			1									//�û��ʺ�
#define DTP_GP_UI_NICKNAME			2									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			3									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		4									//����ǩ��
#define DTP_GP_UI_QQ				5									//Q Q ����
#define DTP_GP_UI_EMAIL				6									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		7									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		8									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		9									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	10									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////////////////
//��������
typedef struct _stCmdGpUserBankInfo
{
    BYTE							cbEnjoinTransfer;					//ת�ʿ���
    WORD							wRevenueTake;						//˰�ձ���
    WORD							wRevenueTransfer;					//˰�ձ���
    WORD							wRevenueTransferVip;				//˰�ձ���
    WORD							wGameRoomID;						//�����ʶ
	LONGLONG						llUserGold;							//�û����
	LONGLONG						llUserBankGold;						//���н��
	LONGLONG						llTransferCondition;				//ת������
}ST_CMD_GP_USER_BANK_INFO, *PST_CMD_GP_USER_BANK_INFO;

//��ͨ����
typedef struct _stCmdGpUserBankEnable
{
    DWORD							dwUserID;							//�û�I D
    TCHAR							szLogonPwd[LEN_PWD];				//��¼����
    TCHAR							szBankPwd[LEN_PWD];					//��������
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_USER_BANK_ENABLE, *PST_CMD_GP_USER_BANK_ENABLE;

//������
typedef struct _stCmdGpUserGoldSave
{
    DWORD							dwUserID;							//�û� I D
    LONGLONG						llSaveGold;							//������
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_USER_GOLD_SAVE, *PST_CMD_GP_USER_GOLD_SAVE;

//��ȡ���
typedef struct _stCmdGpUserTakeOutGold
{
    DWORD							dwUserID;							//�û� I D
    LONGLONG						llTakeOutGold;						//��ȡ���
    TCHAR							szBankPwd[LEN_MD5];					//��������
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_USER_GOLD_TAKE_OUT, *PST_CMD_GP_USER_GOLD_TAKE_OUT;

//ת�ʽ��
typedef struct _stCmdGpUserGoldTransfer
{
    DWORD							dwUserID;							//�û� I D
    LONGLONG						lTransferGold;						//ת�ʽ��
    TCHAR							szInsurePwd[LEN_MD5];				//��������
    TCHAR							szAccounts[LEN_MAX_NICKNAME];			//Ŀ���û�
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע
}ST_CMD_GP_USER_GOLD_TRANSFER, *PST_CMD_GP_USER_GOLD_TRANSFER;

//���гɹ�
typedef struct _stCmdGpUserBankSucc
{
    BYTE                            cbOperateType;                      //��������
    DWORD							dwUserID;							//�û� I D
    DWORD                           dwUserMedal;                        //�û�����
    DWORD                           dwRecordIndex;                      //��¼���
	LONGLONG						llUserGold;							//�û����
	LONGLONG						llUserBankGold;						//���н��
    TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GP_USER_BANK_SUCC, *PST_CMD_GP_USER_BANK_SUCC;

//����ʧ��
typedef struct _stCmdGpUserBankFailure
{
    INT								nErrorCode;							//�������
    TCHAR							szDes[LEN_DES];						//������Ϣ
}ST_CMD_GP_USER_BANK_FAILURE, *PST_CMD_GP_USER_BANK_FAILURE;

//��ȡ���
typedef struct _stCmdGpUserGoldTakeOutResult
{
    DWORD							dwUserID;							//�û� I D
	LONGLONG						llUserGold;							//�û����
	LONGLONG						llUserBankGold;						//���н��
}ST_CMD_GP_USER_GOLD_TAKE_OUT_RESULT, *PST_CMD_GP_USER_GOLD_TAKE_OUT_RESULT;

//��ѯ����
typedef struct _stCmdGpUserBankQueryInfo
{
    DWORD							dwUserID;							//�û� I D
    TCHAR							szPwd[LEN_MD5];						//��������
}ST_CMD_GP_USER_BANK_QUERY_INFO, *PST_CMD_GP_USER_BANK_QUERY_INFO;

//��ѯ�û�
typedef struct _stCmdGpUserInfoQueryReq
{
    BYTE                            cbByNickName;                       //�ǳ�����
    TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//Ŀ���û�
}ST_CMD_GP_USER_INFO_QUERY_REQ, *PST_CMD_GP_USER_INFO_QUERY_REQ;

//�û�ת����Ϣ
typedef struct _stCmdGpUserTransferUserInfo
{
    DWORD							dwTargetGameID;						//Ŀ���û�
    TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//Ŀ���˻�
}ST_CMD_GP_USER_TRANSFER_USER_INFO, *PST_CMD_GP_USER_TRANSFER_USER_INFO;

//��ͨ���
typedef struct _stCmdGpUserInsureEnableResult
{
    BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
    TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GP_USER_INSURE_ENABLE_RESULT, *PST_CMD_GP_USER_INSURE_ENABLE_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////////
//��ѯǩ��
typedef struct _stCmdGpCheckInQueryInfo
{
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szLogonPwd[LEN_PWD];				//��¼����
}ST_CMD_GP_CHECK_IN_QUERY_INFO, *PST_CMD_GP_CHECK_IN_QUERY_INFO;

//ǩ����Ϣ
typedef struct _stCmdGpCheckInInfo
{
    WORD							wSeriesDate;						//��������
    bool							bTodayChecked;						//ǩ����ʶ
    LONGLONG						lRewardGold[LEN_WEEK];				//�������
}ST_CMD_GP_CHECK_IN_INFO, *PST_CMD_GP_CHECK_IN_INFO;

//ִ��ǩ��
typedef struct _stCmdGpCheckInDone
{
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szPwd[LEN_PWD];						//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_CHECK_IN_DONE, *PST_CMD_GP_CHECK_IN_DONE;

//ǩ�����
typedef struct _stCmdGpCheckInResult
{
    bool							bSucc;								//�ɹ���ʶ
    LONGLONG						lCurrGold;							//��ǰ���
    TCHAR							szNotifyContent[128];				//��ʾ����
}ST_CMD_GP_CHECK_IN_RESULT, *PST_CMD_GP_CHECK_IN_RESULT;

//////////////////////////////////////////////////////////////////////////////////
//�������

//��������
typedef struct _stCmdGpTaskLoadInfo
{
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szPwd[LEN_PWD];						//�û�����
}ST_CMD_GP_TASK_LOAD_INFO, *PST_CMD_GP_TASK_LOAD_INFO;

//��ȡ����
typedef struct _stCmdGpTaskGet
{
    WORD							wTaskID;							//�����ʶ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szPwd[LEN_PWD];						//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_TASK_GET, *PST_CMD_GP_TASK_GET;

//��ȡ����
typedef struct _stCmdGpTaskReward
{
    WORD							wTaskID;							//�����ʶ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szPwd[LEN_PWD];						//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_TASK_REWARD, *PST_CMD_GP_TASK_REWARD;

//������Ϣ
typedef struct _stCmdGpTaskInfo
{
    WORD							wTaskCount;							//��������
    ST_TASK_STATUS					stTaskStatusArray[TASK_MAX_COUNT];	//����״̬
}ST_CMD_GP_TASK_INFO, *PST_CMD_GP_TASK_INFO;

//������
typedef struct _stCmdGpTaskResult
{
    //�����Ϣ
    bool							bSucc;								//�ɹ���ʶ
    WORD							wCommandID;							//�����ʶ
    
    //�Ƹ���Ϣ
	LONGLONG						lCurrGameCoin;						//��ǰ��Ϸ��
	LONGLONG						lCurrGold;							//��ǰ���
    
    //��ʾ��Ϣ
    TCHAR							szNotifyContent[128];				//��ʾ����
}ST_CMD_GP_TASK_RESULT, *PST_CMD_GP_TASK_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////
//�ͱ�����

//��ȡ�ͱ�
typedef struct _stCmdGpBaseEnsureGet
{
    DWORD							dwUserID;							//�û� I D
    TCHAR							szLogonPwd[LEN_PWD];				//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_BASE_ENSURE_GET, *PST_CMD_GP_BASE_ENSURE_GET;

//�ͱ�����
typedef struct _stCmdGpBaseEnsureParam
{
    LONGLONG						lGameCoinCondition;					//��Ϸ������
    LONGLONG						lGameCoinAmount;					//��Ϸ������
    BYTE							cbGetTimes;							//��ȡ����
}ST_CMD_GP_BASE_ENSURE_PARAM, *PST_CMD_GP_BASE_ENSURE_PARAM;

//�ͱ����
typedef struct _stCmdGpBaseEnsureResult
{
    bool							bSucc;								//�ɹ���ʶ
    LONGLONG						lCurrGameCoin;						//��ǰ��Ϸ��
    TCHAR							szNotifyContent[128];				//��ʾ����
}ST_CMD_GP_BASE_ENSURE_RESULT, *PST_CMD_GP_BASE_ENSURE_RESULT;

///////////////////////////////////////////////////////////////////////////////////////////
//�ƹ����

//�ƹ��ѯ
typedef struct _stCmdGpUserSpreadQuery
{
    DWORD							dwUserID;							//�û���ʶ
}ST_CMD_GP_USER_SPREAD_QUERY, *PST_CMD_GP_USER_SPREAD_QUERY;

//�ƹ����
typedef struct _stCmdGpUserSpreadParam
{
    DWORD							dwSpreadCount;						//�ƹ�����
    LONGLONG						lSpreadReward;						//�ƹ㽱��
}ST_CMD_GP_USER_SPREAD_PARAM, *PST_CMD_GP_USER_SPREAD_PARAM;

///////////////////////////////////////////////////////////////////////////////////////////
//�ȼ�����

//��ѯ�ȼ�
typedef struct _stCmdGpGrowLevelQueryInfo
{
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szPwd[LEN_PWD];						//�û�����
    
    //������Ϣ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_GROW_LEVEL_QUERY_INFO, *PST_CMD_GP_GROW_LEVEL_QUERY_INFO;

//�ȼ�����
typedef struct _stCmdGpGrowLevelConfig
{
    WORD							wLevelCount;						//�ȼ���Ŀ
    _stGrowLevelConfig				GrowLevelItem[60];					//�ȼ�����
}ST_CMD_GP_GROW_LEVEL_CONFIG, *PST_CMD_GP_GROW_LEVEL_CONFIG;

//�ȼ�����
typedef struct _stCmdGpGrowLevelParam
{
    WORD							wCurrLevelID;						//��ǰ�ȼ�
    DWORD							dwCurrExp;							//��ǰ����
    DWORD							dwNextLevelExp;						//�¼�����
    LONGLONG						lUpgradeRewardGameCoin;				//����������Ϸ��
    LONGLONG						lUpgradeRewardGold;					//�����������
}ST_CMD_GP_GROW_LEVEL_PARAM, *PST_CMD_GP_GROW_LEVEL_PARAM;

//�ȼ�����
typedef struct _stCmdGpGrowLevelUpgrade
{
    LONGLONG						lCurrGameCoin;						//��ǰ��Ϸ��
    LONGLONG						lCurrGold;							//��ǰ���
    TCHAR							szNotifyContent[128];				//��ʾ����
}ST_CMD_GP_GROW_LEVEL_UPGRADE, *PST_CMD_GP_GROW_LEVEL_UPGRADE;

/////////////////////////////////////////////////////////////////////////////////////////////
//�һ�����

//��ѯ����
typedef struct _stCmdGpExchangeParam
{
    DWORD							wGoldExchangeGameCoinRate;			//��ҡ���Ϸ�Ҷһ�����
    DWORD							wPresentExchangeGameCoinRate;		//��������Ϸ�Ҷһ���
    DWORD							wRateBeanExchangeGameCoin;			//��Ϸ������Ϸ�Ҷһ���
    WORD							wVipCount;							//��Ա��Ŀ
    _stVipParam						stVipParamArray[10];				//��Ա����
}ST_CMD_GP_EXCHANGE_PARAM, *PST_CMD_GP_EXCHANGE_PARAM;

//��Ҷһ���Ϸ��
typedef struct _stCmdGpExchangeGameCoinByGold
{
    DWORD							dwUserID;							//�û���ʶ
    LONGLONG						lExchangedGold;						//���һ����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
}ST_CMD_GP_EXCHANGE_GAME_COIN_BY_GOLD, *PST_CMD_GP_EXCHANGE_GAME_COIN_BY_GOLD;

//��Ϸ���һ���Ϸ��
typedef struct _stCmdGpExchangeGameCoinByBean
{
    DWORD							dwUserID;							//�û���ʶ
    double							dExchangedBean;						//���һ���Ϸ��
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
}ST_CMD_GP_EXCHANGE_GAME_COIN_BY_BEAN, *PST_CMD_GP_EXCHANGE_GAME_COIN_BY_BEAN;

//�һ����
typedef struct _stCmdGpExchangeGameCoinResult
{
    bool							bSucc;								//�ɹ���ʶ
	LONGLONG						lCurrGameCoin;						//��ǰ��Ϸ��
	LONGLONG						lCurrGold;							//��ǰ���
    double                          dCurrBean;							//��ǰ��Ϸ��
    TCHAR							szNotifyContent[128];				//��ʾ����
}ST_CMD_GP_EXCHANGE_GAME_COIN_RESULT, *PST_CMD_GP_EXCHANGE_GAME_COIN_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////
//�齱����

//��������
typedef struct _stCmdGpLotteryConfigRequest
{
    WORD							wKindID;							//��Ϸ��ʶ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szLogonPwd[LEN_MD5];				//��¼����
}ST_CMD_GP_LOTTERY_CONFIG_REQUEST, *PST_CMD_GP_LOTTERY_CONFIG_REQUEST;

//�齱����
typedef struct _stCmdGpLotteryConfig
{
    WORD							wLotteryCount;						//�������
    _stLotteryItem					stLotteryItemArray[MAX_LOTTERY];	//��������
}ST_CMD_GP_LOTTERY_CONFIG, *PST_CMD_GP_LOTTERY_CONFIG;

//�齱��Ϣ
typedef struct _stCmdGpLotteryUserInfo
{
    BYTE							cbFreeCount;						//��Ѵ���
    BYTE							cbAlreadyCount;						//�������
    WORD							wKindID;							//��Ϸ��ʶ
    DWORD							dwUserID;							//�û���ʶ
    LONGLONG						lChargeFee;							//�齱����
}ST_CMD_GP_LOTTERY_USER_INFO, *PST_CMD_GP_LOTTERY_USER_INFO;

//��ʼ�齱
typedef struct _stCmdGpLotteryStart
{
    WORD							wKindID;							//��Ϸ��ʶ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szLogonPwd[LEN_MD5];				//��¼����
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_LOTTERY_START, *PST_CMD_GP_LOTTERY_START;

//�齱���
typedef struct _stCmdGpLotteryResult
{
    bool							bWined;								//�н���ʶ
    WORD							wKindID;							//��Ϸ��ʶ
    DWORD							dwUserID;							//�û���ʶ
    LONGLONG						lUserScore;							//�û�����
    DOUBLE							dUserBeans;							//�û���Ϸ��
    _stLotteryItem					LotteryItem;						//�н�����
}ST_CMD_GP_LOTTERY_RESULT, *PST_CMD_GP_LOTTERY_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
//#pragma once

typedef struct _stCmdGpQueryUserGameData
{
    WORD							wKindID;							//��Ϸ��ʶ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szDynamicPwd[LEN_MD5];				//�û�����
}ST_CMD_GP_QUERY_USER_GAEM_DATA, *PST_CMD_GP_QUERY_USER_GAEM_DATA;

//������Ϣ
#define DTP_GP_UI_USER_GAME_DATA	1									//��Ϸ����
////////////////////////////////////////////////////////////////////////////////////////////////

//��������
typedef struct _stCmdGpMatchSignUp
{
    //������Ϣ
    WORD							wGameRoomID;						//�����ʶ
    DWORD							dwMatchID;							//������ʶ
    DWORD							dwMatchNO;							//��������
    
    //�û���Ϣ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szLogonPwd[LEN_MD5];				//��¼����
    
    //������Ϣ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_MATCH_SIGN_UP, *PST_CMD_GP_MATCH_SIGN_UP;

//ȡ������
typedef struct _stCmdGpMatchUnSignUp
{
    //������Ϣ
    WORD							wGameRoomID;						//�����ʶ
    DWORD							dwMatchID;							//������ʶ
    DWORD							dwMatchNO;							//��������
    //�û���Ϣ
    DWORD							dwUserID;							//�û���ʶ
    TCHAR							szLogonPwd[LEN_MD5];				//��¼����
    
    //������Ϣ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
}ST_CMD_GP_MATCH_UN_SIGN_UP, *PST_CMD_GP_MATCH_UN_SIGN_UP;

//�������
typedef struct _stCmdGpMatchSignUpResult
{
    bool							bSignup;							//������ʶ
    bool							bSucc;								//�ɹ���ʶ
    TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GP_MATCH_SIGN_UP_RESULT, *PST_CMD_GP_MATCH_SIGN_UP_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////
//����ʧ��
typedef struct _stCmdGpOperateFailure
{
    INT								nErrorCode;							//�������
    TCHAR							szDes[128];							//������Ϣ
}ST_CMD_GP_OPERATE_FAILURE, *PST_CMD_GP_OPERATE_FAILURE;

//�����ɹ�
typedef struct _stCmdGpOperateSucc
{
    INT								nErrorCode;							//��������
    TCHAR							szDes[128];							//�ɹ���Ϣ
}ST_CMD_GP_OPERATE_SUCC, *PST_CMD_GP_OPERATE_SUCC;

//////////////////////////////////////////////////////////////////////////////////////////////////
//Զ�̷���
#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//Э������
typedef struct _stCmdGpSearchCorrespondClient
{
    DWORD							dwGameID;							//��Ϸ��ʶ
    TCHAR							szNickName[LEN_MAX_NICKNAME];			//�û��ǳ�
}ST_CMD_GP_SEARCH_CORRESPOND_CLIENT, *PST_CMD_GP_SEARCH_CORRESPOND_CLIENT;

//Э������
typedef struct _stCmdGpSearchCorrespondServer
{
    WORD							wUserCount;							//�û���Ŀ
    _stUserRemoteInfo				stUserRemoteInfoArray[16];			//�û���Ϣ
}ST_CMD_GP_SEARCH_CORRESPOND_SERVER, *PST_CMD_GP_SEARCH_CORRESPOND_SERVER;


//////////////////////////////////////////////////////////////////////////////////
//��½����
#define MDM_MB_LOGON				100									//�㳡��½

//��½ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D��½
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�½
#define SUB_MB_REGISITER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_LOGON_OTHERPLATFORM	4									//������½
#define SUB_MB_LOGON_VISITOR        5									//�ο͵�¼
#define SUB_MB_ONLINE_CHECK         6									//���߼��

//��½���
#define SUB_MB_LOGON_SUCCESS		100									//��½�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��½ʧ��

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//�ο͵�¼
typedef struct _stCmdMbLogonVisitor
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE							cbDeviceType;						//�豸����
    
    //������Ϣ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
}ST_CMD_MB_LOGON_VISITOR, *PST_CMD_MB_LOGON_VISITOR;

//�ο���֤
typedef struct _stCmdMbOnlineCheck
{
    DWORD							dwUserID;							//�û�ID
}ST_CMD_MB_ONLINE_CHECK, *PST_CMD_MB_ONLINE_CHECK;

//�ʺŵ�½
typedef struct _stCmdMbLogonAccounts
{
    //ϵͳ��Ϣ
    WORD							wModuleID;							//ģ���ʶ
    DWORD							dwPlazaVersion;						//�㳡�汾
    BYTE							cbDeviceType;						//�豸����
    
    //��½��Ϣ
    TCHAR							szLogonPwd[LEN_MD5];				//��½����
    TCHAR							szLogonAccounts[LEN_MAX_ACCOUNT];		//��½�ʺ�
    
    //������Ϣ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
}ST_CMD_MB_LOGON_ACCOUNTS, *PST_CMD_MB_LOGON_ACCOUNTS;

//ע���ʺ�
typedef struct _stCmdMbRegAccounts
{
    //ϵͳ��Ϣ
    WORD							wModuleID;							//ģ���ʶ
    DWORD							dwPlazaVersion;						//�㳡�汾
    BYTE							cbDeviceType;                       //�豸����
    
    //�������
    TCHAR							szLogonPwd[LEN_MD5];				//��¼����
    
    //ע����Ϣ
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//ע���ʺ�
    TCHAR							szNickName[LEN_MAX_NICKNAME];			//�û��ǳ�
    
    //������Ϣ
    TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
}ST_CMD_MB_REG_ACCOUNTS, *PST_CMD_MB_REG_ACCOUNTS;

//����ƽ̨��½
typedef struct _stCmdMbLogonOtherPlatform
{
    //ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE							cbDeviceType;                       //�豸����

	//ע����Ϣ
	BYTE							cbGender;                           //�û��Ա�
	BYTE							cbPlatformID;                       //ƽ̨���
	TCHAR							szUserUin[LEN_USER_UIN];            //��¼�ʺ�
	TCHAR							szNickName[LEN_MAX_NICKNAME];			//�û��ǳ�
	TCHAR							zsCompellation[LEN_COMPELLATION];   //��ʵ����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
}ST_CMD_MB_LOGON_OTHER_PLATFORM, *PST_CMD_MB_LOGON_OTHER_PLATFORM;

//��½�ɹ�
typedef struct _stCmdMbLogonSucc
{
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    BYTE							cbMoorMachine;                      //��������
    DWORD							cbCustomFaceID;                     //�Զ���ͷ��
    DWORD							dwUserID;							//�û�ID
    DWORD							dwGameID;							//��ϷID
    DWORD							dwExp;								//������ֵ
    DWORD							dwLoveLiness;						//�û�����
    TCHAR							szAccounts[LEN_MAX_ACCOUNT];			//�û��ʺ�
    TCHAR							szNickName[LEN_MAX_ACCOUNT];			//�û��ǳ�
    TCHAR							szDynamicPwd[LEN_PWD];				//��̬����
    
    //�û��ɼ�
    LONGLONG						lUserGameCoin;                      //�û���Ϸ��
    LONGLONG						lUserGold;							//�û����
    LONGLONG						lUserInsure;                        //�û�����
    DOUBLE							dUserBean;							//�û���Ϸ��
    
    //��չ��Ϣ
    BYTE							cbInsureEnable;                     //ʹ�ܱ�ʾ
}ST_CMD_MB_LOGON_SUCC, *PST_CMD_MB_LOGON_SUCC;

//��½ʧ��
typedef struct _stCmdMbLogonFailure
{
	INT								lErrorCode;							//�������
	TCHAR							szDes[128];							//������Ϣ
}ST_CMD_MB_LOGON_FAILURE, *PST_CMD_MB_LOGON_FAILURE;

//������ʾ
typedef struct _stCmdMbUpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//����汾
	DWORD							dwCurrentVersion;					//��ǰ�汾
}ST_CMD_MB_UPDATE_NOTIFY, *PST_CMD_MB_UPDATE_NOTIFY;

/////////////////////////////////////////////////////////////////////
//�б�����
#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_MATCH_SERVER         102									//�����б�
#define SUB_GP_LIST_MATCH			105									//�����б�

#define SUB_MB_LIST_FINISH			200									//�б����

#define SUB_MB_ONLINE_GET			300									//��ȡ����
#define SUB_MB_ONLINE_GAME_TYPE		301									//��������
#define SUB_MB_ONLINE_GAME_ROOM		302									//��������

//��ȡ����
typedef struct _stCmdMbOnlineGet
{
    WORD							wCount;								//������Ŀ
    WORD							wOnLineGameRoomID[MAX_GAME_ROOM];	//�����ʶ
}ST_CMD_MB_ONLINE_GET, *PST_CMD_MB_ONLINE_GET;

//��������
typedef struct _stCmdMbOnlineGameType
{
    WORD							wCount;								//������Ŀ
    _stOnlineInfoType				OnLineInfoGameType[MAX_GAME_TYPE];	//��������
}ST_CMD_MB_ONLINE_GAME_TYPE, *PST_CMD_MB_ONLINE_GAME_TYPE;

//��������
typedef struct _stCmdMbOnlineGameRoom
{
    WORD							wCount;								//������Ŀ
    _stOnlineInfoGameRoom			OnLineInfoGameRoom[MAX_GAME_ROOM];	//��������
}ST_CMD_MB_ONLINE_GAME_ROOM, *PST_CMD_MB_ONLINE_GAME_ROOM;

#pragma pack()

#endif