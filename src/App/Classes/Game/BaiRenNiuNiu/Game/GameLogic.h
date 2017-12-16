#ifndef brnn_GAME_LOGIC_HEAD_FILE
#define brnn_GAME_LOGIC_HEAD_FILE

#pragma once

#include "define.h"

//////////////////////////////////////////////////////////////////////////
//牌型
namespace brnn_game {
    
enum emCardType
{
	BRNN_CT_ERROR               =		0,								//错误类型
	BRNN_CT_POINT               =		1,								//点数类型
	BRNN_CT_SPECIAL_NIU1		=		2,								//特殊类型
	BRNN_CT_SPECIAL_NIU2		=		3,								//特殊类型
	BRNN_CT_SPECIAL_NIU3		=		4,								//特殊类型
	BRNN_CT_SPECIAL_NIU4		=		5,								//特殊类型
	BRNN_CT_SPECIAL_NIU5		=		6,								//特殊类型
	BRNN_CT_SPECIAL_NIU6		=		7,								//特殊类型
	BRNN_CT_SPECIAL_NIU7		=		8,								//特殊类型
	BRNN_CT_SPECIAL_NIU8		=		9,								//特殊类型
	BRNN_CT_SPECIAL_NIU9		=	    10,								//特殊类型
	BRNN_CT_SPECIAL_NIUNIU      =		11,								//特殊类型
	BRNN_CT_SPECIAL_NIUNIUXW	=		12,								//特殊类型
	BRNN_CT_SPECIAL_NIUNIUDW	=		13,								//特殊类型
	BRNN_CT_SPECIAL_NIUYING     =		14,								//特殊类型
	BRNN_CT_SPECIAL_NIUKING     =	    15,								//特殊类型
	BRNN_CT_SPECIAL_BOMEBOME	=		16								//特殊类型
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
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////	

//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//变量定义
public:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//获取花色
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//混乱扑克
	void RandCardListEx(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	int RetType(int itype);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE *bcOutCadData = NULL);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple);
	//逻辑大小
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////
}
#endif
