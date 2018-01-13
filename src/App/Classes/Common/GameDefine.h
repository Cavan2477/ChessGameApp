/************************************************************************************
 * file: 		GameDefine.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:45:58
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __GAME_DEFINE_H__
#define __GAME_DEFINE_H__

#include "PublicDefine.h"

//排序类型
#define ST_ORDER					1									//大小排序
#define ST_COUNT					2									//数目排序
#define ST_CUSTOM					3									//自定排序

//倒计时控制
const int COUNTDOWN_READY = 30;				//准备倒计时
const int COUNTDOWN_CALLBANKER = 30;        //庄家
const int COUNTDOWN_CALLSCORE = 30;			//叫分倒计时
const int COUNTDOWN_OUTCARD = 30;			//出牌倒计时

//倒计时定时器tag
enum enCountDownTag
{
    kDefaultCountDown,
    kREADY_COUNTDOWN,			//游戏准备倒计时
    kBANKER_COUNTDOWN,			//庄家倒计时
    kSCORE_COUNTDOWN,			//叫分倒计时
    kOUTCARD_COUNTDOWN,			//出牌倒计时
};

//游戏胜利方
enum enGameResult
{
    kDefault	= -1,
    kLanderWin	= 0,
    kLanderLose,
    kFarmerWin,
    kFarmerLose,
};

//游戏方
enum enGameRoleDir
{
    kDEFAULT_ROLE = -1,
    kMY_ROLE,
    kRIGHT_ROLE,
    kLEFT_ROLE,
    kTOP_ROLE,
    kBANKER_ROLE,
};

//叫分动画(基本)
const std::string CALLSCORE_ANIMATION_KEY = "callscore_key";

//叫分1
const std::string CALLONE_ANIMATION_KEY = "1_score_key";

//叫分2
const std::string CALLTWO_ANIMATION_KEY = "2_score_key";

//叫分3
const std::string CALLTHREE_ANIMATION_KEY = "3_score_key";

//飞机动画
const std::string AIRSHIP_ANIMATION_KEY = "airship_key";

//火箭动画
const std::string ROCKET_ANIMATION_KEY = "rocket_key";

//报警动画
const std::string ALARM_ANIMATION_KEY = "alarm_key";

//炸弹动画
const std::string BOMB_ANIMATION_KEY = "bomb_key";

struct tagBankerInfo
{
    enGameRoleDir m_enBankerDir;
    BYTE m_cbBankerChair;
    DWORD m_dwBankerID;
};

//扑克卡牌
struct tagCards
{
    //卡牌数据
    BYTE	m_cardData[20];
    //卡牌数量
    BYTE    m_cardCount;
    tagCards()
    {
        memset(m_cardData,0,20);
        m_cardCount = 0;
    }
    
    tagCards& operator=(const tagCards &t)
    {
        if (this == &t)
        {
            return *this;
        }
        
        memcpy(&this->m_cardData, t.m_cardData, sizeof(BYTE) * t.m_cardCount);
        this->m_cardCount = t.m_cardCount;
        
        return *this;
    }
    
    tagCards(const tagCards& t)
    {
        *this = t;
    }
};
#endif