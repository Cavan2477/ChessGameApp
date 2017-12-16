#ifndef __bjl_GameLogicDefine_H__
#define __bjl_GameLogicDefine_H__

#include "cocos2d.h"
#include "define.h"
#include "CMD_Game.h"
namespace bjl_game{
    
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

//倒计时控制
const int bjl_COUNTDOWN_READY = 30;				//准备倒计时
const int bjl_COUNTDOWN_CALLBANKER = 30;        //庄家
const int bjl_COUNTDOWN_CALLSCORE = 30;			//叫分倒计时
const int bjl_COUNTDOWN_ROD = 30;               //抢倒计时
const int bjl_COUNTDOWN_DOUBLE = 30;            //加倍倒计时
const int bjl_COUNTDOWN_OUTCARD = 30;			//出牌倒计时
    
//倒计时定时器tag
enum bjl_enCountDownTag
{
    bjl_kDefaultCountDown,
    //游戏准备倒计时
    bjl_kREADY_COUNTDOWN,
    //庄家倒计时
    bjl_kBANKER_COUNTDOWN,
    //叫分倒计时
    bjl_kSCORE_COUNTDOWN,
    //抢地主倒计时
    bjl_kROD_COUNTDOWN,
    //加倍倒计时
    bjl_kDOUBLE_COUNTDOWN,
    //出牌倒计时
    bjl_kOUTCARD_COUNTDOWN,
    //空闲倒计时
    bjl_kGAMEFREE_COUNTDOWN,
    //游戏开始(下注倒计时)
    bjl_kGAMEPLAY_COUNTDOWN,
    //结束倒计时
    bjl_kGAMEOVER_COUNTDOWN,
};
    
//游戏状态
enum emGameState
{
    kGAME_FREE          = 0,                            //等待开始
    kGAME_START         = 1,                            //游戏开始
    kGAME_PLAY          = 100,                          //游戏进行
    kGAME_JETTON        = 100,                          //下注状态
    kGAME_END           = 101,                          //游戏结束
};
    
struct tagGameResult
{
    //分数
    LONGLONG m_pAreaScore[AREA_MAX];
    //总分
    LONGLONG m_llTotal;
    
    tagGameResult()
    {
        for (int i = 0; i < AREA_MAX; ++i)
        {
            m_pAreaScore[i] = 0;
        }
        m_llTotal = 0;
    }
};
    
enum enUserSelect
{
    kSelectDefault = -1,
    //庄赢
    kSelectMasterWin,
    //闲赢
    kSelectIdleWin,
    //平局
    kSelectDraw,
};
    
//用户参与记录
struct tagUserJoinRecord
{
    //是否参与
    bool m_bJoin;
    //用户选择
    enUserSelect m_enSelect;
    //输赢
    bool m_bWin;
    
    tagUserJoinRecord()
    {
        m_bJoin = false;
        m_enSelect = kSelectDefault;
        m_bWin = false;
    }
};
   
//游戏记录
struct tagGameRecord
{
    //服务器记录信息
    tagServerGameRecord *m_pServerRecord;
    //用户记录
    tagUserJoinRecord m_tagUserRecord;
    //游戏结果
    BYTE m_cbGameResult;
    
    tagGameRecord()
    {
        m_pServerRecord = nullptr;
        m_tagUserRecord = tagUserJoinRecord();
        m_cbGameResult = AREA_MAX;
    }
};

//路单
struct tagGameWallBill
{
    //路单一列数据
    BYTE m_pRecords[6];
    //路单列数据索引
    BYTE m_cbIndex;
    //路单除去平局索引
    BYTE m_cbIndexWithoutPing;
    //是否连胜
    bool m_bWinList;
    //是否跳过
    bool m_bJumpIdx;
    
    tagGameWallBill()
    {
        memset(m_pRecords, AREA_MAX, 6);
        m_cbIndex = -1;
        m_cbIndexWithoutPing = 0;
        m_bWinList = false;
        m_bJumpIdx = false;
    }
};
}
////////////////////////////////////////////////////////////////////////// 牛牛

#endif