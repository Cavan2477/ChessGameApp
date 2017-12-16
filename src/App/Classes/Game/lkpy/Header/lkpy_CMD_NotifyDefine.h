#pragma once

#pragma pack(1)

#include "cocos2d.h"

typedef int RESULT_TYPE;            //结果处理
#define RESULT_DEFAULT      0       //默认结果

#define RESULT_SUCCESS      1       //处理成功
#define RESULT_FAILURE      2       //处理失败

#define RESULT_BEGAN        3       //处理开始
#define RESULT_INGED        4       //处理中
#define RESULT_FINISH       5       //处理完成

#define RESULT_OTHER        10      //其他处理


//用户金币改变通知
//const std::string whEvent_User_Data_Change  = "whEvent_User_Data_Change";
//const std::string whEvent_Desk_Status       = "whEvent_Desk_Status";                    //桌子状态变化，是否加锁
//const std::string whEvent_Room_StartGame    = "whNd_Room_StartGame";                   //启动游戏
//const std::string whNd_Room_LeaveGame       = "whNd_Room_LeaveGame";                   //离开游戏
const std::string whNd_Auto_Change          = "whNd_Auto_Change";                       //自动设计状态变化

//刷新排行榜数据
static const char whNd_Refresh_Ranking_Data[] = "whNd_Refresh_Ranking_Data";
//反馈消息结果通知
static const char whNd_FeedBack_Result[] = "whNd_FeedBack_Result";
//用户昵称改变通知
static const char whNd_User_NickName_Change[] = "whNd_User_NickName_Change";
//第三方登录结果
static const char whNd_ThirdLogin_Result[] = "whNd_ThirdLogin_Result";
//商店道具数据请求完成
static const char whNd_ShopList_Request_Result[] = "whNd_ShopList_Request_Result";
//新添加socket recv通知
static const char whNd_Socket_RecvPacketNotify[] = "whNd_Socket_RecvPacketNotify";          //socket recv 通知


/****************************************************************************************************/
//已使用
//MARK:socket错误
static const char whNd_Socket_SendPacketFailure[] = "whNd_Socket_SendPacketFailure";         //socket发送失败

//MARK:大厅登陆
static const char whNd_Hall_Sub_MB_LogonResult[]  = "whNd_Hall_Sub_MB_LogonResult";          //登陆通知
static const char whNd_Hall_Sub_GR_ConfigList[]   = "whNd_Hall_Sub_GR_ConfigList";           //列表配置

//MARK:房间登陆
static const char whNd_Hall_Sub_GR_LogonResult[]  = "whNd_Hall_Sub_GR_LogonResult";          //房间登陆通知
static const char whNd_Hall_Sub_GR_ConfigRoom[]   = "whNd_Hall_Sub_GR_ConfigRoom";           //房间配置

//MARK:启动通知
static const char whNd_Hall_StartRoomList[]       = "whNd_Hall_StartRoomList";               //启动房间
static const char whNd_Room_StartTableList[]      = "whNd_Room_StartTableList";              //启动桌子


static const char whNd_Room_CloseGame[]           = "whNd_Room_CloseGame";                   //关闭游戏

/***********************************************************************************************/

//下面未修改
//MARK:系统公告
static const char whNd_Main_Sub_CM_System[]       = "whNd_Main_Sub_CM_System";               //系统公告
static const char whNd_Main_Sub_CM_CloseGame[]    = "whNd_Main_Sub_CM_CloseGame";            //关闭游戏
static const char whNd_Main_Sub_CM_CloseRoom[]    = "whNd_Main_Sub_CM_CloseRoom";            //关闭房间
static const char whNd_Main_Sub_CM_CloseLink[]    = "whNd_Main_Sub_CM_CloseLink";            //关闭连接

//MARK:道具通知
static const char whNd_Main_Sub_GR_PropertySuccess[] = "whNd_Main_Sub_GR_PropertySuccess";   //道具成功
static const char whNd_Main_Sub_GR_PropertyFailure[] = "whNd_Main_Sub_GR_PropertyFailure";   //道具失败
static const char whNd_Main_Sub_GR_PropertyBuy[]     = "whNd_Main_Sub_GR_PropertyBuy";       //道具购买
static const char whNd_Room_Sub_GR_PropertyTrumpet[] = "whNd_Room_Sub_GR_PropertyTrumpet";   //喇叭消息
static const char whNd_Main_Sub_GR_PropUse[]         = "whNd_Main_Sub_GR_PropUse";           //使用道具

#pragma mark -
#pragma mark FramePlaza 通知
//锁屏超时处理
static const char whNd_Spinner_TimeOut[]     = "whNd_Spinner_TimeOut";                      //锁屏超时处理

//MARK:socket提示
static const char whNd_Socket_CloseLink[]    = "whNd_Socket_CloseLink";                     //超时关闭
static const char whNd_Socket_OutTime[]      = "whNd_socket_OutTime";                       //超时退出
//static const char whNd_Socket_Disconnect[]   = "whNd_Socket_Disconnect";                    //网络断开

//修改资料
static  const char whNd_userInfo_headRepair[]  = "whNd_userInfo_headRepair";                  //头像修改
static const char whNd_userInfo_modify[]  = "whNd_userInfo_modify";                          //资料修改

//等级参数
static const char  whNd_Level_Para[] = "whNd_Level_para";
static const char whNd_GrowLevelUpgrade[] = "whNd_GrowLevelUpgrade";
//签到信息
static const char whNd_Check_Info[] = "whNd_Check_Info";
static const char whNd_check_Result[] = "whNd_check_Result";

//低保
static const char whNd_BASEENSURE_PARAMETER[] = "whNd_BASEENSURE_PARAMETER";
static const char whNd_BASEENSURE_RESULT[] = "whNd_BASEENSURE_RESULT";

//任务
static const char whNd_TASK_LIST[] = "whNd_TASK_LIST";
static const char whNd_TASK_INFO[] = "whNd_TASK_INFO";
static const char whNd_TASK_RESULT[] = "whNd_TASK_RESULT";


//兑换
static const char whNd_EXCHANGE_PARAMETER[] = "whNd_EXCHANGE_PARAMETER";
static const char whNd_EXCHANGE_RESULT[] = "whNd_EXCHANGE_RESULT";
#pragma pack()

















 
 
