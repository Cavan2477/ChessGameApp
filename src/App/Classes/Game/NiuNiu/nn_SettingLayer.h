/************************************************************************************
 * file: 		nn_SettingLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:39:34
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __NN_SETTING_LAYER_HPP__
#define __NN_SETTING_LAYER_HPP__

#include <stdio.h>
#include "CocosHeader.h"
#include "../../LibExt/ButtonGroup.h"

NN_NAMESPACE_BEGIN

class SettingLayer : public UILayer, public ButtonGroupListen
{
public:
    SettingLayer();
    ~SettingLayer();
    
    CREATE_FUNC(SettingLayer);

    virtual bool init() override;
    virtual bool preButtonChecked(cocos2d::Node *pSender,bool bSelected) override;
    virtual void onButtonChecked(cocos2d::Node *pSender) override;
    virtual void onCheckStatuChange(cocos2d::Node *pSender,bool bSelected) override;
    
    CC_SYNTHESIZE(UILayer*, m_notify, Notify);
    void showLayer(bool bShow);

private:
    void checkEvent(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type);
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    void enableSlider(cocos2d::ui::Slider *slider, bool bEnable);

private:
    cocos2d::ui::Slider *m_sliderEffect;
    cocos2d::ui::Slider *m_sliderMusic;
    cocos2d::ui::CheckBox *m_checkOpenShake;
    cocos2d::ui::CheckBox *m_checkCloseShake;
    cocos2d::ui::CheckBox *m_checkAllClose;

    ButtonGroup *m_group;

    bool m_bInit;
};

NN_NAMESPACE_END

#endif /* SettingLayer_hpp */
