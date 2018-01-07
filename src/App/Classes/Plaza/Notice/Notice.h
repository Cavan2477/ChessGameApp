/************************************************************************************
 * file: 		Notice.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:26:40
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __NOTICE_H__
#define __NOTICE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "../../Common/MacroDefine.h"
#include "../../Common/Project.h"

USING_NS_CC;

using namespace cocos2d::network;

class Notice : public ClippingRectangleNode
{
public:
    Notice();
    virtual ~Notice();
    static Notice* CreateNotice();
    
    bool   initNotice();

    void NoticeReuqcestEvent(cocos2d::EventCustom *event);
    void RequestNoticeContent();
    void NoticeRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void runAction();

private:
    CC_SYNTHESIZE_RETAIN(Label *, _noticeText, NoticeText);

    bool m_bGetNotice;
};

#endif //__NOTICE_H__
