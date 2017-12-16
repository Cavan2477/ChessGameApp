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

//大厅数据管理 以及玩家数据

#ifndef __HallDataMgr_h__
#define __HallDataMgr_h__

#include <stdio.h>
#include "cocos2d.h"
#include "UserData.h"
#include "../Common/CMD_Stdafx.h"
#include "../Scene/ModeLayer.h"
#include "../Public/Help.h"
#include "../Plaza/Personal/PhotoBridge.h"

//分系统游戏数据 add by zhong
struct tagSubSystemParam
{
    int m_nPlayerCount;         //游戏桌人数
    int m_nMaxCount;            //游戏元素最大数量 (如最大扑克数量)
    
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
    
    //保存和读取桌子密码
    void readTablepass();
    
    void saveTablepass(const std::string &pass);
    
    void roomlistclear();
    
    //获取签名验证
    std::string getSignature(SCORE times);
    
    cocos2d::Map<int , cocos2d::Texture2D *> m_Headlist;//头像
    
    std::vector<_stGameRoomServer *> m_roomList;    //所有的房间列表
    
    std::vector<_stGameRoomServer *> m_subRoomList; //分系统房间列表
    
    cocos2d::Map<int, UserData *> m_UserList;// 用户列表
    
    CMD_GP_GrowLevelParameter m_levelData;
    
    cocos2d::Vector<cocos2d::__String *> m_chatlist;//系统消息列表
    
    cocos2d::Vector<cocos2d::__String *> m_noticelist;//系统公告列表
    
    std::map<std::string,std::map<int,std::string>> m_adderssList;   //地址解析
    
    PhotoCallback m_completecallback;               //自定义头像
    std::function<void()> _versionCheckcallback;    //版本校验
    
    UserData* getUserData(const DWORD &userId);
    
    void clearUserData();
    
    UserData* getUserDataByChair(const DWORD &chairId);
public:
    cocos2d::ApplicationProtocol::Platform m_targetPlatform;
    
    //调整到分系统游戏缩放标准
    void toSubSystemScale();
    
    //调整到主系统游戏缩放标准
    void toMainSystemScale();
    
    //两套尺寸
    CC_SYNTHESIZE(cocos2d::Size, m_szMainSize, MainSystemSize);
    CC_SYNTHESIZE(cocos2d::Size, m_szSubSize, SubSystemSize);
public:
    //登录设置
    bool						m_bRemember;					//记录密码
    bool						m_bAutomatic;					//自动登录
    bool						m_bStartType;					//启动标识
    bool                        m_bStartGame;                   //启动游戏
    BYTE                        m_cbMoorMachine;                //锁定机器
    
    BYTE						m_cbGameStatus;					//游戏状态
    
    EM_LOAD_TYPE                    m_loadtype;                     //登陆模式
    bool                        m_bcancel;                      //是否注销当前帐号
    
    BYTE                        m_cbInsureEnable;               //银行使能标示
    
    WORD                        m_Gametype;                     //游戏类型，比赛或者普通
    DWORD                       m_RoomRule;                     //房间规则
    
    int                         m_RoomType;                     //是在房间界面还是桌子界面
    std::string                 m_roomaddress;                    //房间IP，游戏断线重连
    std::string                 m_roomPass;                     //房间密码，游戏断线重连
    WORD                        m_roomPort;                     //房间端口，游戏断线重连
    
    int                         m_TableCount;                   //总的桌子数
    int							m_ChairCount;                   //桌子椅子数  每页:默认 6
    CMD_GR_TableInfo            m_Tableinfo;
    
    std::string                 m_MethodHeadUrl;                //第三方头像地址
    
    bool                        m_isEnterBack;                  //游戏是否退到后台，退到后台子弹发射消息不处理
    LONGLONG                    m_enterbacktime;                //进入后台时间
    
    SCORE                       m_Logintime;//登录成功时间
    
    bool                        m_IsLogin;                      //是否是登录界面
    
    bool                        m_isFirstSendGameOption;        //是否第一次发送查询场景消息
    //分系统游戏参数
    tagSubSystemParam           m_tagSubParam;                  //分系统游戏参数
    //银行赠送信息
    _stBankGiftInfo             m_tagBankSend;
    //ip地址
    std::string                 m_strGameIpAdress;
private:
    cocos2d::Layer *m_popLayer;//消息提示弹出框

};
#endif /* defined(__MyGame__HallDataMgr__) */
