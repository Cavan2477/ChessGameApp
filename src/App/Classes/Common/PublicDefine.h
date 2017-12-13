//
//  PublicDefine.h
//  MyGame
//
//  Created by wh on 15/4/23.
//
//

#ifndef MyGame_PublicDefine_h
#define MyGame_PublicDefine_h

#include "MacroDefine.h"

#define WinSize Director::getInstance()->getVisibleSize()
#define ThirdAccount            "third_account"//第三方帐号
#define ThirdNickName           "third_nickname"//第三方昵称
#define ThirdHeadImageUrl       "head_image_url"//第三方头像url

#define FontNormal              "Arial"
#define FontBold                "Arial-BoldMT"
#define FONT_DEFAULT            "TrebuchetMS-Bold"
#define DefaultColor  Color3B(75, 75, 75) //默认字体颜色
#define WATHETBLUE    Color3B(10, 224, 206)//浅蓝色

#define RoomCurrentLayer        100         //房间当前层tag值

#define WinSize Director::getInstance()->getVisibleSize()
#define JudgeScale WinSize.width/1136.f
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
typedef std::function<void(WORD, void *, WORD)> gameMessageCallback;//消息回调
typedef std::function<void(void *, WORD)> MessageCallback1;//用户服务消息
typedef std::function<void(void *, void *, WORD, bool)> gameMessageRecv;//接受消息
#define GLCOLOR_RGB(r,g,b)              ((COLORREF)(((0)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
//UM
#define UMKey               @"56dfc4d567e58ecbf2001ede"

//
//wx
#define WXKey               @"wx56b110375f495f83"
#define WXSecret            @"6b513229647af031180038742d5cbbcd"
#define WXURL               @"http://jd.foxuc.net"


//sina
#define kAppKey             @"34264003"
#define kAppSecret          @"48333fe8331979d83ebd2d38c1077038"
#define kRedirectURI        @"http://jd.foxuc.net"

enum ModeType
{
    Type_Wait = 0,//等待弹框
    Type_Wait_Text,//文字提示等待
    Type_Ensure,//确定弹框
    Type_Ensure_Cancel,//确定取消弹框
    Type_Info_Reminder,//消息提示弹框
    Type_Delete,//移除弹框
};

enum UserDate_ChangeType
{
    User_Change_Name = 0,
    User_Change_Head,
    User_Change_Level,
    User_Change_Score,
    User_Change_Bean,
};

//喇叭数据
class TrumpetData:public cocos2d::Ref
{
public:
    TrumpetData()
    :sendnickname("")
    ,sendtext("")
    {
        
    }
    
    std::string sendnickname;//发送者昵称
    std::string sendtext;//发送内容
};

//游戏编号
typedef enum
{
    kind_default = INVALID_DWORD,   //默认类型
    kind_niuniu = 27,               //牛牛类型
    kind_baijiale = 122,            //百家乐类型
    kind_lkpy = 510,                //李逵劈鱼
    kind_brnn = 104,                //百人牛牛
    kind_zjh = 6,                   //扎金花
    kind_tbnn = 36,                 //通比牛牛
}LIST_Kind;

//游戏元素数量
typedef enum
{
    element_landlord = 20,          //斗地主一方玩家最大牌
}LIST_GAMEELEMENT;

//下载状态
typedef enum
{
    DownLoadNone      = 0 ,
    DownLoading       = 1,
    DownLoadDone      = 2,
    DownLoadCancel    = 3,
    DownLoadFailure   = 4 
    
}DownLoadStatus;


typedef struct
{
    LIST_Kind   eKind;  //游戏类型
    
    DOUBLE      dPercent; //下载进度
    
    
}DownRefresh;

//下载信息
typedef struct
{
    
    LIST_Kind       kindID;
    DownLoadStatus  status;
    
}DownLoadInfo;

//赠送信息
struct tagBankSendInfo
{
    int  nSendType; //0:id;1:昵称
    std::string sReceiveUserName;
    DWORD dwReceiveUserId;
    SCORE llSendCount;
};

#define BANK   1000
#define SHOP   2000
#define TASK   3000
#define RANK   4000
#define PERSONAL 5000
#endif
