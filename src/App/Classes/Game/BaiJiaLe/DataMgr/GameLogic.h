//
//  GameLogic.hpp
//  DouDiZhu
//
//  Created by zhong on 1/13/16.
//
//

#ifndef GameLogic_hpp
#define GameLogic_hpp

#include "CocosHeader.h"
#include "GameDefine.h"
#include "BaiJiaLeLogicDefine.h"
#include "Singleton_Z.h"
#include <mutex>
#include "define.h"
#include "CMD_Game.h"

BJL_NAMESPACE_BEGIN
#ifdef WIN32
#else
#ifndef CopyMemory
#define CopyMemory memcpy
#endif
#endif // WIN32
class CGameLogic
{
    CREATE_SINGLETON_MUTEX(CGameLogic,m_gameInstance,nullptr);
    void init();
public:
    //扑克数据
    static const BYTE				m_cbCardListData[52*8];
#pragma mark -
#pragma mark 类型函数
public:
    //获取类型
    BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
    //获取类型
    BYTE GetBackCardType(const BYTE cbCardData[]);
    //获取数值
    BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
    //获取花色
    BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }
    
#pragma mark -
#pragma mark 控制函数
public:
    //混乱扑克
    void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
    //排列扑克
    void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
#pragma mark -
#pragma mark 逻辑函数
public:
    //获取牌点
    BYTE GetCardPip(BYTE cbCardData);
    //获取牌点
    BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
    
    //自己是否参与游戏
    CC_SYNTHESIZE(bool, m_bJoinGame, JoinGame);
    //游戏结果
    CC_SYNTHESIZE(tagGameResult, m_tagResult, GameResult);
    //是否动态加入
    CC_SYNTHESIZE(bool, m_bDynamicJoin, DynamicJoin);
    
    void calcuteRate(const int &nWin, const int &nLost);
    SCORE m_llTotalScore;       //游戏成绩
    float m_fGameRate;          //命中率
    int   m_nTotalRound;
    int   m_nTotalWinRound;
private:
    static CGameLogic* m_gameInstance;
};
BJL_NAMESPACE_END
#endif /* GameLogic_hpp */
