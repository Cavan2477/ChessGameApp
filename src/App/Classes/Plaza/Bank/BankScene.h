/************************************************************************************
 * file: 		BankScene.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:18:57
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __BankScene_h__
#define __BankScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../Common/MacroDefine.h"

USING_NS_CC;

class BankScene : public Layer
{
public:
    CREATE_FUNC(BankScene);
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();

public:
    //发送开通银行
    void sendInsureEnable(const std::string &pass);

    //查询银行
    void sendInsureInfo();

    //存款
    void sendSaveScore(LONG_LONG score);

    //取款
    void sendTakeScore(LONG_LONG score, const std::string &pass);

    //转帐
    void sendTransferScore(LONG_LONG score, const std::string &pass, int type, const std::string &nickname);
    
    //大厅消息回包处理
    //开通银行结果
    virtual void InsureEnableResult(void* pData, WORD wSize){};

    //银行资料
    virtual void InsureInfoResult(void* pData, WORD wSize){};

    //操作成功
    virtual void BankSuccedResult(void* pData, WORD wSize){};

    //操作失败
    virtual void BankFailureResult(void* pData, WORD wSize){};

    //用户信息
    virtual void BankUserInfoResult(void* pData, WORD wSize){};
    
    //游戏消息回包
    virtual void BankInsureResult(WORD  wSubCmdID, void* pData, WORD wSize){};
    
protected:
    LONG_LONG   m_TransferScore;    //转帐金额
    
    std::string m_BankPass;     //转帐输入密码
};

#endif /* Bank_h */
