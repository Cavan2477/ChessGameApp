//
//  BankLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//银行界面

#ifndef __brnn_MyGame__BankLayer__
#define __brnn_MyGame__BankLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CMD_Stdafx.h"

namespace brnn_game {
    

class BankLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    
    virtual bool init();
    
    void typeinit();
    
    void openinit();
    
    void accessinit();
    
    void presentedinit();
    
    //游戏中银行
    static BankLayer * createWithGame();
    //游戏中银行初始化
    bool bankgameinit();
    //
    void gameopeninit();
    
    void gameaccessinit();
    
    //存取按钮更新
    void updategamesave(bool enable);
    
    //刷新用户金币数显示
    void updateScore();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    bool checkStrlength(const std::string &str, const std::string &title);
    
    //发送开通银行
    void sendInsureEnable(const std::string &pass);
    //查询银行
    void sendInsureInfo();
    //存款
    void sendSaveScore(SCORE score);
    //取款
    void sendTakeScore(SCORE score, const std::string &pass);
    //转帐
    void sendTransferScore(SCORE score, const std::string &pass, int type, const std::string &nickname);
    
    CREATE_FUNC(BankLayer)
    
    //大厅消息回包处理
    //开通银行结果
    void InsureEnableResult(void* pData, WORD wSize);
    //银行资料
    void InsureInfoResult(void* pData, WORD wSize);
    //操作成功
    void BankSuccedResult(void* pData, WORD wSize);
    //操作失败
    void BankFailureResult(void* pData, WORD wSize);
    //用户信息
    void BankUserInfoResult(void* pData, WORD wSize);
    
    //游戏消息回包
    void BankInsureResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    
private:
    int     m_nSendType;		//发送模式	0 id 1昵称
    
    SCORE   m_TransferScore;    //转帐金额
    
    std::string m_BankPass;     //转帐输入密码
    
    CMD_GP_UserInsureInfo  m_info;
    
    cocos2d::ui::Widget *m_frame;//弹框
    
    cocos2d::ui::Button *m_save;//游戏币存取
    
    cocos2d::ui::Button *m_send;//游戏币赠送
};

}
#endif /* defined(__MyGame__BankLayer__) */
