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
typedef struct _stCMD_GP_LogonGameID
{
    //系统信息
    DWORD							dwPlazaVersion;             //广场版本
    TCHAR							szMachineID[LEN_MACHINE_ID];//机器序列
   
    //登录信息
	DWORD							dwGameID;                   //游戏 I D
    TCHAR							szPassword[LEN_MD5];        //登录密码
    BYTE							cbValidateFlags;            //校验标识
}ST_CMD_GP_LOGON_GAME_ID, *PST_CMD_GP_LOGON_GAME_ID;

//帐号登录
typedef struct _stCMD_GP_LogonAccounts
{
    //系统信息
    DWORD							dwPlazaVersion;                 //广场版本
    TCHAR							szMachineID[LEN_MACHINE_ID];    //机器序列
    
    //登录信息
    BYTE							cbValidateFlags;                //校验标识
    TCHAR							szPwd[LEN_MD5];					//登录密码
    TCHAR							szAccount[LEN_ACCOUNT];		//登录帐号
    TCHAR							szIdentityID[LEN_IDENTITY_ID];	//身份证号
}CMD_GP_LOGON_ACCOUNTS, *PCMD_GP_LOGON_ACCOUNTS;

//注册帐号
typedef struct _stCMD_GP_RegisterAccount
{
    //系统信息
	DWORD							dwPlazaVersion;                     //广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];        //机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];               //登录密码

	//注册信息
	WORD							wFaceID;                            //头像标识
	BYTE							cbGender;                           //用户性别
	TCHAR							szAccount[LEN_ACCOUNT];				//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];           //用户昵称
	TCHAR							szSpreader[LEN_ACCOUNT];			//推荐帐号
	TCHAR							szPassPortID[LEN_IDENTITY_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];   //真实名字
    BYTE							cbValidateFlags;                    //校验标识
}ST_CMD_GP_REGISTER_ACCOUNT, *PST_CMD_GP_REGISTER_ACCOUNT;

//验证资料
typedef struct _stCMD_GP_VerifyIndividual
{
    //系统信息
    DWORD							dwPlazaVersion;                     //广场版本
    
    //验证信息
    WORD							wVerifyMask;                        //验证掩码
}ST_CMD_GP_VERIFY_INDIVIDUAL, *PST_CMD_GP_VERIFY_INDIVIDUAL;

//登录成功
typedef struct _stCMD_GP_LogonSuccess
{
    //属性资料
    WORD							wFaceID;            			//头像标识
    DWORD							dwUserID;           			//用户 I D
    DWORD							dwGameID;           			//游戏 I D
    DWORD							dwGroupID;          			//社团标识
    DWORD							dwCustomID;         			//自定标识
    DWORD							dwExperience;       			//经验数值
    DWORD							dwLoveLiness;       			//用户魅力
    
    //用户成绩
    SCORE							lUserScore;         			//用户金币
    SCORE							lUserInsure;        			//用户银行
    SCORE							lUserIngot;						//用户元宝
    DOUBLE							dUserBeans;						//用户游戏豆
    
    //用户信息
    BYTE							cbGender;						//用户性别
    BYTE							cbMoorMachine;					//锁定机器
    TCHAR							szAccounts[LEN_ACCOUNT];		//登录帐号
    TCHAR							szNickName[LEN_ACCOUNT];		//用户昵称
    TCHAR							szDynamicPass[LEN_PASSWORD];	//动态密码
    TCHAR							szGroupName[LEN_GROUP_NAME];	//社团名字
    
    //配置信息
	BYTE							cbInsureEnabled;				//银行使能标识
	BYTE                            cbShowServerStatus;				//显示服务器状态
}ST_CMD_GP_LOGON_SUCCESS, *PST_CMD_GP_LOGON_SUCCESS;

//登录失败
typedef struct _stCMD_GP_LogonFail
{
    INT								nErrCode;						//错误代码
    TCHAR							szDesc[256];
}ST_CMD_GP_LOGON_FAIL, *PST_CMD_GP_LOGON_FAIL;

//登陆完成
typedef struct _stCMD_GP_LogonFinish
{
	WORD							wIntermitTime;					//中断时间
	WORD							wOnlineCountTime;				//更新时间
}ST_CMD_GP_LOGON_FINISH, *PST_CMD_GP_LOGON_FINISH;

//登录失败
typedef struct _stCMD_GP_ValidateMBCard
{
    UINT							uMBCardID;						// 机器序列
}ST_CMD_GP_VALIDATE_MB_CARD, *PST_CMD_GP_VALIDATE_MB_CARD;

//验证结果
typedef struct _stCMD_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;					//验证通过
	WORD							wVerifyMask;					//验证掩码
	TCHAR							szErrorMsg[128];				//错误信息
}ST_CMD_GP_VERIFY_INDIVIDUAL_RESULT, *PST_CMD_GP_VERIFY_INDIVIDUAL_RESULT;

//升级提示
typedef struct _stCMD_GP_UpdateNotify
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
typedef struct _stDTP_GP_GroupInfo
{
    DWORD							dwGroupID;							//社团索引
    TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
}ST_DTP_GP_GROUP_INFO, *PST_DTP_GP_GROUP_INFO;

//会员信息
typedef struct _stDTP_GP_MemberInfo
{
    BYTE							cbMemberOrder;						//会员等级
    ST_SYS_TIME						MemberOverDate;						//到期时间
}ST_DTP_GP_MEMBER_INFO, *PST_DTP_GP_MEMBER_INFO;

//列表命令
#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_MATCH			3									//获取比赛
#define SUB_GP_GET_ONLINE			4									//获取在线
#define SUB_GP_GET_COLLECTION		5									//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_LIST_MATCH			105									//比赛列表
#define SUB_GP_VIDEO_OPTION			106									//视频配置

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成
#define SUB_GP_MATCH_FINISH			202									//比赛完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

/////////////////////////////////////////////////////////////////////////////////////////////////

//获取在线
typedef struct _stCMD_GP_GetOnline
{
    WORD							wServerCount;						//房间数目
    WORD							wOnLineServerID[MAX_SERVER];		//房间标识
}ST_CMD_GP_GET_ONLINE, *PST_CMD_GP_GET_ONLINE;

//类型在线
typedef struct _stCMD_GP_TypeOnline
{
    WORD							wTypeCount;							//类型数目
    _stOnlineInfoType				OnlineInfoType[MAX_KIND];			//类型在线
};

//房间在线
typedef struct CMD_GP_ServerOnline
{
    WORD							wServerCount;						//房间数目
    _stOnlineInfoRoomServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

////////////////////////////////////////////////////////////////////////////////////////////////
//服务命令
#define MDM_GP_USER_SERVICE				3								//用户服务

//帐号服务
#define SUB_GP_MODIFY_MACHINE			100								//修改机器
#define SUB_GP_MODIFY_LOGON_PASS		101								//修改密码
#define SUB_GP_MODIFY_INSURE_PASS		102								//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE		103								//修改签名

//修改头像
#define SUB_GP_USER_FACE_INFO			120								//头像信息
#define SUB_GP_SYSTEM_FACE_INFO			122								//系统头像
#define SUB_GP_CUSTOM_FACE_INFO			123								//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL			140								//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL			141								//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL		152								//修改资料

//银行服务
#define SUB_GP_USER_ENABLE_INSURE		160								//开通银行
#define SUB_GP_USER_SAVE_SCORE			161								//存款操作
#define SUB_GP_USER_TAKE_SCORE			162								//取款操作
#define SUB_GP_USER_TRANSFER_SCORE		163								//转帐操作
#define SUB_GP_USER_INSURE_INFO			164								//银行资料
#define SUB_GP_QUERY_INSURE_INFO		165								//查询银行
#define SUB_GP_USER_INSURE_SUCCESS		166								//银行成功
#define SUB_GP_USER_INSURE_FAILURE		167								//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST	168								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT	169								//用户信息
#define SUB_GP_USER_INSURE_ENABLE_RESULT 170							//开通结果

//比赛服务
#define SUB_GP_MATCH_SIGNUP				200								//比赛报名
#define SUB_GP_MATCH_UNSIGNUP			201								//取消报名
#define SUB_GP_MATCH_SIGNUP_RESULT		202								//报名结果

//签到服务
#define SUB_GP_CHECKIN_QUERY			220								//查询签到
#define SUB_GP_CHECKIN_INFO				221								//签到信息
#define SUB_GP_CHECKIN_DONE				222								//执行签到
#define SUB_GP_CHECKIN_RESULT			223								//签到结果

//任务服务
#define SUB_GP_TASK_LOAD				240								//加载任务
#define SUB_GP_TASK_TAKE				241								//领取任务
#define SUB_GP_TASK_REWARD				242								//任务奖励
#define SUB_GP_TASK_INFO				243								//任务信息
#define SUB_GP_TASK_LIST				244								//任务信息
#define SUB_GP_TASK_RESULT				245								//任务结果

//低保服务
#define SUB_GP_BASEENSURE_LOAD			260								//加载低保
#define SUB_GP_BASEENSURE_TAKE			261								//领取低保
#define SUB_GP_BASEENSURE_PARAMETER		262								//低保参数
#define SUB_GP_BASEENSURE_RESULT		263								//低保结果

//推广服务
#define SUB_GP_SPREAD_QUERY				280								//推广奖励
#define SUB_GP_SPREAD_INFO				281								//奖励参数

//等级服务
#define SUB_GP_GROWLEVEL_QUERY			300								//查询等级
#define SUB_GP_GROWLEVEL_PARAMETER		301								//等级参数
#define SUB_GP_GROWLEVEL_UPGRADE		302								//等级升级

//兑换服务
#define SUB_GP_EXCHANGE_QUERY			320								//兑换参数
#define SUB_GP_EXCHANGE_PARAMETER		321								//兑换参数
#define SUB_GP_PURCHASE_MEMBER			322								//购买会员
#define SUB_GP_PURCHASE_RESULT			323								//购买结果
#define SUB_GP_EXCHANGE_SCORE_BYINGOT	324                             //兑换游戏币
#define SUB_GP_EXCHANGE_SCORE_BYBEAN	325                             //兑换游戏币

#define SUB_GP_EXCHANGE_RESULT			326								//兑换结果

//抽奖服务
#define SUB_GP_LOTTERY_CONFIG_REQ		340								//请求配置
#define SUB_GP_LOTTERY_CONFIG			341								//抽奖配置
#define SUB_GP_LOTTERY_USER_INFO		342								//抽奖信息
#define SUB_GP_LOTTERY_START			343								//开始抽奖
#define SUB_GP_LOTTERY_RESULT			344								//抽奖结果

//游戏服务
#define SUB_GP_QUERY_USER_GAME_DATA		360								//查询数据


//操作结果
#define SUB_GP_OPERATE_SUCCESS			500								//操作成功
#define SUB_GP_OPERATE_FAILURE			501								//操作失败

////////////////////////////////////////////////////////////////////////////////////////////////

//修改密码
typedef struct
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
    TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
}CMD_GP_ModifyLogonPass;

//修改密码
typedef struct
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
    TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
}CMD_GP_ModifyInsurePass;

//修改签名
struct CMD_GP_ModifyUnderWrite
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

///////////////////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
    WORD							wFaceID;							//头像标识
    DWORD							dwCustomID;							//自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
    WORD							wFaceID;							//头像标识
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
typedef struct
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
}CMD_GP_CustomFaceInfo;

///////////////////////////////////////////////////////////////////////////////////////////////
//绑定机器
struct CMD_GP_ModifyMachine
{
    BYTE							cbBind;								//绑定标志
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

///////////////////////////////////////////////////////////////////////////////////////////////
//个人资料
struct CMD_GP_UserIndividual
{
    DWORD							dwUserID;							//用户 I D
};

//查询信息
struct CMD_GP_QueryIndividual
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//修改资料
typedef struct
{
    BYTE							cbGender;							//用户性别
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
}CMD_GP_ModifyIndividual;


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
typedef struct
{
    BYTE							cbEnjoinTransfer;					//转帐开关
    WORD							wRevenueTake;						//税收比例
    WORD							wRevenueTransfer;					//税收比例
    WORD							wRevenueTransferMember;				//税收比例
    WORD							wServerID;							//房间标识
    SCORE							lUserScore;							//用户金币
    SCORE							lUserInsure;						//银行金币
    SCORE							lTransferPrerequisite;				//转帐条件
}CMD_GP_UserInsureInfo;

//开通银行
typedef struct _CMD_GP_UserEnableInsure
{
    DWORD							dwUserID;							//用户I D
    TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
    TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_UserEnableInsure;

//存入金币
typedef struct _CMD_GP_UserSaveScore
{
    DWORD							dwUserID;							//用户 I D
    SCORE							lSaveScore;							//存入金币
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_UserSaveScore;

//提取金币
typedef struct _CMD_GP_UserTakeScore
{
    DWORD							dwUserID;							//用户 I D
    SCORE							lTakeScore;							//提取金币
    TCHAR							szPassword[LEN_MD5];				//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_UserTakeScore;

//转帐金币
typedef struct _CMD_GP_UserTransferScore
{
    DWORD							dwUserID;							//用户 I D
    SCORE							lTransferScore;						//转帐金币
    TCHAR							szPassword[LEN_MD5];				//银行密码
    TCHAR							szAccounts[LEN_NICKNAME];			//目标用户
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转帐备注
}CMD_GP_UserTransferScore;
//银行成功
typedef struct
{
    BYTE                            cbOperateType;                      //操作类型
    DWORD							dwUserID;							//用户 I D
    DWORD                           dwUserMedal;                        //用户奖牌
    DWORD                           dwRecordIndex;                      //记录编号
    SCORE							lUserScore;							//用户金币
    SCORE							lUserInsure;						//银行金币
    TCHAR							szDescribeString[128];				//描述消息
}CMD_GP_UserInsureSuccess;

//银行失败
typedef struct _CMD_GP_UserInsureFailure
{
    INT							lResultCode;						//错误代码
    TCHAR							szDescribeString[128];				//描述消息
}CMD_GP_UserInsureFailure;

//提取结果

typedef struct _CMD_GP_UserTakeResult
{
    DWORD							dwUserID;							//用户 I D
    SCORE							lUserScore;							//用户金币
    SCORE							lUserInsure;						//银行金币
}CMD_GP_UserTakeResult;

//查询银行
typedef struct _CMD_GP_QueryInsureInfo
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_MD5];				//银行密码
}CMD_GP_QueryInsureInfo;

//查询用户
typedef struct _CMD_GP_QueryUserInfoRequest
{
    BYTE                            cbByNickName;                       //昵称赠送
    TCHAR							szAccounts[LEN_ACCOUNT];			//目标用户
}CMD_GP_QueryUserInfoRequest;

//用户信息
typedef struct _CMD_GP_UserTransferUserInfo
{
    DWORD							dwTargetGameID;						//目标用户
    TCHAR							szAccounts[LEN_ACCOUNT];			//目标用户
}CMD_GP_UserTransferUserInfo;

//开通结果
typedef struct _CMD_GP_UserInsureEnableResult
{
    
    BYTE							cbInsureEnabled;					//使能标识
    TCHAR							szDescribeString[128];				//描述消息
}CMD_GP_UserInsureEnableResult;

////////////////////////////////////////////////////////////////////////////////////////////////////
//查询签到
typedef struct _CMD_GP_CheckInQueryInfo
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//登录密码
}CMD_GP_CheckInQueryInfo;

//签到信息
typedef struct _CMD_GP_CheckInInfo
{
    WORD							wSeriesDate;						//连续日期
    bool							bTodayChecked;						//签到标识
    SCORE							lRewardGold[LEN_WEEK];				//奖励金币
}CMD_GP_CheckInInfo;

//执行签到
typedef struct _CMD_GP_CheckInDone
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_CheckInDone;


//签到结果
typedef struct _CMD_GP_CheckInResult
{
    bool							bSuccessed;							//成功标识
    SCORE							lScore;								//当前金币
    TCHAR							szNotifyContent[128];				//提示内容
}CMD_GP_CheckInResult;

//任务服务
//加载任务
typedef struct _CMD_GP_TaskLoadInfo
{
    
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
}CMD_GP_TaskLoadInfo;

//领取任务
typedef struct _CMD_GP_TaskTake
{
    WORD							wTaskID;							//任务标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_TaskTake;

//领取奖励
typedef struct _CMD_GP_TaskReward
{
    WORD							wTaskID;							//任务标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_TaskReward;

//任务信息
typedef struct _CMD_GP_TaskInfo
{
    WORD							wTaskCount;							//任务数量
    ST_TASK_STATUS					TaskStatus[TASK_MAX_COUNT];			//任务状态
}CMD_GP_TaskInfo;

//任务结果
typedef struct _CMD_GP_TaskResult
{
    //结果信息
    bool							bSuccessed;							//成功标识
    WORD							wCommandID;							//命令标识
    
    //财富信息
    SCORE							lCurrScore;							//当前金币
    SCORE							lCurrIngot;							//当前元宝
    
    //提示信息
    TCHAR							szNotifyContent[128];				//提示内容
}CMD_GP_TaskResult;
////////////////////////////////////////////////////////////////////////////////////////////
//低保服务

//领取低保
typedef struct _CMD_GP_BaseEnsureTake
{
    DWORD							dwUserID;							//用户 I D
    TCHAR							szPassword[LEN_PASSWORD];			//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_BaseEnsureTake;

//低保参数
typedef struct _CMD_GP_BaseEnsureParamter
{
    SCORE							lScoreCondition;					//游戏币条件
    SCORE							lScoreAmount;						//游戏币数量
    BYTE							cbTakeTimes;						//领取次数
}CMD_GP_BaseEnsureParamter;

//低保结果
typedef struct _CMD_GP_BaseEnsureResult
{
    bool							bSuccessed;							//成功标识
    SCORE							lGameScore;							//当前游戏币
    TCHAR							szNotifyContent[128];				//提示内容
}CMD_GP_BaseEnsureResult;
///////////////////////////////////////////////////////////////////////////////////////////
//推广服务

//推广查询
struct CMD_GP_UserSpreadQuery
{
    DWORD							dwUserID;							//用户标识
};
//推广参数
struct CMD_GP_UserSpreadInfo
{
    DWORD							dwSpreadCount;						//推广人数
    SCORE							lSpreadReward;						//推广奖励
};
///////////////////////////////////////////////////////////////////////////////////////////
//等级服务

//查询等级
typedef struct _CMD_GP_GrowLevelQueryInfo
{
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_PASSWORD];			//用户密码
    
    //附加信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}CMD_GP_GrowLevelQueryInfo;

//等级配置
struct CMD_GP_GrowLevelConfig
{
    WORD							wLevelCount;						//等级数目
    _stGrowLevelConfig				GrowLevelItem[60];					//等级配置
};
//等级参数
typedef struct _CMD_GP_GrowLevelParameter
{
    WORD							wCurrLevelID;						//当前等级
    DWORD							dwExperience;						//当前经验
    DWORD							dwUpgradeExperience;				//下级经验
    SCORE							lUpgradeRewardGold;					//升级奖励
    SCORE							lUpgradeRewardIngot;				//升级奖励
}CMD_GP_GrowLevelParameter;

//等级升级
typedef struct _CMD_GP_GrowLevelUpgrade
{
    SCORE							lCurrScore;							//当前游戏币
    SCORE							lCurrIngot;							//当前元宝
    TCHAR							szNotifyContent[128];				//提示内容
}CMD_GP_GrowLevelUpgrade;

/////////////////////////////////////////////////////////////////////////////////////////////
//兑换服务

//查询参数
typedef struct _CMD_GP_ExchangeParameter
{
    DWORD							wExchangeRate;						//元宝游戏币兑换比率
    DWORD							wPresentExchangeRate;				//魅力游戏币兑换率
    DWORD							wRateGold;							//游戏豆游戏币兑换率
    WORD							wMemberCount;						//会员数目
    _stMemberParam				MemberParameter[10];				//会员参数
}CMD_GP_ExchangeParameter;

//兑换游戏币
typedef struct _CMD_GP_ExchangeScoreByIngot
{
    
    DWORD							dwUserID;							//用户标识
    SCORE							lExchangeIngot;						//兑换元宝
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}CMD_GP_ExchangeScoreByIngot;

//兑换游戏币
typedef struct _CMD_GP_ExchangeScoreByBean
{
    
    DWORD							dwUserID;							//用户标识
    double							dExchangeBean;						//兑换元宝
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}CMD_GP_ExchangeScoreByBean;

//兑换结果
typedef struct _CMD_GP_ExchangeResult
{
    bool							bSuccessed;							//成功标识
    SCORE							lCurrScore;							//当前游戏币
    SCORE							lCurrIngot;							//当前元宝
    double                          dCurrBeans;                         //当前游戏豆
    TCHAR							szNotifyContent[128];				//提示内容
}CMD_GP_ExchangeResult;


////////////////////////////////////////////////////////////////////////////////////////////////
//抽奖服务

//请求配置
struct CMD_GP_LotteryConfigReq
{
    WORD							wKindID;								//游戏标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPass[LEN_MD5];					//登录密码
};

//抽奖配置
struct CMD_GP_LotteryConfig
{
    WORD							wLotteryCount;						//奖项个数
    _stLotteryItem					LotteryItem[MAX_LOTTERY];				//奖项内容
};

//抽奖信息
struct CMD_GP_LotteryUserInfo
{
    BYTE								cbFreeCount;							//免费次数
    BYTE								cbAlreadyCount;						//已领次数
    WORD							wKindID;								//游戏标识
    DWORD							dwUserID;							//用户标识
    SCORE							lChargeFee;							//抽奖费用
};

//开始抽奖
struct CMD_GP_LotteryStart
{
    WORD							wKindID;								//游戏标识
    DWORD							dwUserID;							//用户标识
    TCHAR							szLogonPass[LEN_MD5];					//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];			//机器序列
};

//抽奖结果
struct CMD_GP_LotteryResult
{
    bool								bWined;								//中奖标识
    WORD							wKindID;								//游戏标识
    DWORD							dwUserID;							//用户标识
    SCORE							lUserScore;							//用户分数
    DOUBLE							dUserBeans;							//用户游戏豆
    _stLotteryItem					LotteryItem;							//中奖内容
};

////////////////////////////////////////////////////////////////////////////////////////////////
//游戏服务
//#pragma once

struct CMD_GP_QueryUserGameData
{
    WORD							wKindID;					//游戏标识
    DWORD							dwUserID;					//用户标识
    TCHAR							szDynamicPass[LEN_MD5];				//用户密码
};

//附加信息
#define DTP_GP_UI_USER_GAME_DATA	1									//游戏数据
////////////////////////////////////////////////////////////////////////////////////////////////

//比赛报名
struct CMD_GP_MatchSignup
{
    //比赛信息
    WORD							wServerID;							//房间标识
    DWORD							dwMatchID;							//比赛标识
    DWORD							dwMatchNO;							//比赛场次
    
    //用户信息
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_MD5];				//登录密码
    
    //机器信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取消报名
struct CMD_GP_MatchUnSignup
{
    //比赛信息
    WORD							wServerID;							//房间标识
    DWORD							dwMatchID;							//比赛标识
    DWORD							dwMatchNO;							//比赛场次
    //用户信息
    DWORD							dwUserID;							//用户标识
    TCHAR							szPassword[LEN_MD5];				//登录密码
    
    //机器信息
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//报名结果
struct CMD_GP_MatchSignupResult
{
    bool							bSignup;							//报名标识
    bool							bSuccessed;							//成功标识
    TCHAR							szDescribeString[128];				//描述信息
};
////////////////////////////////////////////////////////////////////////////////////////////////
//操作失败
typedef struct
{
    INT							lResultCode;						//错误代码
    TCHAR							szDescribeString[128];				//描述消息
}CMD_GP_OperateFailure;

//操作成功
typedef struct
{
    INT							lResultCode;						//操作代码
    TCHAR							szDescribeString[128];				//成功消息
}CMD_GP_OperateSuccess;

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
struct CMD_GP_C_SearchCorrespond
{
    DWORD							dwGameID;							//游戏标识
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
    WORD							wUserCount;							//用户数目
    _stUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};


//////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////

//TODO:登陆命令
#define MDM_MB_LOGON				100				//广场登陆

//登陆模式
#define SUB_MB_LOGON_GAMEID			1				//I D登陆
#define SUB_MB_LOGON_ACCOUNTS		2				//帐号登陆
#define SUB_MB_REGISITER_ACCOUNTS  3               //注册帐号
#define SUB_MB_LOGON_OTHERPLATFORM	4               //其他登陆
#define SUB_MB_LOGON_VISITOR        5               //游客登录
#define SUB_MB_ONLINE_CHECK         6               //在线检测


//登陆结果
#define SUB_MB_LOGON_SUCCESS		100				//登陆成功
#define SUB_MB_LOGON_FAILURE		101				//登陆失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200				//升级提示
//游客登录
typedef struct {
    //系统消息
    WORD                wModuleID;                  //模块标识
    DWORD               dwPlazaVersion;             //广场版本
    BYTE                cbDeviceType;               //设备类型
    
    //连接信息
    TCHAR	szMachineID[LEN_MACHINE_ID];			//机器标识
    TCHAR	szMobilePhone[LEN_MOBILE_PHONE];		//电话号码
}CMD_MB_LogonVisitor;

//游客验证
typedef struct {
    DWORD               dwUserID;                   //用户ID
}CMD_MB_OnlineCheck;

//帐号登陆
typedef struct
{
    //系统信息
    WORD	wModuleID;								//模块标识
    DWORD	dwPlazaVersion;							//广场版本
    BYTE	cbDeviceType;							//设备类型
    
    //登陆信息
    TCHAR	szPassword[LEN_MD5];					//登陆密码
    TCHAR	szAccounts[LEN_ACCOUNT];				//登陆帐号
    
    //连接信息
    TCHAR	szMachineID[LEN_MACHINE_ID];			//机器标识
    TCHAR	szMobilePhone[LEN_MOBILE_PHONE];		//电话号码
    
}CMD_MB_LogonAccounts;

//注册帐号
typedef struct
{
    //系统信息
    WORD		wModuleID;							//模块标识
    DWORD		dwPlazaVersion;						//广场版本
    BYTE        cbDeviceType;                       //设备类型
    
    //密码变量
    TCHAR		szLogonPass[LEN_MD5];				//登录密码
    
    //注册信息
    WORD		wFaceID;							//头像标识
    BYTE		cbGender;							//用户性别
    TCHAR		szAccounts[LEN_ACCOUNT];			//登录帐号
    TCHAR		szNickName[LEN_NICKNAME];			//用户昵称
    
    //连接信息
    TCHAR		szMachineID[LEN_MACHINE_ID];		//机器标识
    TCHAR		szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
    
}CMD_MB_RegisterAccounts;

//其他登陆
typedef struct
{
    //系统信息
    WORD		wModuleID;							//模块标识
    DWORD		dwPlazaVersion;						//广场版本
    BYTE        cbDeviceType;                       //设备类型
    
    //注册信息
    BYTE        cbGender;                           //用户性别
    BYTE        cbPlatformID;                       //平台编号
    TCHAR       szUserUin[LEN_USER_UIN];            //登录帐号
    TCHAR		szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR       zsCompellation[LEN_COMPELLATION];   //真实名字
    
    //连接信息
    TCHAR		szMachineID[LEN_MACHINE_ID];		//机器标识
    TCHAR		szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
    
}CMD_MB_LogonOtherPlatform;



//登陆成功
typedef struct
{
    WORD	wFaceID;								//头像标识
    BYTE	cbGender;								//用户性别
    BYTE    cbMoorMachine;                          //锁定机器
    DWORD   cbCustomID;                             //自定义头像
    DWORD	dwUserID;								//用户ID
    DWORD	dwGameID;								//游戏ID
    DWORD	dwExperience;							//经验数值
    DWORD	dwLoveLiness;							//用户魅力
    TCHAR	szAccounts[LEN_ACCOUNT];				//用户帐号
    TCHAR	szNickName[LEN_ACCOUNT];				//用户昵称
    TCHAR   szDynamicPasswd[LEN_PASSWORD];          //动态密码
    
    //用户成绩
    SCORE   lUserScore;                             //用户游戏币
    SCORE   lUserIngot;                             //用户元宝
    SCORE   lUserInsure;                            //用户银行
    DOUBLE  dUserBeans;                             //用户游戏豆
    
    //扩展信息
    BYTE    cbInsureEnable;                         //使能标示
    
}CMD_MB_LogonSuccess;

//登陆失败
typedef struct
{
	INT	lErrorCode;								//错误代码
	TCHAR	szDescribeString[128];					//错误消息
}CMD_MB_LogonFailure;

//升级提示
typedef struct
{
	BYTE	cbMustUpdate;							//强行升级
	BYTE	cbAdviceUpdate;							//建议版本
	DWORD	dwCurrentVersion;						//当前版本
}CMD_MB_UpdateNotify;


/////////////////////////////////////////////////////////////////////
//列表命令
#define MDM_MB_SERVER_LIST			101				//列表信息

#define SUB_MB_LIST_KIND			100				//种类列表
#define SUB_MB_LIST_SERVER			101				//房间列表
#define SUB_MB_MATCH_SERVER         102             //比赛列表
#define SUB_GP_LIST_MATCH			105				//比赛列表
#define SUB_MB_LIST_FINISH			200				//列表完成

#define SUB_MB_GET_ONLINE			300									//获取在线
#define SUB_MB_KINE_ONLINE			301									//类型在线
#define SUB_MB_SERVER_ONLINE		302									//房间在线

//获取在线
typedef struct
{
    WORD							wServerCount;						//房间数目
    WORD							wOnLineServerID[MAX_SERVER];		//房间标识
}CMD_MB_GetOnline;

//类型在线
typedef struct
{
    WORD							wKindCount;							//类型数目
    _stOnlineInfoType				OnLineInfoKind[MAX_KIND];			//类型在线
}CMD_MB_KindOnline;

//房间在线
typedef struct
{
    WORD							wServerCount;						//房间数目
    _stOnlineInfoRoomServer				OnLineInfoServer[MAX_SERVER];		//房间在线
}CMD_MB_ServerOnline;

#pragma pack()

#endif