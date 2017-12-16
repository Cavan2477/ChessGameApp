//
//  CMD_Game.h
//  DouDiZhu
//
//  Created by zhong on 1/13/16.
//
//

#ifndef CMD_Game_h
#define CMD_Game_h

#include "define.h"

#pragma pack(1)
namespace bjl_game{
//////////////////////////////////////////////////////////////////////////
//公共宏定义

//组件属性
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//玩家索引
#define AREA_XIAN					0									//闲家索引
#define AREA_PING					1									//平家索引
#define AREA_ZHUANG					2									//庄家索引
#define AREA_XIAN_TIAN				3									//闲天王
#define AREA_ZHUANG_TIAN			4									//庄天王
#define AREA_TONG_DUI				5									//同点平
#define AREA_XIAN_DUI				6									//闲对子
#define AREA_ZHUANG_DUI				7									//庄对子
#define AREA_MAX					8									//最大区域

//区域倍数multiple
#define MULTIPLE_XIAN				2									//闲家倍数
#define MULTIPLE_PING				9									//平家倍数
#define MULTIPLE_ZHUANG				2									//庄家倍数
#define MULTIPLE_XIAN_TIAN			3									//闲天王倍数
#define MULTIPLE_ZHUANG_TIAN		3									//庄天王倍数
#define MULTIPLE_TONG_DIAN			33									//同点平倍数
#define MULTIPLE_XIAN_PING			12									//闲对子倍数
#define MULTIPLE_ZHUANG_PING		12									//庄对子倍数


//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度

#define IDM_UPDATE_STORAGE			WM_USER+1001

//设备坐标定义
#define POINT(width, x, y)          ( (width>480) ? ccp(x+44, y):ccp(x, y) )
//机器版本 iPhone5 4inch other 3.5inch
#define MachineVersion()            ( (P_WIDTH) > 480 ? (6) : (5) )

//记录信息
struct tagServerGameRecord
{
    BYTE							cbKingWinner;						//天王赢家
    bool							bPlayerTwoPair;						//对子标识
    bool							bBankerTwoPair;						//对子标识
    BYTE							cbPlayerCount;						//闲家点数
    BYTE							cbBankerCount;						//庄家点数
};

typedef struct
{
    //坐庄
    int                             nEnableRobotBanker;				//是否做庄
    LONGLONG                        lRobotBankerCountMin;			//坐庄次数
    LONGLONG						lRobotBankerCountMax;			//坐庄次数
    LONGLONG						lRobotListMinCount;				//列表人数
    LONGLONG						lRobotListMaxCount;				//列表人数
    LONGLONG						lRobotApplyBanker;				//最多申请个数
    LONGLONG						lRobotWaitBanker;				//空盘重申
    
    //下注
    LONGLONG						lRobotMinBetTime;				//下注筹码个数
    LONGLONG						lRobotMaxBetTime;				//下注筹码个数
    LONGLONG						lRobotMinJetton;				//下注筹码金额
    LONGLONG						lRobotMaxJetton;				//下注筹码金额
    LONGLONG						lRobotBetMinCount;				//下注机器人数
    LONGLONG						lRobotBetMaxCount;				//下注机器人数
    LONGLONG						lRobotAreaLimit;				//区域限制
    
    //存取款
    LONGLONG						lRobotScoreMin;					//鹿豆下限
    LONGLONG						lRobotScoreMax;					//鹿豆上限
    LONGLONG						lRobotBankGetMin;				//取款最小值(非庄)
    LONGLONG						lRobotBankGetMax;				//取款最大值(非庄)
    LONGLONG						lRobotBankGetBankerMin;			//取款最小值(坐庄)
    LONGLONG						lRobotBankGetBankerMax;			//取款最大值(坐庄)
    LONGLONG						lRobotBankStoMul;				//存款百分比
    
    
    int                             nAreaChance[8];
    
}tagCustomAndroid;

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_SEND_RECORD			106									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		107									//下注失败
#define SUB_S_CANCEL_BANKER			108									//取消申请
#define SUB_S_AMDIN_COMMAND			109									//管理员命令
#define SUB_S_UPDATE_STORAGE        110									//更新库存


//请求回复
struct CMD_S_CommandResult
{
    BYTE cbAckType;					//回复类型
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
    BYTE cbResult;
#define CR_ACCEPT  2			//接受
#define CR_REFUSAL 3			//拒绝
    BYTE cbExtendData[20];			//附加数据
};


//更新库存
struct CMD_S_UpdateStorage
{
    LONGLONG						lStorage;							//新库存值
    LONGLONG						lStorageDeduct;						//库存衰减
};

//失败结构
struct CMD_S_PlaceBetFail
{
    WORD							wPlaceUser;							//下注玩家
    BYTE							lBetArea;							//下注区域
    LONGLONG						lPlaceScore;						//当前下注
};

//申请庄家
struct CMD_S_ApplyBanker
{
    WORD							wApplyUser;							//申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
    WORD							wCancelUser;						//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
    WORD							wBankerUser;						//当庄玩家
    LONGLONG						lBankerScore;						//庄家分数
};

//游戏状态
struct CMD_S_StatusFree
{
    //全局信息
    BYTE							cbTimeLeave;						//剩余时间
    
    //玩家信息
    LONGLONG						lPlayFreeSocre;						//玩家自由鹿豆
    
    //庄家信息
    WORD							wBankerUser;						//当前庄家
    LONGLONG						lBankerScore;						//庄家分数
    LONGLONG						lBankerWinScore;					//庄家赢分
    WORD							wBankerTime;						//庄家局数
    
    //是否系统坐庄
    bool							bEnableSysBanker;					//系统做庄
    
    //控制信息
    LONGLONG						lApplyBankerCondition;				//申请条件
    LONGLONG						lAreaLimitScore;					//区域限制
    
    //房间信息
    TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
    bool                            bGenerEducate;
    tagCustomAndroid				CustomAndroid;						//机器人配置
    
};
//游戏状态
struct CMD_S_StatusPlay
{
    //全局信息
    BYTE							cbTimeLeave;						//剩余时间
    BYTE							cbGameStatus;						//游戏状态
    
    //下注数
    LONGLONG						lAllBet[AREA_MAX];					//总下注
    LONGLONG						lPlayBet[AREA_MAX];					//玩家下注
    
    //玩家积分
    LONGLONG						lPlayBetScore;						//玩家最大下注
    LONGLONG						lPlayFreeSocre;						//玩家自由鹿豆
    
    //玩家输赢
    LONGLONG						lPlayScore[AREA_MAX];				//玩家输赢
    LONGLONG						lPlayAllScore;
    LONGLONG                        lRevenue;                           //税收
    //玩家成绩
    
    //庄家信息
    WORD							wBankerUser;						//当前庄家
    LONGLONG						lBankerScore;						//庄家分数
    LONGLONG						lBankerWinScore;					//庄家赢分
    WORD							wBankerTime;						//庄家局数
    
    //是否系统坐庄
    bool							bEnableSysBanker;					//系统做庄
    
    //控制信息
    LONGLONG						lApplyBankerCondition;				//申请条件
    LONGLONG						lAreaLimitScore;					//区域限制
    
    //扑克信息
    BYTE							cbCardCount[2];						//扑克数目
    BYTE							cbTableCardArray[2][3];				//桌面扑克
    
    //房间信息
    TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
    bool                            bGenerEducate;
    tagCustomAndroid				CustomAndroid;						//机器人配置
};

//游戏空闲
struct CMD_S_GameFree
{
    BYTE							cbTimeLeave;						//剩余时间
};

//游戏开始
struct CMD_S_GameStart
{
    BYTE							cbTimeLeave;						//剩余时间
    
    WORD							wBankerUser;						//庄家位置
    LONGLONG						lBankerScore;						//庄家金币
    
    LONGLONG						lPlayBetScore;						//玩家最大下注
    LONGLONG						lPlayFreeSocre;						//玩家自由金币
    
    int								nChipRobotCount;					//人数上限 (下注机器人)
    LONGLONG                         nListUserCount;						//列表人数
    int								nAndriodCount;						//机器人列表人数
};

//用户下注
struct CMD_S_PlaceBet
{
    WORD							wChairID;							//用户位置
    BYTE							cbBetArea;							//筹码区域
    LONGLONG						lBetScore;							//加注数目
    BYTE							cbAndroidUser;						//机器标识
    BYTE							cbAndroidUserT;						//机器标识
};

//游戏结束
struct CMD_S_GameEnd
{
    //下局信息
    BYTE							cbTimeLeave;						//剩余时间
    
    //扑克信息
    BYTE							cbCardCount[2];						//扑克数目
    BYTE							cbTableCardArray[2][3];				//桌面扑克
    
    //庄家信息
    LONGLONG						lBankerScore;						//庄家成绩
    LONGLONG						lBankerTotallScore;					//庄家成绩
    int								nBankerTime;						//做庄次数
    
    //玩家成绩
    LONGLONG						lPlayScore[AREA_MAX];				//玩家成绩
    LONGLONG						lPlayAllScore;						//玩家成绩
    
    //全局信息
    LONGLONG						lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_AMDIN_COMMAND			4									//管理员命令
#define SUB_C_UPDATE_STORAGE        5									//更新库存

struct CMD_C_AdminReq
{
    BYTE cbReqType;
#define RQ_SET_WIN_AREA	1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
    BYTE cbExtendData[20];			//附加数据
};


//struct CMD_C_AdminReq
//{
//	BYTE cbReqType;
//		 #define RQ_SETWINAREA   1
//	BYTE cbExtendData[120];
//};


//用户下注
struct CMD_C_PlaceBet
{
    
    BYTE							cbBetArea;						//筹码区域
    LONGLONG						lBetScore;						//加注数目
};

#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2

//更新库存
struct CMD_C_UpdateStorage
{
    BYTE                            cbReqType;						//请求类型
    LONGLONG						lStorage;						//新库存值
    LONGLONG						lStorageDeduct;					//库存衰减
};

//////////////////////////////////////////////////////////////////////////////////
}
#pragma pack(0)
#endif /* CMD_Game_h */
