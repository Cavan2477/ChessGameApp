

#pragma once

#include "define.h"
#include "cocos2d.h"
#include "define.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "CocosHeader.h"
USING_NS_CC;
ZJH_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
//∫Í∂®“Â

#define MAX_COUNT					3
#define	DRAW						2

// ˝÷µ—⁄¬Î
#define	LOGIC_MASK_COLOR			0xF0
#define	LOGIC_MASK_VALUE			0x0F

//∆ÀøÀ¿‡–Õ
#define CT_SINGLE					1
#define CT_DOUBLE					2
#define	CT_SHUN_ZI					3
#define CT_JIN_HUA					4
#define	CT_SHUN_JIN					5
#define	CT_BAO_ZI					6
#define CT_SPECIAL					7

//////////////////////////////////////////////////////////////////////////


class CGameLogic : public Node
{
	
private:
	static BYTE						m_cbCardListData[52];

	
public:
	
	CGameLogic();
	
	virtual ~CGameLogic();

	CREATE_FUNC(CGameLogic);
public:
	
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//øÿ÷∆∫Ø ˝
public:
	
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	
public:
	
	BYTE GetCardLogicValue(BYTE cbCardData);
	
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

const BYTE m_cbCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D
    
	//0x05,0x1c,0x34,0x0A,0x0B,0x0C,0x19,0x09,0x2b,0x15,0x16,0x03,0x0D,	//∑ΩøÈ A - K
	//0x11,0x12,0x13,0x14,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x01,0x11,	//√∑ª® A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//∫ÏÃ“ A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//∫⁄Ã“ A - K
};
ZJH_NAMESPACE_END