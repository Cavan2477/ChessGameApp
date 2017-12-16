#ifndef tbnn_GAME_LOGIC_HEAD_FILE
#define tbnn_GAME_LOGIC_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#include "../Header/cmd_game.h"
#include "Singleton_Z.h"
#include "CocosHeader.h"
#include "../Header/TBNNLogicDefine.h"

TBNN_NAMESPACE_BEGIN
//宏定义
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define tbnn_OX_VALUE_0					0									//无牛牌型
#define tbnn_OX_VALUE_BIG_0				1									//无牛牌型
#define tbnn_OX_VALUE_1					2									//牛一牌型
#define tbnn_OX_VALUE_BIG_1				3									//牛一牌型
#define tbnn_OX_VALUE_2					4									//牛二牌型
#define tbnn_OX_VALUE_BIG_2				5									//牛二牌型
#define tbnn_OX_VALUE_3					6									//牛三牌型
#define tbnn_OX_VALUE_BIG_3				7									//牛三牌型
#define tbnn_OX_VALUE_4					8									//牛四牌型
#define tbnn_OX_VALUE_BIG_4				9									//牛四牌型
#define tbnn_OX_VALUE_5					10									//牛五牌型
#define tbnn_OX_VALUE_BIG_5				11									//牛五牌型
#define tbnn_OX_VALUE_6					12									//牛六牌型
#define tbnn_OX_VALUE_BIG_6				13									//牛六牌型
#define tbnn_OX_VALUE_7					14									//牛七牌型
#define tbnn_OX_VALUE_BIG_7				15									//牛七牌型
#define tbnn_OX_VALUE_8					16									//牛八牌型
#define tbnn_OX_VALUE_BIG_8				17									//牛八牌型
#define tbnn_OX_VALUE_9					18									//牛九牌型
#define tbnn_OX_VALUE_BIG_9				19									//牛九牌型
#define tbnn_OX_VALUE_10				20									//牛牛牌型
#define tbnn_OX_VALUE_BIG_10			21									//牛牛牌型
#define tbnn_OX_FOUR_KING				22									//四花牛牌
#define tbnn_OX_FIVE_KING				23									//五花牛牌

//////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE 							cbFourLogicVolue[1];				//四张列表
	BYTE 							cbThreeLogicVolue[1];				//三张列表
	BYTE 							cbDoubleLogicVolue[2];				//两张列表
	BYTE 							cbSignedLogicVolue[5];				//单张列表
	BYTE							cbFourCardData[tbnn_MAX_COUNT];			//四张列表
	BYTE							cbThreeCardData[tbnn_MAX_COUNT];			//三张列表
	BYTE							cbDoubleCardData[tbnn_MAX_COUNT];		//两张列表
	BYTE							cbSignedCardData[tbnn_MAX_COUNT];		//单张数目
};

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
private:
	static BYTE						m_cbCardListData[54];				//扑克定义

	//函数定义
public:
    CREATE_SINGLETON_MUTEX(CGameLogic,m_gameInstance,nullptr);
    void init();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//获取倍数
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//获取牛牛
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//获取整数
	bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

    //牛牛牌过滤
    void FilterNiuNiu(BYTE cbCardData[], BYTE cbCardCount, tagNiuNiuFilter &tagResult);
	//功能函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,bool FirstOX,bool NextOX);
    
public:
    enGameChairIndex switchToGameChair(const WORD &wChair);
    
    bool isValidChair(const enGameChairIndex &chair);
private:
    static CGameLogic* m_gameInstance;
};

//////////////////////////////////////////////////////////////////////////
TBNN_NAMESPACE_END
#endif
