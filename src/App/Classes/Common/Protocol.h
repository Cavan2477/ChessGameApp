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
 定义所有委托协议方法
 大厅需要实现所有界面的共有方法
	
	1.协议		流委托
	2.主界面	按钮委托
	3.登录界面	登录委托
	4.房间界面	按钮委托
	5.桌位界面	按钮委托
	6.游戏界面	待完成
 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "cocos2d.h"
#include "MacroDefine.h"
#include "CMD_GameServer.h"
#include "struct.h"
#include "../Public/Help.h"

#pragma mark -
#pragma mark 警告提示

//弹窗提示
#define ALERT_DEFAULT_TIPS					0								// 默认
#define ALERT_LOGON_FAILURE					2								// 连接失败
#define ALERT_EXCEPTION_TIPS				3								// 异常断开
#define ALERT_OUTTIME_TIPS					4								// 网络超时断开
#define ALERT_ROOMLOAD_TIPS					5								// 进入房间失败
#define ALERT_DESKLIST_TIPS					6								// 坐下失败
#define ALERT_GAME_DISCONNET				7								// 游戏中异常断开
#define ALERT_UPDATE_TIPS					9								// 更新提示
#define ALERT_REQUESTFAILURE_TIPS			10								// 请求失败提示

//大厅提示
#define ALERT_GP_DEFAULT                    0                               // 默认
#define ALERT_GP_LOGON_FAILURE              1                               // 登陆失败
#define ALERT_GR_LOGON_FAILURE              2                               // 房间失败


//系统提示
#define ALERT_CLOSE_GAME                    11                              // 关闭游戏
#define ALERT_CLOSE_ROOM                    12                              // 关闭房间
#define ALERT_CLOSE_LINK                    13                              // 中断连接
#define ALERT_SYSTEM_MGS                    14                              // 系统消息
#define ALERT_ACTION_MGS                    15                              // 动作消息

#define ALERT_SHORTSTREAM                   20                              // 短连接处理

//#pragma pack(0)

//#pragma pack(1)
//登陆类型
typedef enum 
{
    EM_LOAD_TYPE_DEFAULT = -1,
	EM_LOAD_TYPE_NORMAL,
	EM_LOAD_TYPE_QQ,
	EM_LOAD_TYPE_SINA,
	EM_LOAD_TYPE_RENREN,
	EM_LOAD_TYPE_VISITOR
}EM_LOAD_TYPE;

//数据分类
typedef enum _EM_DATA_TYPE
{
	EM_DATA_TYPE_DEFAULT= 0,			// 默认
	EM_DATA_TYPE_LOAD	= 1,			// 登录命令
	EM_DATA_TYPE_ROOM	= 2,			// 房间命令
	EM_DATA_TYPE_END	= 10,			// 结束
}EM_DATA_TYPE;

//MARK::游戏协议
class CGameSocketDelegate
{
public:
    virtual bool OnGameMessageWithCmdID(void* pData, WORD wSize, WORD wSubCmdID)=0;  //游戏消息
    virtual bool OnGameMessageWithScene(void* pData, WORD wSize)=0;                  //场景消息
};

//MARK::帐号数据
class CAccountData
{
public:
    CAccountData()
    :m_cbDevice(DEVICE_TYPE_DEFAULT),m_dwUserID(0),m_wTableID(0),m_wChairID(0),m_dwGameID(0),m_cbStatus(US_NULL),
    m_pAccounts(""),m_pNickName(""),m_pPassword(""),m_pBankword(""),m_Ingot(0),m_Bean(0),m_UserInsure(0),
    m_UserScore(0),m_Tablepass("")
    {
    }
    
    ~CAccountData(){}
public:
    std::string     m_pAccounts;                            //帐号
    std::string     m_pPassword;                            //密码
    std::string     m_pBankword;                            //银行密码
    std::string     m_pNickName;                            //用户昵称
    std::string     m_dynamicpass;                          //动态密码
    
    std::string     m_Tablepass;                            //桌子密码
    
    std::string     m_Machine;                              //硬件标识
    
    std::string     m_pPhoneNum;                            //电话
    
public:
    BYTE            m_cbDevice;                             //设备I D
    WORD			m_wChairID;                             //椅子I D
    WORD			m_wTableID;                             //桌子I D
    DWORD			m_dwUserID;                             //用户I D
    DWORD			m_dwGameID;                             //游戏I D
    DWORD			m_dwKindID;                             //类型I D
    BYTE			m_cbStatus;                             //用户状态
    WORD			m_wFaceID;                              //头像I D
    WORD            m_wCustom;                              //自定义头像
    BYTE			m_cbGender;                             //用户性别 0为女，1为男
    
    SCORE           m_Ingot;                                //元宝
    DOUBLE          m_Bean;                                 //游戏豆
    SCORE           m_UserInsure;                           //银行金币
    SCORE           m_UserScore;                            //携带金币
};

//MARK::域名数据
class CYMDataDelegate
{
public:
    CYMDataDelegate()
    :m_nLogonPort(0),m_nGamePort(0)
    {
        
    }
    ~CYMDataDelegate(){}
public:
    TCHAR           m_pYMAddress[32];                       //域名地址
    TCHAR           m_pIPAddress[32];                       //ip地址
    int             m_nLogonPort;                           //登陆端口
    int             m_nGamePort;                            //游戏端口
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
    float           m_fOptionSound;                         //音效大小
    float           m_fOptionMusic;                         //音乐大小
    bool            m_bOptionMute;                          //静音标识
    bool            m_bOptionShake;                         //震动标识
    bool            m_bOptionPower;                         //省电标识
    bool            m_bOptionLookOn;                        //旁观标识
    bool            m_bOptionNovice;                        //提示标识
};

//MARK::用户协议
class UserItemDelegate
{
public:
    //用户进入
    virtual void OnUserEnter(void* pUser){}
    //用户状态
    virtual void OnUserStatus(void* pUser){}
    //用户分数
    virtual void OnUserScore(void* pUser){}
    //用户聊天
    virtual void OnUserChat(WORD wSubCode, void* pData, WORD wSize)
    {
        cocos2d::log("base chat:%d",wSubCode);
    }
};

#endif






