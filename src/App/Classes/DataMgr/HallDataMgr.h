/************************************************************************************
 * file: 		HallDataMgr.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:48:02
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

//�������ݹ��� �Լ��������

#ifndef __HallDataMgr_h__
#define __HallDataMgr_h__

#include <stdio.h>
#include "cocos2d.h"
#include "UserData.h"
#include "../Common/CMD_Stdafx.h"
#include "../Scene/ModeLayer.h"
#include "../Public/Help.h"
#include "../Plaza/Personal/PhotoBridge.h"

//��ϵͳ��Ϸ���� add by zhong
struct tagSubSystemParam
{
    int m_nPlayerCount;         //��Ϸ������
    int m_nMaxCount;            //��ϷԪ��������� (������˿�����)
    
    tagSubSystemParam()
    {
        m_nPlayerCount = 1;
        m_nMaxCount = 0;
    }
};

class HallDataMgr : public cocos2d::Ref, public CAccountData, public COption
{
public:
    HallDataMgr();
    
    ~HallDataMgr();
    
    cocos2d::Layer *AddpopLayer(const std::string &title, const std::string &content, int type,bool close =false);
    
    static HallDataMgr *getInstance();
    
    void saveConfig();
    
    void readConfig();
    
    void readSelectServer();
    
    void saveSelectServer(const std::string &strAdress);
    
    //����Ͷ�ȡ��������
    void readTablepass();
    
    void saveTablepass(const std::string &pass);
    
    void roomlistclear();
    
    //��ȡǩ����֤
    std::string getSignature(LONGLONG times);
    
    cocos2d::Map<int , cocos2d::Texture2D *> m_Headlist;//ͷ��
    
    std::vector<_stGameRoomServer *> m_roomList;    //���еķ����б�
    
    std::vector<_stGameRoomServer *> m_subRoomList; //��ϵͳ�����б�
    
    cocos2d::Map<int, UserData *> m_UserList;// �û��б�
    
    ST_CMD_GP_GROW_LEVEL_PARAM m_levelData;
    
    cocos2d::Vector<cocos2d::__String *> m_chatlist;//ϵͳ��Ϣ�б�
    
    cocos2d::Vector<cocos2d::__String *> m_noticelist;//ϵͳ�����б�
    
    std::map<std::string,std::map<int,std::string>> m_adderssList;   //��ַ����
    
    PhotoCallback m_completecallback;               //�Զ���ͷ��
    std::function<void()> _versionCheckcallback;    //�汾У��
    
    UserData* getUserData(const DWORD &userId);
    
    void clearUserData();
    
    UserData* getUserDataByChair(const DWORD &chairId);
public:
    cocos2d::ApplicationProtocol::Platform m_targetPlatform;
    
    //��������ϵͳ��Ϸ���ű�׼
    void toSubSystemScale();
    
    //��������ϵͳ��Ϸ���ű�׼
    void toMainSystemScale();
    
    //���׳ߴ�
    CC_SYNTHESIZE(cocos2d::Size, m_szMainSize, MainSystemSize);
    CC_SYNTHESIZE(cocos2d::Size, m_szSubSize, SubSystemSize);
public:
    //��¼����
    bool						m_bRemember;					//��¼����
    bool						m_bAutomatic;					//�Զ���¼
    bool						m_bStartType;					//������ʶ
    bool                        m_bStartGame;                   //������Ϸ
    BYTE                        m_cbMoorMachine;                //��������
    
    BYTE						m_cbGameStatus;					//��Ϸ״̬
    
    EM_LOAD_TYPE                    m_loadtype;                     //��½ģʽ
    bool                        m_bcancel;                      //�Ƿ�ע����ǰ�ʺ�
    
    BYTE                        m_cbInsureEnable;               //����ʹ�ܱ�ʾ
    
    WORD                        m_Gametype;                     //��Ϸ���ͣ�����������ͨ
    DWORD                       m_RoomRule;                     //�������
    
    int                         m_RoomType;                     //���ڷ�����滹�����ӽ���
    std::string                 m_roomaddress;                    //����IP����Ϸ��������
    std::string                 m_roomPass;                     //�������룬��Ϸ��������
    WORD                        m_roomPort;                     //����˿ڣ���Ϸ��������
    
    int                         m_TableCount;                   //�ܵ�������
    int							m_ChairCount;                   //����������  ÿҳ:Ĭ�� 6
    ST_CMD_GR_TABLE_INFO            m_Tableinfo;
    
    std::string                 m_MethodHeadUrl;                //������ͷ���ַ
    
    bool                        m_isEnterBack;                  //��Ϸ�Ƿ��˵���̨���˵���̨�ӵ�������Ϣ������
    LONGLONG                    m_enterbacktime;                //�����̨ʱ��
    
    LONGLONG                       m_Logintime;//��¼�ɹ�ʱ��
    
    bool                        m_IsLogin;                      //�Ƿ��ǵ�¼����
    
    bool                        m_isFirstSendGameOption;        //�Ƿ��һ�η��Ͳ�ѯ������Ϣ
    //��ϵͳ��Ϸ����
    tagSubSystemParam           m_tagSubParam;                  //��ϵͳ��Ϸ����
    //����������Ϣ
    _stBankGiftInfo             m_tagBankSend;
    //ip��ַ
    std::string                 m_strGameIpAdress;
private:
    cocos2d::Layer *m_popLayer;//��Ϣ��ʾ������

};
#endif /* defined(__MyGame__HallDataMgr__) */
