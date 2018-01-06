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
//游戏列表

//游戏类型
typedef struct _stGameType
{
	WORD				wJoinID;					//挂接索引
	WORD				wSortID;					//排序索引
	WORD				wTypeID;					//类型索引
	TCHAR				szTypeName[LEN_TYPE];		//种类名字
}ST_GAME_TYPE, *PST_GAME_TYPE;

//游戏种类
typedef struct _stGameKind
{
	WORD				wTypeID;					//类型索引
	WORD				wJoinID;					//挂接索引
	WORD				wSortID;					//排序索引
	WORD				wKindID;					//类型索引
	WORD				wGameID;					//模块索引 
	DWORD				dwOnlineCount;				//在线人数
    DWORD				dwAndroidCount;				//在线人数
	DWORD				dwFullCount;				//满员人数
	TCHAR				szKindName[LEN_KIND];		//类型名字
	TCHAR				szProcessName;				//进程名字
}ST_GAME_KIND, *PST_GAME_KIND;

//游戏节点
typedef struct _stGameNode
{
	WORD				wKindID;					//名称索引
	WORD				wJoinID;					//挂接索引
	WORD				wSortID;					//排序索引
	WORD				wNodeID;					//节点索引
	TCHAR				szNodeName;					//节点名字
}ST_GAME_NODE, *PST_GAME_NODE;

//定制类型
typedef struct _stGamePage
{
	WORD				wPageID;					//页面索引
	WORD				wKindID;					//名称索引
	WORD				wNodeID;					//节点索引
	WORD				wSortID;					//排序索引
	WORD				wOperateType;				//控制类型
	TCHAR				szDisPlayeName[LEN_PAGE];	//显示名称
}ST_GAME_PAGE, *PST_GAME_PAGE;

//游戏房间服务器
typedef struct _stGameRoomServer
{
	WORD				wKindID;					//名称索引
	WORD				wNodeID;					//节点索引
	WORD				wSortID;					//排序索引
	WORD				wServerID;					//房间索引
    WORD                wServerKind;                //房间类型
    WORD                wServerType;                //房间类型
    WORD                wServerLevel;               //房间等级
	WORD				wServerPort;				//房间端口
    LONGLONG               lCellScore;                 //单元积分
    LONGLONG               lEnterScore;                //进入积分
    DWORD				dwServerRule;               //房间规则
	DWORD				dwOnlineCount;				//在线人数
    DWORD               dwAndroidCount;             //机器人数
	DWORD				dwFullCount;				//满员人数
	TCHAR				szServerAddr[32];			//房间地址
	TCHAR				szServerName[LEN_SERVER];	//房间名称
}ST_GAME_ROOM_SERVER, *PST_GAME_ROOM_SERVER;

//比赛报名
typedef struct _stSignupMatchInfo
{
	WORD				wServerID;					//房间标识
	DWORD				dwMatchID;					//比赛标识
	DWORD				dwMatchNO;					//比赛场次
}ST_SIGNUP_MATCH_INFO, *PST_SIGNUP_MATCH_INFO;

//比赛信息
typedef struct _stGameMatch
{
	//基本信息
	WORD				wServerID;					//房间标识
	DWORD				dwMatchID;					//比赛标识
	DWORD				dwMatchNO;					//比赛场次
	BYTE				cbMatchType;				//比赛类型
	TCHAR				szMatchName[32];			//比赛名称

	//比赛信息
	BYTE				cbMemberOrder;				//会员等级
	BYTE				cbMatchFeeType;				//扣费类型
	LONGLONG				lMatchFee;					//比赛费用

	//比赛信息
	WORD				wStartUserCount;			//开赛人数
	WORD				wMatchPlayCount;			//比赛局数

	//比赛奖励
	WORD                wRewardCount;               //奖励人数

	//比赛时间
	ST_SYS_TIME			stMatchStartTime;			//开始时间
	ST_SYS_TIME			stMatchEndTime;				//结束时间
}ST_GAME_MATCH, *PST_GAME_MATCH;

//视频配置
typedef struct _stAVServerConfig
{
	WORD				wAVServerPort;				//视频端口
	DWORD				dwAVServerAddr;				//视频地址
}ST_AV_SERVER_CONFIG, *PST_AV_SERVER_CONFIG;

//在线信息
typedef struct _stOnlineInfoType
{
	WORD				wKindID;					//类型标识
	DWORD				dwOnlineCount;				//在线人数
}ST_ONLINE_INFO_TYPE, *PST_ONLINE_INFO_TYPE;


//在线统计
typedef struct _stOnlineInfoStatistics
{
	WORD				wKindID;					//类型标识
	DWORD				dwOnLineCount;				//在线人数
	DWORD				dwAndroidCount;				//机器人数
}ST_ONLINE_INFO_STATISTICS, *PST_ONLINE_INFO_STATISTICS;

//游戏房间在线信息
typedef struct _stOnlineInfoGameRoom
{
	WORD				wServerID;					//房间标识
	DWORD				dwOnLineCount;				//在线人数
}ST_ONLINE_INFO_ROOM_SERVER, *PST_ONLINE_INFO_ROOM_SERVER;

/////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 用户信息

//桌子状态
typedef struct _stTableStatus
{
	BYTE				cbTableLock;				//锁定标志
	BYTE				cbPlayStatus;				//游戏标志
    INT					nCellScore;					//单元积分
}ST_TABLE_STATUS, *PST_TABLE_STATUS;

//用户状态
typedef struct _stUserStatus
{
	WORD				wTableID;					//桌子索引
	WORD				wChairID;					//椅子位置
	BYTE				cbUserStatus;				//用户状态
}ST_USER_STATUS, *PST_USER_STATUS;

typedef struct _stUserRelationship
{
    BYTE               cbCompanion;                 //用户关系
}ST_USER_RELATIONSHIP, *PST_USER_RELATIONSHIP;

//用户积分
typedef	struct _stUserScore
{
	//积分信息
	LONGLONG			llGameCoin;					//用户游戏币
    LONGLONG            llGrade;                    //用户成绩
	LONGLONG			llBanker;					//银行分数
    LONGLONG            llGold;						//用户金币
	
	//输赢信息
	DWORD				dwWinCount;					//胜利盘数
	DWORD				dwLostCount;				//失败盘数
	DWORD				dwDrawCount;				//和局盘数
	DWORD				dwFleeCount;				//逃跑盘数
	
	//全局信息
	DWORD				dwUserMedal;				//用户奖牌
	DWORD				dwExp;						//用户经验
	DWORD				dwLoveLiness;				//用户魅力
}ST_USER_SCORE, *PST_USER_SCORE;


#pragma mark -
#pragma mark 用户积分
typedef struct _stMobileUserScore
{
	//积分信息
	LONGLONG			llGameCoin;					//用户游戏币
	DOUBLE              dBean;                      //用户游戏豆

	//输赢信息
	DWORD				dwWinCount;					//胜利盘数
	DWORD				dwLostCount;				//失败盘数
	DWORD				dwDrawCount;				//和局盘数
	DWORD				dwFleeCount;				//逃跑盘数
	
	//全局信息
	DWORD				dwExperience;				//用户经验
}ST_MOBILE_USER_SCORE, *PST_MOBILE_USER_SCORE;

//道具使用
typedef struct _stUsePropInfo
{
    WORD                wPropCount;					//道具数目
    WORD                dwValidNum;                 //有效次数
    DWORD               dwEffectTime;               //生效时间
}ST_USER_PROP_INFO, *PST_USER_PROP_INFO;

//用户道具
typedef struct _stUserProp
{
	WORD                wPropUseFlag;				//道具使用标示
	ST_USER_PROP_INFO	stPropInfo[MAX_PT_MARK];	//使用信息
}ST_USER_PROP, *PST_USER_PROP;

//道具包裹
typedef struct _stPropPackage
{
	WORD                wTrumpetCount;              //小喇叭数
	WORD                wTyphonCount;               //大喇叭数
}ST_PROP_PACKAGE, *PST_PROP_PACKAGE;

//时间信息
typedef struct _stTimeInfo
{
	DWORD				dwEnterTableTimer;			//进出桌子时间
	DWORD				dwLeaveTableTimer;			//离开桌子时间
	DWORD				dwStartGameTimer;			//开始游戏时间
	DWORD				dwEndGameTimer;				//离开游戏时间
}ST_TIME_INFO, *PST_TIME_INFO;

//用户信息
typedef struct _stUserInfo
{
	//基本属性
	DWORD				dwUserID;						//用户I D
	DWORD				dwGameID;						//游戏I D
	DWORD				dwGroupID;						//社团I D
	
	TCHAR				szNickName[LEN_NICENAME];		//用户昵称
	TCHAR				szGroupName[LEN_GROUP_NAME];	//社团名字
	TCHAR				szUnderWrite[LEN_UNDER_WRITE];	//个性签名
	
	//头像信息
	DWORD				wFaceID;						//头像索引
	DWORD				dwCustomID;						//自定标识
	
	//用户资料
	BYTE				cbGender;						//用户性别
	BYTE				cbMemberOrder;					//会员等级
	BYTE				cbMasterOrder;					//管理等级
	
	//用户状态
	WORD				wTableID;						//桌子索引
    WORD                wLastTableID;                   //游戏桌子
	WORD				wChairID;						//椅子索引
	BYTE				cbUserStatus;					//用户状态
	
	//积分信息
	LONGLONG			llGameCoin;						//用户分数
	LONGLONG			llGrade;						//用户成绩
	LONGLONG			llInsureGold;					//银行金币
	LONGLONG            llGold;                         //用户金币
    
	//游戏信息
	DWORD				dwWinCount;						//胜利盘数
	DWORD				dwLostCount;					//失败盘数
	DWORD				dwDrawCount;					//和局盘数
	DWORD				dwFleeCount;					//逃跑盘数
	DWORD				dwExperience;					//用户经验
	DWORD				dwLoveLiness;					//用户魅力
    
    //时间信息
    ST_TIME_INFO        stTimeInfo;						//时间信息
}ST_USER_INFO, *PST_USER_INFO;

//用户信息 tagUserInfoHead
typedef struct _stUserInfoHead
{
public:
	_stUserInfoHead()
    :dwGameID(0.0),dwUserID(0.0),wFaceID(0.0),dwCustomID(0.0),cbGender('0'),cbMemberOrder('0'),
    wTableID(0.0),wChairID(0.0),cbUserStatus('0'),llGameCoin(0.0),dwWinCount(0.0),dwLostCount(0.0),
    dwDrawCount(0.0),dwFleeCount(0.0),dwExperience(0.0)
    {
        
    }
    
	~_stUserInfoHead(){}
    
public:
	//用户属性
	DWORD				dwGameID;						//游戏I D
	DWORD				dwUserID;						//用户I D
    DWORD               dwGroupID;                      //社团id
	
	//头像信息
	WORD				wFaceID;						//头像索引
	DWORD				dwCustomID;						//自定标识
	
	//用户属性
    bool                BIsAndroid;                     //机器标示
	BYTE				cbGender;						//用户性别
	BYTE				cbMemberOrder;					//会员等级
    BYTE                cbMasterOrder;                  //管理等级
	
	//用户状态
	WORD				wTableID;						//桌子索引
	WORD				wChairID;						//椅子索引
	BYTE				cbUserStatus;					//用户状态
	
	//积分信息
	LONGLONG			llGameCoin;						//用户游戏币
	LONGLONG            llGrade;                        //用户成绩
	LONGLONG            llInsureGold;                   //用户银行金币
	LONGLONG            llGold;                         //用户金币
	
	//游戏信息
	DWORD				dwWinCount;						//胜利盘数
	DWORD				dwLostCount;					//失败盘数
	DWORD				dwDrawCount;					//和局盘数
	DWORD				dwFleeCount;					//逃跑盘数
	DWORD				dwExperience;					//用户经验
	DWORD				dwLoveliness;					//用户魅力
}ST_USER_INFO_HEAD, *PST_USER_INFO_HEAD;

//扩展昵称
typedef struct _stUserDataExt
{
	WORD				wDataSize;						//数据大小
	WORD				wDataDesc;						//数据描述
}ST_USER_DATA_EXT, *PST_USER_DATA_EXT;

//头像信息
typedef struct _stCustomFaceInfo
{
	DWORD				dwDataSize;						//数据大小
	DWORD				dwCutstomFace[FACE_CX*FACE_CY];	//图片信息
}ST_CUSTOM_FACE_INFO, *PST_CUSTOM_FACE_INFO;

//用户信息
typedef struct _stUserRemoteInfo
{
	//用户信息
	DWORD				dwUserID;						//用户标识
	DWORD				dwGameID;						//游戏标识
	TCHAR				szNickName[LEN_NICKNAME];		//用户昵称

	//等级信息
	BYTE				cbGender;						//用户性别
	BYTE				cbMemberOrder;					//会员等级
	BYTE				cbMasterOrder;					//管理等级

	//位置信息
	WORD				wKindID;						//类型标识
	WORD				wServerID;						//房间标识
	TCHAR				szGameServer[LEN_SERVER];		//房间位置
}ST_USER_REMOTE_INFO, *PST_USER_REMOTE_INFO;

//任务参数
typedef struct _stTaskParam
{
	//基本信息
	WORD				wTaskID;						//任务标识
	WORD				wTaskType;						//任务类型
    WORD				wTaskObject;					//任务目标
	BYTE				cbPlayerType;					//玩家类型
	WORD				wKindID;						//类型标识
	DWORD				dwTimeLimit;					//时间限制

	//奖励信息
	LONGLONG				lStandardAwardGold;				//奖励金币
	LONGLONG				lStandardAwardMedal;			//奖励奖牌
	LONGLONG				lMemberAwardGold;				//奖励金币
	LONGLONG				lMemberAwardMedal;				//奖励奖牌

	//描述信息
	TCHAR				szTaskName[LEN_TASK_NAME];		//任务名称
	TCHAR				szTaskDesc[320];				//任务描述
}ST_TASK_PARAM, *PST_TASK_PARAM;

//任务状态
typedef struct _stTaskStatus
{
	WORD				wTaskID;						//任务标识
	WORD				wTaskProgress;					//任务进度
	BYTE				cbTaskStatus;					//任务状态
}ST_TASK_STATUS, *PST_TASK_STATUS;

//低保参数
typedef struct _stBaseEnsureParam
{
	LONGLONG				lScoreCondition;				//游戏币条件
	LONGLONG				lScoreAmount;					//游戏币数量
	BYTE				cbTakeTimes;					//领取次数
}ST_BASE_ENSURE_PARAM, *PST_BASE_ENSURE_PARAM;

//推广信息
typedef struct _stUserSpreadInfo
{
	DWORD				dwSpreadCount;					//推广人数
	LONGLONG				lSpreadReward;					//推广奖励
}ST_USER_SPREAD_INFO, *PST_USER_SPREAD_INFO;

//等级配置
typedef struct _stGrowLevelConfig
{
	WORD				wLevelID;						//等级ID
	DWORD				dwExperience;					//相应经验
}ST_GROW_LEVEL_CONFIG, *PST_GROW_LEVEL_CONFIG;

//等级参数
typedef struct _stGrowLevelParam
{
	WORD				wCurrLevelID;					//当前等级
	DWORD				dwExperience;					//当前经验
	DWORD				dwUpgradeExperience;			//下级经验
	LONGLONG			lUpgradeRewardGold;				//升级奖励
	LONGLONG			lUpgradeRewardIngot;			//升级奖励
}ST_GROW_LEVEL_PARAM, *PST_GROW_LEVEL_PARAM;

//会员参数
typedef struct _stVipParam
{
	BYTE				cbMemberOrder;					//会员标识
	TCHAR				szMemberName[16];				//会员名称
	LONGLONG			lMemberPrice;					//会员价格
	LONGLONG			lPresentScore;					//赠送游戏币
}ST_VIP_PARAM, *PST_VIP_PARAM;

//房间配置
typedef struct _stGameRoomConfig
{
	//挂接属性
	WORD				wKindID;						//挂接类型
	WORD				wNodeID;						//挂接节点
    WORD				wSortID;						//排列标识

	//税收配置
	WORD				wRevenueRatio;					//税收比例
	LONGLONG			lServiceScore;					//服务费用

	//房间配置
	LONGLONG			lRestrictScore;					//限制积分
	LONGLONG			lMinTableScore;					//最低积分
	LONGLONG			lMinEnterScore;					//最低积分
	LONGLONG			lMaxEnterScore;					//最高积分
	
	//会员限制
	BYTE				cbMinEnterMember;				//最低会员
	BYTE				cbMaxEnterMember;				//最高会员

	//房间属性
	DWORD				dwServerRule;					//房间规则
	TCHAR				szServerName[LEN_SERVER];		//房间名称
}ST_GAME_ROOM_CONFIG, *PST_GAME_ROOM_CONFIG;

//用户信息
typedef struct _stMobileUserHeadInfo
{
	//用户属性
	DWORD				dwGameID;						//游戏 I D
	DWORD				dwUserID;						//用户 I D
    
	//头像信息
	WORD				wFaceID;						//头像索引
	DWORD				dwCustomID;						//自定标识
    
	//用户属性
	BYTE				cbGender;						//用户性别
	BYTE				cbMemberOrder;					//会员等级

	//用户状态
	WORD				wTableID;						//桌子索引
	WORD				wChairID;						//椅子索引
	BYTE				cbUserStatus;					//用户状态

	//积分信息
	LONGLONG			llGameCoin;						//用户游戏币
    LONGLONG            llGold;                         //用户金币
    DOUBLE              dBean;                          //用户游戏豆

	//游戏信息
	DWORD				dwWinCount;						//胜利盘数
	DWORD				dwLostCount;					//失败盘数
	DWORD				dwDrawCount;					//和局盘数
	DWORD				dwFleeCount;					//逃跑盘数
	DWORD				dwExperience;					//用户经验
}ST_MOBILE_USER_HEAD_INFO, *PST_MOBILE_USER_HEAD_INFO;


//广场子项
typedef struct _stGamePlaza
{
	WORD				wPlazaID;						//广场标识
	TCHAR				szServerAddr[32];				//服务地址
	TCHAR				szServerName[32];				//服务器名
}ST_GAME_PLAZA, *PST_GAME_PLAZA;

//级别子项
typedef struct _stLevelItem
{
	INT					lLevelScore;					//级别积分
	TCHAR				szLevelName[16];				//级别描述
}ST_LEVEL_ITEM, *PST_LEVEL_ITEM;

//会员子项
typedef struct _stMemberItem
{
	BYTE				cbMemberOrder;					//等级标识
	TCHAR				szMemberName[16];				//等级名字
}ST_MEMBER_ITEM, *PST_MEMBER_ITEM;

//管理子项
typedef struct _stMasterItem
{
	BYTE				cbMasterOrder;					//等级标识
	TCHAR				szMasterName[16];				//等级名字
}ST_MASTER_ITEM, *PST_MASTER_ITEM;

//列表子项
typedef struct _stColumnItem
{
	BYTE				cbColumnWidth;					//列表宽度
	BYTE				cbColumnType;					//字段类型
	TCHAR				szColumnName[16];				//列表名字
}ST_COLUMN_ITEM, *PST_COLUMN_ITEM;

//地址信息
typedef struct _stAddressInfo
{
	TCHAR				szAddress[32];					//服务器地址
}ST_ADDRESS_INFO, *PST_ADDRESS_INFO;

//数据库信息
typedef struct _stDataBaseParam
{
	WORD				wDataBasePort;					//数据库端口
	TCHAR				szDataBaseAddr[32];				//数据库地址
	TCHAR				szDataBaseUser[32];				//数据库用户
	TCHAR				szDataBasePwd[32];				//数据库密码
	TCHAR				szDataBaseName[32];				//数据库名字
}ST_DATABASE_PARAM, *PST_DATABASE_PARAM;

//转盘奖励，奖项子项
typedef struct _stLotteryItem
{
    BYTE				cbItemIndex;					//奖项索引(1-N)
    BYTE				cbItemType;						//奖励类型(0游戏币，1游戏豆)
    LONGLONG               lItemQuota;						//奖励额度
}ST_LOTTERY_ITEM, *PST_LOTTERY_ITEM;

#pragma pack()

#endif