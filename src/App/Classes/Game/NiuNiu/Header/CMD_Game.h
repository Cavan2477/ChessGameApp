/************************************************************************************
 * file: 		CMD_Game.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:47:49
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __NN_CMD_GAME_H__
#define __NN_CMD_GAME_H__

#include "../../../Common/PublicDefine.h"

#pragma pack(1)

namespace nn_game
{
//////////////////////////////////////////////////////////////////////////
//公共宏定义
    /*
//101
#define KIND_ID						27										//游戏 ID
#define NNNNGAME_PLAYER					4									//游戏人数
#define GAME_NAME					TEXT("牛牛")								//游戏名字
     */
#define NNGAME_PLAYER					4									//游戏人数
#define NN_MAXCOUNT					5										//最大数目
#define MAX_JETION_AREA             4                                       //下注区域
#define MAX_TIMES                   5                                       //最大赔率
#define PAGE_GROUP_COUNT            4                                       //桌子分页数量

#define VERSION_SERVER				PROCESS_VERSION(6,0,3)					//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)					//程序版本

#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)		//游戏类型


//结束原因
#define GER_NO_PLAYER				0x10									//没有玩家

//游戏状态
#define nn_GS_TK_FREE					(0)                                     //等待开始
#define nn_GS_TK_CALL					(100)                                   //叫庄状态
#define nn_GS_TK_SCORE					(101)                                   //下注状态
#define nn_GS_TK_PLAYING				(102)                                   //游戏进行
#define nn_GS_TK_END                    (255)                                   //游戏结束

#define SERVER_LEN                  32

//用户状态
#define USEX_NULL					0										//用户状态
#define USEX_PLAYING				1										//用户状态
#define USEX_DYNAMIC				2										//用户状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define NN_SUB_S_GAME_START                 100									//游戏开始
#define NN_SUB_S_ADD_SCORE					101									//加注结果
#define NN_SUB_S_PLAYER_EXIT				102									//用户强腿
#define NN_SUB_S_SEND_CARD					103									//发牌消息
#define NN_SUB_S_GAME_END					104									//游戏结束
#define NN_SUB_S_OPEN_CARD					105									//用户摊牌
#define NN_SUB_S_CALL_BANKER				106									//用户叫庄
#define NN_SUB_S_CALL_CARD                  107                                 //发牌消息
#define NN_SUB_S_ADMIN_COMMAND              108                                 //系统控制
#define NN_SUB_S_BANKER_OPERATE             109                                 //存取款
struct tagRobotConfig
{
    LONGLONG						lRobotScoreMin;
    LONGLONG						lRobotScoreMax;
    LONGLONG						lRobotBankGet;
    LONGLONG						lRobotBankGetBanker;
    LONGLONG						lRobotBankStoMul;
};
    
//游戏状态
typedef struct
{
	LONGLONG                            lCellScore;							//基础积分

	//历史积分
	LONGLONG                            lTurnScore[NNGAME_PLAYER];            //积分信息
	LONGLONG                            lCollectScore[NNGAME_PLAYER];         //积分信息
	TCHAR                               szGameRoomName[SERVER_LEN];         //房间名称
    
    tagRobotConfig						RobotConfig;						//机器人配置
    LONGLONG							lStartStorage;						//起始库存
    LONGLONG							lBonus;
}CMD_S_StatusFree;

//游戏状态
typedef struct 
{
	WORD							    wCallBanker;						//叫庄用户
	BYTE                                cbDynamicJoin;                      //动态加入
	BYTE							    cbPlayStatus[NNGAME_PLAYER];          //用户状态

	//历史积分
	LONGLONG                            lTurnScore[NNGAME_PLAYER];            //积分信息
	LONGLONG                            lCollectScore[NNGAME_PLAYER];         //积分信息
	TCHAR                               szGameRoomName[SERVER_LEN];         //房间名称
    
    tagRobotConfig						RobotConfig;						//机器人配置
    LONGLONG							lStartStorage;						//起始库存
    LONGLONG							lBonus;
}CMD_S_StatusCall;

//游戏状态
typedef struct 
{
	//下注信息
	BYTE                                    cbPlayStatus[NNGAME_PLAYER];          //用户状态
	BYTE                                    cbDynamicJoin;                      //动态加入
	LONGLONG								lTurnMaxScore;						//最大下注
	LONGLONG								lTableScore[NNGAME_PLAYER];			//下注数目
	WORD							    	wBankerUser;						//庄家用户
	TCHAR                                   szGameRoomName[SERVER_LEN];         //房间名称

	//历史积分
	LONGLONG                                lTurnScore[NNGAME_PLAYER];            //积分信息
	LONGLONG                                lCollectScore[NNGAME_PLAYER];         //积分信息
    
    tagRobotConfig						RobotConfig;						//机器人配置
    LONGLONG							lStartStorage;						//起始库存
    LONGLONG							lBonus;
}CMD_S_StatusScore;

//游戏状态
typedef struct 
{
	//状态信息	
	BYTE								    cbPlayStatus[NNGAME_PLAYER];          //用户状态
	BYTE                                    cbDynamicJoin;                      //动态加入    
	LONGLONG								lTurnMaxScore;						//最大下注
	LONGLONG								lTableScore[NNGAME_PLAYER];			//下注数目
	WORD								    wBankerUser;						//庄家用户

	//扑克信息
	BYTE							    	cbHandCardData[NNGAME_PLAYER][NN_MAXCOUNT];//桌面扑克
	BYTE						      		bOxCard[NNGAME_PLAYER];				//牛牛数据

	//历史积分
	LONGLONG                                lTurnScore[NNGAME_PLAYER];            //积分信息
	LONGLONG                                lCollectScore[NNGAME_PLAYER];         //积分信息
	TCHAR                                   szGameRoomName[SERVER_LEN];         //房间名称
    
    tagRobotConfig                          RobotConfig;						//机器人配置
    LONGLONG                                lStartStorage;						//起始库存
    LONGLONG                                lBonus;
}CMD_S_StatusPlay;

//用户叫庄
typedef struct 
{
	WORD							     	wCallBanker;						//叫庄用户
	bool							    	bFirstTimes;						//首次叫庄
	BYTE                                    cbPlayerStatus[NNGAME_PLAYER];		//玩家状态
}CMD_S_CallBanker;

//游戏开始
typedef struct 
{
	//下注信息
	LONGLONG								lTurnMaxScore;						//最大下注
	WORD							     	wBankerUser;						//庄家用户
}CMD_S_GameStart;

//用户下注
typedef struct 
{
	WORD							    	wAddScoreUser;						//加注用户
	LONGLONG								lAddScoreCount;						//加注数目
}CMD_S_AddScore;

//游戏结束
typedef struct 
{
	LONGLONG								lGameTax[NNGAME_PLAYER];				//游戏税收
	LONGLONG								lGameScore[NNGAME_PLAYER];			//游戏得分
	BYTE							     	cbCardData[NNGAME_PLAYER];			//用户扑克
	BYTE                                    cbDelayOverGame;
    bool                                    bfiveking[NNGAME_PLAYER];           //五花牛标志
}CMD_S_GameEnd;

//发牌数据包
typedef struct 
{
	BYTE								    cbCardData[NNGAME_PLAYER][NN_MAXCOUNT];	//用户扑克
}CMD_S_SendCard;

//用户强退
typedef struct 
{
	WORD						      		wPlayerID;							//退出用户
}CMD_S_PlayerExit;

//用户摊牌
typedef struct 
{
	WORD							     	wPlayerID;							//摊牌用户
	BYTE							      	bOpen;								//摊牌标志
}CMD_S_Open_Card;

//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define NN_SUB_C_CALL_BANKER				1										//用户叫庄
#define NN_SUB_C_ADD_SCORE					2										//用户加注
#define NN_SUB_C_OPEN_CARD					3										//用户摊牌
#define NN_SUB_C_SPECIAL_CLIENT_REPORT      4                                       //特殊终端
#define NN_SUB_C_ADMIN_COMMAND              5                                       //系统控制

//用户叫庄
typedef struct 
{
	BYTE							    	bBanker;							//坐庄标志
}CMD_C_CallBanker;

//用户加注
typedef struct 
{
	LONGLONG								lScore;								//加注数目
}CMD_C_AddScore;

//用户摊牌
typedef struct 
{
	BYTE							    	bOX;								//牛牛标志
}CMD_C_OxCard;

//////////////////////////////////////////////////////////////////////////
}

#pragma pack(0)

#endif