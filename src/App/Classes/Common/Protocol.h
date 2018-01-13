/************************************************************************************
 * file: 		Protocol.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:21:23
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

/*
 ��������ί��Э�鷽��
 ������Ҫʵ�����н���Ĺ��з���
	
	1.Э��		��ί��
	2.������	��ťί��
	3.��¼����	��¼ί��
	4.�������	��ťί��
	5.��λ����	��ťί��
	6.��Ϸ����	�����
 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "cocos2d.h"
#include "MacroDefine.h"
#include "CMD_GameServer.h"
#include "struct.h"
#include "../Public/Help.h"

#pragma mark -
#pragma mark ������ʾ

//������ʾ
#define ALERT_DEFAULT_TIPS					0								// Ĭ��
#define ALERT_LOGON_FAILURE					2								// ����ʧ��
#define ALERT_EXCEPTION_TIPS				3								// �쳣�Ͽ�
#define ALERT_OUTTIME_TIPS					4								// ���糬ʱ�Ͽ�
#define ALERT_ROOMLOAD_TIPS					5								// ���뷿��ʧ��
#define ALERT_DESKLIST_TIPS					6								// ����ʧ��
#define ALERT_GAME_DISCONNET				7								// ��Ϸ���쳣�Ͽ�
#define ALERT_UPDATE_TIPS					9								// ������ʾ
#define ALERT_REQUESTFAILURE_TIPS			10								// ����ʧ����ʾ

//������ʾ
#define ALERT_GP_DEFAULT                    0                               // Ĭ��
#define ALERT_GP_LOGON_FAILURE              1                               // ��½ʧ��
#define ALERT_GR_LOGON_FAILURE              2                               // ����ʧ��


//ϵͳ��ʾ
#define ALERT_CLOSE_GAME                    11                              // �ر���Ϸ
#define ALERT_CLOSE_ROOM                    12                              // �رշ���
#define ALERT_CLOSE_LINK                    13                              // �ж�����
#define ALERT_SYSTEM_MGS                    14                              // ϵͳ��Ϣ
#define ALERT_ACTION_MGS                    15                              // ������Ϣ

#define ALERT_SHORTSTREAM                   20                              // �����Ӵ���

//#pragma pack(0)

//#pragma pack(1)
//��½����
typedef enum 
{
    EM_LOAD_TYPE_DEFAULT = -1,
	EM_LOAD_TYPE_NORMAL,
	EM_LOAD_TYPE_QQ,
	EM_LOAD_TYPE_SINA,
	EM_LOAD_TYPE_RENREN,
	EM_LOAD_TYPE_VISITOR
}EM_LOAD_TYPE;

//���ݷ���
typedef enum _EM_DATA_TYPE
{
	EM_DATA_TYPE_DEFAULT= 0,			// Ĭ��
	EM_DATA_TYPE_LOAD	= 1,			// ��¼����
	EM_DATA_TYPE_ROOM	= 2,			// ��������
	EM_DATA_TYPE_END	= 10,			// ����
}EM_DATA_TYPE;

//MARK::��ϷЭ��
class CGameSocketDelegate
{
public:
    virtual bool OnGameMessageWithCmdID(void* pData, WORD wSize, WORD wSubCmdID)=0;  //��Ϸ��Ϣ
    virtual bool OnGameMessageWithScene(void* pData, WORD wSize)=0;                  //������Ϣ
};

//MARK::�ʺ�����
class CAccountData
{
public:
    CAccountData()
    :m_cbDevice(DEVICE_TYPE_DEFAULT),m_dwUserID(0),m_wTableID(0),m_wChairID(0),m_dwGameID(0),m_cbStatus(US_NULL),
    m_strAccounts(""),m_strNickName(""),m_strPwd(""),m_strBankPwd(""),m_llGold(0),m_dBean(0),m_llUserBankGold(0),
    m_llUserGold(0),m_strTablePwd("")
    {
    }
    
    ~CAccountData(){}

public:
    std::string     m_strAccounts;                          //�ʺ�
    std::string     m_strPwd;								//����
    std::string     m_strBankPwd;                           //��������
    std::string     m_strNickName;                          //�û��ǳ�
    std::string     m_strDynamicPwd;                        //��̬����
    std::string     m_strTablePwd;                          //��������
    std::string     m_strMachineID;                         //Ӳ����ʶ
    std::string     m_strPhone;								//�绰
    
public:
    BYTE            m_cbDevice;                             //�豸I D
    WORD			m_wChairID;                             //����I D
    WORD			m_wTableID;                             //����I D
    DWORD			m_dwUserID;                             //�û�I D
    DWORD			m_dwGameID;                             //��ϷI D
    DWORD			m_dwKindID;                             //����I D
    BYTE			m_cbStatus;                             //�û�״̬
    WORD			m_wFaceID;                              //ͷ��I D
    WORD            m_wCustom;                              //�Զ���ͷ��
    BYTE			m_cbGender;                             //�û��Ա� 0ΪŮ��1Ϊ��
    
    LONGLONG		m_llGold;								//���
    DOUBLE			m_dBean;								//��Ϸ��
    LONGLONG		m_llUserBankGold;						//���н��
    LONGLONG		m_llUserGold;							//Я�����
};

//MARK::��������
class CYMDataDelegate
{
public:
    CYMDataDelegate()
    :m_nLogonPort(0),m_nGamePort(0)
    {
        
    }
    ~CYMDataDelegate(){}
public:
    TCHAR           m_pYMAddress[32];                       //������ַ
    TCHAR           m_pIPAddress[32];                       //ip��ַ
    int             m_nLogonPort;                           //��½�˿�
    int             m_nGamePort;                            //��Ϸ�˿�
};

class COption
{
public:
    COption()
    :m_fOptionMusic(1.0f),m_fOptionSound(1.0f),m_bOptionMute(false),m_bOptionShake(false),
    m_bOptionPower(false),m_bOptionNovice(false),m_bOptionLookOn(false)
    {
        
    }
    
    ~COption(){}
    float           m_fOptionSound;                         //��Ч��С
    float           m_fOptionMusic;                         //���ִ�С
    bool            m_bOptionMute;                          //������ʶ
    bool            m_bOptionShake;                         //�𶯱�ʶ
    bool            m_bOptionPower;                         //ʡ���ʶ
    bool            m_bOptionLookOn;                        //�Թ۱�ʶ
    bool            m_bOptionNovice;                        //��ʾ��ʶ
};

//MARK::�û�Э��
class UserItemDelegate
{
public:
    //�û�����
    virtual void OnUserEnter(void* pUser){}
    //�û�״̬
    virtual void OnUserStatus(void* pUser){}
    //�û�����
    virtual void OnUserScore(void* pUser){}
    //�û�����
    virtual void OnUserChat(WORD wSubCode, void* pData, WORD wSize)
    {
        cocos2d::log("base chat:%d",wSubCode);
    }
};

#endif






