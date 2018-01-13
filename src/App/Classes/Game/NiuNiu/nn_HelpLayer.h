/************************************************************************************
 * file: 		nn_HelpLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:52:08
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __NN_HELP_LAYER_HPP__
#define __NN_HELP_LAYER_HPP__

#include <stdio.h>
#include "CocosHeader.h"

NN_NAMESPACE_BEGIN

class HelpLayer:public UILayer
{
public:
    HelpLayer();
    ~HelpLayer();
    
    static cocos2d::Scene* createScene();

    CREATE_FUNC(HelpLayer);

    virtual bool init() override;
    
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

NN_NAMESPACE_END

#endif /* HelpLayer_hpp */
