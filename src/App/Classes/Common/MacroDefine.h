/************************************************************************************
 * file: 		MacroDefine.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 22:52:04
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __MACRO_DEFINE_H__
#define __MACRO_DEFINE_H__

#include "cocos2d.h"

#pragma pack(1)

//对应数据类型
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
//typedef unsigned int		DWORD;

//typedef WORD				TCHAR;
//typedef DWORD				UINT;
typedef DWORD               COLORREF;

typedef	long long			LONGLONG;
typedef long long			SCORE;
typedef double              DOUBLE;
typedef short               SHORT;
typedef int                 INT;
typedef float               FLOAT;

typedef long                LONG;

typedef const char c_char;
typedef const char* pConstChar;

typedef struct _stSystemTime
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}SYSTEM_TIME;

//兼容PC函数
#define ASSERT(e)                       assert(e)
#define CopyMemory(des,src,size_t)      memcpy(des,src,size_t)
#define ZeroMemory(des,size)            memset(des,0,size)
#define VERIFY(e)                       assert(e)


#pragma mark 数值定义

//头像大小
#define	FACE_CX				48						//头像宽度
#define FACE_CY				48						//头像高度

//上传头像大小
#define FACE_SCX            256                     //上传头像宽度
#define FACE_SCY            48                      //上传头像高度

//长度定义
#define	LEN_LESS_ACCOUNTS	6						//最短帐号
#define LEN_LESS_NICKNAME	6						//最短昵称
#define LED_LESSPASSWORD	6						//最短密码

//人数定义
#define MAX_CHAIR			100						//最大椅子
#define	MAX_TABLE			512						//最大桌子
#define	MAX_COLUMN			32						//最大列表
#define MAX_BATCH           48                      //最大批次
#define	MAX_ANDROID			256						//最大机器
#define	MAX_PROPORTY		128						//最大道具
#define	MAX_WHISPER_USER	16						//最大私聊
#define MAX_CHAIR_GENERAL   8                       //最大椅子

//裂表定义
#define MAX_KIND			256						//最大类型
#define MAX_SERVER			1024					//最大房间

//参数定义
#define	INVALID_CHAIR		0xFFFF					//无效椅子
#define	INVALID_TABLE		0xFFFF					//无效桌子
#define INVALID_USERID		0						//无效用户

//税收起点
#define	REVENUE_BENCHMARK	0L						//税收起点
#define	REVENUE_DENOMINATOR	1000L					//税收分母


#pragma mark 系统参数
//游戏状态
#define	GAME_STATUS_FREE	0						//空闲状态
#define	GAME_STATUS_PLAY	100						//游戏状态
#define GAME_STATUS_WAIT	200						//等待状态
#define GAME_STATUS_ENDED   255                     //结束状态

//系统参数
#define LEN_USER_CHAT		128						//聊天长度
#define	TIME_USER_CHAT		1L						//聊天间隔
#define TRUMPET_MAX_CHAR    128                     //喇叭长度
	

#pragma mark 索引质数
//列表质数
#define PRIME_TYPE			11L						//种类数目
#define PRIME_KIND			53L						//类型数目
#define PRIME_NODE			101L					//节点数目
#define	PRIME_PAGE			53L						//自定树木
#define PRIME_SERVER		1009L					//房间树木

//人物质数
#define PRIME_SERVER_USER	503L					//房间人数
#define PRIME_ANDROID_USER	503L					//机器人数
#define PRIME_PLATFORM_USER	100003L					//平台人数

//平台ID
#define PLATFORM_Account 1
#define PLATFORM_Visitor 2
#define PLATFORM_Sina    3
#define PLATFORM_Tencent 4
#define PLATFORM_RenRen  5

//房间类型
#define GAME_GENER_DEFAULT  0x0000
#define GAME_GENRE_GOLD     0x0001					//金币类型
#define GAME_GENRE_SCORE    0x0002					//点值类型
#define GAME_GENRE_MATCH    0x0004					//比赛类型
#define GAME_GENRE_EDUCATE  0x0008					//训练类型

#define SERVER_GENRE_NORMAL  0X0001					//普通类型
#define SERVER_GENRE_PASSWD  0X0002					//密码类型


#pragma mark 数据长度
//资料数据
#define LEN_USER_UIN        33 
#define LEN_MD5				33						//加密密码
#define LEN_ACCOUNTS		32						//备注长度
#define LEN_NICENAME		32						//帐号长度	
#define LEN_NICKNAME		32						//昵称长度
#define LEN_PASSWORD		33						//密码长度	
#define LEN_GROUP_NAME		32						//社团名字	
#define LEN_UNDER_WRITE		32						//个性签名

//数据长度
#define LEN_QQ				16						//q q号码
#define LEN_EMAIL			33						//电子邮件
#define LEN_USER_NOTE		256						//用户备注
#define LEN_SEAT_PHONE		33						//固定电话
#define LEN_MOBILE_PHONE	12						//移动电话
#define LEN_PASS_PORT_ID	19						//证件号码
#define LEN_COMPELLATION	16						//真实名字
#define LEN_DWELLING_PLACE	128						//联系地址
#define LEN_WEEK            7                       //星期长度
#define LEN_TASK_NAME       64                      //任务名称
#define LEN_TRANS_REMARK    32                      //转帐备注

//机器标识
#define LEN_MACHINE_ID		33						//序列长度
#define LEN_NETWORK_ID		13						//网卡长度

//列表数据
#define LEN_TYPE			32						//种类长度
#define LEN_KIND			32						//类型长度	
#define LEN_NODE			32						//节点长度
#define LEN_PAGE			32						//定制长度
#define LEN_SERVER			32						//房间长度
#define LEN_PROCESS			32						//进程长度


#pragma mark 好友关系
#define CP_NORMAL			0						//未知关系
#define CP_ERIEND			1						//好友关系
#define CP_DETEST			2						//厌恶关系
#define CP_SHIELD           3                       //屏蔽聊天

#pragma mark 性别定义
#define GENDER_FRMALE		0						//女性性别
#define	GENDER_MANKIND		1						//男性性别


#pragma mark 分数模式
#define	SCORE_GENRE_NORMAL		0x0100				//普通模式
#define SCORE_GENRE_POSITIVE	0x0200				//非负模式

#pragma mark 任务类型
#define TASK_TYPE_WIN_INNINGS   0x01                //赢局任务
#define TASK_TYPE_SUM_INNINGS   0x02                //总局任务
#define TASK_TYPE_FIRST_WIN     0x04                //首胜任务
#define TASK_TYPE_JOIN_MATCH    0x08                //比赛任务


//任务状态
#define TASK_STATUS_UNFINISH		0				//任务状态
#define TASK_STATUS_SUCCESS			1				//任务成功
#define TASK_STATUS_FAILED			2				//任务失败
#define TASK_STATUS_REWARD			3				//领取奖励

//任务数量
#define TASK_MAX_COUNT				128				//任务数量

#pragma mark 用户状态
#define	US_NULL				0x00					//没有状态
#define US_FREE				0x01					//站立状态
#define US_SIT				0x02					//坐下状态
#define US_READY			0x03					//同意状态
#define US_LOOKON			0x04					//旁观状态
#define US_PLAYING			0x05					//游戏状态
#define US_OFFLINE			0x06					//断线状态

//MARK::比赛描述
#define MS_NULL             0x00					//没有状态
#define MS_SIGNUP			0x01					//报名状态
#define MS_MATCHING			0x02					//比赛状态
#define MS_MATCHEND         0x03					//比赛结束
#define MS_OUT				0x04					//淘汰状态


#pragma mark 房间规则
#define	SRL_LOOKON			0x00000001				//旁观标志
#define SRL_OFFLINE			0x00000002				//断线标志
#define	SRL_SAME_IP			0x00000004				//同网标志

#define	SRL_ROOM_CHAT		0x00000100				//聊天标志
#define SRL_GAME_CHAT		0x00000200				//聊天标志
#define SRL_WISPER_CHAT		0x00000400				//私聊标志
#define SRL_HIDE_USER_INFO	0x00000800				//隐藏标志


#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040		//隐藏信息

#pragma mark 列表数据
//无效属性
#define	UD_NULL				0						//无效子项
#define UD_IMAGE			100						//图形子项

//基本属性
#define UD_GAME_ID			1						//游戏标识
#define UD_USER_ID			2						//用户标识
#define UD_NICKNAME			3						//用户昵称

//扩展属性
#define	UD_GENDER			10						//用户性别
#define UD_GROUP_NAME		11						//社团名字
#define	US_UNDER_WRITE		12						//个性签名

//状态信息
#define	UD_TABLE			20						//游戏桌号
#define UD_CHAIR			21						//椅子号码

//积分信息
#define	UD_SCORE			30						//用户分数
#define UD_BANKER			31						//用户银行
#define UD_EXPERIENCE		32						//用户经验
#define UD_LOVELINESS		33						//用户魅力
#define	UD_WIN_COUNT		34						//胜利盘数
#define UD_LOSE_COUNT		35						//输局盘数 
#define	UD_DRAW_COUNT		36						//和局盘数
#define UD_FLEE_COUNT		37						//逃局盘数
#define UD_PLAY_COUNT		38						//总局盘数

//积分比率
#define	UD_WIN_RATE			40						//用户胜率
#define	UD_LOSE_RATE		41						//用户输率
#define UD_DRAW_RATE		42						//用户和率
#define	UD_FLEE_RATE		43						//用户逃率
#define UD_GAME_LEVEL		44						//游戏等级

//扩展信息
#define	UD_NOTE_INFO		50						//用户备注
#define	UD_LOOKON_USER		51						//旁官用户

//图像列表
#define	UD_FLAG				(UD_IMAGE+1)			//用户标志
#define UD_STATUS			(UD_IMAGE+2)			//用户状态

#pragma mark 数据库定义
#define	DB_ERROR			-1						//处理失败
#define	DB_SUCCESS			0						//处理成功

#define SCORE_REASON_MATCH_FEE          3           //比赛报名
#define SCORE_REASON_MATCH_QUIT         4           //比赛退赛


#define MAX_PT_MARK						4           //标识数目


//有效范围
#define VALID_TIME_DOUBLE_SCORE     3600			//有效时间
#define VALID_TIME_FOUR_SCORE       3600			//有效时间
#define VALID_TIME_GUARDKICK_CARD   3600			//防踢时间
#define VALID_TIME_POSSESS          3600			//附身时间
#define VALID_TIME_KICK_BY_MANAGER  3600			//游戏时间

//手机定义
//视图模式
#define	VIEW_MODE_ALL				0x0001			//全部可视
#define	VIEW_MODE_PART				0x0002			//部分可视

//信息模式

#define VIEW_INFO_LEVEL_1			0x0010			//部分信息
#define VIEW_INFO_LEVEL_2			0x0020			//部分信息
#define VIEW_INFO_LEVEL_3			0x0040			//部分信息
#define VIEW_INFO_LEVEL_4			0x0080			//部分信息

//其他配置

#define RECVICE_GAME_CHAT			0x0100			//接收聊天
#define RECVICE_ROOM_CHAT			0x0200			//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400			//接收私聊

//行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000          //普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000          //立即登录

//处理结果
#define RESULT_ERROR 					-1  		//处理错误
#define RESULT_SUCCESS 					0  			//处理成功
#define RESULT_FAIL 					1  			//处理失败

//变化原因
#define SCORE_REASON_WRITE              0           //写分变化
#define SCORE_REASON_INSURE             1           //银行变化
#define SCORE_REASON_PROPERTY           2           //道具变化
#define SCORE_REASON_MATCH_FEE          3           //比赛报名
#define SCORE_REASON_MATCH_QUIT         4           //比赛退赛

//登录房间失败原因
#define LOGON_FAIL_SERVER_INVALIDATION  200

//机器类型
#define ANDROID_SIMULATE				0x01		//相互模拟
#define ANDROID_PASSIVITY				0x02		//被动陪打
#define ANDROID_INITIATIVE				0x04		//主动陪打
//比赛定义
//扣费类型
#define MATCH_FEE_TYPE_GOLD				0x00		//扣费类型
#define MATCH_FEE_TYPE_MEDAL			0x01		//扣费类型

//比赛类型
#define MATCH_TYPE_LOCKTIME				0x00		//定时类型
#define MATCH_TYPE_IMMEDIATE			0x01		//即时类型


//转盘奖励
#define MAX_LOTTERY                     16			//最多奖项
//游戏数据
#define LEN_GAME_DATA					1024		//游戏数据

//Macro.h
///////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define	INVALID_BYTE		((BYTE)(0xff))			//无效数值
#define INVALID_WORD		((WORD)(0xffff))		//无效数值
#define INVALID_DWORD		((DWORD)(0xffffffff))	//无效数值

//数组维数
#define CountArray(Array)	(sizeof(Array)/sizeof(Array[0]))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress)	(((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//跟踪输出
#define DebugLog(_FORMAT, ...)		cocos2d::log("---------------------------------------------------------COCOS2D: %s",_FORMAT, ##__VA_ARGS__)
#define DebugFileLine(_FORMAT, ...) cocos2d::log("COCOS2D:%s %d %s",__FILE__,__LINE__,_FORMAT, ##__VA_ARGS__)

#pragma pack(0)
#endif




























