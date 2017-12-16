//
//  BankLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#ifndef bjl_header_BankLayer_hpp
#define bjl_header_BankLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

#include "OpenBankLayer.h"
#include "BankSaveLayer.h"
#include "BankSendLayer.h"
#include "ButtonGroup.h"

BJL_NAMESPACE_BEGIN

class BankLayer:public UILayer,
public ButtonGroupListen
{
public:
    BankLayer();
    ~BankLayer();
    
    CREATE_FUNC(BankLayer);
    virtual bool init() override;
    //ui 线程消息(不能在非ui线程内调用)
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    virtual bool preButtonChecked(cocos2d::Node *pSender,bool bSelected) override;
    virtual void onButtonChecked(cocos2d::Node *pSender) override;
    virtual void onCheckStatuChange(cocos2d::Node *pSender,bool bSelected) override;
    /*
     * @brief 银行层加载形式(创建银行场景、直接加载银行层)
     */
    CC_SYNTHESIZE(bool, m_bSceneMode, SceneMode);
    
    void showLayer(bool bShow);
public:
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
    
    //
    void updateScore();
    
    void openBankSuccess();
    
    void onGetBankInsureInfo(void *pData, WORD wSize);
    
    void onGetBankUserInfo(const std::string &nickname);
private://网络
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
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    bool handleBankUIMsg(const int &sub,void *pData, const int &nLen);
    
    bool checkStrlength(const std::string &str, const std::string &title);
    
    //开通
    void showOpenBank(bool bShow);
    
    //存取
    void showBankSave(bool bShow);
    
    //赠送
    void showBankSend(bool bShow);
private:
    cocos2d::ui::Layout *m_btnLayout;
    cocos2d::ui::CheckBox *m_checkSave; //存取款
    cocos2d::ui::CheckBox *m_checkSend; //赠送
    ButtonGroup *m_group;
    
    OpenBankLayer *m_openBankLayer;
    BankSaveLayer *m_bankSaveLayer;
    BankSendLayer *m_bankSendLayer;
private:
    SCORE   m_TransferScore;    //转帐金额
    std::string m_BankPass;     //转帐输入密码
    CMD_GP_UserInsureInfo  m_info;
};

BJL_NAMESPACE_END
#endif /* BankLayer_hpp */
