/************************************************************************************
 * file: 		Struct.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:23:00
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef	__STRUCT_H__
#define	__STRUCT_H__

#include "MacroDefine.h"
#include "cocos2d.h"

#pragma pack(1)

/////////////////////////////////////////////////////////////////////
//��Ϸ�б�

//��Ϸ����
typedef struct _stGameType
{
	WORD				wJoinID;					//�ҽ�����
	WORD				wSortID;					//��������
	WORD				wTypeID;					//��������
	TCHAR				szTypeName[LEN_TYPE];		//��������
}ST_GAME_TYPE, *PST_GAME_TYPE;

//��Ϸ����
typedef struct _stGameKind
{
	WORD				wTypeID;					//��������
	WORD				wJoinID;					//�ҽ�����
	WORD				wSortID;					//��������
	WORD				wKindID;					//��������
	WORD				wGameID;					//ģ������ 
	DWORD				dwOnlineCount;				//��������
    DWORD				dwAndroidCount;				//��������
	DWORD				dwFullCount;				//��Ա����
	TCHAR				szKindName[LEN_KIND];		//��������
	TCHAR				szProcessName;				//��������
}ST_GAME_KIND, *PST_GAME_KIND;

//��Ϸ�ڵ�
typedef struct _stGameNode
{
	WORD				wKindID;					//��������
	WORD				wJoinID;					//�ҽ�����
	WORD				wSortID;					//��������
	WORD				wNodeID;					//�ڵ�����
	TCHAR				szNodeName;					//�ڵ�����
}ST_GAME_NODE, *PST_GAME_NODE;

//��������
typedef struct _stGamePage
{
	WORD				wPageID;					//ҳ������
	WORD				wKindID;					//��������
	WORD				wNodeID;					//�ڵ�����
	WORD				wSortID;					//��������
	WORD				wOperateType;				//��������
	TCHAR				szDisPlayeName[LEN_PAGE];	//��ʾ����
}ST_GAME_PAGE, *PST_GAME_PAGE;

//��Ϸ���������
typedef struct _stGameRoomServer
{
	WORD				wKindID;					//��������
	WORD				wNodeID;					//�ڵ�����
	WORD				wSortID;					//��������
	WORD				wServerID;					//��������
    WORD                wServerKind;                //��������
    WORD                wServerType;                //��������
    WORD                wServerLevel;               //����ȼ�
	WORD				wServerPort;				//����˿�
    LONGLONG            llCellScore;                //��Ԫ����
    LONGLONG            llEnterScore;               //�������
    DWORD				dwServerRule;               //�������
	DWORD				dwOnlineCount;				//��������
    DWORD               dwAndroidCount;             //��������
	DWORD				dwFullCount;				//��Ա����
	TCHAR				szServerAddr[32];			//�����ַ
	TCHAR				szServerName[LEN_GAME_ROOM];//��������
}ST_GAME_ROOM_SERVER, *PST_GAME_ROOM_SERVER;

//��������
typedef struct _stSignupMatchInfo
{
	WORD				wServerID;					//�����ʶ
	DWORD				dwMatchID;					//������ʶ
	DWORD				dwMatchNO;					//��������
}ST_SIGNUP_MATCH_INFO, *PST_SIGNUP_MATCH_INFO;

//������Ϣ
typedef struct _stGameMatch
{
	//������Ϣ
	WORD				wServerID;					//�����ʶ
	DWORD				dwMatchID;					//������ʶ
	DWORD				dwMatchNO;					//��������
	BYTE				cbMatchType;				//��������
	TCHAR				szMatchName[32];			//��������

	//������Ϣ
	BYTE				cbMemberOrder;				//��Ա�ȼ�
	BYTE				cbMatchFeeType;				//�۷�����
	LONGLONG				lMatchFee;					//��������

	//������Ϣ
	WORD				wStartUserCount;			//��������
	WORD				wMatchPlayCount;			//��������

	//��������
	WORD                wRewardCount;               //��������

	//����ʱ��
	ST_SYS_TIME			stMatchStartTime;			//��ʼʱ��
	ST_SYS_TIME			stMatchEndTime;				//����ʱ��
}ST_GAME_MATCH, *PST_GAME_MATCH;

//��Ƶ����
typedef struct _stAVServerConfig
{
	WORD				wAVServerPort;				//��Ƶ�˿�
	DWORD				dwAVServerAddr;				//��Ƶ��ַ
}ST_AV_SERVER_CONFIG, *PST_AV_SERVER_CONFIG;

//������Ϣ
typedef struct _stOnlineInfoType
{
	WORD				wKindID;					//���ͱ�ʶ
	DWORD				dwOnlineCount;				//��������
}ST_ONLINE_INFO_TYPE, *PST_ONLINE_INFO_TYPE;


//����ͳ��
typedef struct _stOnlineInfoStatistics
{
	WORD				wKindID;					//���ͱ�ʶ
	DWORD				dwOnLineCount;				//��������
	DWORD				dwAndroidCount;				//��������
}ST_ONLINE_INFO_STATISTICS, *PST_ONLINE_INFO_STATISTICS;

//��Ϸ����������Ϣ
typedef struct _stOnlineInfoGameRoom
{
	WORD				wServerID;					//�����ʶ
	DWORD				dwOnLineCount;				//��������
}ST_ONLINE_INFO_ROOM_SERVER, *PST_ONLINE_INFO_ROOM_SERVER;

/////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark �û���Ϣ

//����״̬
typedef struct _stTableStatus
{
	BYTE				cbTableLock;				//������־
	BYTE				cbPlayStatus;				//��Ϸ��־
    INT					nCellScore;					//��Ԫ����
}ST_TABLE_STATUS, *PST_TABLE_STATUS;

//�û�״̬
typedef struct _stUserStatus
{
	WORD				wTableID;					//��������
	WORD				wChairID;					//����λ��
	BYTE				cbUserStatus;				//�û�״̬
}ST_USER_STATUS, *PST_USER_STATUS;

typedef struct _stUserRelationship
{
    BYTE               cbCompanion;                 //�û���ϵ
}ST_USER_RELATIONSHIP, *PST_USER_RELATIONSHIP;

//�û�����
typedef	struct _stUserScore
{
	//������Ϣ
	LONGLONG			llGameCoin;					//�û���Ϸ��
    LONGLONG            llGrade;                    //�û��ɼ�
	LONGLONG			llBanker;					//���з���
    LONGLONG            llGold;						//�û����
	
	//��Ӯ��Ϣ
	DWORD				dwWinCount;					//ʤ������
	DWORD				dwLostCount;				//ʧ������
	DWORD				dwDrawCount;				//�;�����
	DWORD				dwEscapeCount;				//��������
	
	//ȫ����Ϣ
	DWORD				dwUserMedal;				//�û�����
	DWORD				dwExp;						//�û�����
	DWORD				dwLoveLiness;				//�û�����
}ST_USER_SCORE, *PST_USER_SCORE;


#pragma mark -
#pragma mark �û�����
typedef struct _stMobileUserGameCoin
{
	//������Ϣ
	LONGLONG			llGameCoin;					//�û���Ϸ��
	DOUBLE              dBean;                      //�û���Ϸ��

	//��Ӯ��Ϣ
	DWORD				dwWinCount;					//ʤ������
	DWORD				dwLostCount;				//ʧ������
	DWORD				dwDrawCount;				//�;�����
	DWORD				dwEscapeCount;				//��������
	
	//ȫ����Ϣ
	DWORD				dwExp;						//�û�����
}ST_MOBILE_USER_SCORE, *PST_MOBILE_USER_SCORE;

//����ʹ��
typedef struct _stUsePropInfo
{
    WORD                wPropCount;					//������Ŀ
    WORD                wValidNum;					//��Ч����
    DWORD               dwEffectTime;               //��Чʱ��
}ST_USER_PROP_INFO, *PST_USER_PROP_INFO;

//�û�����
typedef struct _stUserProp
{
	WORD                wPropUseFlag;				//����ʹ�ñ�ʾ
	ST_USER_PROP_INFO	stUserPropInfoArray[MAX_PT_MARK];//ʹ����Ϣ
}ST_USER_PROP, *PST_USER_PROP;

//���߰���
typedef struct _stPropPackage
{
	WORD                wTrumpetCount;              //С������
	WORD                wTyphonCount;               //��������
}ST_PROP_PACKAGE, *PST_PROP_PACKAGE;

//ʱ����Ϣ
typedef struct _stTimeInfo
{
	DWORD				dwEnterTableTimer;			//��������ʱ��
	DWORD				dwLeaveTableTimer;			//�뿪����ʱ��
	DWORD				dwStartGameTimer;			//��ʼ��Ϸʱ��
	DWORD				dwEndGameTimer;				//�뿪��Ϸʱ��
}ST_TIME_INFO, *PST_TIME_INFO;

//�û���Ϣ
typedef struct _stUserInfo
{
	//��������
	DWORD				dwUserID;						//�û�I D
	DWORD				dwGameID;						//��ϷI D
	DWORD				dwGroupID;						//����I D
	
	TCHAR				szNickName[LEN_MAX_NICKNAME];	//�û��ǳ�
	TCHAR				szGroupName[LEN_MAX_GROUP_NAME];//��������
	TCHAR				szUnderWrite[LEN_SIGNATURE];	//����ǩ��
	
	//ͷ����Ϣ
	DWORD				dwFaceID;						//ͷ������
	DWORD				dwCustomID;						//�Զ���ʶ
	
	//�û�����
	BYTE				cbGender;						//�û��Ա�
	BYTE				cbMemberOrder;					//��Ա�ȼ�
	BYTE				cbMasterOrder;					//����ȼ�
	
	//�û�״̬
	WORD				wTableID;						//��������
    WORD                wLastTableID;                   //��Ϸ����
	WORD				wChairID;						//��������
	BYTE				cbUserStatus;					//�û�״̬
	
	//������Ϣ
	LONGLONG			llGameCoin;						//�û���Ϸ��
	LONGLONG			llGrade;						//�û��ɼ�
	LONGLONG			llBankGold;						//���н��
	LONGLONG            llGold;                         //�û����
    
	//��Ϸ��Ϣ
	DWORD				dwWinCount;						//ʤ������
	DWORD				dwLostCount;					//ʧ������
	DWORD				dwDrawCount;					//�;�����
	DWORD				dwEscapeCount;					//��������
	DWORD				dwExp;							//�û�����
	DWORD				dwLoveLiness;					//�û�����
    
    //ʱ����Ϣ
    ST_TIME_INFO        stTimeInfo;						//ʱ����Ϣ
}ST_USER_INFO, *PST_USER_INFO;

//�û���Ϣ tagUserInfoHead
typedef struct _stUserInfoHead
{
public:
	_stUserInfoHead()
    :dwGameID(0.0),dwUserID(0.0),dwFaceID(0.0),dwCustomID(0.0),cbGender('0'),cbVipLevel('0'),
    wTableID(0.0),wChairID(0.0),cbUserStatus('0'),llGameCoin(0.0),dwWinCount(0.0),dwLostCount(0.0),
    dwDrawCount(0.0),dwEscapeCount(0.0),dwExp(0.0)
    {
        
    }
    
	~_stUserInfoHead(){}
    
public:
	//�û�����
	DWORD				dwGameID;						//��ϷI D
	DWORD				dwUserID;						//�û�I D
    DWORD               dwGroupID;                      //����id
	
	//ͷ����Ϣ
	DWORD				dwFaceID;						//ͷ������
	DWORD				dwCustomID;						//�Զ���ʶ
	
	//�û�����
    bool                bIsAndroid;                     //OS��ʾ
	BYTE				cbGender;						//�û��Ա�
	BYTE				cbVipLevel;						//��Ա�ȼ�
    BYTE                cbMasterLevel;                  //����ȼ�
	
	//�û�״̬
	WORD				wTableID;						//��������
	WORD				wChairID;						//��������
	BYTE				cbUserStatus;					//�û�״̬
	
	//������Ϣ
	LONGLONG			llGameCoin;						//�û���Ϸ��
	LONGLONG            llGrade;                        //�û��ɼ�
	LONGLONG            llBankGold;						//�û����н��
	LONGLONG            llGold;                         //�û����
	
	//��Ϸ��Ϣ
	DWORD				dwWinCount;						//ʤ������
	DWORD				dwLostCount;					//ʧ������
	DWORD				dwDrawCount;					//�;�����
	DWORD				dwEscapeCount;					//��������
	DWORD				dwExp;							//�û�����
	DWORD				dwLoveliness;					//�û�����
}ST_USER_INFO_HEAD, *PST_USER_INFO_HEAD;

//��չ�ǳ�
typedef struct _stUserDataExt
{
	WORD				wDataSize;						//���ݴ�С
	WORD				wDataDesc;						//��������
}ST_USER_DATA_EXT, *PST_USER_DATA_EXT;

//ͷ����Ϣ
typedef struct _stCustomFaceInfo
{
	DWORD				dwDataSize;						//���ݴ�С
	DWORD				dwCutstomFace[FACE_CX*FACE_CY];	//ͼƬ��Ϣ
}ST_CUSTOM_FACE_INFO, *PST_CUSTOM_FACE_INFO;

//�û���Ϣ
typedef struct _stUserRemoteInfo
{
	//�û���Ϣ
	DWORD				dwUserID;						//�û���ʶ
	DWORD				dwGameID;						//��Ϸ��ʶ
	TCHAR				szNickName[LEN_MAX_NICKNAME];		//�û��ǳ�

	//�ȼ���Ϣ
	BYTE				cbGender;						//�û��Ա�
	BYTE				cbVipLevel;						//��Ա�ȼ�
	BYTE				cbMasterLevel;					//����ȼ�

	//λ����Ϣ
	WORD				wGameTypeID;					//��Ϸ���ͱ�ʶ
	WORD				wGameRoomID;					//��Ϸ�����ʶ
	TCHAR				szGameRoom[LEN_GAME_ROOM];		//����λ��
}ST_USER_REMOTE_INFO, *PST_USER_REMOTE_INFO;

//�������
typedef struct _stTaskParam
{
	//������Ϣ
	WORD				wTaskID;						//�����ʶ
	WORD				wTaskType;						//��������
    WORD				wTaskObject;					//����Ŀ��
	BYTE				cbPlayerType;					//�������
	WORD				wKindID;						//���ͱ�ʶ
	DWORD				dwTimeLimit;					//ʱ������

	//������Ϣ
	LONGLONG			llStandardAwardGold;			//�������
	LONGLONG			llStandardAwardMedal;			//��������
	LONGLONG			llVipAwardGold;					//�������
	LONGLONG			llVipAwardMedal;				//��������

	//������Ϣ
	TCHAR				szTaskName[LEN_TASK_NAME];		//��������
	TCHAR				szTaskDes[LEN_TASK_DES];		//��������
}ST_TASK_PARAM, *PST_TASK_PARAM;

//����״̬
typedef struct _stTaskStatus
{
	WORD				wTaskID;						//�����ʶ
	WORD				wTaskProgress;					//�������
	BYTE				cbTaskStatus;					//����״̬
}ST_TASK_STATUS, *PST_TASK_STATUS;

//�ͱ�����
typedef struct _stBaseEnsureParam
{
	LONGLONG			llGameCoinCondition;			//��Ϸ������
	LONGLONG			llGameCoinAmount;				//��Ϸ������
	BYTE				cbTakeTimes;					//��ȡ����
}ST_BASE_ENSURE_PARAM, *PST_BASE_ENSURE_PARAM;

//�ƹ���Ϣ
typedef struct _stUserSpreadInfo
{
	DWORD				dwSpreadCount;					//�ƹ�����
	LONGLONG			llSpreadReward;					//�ƹ㽱��
}ST_USER_SPREAD_INFO, *PST_USER_SPREAD_INFO;

//�ȼ�����
typedef struct _stGrowLevelConfig
{
	WORD				wLevel;							//�ȼ�ID
	DWORD				dwExp;							//��Ӧ����
}ST_GROW_LEVEL_CONFIG, *PST_GROW_LEVEL_CONFIG;

//�ȼ�����
typedef struct _stLevelUpParam
{
	WORD				wCurrLevel;						//��ǰ�ȼ�
	DWORD				dwCurrExp;						//��ǰ����
	DWORD				dwNextLevelExp;					//�¼�����
	LONGLONG			llLevelUpRewardGameCoin;		//����������Ϸ��
	LONGLONG			llLevelUpRewardGold;			//�����������
}ST_LEVEL_UP_PARAM, *PST_LEVEL_UP_PARAM;

//��Ա����
typedef struct _stVipParam
{
	BYTE				cbVipFlag;						//��Ա��ʶ
	TCHAR				szVipName[16];					//��Ա����
	LONGLONG			llVipPrice;						//��Ա�۸�
	LONGLONG			llPresentGameCoin;				//������Ϸ��
}ST_VIP_PARAM, *PST_VIP_PARAM;

//��������
typedef struct _stGameRoomConfig
{
	//�ҽ�����
	WORD				wKindID;						//�ҽ�����
	WORD				wNodeID;						//�ҽӽڵ�
    WORD				wSortID;						//���б�ʶ

	//˰������
	WORD				wRevenueRatio;					//˰�ձ���
	LONGLONG			llServiceGameCoin;				//�������:��Ϸ��

	//��������
	LONGLONG			llLimitGameCoin;				//������Ϸ��
	LONGLONG			lMinTableGameCoin;				//���ͬ����Ϸ��
	LONGLONG			lMinEnterGameCoin;				//��ͽ�����Ϸ��
	LONGLONG			lMaxEnterGameCoin;				//��߽�����Ϸ��
	
	//��Ա����
	BYTE				cbMinEnterVipLevel;				//��ͽ���Vip�ȼ�
	BYTE				cbMaxEnterVipLevel;				//��߽���Vip�ȼ�

	//��������
	DWORD				dwGameRoomRule;					//��Ϸ�������
	TCHAR				szGameRoomName[LEN_GAME_ROOM];	//��Ϸ��������
}ST_GAME_ROOM_CONFIG, *PST_GAME_ROOM_CONFIG;

//�û���Ϣ
typedef struct _stMobileUserHeadInfo
{
	//�û�����
	DWORD				dwGameID;						//��Ϸ I D
	DWORD				dwUserID;						//�û� I D
    
	//ͷ����Ϣ
	DWORD				dwFaceID;						//ͷ������
	DWORD				dwCustomID;						//�Զ���ʶ
    
	//�û�����
	BYTE				cbGender;						//�û��Ա�
	BYTE				cbVipLevel;						//��Ա�ȼ�

	//�û�״̬
	WORD				wTableID;						//��������
	WORD				wChairID;						//��������
	BYTE				cbUserStatus;					//�û�״̬

	//������Ϣ
	LONGLONG			llGameCoin;						//�û���Ϸ��
    LONGLONG            llGold;                         //�û����
    DOUBLE              dBean;                          //�û���Ϸ��

	//��Ϸ��Ϣ
	DWORD				dwWinCount;						//ʤ������
	DWORD				dwLostCount;					//ʧ������
	DWORD				dwDrawCount;					//�;�����
	DWORD				dwEscapeCount;					//��������
	DWORD				dwExp;							//�û�����
}ST_MOBILE_USER_HEAD_INFO, *PST_MOBILE_USER_HEAD_INFO;


//�㳡����
typedef struct _stGamePlaza
{
	WORD				wPlazaID;						//�㳡��ʶ
	TCHAR				szServerAddr[32];				//�����ַ
	TCHAR				szServerName[32];				//��������
}ST_GAME_PLAZA, *PST_GAME_PLAZA;

//��������
typedef struct _stLevelItem
{
	INT					nLevelGameCoin;					//������Ϸ��
	TCHAR				szLevelDes[16];					//��������
}ST_LEVEL_ITEM, *PST_LEVEL_ITEM;

//��Ա����
typedef struct _stMemberItem
{
	BYTE				cbVipLevel;						//��Ա�ȼ�
	TCHAR				szVipName[16];					//��Ա�ȼ�����
}ST_MEMBER_ITEM, *PST_MEMBER_ITEM;

//��������
typedef struct _stMasterItem
{
	BYTE				cbMasterLevel;					//����ȼ���ʶ
	TCHAR				szMasterName[16];				//����ȼ�����
}ST_MASTER_ITEM, *PST_MASTER_ITEM;

//�б�����
typedef struct _stColumnItem
{
	BYTE				cbColumnWidth;					//�б���
	BYTE				cbColumnType;					//�ֶ�����
	TCHAR				szColumnName[16];				//�б�����
}ST_COLUMN_ITEM, *PST_COLUMN_ITEM;

//��ַ��Ϣ
typedef struct _stAddressInfo
{
	TCHAR				szAddr[32];						//��������ַ
}ST_ADDRESS_INFO, *PST_ADDRESS_INFO;

//���ݿ���Ϣ
typedef struct _stDbConfig
{
	WORD				wPort;							//���ݿ�˿�
	TCHAR				szAddr[32];						//���ݿ��ַ
	TCHAR				szUser[32];						//���ݿ��û�
	TCHAR				szPwd[32];						//���ݿ�����
	TCHAR				szName[32];						//���ݿ�����
}ST_DB_CONFIG, *PST_DB_CONFIG;

//ת�̽�������������
typedef struct _stLotteryItem
{
    BYTE				cbItemIndex;					//��������(1-N)
    BYTE				cbItemType;						//��������(0��Ϸ�ң�1��Ϸ��)
    LONGLONG            llItemQuota;					//�������
}ST_LOTTERY_ITEM, *PST_LOTTERY_ITEM;

#pragma pack()

#endif