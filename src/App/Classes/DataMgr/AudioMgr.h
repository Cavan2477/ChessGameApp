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
    //���ű�������
    void playBackgroudAudio(bool bPlay, const std::string &bgfile = "");
    
    //����ͨ�ñ�������
    void playGeneralBackgroudAudio(bool bPlay);
    
    //���ŵ����Ч
    void playClickAudio();
    
    //���ž���
    void playAlertAudio();
    
    /*
     * @brief �����ı���
     * @param[dwUser] ���id
     * @param[idx] �ı�����
     */
    void playTextAudio(DWORD dwUser,const int &idx);
    
    //���ŷ�����Ч
    void playSendCardAudio();
    
    //���ſ�����Ч
    void playOpenCardAudio();
    
    //����ѡ����Ч
    void playSelectCardAudio();
    
    /*
     * @brief ������ע���ӷ���Ч
     * @param[dwUser] �û�
     * @param[llScore] ��ע���ӷ���Ŀ
     */
    void playAddScoreAudio(DWORD dwUser,LONGLONG llScore);
    
    //������Ϸ��ʼ
    void playGameStartAudio();
    
    //������Ϸ����
    void playGameEndAudio(bool bWin);
    
    //��Ϸ����
    void playGameConcludeAudio();

#pragma mark ��������Ч
    /*
     * @brief ���ŶԱ���Ч
     * @param[dwUser] �û�id
     * @param[bLarge] �Ƿ����
     */
    void playCompaireAudio(const DWORD &dwUser, bool bLarge);
    
    /*
     * @brief ���ų���������Ч
     * @param[dwUser] �û�id
     * @param[carddata] �˿��߼���ֵ
     */
    void playOutSingleCardAudio(const DWORD &dwUser,const BYTE &carddata);
    
    /*
     * @brief ���ݳ������Ͳ�����Ч
     * @param[dwUser] �û�id
     * @param[cardType] ��������
     */
    void playOutCardWithTypeAudio(const DWORD &dwUser,const BYTE &cardType);
    
    /*
     * @brief �ɻ�������Ч
     */
    void playAirplaneEffectAudio();
    
    /*
     * @brief ���������Ч
     */
    void playRocketEffectAudio();
    
    /*
     * @brief ը��������Ч
     */
    void playBombEffectAudio();
    
    /*
     * @brief ���涯����Ч
     */
    void playAlarmEffectAudio();
    
    /*
     * @brief ϴ����Ч
     */
    void playDispatchEffectAudio();
#pragma mark ţţ��Ч
    /*
     * @brief ������ע���ӷ���Ч
     * @param[llScore] ��ע���ӷ���Ŀ
     */
    void playNiuNiuAddScoreAudio(LONGLONG llScore);
    
    /*
     * @brief ����ţţ��Ч
     * @param[dwUser] ���id
     * @param[cbType] ţţ��־
     */
    void playNiuNiuAudio(DWORD dwUser, BYTE cbType);
    
    /*
     * @brief ������Ϸ��ׯ��Ч
     * @param[dwUser] ���id
     * @param[bCall] �Ƿ���ׯ
     */
    void playCallBankerAudio(DWORD dwUser, bool bCall);
    
    /*
     * @brief ����˭��ׯ��
     * @param[dwUser] ���id
     */
    void playGetBankerAudio(DWORD dwUser);
#pragma mark �ټ�����Ч
    void playBJLAddScoreAudio();
    /*
     * @brief ������Ϸ���
     * @param[cbRes] 0:ƽ;1:��;2:Ӯ
     */
    void playBJLGameResAudio(const BYTE &cbRes);
    
#pragma mark ���ֶ�������Ч
    //������Ϸ��ʼ
    void playHL_GameStartAudio();
    
    //������Ϸ����
    void playHL_GameEndAudio(bool bWin);
    
    /*
     *  @brief ��ʱ����
     */
    void playHL_ClockWarnAudio();
    
    /*
     * @brief ���Ż�ȡׯ����Ч
     */
    void playHL_BankerInfoAudio();
    
    /*
     * @brief ����������Ч
     * @param[dwUser] �û�id
     */
    void playHL_ValidCard(const DWORD &dwUser);
    
    /*
     * @brief ���ŶԱ���Ч
     * @param[dwUser] �û�id
     * @param[bLarge] �Ƿ����
     */
    void playHL_CompaireAudio(const DWORD &dwUser, bool bLarge);
    
    /*
     * @brief ���ų���������Ч
     * @param[dwUser] �û�id
     * @param[carddata] �˿��߼���ֵ
     */
    void playHL_OutSingleCardAudio(const DWORD &dwUser,const BYTE &carddata);
    
    /*
     * @brief ���ݳ������Ͳ�����Ч
     * @param[dwUser] �û�id
     * @param[cardType] ��������
     */
    void playHL_OutCardWithTypeAudio(const DWORD &dwUser,const BYTE &cardType);
    
    /*
     * @brief ���ŷɻ���Ч
     * @param[dwUser] �û�id
     */
    void playHL_PlaneAudio(const DWORD &dwUser);
    
    /*
     * @brief ����ը����Ч
     * @param[dwUser] �û�id
     */
    void playHL_BombAudio(const DWORD &dwUser);
    
    /*
     * @brief ���Žе�����Ч
     * @param[dwUser] �û�id
     * @param[cbCall] �Ƿ�е���(1:�У�2:����)
     */
    void playHL_CallBankerAudio(const DWORD &dwUser, const BYTE &cbCall);
    
    /*
     * @brief ������������Ч
     * @param[dwUser] �û�id
     * @param[cbRod] �Ƿ�������(1:����2:����)
     */
    void playHL_RodBankerAudio(const DWORD &dwUser, const BYTE &cbRod);
    
    /*
     * @brief ���żӱ���Ч
     * @param[dwUser] �û�id
     * @param[cbRod] �Ƿ�ӱ�(1:�ӣ�2:����)
     */
    void playHL_DoubleAudio(const DWORD &dwUser, const BYTE &cbDouble);
    
    /*
     * @brief �ɻ�������Ч
     */
    void playHL_AirplaneEffectAudio();
    
    /*
     * @brief ���������Ч
     */
    void playHL_RocketEffectAudio();
    
    /*
     * @brief ը��������Ч
     */
    void playHL_BombEffectAudio();
    
    /*
     * @brief ���涯����Ч
     */
    void playHL_AlarmEffectAudio();
    
    /*
     * @brief ϴ����Ч
     */
    void playHL_DispatchEffectAudio();
    
#pragma mark ͨ��ţţ��Ч
    /*
     * @brief ����
     */
    void playTBNN_OpenCardAudio();
    
    /*
     * @brief ��ȡ��ţţ
     */
    void playTBNN_NiuNiuAudio();
    
    /*
     * @brief ͨ��ţţ��Ϸ�����Ч
     * @param[cbRes] 0:δ���룻1:Ӯ��2:��
     */
    void playTBNN_GameEndAudio(const BYTE &cbRes);

private:
    //�Ƿ������Ч
    bool filterAudio();

private:
    static AudioMgr* s_audioInstance;
};

#endif /* AudioMgr_hpp */
