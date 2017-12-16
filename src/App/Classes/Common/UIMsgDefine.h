/************************************************************************************
 * file: 		UIMsgDefine.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/16 22:29:05
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/16			
 *
 ************************************************************************************/

#ifndef __UI_MSG_DEFINE_H__
#define __UI_MSG_DEFINE_H__

const int UI_BASE_CODE = 100;

typedef enum 
{
    EM_UI_MSG_BEGIN = UI_BASE_CODE,
    EM_UI_MSG_LOGIN,
	EM_UI_MSG_PLAZA,
	EM_UI_MSG_GAME,
    EM_UI_MSG_TASK,
    EM_UI_MSG_BANK,
    EM_UI_MSG_CHECKIN,
    EM_UI_MSG_EXCHANGE,
    EM_UI_MSG_PERSON,
    EM_UI_MSG_COMMON,
}ENUM_UI_MSG;

typedef enum 
{
    EM_LOGIN_UI_REISTER_LAYER	= ENUM_UI_MSG::EM_UI_MSG_LOGIN + 1,						// 注册界面
    EM_LOGIN_UI_LAYER,                                                                  // 登陆界面
    EM_LOGIN_UI_SELECT_ACCOUNT,                                                         // 选择账号
    EM_LOGIN_UI_DELETE_ACCOUNT,                                                         // 删除账号
}ENUM_LOGIN_UI;

typedef enum 
{
	EM_PLAZA_UI_ENTER_ROOM		= ENUM_UI_MSG::EM_UI_MSG_PLAZA + 1,						// 进入房间
	EM_PLAZA_UI_SIT_DOWN,                                                               // 入席
	EM_PLAZA_UI_ENTER_GAME,                                                             // 进入游戏
	EM_PLAZA_UI_EXIT_PLAZA,                                                             // 退出大厅
	EM_PLAZA_UI_CLICK_DESK,                                                             // 点击桌子按钮
	EM_PLAZA_UI_REMOVE_TOP_LAYER,                                                       // 移除覆盖的层
}ENUM_PLAZA_UI;

typedef enum 
{
    EM_GAME_UI_EXIT_GAME       = ENUM_UI_MSG::EM_UI_MSG_GAME + 1,						// 退出游戏
    EM_GAME_UI_BROW_CHAT,                                                               // 表情聊天
    EM_GAME_UI_TEXT_CHAT,                                                               // 文本聊天
    EM_GAME_UI_ROLE_INFO,
    EM_GAME_UI_CONTINUE,
    EM_GAME_UI_HIDE_CARD_LAYER,
    EM_GAME_UI_APPLY_BANKER,
    EM_GAME_UI_VALID_START,                                                             // 明牌开始
}ENUM_GAME_UI;

typedef enum 
{
    EM_TASK_UI_TAKE_TASK       = ENUM_UI_MSG::EM_UI_MSG_TASK + 1,						// 领取任务
    EM_TASK_UI_TAKE_REWARD,                                                             // 领取奖励
}ENUM_TASK_UI;

typedef enum 
{
    EM_BANK_UI_OPEN_BANK		= ENUM_UI_MSG::EM_UI_MSG_BANK + 1,						// 开通银行
    EM_BANK_UI_SAVE_GOLD,                                                               // 存入
    EM_BANK_UI_OUT_GOLD,                                                                // 取出
	EM_BANK_UI_SEND_GOLD_ID,                                                            // 赠送(by id)
	EM_BANK_UI_SEND_GOLD_NICK,                                                          // 赠送(by nick)
	EM_BANK_UI_STR_CHECK,                                                               // 字符串检查
    EM_BANK_UI_PWD_CHECK,
    EM_BANK_UI_SUREPWD_CHECK,
}ENUM_BANK_UI;

typedef enum 
{
    EM_CHECKIN_UI_GET_BASE_ENSURE= ENUM_UI_MSG::EM_UI_MSG_CHECKIN + 1,					// 领取低保
    EM_CHECKIN_UI_GAME_CHECKIN,                                                         // 游戏签到
}ENUM_CHECKIN_UI;

typedef enum 
{
    EM_EXCHANGE_UI_BTN			= ENUM_UI_MSG::EM_UI_MSG_EXCHANGE + 1,					// 兑换
    EM_EXCHANGE_UI_INGOT,
    EM_EXCHANGE_UI_BEAN,
    EM_EXCHANGE_UI_VIEW_MODE,
}ENUM_EXCHANGE_UI;

typedef enum 
{
    EM_PERSON_UI_CHANGE_LOGIN_PWD = ENUM_UI_MSG::EM_UI_MSG_PERSON + 1,					// 修改登陆密码
    EM_PERSON_UI_CHANGE_BANK_PWD,
    EM_PERSON_UI_CHANGE_INFO,
    EM_PERSON_UI_CHANGE_HEAD_IMAGE,														// 头像
}ENUM_PERSON_UI;

typedef enum 
{
    EM_COMMON_UI_ENABLE_BTN,                                                            // 界面按钮
}ENUM_COMMON_UI;

#endif	// __UIMsgDefine_H__