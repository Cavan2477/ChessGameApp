#ifndef __nn_GameLogicDefine_H__
#define __nn_GameLogicDefine_H__

#include "cocos2d.h"
#include "define.h"
#include "CMD_Game.h"
namespace nn_game{
//排序类型
#define ST_ORDER					1									//大小排序
#define ST_COUNT					2									//数目排序
#define ST_CUSTOM					3									//自定排序
//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码
//排序类型
#define ST_ORDER					1									//大小排序
#define ST_COUNT					2									//数目排序
#define ST_CUSTOM					3									//自定排序
 
//彩金动画
#define CAIJIN_ANIMATION            "caijin_animation"

//牛牛
enum emNiuNiuType
{
    OX_VALUE0           =       0,                              //无牛
    OX_NIU1             =		1,								//特殊类型牛1
    OX_NIU2             =		2,								//特殊类型牛2
    OX_NIU3             =		3,								//特殊类型牛3
    OX_NIU4             =		4,								//特殊类型牛4
    OX_NIU5             =		5,								//特殊类型牛5
    OX_NIU6             =		6,								//特殊类型牛6
    OX_NIU7             =		7,								//特殊类型牛7
    OX_NIU8             =		8,								//特殊类型牛8
    OX_NIU9             =	    9,								//特殊类型牛9
    OX_NIUNIU           =		10,								//特殊类型牛牛
    OX_THREE_SAME       =       102,                            //三个一样
    OX_FOUR_SAME        =       103,                            //四个一样
    OX_FOURKING         =       104,                            //四金花
    OX_FIVEKING         =       105,                            //五金花
};

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_NEW					    3									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//扑克数目
#define CARD_COUNT					54									//扑克数目

//扑克数据
static const BYTE g_cbCardData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方片 A - K

	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K

	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
};

//大小王
const BYTE LITTLE_JOKER = 0x4E;
const BYTE BIG_JOKER = 0x4F;
static const char* LITTLE_JOKER_FILE = "card_little_joker.png";
static const char* BIG_JOKER_FILE = "card_big_joker.png";

//////////////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//扑克数目
	BYTE							cbCardData[4][NN_MAXCOUNT];			//扑克数据
};

//出牌结果
struct tagOutCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbResultCard[NN_MAXCOUNT];			//结果扑克
};

//分布信息
struct tagDistributing
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbDistributing[15][6];				//分布信息
};

//搜索结果
struct tagSearchCardResult
{
	BYTE							cbSearchCount;						//结果数目（有多少组结果）
	BYTE							cbCardCount[NN_MAXCOUNT];				//扑克数目（每一组结果的牌数量）
	BYTE							cbResultCard[NN_MAXCOUNT][NN_MAXCOUNT];	//结果扑克（每一组牌的数据）
};

//牛牛筛选结果
struct tagNiuNiuFilter
{
    BYTE                            cbNiuNiuCardCount;                  //牛牛扑克数目
    BYTE                            cbNormalCount;                      //普通牌数目
    BYTE                            cbCardData[2][NN_MAXCOUNT];           //扑克牌(0下标:牛牛牌，1下标:非牛牛)
};
}

////////////////////////////////////////////////////////////////////////// 牛牛

#endif