#ifndef tbnn_CMD_OX_HEAD_FILE
#define tbnn_CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#include "define.h"
#pragma pack(1)
//公共宏定义
namespace tbnn_game {
    
#define tbnn_GAME_PLAYER						6									//游戏人数
#define tbnn_GAME_NAME						TEXT("通比牛牛")					//游戏名字
#define tbnn_MAX_COUNT						5									//最大数目
#define MAX_JETTON_AREA					6									//下注区域
#define MAX_TIMES						5									//最大赔率

#define tbnn_VERSION_SERVER					PROCESS_VERSION(7,0,1)				//程序版本
#define tbnn_VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//程序版本

//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家

//游戏状态
#define tbnn_GS_TK_FREE						GAME_STATUS_FREE					//等待开始
#define tbnn_GS_TK_PLAYING					GAME_STATUS_PLAY					//游戏进行
#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////////////////

//控制区域
struct tagControlInfo
{
	INT  nAreaWin;		//控制区域
};

struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA					1
#define RQ_RESET_CONTROL				2
#define RQ_PRINT_SYN					3
	BYTE cbExtendData[20];													//附加数据
};

//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;															//回复类型
#define ACK_SET_WIN_AREA				1
#define ACK_PRINT_SYN					2
#define ACK_RESET_CONTROL				3
	BYTE cbResult;
#define CR_ACCEPT						2									//接受
#define CR_REFUSAL						3									//拒绝
	BYTE cbExtendData[20];													//附加数据
};

#define IDM_ADMIN_COMMDN WM_USER+1000

//////////////////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define tbnn_SUB_S_GAME_START                   100									//游戏开始
#define tbnn_SUB_S_PLAYER_EXIT                  101									//用户强退
#define tbnn_SUB_S_SEND_CARD					102									//发牌消息
#define tbnn_SUB_S_GAME_END                     103									//游戏结束
#define tbnn_SUB_S_OPEN_CARD					104									//用户摊牌
#define tbnn_SUB_S_AMDIN_COMMAND				105									//系统控制
#define tbnn_SUB_S_ADMIN_REFRESH				106									//刷新用户
    
struct tagRobotConfig
{
    LONGLONG						lRobotScoreMin;
    LONGLONG						lRobotScoreMax;
    LONGLONG						lRobotBankGet;
    LONGLONG						lRobotBankGetBanker;
    LONGLONG						lRobotBankStoMul;
};

//游戏状态
struct CMD_S_StatusFree
{
	//游戏变量
	LONGLONG							lCellScore;                                 //基础积分
	TCHAR								szGameRoomName[SERVER_LEN];                 //房间名称

	//历史积分
	LONGLONG							lTurnScore[tbnn_GAME_PLAYER];               //积分信息
	LONGLONG							lCollectScore[tbnn_GAME_PLAYER];			//积分信息
    
    tagRobotConfig                      RobotConfig;                                //机器人配置
    LONGLONG                            lStartStorage;                              //起始库存
    LONGLONG                            lBonus;
};

//游戏状态
struct CMD_S_StatusPlay
{
	//状态信息
	BYTE                                cbDynamicJoin;                                      //动态加入
	BYTE                                cbPlayStatus[tbnn_GAME_PLAYER];                     //用户状态
	LONGLONG							lTableScore[tbnn_GAME_PLAYER];                      //下注数目
	LONGLONG							lCellScore;                                         //基础积分
	WORD								wBankerUser;                                        //庄家用户
	TCHAR								szGameRoomName[SERVER_LEN];                         //房间名称

	//扑克信息
	BYTE								bOxCard[tbnn_GAME_PLAYER];                          //牛牛数据
	BYTE								cbOxCardData[tbnn_GAME_PLAYER][tbnn_MAX_COUNT];     //牛牛扑克
	BYTE								cbHandCardData[tbnn_GAME_PLAYER][tbnn_MAX_COUNT];   //桌面扑克
	

	//历史积分
	LONGLONG							lTurnScore[tbnn_GAME_PLAYER];                       //积分信息
	LONGLONG							lCollectScore[tbnn_GAME_PLAYER];                    //积分信息
    
    tagRobotConfig                      RobotConfig;                                        //机器人配置
    LONGLONG                            lStartStorage;                                      //起始库存
    LONGLONG                            lBonus;
};

//游戏开始
struct CMD_S_GameStart
{
	WORD								wBankerUser;                                        //庄家用户
	BYTE                                cbPlayStatus[tbnn_GAME_PLAYER];                     //用户状态
	BYTE								cbCardData[tbnn_GAME_PLAYER][tbnn_MAX_COUNT];       //用户扑克
	LONGLONG							lCellScore;                                         //游戏底分
};

//游戏结束
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[tbnn_GAME_PLAYER];                         //游戏税收
	LONGLONG							lGameScore[tbnn_GAME_PLAYER];                       //游戏得分
	BYTE								cbCardData[tbnn_GAME_PLAYER][tbnn_MAX_COUNT];       //用户扑克
	BYTE								cbDelayOverGame;
    bool                                bFiveKing[tbnn_GAME_PLAYER];                        //五花牛标示
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								cbCardData[tbnn_GAME_PLAYER][tbnn_MAX_COUNT];	//用户扑克
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//摊牌用户
	BYTE								bOpen;								//摊牌标志
};
//////////////////////////////////////////////////////////////////////////

//客户端命令结构
#define tbnn_SUB_C_OPEN_CARD					5									//用户摊牌
#define SUB_C_MODIFY_STORAGE                    6									//修改库存
#define SUB_C_REQUEST_ADD_USERROSTER            7									//请求添加用户名单
#define SUB_C_REQUEST_DELETE_USERROSTER         8									//请求删除用户名单
#define SUB_C_REQUEST_UPDATE_USERROSTER         9									//请求更新用户名单

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
	BYTE								cbOxCardData[tbnn_MAX_COUNT];			//牛牛扑克
};
    
}
#pragma pack(0)
#endif
