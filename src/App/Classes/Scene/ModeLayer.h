/************************************************************************************
 * file: 		ModeLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:49:10
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

//模态层

#ifndef __ModeLayer_h__
#define __ModeLayer_h__

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
    
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    

    CREATE_FUNC(ModeLayer);
    
    static ModeLayer* createBytype(const std::string& titletext,  const std::string &contenttext, ModeType type,bool close = false);
    
    void TextInit(const std::string &title, const std::string &content);
    
    void WaitInit();
    
    void WaitTextInit(const std::string &content ,bool close = false);
    
    void EnsureInit();
    
    void EnsuercancelInit();
    
    void RemindInit(const std::string &content);
    
    void RemindCallback();
    
    void setEnsureCallback(const std::function<void()> &function);
    
    void setCancelCallback(const std::function<void()> &function);
private:
    EventListenerTouchOneByOne *m_Listener;//触摸监听
    
    Button *m_ensure;
    
    Button *m_cancel;
    
    Button *m_toubutton;
    
    ImageView *m_bg;
    
    ModeType m_type;
    
    std::function<void()> EnsureCallback;//确定回调函数
    
    std::function<void()> CancelCallback;//取消回调函数
};

#endif /* defined(__Cheyou__ModeLayer__) */
