/************************************************************************************
 * file: 		AudioMgr.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:37:06
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __AUDIO_MGR_H__
#define __AUDIO_MGR_H__

#include <stdio.h>
#include "CocosHeader.h"
#include "../Pattern/Singleton.h"
#include "../Common/MacroDefine.h"

class AudioMgr
{
    CREATE_SINGLETON_MUTEX_CLEAR(AudioMgr,s_audioInstance,nullptr);

    void init();
    void clear();

public:
    void preloadGameAudio();
    
    void preloadBackgroundAudio(const std::string &path);
    
    void preloadEffectAudio(const std::string &path);
    
    void unloadGameAudio();
    
    void unloadEffectAudio(const std::string &path);

public:
    //播放背景音乐
    void playBackgroudAudio(bool bPlay, const std::string &bgfile = "");
    
    //播放通用背景音乐
    void playGeneralBackgroudAudio(bool bPlay);
    
    //播放点击音效
    void playClickAudio();
    
    //播放警告
    void playAlertAudio();
    
    /*
     * @brief 播放文本音
     * @param[dwUser] 玩家id
     * @param[idx] 文本索引
     */
    void playTextAudio(DWORD dwUser,const int &idx);
    
    //播放发牌音效
    void playSendCardAudio();
    
    //播放开牌音效
    void playOpenCardAudio();
    
    //播放选牌音效
    void playSelectCardAudio();
    
    /*
     * @brief 播放下注、加分音效
     * @param[dwUser] 用户
     * @param[llScore] 下注、加分数目
     */
    void playAddScoreAudio(DWORD dwUser,LONGLONG llScore);
    
    //播放游戏开始
    void playGameStartAudio();
    
    //播放游戏结束
    void playGameEndAudio(bool bWin);
    
    //游戏结束
    void playGameConcludeAudio();

#pragma mark 斗地主音效
    /*
     * @brief 播放对比音效
     * @param[dwUser] 用户id
     * @param[bLarge] 是否更大
     */
    void playCompaireAudio(const DWORD &dwUser, bool bLarge);
    
    /*
     * @brief 播放出单张牌音效
     * @param[dwUser] 用户id
     * @param[carddata] 扑克逻辑数值
     */
    void playOutSingleCardAudio(const DWORD &dwUser,const BYTE &carddata);
    
    /*
     * @brief 根据出牌类型播放音效
     * @param[dwUser] 用户id
     * @param[cardType] 出牌类型
     */
    void playOutCardWithTypeAudio(const DWORD &dwUser,const BYTE &cardType);
    
    /*
     * @brief 飞机动画音效
     */
    void playAirplaneEffectAudio();
    
    /*
     * @brief 火箭动画音效
     */
    void playRocketEffectAudio();
    
    /*
     * @brief 炸弹动画音效
     */
    void playBombEffectAudio();
    
    /*
     * @brief 警告动画音效
     */
    void playAlarmEffectAudio();
    
    /*
     * @brief 洗牌音效
     */
    void playDispatchEffectAudio();
#pragma mark 牛牛音效
    /*
     * @brief 播放下注、加分音效
     * @param[llScore] 下注、加分数目
     */
    void playNiuNiuAddScoreAudio(LONGLONG llScore);
    
    /*
     * @brief 播放牛牛音效
     * @param[dwUser] 玩家id
     * @param[cbType] 牛牛标志
     */
    void playNiuNiuAudio(DWORD dwUser, BYTE cbType);
    
    /*
     * @brief 播放游戏抢庄音效
     * @param[dwUser] 玩家id
     * @param[bCall] 是否抢庄
     */
    void playCallBankerAudio(DWORD dwUser, bool bCall);
    
    /*
     * @brief 播放谁是庄家
     * @param[dwUser] 玩家id
     */
    void playGetBankerAudio(DWORD dwUser);
#pragma mark 百家乐音效
    void playBJLAddScoreAudio();
    /*
     * @brief 播放游戏结果
     * @param[cbRes] 0:平;1:输;2:赢
     */
    void playBJLGameResAudio(const BYTE &cbRes);
    
#pragma mark 欢乐斗地主音效
    //播放游戏开始
    void playHL_GameStartAudio();
    
    //播放游戏结束
    void playHL_GameEndAudio(bool bWin);
    
    /*
     *  @brief 超时警告
     */
    void playHL_ClockWarnAudio();
    
    /*
     * @brief 播放获取庄家音效
     */
    void playHL_BankerInfoAudio();
    
    /*
     * @brief 播放明牌音效
     * @param[dwUser] 用户id
     */
    void playHL_ValidCard(const DWORD &dwUser);
    
    /*
     * @brief 播放对比音效
     * @param[dwUser] 用户id
     * @param[bLarge] 是否更大
     */
    void playHL_CompaireAudio(const DWORD &dwUser, bool bLarge);
    
    /*
     * @brief 播放出单张牌音效
     * @param[dwUser] 用户id
     * @param[carddata] 扑克逻辑数值
     */
    void playHL_OutSingleCardAudio(const DWORD &dwUser,const BYTE &carddata);
    
    /*
     * @brief 根据出牌类型播放音效
     * @param[dwUser] 用户id
     * @param[cardType] 出牌类型
     */
    void playHL_OutCardWithTypeAudio(const DWORD &dwUser,const BYTE &cardType);
    
    /*
     * @brief 播放飞机音效
     * @param[dwUser] 用户id
     */
    void playHL_PlaneAudio(const DWORD &dwUser);
    
    /*
     * @brief 播放炸弹音效
     * @param[dwUser] 用户id
     */
    void playHL_BombAudio(const DWORD &dwUser);
    
    /*
     * @brief 播放叫地主音效
     * @param[dwUser] 用户id
     * @param[cbCall] 是否叫地主(1:叫，2:不叫)
     */
    void playHL_CallBankerAudio(const DWORD &dwUser, const BYTE &cbCall);
    
    /*
     * @brief 播放抢地主音效
     * @param[dwUser] 用户id
     * @param[cbRod] 是否抢地主(1:抢，2:不抢)
     */
    void playHL_RodBankerAudio(const DWORD &dwUser, const BYTE &cbRod);
    
    /*
     * @brief 播放加倍音效
     * @param[dwUser] 用户id
     * @param[cbRod] 是否加倍(1:加，2:不加)
     */
    void playHL_DoubleAudio(const DWORD &dwUser, const BYTE &cbDouble);
    
    /*
     * @brief 飞机动画音效
     */
    void playHL_AirplaneEffectAudio();
    
    /*
     * @brief 火箭动画音效
     */
    void playHL_RocketEffectAudio();
    
    /*
     * @brief 炸弹动画音效
     */
    void playHL_BombEffectAudio();
    
    /*
     * @brief 警告动画音效
     */
    void playHL_AlarmEffectAudio();
    
    /*
     * @brief 洗牌音效
     */
    void playHL_DispatchEffectAudio();
    
#pragma mark 通比牛牛音效
    /*
     * @brief 开牌
     */
    void playTBNN_OpenCardAudio();
    
    /*
     * @brief 获取到牛牛
     */
    void playTBNN_NiuNiuAudio();
    
    /*
     * @brief 通比牛牛游戏结果音效
     * @param[cbRes] 0:未参与；1:赢；2:输
     */
    void playTBNN_GameEndAudio(const BYTE &cbRes);

private:
    //是否过滤音效
    bool filterAudio();

private:
    static AudioMgr* s_audioInstance;
};

#endif /* AudioMgr_hpp */
