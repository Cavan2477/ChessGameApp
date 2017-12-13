#ifndef __UIMsgDefine_H__
#define __UIMsgDefine_H__

const int UI_BASE_CODE = 100;

enum UI_MSG_BASE
{
    UI_MSG_BEGIN = UI_BASE_CODE,
    UI_MSG_LOGIN,
	UI_MSG_PLAZA,
	UI_MSG_GAME,
    UI_MSG_TASK,
    UI_MSG_BANK,
    UI_MSG_CHECKIN,
    UI_MSG_EXCHANGE,
    UI_MSG_PERSON,
    UI_MSG_COMMON,
};

enum LOGIN_UI
{
    REISTER_LAYER                           = UI_MSG_BASE::UI_MSG_LOGIN + 1,            //注册界面
    LOGIN_LAYER,                                                                        //登陆界面
    SELECT_ACCOUNT,                                                                     //选择账号
    DELETE_ACCOUNT,                                                                     //删除账号
};

enum PLAZA_UI
{
    ENTER_ROOM                              = UI_MSG_BASE::UI_MSG_PLAZA + 1,			//进入房间
	SIT_DOWN,                                                                           //入席
	ENTER_GAME,                                                                         //进入游戏
	EXIT_PLAZA,                                                                         //退出大厅
    CLICK_DESK,                                                                         //点击桌子按钮
    REMOVE_TOP_LAYER,                                                                   //移除覆盖的层
};

enum GAME_UI
{
    EXIT_GAME                               = UI_MSG_BASE::UI_MSG_GAME + 1,				//退出游戏
    BROW_CHAT,                                                                          //表情聊天
    TEXT_CHAT,                                                                          //文本聊天
    ROLE_INFO,
    CONTINUE,
    HIDE_CARDLAYER,
    APPLY_BANKER,
    VALID_START,                                                                        //明牌开始
};

enum TASK_UI
{
    TAKE_TASK                               = UI_MSG_BASE::UI_MSG_TASK + 1,             //领取任务
    TAKE_REWARD,                                                                        //领取奖励
};

enum BANK_UI
{
    OPEN_BANK                               = UI_MSG_BASE::UI_MSG_BANK + 1,             //开通银行
    SAVE_GOLD,                                                                          //存入
    OUT_GOLD,                                                                           //取出
    SEND_GOLD_ID,                                                                       //赠送(by id)
    SEND_GOLD_NICK,                                                                     //赠送(by nick)
    STR_CHECK,                                                                          //字符串检查
    PASSWD_CHECK,
    SUREPASSWD_CHECK,
};

enum CHECKIN_UI
{
    GET_BASEENSURE                          = UI_MSG_BASE::UI_MSG_CHECKIN + 1,          //领取低保
    GAME_CHECKIN,                                                                       //游戏签到
};

enum EXCHANGE_UI
{
    EXCHANGE_BTN                            = UI_MSG_BASE::UI_MSG_EXCHANGE + 1,         //兑换
    EXCHANGE_INGOT,
    EXCHANGE_BEAN,
    VIEW_MODE,
};

enum PERSON_UI
{
    CHANGE_LOGIN_PW                         = UI_MSG_BASE::UI_MSG_PERSON + 1,           //修改登陆密码
    CHANGE_BANK_PW,
    CHANGE_INFO,
    CHANGE_IMG,//头像
};

enum COMMON_UI
{
    ENABLE_BTN,                                                                         //界面按钮
};

#endif	// __UIMsgDefine_H__