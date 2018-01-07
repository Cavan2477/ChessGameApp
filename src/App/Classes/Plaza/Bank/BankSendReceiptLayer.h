/************************************************************************************
 * file: 		BankSendReceiptLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:18:17
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __BANK_SEND_RECEIPT_LAYER_H__
#define __BANK_SEND_RECEIPT_LAYER_H__

#include <stdio.h>
#include "CocosHeader.h"

typedef struct _stReceipt
{
    DWORD		dwRecordIndex;				//记录编号
    std::string strSendUserName;			//赠送人昵称
    DWORD		dwSendUserId;				//赠送人id
    std::string strRecUserName;				//接收人昵称
    DWORD		dwRecUserId;				//接收人id
    LONGLONG	llSendCount;				//赠送数额
}ST_RECEIPT, *PST_RECEIPT;

class BankSendReceiptLayer: public UILayer
{
public:
    BankSendReceiptLayer();
    ~BankSendReceiptLayer();
    
    CREATE_FUNC(BankSendReceiptLayer);

    virtual bool init() override;
    
    void refreshReceipt(const _stReceipt &tagRec);
    
private:
    std::string numberTransiform(const std::string &strCount);
    
    std::string replaceAll(std::string &src, const std::string &regex, const std::string &replacement);
    
    std::string cleanZero(std::string &s);

private:
    cocos2d::Node *m_root;
    
    //编号
    cocos2d::ui::Text *m_textCode;

    //赠送人昵称
    cocos2d::ui::Text *m_textSendUserName;

    //赠送人id
    cocos2d::ui::Text *m_textSendUserId;

    //接收人昵称
    cocos2d::ui::Text *m_textRecUserName;

    //接收人id
    cocos2d::ui::Text *m_textRecUserId;

    //赠送游戏币
    cocos2d::ui::Text *m_textSendCount;

    //数额大写
    cocos2d::ui::Text *m_textDaXie;

    //日期
    cocos2d::ui::Text *m_textSendDate;
    
    //赠送结果
    cocos2d::Sprite *m_spSendRes;
};

#endif /* BankSendReceiptLayer_hpp */
