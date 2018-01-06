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
typedef struct _stCmdGrLogonUserID
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本
	
	//登录信息
	DWORD							dwUserID;							//用户id
	TCHAR							szLogonPwd[LEN_MD5];				//登录密码
    TCHAR                           szGameRoomPwd[LEN_PWD];				//房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    WORD                            wGameTypeID;                        //游戏类型索引
}ST_CMD_GR_LOGON_USER_ID, *PST_CMD_GR_LOGON_USER_ID;

//手机登录
typedef struct _stCmdGrLogonMobile
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
	TCHAR							szDynamicPwd[LEN_MD5];				//登录密码
    TCHAR                           szServerPwd[LEN_PWD];				//房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}ST_CMD_GR_LOGON_MOBILE, *PST_CMD_GR_LOGON_MOBILE;

//帐号登录
typedef struct _stCmdGrLogonAccounts
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本
	
	//登录信息
	TCHAR							szAccounts[LEN_ACCOUNT];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GR_LOGON_ACCOUNTS, *PST_CMD_GR_LOGON_ACCOUNTS;

//登录成功
typedef struct _stCmdGrLogonSucc
{
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
}ST_CMD_GR_LOGON_SUCC, *PST_CMD_GR_LOGON_SUCC;

//登录失败
typedef struct _stCmdGrLogonFailure
{
	INT								lErrorCode;							//错误代码
	TCHAR							szDescription[128];					//错误描述
}ST_CMD_GR_LOGON_FAILURE, *PST_CMD_GR_LOGON_FAILURE;

//登录完成
typedef struct _stCmdGrLogonFinish
{
    bool                            bGuideTask;                        //引导任务
}ST_CMD_GR_LOGON_FINISH, *PST_CMD_GR_LOGON_FINISH;

//升级提示
typedef struct _stCmdUpdateNotify
{	
	//升级标志
	BYTE							cbMustUpdatePlaza;					//强行升级
	BYTE							cbMustUpdateClient;					//强行升级
	BYTE							cbAdviceUpdateClient;				//建议升级
	
	//当前版本
	DWORD							dwCurrentPlazaVersion;				//当前版本
	DWORD							dwCurrentFrameVersion;				//当前版本
	DWORD							dwCurrentClientVersion;				//当前版本
}ST_CMD_GR_UPDATE_NOTIFY, *PST_CMD_GR_UPDATE_NOTIFY;

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
typedef struct _stCmdGrConfigColumn
{
	BYTE							cbColumnCount;						//列表数目
	_stColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
}ST_CMD_GR_CONFIG_COLUMN, *PST_CMD_GR_CONFIG_COLUMN;

//房间配置
typedef struct _stCmdGrConfigGameRoom
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目
	
	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
}ST_CMD_GR_CONFIG_GAME_ROOM, *PST_CMD_GR_CONFIG_GAME_ROOM;

//道具配置
typedef struct _stCmdGrConfigProperty
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPORTY];			//道具描述
}ST_CMD_GR_CONFIG_PROPERTY, *PST_CMD_GR_CONFIG_PROPERTY;

//玩家权限
typedef struct _stCmdGrConfigUserPriview
{
    DWORD                           dwUserRight;                        //玩家权限
}ST_CMD_GR_CONFIG_USER_PRIVIEW, *PST_CMD_GR_CONFIG_USER_PRIVIEW;

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
typedef struct _stCmdGrUserLookon
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
}ST_CMD_GR_USER_LOOK_ON, *PST_CMD_GR_USER_LOOK_ON;

//坐下请求
typedef struct _stCmdGrUserSitDown
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	TCHAR							szPwd[LEN_PWD];						//桌子密码
}ST_CMD_GR_USER_SIT_DOWN, *PST_CMD_GR_USER_SIT_DOWN;

//起立请求
typedef struct _stCmdGrUserStandUp
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
}ST_CMD_GR_USER_STAND_UP, *PST_CMD_GR_USER_STAND_UP;

//邀请用户
typedef struct _stCmdGrUserInvite
{
    WORD                            wTableID;                           //桌号码
    DWORD                           dwUserID;                           //用户id
}ST_CMD_GR_USER_INVITE, *PST_CMD_GR_USER_INVITE;

//邀请用户请求
typedef struct _stCmdGrUserInviteReq
{
    WORD                            wTableID;                           //桌号码
    DWORD                           dwUserID;                           //用户id
}ST_CMD_GR_USER_INVITE_REQ, *PST_CMD_GR_USER_INVITE_REQ;

//请求用户信息
typedef struct _stCmdGrUserInfoReq
{
    DWORD							dwUserIDReq;						//请求用户
    WORD							wTablePos;							//桌子位置
}ST_CMD_GR_USER_INFO_REQ, *PST_CMD_GR_USER_INFO_REQ;

//请求椅子信息
typedef struct _stCmdGrUserInfoChairReq
{
    WORD							wTableID;							//桌子号码
    WORD							wChairID;							//椅子号码
}ST_CMD_GR_USER_INFO_CHAIR_REQ, *PST_CMD_GR_USER_INFO_CHAIR_REQ;

//用户分数
typedef struct _stCmdGrUserScore
{
	DWORD							dwUserID;							//用户标识
	_stUserScore					stUserScore;						//积分信息
}ST_CMD_GR_USER_SCORE, *PST_CMD_GR_USER_SCORE;

#pragma mark -
#pragma mark 用户分数
typedef struct _stCmdGrMobileUserScore
{
	DWORD							dwUserID;							//用户标识
	_stMobileUserScore				stUserScore;						//积分信息
}ST_CMD_GR_MOBILE_USER_SCORE, *PST_CMD_GR_MOBILE_USER_SCORE;

//用户状态
typedef struct _stCmdGrUserStatus
{
	DWORD							dwUserID;							//用户标识
	_stUserStatus					stUserStatus;						//用户状态
}ST_CMD_GR_USER_STATUS, *PST_CMD_GR_USER_STATUS;

//请求失败
typedef struct _stCmdGrReqFailure
{
	INT								nErrorCode;							//错误代码
	TCHAR							szDes[256];							//描述信息
}ST_CMD_GR_REQ_FAILURE, *PST_CMD_GR_REQ_FAILURE;

//用户聊天
typedef struct _stCmdGrUserClientChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargerUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}ST_CMD_GR_USER_CLIENT_CHAT, *PST_CMD_GR_USER_CLIENT_CHAT;

//用户聊天
typedef struct _stCmdGrUserServerChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}ST_CMD_GR_USER_SERVER_CHAT, *PST_CMD_GR_USER_SERVER_CHAT;

//用户表情
typedef struct _stCmdGrUserClientExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
}ST_CMD_GR_USER_CLIENT_EXPRESSION, *PST_CMD_GR_USER_CLIENT_EXPRESSION;

//用户表情
typedef struct _stCmdGrUserServerExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
}ST_CMD_GR_USER_SERVER_EXPRESSION, *PST_CMD_GR_USER_SERVER_EXPRESSION;

//用户私聊
typedef struct _stCmdGrUserClientWisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
    DWORD                           dwSendUserID;                       //发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}ST_CMD_GR_USER_CLIENT_WISPER_CHAT, *PST_CMD_GR_USER_CLIENT_WISPER_CHAT;

//用户私聊
typedef struct _stCmdGrUserServerWisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}ST_CMD_GR_USER_SERVER_WISPER_CHAT, *PST_CMD_GR_USER_SERVER_WISPER_CHAT;

//私聊表情
typedef struct _stCmdGrUserClientWisperExpression
{
	WORD							wItemIndex;							//表情索引
    DWORD                           dwSendUserID;                       //发送用户
	DWORD							dwTargetUserID;						//目标用户
}ST_CMD_GR_USER_CLIENT_WISPER_EXPRESSION, *PST_CMD_GR_USER_CLIENT_WISPER_EXPRESSION;

//私聊表情
typedef struct _stCmdGrUserServerWisperExpression
{
	DWORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
}ST_CMD_GR_USER_SERVER_WISPER_EXPRESSION, *PST_CMD_GR_USER_SERVER_WISPER_EXPRESSION;

//用户会话
typedef struct _stCmdGrGrColloquyChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwConversationID;					//会话标识
	DWORD							dwTargetUserID[16];					//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
}ST_CMD_GR_GR_COLLOQUY_CHAT, *PST_CMD_GR_GR_COLLOQUY_CHAT;

//邀请用户
typedef struct _stCmdGrUserClientInviteUser
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
}ST_CMD_GR_USER_CLIENT_INVITE_USER, *PST_CMD_GR_USER_CLIENT_INVITE_USER;

//邀请用户
typedef struct _stCmdGrUserServerInviteUser
{
	DWORD							dwTargetUserID;						//目标用户
}ST_CMD_GR_USER_SERVER_INVITE_USER, *PST_CMD_GR_USER_SERVER_INVITE_USER;

//购买道具
typedef struct _stCmdGrClientBuyProperty
{
    BYTE                            cbRequestArea;                      //请求范围
    BYTE                            cbConsumScore;                      //积分消费
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwTargetUserID;						//使用对象
}ST_CMD_GR_CLIENT_BUY_PROPERTY, *PST_CMD_GR_CLIENT_BUY_PROPERTY;

//道具成功
typedef struct _stCmdGrServerPropertySucc
{
    BYTE							cbRequestArea;						//使用环境
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//目标对象
    DWORD							dwTargetUserID;						//使用对象
}ST_CMD_GR_SERVER_PROPERTY_SUCC, *PST_CMD_GR_SERVER_PROPERTY_SUCC;

//道具失败
typedef struct _stCmdGrServerPropertyFailure
{
    WORD							wReqArea;							//请求区域
	INT								lErrorCode;							//错误代码
	TCHAR               			szDes[256];							//描述信息
}ST_CMD_GR_SERVER_PROPERTY_FAILURE, *PST_CMD_GR_SERVER_PROPERTY_FAILURE;

//道具消息
typedef struct _stCmdGrServerPropertyMsg
{
	WORD                			wPropertyIndex;						//道具索引
	WORD                			wPropertyCount;						//道具数目
	DWORD               			dwSourceUserID;						//目标对象
	DWORD               			dwTargetUserID;						//使用对象
}ST_CMD_GR_SERVER_PROPERTY_MSG, *PST_CMD_GR_SERVER_PROPERTY_MSG;

//道具效应
typedef struct _stCmdGrServerPropertyEffect
{
	DWORD               			wUserID;							//用户I D
	BYTE                			cbVipLevel;							//会员等级
}ST_CMD_GR_SERVER_PROPERTY_EFFECT, *PST_CMD_GR_SERVER_PROPERTY_EFFECT;

//发送喇叭
typedef struct _stCmdGrClientSendTrumpet
{
    BYTE							cbRequestArea;
    WORD							wPropertyIndex;
    DWORD							TrumpetColor;
    TCHAR							szTrumpetContent[TRUMPET_MAX_CHAR];
}ST_CMD_GR_CLIENT_SEND_TRUMPET, *PST_CMD_GR_CLIENT_SEND_TRUMPET;

//发送喇叭
typedef struct _stCmdGrServerSendTrumpet
{
	WORD							wPropertyIndex;
	DWORD							dwSendUserID;
	DWORD							TrumpetColor;
	TCHAR							szSendNickName[32];
	TCHAR							szTrumpetContent[TRUMPET_MAX_CHAR];
}ST_CMD_GR_SERVER_SEND_TRUMPET, *PST_CMD_GR_SERVER_SEND_TRUMPET;

//用户拒绝黑名单坐下
typedef struct _stCmdGrUserRepulseSit
{
    WORD							wTableID;
    WORD							wChairID;
	DWORD							dwUserID;							//用户id
	DWORD							dwrepulseUserID;					//用户id
}ST_CMD_GR_USER_REPULSE_SIT, *PST_CMD_GR_USER_REPULSE_SIT;

//赠送通知
typedef struct _stCmdGrPlayTimePresentNotify
{
	DWORD							dwPlayTimeCount;					//游戏时间
	DWORD							dwPresentPlayTime;					//赠送时间
}ST_CMD_GR_PLAY_TIME_PRESENT_NOTIFY, *PST_CMD_GR_PLAY_TIME_PRESENT_NOTIFY;

//赠送领取
typedef struct _stCmdGrPlayTimePresentDraw
{
    BYTE							cbActionGame;						//游戏操作-0:大厅领取 | 1:游戏领取
}ST_CMD_GR_PLAY_TIME_PRESENT_DRAW, *PST_CMD_GR_PLAY_TIME_PRESENT_DRAW;

/////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
typedef struct _stCmdGrUserRule
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	INT								lMaxGameScore;						//最高分数
	INT								lMinGameScore;						//最低分数
}ST_CMD_GR_USER_RULE, *PST_CMD_GR_USER_RULE;

/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define	SUB_GR_TABLE_STATUS			101									//桌子状态

//桌子信息
typedef struct _stCmdGrTableInfo
{
	WORD							wTableCount;						//桌子数目
	_stTableStatus					stTableStatusArray[512];			//桌子状态
}ST_CMD_GR_TABLE_INFO, *PST_CMD_GR_TABLE_INFO;

//桌子状态
typedef struct _stCmdGrTableStatus
{
	WORD							wTableID;							//桌子号码
	_stTableStatus					stTableStatus;						//桌子状态
}ST_CMD_GR_TABLE_STATUS, *PST_CMD_GR_TABLE_STATUS;

/////////////////////////////////////////////////////////////////////////////////
//银行命令

#define MDM_GR_INSURE							5						//用户信息

//银行命令
#define SUB_GR_ENABLE_INSURE_REQUEST            1						//开通银行
#define SUB_GR_QUERY_INSURE_INFO                2						//查询银行
#define SUB_GR_SAVE_SCORE_REQUEST               3						//存款操作
#define SUB_GR_TAKE_SCORE_REQUEST               4						//取款操作
#define SUB_GR_TRANSFER_SCORE_REQUEST           5						//取款操作
#define SUB_GR_QUERY_USER_INFO_REQUEST          6						//查询用户


#define SUB_GR_USER_INSURE_INFO                 100						//银行资料
#define SUB_GR_USER_INSURE_SUCCESS              101						//银行成功
#define SUB_GR_USER_INSURE_FAILURE              102						//银行失败
#define SUB_GR_USER_TRANSFER_USER_INFO          103						//用户资料
#define SUB_GR_USER_INSURE_ENABLE_RESULT        104						//开通结果

//操作方式
#define INSURE_SAVE								0						//存取方式
#define INSURE_TRANSFER							1						//转帐方式

//开通银行
typedef struct _stCmdGrClientEnableInsureReq
{
    BYTE							cbActivityGame;						//游戏动作
    DWORD							dwUserID;							//用户id
    TCHAR							szLogonPwd[LEN_PWD];				//登录密码
    TCHAR							szInsurePwd[LEN_PWD];				//银行密码
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
}ST_CMD_GR_CLIENT_ENABLE_INSURE_REQ, *PST_CMD_GR_CLIENT_ENABLE_INSURE_REQ;

#pragma mark -
#pragma mark 客户端结构体
//查询银行
typedef struct _stCmdGrClientQueryInsureReq
{
	BYTE							cbActivityGame;						//游戏动作
	TCHAR							szInsurePwd[LEN_PWD];				//银行密码
}ST_CMD_GR_CLIENT_QUERY_INSURE_INFO_REQ, *PST_CMD_GR_CLIENT_QUERY_INSURE_INFO_REQ;

//存款请求
typedef struct _stCmdGrClientSaveGoldReq
{
	BYTE							cbActivityGame;						//游戏动作
	LONGLONG						lSaveGold;							//存款数目
}ST_CMD_GR_CLIENT_SAVE_GOLD_REQ, *PST_CMD_GR_CLIENT_SAVE_GOLD_REQ;

//取款请求
typedef struct _stCmdGrClientTakeOutGoldReq
{
	BYTE							cbAvtivityGame;						//游戏动作
	LONGLONG						lTakeOutGold;						//取款数目
	TCHAR							szInsurePwd[LEN_PWD];				//银行密码
}ST_CMD_GR_CLIENT_TAKE_OUT_GOLD_REQ, *PST_CMD_GR_CLIENT_TAKE_OUT_GOLD_REQ;

//查询用户
typedef struct _stCmdGrClientQueryUserInfoReq
{
    BYTE							cbActivityGame;                     //游戏动作
    BYTE							cbByNickName;                       //昵称赠送
    TCHAR							szAccounts[LEN_ACCOUNT];			//目标用户
}ST_CMD_GR_CLIENT_QUERY_USER_INFO_REQ, *PST_CMD_GR_CLIENT_QUERY_USER_INFO_REQ;

//转帐金币
typedef struct _stCmdGrClientTransferGoldReq
{
	BYTE							cbActivityGame;						//游戏动作
	LONGLONG						lTransferGold;						//转帐金币
	TCHAR							szAccounts[LEN_ACCOUNT];			//目标用户
	TCHAR							szInsurePwd[LEN_PWD];				//银行密码
	TCHAR							szTransRemark[LEN_TRANS_REMARK];    //转帐备注
}ST_CMD_GR_CLIENT_TRANSFER_GOLD_REQ, *PST_CMD_GR_CLIENT_TRANSFER_GOLD_REQ;

#pragma mark -
#pragma mark 服务器发送结构体
//银行资料
typedef struct _stCmdGrServerUserInsureInfo
{
	BYTE							cbActivityGame;						//游戏动作
	BYTE							cbEnjoinTransfer;                   //转帐开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferVip;				//税收比例
	WORD							wGameRoomID;						//房间标识
	LONGLONG						lUserGold;							//用户金币
	LONGLONG						lUserInsureGold;					//银行金币
	LONGLONG						lTransferPrerequisite;				//转帐条件
}ST_CMD_GR_SERVER_USER_INSURE_INFO, *PST_CMD_GR_SERVER_USER_INSURE_INFO;

//银行成功
typedef struct _stCmdGrServerUserInsureSucc
{
	BYTE							cbActivityGame;						//游戏动作
    BYTE							cbOperateType;                      //操作类型
    DWORD							dwUserMedal;                        //用户奖牌
    DWORD							dwRecordNo;							//记录编号
	LONGLONG						lUserGold;							//用户金币
	LONGLONG						lUserInsureGold;					//银行金币
	TCHAR							szDes[128];							//描述信息
}ST_CMD_GR_SERVER_USER_INSURE_SUCC, *PST_CMD_GR_SERVER_USER_INSURE_SUCC;

//银行失败
typedef struct _stCmdGrServerUserInsureFailure
{
	BYTE							cbActivityGame;						//游戏动作
	INT								nErrorCode;							//错误代码
	TCHAR							szDes[128];							//描述消息
}ST_CMD_GR_SERVER_USER_INSURE_FAILURE, *PST_CMD_GR_SERVER_USER_INSURE_FAILURE;

//用户信息
typedef struct _stCmdGrServerUserTransferUserInfo
{
	BYTE							cbActivityGame;						//游戏动作
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szAccounts[LEN_ACCOUNT];			//目标用户
}ST_CMD_GR_SERVER_USER_TRANSFER_USRE_INFO, *PST_CMD_GR_SERVER_USER_TRANSFER_USRE_INFO;

//开通结果
typedef struct _stCmdGrServerUserInsureEnableResult
{
	BYTE							cbActivityGame;						//游戏动作
	BYTE							cbInsureEnabled;					//使能标识
	TCHAR							szDes[128];							//描述消息
}ST_CMD_GR_SERVER_USER_INSURE_ENABLE_RESULT, *PST_CMD_GR_SERVER_USER_INSURE_ENABLE_RESULT;

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
typedef struct _stCmdGrClientLoadTaskInfo
{
    DWORD							dwUserID;
    TCHAR							szPwd[LEN_PWD];						//用户密码
}ST_CMD_GR_CLIENT_LOAD_TASK_INFO, *PST_CMD_GR_CLIENT_LOAD_TASK_INFO;

//领取任务
typedef struct _stCmdGrClientTaskGet
{
	WORD							wTaskID;                            //任务标识
	DWORD							dwUserID;                           //用户标识
    TCHAR							szLogonPwd[LEN_PWD];				//登录密码
    TCHAR							szMachineID[LEN_MACHINE_ID];        //机器序列
}ST_CMD_GR_CLIENT_TASK_GET, *PST_CMD_GR_CLIENT_TASK_GET;

//领取奖励
typedef struct _stCmdGrClientTaskReward
{
	WORD							wTaskID;                            //任务标识
	DWORD							dwUserID;                           //用户标识
	TCHAR							szLogonPwd[LEN_PWD];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];        //机器序列
}ST_CMD_GR_CLIENT_TASK_REWARD, *PST_CMD_GR_CLIENT_TASK_REWARD;

//任务信息
typedef struct _stCmdGrServerTaskInfo
{
    WORD							wTaskCount;							//任务数量
    ST_TASK_STATUS					stTaskStatusArray[TASK_MAX_COUNT];	//任务状态
}ST_CMD_GR_SERVER_TASK_INFO, *PST_CMD_GR_SERVER_TASK_INFO;

//任务完成
typedef struct _stCmdGrServerTaskFinish
{
	WORD							wFinishTaskID;                      //任务标识
	TCHAR							szTaskName[LEN_TASK_NAME];          //任务名称
}ST_CMD_GR_SERVER_TASK_FINISH, *PST_CMD_GR_SERVER_TASK_FINISH;

//任务结果
typedef struct _stCmdGrServerTaskResult
{
    //结果信息
    bool							bSucc;								//成功标识
    WORD							wCommandID;                         //命令标识
    
    //财富信息
    LONGLONG						lCurrGameCoin;                      //当前游戏币
    LONGLONG						lCurGold;							//当前金币
    
    //提示信息
    TCHAR							szNotifyContent[128];               //提示内容
}ST_CMD_GR_SERVER_TASK_RESULT, *PST_CMD_GR_SERVER_TASK_RESULT;

////////////////////////////////////////////////////////////////////////////////////////////////

//兑换命令
#define MDM_GR_EXCHANGE						7                           //兑换命令

#define  SUB_GR_EXCHANGE_LOAD_INFO          400                         //加载信息
#define  SUB_GR_EXCHANGE_PARAM_INFO         401                         //兑换参数
#define  SUB_GR_PURCHASE_MEMBER             402                         //购买会员
#define  SUB_GR_PURCHASE_RESULT             403                         //购买结果
#define  SUB_GR_EXCHANGE_GAME_COIN_BY_GOLD  404                         //金币兑换游戏币
#define  SUB_GR_EXCHANGE_GAME_COIN_BY_BEAN  405							//游戏豆兑换游戏币
#define  SUB_GR_EXCHANGE_RESULT             406                         //兑换结果

///////////////////////////////////////////////////////////////////////////////////////////////
//查询参数
typedef struct _stCmdGrExchangeParam
{
    DWORD							wGoldExchangeGameCoinRate;			//金币→游戏币兑换比率
    DWORD							wPresentExchangeGameCoinRate;		//魅力→游戏币兑换率
    DWORD							wRateBeanExchangeGameCoin;			//游戏豆→游戏币兑换率
    WORD							wVipCount;							//会员数目
    _stVipParam						stVipParamArray[10];				//会员参数
}ST_CMD_GR_EXCHANGE_PARAM, *PST_CMD_GR_EXCHANGE_PARAM;

//兑换游戏币
typedef struct _stCmdGrExchangeGameCoin
{
    DWORD                           dwUserID;                           //用户标识
    LONGLONG						lExchangedGold;                     //待兑换金币
    TCHAR                           szMachineID[LEN_MACHINE_ID];        //机器标识
}ST_CMD_GR_EXCHANGE_GAME_GOIN, *PST_CMD_GR_EXCHANGE_GAME_GOIN;

//兑换游戏币
typedef struct _stCmdGrExchangeGameCoinByBean
{
    DWORD							dwUserID;							//用户标识
    double							dExchangedBean;						//待兑换游戏豆
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
}ST_CMD_GR_EXCHANGE_GAME_COIN_BY_BEAN, *PST_CMD_GR_EXCHANGE_GAME_COIN_BY_BEAN;

//兑换结果
typedef struct _stCmdGrExchangeResult
{
    bool                            bSucc;								//成功标识
    LONGLONG						lCurrGameCoin;                      //当前游戏币
    LONGLONG						lCurGold;							//当前金币
    TCHAR                           szNotifyContent[128];               //提示内容
}ST_CMD_GR_EXCHANGE_RESULT, *PST_CMD_GR_EXCHANGE_RESULT;

//购买会员
typedef struct _stCmdGrPurchaseVip
{
	DWORD                           dwUserID;                           //用户标识
	BYTE                            cbVipOrder;							//会员标识
	WORD                            wPurchaseTime;                      //购买时间
	TCHAR                           szMachineID[LEN_MACHINE_ID];        //机器标识
}ST_CMD_GR_PURCHASE_VIP, *PST_CMD_GR_PURCHASE_VIP;

//购买结果
typedef struct _stCmdGrPurchaseResult
{
	bool                            bSucc;								//成功标识
	BYTE                            cbVipOrder;							//会员系列
	LONGLONG						lCurrGameCoin;                      //当前游戏币
	DWORD                           dCurrBean;							//当前游戏豆
	TCHAR                           szNotifyContent[128];               //提示内容
}ST_CMD_GR_PURCHASE_RESULT, *PST_CMD_GR_PURCHASE_RESULT;

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
typedef struct _stCmdGrSendWarning
{
    WORD                            wLen;								//信息长度
    DWORD                           dwUserID;							//目标用户
    TCHAR                           szWarningMsg[LEN_USER_CHAT];		//警告消息
}ST_CMD_GR_SEND_WARNING, *PST_CMD_GR_SEND_WARNING;

//系统消息
typedef struct _stCmdGrSendMsg
{
	BYTE                            cbGame;                             //游戏消息
	BYTE                            cbRoom;                             //游戏消息
	BYTE                            cbAllRoom;                          //游戏消息
	BYTE                            cbLoop;                             //循环标识
	DWORD                           dwTimeRate;                         //循环间隔
	INT								lEndTime;							//结束时间
	WORD                            wLen;								//信息长度
	TCHAR                           szSysMsg[LEN_USER_CHAT];			//系统消息
}ST_CMD_GR_SEND_MSG, *PST_CMD_GR_SEND_MSG;

//查看地址
typedef struct _stCmdGrUserLookIP
{
    DWORD                           dwUserID;
}ST_CMD_GR_USER_LOOK_IP, *PST_CMD_GR_USER_LOOK_IP;

//踢出用户
typedef struct _stCmdGrUserKickOut
{
    DWORD                           dwUserID;
}ST_CMD_GR_USER_KICK_OUT, *PST_CMD_GR_USER_KICK_OUT;

//禁用帐户
typedef struct _stCmdGrUserDisableAccounts
{
    DWORD                           dwUserID;
}ST_CMD_GR_USER_DISABLE_ACCOUNTS, *PST_CMD_GR_USER_DISABLE_ACCOUNTS;

//权限设置
typedef struct _stCmdGrUserGetPriview
{
    //目标用户
    WORD                            dwUserID;
    
    //绑定变量
    BYTE                            cbGamePriview;                      //帐号权限
    BYTE                            cbAccountsPriview;                  //帐号权限
    
    //权限变化
    BYTE                            cbLimitRoomChat;                    //大厅聊天
    BYTE                            cbLimitGameChat;                    //游戏聊天
    BYTE                            cbLimitPlayGame;                    //游戏权限
    BYTE                            cbLimitSendWisper;                  //发送消息
    BYTE                            cbLimitLookonGame;                  //旁观权限
}ST_CMD_GR_USER_SET_PRIVIEW, *PST_CMD_GR_USER_SET_PRIVIEW;

//房间设置
typedef struct _stCmdGrGameRoomSetting
{
    DWORD                           dwRuleMask;                         //规则掩码
    _stGameRoomConfig				stGameRoomConfig;                   //房间配置
}ST_CMD_GR_GAME_ROOM_SETTING, *PST_CMD_GR_GAME_ROOM_SETTING;

//游戏房间设置
typedef struct _stCmdGrGameRoomConfig
{
    _stGameRoomConfig				stGameRoomConfig;                   //房间配置
}ST_CMD_GR_GAME_ROOM_CONFIG;

//剔出所有用户
typedef struct _stCmdGrUserKickOutAll
{
    TCHAR                           szMsg[LEN_USER_CHAT];				//剔出提示
}ST_CMD_GR_USER_KICK_OUT_ALL, *PST_CMD_GR_USER_KICK_OUT_ALL;

typedef struct _stCmdGrDissmissGame
{
    WORD                            wTableNum;							//解散桌号
}ST_CMD_GR_DISSMISS_GAME, *PST_CMD_GR_DISSMISS_GAME;

//设置标识
#define  OSF_ROOM_CHAT              1                                   //大厅聊天
#define  OSF_GAME_CHAT              2                                   //游戏聊天
#define  OSF_ROOM_WISPER            3                                   //大厅私聊
#define  OSF_ENTER_TABLE            4                                   //进入游戏
#define  OSF_ENTER_SERVER           5                                   //进入房间
#define  OSF_SEND_BUGLE             12                                  //发送喇叭

//房间设置
typedef struct _stCmdGrServerGameRoomConfig
{
	BYTE                            cbFlags;							//设置标识
	BYTE                            cbValue;							//设置值
}ST_CMD_GR_SERVER_GAME_ROOM_CONFIG, *PST_CMD_GR_SERVER_GAME_ROOM_CONFIG;

//限制聊天
typedef struct _stCmdGrUserLimitChat
{
	DWORD                           dwUserID;
	BYTE                            cbFlags;							//限制标志
	BYTE                            cbValue;							//限制与否
}ST_CMD_GR_USER_LIMIT_CHAT, *PST_CMD_GR_USER_LIMIT_CHAT;

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
#define SUB_GR_MATCH_GOLD_UPDATE     409                                //金币更新
#define SUB_GR_MATCH_ELIMINATE      410                                 //比赛淘汰

//比赛费用
typedef struct _stCmdGrMatchFee
{
    LONGLONG                       lSignUpFee;                         //报名费用
    TCHAR                           szNotifyContent[128];               //提示内容
}ST_CMD_GR_MATCH_FEE, *PST_CMD_GR_MATCH_FEE;

//比赛人数
typedef struct _stCmdGrMatchUserNum
{
	DWORD							dwWaittingNum;						//等待人数
	DWORD							dwTotalNum;							//开赛人数
}ST_CMD_GR_MATCH_USER_NUM, *PST_CMD_GR_MATCH_USER_NUM;

//赛事信息
typedef struct _stCmdGrMatchInfo
{
	TCHAR							szTitle[4][64];						//信息标题
    WORD							wGameCount;							//游戏局数
}ST_CMD_GR_MATCH_INFO, *PST_CMD_GR_MATCH_INFO;

//提示信息
typedef struct _stCmdGrMatchWaitTip
{
	LONGLONG						lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
    WORD                            wCurGameCount;                      //当前局数
    WORD                            wGameCount;                         //总共局数
	WORD							wPlayingTable;						//游戏桌数
    TCHAR							szMatchName[LEN_SERVER];			//比赛名称
}ST_CMD_GR_MATCH_WAIT_TIP, *PST_CMD_GR_MATCH_WAIT_TIP;

//比赛结果
typedef struct _stCmdGrMatchResult
{
    LONGLONG						llGameCoin;							//游戏币奖励
    DWORD							dwGold;								//金币奖励
    DWORD							dwExp;								//经验奖励
    TCHAR							szDes[256];							//得奖描述
}ST_CMD_GR_MATCH_RESULT, *PST_CMD_GR_MATCH_RESULT;

#define MAX_MATCH_DES				4									//最多描述

//比赛描述
typedef struct _stCmdGrMatchDes
{
    TCHAR							szTitle[MAX_MATCH_DES][16];			//信息标题
    TCHAR							szDes[MAX_MATCH_DES][64];			//描述内容
    DWORD                           crTitleColor;						//标题颜色
    DWORD                           crDesColor;							//描述颜色
}ST_CMD_GR_MATCH_DES, *PST_CMD_GR_MATCH_DES;

//金币更新
typedef struct _stCmdGrMatchGoldUpdate
{
    LONGLONG                       llCurrGameCoin;						//当前游戏币
    LONGLONG                       llCurGold;							//当前金币
    DWORD                          dwCurrExp;							//当前经验
}ST_CMD_GR_MATCH_GOLD_UPDATE, *PST_CMD_GR_MATCH_GOLD_UPDATE;

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
typedef struct _stCmdGfGameConfig
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
}ST_CMD_GF_GAME_CONFIG, *PST_CMD_GF_GAME_CONFIG;

//旁观配置
typedef struct _stCmdGfLookOnConfig
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
}ST_CMD_GF_LOOK_ON_CONFIG, *PST_CMD_GF_LOOK_ON_CONFIG;

//旁观状态
typedef struct _stCmdGfLookOnStatus
{
	BYTE							cbAllowLookon;						//允许旁观
}ST_CMD_GF_LOOK_ON_STATUS, *PST_CMD_GF_LOOK_ON_STATUS;

//游戏环境
typedef struct _stCmdGfGameStatus
{
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
}ST_CMD_GF_GAME_STATUS, *PST_CMD_GF_GAME_STATUS;

//用户聊天
typedef struct _stCmdGfUserClientChat
{
	DWORD                           dwUserID;							//用户ID
    WORD                            wLen;
    DWORD                           dwChatColor;						//聊天背景
    TCHAR                           szChatString[LEN_USER_CHAT];
}ST_CMD_GF_USER_CLIENT_CHAT, *PST_CMD_GF_USER_CLIENT_CHAT;

typedef struct _stCmdGfUserServerChat
{
    WORD                            wLen;
    DWORD                           dwChatColor;
    DWORD                           dwSendUserID;
	DWORD                           dwRecvUserID;
    TCHAR                           szChatString[LEN_USER_CHAT];
}ST_CMD_GF_USER_SERVER_CHAT, *PST_CMD_GF_USER_SERVER_CHAT;

//用户表情
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

//...
/////////////////////////////////////////////////////////////////////////////////

//游戏命令
#define MDM_GF_GAME					200									//游戏命令

//其他信息
#define DTP_GR_TABLE_PWD			1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

/////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQ_FAILURE_NORMAL			0									//常规原因
#define REQ_FAILURE_NO_GOLD			1									//金币不足
#define REQ_FAILURE_NO_GAME_COIN	2									//游戏币不足
#define REQ_FAILURE_PWD				3									//密码错误
#define REQ_FAILURE_ACCOUNTS		7									//昵称错误

#pragma pack()

#endif