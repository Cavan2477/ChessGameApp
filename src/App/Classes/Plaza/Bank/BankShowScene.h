/************************************************************************************
 * file: 		BankShow.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:16:00
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __BankShowScene_h__
#define __BankShowScene_h__

#include "BankScene.h"
#include "../../Common/CMD_Stdafx.h"
#include "network/HttpClient.h"

class BankRecord :public cocos2d::Ref
{
public:
    //交易时间
    CC_SYNTHESIZE(std::string, m_Date, Date)
    
    //交易类别
    CC_SYNTHESIZE(std::string, m_Type, Type)
    
    //交易金额
    CC_SYNTHESIZE(SCORE, m_SwapScore, SwapScore)
    
    //服务费
    CC_SYNTHESIZE(SCORE, m_Revenue, Revenue)
};

class BankShowScene : public BankScene , public cocos2d::ui::EditBoxDelegate
{
    
public:
    BankShowScene();
    virtual ~BankShowScene();
    
    CREATE_FUNC(BankShowScene);
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
public:
    
    void initApply();
    void initSaveTake();
    void initPresent();
    void initRecord();
    void updateScore();
    int getBankType() const;
    void showRecord();
    bool checkSpaceStr(const std::string str) const;
public:
    //大厅消息回包处理
    //开通银行结果
    virtual void InsureEnableResult(void* pData, WORD wSize);
    //银行资料
    virtual void InsureInfoResult(void* pData, WORD wSize);
    //操作成功
    virtual void BankSuccedResult(void* pData, WORD wSize);
    //操作失败
    virtual void BankFailureResult(void* pData, WORD wSize);
    //用户信息
    virtual void BankUserInfoResult(void* pData, WORD wSize);
    
    //发送请求用户信息
    void queryUserInfo(const BYTE &cbType, const std::string &sTarget);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    
    void buttonEventWithTouch(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
    void buttonEventWithSendType(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
    void buttonEventWithRecord(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
    void buttonEventWithPresent(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
    void RecoreRequestCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void RecordReuqcestEvent(cocos2d::EventCustom *event);
    
public:
    void popBank();
    
public:
    enum BankType
    {
        type_apply,             //开通银行
        type_savetake,          //存取款
        type_present,           //赠送
        type_record             //操作记录
        
    };

private:
    int     m_nSendType;		//发送模式	0 id  1昵称
    
    CMD_GP_USER_INSURE_INFO  m_sInfo;
    
    cocos2d::ui::Layout  *               _layout;
    cocos2d::ui::Layout  *               _applyLayout;
    cocos2d::ui::Layout  *               _saveLayout;
    cocos2d::ui::Layout  *               _presentLayout;
    cocos2d::ui::Layout  *               _recordLayout;
    
    BankType                            m_eType;
    
    CC_SYNTHESIZE_RETAIN(Label *, _userScoreLB, UserScoreLB);
    CC_SYNTHESIZE_RETAIN(Label *, _userInsureScoreLB, UserInsureScoreLB);
    
    std::vector<BankRecord *>   m_Datalist;
    
    
};

#endif /* BankShow_h*/
