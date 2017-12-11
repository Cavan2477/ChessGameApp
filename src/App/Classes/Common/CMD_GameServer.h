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

#ifndef __GameServer_h__
#define __GameServer_h__

#include "MacroDefine.h"
#include "Packet.h"
#include "Struct.h"
#include "Property.h"

#pragma pack(1)

///////////////////////////////////////////////////////////////////////////
//登录命令
#define MDM_GR_LOGON				1									//登录信息

#pragma mark -
#pragma mark 登录模式
#define SUB_GR_LOGON_USERID			1									//id 登录
#define SUB_GR_LOGON_MOBILE			2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS		3									//帐户登录

#pragma mark -
#pragma mark 登录结果
#define SUB_GR_LOGON_SUCCESS		100									//登录成功
#define SUB_GR_LOGON_FAILURE		101									//登录失败
#define SUB_GR_LOGON_FINISH			102									//登录完成

#pragma mark -
#pragma mark 升级提示
#define SUB_GR_UPDATE_NOTIFY		200									//升级提示

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 设备类型
#define DEVICE_TYPE_ITOUCH			0x20								//iTouch
#define DEVICE_TYPE_IPHONE			0x40								//iPhone
#define DEVICE_TYPE_IPAD			0x80								//iPad
#define DEVICE_TYPE_DEFAULT         DEVICE_TYPE_IPHONE

#pragma mark -
#pragma mark 视图模式
#define	VIEW_MODE_ALL				0x0001								//全部可视
#define	VIEW_MODE_PART				0x0002								//部分可视

#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040							//隐藏信息

#pragma mark -
#pragma mark 信息模式
#define VIEW_INFO_LEVEL_1			0x0010								//部分信息
#define VIEW_INFO_LEVEL_2			0x0020								//部分信息
#define VIEW_INFO_LEVEL_3			0x0040								//部分信息
#define VIEW_INFO_LEVEL_4			0x0080								//部分信息

#pragma mark -
#pragma mark 其他配置
#define RECVICE_GAME_CHAT			0x0100								//接收聊天
#define RECVICE_ROOM_CHAT			0x0200								//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								//接收私聊

#pragma mark -
#pragma mark 行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //立即登录

//id 登录
typedef struct
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本
	
	//登录信息
	DWORD							dwUserID;							//用户id
	TCHAR							szPassword[LEN_MD5];				//登录密码
    TCHAR                           szServerPasswd[LEN_PASSWORD];       //房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    WORD                            wKindID;                            //类型索引
	
}CMD_GR_LogonUserID;

//手机登录
typedef struct
{
	//版本信息
	WORD							wGameID;							//游戏标识
	DWORD							dwProcessVersion;					//进程版本
	
	//桌子区域
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数
	
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDynamicPassword[LEN_MD5];			//登录密码
    TCHAR                           szServerPasswd[LEN_PASSWORD];       //房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}CMD_GR_LogonMobile;

//帐号登录
typedef struct
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本
	
	//登录信息
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

}CMD_GR_LogonAccounts;

//登录成功
typedef struct
{
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
}CMD_GR_LogonSuccess;

//登录失败
typedef struct
{
	INT							lErrorCode;								//错误代码
	TCHAR							szDescribeString[128];				//错误描述
}CMD_GR_LogonFailure;

//登录完成
typedef struct{
    
    bool                            bGuideTask ;                        //引导任务
}CMD_GR_LogonFinish;

//升级提示
typedef struct
{	
	//升级标志
	BYTE							cbMustUpdatePlaza;					//强行升级
	BYTE							cbMustUpdateClient;					//强行升级
	BYTE							cbAdviceUpdateClient;				//建议升级
	
	//当前版本
	DWORD							dwCurrentPlazaVersion;				//当前版本
	DWORD							dwCurrentFrameVersion;				//当前版本
	DWORD							dwCurrentClientVersion;				//当前版本
}CMD_GR_UpdateNotify;

////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 配置命令

#define	MDM_GR_CONFIG				2									//配置信息

#define	SUB_GR_CONFIG_COLUMN		100									//列表配置
#define SUB_GR_CONFIG_SERVER		101									//房间配置
#define SUB_GR_CONFIG_PROPERTY		102									//道具配置

#define SUB_GR_CONFIG_FINISH		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT    104                                 //玩家权限

/////////////////////////////////////////////////////////////////////////////////

//列表配置
typedef struct
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
}CMD_GR_ConfigColumn;

//房间配置
typedef struct
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目
	
	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
}CMD_GR_ConfigServer;

//道具配置
typedef struct
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPORTY];			//道具描述
}CMD_GR_ConfigProperty;

//玩家权限
typedef struct 
{
    DWORD                           dwUserRight;                        //玩家权限
}CMD_GR_ConfigUserRight;

/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 用户命令

#define MDM_GR_USER					3									//用户信息

//用户动作
#define SUB_GR_USER_RULE			1									//用户规则
#define	SUB_GR_USER_LOOKON			2									//旁观请求
#define	SUB_GR_USER_SITDOWN			3									//坐下请求
#define	SUB_GR_USER_STANDUP			4									//起立请求
#define SUB_GR_USER_INVITE			5									//用户邀请
#define SUB_GR_USER_INVITE_REQ		6									//邀请请求
#define SUB_GR_USER_REPULSE_SIT  	7									//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER       8                                   //踢出用户
#define SUB_GR_USER_INFO_REQ        9                                   //请求用户信息
#define SUB_GR_USER_CHAIR_REQ       10                                  //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ	11									//请求椅子用户信息
#define SUB_GR_USER_WAIT_DISTRIBUTE 12                                  //等待分配


//用户状态
#define	SUB_GR_USER_ENTER			100									//用户进入
#define	SUB_GR_USER_SCORE			101									//用户分数
#define SUB_GR_USER_STATUS			102									//用户状态

#define	SUB_GR_REQUEST_FAILURE		103									//请求失败

//聊天命令
#define	SUB_GR_USER_CHAT			201									//聊天信息
#define	SUB_GR_USER_EXPRESSION		202									//表情消息
#define SUB_GR_WISPER_CHAT			203									//私聊消息
#define	SUB_GR_WISPER_EXPRESSION	204									//私聊表情
#define SUB_GR_COLLOQUY_CHAT		205									//会话消息
#define	SUB_GR_COLLOQUY_ERPRESSION	206									//会话表情

//道具命令
#define SUB_GR_PROPERTY_BUY			300									//购买道具
#define SUB_GR_PROPERTY_SUCCESS		301									//道具成功
#define SUB_GR_PROPERTY_FAILURE		302									//道具失败
#define SUB_GR_PROPERTY_MESSAGE     303                                 //道具消息
#define SUB_GR_PROPERTY_EFFECT      304                                 //道具效应
#define SUB_GR_PROPERTY_TRUMPET     305                                 //用户喇叭

//赠送命令
#define SUB_GR_PLAYTIME_PRESENT     400                                 //赠送通知
#define SUB_GR_PRESENT_DRAW         401                                 //赠送领取

#define SR_FORFEND_LOCK_TABLE		0x00200000							//禁止锁桌

/////////////////////////////////////////////////////////////////////////////////

//旁观请求
typedef struct
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
}CMD_GR_UserLookon;

//坐下请求
typedef struct
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
}CMD_GR_UserSitDown;

//起立请求
typedef struct
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
}CMD_GR_UserStandUp;

//邀请用户
typedef struct
{
    WORD                            wTableID;                           //桌号码
    DWORD                           dwUserID;                           //用户id
    
}CMD_GR_UserInvite;

//邀请用户请求
typedef struct
{
    WORD                            wTableID;                           //桌号码
    DWORD                           dwUserID;                           //用户id
    
}CMD_GR_UserInviteReq;

//请求用户信息
typedef struct
{
    DWORD							dwUserIDReq;						//请求用户
    WORD							wTablePos;							//桌子位置
}CMD_GR_UserInfoReq;

//请求椅子信息
typedef struct
{
    WORD							wTableID;							//桌子号码
    WORD							wChairID;							//椅子号码
}CMD_GR_ChairUserInfoReq;

//用户分数
typedef struct
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//积分信息
}CMD_GR_UserScore;

#pragma mark -
#pragma mark 用户分数
typedef struct 
{
	DWORD							dwUserID;							//用户标识
	tagMobileUserScore				UserScore;							//积分信息
}CMD_GR_MobileUserScore;

//用户状态
typedef struct
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
}CMD_GR_UserStatus;

//请求失败
typedef struct
{
	INT								cbFailureCode;						//错误代码
	TCHAR							szDescribeString[256];				//描述信息
}CMD_GR_RequestFailure;

//用户聊天
typedef struct
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargerUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}CMD_GR_C_UserChat;

//用户聊天
typedef struct
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}CMD_GR_S_UserChat;

//用户表情
typedef struct
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
}CMD_GR_C_UserExpression;

//用户表情
typedef struct
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
}CMD_GR_S_UserExpression;

//用户私聊
typedef struct
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
    DWORD                           dwSendUserID;                       //发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}CMD_GR_C_WisperChat;

//用户私聊
typedef struct
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}CMD_GR_S_WisperChat;

//私聊表情
typedef struct
{
	WORD							wItemIndex;							//表情索引
    DWORD                           dwSendUserID;                       //发送用户
	DWORD							dwTargetUserID;						//目标用户
}CMD_GR_C_WisperExpression;

//私聊表情
typedef struct
{
	DWORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
}CMD_GR_S_WisperExpression;

//用户会话
typedef struct
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwConversationID;					//会话标识
	DWORD							dwTargetUserID[16];					//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}CMD_GR_GR_ColloquyChat;

//邀请用户
typedef struct
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
}CMD_GR_C_InviteUser;

//邀请用户
typedef struct
{
	DWORD							dwTargetUserID;						//目标用户
}CMD_GR_S_InviteUser;

//购买道具
typedef struct _CMD_GR_C_BuyProperty
{
    BYTE                            cbRequestArea;                      //请求范围
    BYTE                            cbConsumScore;                      //积分消费
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwTargetUserID;						//使用对象
}CMD_GR_C_BuyProperty;

//道具成功
typedef struct _CMD_GR_S_PropertySuccess
{
    BYTE                            cbRequestArea;                      //使用环境
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//目标对象
    DWORD                           dwTargetUserID;                     //使用对象
}CMD_GR_S_PropertySuccess;

//道具失败
typedef struct _CMD_GR_S_PropertyFailure
{
    WORD                            wRequestArea;                       //请求区域
    INT								lErrorCode;                         //错误代码
    TCHAR                           szDescribeString[256];              //描述信息
}CMD_GR_S_PropertyFailure;

//道具消息
typedef struct _CMD_GR_S_PropertyMessage
{
    WORD                            wPropertyIndex;                     //道具索引
    WORD                            wPropertyCount;                     //道具数目
    DWORD                           dwSourceUserID;                     //目标对象
    DWORD                           dwTargetUserID;                     //使用对象
}CMD_GR_S_PropertyMessage;

//道具效应
typedef struct _CMD_GR_S_PropertyEffect
{
    DWORD                           wUserID;                            //用户I D
    BYTE                            cbMemberOrder;                      //会员等级
}CMD_GR_S_PropertyEffect;

//发送喇叭
typedef struct _CMD_GR_C_SendTrumpet
{
    BYTE                            cbRequestArea;                      //
    WORD                            wPropertyIndex;
    DWORD                           TrumpetColor;
    TCHAR                           szTrumpetContent[TRUMPET_MAX_CHAR];
}CMD_GR_C_SendTrumpet;

//发送喇叭
typedef struct _CMD_GR_S_SendTrumpet
{
    WORD                            wPropertyIndex;
    DWORD                           dwSendUserID;
    DWORD                           TrumpetColor;
    TCHAR                           szSendNickName[32];
    TCHAR                           szTrumpetContent[TRUMPET_MAX_CHAR];
}CMD_GR_S_SendTrumpet;

//用户拒绝黑名单坐下
typedef struct
{
    WORD                            wTableID;
    WORD                            wChairID;
    DWORD                           dwUserID;							//用户id
    DWORD                           dwrepulseUserID;					//用户id
}CMD_GR_UserRepulseSit;


//赠送通知
typedef struct _CMD_GR_PlayTimePresentNotify
{
    DWORD							dwPlayTimeCount;					//游戏时间
    DWORD							dwPresentPlayTime;					//赠送时间
}CMD_GR_PlayTimePresentNotify;


//赠送领取
typedef struct _CMD_gr_playTimePresentDraw
{
    BYTE     cbActionGame;     //游戏操作   大厅领取0  游戏领取1
}CMD_gr_playTimePresentDraw;

/////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
typedef struct
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	INT								lMaxGameScore;						//最高分数
	INT								lMinGameScore;						//最低分数
}CMD_GR_UserRule;


/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define	SUB_GR_TABLE_STATUS			101									//桌子状态

//桌子信息
typedef struct
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[512];				//桌子状态
}CMD_GR_TableInfo;

//桌子状态
typedef struct
{
	WORD							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
}CMD_GR_TableStatus;

/////////////////////////////////////////////////////////////////////////////////
//银行命令

#define MDM_GR_INSURE					5								//用户信息

//银行命令
#define SUB_GR_ENABLE_INSURE_REQUEST            1                       //开通银行
#define SUB_GR_QUERY_INSURE_INFO                2                       //查询银行
#define SUB_GR_SAVE_SCORE_REQUEST               3                       //存款操作
#define SUB_GR_TAKE_SCORE_REQUEST               4                       //取款操作
#define SUB_GR_TRANSFER_SCORE_REQUEST           5                       //取款操作
#define SUB_GR_QUERY_USER_INFO_REQUEST          6                       //查询用户


#define SUB_GR_USER_INSURE_INFO                 100                     //银行资料
#define SUB_GR_USER_INSURE_SUCCESS              101                     //银行成功
#define SUB_GR_USER_INSURE_FAILURE              102                     //银行失败
#define SUB_GR_USER_TRANSFER_USER_INFO          103                     //用户资料
#define SUB_GR_USER_INSURE_ENABLE_RESULT        104                     //开通结果

//操作方式
#define INSURE_SAVE                     0                               //存取方式
#define INSURE_TRANSFER                 1                               //转帐方式

//开通银行
typedef struct
{
    BYTE                            cbActivityGame;                     //游戏动作
    DWORD                           dwUserID;                           //用户id
    TCHAR                           szLogonPass[LEN_PASSWORD];          //登录密码
    TCHAR                           szInsurePass[LEN_PASSWORD];         //银行密码
    TCHAR                           szMachineID[LEN_MACHINE_ID];        //机器序列
    
}CMD_GR_C_EnableInsureRequest;


#pragma mark -
#pragma mark 客户端结构体
//查询银行
typedef struct
{
	BYTE	cbActivityGame;							//游戏动作
	TCHAR	szInsurePass[LEN_PASSWORD];				//银行密码
}CMD_GR_C_QueryInsureInfoRequest;

//存款请求
typedef struct
{
	BYTE	cbActivityGame;							//游戏动作
	SCORE	lSaveScore;								//存款数目
}CMD_GR_C_SaveScoreRequest;

//取款请求
typedef struct
{
	BYTE	cbAvtivityGame;							//游戏动作
	SCORE	lTakeScore;								//取款数目
	TCHAR	szInsurePass[LEN_PASSWORD];				//银行密码
}CMD_GR_C_TakeScoreRequest;

//查询用户
typedef struct
{
    BYTE	cbActivityGame;                         //游戏动作
    BYTE	cbByNickName;                           //昵称赠送
    TCHAR	szAccounts[LEN_ACCOUNTS];				//目标用户
}CMD_GR_C_QueryUserInfoRequest;

//转帐金币
typedef struct
{
	BYTE	cbActivityGame;							//游戏动作
	SCORE	lTransferScore;							//转帐金币
	TCHAR	szAccounts[LEN_ACCOUNTS];				//目标用户
	TCHAR	szInsurePass[LEN_PASSWORD];				//银行密码
    TCHAR   szTransRemark[LEN_TRANS_REMARK];        //转帐备注
}CMD_GR_C_TransferScoreRequest;


#pragma mark -
#pragma mark 服务器发送结构体
//银行资料
typedef struct
{
	BYTE	cbActivityGame;							//游戏动作
    BYTE    cbEnjoinTransfer;                       //转帐开关
	WORD	wRevenueTake;							//税收比例
	WORD	wRevenueTransfer;						//税收比例
    WORD	wRevenueTransferMember;                 //税收比例
	WORD	wServerID;								//房间标识
	SCORE	lUserScore;								//用户金币
	SCORE	lUserInsure;							//银行金币
	SCORE	lTransferPrerequisite;					//转帐条件
}CMD_GR_S_UserInsureInfo;

//银行成功
typedef struct	
{
	BYTE	cbActivityGame;							//游戏动作
    BYTE    cbOperateType;                          //操作类型
    DWORD   dwUserMedal;                            //用户奖牌
    DWORD   dwRecordIndex;                          //记录编号
	SCORE	lUserScore;								//用户金币
	SCORE	lUserInsure;							//银行金币
	TCHAR	szDescribrString[128];					//描述信息
}CMD_GR_S_UserInsureSuccess;		

//银行失败
typedef struct	
{
	BYTE	cbActivityGame;							//游戏动作
	INT		lErrorCode;								//错误代码
	TCHAR	szDescribeString[128];					//描述消息
}CMD_GR_S_UserInsureFailure;

//用户信息
typedef struct
{
	BYTE	cbActivityGame;							//游戏动作
	DWORD   dwTargerUserID;                         //目标用户
	TCHAR   szAccounts[LEN_ACCOUNTS];				//目标用户
}CMD_GR_S_UserTransferUserInfo;

//开通结果
typedef struct
{
    BYTE     cbActivityGame;                        //游戏动作
    BYTE     cbInsureEnabled;                       //使能标识
    TCHAR    szDescribeString[128];                 //描述消息
    
}CMD_GR_S_UserInsureEnableResult;

/////////////////////////////////////////////////////////////////////////////////

#define	MDM_GR_TASK                 6									//任务命令


#define SUB_GR_TASK_LOAD_INFO       1                                   //加载任务
#define SUB_GR_TASK_TAKE            2                                   //领取任务
#define SUB_GR_TASK_REWARD          3                                   //任务奖励
#define SUB_GR_TASK_INFO            4                                   //任务信息
#define SUB_GR_TASK_FINISH          5                                   //任务完成
#define SUB_GR_TASK_LIST            6                                   //任务列表
#define SUB_GR_TASK_RESULT          7                                   //任务结果




///////////////////////////////////////////////////////////////////////////////////

//加载任务
typedef struct
{
    DWORD              dwUserID;
    TCHAR              szPassword[LEN_PASSWORD];                        //用户密码
}CMD_GR_C_LoadTaskInfo;

//领取任务
typedef struct
{
    
    WORD               wTaskID;                                         //任务标识
    DWORD              dwUserID;                                        //用户标识
    TCHAR              szPassword[LEN_PASSWORD];                        //登录密码
    TCHAR              szMachineID[LEN_MACHINE_ID];                     //机器序列
    
}CMD_GR_C_TakeTask;

//领取奖励
typedef struct
{
    WORD                wTaskID;                                        //任务标识
    DWORD               dwUserID;                                       //用户标识
    TCHAR               szPassword[LEN_PASSWORD];                        //登录密码
    TCHAR               szMachineID[LEN_MACHINE_ID];                    //机器序列
    
    
}CMD_GR_C_TakeReward;

//任务信息
typedef struct
{
    WORD                wTaskCount;                                     //任务数量
    tagTaskStatus       TaskStatus[TASK_MAX_COUNT];                     //任务状态
    
}CMD_GR_S_TaskInfo;

//任务完成
typedef struct
{
    WORD                wFinishTaskID;                                  //任务标识
    TCHAR               szTaskName[LEN_TASK_NAME];                      //任务名称
    
}CMD_GR_S_TaskFinish;

//任务结果
typedef struct
{
    //结果信息
    bool                bSuccessed;                                     //成功标识
    WORD                wCommandID;                                     //命令标识
    
    //财富信息
    SCORE               lCurrScore;                                     //当前金币
    SCORE               lCurIngot;                                      //当前元宝
    
    //提示信息
    TCHAR               szNotifyContent[128];                           //提示内容
    
}CMD_GR_S_TaskResult;

////////////////////////////////////////////////////////////////////////////////////////////////

//兑换命令
#define MDM_GR_EXCHANGE             7                                   //兑换命令

#define  SUB_GR_EXCHANGE_LOAD_INFO          400                         //加载信息
#define  SUB_GR_EXCHANGE_PARAM_INFO         401                         //兑换参数
#define  SUB_GR_PURCHASE_MEMBER             402                         //购买会员
#define  SUB_GR_PURCHASE_RESULT             403                         //购买结果
#define  SUB_GR_EXCHANGE_SCORE_BYINGOT      404                         //兑换游戏币
#define  SUB_GR_EXCHANGE_SCORE_BYBEANS      405
#define  SUB_GR_EXCHANGE_RESULT             406                         //兑换结果

///////////////////////////////////////////////////////////////////////////////////////////////
//查询参数
typedef struct
{
    DWORD							wExchangeRate;						//元宝游戏币兑换比率
    DWORD							wPresentExchangeRate;				//魅力游戏币兑换率
    DWORD							wRateGold;							//游戏豆游戏币兑换率
    WORD							wMemberCount;						//会员数目
    tagMemberParameter				MemberParameter[10];				//会员参数
}CMD_GR_ExchangeParameter;


//购买会员
typedef struct
{
    DWORD                           dwUserID;                           //用户标识
    BYTE                            cbMemberOrder;                      //会员标识
    WORD                            wPurchaseTime;                      //购买时间
    TCHAR                           szMachineID[LEN_MACHINE_ID];        //机器标识
}CMD_GR_PurchaseMember;


//购买结果
typedef struct
{
    bool                            bSuccessed;                         //成功标识
    BYTE                            cbMemberOrder;                      //会员系列
    SCORE                           lCurrScore;                         //当前游戏币
    DWORD                           dCurrBeans;                         //当前游戏豆
    TCHAR                           szNotifyContent[128];               //提示内容
}CMD_GR_PurchaseResult;

//兑换游戏比
typedef struct
{
    DWORD                           dwUserID;                           //用户标识
    SCORE                           lExchangeIngot;                     //兑换元宝
    TCHAR                           szMachineID[LEN_MACHINE_ID];        //机器标识
}CMD_GR_ExchangeScore;

//兑换游戏币
typedef struct
{
    
    DWORD							dwUserID;							//用户标识
    double							dExchangeBean;						//兑换元宝
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}CMD_GR_ExchangeScoreByBean;

//兑换结果
typedef struct
{
    bool                            bSuccessed;                         //成功标识
    SCORE                           lCurrScore;                         //当前游戏币
    SCORE                           lCurIngot;                          //当前元宝
    TCHAR                           szNotifyContent[128];               //提示内容
    
}CMD_GR_ExchangeResult;

/////////////////////////////////////////////////////////////////////////////////
//管理命令

//管理命令
#define      MDM_GR_MANAGE          8                                   //管理命令

#define      SUB_GR_SEND_WARNING    1                                   //发送警告
#define      SUB_GR_SEND_MESSAGE    2                                   //发送消息
#define      SUB_GR_LOOK_USER_IP    3                                   //查看地址
#define      SUB_GR_KILL_USER       4                                   //剔出用户
#define      SUB_GR_LIMIT_ACCOUNTS  5                                   //禁止帐户
#define      SUB_GR_SET_USER_RIGHT  6                                   //权限设置


//房间设置
#define      SUB_GR_QUERY_OPTION    7                                   //查询设置
#define      SUB_GR_OPTION_SERVER   8                                   //房间设置
#define      SUB_GR_OPTION_CURRENT  9                                   //当前设置
#define      SUB_GR_LIMIT_USER_CHAT 10                                  //限制聊天
#define      SUB_GR_KICK_ALL_USER   11                                  //剔出用户
#define      SUB_GR_DISMISSGAME     12                                  //解算游戏

//...
//发送警告
typedef struct
{
    WORD                            wChatLength;                        //信息长度
    DWORD                           dwTargerUserID;                     //目标用户
    TCHAR                           szWarningMessage[LEN_USER_CHAT];    //警告消息
    
}CMD_GR_SendWarning;

//系统消息
typedef struct
{
	BYTE                            cbGame;                             //游戏消息
	BYTE                            cbRoom;                             //游戏消息
	BYTE                            cbAllRoom;                          //游戏消息
	BYTE                            cbLoop;                             //循环标识
	DWORD                           dwTimeRate;                         //循环间隔
	INT								lConcludeTime;						//结束时间
	WORD                            wChatLength;                        //信息长度
	TCHAR                           szSystemMessage[LEN_USER_CHAT];     //系统消息
    
}CMD_GR_SendMessage;

//查看地址
typedef struct
{
    DWORD                           dwTargerUserID;
    
}CMD_GR_LookUserIP;


//剔出用户
typedef struct
{
    DWORD                           dwTargerUserID;
}CMD_GR_KickUser;

//禁用帐户
typedef struct
{
    DWORD                           dwTargerUserID;
    
}CMD_GR_limitAccounts;

//权限设置
typedef struct
{
    //目标用户
    WORD                            dwTargerUserID;
    
    //绑定变量
    BYTE                            cbGameRight;                            //帐号权限
    BYTE                            cbAccountsRight;                        //帐号权限
    
    //权限变化
    BYTE                            cbLimitRoomChat;                        //大厅聊天
    BYTE                            cbLimitGameChat;                        //游戏聊天
    BYTE                            cbLimitPlayGame;                        //游戏权限
    BYTE                            cbLimitSendWisper;                      //发送消息
    BYTE                            cbLimitLookonGame;                      //旁观权限
    
}CMD_GR_SetUserRight;

//房间设置
typedef struct
{
    DWORD                           dwRuleMask;                         //规则掩码
    tagServerOptionInfo             ServerOptionInfo;                   //房间配置
}CMD_GR_OptionCurrent;

//房间设置
typedef struct
{
    tagServerOptionInfo             ServerOptionInfo;                   //房间配置
}CMD_GR_ServerOption;

//剔出所有用户
typedef struct
{
    TCHAR                           szKickMessage[LEN_USER_CHAT];       //剔出提示
}CMD_GR_KickAllUser;

typedef struct
{
    WORD                            wDissmissTableNum;                  //解算桌号
}CMD_GR_DissmissGame;

//设置标识
#define  OSF_ROOM_CHAT              1                                   //大厅聊天
#define  OSF_GAME_CHAT              2                                   //游戏聊天
#define  OSF_ROOM_WISPER            3                                   //大厅私聊
#define  OSF_ENTER_TABLE            4                                   //进入游戏
#define  OSF_ENTER_SERVER           5                                   //进入房间
#define  OSF_SEND_BUGLE             12                                  //发送喇叭


//房间设置
typedef struct
{
    BYTE                              cbOptionFlags;                    //设置标识
    BYTE                              cbOptionValue;                    //设置标识
    
}CMD_GR_OPtionServer;

//限制聊天
typedef struct
{
    DWORD                              dwTargerUserID;
    BYTE                                cbLimitFlags;                   //限制标志
    BYTE                                cbLimitValue;                   //限制与否
    
}CMD_GR_LimitUserChat;



////////////////////////////////////////////////////////////////////////////////////////////
//比赛命令
#define MDM_GR_MATCH				9									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_DESC			408									//比赛描述
#define SUB_GR_MATCH_GOLDUPDATE     409                                 //金币更新
#define SUB_GR_MATCH_ELIMINATE      410                                 //比赛淘汰

typedef struct _CMD_GR_Match_Fee
{
    SCORE                           lMatchFee;                          //报名费用
    TCHAR                           szNotifyContent[128];               //提示内容
}CMD_GR_Match_Fee;

//比赛人数
typedef struct _CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
}CMD_GR_Match_Num;

//赛事信息
typedef struct _CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//信息标题
    WORD							wGameCount;							//游戏局数
}CMD_GR_Match_Info;

//提示信息
typedef struct _CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
    WORD                            wCurGameCount;                      //当前局数
    WORD                            wGameCount;                         //总共局数
	WORD							wPlayingTable;						//游戏桌数
    TCHAR							szMatchName[LEN_SERVER];			//比赛名称
}CMD_GR_Match_Wait_Tip;

//比赛结果
typedef struct _CMD_GR_MatchResult
{
    SCORE                           lGold;								//金币奖励
    DWORD							dwIngot;							//元宝奖励
    DWORD							dwExperience;						//经验奖励
    TCHAR							szDescribe[256];					//得奖描述
}CMD_GR_MatchResult;

#define MAX_MATCH_DESC				4									//最多描述
//比赛描述
typedef struct _CMD_GR_MatchDesc
{
    TCHAR							szTitle[MAX_MATCH_DESC][16];		//信息标题
    TCHAR							szDescribe[MAX_MATCH_DESC][64];		//描述内容
    DWORD                           crTitleColor;						//标题颜色
    DWORD                           crDescribeColor;					//描述颜色
}CMD_GR_MatchDesc;

//金币更新
typedef struct _CMD_GR_MatchGoldUpdate
{
    SCORE                             lCurrScore;                       //当前金币
    SCORE                             lCurIngot;                        //当前元宝
    DWORD                             dwCurrExprience;                  //当前经验
}CMD_GR_MatchGoldUpdate;


////////////////////////////////////////////////////////////////////////////////
//...

///////////////////////////////////////////////////////////////////////////////
//框架命令

#define	MDM_GF_FRAME				100									//框架命令

//用户命令
#define SUB_GF_GAME_OPTION			1									//游戏配置
#define SUB_GF_USER_READY			2									//用户准备
#define SUB_GF_LOOKON_CONFIG		3									//旁观配置
#define SUB_GF_DISTRIBUTE_AFFIRM	4									//分组确认

//聊天命令
#define SUB_GF_USER_CHAT			10									//用户聊天
#define SUB_GF_USER_EXPRESSION		11									//用户表情

//游戏信息
#define SUB_GF_GAME_STATUS			100									//游戏状态
#define SUB_GF_GAME_SCENE			101									//旁观场景
#define SUB_GF_LOOKON_STATUS		102									//旁观状态

//系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									//系统消息
#define SUB_GF_ACTION_MESSAGE		201									//动作消息

/////////////////////////////////////////////////////////////////////////////////

//游戏配置
typedef struct
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
}CMD_GF_GameOption;

//旁观配置
typedef struct
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
}CMD_GF_LookonConfig;

//旁观状态
typedef struct
{
	BYTE							cbAllowLookon;						//允许旁观
}CMD_GF_LookonStatus;

//游戏环境
typedef struct
{
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
}CMD_GF_GameStatus;

//用户聊天
typedef struct
{
    WORD                            wChatLength;
    DWORD                           dwChatColor;
    DWORD                           dwTargerUserID;
    TCHAR                           szChatString[LEN_USER_CHAT];
}CMD_GF_C_UserChat;

typedef struct
{
    WORD                            wChatLength;
    DWORD                           dwChatColor;
    DWORD                           dwSendUserID;
    DWORD                           dwTargerUserID;
    TCHAR                           szChatString[LEN_USER_CHAT];
}CMD_GF_S_UserChat;

//用户表情
typedef struct
{
    WORD                            wItemIndex;
    DWORD                           dwTargerUserID;
    
}CMD_GF_C_UserExpression;

typedef struct
{
    WORD                            wItemIndex;
    DWORD                           dwSendUserID;
    DWORD                           dwTargerUserID;
}CMD_GF_S_UserExpression;

//...

/////////////////////////////////////////////////////////////////////////////////

//游戏命令
#define MDM_GF_GAME					200									//游戏命令

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像


/////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

#define REQUEST_FAILURE_ACCOUNTS	7									//昵称错误

#pragma pack()

#endif
