/************************************************************************************
 * file: 		ModeLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/17 17:40:43
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/17			
 *
 ************************************************************************************/

//模态层
#ifndef __MODELAYER_H__
#define __MODELAYER_H__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../Common/PublicDefine.h"

USING_NS_CC;

using namespace ui;

class ModeLayer : public Layer
{
public:
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void touchEvent(Ref *pRefSender, Widget::TouchEventType touchEventType);
    
    CREATE_FUNC(ModeLayer);
    
	static ModeLayer* createBytype(const std::string& strTitleText, const std::string &strContentText, EM_MODE_TYPE emModeType, bool bClose = false);
    
    void TextInit(const std::string &strTitle, const std::string &strContentText);
    
    void WaitInit();
    
    void WaitTextInit(const std::string &strContentText ,bool bClose = false);
    
    void EnsureInit();
    
    void EnsuercancelInit();
    
    void RemindInit(const std::string &strContentText);
    
    void RemindCallback();
    
    void setEnsureCallback(const std::function<void()> &function);
    
    void setCancelCallback(const std::function<void()> &function);

private:
    EventListenerTouchOneByOne *m_Listener;//触摸监听
    
    Button *m_ensure;
    
    Button *m_cancel;
    
    Button *m_toubutton;
    
    ImageView *m_bg;
    
	EM_MODE_TYPE m_emModeType;
    
    std::function<void()> EnsureCallback;//确定回调函数
    
    std::function<void()> CancelCallback;//取消回调函数
};

#endif /* defined(__Cheyou__ModeLayer__) */
