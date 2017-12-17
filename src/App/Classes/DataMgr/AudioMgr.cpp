/************************************************************************************
 * file: 		AudioMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:21:08
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "../DataMgr/AudioMgr.h"
#include "../DataMgr/HallDataMgr.h"
#include "../Common/Packet.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

//背景音乐
const std::string BACKGROUND_AUDIO = "sound_res/GAME_BLACKGROUND.mp3";

//点击音效
const std::string CLICK_AUDIO = "sound_res/Click.wav";

//警告音效
const std::string ALERT_AUDIO = "sound_res/open_alert.wav";

//发牌音效
const std::string SENDCARD_AUDIO = "sound_res/SEND_CARD.wav";

//开牌音效
const std::string OPENCARD_AUDIO = "sound_res/OPEN_CARD.wav";

//选牌音效
const std::string SELECTCARD_AUDIO = "sound_res/xuanpai.wav";

//下注音效
const std::string ADDSCORE_AUDIO = "sound_res/ADD_SCORE.wav";

//游戏开始
const std::string GAMESTART_AUDIO = "sound_res/GAME_START.wav";

//游戏结束-胜利
const std::string GAMEWIN_AUDIO = "sound_res/putong/win_0.wav";

//游戏结束-失败
const std::string GAMELOST_AUDIO = "sound_res/putong/lose_1.wav";

//游戏结束
const std::string GAME_CONCLUDE_AUDIO = "sound_res/GAME_CONCLUDE.wav";

AudioMgr* AudioMgr::s_audioInstance = nullptr;

void AudioMgr::init()
{
    //preloadBackgroundAudio(BACKGROUND_AUDIO);
    preloadEffectAudio(CLICK_AUDIO);
    preloadEffectAudio(ALERT_AUDIO);
    preloadGameAudio();
}

void AudioMgr::clear()
{
    unloadEffectAudio(CLICK_AUDIO);
    unloadEffectAudio(ALERT_AUDIO);
    unloadGameAudio();
}

void AudioMgr::preloadGameAudio()
{
    preloadEffectAudio(SENDCARD_AUDIO);
    preloadEffectAudio(OPENCARD_AUDIO);
    preloadEffectAudio(ADDSCORE_AUDIO);
    preloadEffectAudio(GAMESTART_AUDIO);
    //preloadEffectAudio(GAMEWIN_AUDIO);
    //preloadEffectAudio(GAMELOST_AUDIO);
    preloadEffectAudio(SELECTCARD_AUDIO);
}

void AudioMgr::preloadBackgroundAudio(const std::string &path)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(path.c_str());
}

void AudioMgr::preloadEffectAudio(const std::string &path)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(path.c_str());
}

void AudioMgr::unloadGameAudio()
{
    unloadEffectAudio(SENDCARD_AUDIO);
    unloadEffectAudio(OPENCARD_AUDIO);
    unloadEffectAudio(ADDSCORE_AUDIO);
    unloadEffectAudio(GAMESTART_AUDIO);
    unloadEffectAudio(GAMEWIN_AUDIO);
    unloadEffectAudio(GAMELOST_AUDIO);
    unloadEffectAudio(SELECTCARD_AUDIO);
}

void AudioMgr::unloadEffectAudio(const std::string &path)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(path.c_str());
}

void AudioMgr::playBackgroudAudio(bool bPlay, const std::string &bgfile/* = ""*/)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    if (bPlay)
    {
        if (HallDataMgr::getInstance()->m_bOptionMute)
        {
            return;
        }
        if ("" != bgfile)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(bgfile.c_str(),true);
        }
        else
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_AUDIO.c_str(),true);
        }
    }
}

void AudioMgr::playGeneralBackgroudAudio(bool bPlay)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    if (bPlay)
    {
        if (HallDataMgr::getInstance()->m_bOptionMute)
        {
            return;
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/menuBGMMono.mp3",true);
    }
}

void AudioMgr::playClickAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_AUDIO.c_str());
}

void AudioMgr::playAlertAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(ALERT_AUDIO.c_str());
}

void AudioMgr::playTextAudio(DWORD dwUser,const int &idx)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/1000_%d.wav",idx);
        }
        else
        {
            sprintf(buf, "sound_res/2000_%d.wav",idx);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playSendCardAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SENDCARD_AUDIO.c_str());
}

void AudioMgr::playOpenCardAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(OPENCARD_AUDIO.c_str());
}

void AudioMgr::playSelectCardAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SELECTCARD_AUDIO.c_str());
}

void AudioMgr::playAddScoreAudio(DWORD dwUser,SCORE llScore)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/0_cs%lld.wav",llScore);
        }
        else
        {
            sprintf(buf, "sound_res/1_cs%lld.wav",llScore);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

#pragma mark 斗地主音效
void AudioMgr::playCompaireAudio(const DWORD &dwUser, bool bLarge)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        int randidx = rand()%(2);
        if (1 == pUser->m_date.cbGender) //男
        {
            if (bLarge)
            {
                sprintf(buf, "sound_res/0_ya%d.wav",randidx);
            }
            else
            {
                sprintf(buf, "sound_res/0_pass%d.wav",randidx);
            }
        }
        else
        {
            if (bLarge)
            {
                sprintf(buf, "sound_res/1_ya%d.wav",randidx);
            }
            else
            {
                sprintf(buf, "sound_res/1_pass%d.wav",randidx);
            }
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playOutSingleCardAudio(const DWORD &dwUser, const BYTE &carddata)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/0_%d.wav",carddata);
        }
        else
        {
            sprintf(buf, "sound_res/1_%d.wav",carddata);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playOutCardWithTypeAudio(const DWORD &dwUser, const BYTE &cardType)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/0_type%d.wav",cardType);
        }
        else
        {
            sprintf(buf, "sound_res/1_type%d.wav",cardType);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playAirplaneEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_plane.wav");
}

void AudioMgr::playRocketEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
}

void AudioMgr::playBombEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_bomb.wav");
}

void AudioMgr::playAlarmEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_alert.wav");
}

void AudioMgr::playDispatchEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/DISPATCH_CARD.wav");
}

void AudioMgr::playGameStartAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAMESTART_AUDIO.c_str());
}

void AudioMgr::playGameEndAudio(bool bWin)
{
    if (filterAudio())
    {
        return;
    }
    if (bWin)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAMEWIN_AUDIO.c_str());
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAMELOST_AUDIO.c_str());
    }
}

void AudioMgr::playGameConcludeAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAME_CONCLUDE_AUDIO.c_str());
}

#pragma mark 牛牛音效
void AudioMgr::playNiuNiuAddScoreAudio(SCORE llScore)
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(ADDSCORE_AUDIO.c_str());
}

void AudioMgr::playNiuNiuAudio(DWORD dwUser, BYTE cbType)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/putong/man/nn%d_1.wav",cbType);
        }
        else
        {
            sprintf(buf, "sound_res/putong/wom/nn%d_0.wav",cbType);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playCallBankerAudio(DWORD dwUser, bool bCall)
{
    if (filterAudio())
    {
        return;
    }
    std::string buf = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            if (bCall)
            {
                buf = "sound_res/putong/man/qiang_1.wav";
            }
            else
            {
                buf = "sound_res/putong/man/buqiang_1.wav";
            }
        }
        else
        {
            if (bCall)
            {
                buf = "sound_res/putong/wom/qiang_0.wav";
            }
            else
            {
                buf = "sound_res/putong/wom/buqiang_0.wav";
            }
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf.c_str());
    }
}

void AudioMgr::playGetBankerAudio(DWORD dwUser)
{
    if (filterAudio())
    {
        return;
    }
    std::string buf = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            buf = "sound_res/putong/man/nt_1.wav";
        }
        else
        {
            buf = "sound_res/putong/wom/nt_0.wav";
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf.c_str());
    }
}

#pragma mark 百家乐音效
void AudioMgr::playBJLAddScoreAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/ADD_SCORE.wav");
}

void AudioMgr::playBJLGameResAudio(const BYTE &cbRes)
{
    if (filterAudio())
    {
        return;
    }
    std::string str = "";
    if (0 == cbRes) //平
    {
        str = "sound_res/END_DRAW.wav";
    }
    else if (1 == cbRes) //输
    {
        str = "sound_res/END_LOST.wav";
    }
    else if (2 == cbRes) //赢
    {
        str = "sound_res/END_WIN.wav";
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(str.c_str());
}

#pragma mark 欢乐斗地主音效
void AudioMgr::playHL_ValidCard(const DWORD &dwUser)
{
    if (filterAudio())
    {
        return;
    }
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/m/VALID_CARD.wav");
        }
        else
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/w/VALID_CARD.wav");
        }
    }
}

void AudioMgr::playHL_CompaireAudio(const DWORD &dwUser, bool bLarge)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            if (bLarge)
            {
                sprintf(buf, "sound_res/%s/OUT_CARD.wav","m");
            }
            else
            {
                sprintf(buf, "sound_res/%s/PASS_CARD.wav","m");
            }
        }
        else
        {
            if (bLarge)
            {
                sprintf(buf, "sound_res/%s/OUT_CARD.wav","w");
            }
            else
            {
                sprintf(buf, "sound_res/%s/PASS_CARD.wav","w");
            }
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playHL_OutSingleCardAudio(const DWORD &dwUser, const BYTE &carddata)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/m/SINGLE_%d.wav",carddata);
        }
        else
        {
            sprintf(buf, "sound_res/w/SINGLE_%d.wav",carddata);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playHL_OutCardWithTypeAudio(const DWORD &dwUser, const BYTE &cardType)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/m/0_type%d.wav",cardType);
        }
        else
        {
            sprintf(buf, "sound_res/w/1_type%d.wav",cardType);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playHL_PlaneAudio(const DWORD &dwUser)
{
    if (filterAudio())
    {
        return;
    }
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/m/THREE_ONE_LINE.wav");
        }
        else
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/w/THREE_ONE_LINE.wav");
        }
    }
}

void AudioMgr::playHL_BombAudio(const DWORD &dwUser)
{
    if (filterAudio())
    {
        return;
    }
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/m/BOMB_CARD.wav");
        }
        else
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/w/BOMB_CARD.wav");
        }
    }
}

void AudioMgr::playHL_CallBankerAudio(const DWORD &dwUser, const BYTE &cbCall)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/m/call_banker_%d.wav",cbCall);
        }
        else
        {
            sprintf(buf, "sound_res/w/call_banker_%d.wav",2);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playHL_RodBankerAudio(const DWORD &dwUser, const BYTE &cbRod)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/m/rod_banker_%d.wav",cbRod);
        }
        else
        {
            sprintf(buf, "sound_res/w/rod_banker_%d.wav",cbRod);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playHL_DoubleAudio(const DWORD &dwUser, const BYTE &cbDouble)
{
    if (filterAudio())
    {
        return;
    }
    char buf[64] = "";
    //根据性别播放不同音效
    UserData *pUser = HallDataMgr::getInstance()->getUserData(dwUser);
    if (nullptr != pUser)
    {
        if (1 == pUser->m_date.cbGender) //男
        {
            sprintf(buf, "sound_res/m/double_%d.wav",cbDouble);
        }
        else
        {
            sprintf(buf, "sound_res/w/double_%d.wav",cbDouble);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
    }
}

void AudioMgr::playHL_AirplaneEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_plane.wav");
}

void AudioMgr::playHL_RocketEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_rocket.wav");
}

void AudioMgr::playHL_BombEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_bomb.wav");
}

void AudioMgr::playHL_AlarmEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/common_alert.wav");
}

void AudioMgr::playHL_DispatchEffectAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/DISPATCH_CARD.wav");
}

void AudioMgr::playHL_GameStartAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAMESTART_AUDIO.c_str());
}

void AudioMgr::playHL_GameEndAudio(bool bWin)
{
    if (filterAudio())
    {
        return;
    }
    if (bWin)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAMEWIN_AUDIO.c_str());
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAMELOST_AUDIO.c_str());
    }
}

void AudioMgr::playHL_ClockWarnAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_WARN.wav");
}

void AudioMgr::playHL_BankerInfoAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/BANKER_INFO.wav");
}

#pragma mark 通比牛牛音效
void AudioMgr::playTBNN_OpenCardAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/OPEN_CARD.wav");
}

void AudioMgr::playTBNN_NiuNiuAudio()
{
    if (filterAudio())
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_OXOX.wav");
}

void AudioMgr::playTBNN_GameEndAudio(const BYTE &cbRes)
{
    if (filterAudio())
    {
        return;
    }
    
    switch (cbRes)
    {
        case 0:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_END.wav");
            break;
        case 1:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_WIN.wav");
            break;
        case 2:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_LOSE.wav");
            break;
        default:
            break;
    }
}

bool AudioMgr::filterAudio()
{
    return HallDataMgr::getInstance()->m_bOptionMute;
}