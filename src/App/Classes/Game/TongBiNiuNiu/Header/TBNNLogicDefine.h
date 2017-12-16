//
//  TBNNLogicDefine.h
//  GameProject
//
//  Created by zhong on 4/19/16.
//
//

#ifndef TBNNLogicDefine_h
#define TBNNLogicDefine_h

#include "define.h"
#include "cmd_game.h"

namespace tbnn_game {
    //彩金动画
    #define tbnn_CAIJIN_ANIMATION            "caijin_animation"
    
    enum en_TBNN_GameState
    {
        TBNN_kGameFree,
        TBNN_kGameStart,
        TBNN_kGamePlay,
        TBNN_kGameEnd,
    };
    
    //倒计时控制
    const int tbnn_COUNTDOWN_READY = 30;				//准备倒计时
    const int tbnn_COUNTDOWN_OPENCARD = 30;             //开牌
    
    //倒计时定时器tag
    enum en_tbnn_CountDownTag
    {
        tbnn_kDefaultCountDown,
        //游戏准备倒计时
        tbnn_kREADY_COUNTDOWN,
        //开牌倒计时
        tbnn_kOPENCARD_COUNTDOWN,
    };
    
    //游戏内座椅编号
    enum enGameChairIndex
    {
        tbnn_kDefaultChair      = -1,
        tbnn_kGameChair0,
        tbnn_kGameChair1,
        tbnn_kGameChair2,
        tbnn_kGameChair3,
        tbnn_kGameChair4,
        tbnn_kGameChair5,
    };
    
    //牛牛筛选结果
    struct tagNiuNiuFilter
    {
        BYTE                            cbNiuNiuCardCount;                  //牛牛扑克数目
        BYTE                            cbNormalCount;                      //普通牌数目
        BYTE                            cbCardData[2][tbnn_MAX_COUNT];           //扑克牌(0下标:牛牛牌，1下标:非牛牛)
    };

}
#endif /* TBNNLogicDefine_h */
