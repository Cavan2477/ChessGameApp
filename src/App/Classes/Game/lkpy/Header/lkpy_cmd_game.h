#pragma once

#include "cocos2d.h"
#include "define.h"
#include "CocosHeader.h"

LKPY_NAMESPACE_BEGIN
#define PI 3.14
// 1字节对其
#pragma pack(1)

////////////////////////////////////////////////////////////////////////// 
// 服务定义

// 公共宏定义
#define KIND_ID						510								// 游戏 I D

#define GAME_PLAYER					6								// 游戏人数
#define GAME_NAME					TEXT("人鱼奇缘")				// 游戏名字


// 组件属性
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				// 程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				// 程序版本

////////////////////////////////////////////////////////////////////////// 

// 状态定义
#define	GS_BET						GS_PLAYING						// 下注状态
#define	GS_BET_END					GS_PLAYING+1					// 下注结束状态
#define	GS_HORSES					GS_PLAYING+2					// 结束状态

// 游戏消息
#define WM_D3D_RENDER				(WM_USER + 300)					// 渲染消息
#define WM_D3D_LOAD					(WM_USER + 301)					// 载入消息
#define WM_SHOW_CHAT				(WM_USER + 302)					// 聊天消息
#define WM_BEGIN_HELP				(WM_USER + 303)					// 开始帮助消息
#define IDM_CATCH_FISH				(WM_USER + 1000)				// 捕鱼信息
#define IDM_FIRE					(WM_USER + 1001)				// 开火信息
#define IDM_TRADE_BULLET			(WM_USER + 1002)				// 购买信息
#define IDM_UPDATE_CONTROLS			(WM_USER + 1004)				// 更新控件信息
#define IDM_SET						(WM_USER + 1005)				// 设置消息
#define IDM_FIRE_SWIM_OVER			(WM_USER + 1007)				// 游动完结
#define IDM_TEMP_SET				(WM_USER + 1008)				// 临时设置消息
#define IDM_COLOR					(WM_USER + 1009)				// 颜色消息
#define IDM_PROP_USE				(WM_USER + 1010)				// 道具消息
#define IDM_PROP_BUY				(WM_USER + 1011)				// 道具消息
#define IDM_PROP_SELECTED			(WM_USER + 1012)				// 道具消息
#define IDM_BEGIN_LASER				(WM_USER + 1013)				// 准备激光
#define IDM_LASER					(WM_USER + 1014)				// 激光
#define IDM_ACCEPT_TASK				(WM_USER + 1015)				// 接受任务
#define IDM_GIVEUP_TASK				(WM_USER + 1016)				// 放弃任务
#define IDM_COMPLETE_TASK			(WM_USER + 1017)				// 完成任务
#define IDM_GOLD_SELETE				(WM_USER + 1018)				// 金龟选择
#define IDM_FISH_BOMB				(WM_USER + 1019)				// 炸弹爆炸
#define IDM_SPEECH					(WM_USER + 1020)				// 语音消息
#define IDM_MULTIPLE				(WM_USER + 1021)				// 倍数消息
#define IDM_MULTIPLE_DEAL			(WM_USER + 1022)				// 倍数消息

// 鱼索引

// 正常鱼
#define FISH_XIAO_HUANG_YU			0								// 小黄鱼
#define FISH_XIAO_LAN_YU			1								// 小蓝鱼
#define FISH_XIAO_CHOU_YU			2								// 小丑鱼
#define FISH_SI_LU_YU				3								// 丝鲈鱼
#define FISH_SHENG_XIAN_YU			4								// 神仙鱼
#define FISH_HE_TUN_YU				5								// 河豚鱼
#define FISH_DENG_LONG_YU			6								// 灯笼鱼
#define FISH_BA_ZHUA_YU				7								// 八爪鱼
#define FISH_HAI_GUI				8								// 海龟
#define FISH_SHUI_MU				9								// 水母
#define FISH_JIAN_YU				10								// 剑鱼
#define FISH_MO_GUI_YU				11								// 魔鬼鱼
#define FISH_HAI_TUN				12								// 海豚
#define FISH_SHA_YU					13								// 鲨鱼
#define FISH_LAN_JING				14								// 蓝鲸
#define FISH_YIN_JING				15								// 银鲸
#define FISH_JIN_JING				16								// 金鲸
#define FISH_MEI_REN_YU				17								// 美人鱼

// 特殊鱼
#define FISH_ZHA_DAN				18								// 炸弹
#define FISH_XIANG_ZI				19								// 补给箱

// 鱼索引
#define FISH_KING_MAX				7								// 最大灯笼鱼
#define FISH_NORMAL_MAX				18								// 正常鱼索引
#define FISH_ALL_COUNT				20								// 鱼最大数


// 特殊鱼
#define SPECIAL_FISH_BOMB			0								// 炸弹鱼
#define SPECIAL_FISH_CRAB			1								// 螃蟹
#define SPECIAL_FISH_MAX			2								// 最大数量

// 渔网 
#define NET_COLOR_GREEN				0								// 绿色网
#define NET_COLOR_BLUE				1								// 蓝色网
#define NET_COLOR_YELLOW			2								// 黄色网
#define NET_COLOR_RED				3								// 红色网
#define NET_COLOR_PURPLE			4								// 紫色网
#define NET_MAX_COLOR				5								// 最大颜色数(随机值)

// 语音
#define SPEECH_INDEX_MAX			9

// 帮助
#define HELP_OPERATION 				0
#define HELP_FISH					1
#define HELP_BOX 					2
#define HELP_MAX	 				3

// 道具
#define	PROP_ICE_NET				0								// 冰网
#define PROP_BROKEN_ICE				1								// 破冰器
#define	PROP_CLOUDY_AGENT			2								// 混浊剂
#define	PROP_ARMOUR_PIERCING		3								// 穿甲弹
#define	PROP_EJECTION				4								// 弹射弹
#define	PROP_TRACKING				5								// 追踪弹
#define	PROP_SHOTGUN				6								// 散弹
#define	PROP_ACCELERA				7								// 加速弹
#define	PROP_COUNT_MAX				8								// 总数

// 倍数索引
#define MULTIPLE_MAX_INDEX			6	

#define S_TOP_LEFT					0								// 服务器位置
#define S_TOP_CENTER				1								// 服务器位置
#define S_TOP_RIGHT					2								// 服务器位置
#define S_BOTTOM_LEFT				3								// 服务器位置
#define S_BOTTOM_CENTER				4								// 服务器位置
#define S_BOTTOM_RIGHT				5								// 服务器位置

#define C_TOP_LEFT					0								// 视图位置
#define C_TOP_CENTER				1								// 视图位置
#define C_TOP_RIGHT					2								// 视图位置
#define C_BOTTOM_LEFT				3								// 视图位置
#define C_BOTTOM_CENTER				4								// 视图位置
#define C_BOTTOM_RIGHT				5								// 视图位置

// 相对窗口
#define DEFAULE_WIDTH				(1280)							// 客户端相对宽
#define DEFAULE_HEIGHT				(800)							// 客户端相对高	
#define OBLIGATE_LENGTH				(300)							// 预留宽度

#define CAPTION_TOP_SIZE			25								// 标题大小
#define CAPTION_BOTTOM_SIZE			40								// 标题大小

// 音量
#define  MAX_VOLUME					(3000)

////////////////////////////////////////////////////////////////////////// 
// 时间变换
#define  SECOND_TO_MILLISECOND(A)	(A*1000)
#define  MINUTE_TO_MILLISECOND(A)	(A*1000*60)

// 区间随机数
#define	RAND_INDEX					2
#define RAND_INITIAL( nValue, nMin, nMax )		{ nValue[0] = nMin; nValue[1] = nMax; }
#define RAND_MIN_MAX( nValue, nMin, nMax )		{ if( (nMin) >= (nMax) ) { nValue = nMin;} else { nValue = (rand()%((nMax) - (nMin))) + (nMin); } }
#define RAND_EQUAL_MIN_MAX( nMin, nMax )		( ( (rand() + GetTickCount()) % (nMax - nMin) ) + nMin )
#define RAND_TRUE_FALSE( True, False )			( ( (rand() + GetTickCount()) % ((True) + (False)) ) < (DWORD)(True) )

// 最大路径
#define BEZIER_POINT_MAX			8

// 千炮消耗
#define QIAN_PAO_BULLET				1

//////////////////////////////////////////////////////////////////////////
enum EnumScoreType
{
    EST_Cold,					// 金币
    EST_Laser,					// 激光
    EST_Speed,					// 加速
    EST_Gift,					// 赠送
    EST_Null,					// 空
};

// 房间类型
enum EnumRoomType
{
	ERT_Unknown = 0,									// 无效
	ERT_QianPao,										// 千炮
	ERT_MoNi,											// 模拟
};

// 道具类型
enum EnumPropObjectType
{
	POT_NULL = 0,										// 无效
	POT_ATTACK,											// 攻击
	POT_DEFENSE,										// 防御
	POT_BULLET,											// 子弹
};

// 炮弹
#define BULLET_ONE				0								// 一号炮
#define BULLET_TWO				1								// 二号炮
#define BULLET_THREE			2								// 三号炮
#define BULLET_FOUR				3								// 四号炮
#define BULLET_FIVE				4								// 五号炮
#define BULLET_SIX				5								// 六号炮
#define BULLET_SEVEN			6								// 七号炮
#define BULLET_EIGHT			7								// 八号炮
#define BULLET_MAX				8								// 最大炮种类

// 游戏玩家
enum
{
    PlayChair_Max				= 6,
    PlayChair_Invalid			= 0xffff,
    PlayName_Len				= 32,
    QianPao_Bullet				= 1,							// 千炮消耗
    Multiple_Max				= 6,							// 最大倍数
};


enum EnumFishType
{
    FishType_XiaoHuangCiYu		= 0,							// 小黄刺鱼
    FishType_XiaoCaoYu			= 1,							// 小草鱼
    FishType_ReDaiHuangYu		= 2,							// 热带黄鱼
    FishType_DaYanJinYu			= 3,							// 大眼金鱼
    FishType_ReDaiZiYu			= 4,							// 热带紫鱼
    FishType_XiaoChouYu			= 5,							// 小丑鱼
    FishType_HeTun				= 6,							// 河豚
    FishType_ShiTouYu			= 7,							// 狮头鱼
    FishType_DengLongYu			= 8,							// 灯笼鱼
    FishType_WuGui				= 9,							// 乌龟
    FishType_ShengXianYu		= 10,							// 神仙鱼
    FishType_HuDieYu			= 11,							// 蝴蝶鱼
    FishType_LingDangYu			= 12,							// 铃铛鱼
    FishType_JianYu				= 13,							// 剑鱼
    FishType_MoGuiYu			= 14,							// 魔鬼鱼
    FishType_DaBaiSha			= 15,							// 大白鲨
    FishType_DaJinSha			= 16,							// 大金鲨
    FishType_ShuangTouQiEn		= 17,							// 双头企鹅
    FishType_JuXingHuangJinSha  = 18,							// 巨型黄金鲨
    FishType_JinLong			= 19,							// 金龙
    FishType_LiKui				= 20,							// 李逵
    FishType_ShuiHuZhuan		= 21,							// 水浒传
    FishType_ZhongYiTang		= 22,							// 忠义堂
    FishType_BaoZhaFeiBiao		= 23,							// 爆炸飞镖
    FishType_BaoXiang			= 24,							// 宝箱
    FishType_General_Max		= 21,							// 普通鱼最大
    FishType_Normal_Max			= 24,							// 正常鱼最大
    FishType_Max				= 25,							// 最大数量
    FishType_Small_Max			= 9,							// 小鱼最大索引
    FishType_Moderate_Max		= 15,							// 中鱼索
    FishType_Moderate_Big_Max	= 18,							// 中大鱼索
    FishType_Big_Max			= 24,							// 大鱼索引
    FishType_Invalid			= -1,							// 无效鱼
};

//顶点类
struct CDoublePoint
{
	double				x;
	double				y;

	CDoublePoint() {  x = 0.0; y = 0.0; }
	CDoublePoint( cocos2d::Point & Par ) {  x = Par.x; y = Par.y; }
	CDoublePoint( CDoublePoint& Par ) {  x = Par.x; y = Par.y; }
	CDoublePoint( double ParX, double ParY ) { x = ParX; y = ParY; }
	void operator+= (CDoublePoint& point) { x += point.x; y += point.y; }
	bool operator!= (CDoublePoint& point) { return (x != point.x || y != point.y); }
	inline void SetPoint( double ParX, double ParY ) { x = ParX; y = ParY; }
};

struct CShortPoint
{
	short				x;
	short				y;

	CShortPoint() {  x = 0; y = 0; }
	CShortPoint( cocos2d::Point & Par ) {  x = (short)Par.x; y = (short)Par.y; }
	CShortPoint( CShortPoint& Par ) {  x = Par.x; y = Par.y; }
	CShortPoint( short ParX, short ParY ) { x = ParX; y = ParY; }
	void operator+= (CShortPoint& point) { x += point.x; y += point.y; }
	inline void SetPoint( short ParX, short ParY ) { x = ParX; y = ParY; }

};

struct tagBezierPoint
{
	CDoublePoint			BeginPoint;
	CDoublePoint			EndPoint;
	CDoublePoint			KeyOne;
	CDoublePoint			KeyTwo;
	DWORD					Time;
};

// 载入需要信息
struct tagLoad
{
	void *							pGLDevice;
	void *							pCWnd;
	void *							pHdc;
	void *							pHrc;
	int								nIndex;
};

// RGB 颜色
struct COLORRGBM
{
	BYTE							R;									// 颜色色相
	BYTE							G;									// 颜色饱和
	BYTE							B;									// 颜色亮度
};

// HSB 颜色
struct COLORHSBM
{
	WORD							H;									// 颜色色相
	double							S;									// 颜色饱和
	double							B;									// 颜色亮度
};


struct GameScene
{
    BYTE			cbBackIndex;							// 背景索引
    SCORE			lPlayScore;								// 玩家积分
    SCORE			lPlayCurScore[GAME_PLAYER];				// 玩家积分
    SCORE			lPlayStartScore[GAME_PLAYER];			// 玩家积分
    
    int					nBulletVelocity;						// 子弹速度
    int					nBulletCoolingTime;						// 子弹冷却
    int					nFishMultiple[FishType_Max][2];		// 鱼倍数
    
    SCORE			lBulletConsume[PlayChair_Max];			// 子弹消耗
    int				lPlayFishCount[PlayChair_Max][FishType_Max];	// 玩家捕获鱼数
    int					nMultipleValue[Multiple_Max];		// 房间倍数
    int					nMultipleIndex[PlayChair_Max];			// 当前倍数
    bool				bUnlimitedRebound;						// 无限反弹
    TCHAR				szBrowseUrl[256];						// 充值地址
};

////////////////////////////////////////////////////////////////////////// 
// 服务器命令结构
//enum
//{
//	SUB_S_SYNCHRONOUS = 101,				// 同步信息
//	SUB_S_FISH_CREATE,						// 鱼创建
//	SUB_S_FAST_MOVE,						// 快速移动
//	SUB_S_FISH_CATCH,						// 捕获鱼
//	SUB_S_FIRE,								// 开火
//	SUB_S_TRADE_BULLET,						// 换子弹	
//	SUB_S_EXCHANGE_SCENE,					// 转换场景
//	SUB_S_OVER,								// 结算
//	SUB_S_FISH_FINISH,						// 创建完成
//	SUB_S_DELAY_BEGIN,						// 延迟
//	SUB_S_DELAY,							// 延迟
//	SUB_S_USE_PROP,							// 道具
//	SUB_S_BUY_PROP,							// 购买道具
//	SUB_S_SELECTED_PROP,							// 选中道具
//	SUB_S_BEGIN_LASER,						// 准备激光
//	SUB_S_LASER,							// 激光
//	SUB_S_BANK_TAKE,						// 银行取款
//	SUB_S_SPEECH,							// 语音消息
//	SUB_S_SYSTEM,							// 系统消息
//	SUB_S_MULTIPLE,							// 倍数消息
//	SUB_S_SUPPLY_TIP,						// 补给提示
//	SUB_S_SUPPLY,							// 补给消息
//	SUB_S_AWARD_TIP,						// 分数提示
//    SUB_S_CONTROL,                          // 控制消息
//    SUB_S_UPDATE_GAME,                      // 更新游戏
//};

// 服务器命令结构
enum
{
    SUB_S_SYNCHRONOUS = 101,				// 同步信息101
    SUB_S_FISH_CREATE,						// 鱼创建102
    SUB_S_FISH_CATCH,						// 捕获鱼103
    SUB_S_FIRE,								// 开火104
    SUB_S_EXCHANGE_SCENE,					// 转换场景105
    SUB_S_OVER,								// 结算106
    SUB_S_DELAY_BEGIN,						// 延迟107
    SUB_S_DELAY,							// 延迟108
    SUB_S_BEGIN_LASER,						// 准备激光109
    SUB_S_LASER,							// 激光110
    SUB_S_BANK_TAKE,						// 银行取款111
    SUB_S_SPEECH,							// 语音消息112
    SUB_S_SYSTEM,							// 系统消息113
    SUB_S_MULTIPLE,							// 倍数消息114
    SUB_S_SUPPLY_TIP,						// 补给提示115
    SUB_S_SUPPLY,							// 补给消息116
    SUB_S_AWARD_TIP,						// 分数提示117
    SUB_S_CONTROL,							// 控制消息118
    SUB_S_UPDATE_GAME,						// 更新游戏119
    SUB_S_STAY_FISH,						// 停留鱼120
};

enum EnumFishState
{
    FishState_Normal,		// 普通鱼
    FishState_King,			// 鱼王
    FishState_Killer,		// 杀手鱼
    FishState_Aquatic,		// 水草鱼
};

// 系统消息
struct CMD_S_System
{
	double				dRoomIncome;			// 房间收入
	double				dWholeTax;				// 全部税收
	double				dTableInvest;			// 桌子收入
	double				dTableOutput;			// 桌子产出
	double				dPlayInvest;			// 玩家收入
	double				dPlayOutput;			// 玩家产出
	double				dTableIdle;				// 桌子空闲
};

// 鱼创建
struct CMD_S_FishCreate
{
    // 基本信息
    UINT				nFishKey;				// 鱼关键值
    UINT				unCreateTime;			// 创建时间
    WORD				wHitChair;				// 击杀位置
    BYTE				nFishType;				// 鱼种类
    EnumFishState		nFishState;				// 鱼状态
    
    // 路径偏移
    float				fRotateAngle;			// 角度
    CShortPoint 		PointOffSet;			// 偏移
    
    // 初始角度
    float				fInitialAngle;			// 角度
    
    // 随机路径
    int					nBezierCount;			// 路径数量
    tagBezierPoint		TBezierPoint[BEZIER_POINT_MAX];		// 路径信息
};

// 鱼鱼创建完成
struct CMD_S_FishFinish
{
	DWORD				nOffSetTime;			// 偏移时间	
};

// 捕获鱼
//struct CMD_S_CatchFish
//{
//	int					nFishIndex;				// 鱼索引
//	WORD				wChairID;				// 玩家位置
//	LONGLONG			lScoreCount;			// 获得数量
//	EnumScoreType		nScoreType;				// 获得类型
//};

struct CMD_S_CatchFish
{
    UINT				nFishIndex;				// 鱼索引
    WORD				wChairID;				// 玩家位置
    int					nMultipleCount;			// 倍数数量
    SCORE			lScoreCount;			// 获得数量
};
// 开火
//struct CMD_S_Fire
//{
//	int					nBulletKey;				// 子弹关键值
//	int					nBulletScore;			// 子弹分数
//	BYTE				cbBulletIndex;			// 炮弹索引
//	DWORD				nPropIndex;				// 道具索引
//	int					nTrackFishIndex;		// 追踪鱼索引
//	DWORD			crFishNetColoer;		// 渔网颜色
//	WORD				wChairID;				// 玩家位置
//	CShortPoint			ptPos;					// 位置
//};

struct CMD_S_Fire
{
    UINT				nBulletKey;				// 子弹关键值
    int					nBulletScore;			// 子弹分数
    int					nMultipleIndex;			// 倍数索引
    int					nTrackFishIndex;		// 追踪鱼索引
    WORD				wChairID;				// 玩家位置
    CShortPoint			ptPos;					// 位置
};

// 准备激光
struct CMD_S_BeginLaser
{
	WORD				wChairID;				// 玩家位置
	CShortPoint			ptPos;					// 位置
};

// 激光
struct CMD_S_Laser
{
	WORD				wChairID;				// 玩家位置
	CShortPoint			ptPos;					// 位置
};

// 激光奖励
struct CMD_S_LaserReward
{
	WORD				wChairID;				// 玩家位置
	LONGLONG			lScore;					// 分数奖励
};

// 换炮台
struct CMD_S_TradeBullet
{
	WORD				wChairID;				// 玩家位置
	BYTE				cbBulletIndex;			// 换子弹索引
};

// 换网颜色
struct CMD_S_TradeNetColor
{
	WORD				wChairID;				// 玩家位置
	BYTE				cbNetColorIndex;		// 换网索引
};

// 转换场景
struct CMD_S_ExchangeScene
{
	BYTE				cbBackIndex;			// 背景索引
};

// 结算
struct CMD_S_Over
{
	WORD				wChairID;				// 玩家位置
};

// 延迟
struct CMD_S_Delay
{
	int					nDelay;					// 延迟
	WORD				wChairID;				// 玩家位置
};

// 选中道具
struct CMD_S_SelectedProp
{
	WORD				wChairID;				// 玩家位置
	DWORD				nPropIndex;				// 道具索引
};

// 使用道具
struct CMD_S_UseProp
{
	WORD				wUseChairID;			// 使用玩家位置
	WORD				wTargetChairID;			// 目标玩家位置
	DWORD				nPropIndex;				// 道具索引
};

// 购买道具
struct CMD_S_BuyProp
{
	WORD				wChairID;				// 玩家位置
	DWORD				nPropCount;				// 道具数量
	DWORD				nPropIndex;				// 道具索引
	LONGLONG			lPropScore;				// 道具消耗
};

// 银行去看
struct CMD_S_BankTake
{
	WORD				wChairID;				// 玩家位置
	LONGLONG			lPlayScore;				// 用户积分
};

// 语音消息
struct CMD_S_Speech
{
	WORD				wChairID;				// 玩家位置
	int					nSpeechIndex;			// 语音索引
};

// 倍数选择
struct CMD_S_Multiple
{
	WORD				wChairID;				// 玩家位置
	int					nMultipleIndex;			// 倍数索引
};

// 补给提示
struct CMD_S_SupplyTip
{
	WORD				wChairID;				// 玩家位置
};

// 补给信息
// 补给信息
struct CMD_S_Supply
{
    WORD				wChairID;				// 玩家位置
    SCORE			lSupplyCount;			// 补给数量
    EnumScoreType		nSupplyType;			// 补给类型
};

// 提示消息
struct CMD_S_AwardTip
{
	WORD				wTableID;				// 桌子ID
	WORD				wChairID;				// 位置ID
	TCHAR				szPlayName[32];			// 用户名
	BYTE				nFishType;				// 鱼种类
	int					nFishMultiple;			// 鱼倍数
	LONGLONG			lFishScore;				// 具体分数
	EnumScoreType		nScoreType;				// 分数类型
};

//更新游戏
struct CMD_S_UpdateGame
{
    int					nMultipleValue[MULTIPLE_MAX_INDEX];		// 房间倍数
    int					nCatchFishMultiple[FISH_ALL_COUNT][RAND_INDEX];		// 捕鱼倍数
    bool				bUnlimitedRebound;						// 无限反弹
};

// 停留鱼
struct CMD_S_StayFish
{
    UINT				nFishKey;							// 鱼索引
    UINT				nStayStart;							// 停留开始
    UINT				nStayTime;							// 停留时间
};


////////////////////////////////////////////////////////////////////////// 
// 客户端命令结构
enum
{
    SUB_C_CATCH_FISH = 101,							// 捕鱼信息
    SUB_C_FIRE,										// 开火
    SUB_C_DELAY,									// 延迟
    SUB_C_BEGIN_LASER,								// 准备激光
    SUB_C_LASER,									// 激光
    SUB_C_SPEECH,									// 语音消息
    SUB_C_MULTIPLE,									// 倍数消息
    SUB_C_CONTROL,									// 控制消息
};

// 捕中最大数量
#define FishCatch_Max			5

struct CMD_C_CatchFish
{
    UINT				nBulletKey;					// 子弹关键值
    UINT				nFishKey[FishCatch_Max];	// 捕中鱼
};
// 开火
//struct CMD_C_Fire
//{
//	DWORD			crFishNetColoer;			// 渔网颜色
//	DWORD				nPropIndex;					// 道具索引
//	int					nTrackFishIndex;			// 追踪鱼索引
//	int					nBulletKey;					// 子弹关键值
//	CShortPoint			ptPos;						// 位置
//};

struct CMD_C_Fire
{
    int					nMultipleIndex;				// 倍数索引
    int					nTrackFishIndex;			// 追踪鱼索引
    UINT				nBulletKey;					// 子弹关键值
    CShortPoint			ptPos;						// 位置
};

// 准备激光
struct CMD_C_BeginLaser
{
	CShortPoint			ptPos;					// 位置
};

// 激光
struct CMD_C_Laser
{
	CShortPoint			ptBeginPos;					// 开始位置
	CShortPoint			ptEndPos;					// 结束位置
	DWORD				unLossTime;					// 已过时间
};

// 购买子弹
struct CMD_C_TradeBullet
{
	BYTE				cbBulletIndex;			// 子弹索引
};

// 换网颜色
struct CMD_C_TradeNetColor
{
	BYTE				cbNetColorIndex;		// 换网索引
};

// 道具使用
struct CMD_C_UseProp
{
	WORD				wChairID;				// 目标玩家
	DWORD				nPropIndex;				// 道具索引
};

// 选中道具
struct CMD_C_SelectedProp
{
	DWORD				nPropIndex;				// 道具索引
};

// 购买道具
struct CMD_C_BuyProp
{
	DWORD				nPropCount;				// 道具数量
	DWORD				nPropIndex;				// 道具索引
};


// 选择金龟
struct CMD_C_SeleteTurtle
{
	int					nSeleteIndex;			// 选择索引
};

// 爆炸鱼
struct CMD_C_FishBomb
{
	DWORD				unLossTime;				// 已过时间
};

// 语音消息
struct CMD_C_Speech
{
	int					nSpeechIndex;			// 语音索引
};

// 倍数消息
struct CMD_C_Multiple
{
	int					nMultipleIndex;			// 语音索引
};


////////////////////////////////////////////////////////////////////////// 
// 自定义配置
struct tagCustomRule
{
	// 房间类型
	// EnumRoomType
	int						bRoomQianPao;							// 千炮
	int						bRoomMoNi;								// 模拟

	// 倍数配置
	int						nMultipleValue[MULTIPLE_MAX_INDEX];		// 房间倍数

	// 控制配置
	int						nInitialStock;							// 初始声音
	int						nTaxRatio;								// 抽水比例
	int						nDifficultyTableHardBig;				// 超难难度
	int						nDifficultyTableHardSmall;				// 困难难度
	int						nDifficultyTableSimpleSmall;			// 简单难度
	int						nDifficultyTableSimpleBig;				// 超简难度
	int						nDifficultyPlayHardBig;					// 超难难度
	int						nDifficultyPlayHardSmall;				// 困难难度
	int						nDifficultyPlaySimpleSmall;				// 简单难度
	int						nDifficultyPlaySimpleBig;				// 超简难度

	// 场景配置
	int						nCreateTime;							// 创建时间
	int						nCreateCount;							// 创建数量
	int						nSceneTime;								// 场景维持时间

	// 活动配置
	int						nSupplyCondition[2];					// 补给条件
	int						nLaserTime;								// 激光时间
	int						nLaserChance;							// 激光几率
	int						nBombChance;							// 爆炸几率
	int						nSpeedTime;								// 加速时间
	int						nSpeedChance;							// 加速几率
	int						nStrongTime;							// 强化时间
	int						nStrongChance;							// 强化几率
	int						nGiftScore[5];							// 赠送金币
	int						nGiftChance[5];							// 赠送几率
	int						nNullChance;							// 空箱几率


	// 子弹配置
	int						nBulletMoney[BULLET_MAX];				// 子弹价格
	int						nBulletVelocity[BULLET_MAX];			// 子弹速度
	int						nBulletCoolingTime[BULLET_MAX];			// 子弹冷却
	int						nBulletRange[BULLET_MAX];				// 子弹射程
	int						nBulletScope[BULLET_MAX];				// 子弹范围

	// 道具配置
	DWORD					nPropConsume[PROP_COUNT_MAX];			// 道具

	// 鱼配置
	int						nCatchFishMultiple[FISH_ALL_COUNT][RAND_INDEX];		// 捕鱼倍数

	// 奖牌配置
	int						nMedalCondition[RAND_INDEX];			// 奖牌条件
	int						nMedalCount[RAND_INDEX];				// 奖牌数量

	// 无负设置
	int						nEnsureWin;								// 保证赢


	tagCustomRule()
	{
		DefaultCustomRule();
	}

	// 默认配置
	void DefaultCustomRule()
	{
		// 房间类型
		bRoomQianPao = true;
		bRoomMoNi = false;

		// 房间倍数
		int nTempMultipleValue[MULTIPLE_MAX_INDEX] = { 1, 10, 100, 500, 1000, 5000 };
		memcpy( nMultipleValue, nTempMultipleValue, sizeof(nMultipleValue) );

		// 控制配置
		nInitialStock = 0;
		nTaxRatio = 3;
		nDifficultyTableHardBig = 50;
		nDifficultyTableHardSmall = 250;
		nDifficultyTableSimpleSmall = 1000;
		nDifficultyTableSimpleBig = 3000;
		nDifficultyPlayHardBig = 2000;
		nDifficultyPlayHardSmall = 1000;
		nDifficultyPlaySimpleSmall = -1000;
		nDifficultyPlaySimpleBig = -2000;

		// 场景配置
		nCreateTime = 10;
		nCreateCount = 6;
		nSceneTime = 60 * 8;

		// 活动配置
		RAND_INITIAL(nSupplyCondition, 500, 700);
		nLaserTime = 30;
		nLaserChance = 10;
		nBombChance = 10;
		nSpeedTime = 60;
		nSpeedChance = 60;
		nStrongTime = 30;
		nStrongChance = 60;
		nGiftScore[0] = 10;
		nGiftScore[1] = 30;
		nGiftScore[2] = 50;
		nGiftScore[3] = 80;
		nGiftScore[4] = 120;
		nGiftChance[0] = 5;
		nGiftChance[1] = 5;
		nGiftChance[2] = 5;
		nGiftChance[3] = 5;
		nGiftChance[4] = 5;
		nNullChance = 5;

		// 子弹配置
		int	nTempBulletMoney[BULLET_MAX] = { 1, 2, 3, 4, 5, 6, 7, 8  };
		int	nTempBulletVelocity[BULLET_MAX] = { 6000, 6500, 7000, 7500, 8000, 8500, 9000, 15000 };
		int	nTempBulletCoolingTime[BULLET_MAX] = { 650, 600, 550, 500, 450, 400, 350, 150 };
		int	nTempBulletRange[BULLET_MAX] = { 475, 500, 525, 550, 575, 600, 625, 650 };
		int	nTempBulletScope[BULLET_MAX] = { 50, 55, 60, 65, 70, 75, 80, 85 };
		memcpy(nBulletMoney, nTempBulletMoney, sizeof(nBulletMoney));
		memcpy(nBulletVelocity, nTempBulletVelocity, sizeof(nBulletVelocity));
		memcpy(nBulletCoolingTime, nTempBulletCoolingTime, sizeof(nBulletCoolingTime));
		memcpy(nBulletRange, nTempBulletRange, sizeof(nBulletRange));
		memcpy(nBulletScope, nTempBulletScope, sizeof(nBulletScope));

		// 道具配置
		DWORD nTempPropConsume[PROP_COUNT_MAX] = { 100, 100, 100, 8, 10, 10, 10, 10 };
		memcpy(nPropConsume, nTempPropConsume, sizeof(nPropConsume));

		// 鱼配置
		int	nTempFishMultiple[FISH_ALL_COUNT][RAND_INDEX] = { { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 8, 8 }, { 10, 10 }, { 15, 15 }, { 20, 20 }, { 10, 45 }, { 25, 25 }, { 30, 30 }, { 40, 40 }, { 50, 50 }, { 100, 100 }, { 200, 200 }, { 300, 300 }, { 200, 1000 }, { 0, 0 }, { 0, 0 } };
		memcpy(nCatchFishMultiple, nTempFishMultiple, sizeof(nCatchFishMultiple));

		// 奖牌配置
		RAND_INITIAL(nMedalCondition, 200, 255);
		RAND_INITIAL(nMedalCount, 0, 0);

		// 保证赢
		nEnsureWin = 0;
	}
};


//声音宏
#define DATA_FOMAT .mp3
#define DATA_PATH sounds/mp3/
#define TO_STRING1( x ) #x
#define TO_STRING( x ) TO_STRING1( x )



#define AUDIO_PATH_EFFECT sound_res/
#define AUDIO_PATH_NULL
#define AUDIO_DIAN .

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define AUDIO_FORMAT .ogg
#else
#define AUDIO_FORMAT .wav
#endif


#define BEAUTY_0 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_0" TO_STRING(AUDIO_FORMAT) ""
#define BEAUTY_1 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_1" TO_STRING(AUDIO_FORMAT) ""
#define BEAUTY_2 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_2" TO_STRING(AUDIO_FORMAT) ""
#define BEAUTY_3 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_3" TO_STRING(AUDIO_FORMAT) ""
#define BIG_7 "" TO_STRING(AUDIO_PATH_EFFECT) "big_7" TO_STRING(AUDIO_FORMAT) ""
#define BIG_8 "" TO_STRING(AUDIO_PATH_EFFECT) "big_8" TO_STRING(AUDIO_FORMAT) ""
#define BIG_9 "" TO_STRING(AUDIO_PATH_EFFECT) "big_9" TO_STRING(AUDIO_FORMAT) ""
#define BIG_10 "" TO_STRING(AUDIO_PATH_EFFECT) "big_10" TO_STRING(AUDIO_FORMAT) ""
#define BIG_11 "" TO_STRING(AUDIO_PATH_EFFECT) "big_11" TO_STRING(AUDIO_FORMAT) ""
#define BIG_12 "" TO_STRING(AUDIO_PATH_EFFECT) "big_12" TO_STRING(AUDIO_FORMAT) ""
#define BIG_13 "" TO_STRING(AUDIO_PATH_EFFECT) "big_13" TO_STRING(AUDIO_FORMAT) ""
#define BIG_14 "" TO_STRING(AUDIO_PATH_EFFECT) "big_14" TO_STRING(AUDIO_FORMAT) ""
#define BIG_15 "" TO_STRING(AUDIO_PATH_EFFECT) "big_15" TO_STRING(AUDIO_FORMAT) ""
#define BIG_16 "" TO_STRING(AUDIO_PATH_EFFECT) "big_16" TO_STRING(AUDIO_FORMAT) ""
#define CHANGE_SCENE "" TO_STRING(AUDIO_PATH_EFFECT) "CHANGE_SCENE" TO_STRING(AUDIO_FORMAT) ""
#define COINANIMATION "" TO_STRING(AUDIO_PATH_EFFECT) "CoinAnimation" TO_STRING(AUDIO_FORMAT) ""
#define COINFLY "" TO_STRING(AUDIO_PATH_EFFECT) "coinfly" TO_STRING(AUDIO_FORMAT) ""
#define COINLIGHTMOVE "" TO_STRING(AUDIO_PATH_EFFECT) "CoinLightMove" TO_STRING(AUDIO_FORMAT) ""
#define combo "" TO_STRING(AUDIO_PATH_EFFECT) "combo" TO_STRING(AUDIO_FORMAT) ""
#define fish_special "" TO_STRING(AUDIO_PATH_EFFECT) "fish_special" TO_STRING(AUDIO_FORMAT) ""
#define LOAD_BACK "" TO_STRING(AUDIO_PATH_EFFECT) "LOAD_BACK" TO_STRING(AUDIO_FORMAT) ""
#define MUSIC_BACK_01 "" TO_STRING(AUDIO_PATH_EFFECT) "MUSIC_BACK_01" TO_STRING(AUDIO_FORMAT) ""
#define MUSIC_BACK_02 "" TO_STRING(AUDIO_PATH_EFFECT) "MUSIC_BACK_02" TO_STRING(AUDIO_FORMAT) ""
#define MUSIC_BACK_03 "" TO_STRING(AUDIO_PATH_EFFECT) "MUSIC_BACK_03" TO_STRING(AUDIO_FORMAT) ""
#define PROP_ARMOUR_PIERCING "" TO_STRING(AUDIO_PATH_EFFECT) "PROP_ARMOUR_PIERCING" TO_STRING(AUDIO_FORMAT) ""
#define SHELL_8 "" TO_STRING(AUDIO_PATH_EFFECT) "SHELL_8" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_0 "" TO_STRING(AUDIO_PATH_EFFECT) "small_0" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_1 "" TO_STRING(AUDIO_PATH_EFFECT) "small_1" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_2 "" TO_STRING(AUDIO_PATH_EFFECT) "small_2" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_3 "" TO_STRING(AUDIO_PATH_EFFECT) "small_3" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_4 "" TO_STRING(AUDIO_PATH_EFFECT) "small_4" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_5 "" TO_STRING(AUDIO_PATH_EFFECT) "small_5" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_BEGIN "" TO_STRING(AUDIO_PATH_EFFECT) "SMALL_BEGIN" TO_STRING(AUDIO_FORMAT) ""
#define SMASHFAIL "" TO_STRING(AUDIO_PATH_EFFECT) "SmashFail" TO_STRING(AUDIO_FORMAT) ""#define BEAUTY_0 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_0" TO_STRING(AUDIO_FORMAT) ""
#define BEAUTY_1 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_1" TO_STRING(AUDIO_FORMAT) ""
#define BEAUTY_2 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_2" TO_STRING(AUDIO_FORMAT) ""
#define BEAUTY_3 "" TO_STRING(AUDIO_PATH_EFFECT) "beauty_3" TO_STRING(AUDIO_FORMAT) ""
#define BIG_7 "" TO_STRING(AUDIO_PATH_EFFECT) "big_7" TO_STRING(AUDIO_FORMAT) ""
#define BIG_8 "" TO_STRING(AUDIO_PATH_EFFECT) "big_8" TO_STRING(AUDIO_FORMAT) ""
#define BIG_9 "" TO_STRING(AUDIO_PATH_EFFECT) "big_9" TO_STRING(AUDIO_FORMAT) ""
#define BIG_10 "" TO_STRING(AUDIO_PATH_EFFECT) "big_10" TO_STRING(AUDIO_FORMAT) ""
#define BIG_11 "" TO_STRING(AUDIO_PATH_EFFECT) "big_11" TO_STRING(AUDIO_FORMAT) ""
#define BIG_12 "" TO_STRING(AUDIO_PATH_EFFECT) "big_12" TO_STRING(AUDIO_FORMAT) ""
#define BIG_13 "" TO_STRING(AUDIO_PATH_EFFECT) "big_13" TO_STRING(AUDIO_FORMAT) ""
#define BIG_14 "" TO_STRING(AUDIO_PATH_EFFECT) "big_14" TO_STRING(AUDIO_FORMAT) ""
#define BIG_15 "" TO_STRING(AUDIO_PATH_EFFECT) "big_15" TO_STRING(AUDIO_FORMAT) ""
#define BIG_16 "" TO_STRING(AUDIO_PATH_EFFECT) "big_16" TO_STRING(AUDIO_FORMAT) ""
#define CHANGE_SCENE "" TO_STRING(AUDIO_PATH_EFFECT) "CHANGE_SCENE" TO_STRING(AUDIO_FORMAT) ""
#define COINANIMATION "" TO_STRING(AUDIO_PATH_EFFECT) "CoinAnimation" TO_STRING(AUDIO_FORMAT) ""
#define COINFLY "" TO_STRING(AUDIO_PATH_EFFECT) "coinfly" TO_STRING(AUDIO_FORMAT) ""
#define COINLIGHTMOVE "" TO_STRING(AUDIO_PATH_EFFECT) "CoinLightMove" TO_STRING(AUDIO_FORMAT) ""
#define combo "" TO_STRING(AUDIO_PATH_EFFECT) "combo" TO_STRING(AUDIO_FORMAT) ""
#define fish_special "" TO_STRING(AUDIO_PATH_EFFECT) "fish_special" TO_STRING(AUDIO_FORMAT) ""
#define LOAD_BACK "" TO_STRING(AUDIO_PATH_EFFECT) "LOAD_BACK" TO_STRING(AUDIO_FORMAT) ""
#define MUSIC_BACK_01 "" TO_STRING(AUDIO_PATH_EFFECT) "MUSIC_BACK_01" TO_STRING(AUDIO_FORMAT) ""
#define MUSIC_BACK_02 "" TO_STRING(AUDIO_PATH_EFFECT) "MUSIC_BACK_02" TO_STRING(AUDIO_FORMAT) ""
#define MUSIC_BACK_03 "" TO_STRING(AUDIO_PATH_EFFECT) "MUSIC_BACK_03" TO_STRING(AUDIO_FORMAT) ""
#define PROP_ARMOUR_PIERCING "" TO_STRING(AUDIO_PATH_EFFECT) "PROP_ARMOUR_PIERCING" TO_STRING(AUDIO_FORMAT) ""
#define SHELL_8 "" TO_STRING(AUDIO_PATH_EFFECT) "SHELL_8" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_0 "" TO_STRING(AUDIO_PATH_EFFECT) "small_0" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_1 "" TO_STRING(AUDIO_PATH_EFFECT) "small_1" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_2 "" TO_STRING(AUDIO_PATH_EFFECT) "small_2" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_3 "" TO_STRING(AUDIO_PATH_EFFECT) "small_3" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_4 "" TO_STRING(AUDIO_PATH_EFFECT) "small_4" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_5 "" TO_STRING(AUDIO_PATH_EFFECT) "small_5" TO_STRING(AUDIO_FORMAT) ""
#define SMALL_BEGIN "" TO_STRING(AUDIO_PATH_EFFECT) "SMALL_BEGIN" TO_STRING(AUDIO_FORMAT) ""
#define SMASHFAIL "" TO_STRING(AUDIO_PATH_EFFECT) "SmashFail" TO_STRING(AUDIO_FORMAT) ""
// 还原对其数
#pragma pack()

////////////////////////////////////////////////////////////////////////// 
LKPY_NAMESPACE_END