#pragma once
#include "cocos2d.h"
#include "define.h"
#include "CocosHeader.h"
ZJH_NAMESPACE_BEGIN
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//π´π≤∫Í∂®“Â

#define KIND_ID							6
#define GAME_PLAYER						5
#define GAME_NAME						TEXT("炸金花")
#define MAX_COUNT						3
#define VERSION_SERVER			    	PROCESS_VERSION(6,0,3)
#define VERSION_CLIENT				    PROCESS_VERSION(6,0,3)

//Ω· ¯‘≠“Ú
#define GER_NO_PLAYER					0x10								//没有玩家
#define GER_COMPARECARD					0x20								//比牌结束
#define GER_OPENCARD					0x30								//开牌结束

//”Œœ∑◊¥Ã¨
#define GS_T_FREE					GAME_STATUS_FREE									//等待开始
#define GS_T_SCORE					GAME_STATUS_PLAY								//叫分状态
#define GS_T_PLAYING				GAME_STATUS_PLAY+1							//游戏进行

//////////////////////////////////////////////////////////////////////////
//∑˛ŒÒ∆˜√¸¡ÓΩ·ππ

#define zjh_SUB_S_GAME_START				100									//游戏开始
#define zjh_SUB_S_ADD_SCORE					101									//加注结果
#define zjh_SUB_S_GIVE_UP					102									//放弃跟注
#define zjh_SUB_S_SEND_CARD					103									//发牌消息
#define zjh_SUB_S_GAME_END					104									//游戏结束
#define zjh_SUB_S_COMPARE_CARD				105									//比牌跟注
#define zjh_SUB_S_LOOK_CARD					106									//看牌跟注
#define zjh_SUB_S_PLAYER_EXIT				107									//用户强退
#define zjh_SUB_S_OPEN_CARD					108									//开牌消息
#define zjh_SUB_S_WAIT_COMPARE				109									//等待比牌
#define zjh_SUB_S_ANDROID_CARD				110									//智能消息
#define zjh_SUB_S_CHEAT_CARD				111									//看牌消息

//游戏状态
struct CMD_S_StatusFree
{
    LONGLONG							lCellScore;							//基础积分
    TCHAR                               szGameRoomName[32];                 //房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
    //加注信息
    LONGLONG							lMaxCellScore;						//单元上限
    LONGLONG							lCellScore;							//单元下注
	LONGLONG							lCurrentTimes;						//当前倍数
    LONGLONG							lUserMaxScore;						//用户分数上限
	//状态信息
    WORD								wBankerUser;						//庄家用户
    WORD				 				wCurrentUser;						//当前玩家
    BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态
    bool								bMingZhu[GAME_PLAYER];				//看牌状态
    LONGLONG							lTableScore[GAME_PLAYER];			//下注数目
    TCHAR                               szGameRoomName[32];                 //房间名称

    //扑克信息
    BYTE								cbHandCardData[3];					//扑克数据

    //状态信息
	bool								bCompareState;						//比牌状态
};

//游戏开始
struct CMD_S_GameStart
{
    //下注信息
    LONGLONG							lMaxScore;							//最大下注
    LONGLONG							lCellScore;							//单元下注
    LONGLONG							lCurrentTimes;						//当前倍数
    LONGLONG							lUserMaxScore;						//分数上限
  

    //用户信息
    WORD								wBankerUser;						//庄家用户
    WORD				 				wCurrentUser;						//当前玩家
    
    BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
};

//用户下注
struct CMD_S_AddScore
{
    WORD								wCurrentUser;						//当前用户
    WORD								wAddScoreUser;						//加注用户
	WORD								wCompareState;						//比牌状态
    LONGLONG							lAddScoreCount;						//加注数目
    LONGLONG							lCurrentTimes;						//当前倍数
};

//用户放弃
struct CMD_S_GiveUp
{
    WORD								wGiveUpUser;						//放弃用户
};

//比牌数据包
struct CMD_S_CompareCard
{
    WORD								wCurrentUser;						//当前用户
    WORD								wCompareUser[2];					//比牌用户
    WORD								wLostUser;							//输牌用户
};

//看牌数据包
struct CMD_S_LookCard
{
    WORD								wLookCardUser;						//看牌用户
    BYTE								cbCardData[MAX_COUNT];				//用户扑克
};

//发送扑克
struct CMD_S_SendCard
{
    BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
};

//开牌数据包
struct CMD_S_OpenCard
{
    WORD								wWinner;							//胜利用户
};

//游戏结束
struct CMD_S_GameEnd
{
    LONGLONG							lGameTax;                           //游戏税收
    LONGLONG							lGameScore[GAME_PLAYER];			//游戏得分
    BYTE								cbCardData[GAME_PLAYER][3];			//用户扑克
	WORD								wCompareUser[GAME_PLAYER][4];		//比牌用户
	WORD								wEndState;							//结束状态
    bool								bDelayOverGame;						//延迟开始
    WORD								wServerType;						//房间类型
};

//用户退出
struct CMD_S_PlayerExit
{
    WORD								wPlayerID;							//退出用户
};

//等待比牌
struct CMD_S_WaitCompare
{
    WORD								wCompareUser;						//比牌用户
};

//////////////////////////////////////////////////////////////////////////

//øÕªß∂À√¸¡ÓΩ·ππ
#define SUB_C_ADD_SCORE					1									//用户加注
#define SUB_C_GIVE_UP					2									//放弃消息
#define SUB_C_COMPARE_CARD				3									//比牌消息
#define SUB_C_LOOK_CARD					4									//看牌消息
#define SUB_C_OPEN_CARD					5									//开牌消息
#define SUB_C_WAIT_COMPARE				6									//等待比牌
#define SUB_C_FINISH_FLASH				7									//完成动画
#define SUB_C_ADD_SCORE_TIME            8

//用户加注
struct CMD_C_AddScore
{
	LONGLONG							lScore;								//加注数目
	WORD								wState;								//当前状态
};

//比牌数据包
struct CMD_C_CompareCard
{	
	WORD								wCompareUser;						//比牌用户
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()

ZJH_NAMESPACE_END
