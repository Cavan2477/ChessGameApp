//
//  SettingLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/11.
//
//

#include "SettingLayer.h"
#include "Project.h"
#include "HallDataMgr.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace CocosDenshion;
using namespace brnn_game;

enum ButtonTag
{
    Tag_Bg = 0,
    Tag_BT_Musicplus,
    Tag_BT_Musicminus,
    Tag_BT_Effectplus,
    Tag_BT_Effectminus,
    
    Tag_Sl_Music,
    Tag_Sl_Effect,
    
    Tag_Shake_Switch,
    Tag_Sound_Switch,
    Tag_Switch_Sl,//滑动导致switchsound 响应事件
};

bool SettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    cocos2d::Size viewsize = WinSize;
    
    auto pcolorlayer = LayerColor::create(Color4B(0, 0, 0, 110));
    this->addChild(pcolorlayer);
    
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->setPosition(viewsize/2);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            this->removeFromParent();
        }
    });
    this->addChild(pbuttonbg);
    
    auto set_bg = Button::create("setting_res/im_frame.png", "setting_res/im_frame.png");
    set_bg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    set_bg->setTag(Tag_Bg);
    set_bg->setScale(0.5f);
    set_bg->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.11f, 1.1f), ScaleTo::create(0.11f, 1.f)));
    this->addChild(set_bg);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(cocos2d::Point(set_bg->getContentSize().width-45, set_bg->getContentSize().height-45));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            this->removeFromParent();
        }
    });
    set_bg->addChild(pclose);
    
    //音乐
    auto txt_music = ImageView::create("setting_res/txt_music.png");
    txt_music->setPosition(Vec2(200, set_bg->getContentSize().height-68));
    set_bg->addChild(txt_music);
    
    m_bgmusic = Slider::create();
    m_bgmusic->loadBarTexture("setting_res/slider_bg.png");
    m_bgmusic->loadSlidBallTextures("setting_res/slider_thumb.png", "setting_res/slider_thumb.png", "");
    m_bgmusic->loadProgressBarTexture("setting_res/slider_progress.png");
    m_bgmusic->setPosition(Vec2(set_bg->getContentSize().width/2, set_bg->getContentSize().height-118));
    m_bgmusic->addEventListener(CC_CALLBACK_2(SettingLayer::sliderEvent, this));
    m_bgmusic->setTag(Tag_Sl_Music);
    m_bgmusic->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
    set_bg->addChild(m_bgmusic);
    
    auto music_plus = Button::create("setting_res/bt_plus.png");
    music_plus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    music_plus->setTag(Tag_BT_Musicplus);
    music_plus->setPosition(Vec2(set_bg->getContentSize().width/2+290, m_bgmusic->getPositionY()));
    set_bg->addChild(music_plus);
    
    auto music_minus = Button::create("setting_res/bt_minus.png");
    music_minus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    music_minus->setTag(Tag_BT_Musicminus);
    music_minus->setPosition(Vec2(set_bg->getContentSize().width/2-290, m_bgmusic->getPositionY()));
    set_bg->addChild(music_minus);
    
    //音效
    auto txt_effect = ImageView::create("setting_res/txt_effect.png");
    txt_effect->setPosition(Vec2(200, set_bg->getContentSize().height-172));
    set_bg->addChild(txt_effect);
    
    m_effect = Slider::create();
    m_effect->loadBarTexture("setting_res/slider_bg.png");
    m_effect->loadSlidBallTextures("setting_res/slider_thumb.png", "setting_res/slider_thumb.png", "");
    m_effect->loadProgressBarTexture("setting_res/slider_progress.png");
    m_effect->setPosition(Vec2(set_bg->getContentSize().width/2, set_bg->getContentSize().height-226));
    m_effect->addEventListener(CC_CALLBACK_2(SettingLayer::sliderEvent, this));
    m_effect->setTag(Tag_Sl_Effect);
    m_effect->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
    set_bg->addChild(m_effect);
    
    auto effect_plus = Button::create("setting_res/bt_plus.png");
    effect_plus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    effect_plus->setTag(Tag_BT_Effectplus);
    effect_plus->setPosition(Vec2(set_bg->getContentSize().width/2+290, m_effect->getPositionY()));
    set_bg->addChild(effect_plus);
    
    auto effect_minus = Button::create("setting_res/bt_minus.png");
    effect_minus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    effect_minus->setTag(Tag_BT_Effectminus);
    effect_minus->setPosition(Vec2(set_bg->getContentSize().width/2-290, m_effect->getPositionY()));
    set_bg->addChild(effect_minus);
    
    //声音
    auto txt_sound = ImageView::create("setting_res/txt_sound.png");
    txt_sound->setPosition(Vec2(422, set_bg->getContentSize().height-306));
    set_bg->addChild(txt_sound);
    
    m_sound = Button::create("setting_res/switch_mask.png");
    m_sound->setPosition(Vec2(320, txt_sound->getPositionY()));
    m_sound->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    m_sound->setTag(Tag_Sound_Switch);
    set_bg->addChild(m_sound);
    
    auto pdot = ImageView::create("setting_res/slider_thumb.png");
    pdot->setPosition(m_sound->getContentSize()/2);
    pdot->setTag(10);
    m_sound->addChild(pdot);
    pdot->setVisible(HallDataMgr::getInstance()->m_bOptionMute);
    return true;
}

void SettingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void SettingLayer::onExit()
{
    HallDataMgr::getInstance()->saveConfig();
    Layer::onExit();
}

void SettingLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
   if (type == Widget::TouchEventType::ENDED)
   {
       auto pbutton = static_cast<Button *>(pSender);
       switch (pbutton->getTag()) {
           case Tag_Sound_Switch:
           {
               HallDataMgr::getInstance()->m_bOptionMute = !HallDataMgr::getInstance()->m_bOptionMute;
               if (!HallDataMgr::getInstance()->m_bOptionMute) {
                   m_bgmusic->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
                   m_effect->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
                   SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_bgmusic->getPercent()/100.f);
                   SimpleAudioEngine::getInstance()->setEffectsVolume(m_effect->getPercent()/100.f);
               }
               else
               {
                   m_bgmusic->setPercent(0);
                   m_effect->setPercent(0);
                   SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.f);
                   SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);
               }
               m_sound->getChildByTag(10)->setVisible(HallDataMgr::getInstance()->m_bOptionMute);
           }
               break;
           case Tag_BT_Musicminus:
           {
               m_bgmusic->setPercent(m_bgmusic->getPercent()-10);
               HallDataMgr::getInstance()->m_fOptionMusic = m_bgmusic->getPercent();
           }
               break;
           case Tag_BT_Musicplus:
           {
               m_bgmusic->setPercent(m_bgmusic->getPercent()+10);
               HallDataMgr::getInstance()->m_fOptionMusic = m_bgmusic->getPercent();
               
               HallDataMgr::getInstance()->m_bOptionMute = false;
               m_sound->getChildByTag(10)->setVisible(HallDataMgr::getInstance()->m_bOptionMute);
           }
               break;
           case Tag_BT_Effectplus:
           {
               m_effect->setPercent(m_effect->getPercent()+10);
               HallDataMgr::getInstance()->m_fOptionMusic = m_effect->getPercent();
               
               HallDataMgr::getInstance()->m_bOptionMute = false;
               m_sound->getChildByTag(10)->setVisible(HallDataMgr::getInstance()->m_bOptionMute);
           }
               break;
           case Tag_BT_Effectminus:
           {
               m_effect->setPercent(m_effect->getPercent()-10);
               HallDataMgr::getInstance()->m_fOptionMusic = m_effect->getPercent();
           }
               break;
               
           default:
               break;
       }
   }
}

void SettingLayer::sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
    auto button = (Slider *)pSender;
    auto percent = button->getPercent();
    auto percent1 = percent/100.f;
    if (button->getTag() == Tag_Sl_Effect) {
        HallDataMgr::getInstance()->m_fOptionSound = percent;
        SimpleAudioEngine::getInstance()->setEffectsVolume(percent1);
    }
    else
    {
        HallDataMgr::getInstance()->m_fOptionMusic = percent;
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent1);
    }
    
    if (percent > 0) {
        HallDataMgr::getInstance()->m_bOptionMute = false;
        m_sound->getChildByTag(10)->setVisible(HallDataMgr::getInstance()->m_bOptionMute);
    }
}