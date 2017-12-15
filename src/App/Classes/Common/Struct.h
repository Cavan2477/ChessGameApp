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

//游戏类型	tagGameType
typedef struct
{
	WORD				wJoinID;					//挂接索引
	WORD				wSortID;					//排序索引
	WORD				wTypeID;					//类型索引
	TCHAR				szTypeName[LEN_TYPE];		//种类名字
}tagGameType;

//游戏种类	tagGameKind
typedef struct
{
	WORD				wTypeID;					//类型索引
	WORD				wJoinID;					//挂接索引
	WORD				wSortID;					//排序索引
	WORD				wKindID;					//类型索引
	WORD				wGameID;					//模块索引 
	DWORD				dwOnLineCount;				//在线人数
    DWORD				dwAndroidCount;				//在线人数
	DWORD				dwFullCount;				//满员人数
	TCHAR				szKindName[LEN_KIND];		//游戏名字
	TCHAR				szProcessName;				//进程名字
}tagGameKind;

//游戏节点	tagGameNode
typedef struct
{
	WORD				wKindID;					//名称索引
	WORD				wJoinID;					//挂接索引
	WORD				wSortID;					//排序索引
	WORD				wNodeID;					//节点索引
	TCHAR				szNodeName;					//节点名字
}tagGameNode;

//定制类型	tagGamePage
typedef struct
{
	WORD				wPageID;					//页面索引
	WORD				wKindID;					//名称索引
	WORD				wNodeID;					//节点索引
	WORD				wSortID;					//排序索引
	WORD				wOperateType;				//控制类型
	TCHAR				szDisPlayeName[LEN_PAGE];	//显示名称
}tagGamePage;

//游戏房间	tagGameServer
typedef struct
{
	WORD				wKindID;					//名称索引
	WORD				wNodeID;					//节点索引
	WORD				wSortID;					//排序索引
	WORD				wServerID;					//房间索引
    WORD                wServerKind;                //房间类型
    WORD                wServerType;                //房间类型
    WORD                wServerLevel;               //房间等级
	WORD				wServerPort;				//房间端口
    SCORE               lCellScore;                 //单元积分
    SCORE               lEnterScore;                //进入积分
    DWORD				dwServerRule;               //房间规则
	DWORD				dwOnLineCount;				//在线人数
    DWORD               dwAndroidCount;             //机器人数
	DWORD				dwFullCount;				//满员人数
	TCHAR				szServerAddr[32];			//房间名称
	TCHAR				szServerName[LEN_SERVER];	//房间名称
}tagGameServer;

//比赛报名
struct tagSignupMatchInfo
{
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
};

//比赛信息
typedef struct _tagGameMatch
{
	//基本信息
	WORD							wServerID;							//房间标识
    DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
	BYTE							cbMatchType;						//比赛类型
	TCHAR							szMatchName[32];					//比赛名称

	//比赛信息
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMatchFeeType;						//扣费类型
	SCORE							lMatchFee;							//比赛费用
    
	//比赛信息
	WORD							wStartUserCount;					//开赛人数
	WORD							wMatchPlayCount;					//比赛局数
    
    //比赛奖励
    WORD                            wRewardCount;                       //奖励人数
    
	//比赛时间
	SYSTEM_TIME						MatchStartTime;						//开始时间
	SYSTEM_TIME						MatchEndTime;						//结束时间
}tagGameMatch;

//视频配置
struct tagAVServerOption
{
	WORD							wAVServerPort;						//视频端口
	DWORD							dwAVServerAddr;						//视频地址
};

//在线信息
typedef struct
{
	WORD				wKindID;					//类型标识
	DWORD				dwOnLineCount;				//在线人数
}tagOnLineInfoKind;


//在线统计
struct tagOnLineInfoKindEx
{
	WORD							wKindID;							//类型标识
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwAndroidCount;						//机器人数
};

//在线信息
typedef struct
{
	WORD				wServerID;					//房间标识
	DWORD				dwOnLineCount;				//在线人数
}tagOnLineInfoServer;

/////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark 用户信息

//桌子状态	tagTableStatus
typedef struct
{
	BYTE				cbTableLock;				//锁定标志
	BYTE				cbPlayStatus;				//游戏标志
    INT                lCellScore  ;               //单元积分
}tagTableStatus;

//用户状态	tagUserStatus
typedef struct
{
	WORD				wTableID;					//桌子索引
	WORD				wChairID;					//椅子位置
	BYTE				cbUserStatus;				//用户状态
}tagUserStatus;

typedef struct
{
    BYTE               cbCompanion;                 //用户关系
    
}tagUserAttrib;


//用户积分	tagUserScore
typedef	struct
{
	//积分信息
	LONGLONG			lScore;						//用户分数
    LONGLONG            lGrade;                     //用户成绩
	LONGLONG			lBanker;					//银行分数
    LONGLONG            lIngot;                     //用户元宝
	
	//输赢信息
	DWORD				dwWinCount;					//胜利盘数
	DWORD				dwLostCount;				//失败盘数
	DWORD				dwDrawCount;				//和局盘数
	DWORD				dwFleeCount;				//逃跑盘数
	
	//全局信息
	DWORD				dwUserMedal;				//用户奖牌
	DWORD				dwExperience;				//用户经验
	DWORD				dwLoveLiness;				//用户魅力
	
}tagUserScore;

#pragma mark -
#pragma mark 用户积分
typedef struct
{
	//积分信息
	SCORE							lScore;								//用户分数
	DOUBLE                          dBean;                              //用户游戏豆
	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	
	//全局信息
	DWORD							dwExperience;						//用户经验
}tagMobileUserScore;


//道具使用
typedef struct _tagUsePropertyInfo
{
    WORD                            wPropertyCount;                     //道具数目
    WORD                            dwValidNum;                         //有效数字
    DWORD                           dwEffectTime;                       //生效时间
}tagUsePropertyInfo;

//用户道具
struct tagUserProperty
{
    WORD                            wPropertyUseMark;                   //道具标示
    tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];           //使用信息
};

//道具包裹
struct tagPropertyPackage
{
    WORD                            wTrumpetCount;                      //小喇叭数
    WORD                            wTyphonCount;                       //大喇叭数
};

//时间信息
typedef struct _tagTimeInfo
{
	DWORD						dwEnterTableTimer;						//进出桌子时间
	DWORD						dwLeaveTableTimer;						//离开桌子时间
	DWORD						dwStartGameTimer;						//开始游戏时间
	DWORD						dwEndGameTimer;							//离开游戏时间
}tagTimeInfo;

//用户信息	tagUserInfo
typedef struct
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
	SCORE				lScore;							//用户分数
	SCORE				lGrade;							//用户成绩
	SCORE				lInsure;						//银行分数
	SCORE               lIngot;                         //用户元宝
    
	//游戏信息
	DWORD				dwWinCount;						//胜利盘数
	DWORD				dwLostCount;					//失败盘数
	DWORD				dwDrawCount;					//和局盘数
	DWORD				dwFleeCount;					//逃跑盘数
	DWORD				dwExperience;					//用户经验
	INT				dwLoveLiness;					//用户魅力
    
    //时间信息
    tagTimeInfo         TimerInfo;                      //时间信息
}tagUserInfo;

//用户信息 tagUserInfoHead
typedef struct tagUserInfoHead
{
public:
    tagUserInfoHead()
    :dwGameID(0.0),dwUserID(0.0),wFaceID(0.0),dwCustomID(0.0),cbGender('0'),cbMemberOrder('0'),
    wTableID(0.0),wChairID(0.0),cbUserStatus('0'),lScore(0.0),dwWinCount(0.0),dwLostCount(0.0),
    dwDrawCount(0.0),dwFleeCount(0.0),dwExperience(0.0)
    {
        
    }
    
    ~tagUserInfoHead(){}
    
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
	SCORE				lScore;							//用户分数
    SCORE               lGrade;                         //用户成绩
    SCORE               lInsure;                        //用户银行
    SCORE               lIngot;                         //用户元宝
	
	//游戏信息
	DWORD				dwWinCount;						//胜利盘数
	DWORD				dwLostCount;					//失败盘数
	DWORD				dwDrawCount;					//和局盘数
	DWORD				dwFleeCount;					//逃跑盘数
	DWORD				dwExperience;					//用户经验
    INT                lLoveliness;						//用户魅力
}tagUserInfoHead;

//扩展昵称
typedef struct
{
	WORD				wDataSize;						//数据大小
	WORD				wDataDecribe;					//数据描述
}tagDataDescribe;

//头像信息	tagCustomFaceInfo
typedef struct
{
	DWORD				dwDataSize;						//数据大小
	DWORD				dwCutstomFace[FACE_CX*FACE_CY];	//图片信息
}tagCustomFaceInfo;

//用户信息
typedef struct
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    
	//等级信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//位置信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	TCHAR							szGameServer[LEN_SERVER];			//房间位置
}tagUserRemoteInfo;

//任务参数
typedef struct _tagTaskParameter
{
	//基本信息
	WORD							wTaskID;							//任务标识
	WORD							wTaskType;							//任务类型
    WORD							wTaskObject;						//任务目标
	BYTE							cbPlayerType;						//玩家类型
	WORD							wKindID;							//类型标识
	DWORD							dwTimeLimit;						//时间限制

	//奖励信息
	SCORE							lStandardAwardGold;					//奖励金币
	SCORE							lStandardAwardMedal;				//奖励奖牌
	SCORE							lMemberAwardGold;					//奖励金币
	SCORE							lMemberAwardMedal;					//奖励奖牌

	//描述信息
	TCHAR							szTaskName[LEN_TASK_NAME];			//任务名称
	TCHAR							szTaskDescribe[320];				//任务描述
}tagTaskParameter;

//任务状态
typedef struct _tagTaskStatus
{
	WORD							wTaskID;							//任务标识
	WORD							wTaskProgress;						//任务进度
	BYTE							cbTaskStatus;						//任务状态
}tagTaskStatus;

//低保参数
typedef struct _tagBaseEnsureParameter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数
}tagBaseEnsureParameter;

//推广信息
typedef struct _tagUserSpreadInfo
{
	DWORD							dwSpreadCount;						//推广人数
	SCORE							lSpreadReward;						//推广奖励
}tagUserSpreadInfo;

//等级配置
typedef struct
{
	WORD							wLevelID;							//等级 I D
	DWORD							dwExperience;						//相应经验
}tagGrowLevelConfig;

//等级参数
struct tagGrowLevelParameter
{
	WORD							wCurrLevelID;						//当前等级
	DWORD							dwExperience;						//当前经验
	DWORD							dwUpgradeExperience;				//下级经验
	SCORE							lUpgradeRewardGold;					//升级奖励
	SCORE							lUpgradeRewardIngot;				//升级奖励
};

//会员参数
typedef struct 
{
	BYTE							cbMemberOrder;						//会员标识
	TCHAR							szMemberName[16];					//会员名称
	SCORE							lMemberPrice;						//会员价格
	SCORE							lPresentScore;						//赠送游戏币
}tagMemberParameter;


//房间配置
typedef struct
{
	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
    WORD							wSortID;							//排列标识

	//税收配置
	WORD							wRevenueRatio;						//税收比例
	SCORE							lServiceScore;						//服务费用
	//房间配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分
	//会员限制
	BYTE							cbMinEnterMember;					//最低会员
	BYTE							cbMaxEnterMember;					//最高会员

	//房间属性
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称
}tagServerOptionInfo;

//用户信息
typedef struct
{
	//用户属性
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwUserID;							//用户 I D
    
	//头像信息
	WORD							wFaceID;							//头像索引
	DWORD							dwCustomID;							//自定标识
    
	//用户属性
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
    SCORE                           lIngot;                             //元宝
    DOUBLE                          dBean;                              //用户游戏豆
	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验
}tagMobileUserInfoHead;


//广场子项	tagGamePlaza
typedef struct
{
	WORD				wPlazaID;						//广场标识
	TCHAR				szServerAddr[32];				//服务地址
	TCHAR				szServerName[32];				//服务器名
}tagGamePlaza;

//级别子项	tagLevelItem
typedef struct
{
	INT				lLevelScore;					//级别积分
	TCHAR				szLevelName[16];				//级别描述
}tagLevelItem;

//会员子项	tagMemberItem
typedef struct
{
	BYTE				cbMemberOrder;					//等级标识
	TCHAR				szMemberName[16];				//等级名字
}tagMemberItem;

//管理子项	tagMasterItem
typedef struct
{
	BYTE				cbMasterOrder;					//等级标识
	TCHAR				szMasterName[16];				//等级名字
}tagMasterItem;

//列表子项	tagColumnItem
typedef struct
{
	BYTE				cbColumnWidth;					//列表宽度
	BYTE				cbDataDescribe;					//字段类型
	TCHAR				szColumnName[16];				//列表名字
}tagColumnItem;

//地址信息	tagAddressInfo
typedef struct
{
	TCHAR				szAddress[32];					//服务器地址
}tagAddressInfo;

//数据库信息	tagDataBaseParameter
typedef struct
{
	WORD				wDataBasePort;					//数据库端口
	TCHAR				szDataBaseAddr[32];				//数据库地址
	TCHAR				szDataBaseUser[32];				//数据库用户
	TCHAR				szDataBasePass[32];				//数据库密码
	TCHAR				szDataBaseName[32];				//数据库名字
}tagDataBaseParameter;

//转盘奖励，奖项子项
struct tagLotteryItem
{
    BYTE				cbItemIndex;					//奖项索引(1-N)
    BYTE				cbItemType;						//奖励类型(0游戏币，1游戏豆)
    SCORE               lItemQuota;						//奖励额度
};

#pragma pack()

#endif
















 
 
