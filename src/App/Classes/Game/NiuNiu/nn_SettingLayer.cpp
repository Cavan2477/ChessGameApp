//
//  SettingLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/24/15.
//
//

#include "nn_SettingLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
USING_NN_NAMESPACE;

enum ButtonTag
{
    Tag_Sl_Music,
    Tag_Sl_Effect,
};

SettingLayer::SettingLayer():
m_notify(nullptr)
{
    m_group = new ButtonGroup();
    m_group->setGroupListen(this);
    m_group->setGroupName("shake_group");
}

SettingLayer::~SettingLayer()
{
    m_group->removeFromGroup("shakeclose_check");
    m_group->removeFromGroup("shakeopen_check");
    delete m_group;
    m_group = nullptr;
}

bool SettingLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("setting/SettingLayer.csb");
        m_bInit = true;
        
        m_sliderEffect = static_cast<Slider*>(m_root->getChildByName("effect_slider"));
        CC_ASSERT(m_sliderEffect != nullptr);
        m_sliderEffect->setTag(Tag_Sl_Effect);
        m_sliderEffect->addEventListener(CC_CALLBACK_2(SettingLayer::sliderEvent, this));
        float sound = HallDataMgr::getInstance()->m_fOptionSound;
        m_sliderEffect->setPercent(sound);
        
        m_sliderMusic = static_cast<Slider*>(m_root->getChildByName("music_slider"));
        CC_ASSERT(m_sliderMusic != nullptr);
        m_sliderMusic->setTag(Tag_Sl_Music);
        m_sliderMusic->addEventListener(CC_CALLBACK_2(SettingLayer::sliderEvent, this));
        m_sliderMusic->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
        /*
        m_checkCloseShake = static_cast<CheckBox*>(m_root->getChildByName("shakeclose_check"));
        CC_ASSERT(m_checkCloseShake != nullptr);
        m_group->addToGroup(m_checkCloseShake, "shakeclose_check");
        
        m_checkOpenShake = static_cast<CheckBox*>(m_root->getChildByName("shakeopen_check"));
        CC_ASSERT(m_checkOpenShake != nullptr);
        m_group->addToGroup(m_checkOpenShake, "shakeopen_check");
        
        bool openShake = HallDataMgr::getInstance()->m_bOptionShake;
        if (openShake == false)
        {
            m_group->setSelecte("shakeclose_check");
        }
        else
        {
            m_group->setSelecte("shakeopen_check");
        }
        */
        m_checkAllClose = static_cast<CheckBox*>(m_root->getChildByName("allclose_check"));
        CC_ASSERT(m_checkAllClose != nullptr);
        m_checkAllClose->addEventListener((CC_CALLBACK_2(SettingLayer::checkEvent, this)));
        bool mute = HallDataMgr::getInstance()->m_bOptionMute;
        m_checkAllClose->setSelected(mute);
        enableSlider(m_sliderMusic, !mute);
        enableSlider(m_sliderEffect, !mute);
        
        auto csbBtn = static_cast<Button*>(m_root->getChildByName("close_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener([this](Ref *pSender, Widget::TouchEventType tType)
                                      {
                                          if (pSender == nullptr)
                                          {
                                              return;
                                          }
                                          if (Widget::TouchEventType::ENDED == tType)
                                          {
                                              INSTANCE(AudioMgr)->playClickAudio();
                                              this->showLayer(false);
                                          }                                          
                                      });
        bRes = true;
    } while (false);
    return bRes;
}

bool SettingLayer::preButtonChecked(cocos2d::Node *pSender, bool bSelected)
{
    return true;
}

void SettingLayer::onButtonChecked(cocos2d::Node *pSender)
{
    std::string btnname = pSender->getName();
    log("on: check :%s,select",btnname.c_str());
    
    if (m_bInit)
    {
        m_bInit = false;
        return;
    }
    INSTANCE(MultiPlatformMgr)->switchVibratorState(HallDataMgr::getInstance()->m_bOptionShake);
}

void SettingLayer::onCheckStatuChange(cocos2d::Node *pSender, bool bSelected)
{
    std::string btnname = pSender->getName();
    log("change: check :%s,select",btnname.c_str());
    
    if (0 == strcmp(btnname.c_str(), "shakeopen_check"))
    {
        if (bSelected)
        {
            HallDataMgr::getInstance()->m_bOptionShake = true;
        }
    }
    else if (0 == strcmp(btnname.c_str(), "shakeclose_check"))
    {
        if (bSelected)
        {
            HallDataMgr::getInstance()->m_bOptionShake = false;
        }
    }
}

void SettingLayer::showLayer(bool bShow)
{
    this->setVisible(bShow);
    if (bShow)
    {
        
    }
    else
    {
        if (m_notify)
        {
            m_notify->uiMsg(UI_MSG_BASE::UI_MSG_PLAZA, PLAZA_UI::REMOVE_TOP_LAYER, nullptr, 0);
        }
        HallDataMgr::getInstance()->saveConfig();
    }
}

void SettingLayer::checkEvent(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
    if (nullptr == pSender)
    {
        return;
    }
    CheckBox *pBtn = static_cast<CheckBox*>(pSender);
    std::string btnname = pBtn->getName();
    
    
    bool bCheck = type == CheckBox::EventType::SELECTED;

    HallDataMgr::getInstance()->m_bOptionMute = bCheck;
    if (bCheck)
    {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    else
    {
        float per = HallDataMgr::getInstance()->m_fOptionSound / 100.0f;
        SimpleAudioEngine::getInstance()->setEffectsVolume(per);
        
        per = HallDataMgr::getInstance()->m_fOptionMusic / 100.0f;
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(per);
    }
    enableSlider(m_sliderMusic, !bCheck);
    enableSlider(m_sliderEffect, !bCheck);
}

void SettingLayer::sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
    auto button = (Slider *)pSender;
    auto percent = button->getPercent();
    auto percent1 = percent/100.f;
    if (button->getTag() == Tag_Sl_Effect)
    {
        HallDataMgr::getInstance()->m_fOptionSound = percent;
        SimpleAudioEngine::getInstance()->setEffectsVolume(percent1);
    }
    else
    {
        HallDataMgr::getInstance()->m_fOptionMusic = percent;
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent1);
    }
    HallDataMgr::getInstance()->saveConfig();
}

void SettingLayer::enableSlider(cocos2d::ui::Slider *slider, bool bEnable)
{
    slider->setEnabled(bEnable);
    if (bEnable)
    {
        slider->setOpacity(255);
    }
    else
    {
        slider->setOpacity(125);
    }
}