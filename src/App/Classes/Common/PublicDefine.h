/************************************************************************************
 * file: 		PublicDefine.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/16 22:43:42
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/16			
 *
 ************************************************************************************/

#ifndef __PUBLIC_DEFINE_H__
#define __PUBLIC_DEFINE_H__

#include "MacroDefine.h"
#include <string>

using namespace std;

#define BANK		1000
#define SHOP		2000
#define TASK		3000
#define RANK		4000
#define PERSONAL	5000

#define WIN_SIZE				Director::getInstance()->getVisibleSize()
#define THIRD_ACCOUNT           "third_account"								// 第三方帐号
#define THIRD_NICK_NAME         "third_nickname"							// 第三方昵称
#define THIRD_HEAD_IMAGE_URL    "head_image_url"							// 第三方头像url

#define FONT_ARIAL              "Arial"
#define FONT_ARIAL_BOLDMT       "Arial-BoldMT"
#define FONT_TREBUCHET_MS_BOLD  "TrebuchetMS-Bold"

#define DEFAULT_COLOR			Color3B(75, 75, 75)							// 默认字体颜色
#define WATHETBLUE				Color3B(10, 224, 206)						// 浅蓝色

#define ROOM_CURRENT_LAYER      100											// 房间当前层tag值

#define WIN_SIZE				Director::getInstance()->getVisibleSize()
#define JUDGE_SCALE				WIN_SIZE.width/1136.f
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

typedef std::function<void(WORD, void *, WORD)> gameMessageCallback;		//消息回调
typedef std::function<void(void *, WORD)> MessageCallback1;					//用户服务消息
typedef std::function<void(void *, void *, WORD, bool)> gameMessageRecv;	//接受消息

#define GLCOLOR_RGB(r,g,b)              ((COLORREF)(((0)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))

// 友盟
#define UM_KEY				@"56dfc4d567e58ecbf2001ede"

// 微信
#define WX_KEY				@"wx56b110375f495f83"
#define WX_SECRET			@"6b513229647af031180038742d5cbbcd"
#define WX_URL				@"http://jd.foxuc.net"

// sina
#define SINA_APP_KEY		@"34264003"
#define SINA_APP_SECRET		@"48333fe8331979d83ebd2d38c1077038"
#define SINA_REDIRECT_URI	@"http://jd.foxuc.net"

// 模式类型枚举
typedef enum 
{
	EM_MODE_TYPE_WAIT = 0,				// 等待弹框
	EM_MODE_TYPE_WAIT_TEXT,				// 文字提示等待
	EM_MODE_TYPE_ENSURE,				// 确定弹框
	EM_MODE_TYPE_ENSURE_CANCEL,			// 确定取消弹框
	EM_MODE_TYPE_INFO_REMINDER,			// 消息提示弹框
	EM_MODE_TYPE_REMOVE,				// 移除弹框
}EM_MODE_TYPE;

// 用户数据变更枚举
typedef enum 
{
	EM_USER_DATA_CHANGE_NAME = 0,		// 用户数据变更：昵称
	EM_USER_DATA_CHANGE_HEAD,			// 用户数据变更：头像
	EM_USER_DATA_CHANGE_LEVEL,			// 用户数据变更：等级
	EM_USER_DATA_CHANGE_SCORE,			// 用户数据变更：总分
	EM_USER_DATA_CHANGE_BEAN,			// 用户数据变更：游戏豆
}EM_USER_DATA_CHANGE;

// 游戏种类枚举
typedef enum
{
	EM_GAME_DEFALUT = INVALID_DWORD,	// 默认类型
	EM_GAME_ZJH		= 6,				// 扎金花
	EM_GAME_NIUNIU	= 27,				// 牛牛类型
	EM_GAME_TBNN	= 36,				// 通比牛牛
	EM_GAME_BRNN	= 104,				// 百人牛牛
	EM_GAME_BAIJIALE= 122,				// 百家乐类型
	EM_GAME_LKPY	= 510,				// 李逵劈鱼
}EM_GAME;

//游戏元素数量
typedef enum
{
	EM_GAME_LANDLORD_ELEMENT_DEFAULT = 20,	// 斗地主一方玩家最大牌
}EM_GAME_LANDLORD_ELEMENT;

//下载状态
typedef enum
{
	EM_DOWNLOAD_NONE	= 0,			// 无下载任务
	EM_DOWNLOADING		= 1,			// 下载中
	EM_DOWNLOAD_DONE	= 2,			// 下载完成
	EM_DOWNLOAD_CANCEL	= 3,			// 下载取消
	EM_DOWNLOAD_FAILURE = 4				// 下载失败
}EM_DOWNLOAD;

// 游戏下载进度
typedef struct _stDownloadRefresh
{
	EM_GAME			emGame;				// 游戏类型
	DOUBLE			dDownloadPercent;	// 下载百分比
}ST_DOWNLOAD_REFRESH, *PST_DOWNLOAD_REFRESH;

// 下载信息
typedef struct _stDownloadInfo
{
	EM_GAME			emGame;				// 游戏类型
	EM_DOWNLOAD		emDownloadStatus;	// 下载状态
}ST_DOWNLOAD_INFO, *PST_DOWNLOAD_INFO;

// 银行礼物赠送信息
typedef struct _stBankGiftInfo
{
	int		nSendType;					// 0:id; 1:昵称
	string	sReceiveUserName;
	DWORD	dwReceiveUserId;
	LONGLONG	llSendCount;
}ST_BANK_GIFT_INFO, *PST_BANK_GIFT_INFO;

// 喇叭数据类
class TrumpetData:public cocos2d::Ref
{
public:
    TrumpetData()
    :sendnickname("")
    ,sendtext("")
    {
    }
    
    std::string sendnickname;			// 发送者昵称
    std::string sendtext;				// 发送内容
};

#endif
