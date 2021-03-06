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

#ifndef __LogonServer_h__
#define __LogonServer_h__

#include "MacroDefine.h"
#include "Packet.h"
#include "Struct.h"

#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////
//登录命令
#define MDM_GP_LOGON                1

//登录模式
#define SUB_GP_LOGON_GAMEID         1                       //I D 登录
#define SUB_GP_LOGON_ACCOUNTS       2                       //帐号登录
#define SUB_GP_REGISTER_ACCOUNTS    3                       //注册帐号
#define SUB_GP_LOGON_MANAGETOOL     4                       //管理工具
#define SUB_GP_VERIFY_INDIVIDUAL	5                       //验证资料


//登录结果
#define SUB_GP_LOGON_SUCCESS		100                     //登录成功
#define SUB_GP_LOGON_FAILURE		101                     //登录失败
#define SUB_GP_LOGON_FINISH			102                     //登录完成
#define SUB_GP_VALIDATE_MBCARD      103                     //登录失败
#define SUB_GP_VALIDATE_PASSPORT	104                     //登录失败
#define SUB_GP_VERIFY_RESULT		105                     //验证结果
#define SUB_GP_MATCH_SIGNUPINFO		106                     //报名信息
#define SUB_GP_GROWLEVEL_CONFIG		107                     //等级配置
#define SUB_GP_UPDATE_NOTIFY		200                     //升级提示
//////////////////////////////////////////////////////////////////////////////////////////

#define MB_VALIDATE_FLAGS           0x01                    //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                    //效验低版本
#define VERIFY_ACCOUNTS				0x01                    //校验帐号
#define VERIFY_NICKNAME				0x02                    //校验昵称

//ID登录
typedef struct _stCmdGpLogonGameID
{
    //系统信息
    DWORD							dwPlazaVersion;             //广场版本
    TCHAR							szMachineID[LEN_MACHINE_ID];//机器序列
   
    //登录信息
	DWORD							dwGameID;                   //游戏 I D
    TCHAR							szPwd[LEN_MD5];				//登录密码
    BYTE							cbValidateFlags;            //校验标识
}ST_CMD_GP_LOGON_GAME_ID, *PST_CMD_GP_LOGON_GAME_ID;

//帐号登录
typedef struct _stCmdGpLogonAccounts
{
    //系统信息
    DWORD							dwPlazaVersion;                 //广场版本
    TCHAR							szMachineID[LEN_MACHINE_ID];    //机器序列
    
    //登录信息
    BYTE							cbValidateFlags;                //校验标识
    TCHAR							szPwd[LEN_MD5];					//登录密码
    TCHAR							szAccount[LEN_ACCOUNT];			//登录帐号
    TCHAR							szIdentityID[LEN_IDENTITY_ID];	//身份证号
}CMD_GP_LOGON_ACCOUNTS, *PCMD_GP_LOGON_ACCOUNTS;

//注册帐号
typedef struct _stCmdGpRegAccount
{
    //系统信息
	DWORD							dwPlazaVersion;                     //广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];        //机器序列

	//密码变量
	TCHAR							szLogonPwd[LEN_MD5];				//登录密码

	//注册信息
	WORD							wFaceID;                            //头像标识
	BYTE							cbGender;                           //用户性别
	TCHAR							szAccount[LEN_ACCOUNT];				//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];           //用户昵称
	TCHAR							szSpreader[LEN_ACCOUNT];			//推荐帐号
	TCHAR							szPassportID[LEN_IDENTITY_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];   //真实名字
    BYTE							cbValidateFlags;                    //校验标识
}ST_CMD_GP_REG_ACCOUNT, *PST_CMD_GP_REG_ACCOUNT;

//验证资料
typedef struct _stCmdGpVerifyIndividual
{
    //系统信息
    DWORD							dwPlazaVersion;                     //广场版本
    
    //验证信息
    WORD							wVerifyMask;                        //验证掩码
}ST_CMD_GP_VERIFY_INDIVIDUAL, *PST_CMD_GP_VERIFY_INDIVIDUAL;

//登录成功
typedef struct _stCmdGpLogonSucc
{
    //属性资料
    WORD							wFaceID;            			//头像标识
    DWORD							dwUserID;           			//用户 I D
    DWORD							dwGameID;           			//游戏 I D
    DWORD							dwGroupID;          			//社团标识
    DWORD							dwCustomID;         			//自定标识
    DWORD							dwExp;       					//经验数值
    DWORD							dwLoveLiness;       			//用户魅力
    
    //用户成绩
	LONGLONG						lUserGameCoin;         			//用户游戏币
	LONGLONG						lUserInsure;        			//用户银行
	LONGLONG						lUserGold;						//用户金币
    DOUBLE							dUserBean;						//用户游戏豆
    
    //用户信息
    BYTE							cbGender;						//用户性别
    BYTE							cbMoorMachine;					//锁定机器
    TCHAR							szAccounts[LEN_ACCOUNT];		//登录帐号
    TCHAR							szNickName[LEN_ACCOUNT];		//用户昵称
    TCHAR							szDynamicPwd[LEN_PWD];			//动态密码
    TCHAR							szGroupName[LEN_GROUP_NAME];	//社团名字
    
    //配置信息
	BYTE							cbInsureEnabled;				//银行使能标识
	BYTE                            cbShowServerStatus;				//显示服务器状态
}ST_CMD_GP_LOGON_SUCC, *PST_CMD_GP_LOGON_SUCC;

//登录失败
typedef struct _stCmdGpLogonFailure
{
    INT								nErrCode;						//错误代码
    TCHAR							szDesc[256];
}ST_CMD_GP_LOGON_FAILURE, *PST_CMD_GP_LOGON_FAILURE;

//登陆完成
typedef struct _stCmdGpLogonFinish
{
	WORD							wIntermitTime;					//中断时间
	WORD							wOnlineCountTime;				//更新时间
}ST_CMD_GP_LOGON_FINISH, *PST_CMD_GP_LOGON_FINISH;

//验证卡信息
typedef struct _stCmdGpValidateMBCard
{
    UINT							uMBCardID;						// 机器序列
}ST_CMD_GP_VALIDATE_MB_CARD, *PST_CMD_GP_VALIDATE_MB_CARD;

//验证结果
typedef struct _stCmdGpVerifyIndividualResult
{
	bool							bVerifyPassage;					//验证通过
	WORD							wVerifyMask;					//验证掩码
	TCHAR							szErrorMsg[128];				//错误信息
}ST_CMD_GP_VERIFY_INDIVIDUAL_RESULT, *PST_CMD_GP_VERIFY_INDIVIDUAL_RESULT;

//升级提示
typedef struct _stCmdGpUpdateNotify
{
    BYTE							cbMustUpdate;					//强行升级
    BYTE							cbAdviceUpdate;					//建议升级
    DWORD							dwCurrentVersion;				//当前版本
}ST_CMD_GP_UPDATE_NOTIFY, *PST_CMD_GP_UPDATE_NOTIFY;

//////////////////////////////////////////////////////////////////////////////////////////

//携带信息 _stCMD_GP_LogonSuccess
#define DTP_GP_GROUP_INFO			1								//社团信息
#define DTP_GP_MEMBER_INFO			2								//会员信息
#define	DTP_GP_UNDER_WRITE			3								//个性签名
#define DTP_GP_STATION_URL			4								//主页信息

//社团信息
typedef struct _stCmdGpGroupInfo
{
    DWORD							dwGroupID;						//社团索引
    TCHAR							szGroupName[LEN_GROUP_NAME];	//社团名字
}ST_DTP_GP_GROUP_INFO, *PST_DTP_GP_GROUP_INFO;

//会员信息
typedef struct _stCmdGpVipInfo
{
    BYTE							cbVipLevel;						//会员等级
    ST_SYS_TIME						stSysTimeVipEndDate;			//到期时间
}ST_DTP_GP_VIP_INFO, *PST_DTP_GP_VIP_INFO;

//列表命令
#define MDM_GP_SERVER_LIST			2								//列表信息

//获取命令
#define SUB_GP_GET_LIST				1								//获取列表
#define SUB_GP_GET_SERVER			2								//获取房间
#define SUB_GP_GET_MATCH			3								//获取比赛
#define SUB_GP_GET_ONLINE			4								//获取在线
#define SUB_GP_GET_COLLECTION		5								//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE			100								//类型列表
#define SUB_GP_LIST_KIND			101								//种类列表
#define SUB_GP_LIST_NODE			102								//节点列表
#define SUB_GP_LIST_PAGE			103								//定制列表
#define SUB_GP_LIST_SERVER			104								//房间列表
#define SUB_GP_LIST_MATCH			105								//比赛列表
#define SUB_GP_VIDEO_OPTION			106								//视频配置

//完成信息
#define SUB_GP_LIST_FINISH			200								//发送完成
#define SUB_GP_SERVER_FINISH		201								//房间完成
#define SUB_GP_MATCH_FINISH			202								//比赛完成

//在线信息
#define SUB_GR_KINE_ONLINE			300								//类型在线
#define SUB_GR_SERVER_ONLINE		301								//房间在线

/////////////////////////////////////////////////////////////////////////////////////////////////

//获取在线
typedef struct _stCmdGpGetOnline
{
    WORD							wGameRoomCount;					//房间数目
    WORD							wOnLineGameRoomID[MAX_GAME_ROOM];//房间标识
}ST_CMD_GP_GET_ONLINE, *PST_CMD_GP_GET_ONLINE;

//类型在线
typedef struct _stCmdGpTypeOnline
{
    WORD							wTypeCount;							//类型数目
    _stOnlineInfoType				stOnlineInfoGameType[MAX_GAME_TYPE];//类型在线
}ST_CMD_GP_TYPE_ONLINE, *PST_CMD_GP_TYPE_ONLINE;

//房间在线
typedef struct _stCmdGpServerOnline
{
    WORD							wGameRoomCount;						//房间数目
    _stOnlineInfoGameRoom			stOnLineInfoGameRoom[MAX_GAME_ROOM];//房间在线
}ST_CMD_GP_SERVER_ONLINE, *PST_CMD_GP_SERVER_ONLINE;

////////////////////////////////////////////////////////////////////////////////////////////////
//服务命令
#define MDM_GP_USER_SERVICE					3							//用户服务

//帐号服务
#define SUB_GP_MODIFY_MACHINE				100							//修改机器
#define SUB_GP_MODIFY_LOGON_PASS			101							//修改密码
#define SUB_GP_MODIFY_INSURE_PASS			102							//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE			103							//修改签名

//修改头像
#define SUB_GP_USER_FACE_INFO				120							//头像信息
#define SUB_GP_SYSTEM_FACE_INFO				122							//系统头像
#define SUB_GP_CUSTOM_FACE_INFO				123							//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL				140							//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL				141							//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL			152							//修改资料

//银行服务
#define SUB_GP_USER_ENABLE_INSURE			160							//开通银行
#define SUB_GP_USER_SAVE_SCORE				161							//存款操作
#define SUB_GP_USER_TAKE_SCORE				162							//取款操作
#define SUB_GP_USER_TRANSFER_SCORE			163							//转帐操作
#define SUB_GP_USER_INSURE_INFO				164							//银行资料
#define SUB_GP_QUERY_INSURE_INFO			165							//查询银行
#define SUB_GP_USER_INSURE_SUCCESS			166							//银行成功
#define SUB_GP_USER_INSURE_FAILURE			167							//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST		168							//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT		169							//用户信息
#define SUB_GP_USER_INSURE_ENABLE_RESULT	170							//开通结果

//比赛服务
#define SUB_GP_MATCH_SIGNUP					200							//比赛报名
#define SUB_GP_MATCH_UNSIGNUP				201							//取消报名
#define SUB_GP_MATCH_SIGNUP_RESULT			202							//报名结果

//签到服务
#define SUB_GP_CHECKIN_QUERY				220							//查询签到
#define SUB_GP_CHECKIN_INFO					221							//签到信息
#define SUB_GP_CHECKIN_DONE					222							//执行签到
#define SUB_GP_CHECKIN_RESULT				223							//签到结果

//任务服务
#define SUB_GP_TASK_LOAD					240							//加载任务
#define SUB_GP_TASK_TAKE					241							//领取任务
#define SUB_GP_TASK_REWARD					242							//任务奖励
#define SUB_GP_TASK_INFO					243							//任务信息
#define SUB_GP_TASK_LIST					244							//任务信息
#define SUB_GP_TASK_RESULT					245							//任务结果

//低保服务
#define SUB_GP_BASEENSURE_LOAD				260							//加载低保
#define SUB_GP_BASEENSURE_TAKE				261							//领取低保
#define SUB_GP_BASEENSURE_PARAMETER			262							//低保参数
#define SUB_GP_BASEENSURE_RESULT			263							//低保结果

//推广服务
#define SUB_GP_SPREAD_QUERY					280							//推广奖励
#define SUB_GP_SPREAD_INFO					281							//奖励参数

//等级服务
#define SUB_GP_GROWLEVEL_QUERY				300							//查询等级
#define SUB_GP_GROWLEVEL_PARAMETER			301							//等级参数
#define SUB_GP_GROWLEVEL_UPGRADE			302							//等级升级

//兑换服务
#define SUB_GP_EXCHANGE_QUERY				320							//兑换参数
#define SUB_GP_EXCHANGE_PARAMETER			321							//兑换参数
#define SUB_GP_PURCHASE_MEMBER				322							//购买会员
#define SUB_GP_PURCHASE_RESULT				323							//购买结果
#define SUB_GP_EXCHANGE_GAME_COIN_BY_GOLD	324                         //金币兑换游戏币
#define SUB_GP_EXCHANGE_GAME_COIN_BY_BEAN	325                         //游戏豆兑换游戏币

#define SUB_GP_EXCHANGE_RESULT				326							//兑换结果

//抽奖服务
#define SUB_GP_LOTTERY_CONFIG_REQ			340							//请求配置
#define SUB_GP_LOTTERY_CONFIG				341							//抽奖配置
#define SUB_GP_LOTTERY_USER_INFO			342							//抽奖信息
#define SUB_GP_LOTTERY_START				343							//开始抽奖
#define SUB_GP_LOTTERY_RESULT				344							//抽奖结果

//游戏服务
#define SUB_GP_QUERY_USER_GAME_DATA			360							//查询数据


//操作结果
#define SUB_GP_OPERATE_SUCCESS				500							//操作成功
#define SUB_GP_OPERATE_FAILURE				501							//操作失败

////////////////////////////////////////////////////////////////////////////////////////////////

//修改密码
typedef struct _stCmdGpModifyLogonPwd
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szNewPwd[LEN_PWD];					//新密码
    TCHAR							szOldPwd[LEN_PWD];					//原密码
}ST_CMD_GP_MODIFY_LOGON_PWD, *PST_CMD_GP_MODIFY_LOGON_PWD;

//修改密码
typedef struct _stCmdGpModifyInsurePwd
{
    DWORD							dwUserID;							//用户 I D
	TCHAR							szNewPwd[LEN_PWD];					//新密码
	TCHAR							szOldPwd[LEN_PWD];					//原密码
}ST_CMD_GP_MODIFY_INSURE_PWD, *PST_CMD_GP_MODIFY_INSURE_PWD;

//修改签名
typedef struct _stCmdGpModifyUnderWrite
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPwd[LEN_PWD];						//用户密码
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
}ST_CMD_GP_MODIFY_UNDER_WRITE, *PST_CMD_GP_MODIFY_UNDER_WRITE;

///////////////////////////////////////////////////////////////////////////////////////////////

//用户头像
typedef struct _stCmdGpUserFaceInfo
{
    WORD							wFaceID;							//头像标识
    DWORD							dwCustomID;							//自定标识
}ST_CMD_GP_USER_FACE_INFO, *PST_CMD_GP_USER_FACE_INFO;

//修改头像
typedef struct _stCmdGpSysFaceInfo
{
    WORD							wFaceID;							//头像标识
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPwd[LEN_PWD];						//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_SYS_FACE_INFO, *PST_CMD_GP_SYS_FACE_INFO;

//修改头像
typedef struct _stCmdGpCustomFaceInfo
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPwd[LEN_PWD];						//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
}ST_CMD_GP_CUSTOM_FACE_INFO, *PST_CMD_GP_CUSTOM_FACE_INFO;

///////////////////////////////////////////////////////////////////////////////////////////////
//绑定机器
typedef struct _stCmdGpModifyMachine
{
    BYTE							cbBind;								//绑定标志
    DWORD							dwUserID;							//用户标识
    TCHAR							szPwd[LEN_PWD];						//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_MODIFY_MACHINE, *PST_CMD_GP_MODIFY_MACHINE;

///////////////////////////////////////////////////////////////////////////////////////////////
//个人资料
typedef struct _stCmdGpUserIndividual
{
    DWORD							dwUserID;							//用户 I D
}ST_CMD_GP_USER_INDIVIDUAL, *PST_CMD_GP_USER_INDIVIDUAL;

//查询信息
typedef struct _stCmdGpQueryIndividual
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPwd[LEN_PWD];						//用户密码
}ST_CMD_GP_QUERY_INDIVIDUAL, *PST_CMD_GP_QUERY_INDIVIDUAL;

//修改资料
typedef struct _stCmdGpModifyIndividual
{
    BYTE							cbGender;							//用户性别
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPwd[LEN_PWD];						//用户密码
}ST_CMD_GP_MODIFY_INDIVIDUAL, *PST_CMD_GP_MODIFY_INDIVIDUAL;

///////////////////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual
#define DTP_GP_UI_ACCOUNTS			1									//用户帐号
#define DTP_GP_UI_NICKNAME			2									//用户昵称
#define DTP_GP_UI_USER_NOTE			3									//用户说明
#define DTP_GP_UI_UNDER_WRITE		4									//个性签名
#define DTP_GP_UI_QQ				5									//Q Q 号码
#define DTP_GP_UI_EMAIL				6									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		7									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		8									//移动电话
#define DTP_GP_UI_COMPELLATION		9									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	10									//联系地址

//////////////////////////////////////////////////////////////////////////////////////////////
//银行资料
typedef struct _stCmdGpUserInsureInfo
{
    BYTE							cbEnjoinTransfer;					//转帐开关
    WORD							wRevenueTake;						//税收比例
    WORD							wRevenueTransfer;					//税收比例
    WORD							wRevenueTransferMember;				//税收比例
    WORD							wServerID;							//房间标识
	LONGLONG						lUserGold;							//用户金币
	LONGLONG						lUserInsureGold;					//银行金币
	LONGLONG						lTransferPrerequisite;				//转帐条件
}ST_CMD_GP_USER_INSURE_INFO, *PST_CMD_GP_USER_INSURE_INFO;

//开通银行
typedef struct _stCmdGpUserEnableInsure
{
    DWORD							dwUserID;							//用户I D
    TCHAR							szLogonPwd[LEN_PWD];				//登录密码
    TCHAR							szInsurePwd[LEN_PWD];				//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_USER_ENABLE_INSURE, *PST_CMD_GP_USER_ENABLE_INSURE;

//存入金币
typedef struct _stCmdGpUserSaveGold
{
    DWORD							dwUserID;							//用户 I D
    LONGLONG						lSaveGold;							//存入金币
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_USER_SAVE_GOLD, *PST_CMD_GP_USER_SAVE_GOLD;

//提取金币
typedef struct _stCmdGpUserTakeOutGold
{
    DWORD							dwUserID;							//用户 I D
    LONGLONG						lTakeOutGold;						//提取金币
    TCHAR							szInsurePwd[LEN_MD5];				//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_USER_TAKE_OUT_GOLD, *PST_CMD_GP_USER_TAKE_OUT_GOLD;

//转帐金币
typedef struct _stCmdGpUserTransferGold
{
    DWORD							dwUserID;							//用户 I D
    LONGLONG						lTransferGold;						//转帐金币
    TCHAR							szInsurePwd[LEN_MD5];				//银行密码
    TCHAR							szAccounts[LEN_NICKNAME];			//目标用户
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转帐备注
}ST_CMD_GP_USER_TRANSFER_GOLD, *PST_CMD_GP_USER_TRANSFER_GOLD;

//银行成功
typedef struct _stCmdGpUserInsureSucc
{
    BYTE                            cbOperateType;                      //操作类型
    DWORD							dwUserID;							//用户 I D
    DWORD                           dwUserMedal;                        //用户奖牌
    DWORD                           dwRecordIndex;                      //记录编号
	LONGLONG						lUserGold;							//用户金币
	LONGLONG						lUserInsureGold;					//银行金币
    TCHAR							szDes[128];							//描述消息
}ST_CMD_GP_USER_INSURE_SUCC, *PST_CMD_GP_USER_INSURE_SUCC;

//银行失败
typedef struct _stCmdGpUserInsureFailure
{
    INT								lErrorCode;							//错误代码
    TCHAR							szDes[128];							//描述消息
}ST_CMD_GP_USER_INSURE_FAILURE, *PST_CMD_GP_USER_INSURE_FAILURE;

//提取结果
typedef struct _stCmdGpUserTakeOutResult
{
    DWORD							dwUserID;							//用户 I D
	LONGLONG						lUserGold;							//用户金币
	LONGLONG						lUserInsureGold;					//银行金币
}ST_CMD_GP_USER_TAKE_OUT_RESULT, *PST_CMD_GP_USER_TAKE_OUT_RESULT;

//查询银行
typedef struct _stCmdGpUserQueryInsureInfo
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPwd[LEN_MD5];						//银行密码
}ST_CMD_GP_QUERY_INSURE_INFO, *PST_CMD_GP_QUERY_INSURE_INFO;

//查询用户
typedef struct _stCmdGpUserInfoQueryReq
{
    BYTE                            cbByNickName;                       //昵称赠送
    TCHAR							szAccounts[LEN_ACCOUNT];			//目标用户
}ST_CMD_GP_USER_INFO_QUERY_REQ, *PST_CMD_GP_USER_INFO_QUERY_REQ;

//用户转账信息
typedef struct _stCmdGpUserTransferUserInfo
{
    DWORD							dwTargetGameID;						//目标用户
    TCHAR							szAccounts[LEN_ACCOUNT];			//目标账户
}ST_CMD_GP_USER_TRANSFER_USER_INFO, *PST_CMD_GP_USER_TRANSFER_USER_INFO;

//开通结果
typedef struct _stCmdGpUserInsureEnableResult
{
    BYTE							cbInsureEnabled;					//使能标识
    TCHAR							szDes[128];							//描述消息
}ST_CMD_GP_USER_INSURE_ENABLE_RESULT, *PST_CMD_GP_USER_INSURE_ENABLE_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////////
//查询签到
typedef struct _stCmdGpCheckInQueryInfo
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPwd[LEN_PWD];				//登录密码
}ST_CMD_GP_CHECK_IN_QUERY_INFO, *PST_CMD_GP_CHECK_IN_QUERY_INFO;

//签到信息
typedef struct _stCmdGpCheckInInfo
{
    WORD							wSeriesDate;						//连续日期
    bool							bTodayChecked;						//签到标识
    LONGLONG						lRewardGold[LEN_WEEK];				//奖励金币
}ST_CMD_GP_CHECK_IN_INFO, *PST_CMD_GP_CHECK_IN_INFO;

//执行签到
typedef struct _stCmdGpCheckInDone
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szPwd[LEN_PWD];						//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_CHECK_IN_DONE, *PST_CMD_GP_CHECK_IN_DONE;

//签到结果
typedef struct _stCmdGpCheckInResult
{
    bool							bSucc;								//成功标识
    LONGLONG						lCurrGold;							//当前金币
    TCHAR							szNotifyContent[128];				//提示内容
}ST_CMD_GP_CHECK_IN_RESULT, *PST_CMD_GP_CHECK_IN_RESULT;

//////////////////////////////////////////////////////////////////////////////////
//任务服务

//加载任务
typedef struct _stCmdGpTaskLoadInfo
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szPwd[LEN_PWD];						//用户密码
}ST_CMD_GP_TASK_LOAD_INFO, *PST_CMD_GP_TASK_LOAD_INFO;

//领取任务
typedef struct _stCmdGpTaskGet
{
    WORD							wTaskID;							//任务标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szPwd[LEN_PWD];						//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_TASK_GET, *PST_CMD_GP_TASK_GET;

//领取奖励
typedef struct _stCmdGpTaskReward
{
    WORD							wTaskID;							//任务标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szPwd[LEN_PWD];						//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_TASK_REWARD, *PST_CMD_GP_TASK_REWARD;

//任务信息
typedef struct _stCmdGpTaskInfo
{
    WORD							wTaskCount;							//任务数量
    ST_TASK_STATUS					stTaskStatusArray[TASK_MAX_COUNT];	//任务状态
}ST_CMD_GP_TASK_INFO, *PST_CMD_GP_TASK_INFO;

//任务结果
typedef struct _stCmdGpTaskResult
{
    //结果信息
    bool							bSucc;								//成功标识
    WORD							wCommandID;							//命令标识
    
    //财富信息
	LONGLONG						lCurrGameCoin;						//当前游戏币
	LONGLONG						lCurrGold;							//当前金币
    
    //提示信息
    TCHAR							szNotifyContent[128];				//提示内容
}ST_CMD_GP_TASK_RESULT, *PST_CMD_GP_TASK_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////
//低保服务

//领取低保
typedef struct _stCmdGpBaseEnsureGet
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szLogonPwd[LEN_PWD];				//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_BASE_ENSURE_GET, *PST_CMD_GP_BASE_ENSURE_GET;

//低保参数
typedef struct _stCmdGpBaseEnsureParam
{
    LONGLONG						lGameCoinCondition;					//游戏币条件
    LONGLONG						lGameCoinAmount;					//游戏币数量
    BYTE							cbGetTimes;							//领取次数
}ST_CMD_GP_BASE_ENSURE_PARAM, *PST_CMD_GP_BASE_ENSURE_PARAM;

//低保结果
typedef struct _stCmdGpBaseEnsureResult
{
    bool							bSucc;								//成功标识
    LONGLONG						lCurrGameCoin;						//当前游戏币
    TCHAR							szNotifyContent[128];				//提示内容
}ST_CMD_GP_BASE_ENSURE_RESULT, *PST_CMD_GP_BASE_ENSURE_RESULT;

///////////////////////////////////////////////////////////////////////////////////////////
//推广服务

//推广查询
typedef struct _stCmdGpUserSpreadQuery
{
    DWORD							dwUserID;							//用户标识
}ST_CMD_GP_USER_SPREAD_QUERY, *PST_CMD_GP_USER_SPREAD_QUERY;

//推广参数
typedef struct _stCmdGpUserSpreadParam
{
    DWORD							dwSpreadCount;						//推广人数
    LONGLONG						lSpreadReward;						//推广奖励
}ST_CMD_GP_USER_SPREAD_PARAM, *PST_CMD_GP_USER_SPREAD_PARAM;

///////////////////////////////////////////////////////////////////////////////////////////
//等级服务

//查询等级
typedef struct _stCmdGpGrowLevelQueryInfo
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szPwd[LEN_PWD];						//用户密码
    
    //附加信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_GROW_LEVEL_QUERY_INFO, *PST_CMD_GP_GROW_LEVEL_QUERY_INFO;

//等级配置
typedef struct _stCmdGpGrowLevelConfig
{
    WORD							wLevelCount;						//等级数目
    _stGrowLevelConfig				GrowLevelItem[60];					//等级配置
}ST_CMD_GP_GROW_LEVEL_CONFIG, *PST_CMD_GP_GROW_LEVEL_CONFIG;

//等级参数
typedef struct _stCmdGpGrowLevelParam
{
    WORD							wCurrLevelID;						//当前等级
    DWORD							dwCurrExp;							//当前经验
    DWORD							dwNextLevelExp;						//下级经验
    LONGLONG						lUpgradeRewardGameCoin;				//升级奖励游戏币
    LONGLONG						lUpgradeRewardGold;					//升级奖励金币
}ST_CMD_GP_GROW_LEVEL_PARAM, *PST_CMD_GP_GROW_LEVEL_PARAM;

//等级升级
typedef struct _stCmdGpGrowLevelUpgrade
{
    LONGLONG						lCurrGameCoin;						//当前游戏币
    LONGLONG						lCurrGold;							//当前金币
    TCHAR							szNotifyContent[128];				//提示内容
}ST_CMD_GP_GROW_LEVEL_UPGRADE, *PST_CMD_GP_GROW_LEVEL_UPGRADE;

/////////////////////////////////////////////////////////////////////////////////////////////
//兑换服务

//查询参数
typedef struct _stCmdGpExchangeParam
{
    DWORD							wGoldExchangeGameCoinRate;			//金币→游戏币兑换比率
    DWORD							wPresentExchangeGameCoinRate;		//魅力→游戏币兑换率
    DWORD							wRateBeanExchangeGameCoin;			//游戏豆→游戏币兑换率
    WORD							wVipCount;							//会员数目
    _stVipParam						stVipParamArray[10];				//会员参数
}ST_CMD_GP_EXCHANGE_PARAM, *PST_CMD_GP_EXCHANGE_PARAM;

//金币兑换游戏币
typedef struct _stCmdGpExchangeGameCoinByGold
{
    DWORD							dwUserID;							//用户标识
    LONGLONG						lExchangedGold;						//待兑换金币
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}ST_CMD_GP_EXCHANGE_GAME_COIN_BY_GOLD, *PST_CMD_GP_EXCHANGE_GAME_COIN_BY_GOLD;

//游戏豆兑换游戏币
typedef struct _stCmdGpExchangeGameCoinByBean
{
    DWORD							dwUserID;							//用户标识
    double							dExchangedBean;						//待兑换游戏豆
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}ST_CMD_GP_EXCHANGE_GAME_COIN_BY_BEAN, *PST_CMD_GP_EXCHANGE_GAME_COIN_BY_BEAN;

//兑换结果
typedef struct _stCmdGpExchangeGameCoinResult
{
    bool							bSucc;								//成功标识
	LONGLONG						lCurrGameCoin;						//当前游戏币
	LONGLONG						lCurrGold;							//当前金币
    double                          dCurrBean;							//当前游戏豆
    TCHAR							szNotifyContent[128];				//提示内容
}ST_CMD_GP_EXCHANGE_GAME_COIN_RESULT, *PST_CMD_GP_EXCHANGE_GAME_COIN_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////
//抽奖服务

//请求配置
typedef struct _stCmdGpLotteryConfigRequest
{
    WORD							wKindID;							//游戏标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPwd[LEN_MD5];				//登录密码
}ST_CMD_GP_LOTTERY_CONFIG_REQUEST, *PST_CMD_GP_LOTTERY_CONFIG_REQUEST;

//抽奖配置
typedef struct _stCmdGpLotteryConfig
{
    WORD							wLotteryCount;						//奖项个数
    _stLotteryItem					stLotteryItemArray[MAX_LOTTERY];	//奖项内容
}ST_CMD_GP_LOTTERY_CONFIG, *PST_CMD_GP_LOTTERY_CONFIG;

//抽奖信息
typedef struct _stCmdGpLotteryUserInfo
{
    BYTE							cbFreeCount;						//免费次数
    BYTE							cbAlreadyCount;						//已领次数
    WORD							wKindID;							//游戏标识
    DWORD							dwUserID;							//用户标识
    LONGLONG						lChargeFee;							//抽奖费用
}ST_CMD_GP_LOTTERY_USER_INFO, *PST_CMD_GP_LOTTERY_USER_INFO;

//开始抽奖
typedef struct _stCmdGpLotteryStart
{
    WORD							wKindID;							//游戏标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPwd[LEN_MD5];				//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_LOTTERY_START, *PST_CMD_GP_LOTTERY_START;

//抽奖结果
typedef struct _stCmdGpLotteryResult
{
    bool							bWined;								//中奖标识
    WORD							wKindID;							//游戏标识
    DWORD							dwUserID;							//用户标识
    LONGLONG						lUserScore;							//用户分数
    DOUBLE							dUserBeans;							//用户游戏豆
    _stLotteryItem					LotteryItem;						//中奖内容
}ST_CMD_GP_LOTTERY_RESULT, *PST_CMD_GP_LOTTERY_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////
//游戏服务
//#pragma once

typedef struct _stCmdGpQueryUserGameData
{
    WORD							wKindID;							//游戏标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szDynamicPwd[LEN_MD5];				//用户密码
}ST_CMD_GP_QUERY_USER_GAEM_DATA, *PST_CMD_GP_QUERY_USER_GAEM_DATA;

//附加信息
#define DTP_GP_UI_USER_GAME_DATA	1									//游戏数据
////////////////////////////////////////////////////////////////////////////////////////////////

//比赛报名
typedef struct _stCmdGpMatchSignUp
{
    //比赛信息
    WORD							wGameRoomID;						//房间标识
    DWORD							dwMatchID;							//比赛标识
    DWORD							dwMatchNO;							//比赛场次
    
    //用户信息
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPwd[LEN_MD5];				//登录密码
    
    //机器信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_MATCH_SIGN_UP, *PST_CMD_GP_MATCH_SIGN_UP;

//取消报名
typedef struct _stCmdGpMatchUnSignUp
{
    //比赛信息
    WORD							wGameRoomID;						//房间标识
    DWORD							dwMatchID;							//比赛标识
    DWORD							dwMatchNO;							//比赛场次
    //用户信息
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPwd[LEN_MD5];				//登录密码
    
    //机器信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GP_MATCH_UN_SIGN_UP, *PST_CMD_GP_MATCH_UN_SIGN_UP;

//报名结果
typedef struct _stCmdGpMatchSignUpResult
{
    bool							bSignup;							//报名标识
    bool							bSucc;								//成功标识
    TCHAR							szDes[128];							//描述信息
}ST_CMD_GP_MATCH_SIGN_UP_RESULT, *PST_CMD_GP_MATCH_SIGN_UP_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////
//操作失败
typedef struct _stCmdGpOperateFailure
{
    INT								nErrorCode;							//错误代码
    TCHAR							szDes[128];							//描述消息
}ST_CMD_GP_OPERATE_FAILURE, *PST_CMD_GP_OPERATE_FAILURE;

//操作成功
typedef struct _stCmdGpOperateSucc
{
    INT								nErrorCode;							//操作代码
    TCHAR							szDes[128];							//成功消息
}ST_CMD_GP_OPERATE_SUCC, *PST_CMD_GP_OPERATE_SUCC;

//////////////////////////////////////////////////////////////////////////////////////////////////
//远程服务
#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//协调查找
typedef struct _stCmdGpSearchCorrespondClient
{
    DWORD							dwGameID;							//游戏标识
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
}ST_CMD_GP_SEARCH_CORRESPOND_CLIENT, *PST_CMD_GP_SEARCH_CORRESPOND_CLIENT;

//协调查找
typedef struct _stCmdGpSearchCorrespondServer
{
    WORD							wUserCount;							//用户数目
    _stUserRemoteInfo				stUserRemoteInfoArray[16];			//用户信息
}ST_CMD_GP_SEARCH_CORRESPOND_SERVER, *PST_CMD_GP_SEARCH_CORRESPOND_SERVER;


//////////////////////////////////////////////////////////////////////////////////
//登陆命令
#define MDM_MB_LOGON				100									//广场登陆

//登陆模式
#define SUB_MB_LOGON_GAMEID			1									//I D登陆
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登陆
#define SUB_MB_REGISITER_ACCOUNTS	3									//注册帐号
#define SUB_MB_LOGON_OTHERPLATFORM	4									//其他登陆
#define SUB_MB_LOGON_VISITOR        5									//游客登录
#define SUB_MB_ONLINE_CHECK         6									//在线检测

//登陆结果
#define SUB_MB_LOGON_SUCCESS		100									//登陆成功
#define SUB_MB_LOGON_FAILURE		101									//登陆失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//游客登录
typedef struct _stCmdMbLogonVisitor
{
	//系统消息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE							cbDeviceType;						//设备类型
    
    //连接信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
}ST_CMD_MB_LOGON_VISITOR, *PST_CMD_MB_LOGON_VISITOR;

//游客验证
typedef struct _stCmdMbOnlineCheck
{
    DWORD							dwUserID;							//用户ID
}ST_CMD_MB_ONLINE_CHECK, *PST_CMD_MB_ONLINE_CHECK;

//帐号登陆
typedef struct _stCmdMbLogonAccounts
{
    //系统信息
    WORD							wModuleID;							//模块标识
    DWORD							dwPlazaVersion;						//广场版本
    BYTE							cbDeviceType;						//设备类型
    
    //登陆信息
    TCHAR							szLogonPwd[LEN_MD5];				//登陆密码
    TCHAR							szLogonAccounts[LEN_ACCOUNT];		//登陆帐号
    
    //连接信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
}ST_CMD_MB_LOGON_ACCOUNTS, *PST_CMD_MB_LOGON_ACCOUNTS;

//注册帐号
typedef struct _stCmdMbRegAccounts
{
    //系统信息
    WORD							wModuleID;							//模块标识
    DWORD							dwPlazaVersion;						//广场版本
    BYTE							cbDeviceType;                       //设备类型
    
    //密码变量
    TCHAR							szLogonPwd[LEN_MD5];				//登录密码
    
    //注册信息
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    TCHAR							szAccounts[LEN_ACCOUNT];			//注册帐号
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    
    //连接信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
}ST_CMD_MB_REG_ACCOUNTS, *PST_CMD_MB_REG_ACCOUNTS;

//其他平台登陆
typedef struct _stCmdMbLogonOtherPlatform
{
    //系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE							cbDeviceType;                       //设备类型

	//注册信息
	BYTE							cbGender;                           //用户性别
	BYTE							cbPlatformID;                       //平台编号
	TCHAR							szUserUin[LEN_USER_UIN];            //登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							zsCompellation[LEN_COMPELLATION];   //真实名字

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
}ST_CMD_MB_LOGON_OTHER_PLATFORM, *PST_CMD_MB_LOGON_OTHER_PLATFORM;

//登陆成功
typedef struct _stCmdMbLogonSucc
{
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    BYTE							cbMoorMachine;                      //锁定机器
    DWORD							cbCustomFaceID;                     //自定义头像
    DWORD							dwUserID;							//用户ID
    DWORD							dwGameID;							//游戏ID
    DWORD							dwExp;								//经验数值
    DWORD							dwLoveLiness;						//用户魅力
    TCHAR							szAccounts[LEN_ACCOUNT];			//用户帐号
    TCHAR							szNickName[LEN_ACCOUNT];			//用户昵称
    TCHAR							szDynamicPwd[LEN_PWD];				//动态密码
    
    //用户成绩
    LONGLONG						lUserGameCoin;                      //用户游戏币
    LONGLONG						lUserGold;							//用户金币
    LONGLONG						lUserInsure;                        //用户银行
    DOUBLE							dUserBean;							//用户游戏豆
    
    //扩展信息
    BYTE							cbInsureEnable;                     //使能标示
}ST_CMD_MB_LOGON_SUCC, *PST_CMD_MB_LOGON_SUCC;

//登陆失败
typedef struct _stCmdMbLogonFailure
{
	INT								lErrorCode;							//错误代码
	TCHAR							szDes[128];							//错误消息
}ST_CMD_MB_LOGON_FAILURE, *PST_CMD_MB_LOGON_FAILURE;

//升级提示
typedef struct _stCmdMbUpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议版本
	DWORD							dwCurrentVersion;					//当前版本
}ST_CMD_MB_UPDATE_NOTIFY, *PST_CMD_MB_UPDATE_NOTIFY;

/////////////////////////////////////////////////////////////////////
//列表命令
#define MDM_MB_SERVER_LIST			101									//列表信息

#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_MATCH_SERVER         102									//比赛列表
#define SUB_GP_LIST_MATCH			105									//比赛列表

#define SUB_MB_LIST_FINISH			200									//列表完成

#define SUB_MB_ONLINE_GET			300									//获取在线
#define SUB_MB_ONLINE_GAME_TYPE		301									//类型在线
#define SUB_MB_ONLINE_GAME_ROOM		302									//房间在线

//获取在线
typedef struct _stCmdMbOnlineGet
{
    WORD							wCount;								//房间数目
    WORD							wOnLineGameRoomID[MAX_GAME_ROOM];	//房间标识
}ST_CMD_MB_ONLINE_GET, *PST_CMD_MB_ONLINE_GET;

//类型在线
typedef struct _stCmdMbOnlineGameType
{
    WORD							wCount;								//类型数目
    _stOnlineInfoType				OnLineInfoGameType[MAX_GAME_TYPE];	//类型在线
}ST_CMD_MB_ONLINE_GAME_TYPE, *PST_CMD_MB_ONLINE_GAME_TYPE;

//房间在线
typedef struct _stCmdMbOnlineGameRoom
{
    WORD							wCount;								//房间数目
    _stOnlineInfoGameRoom			OnLineInfoGameRoom[MAX_GAME_ROOM];	//房间在线
}ST_CMD_MB_ONLINE_GAME_ROOM, *PST_CMD_MB_ONLINE_GAME_ROOM;

#pragma pack()

#endif