#include "zjh_GameSettingLayer.h"
#include "Project.h"
#include "HallDataMgr.h"
#include "SimpleAudioEngine.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
USING_ZJH_NAMESPACE;
#define DX -10
#define DY 25
enum ButtonTag
{
    Tag_Bg = 0,
    Tag_BT_Musicplus,
    Tag_BT_Musicminus,
    Tag_BT_Effectplus,
    Tag_BT_Effectminus,
    
    Tag_Sl_Music,
    Tag_Sl_Effect,
    
    Tag_Shack_Close,
    Tag_Shack_Open,
    Tag_Sound_Off,
};

bool SettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    cocos2d::Size viewsize = WinSize;
    //    auto pbg = Button::create("login_res/method_bg.png","login_res/method_bg.png");
    //    pbg->setPosition(cocos2d::Point(viewsize.width/2, viewsize.height/2));
    //    this->addChild(pbg);
    
    set_bg = ImageView::create("JDGswz/common_res/common_backGround.png");
    Size bgSize = set_bg->getContentSize();
    set_bg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    set_bg->setTag(Tag_Bg);
    this->addChild(set_bg);
    //set_bg->setOpacity(0);
    
    
    //音乐
    auto txt_music = ImageView::create("JDGswz/setting_res/txt_music.png");
    txt_music->setPosition(Vec2(180 + DX, set_bg->getContentSize().height-88+ DY));
    set_bg->addChild(txt_music);
    
    auto music_slider = Slider::create();
    music_slider->loadBarTexture("JDGswz/setting_res/slider_bg.png");
    music_slider->loadSlidBallTextures("JDGswz/setting_res/slider_thumb.png", "JDGswz/setting_res/slider_thumb.png", "");
    music_slider->loadProgressBarTexture("JDGswz/setting_res/slider_progress.png");
    music_slider->setPosition(Vec2(set_bg->getContentSize().width/2+ DX +10, set_bg->getContentSize().height-136+ DY));
    music_slider->addEventListener(CC_CALLBACK_2(SettingLayer::sliderEvent, this));
    music_slider->setTag(Tag_Sl_Music);
    music_slider->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
    set_bg->addChild(music_slider);
    
    auto music_plus = Button::create("JDGswz/setting_res/bt_plus.png");
    music_plus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    music_plus->setTag(Tag_BT_Musicplus);
    music_plus->setPosition(Vec2(635+ DX, music_slider->getPositionY()));
    set_bg->addChild(music_plus);
    
    auto music_minus = Button::create("JDGswz/setting_res/bt_minus.png");
    music_minus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    music_minus->setTag(Tag_BT_Musicminus);
    music_minus->setPosition(Vec2(64+ DX-7, music_slider->getPositionY()));
    set_bg->addChild(music_minus);
    
    //音效
    auto txt_effect = ImageView::create("JDGswz/setting_res/txt_effect.png");
    txt_effect->setPosition(Vec2(180+ DX, set_bg->getContentSize().height-190+ DY));
    set_bg->addChild(txt_effect);
    
    auto effect_slider = Slider::create();
    effect_slider->loadBarTexture("JDGswz/setting_res/slider_bg.png");
    effect_slider->loadSlidBallTextures("JDGswz/setting_res/slider_thumb.png", "JDGswz/setting_res/slider_thumb.png", "");
    effect_slider->loadProgressBarTexture("JDGswz/setting_res/slider_progress.png");
    effect_slider->setPosition(Vec2(set_bg->getContentSize().width/2+ DX +10, set_bg->getContentSize().height-238+ DY));
    effect_slider->addEventListener(CC_CALLBACK_2(SettingLayer::sliderEvent, this));
    effect_slider->setTag(Tag_Sl_Effect);
    effect_slider->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
    set_bg->addChild(effect_slider);
    
    auto effect_plus = Button::create("JDGswz/setting_res/bt_plus.png");
    effect_plus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    effect_plus->setTag(Tag_BT_Effectplus);
    effect_plus->setPosition(Vec2(635+ DX, effect_slider->getPositionY()));
    set_bg->addChild(effect_plus);
    
    auto effect_minus = Button::create("JDGswz/setting_res/bt_minus.png");
    effect_minus->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    effect_minus->setTag(Tag_BT_Effectminus);
    effect_minus->setPosition(Vec2(64+ DX-7, effect_slider->getPositionY()));
    set_bg->addChild(effect_minus);
    
    //全部静音
    auto sound_off = Button::create();
    HallDataMgr::getInstance()->m_bOptionMute?sound_off->loadTextures("JDGswz/setting_res/bt_check_yes.png", "JDGswz/setting_res/bt_check_no.png"):sound_off->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
    sound_off->setPosition(Vec2(405 ,set_bg->getContentSize().height-326 + DY));
    sound_off->setTag(Tag_Sound_Off);
    sound_off->addTouchEventListener(CC_CALLBACK_2(SettingLayer::touchEvent, this));
    set_bg->addChild(sound_off);
    if (HallDataMgr::getInstance()->m_bOptionMute) {
        music_slider->setPercent(0);
        effect_slider->setPercent(0);
    }
    
    auto txt_sound_off = ImageView::create("JDGswz/setting_res/txt_sound_off.png");
    txt_sound_off->setPosition(Vec2(480, set_bg->getContentSize().height-326 + DY));
    txt_sound_off->setAnchorPoint(Vec2(0.f, 0.5f));
    set_bg->addChild(txt_sound_off);
    
    return true;
}

bool SettingLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    Point pos = touch->getLocation();
    return true;
}

void SettingLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    Point pos = touch->getLocation();
    
    if(!set_bg->boundingBox().containsPoint(pos))
    {
        this->removeFromParentAndCleanup(true);
        
        //        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([=]{
        //            this->removeFromParentAndCleanup(true);
        //        })));
        
    }
}

void SettingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void SettingLayer::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(SettingLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(SettingLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void SettingLayer::onExit()
{
    HallDataMgr::getInstance()->saveConfig();
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    Layer::onExit();
}

void SettingLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            auto pbg = this->getChildByTag(Tag_Bg);
            switch (tag ) {
                case Tag_BT_Musicminus:
                {
                    auto music_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Music);
                    music_slider->setPercent(music_slider->getPercent()-10);
                    HallDataMgr::getInstance()->m_fOptionMusic = music_slider->getPercent();
                    
                    float percent = music_slider->getPercent();
                    float percent1 = percent/100.f;
                    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent1);
                }
                    break;
                case Tag_BT_Musicplus:
                {
                    auto music_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Music);
                    music_slider->setPercent(music_slider->getPercent()+10);
                    HallDataMgr::getInstance()->m_fOptionMusic = music_slider->getPercent();
                    
                    auto sound_off = (Button *)pbg->getChildByTag(Tag_Sound_Off);
                    sound_off->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionMute = false;
                    
                    float percent = music_slider->getPercent();
                    float percent1 = percent/100.f;
                    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent1);
                }
                    break;
                case Tag_BT_Effectplus:
                {
                    auto effect_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Effect);
                    effect_slider->setPercent(effect_slider->getPercent()+10);
                    HallDataMgr::getInstance()->m_fOptionSound = effect_slider->getPercent();
                    
                    auto sound_off = (Button *)pbg->getChildByTag(Tag_Sound_Off);
                    sound_off->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionMute = false;
                    
                    float percent = effect_slider->getPercent();
                    float percent1 = percent/100.f;
                    SimpleAudioEngine::getInstance()->setEffectsVolume(percent1);
                }
                    break;
                case Tag_BT_Effectminus:
                {
                    auto effect_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Effect);
                    effect_slider->setPercent(effect_slider->getPercent()-10);
                    HallDataMgr::getInstance()->m_fOptionSound = effect_slider->getPercent();
                    
                    float percent = effect_slider->getPercent();
                    float percent1 = percent/100.f;
                    SimpleAudioEngine::getInstance()->setEffectsVolume(percent1);
                }
                    break;
                case Tag_Shack_Open:
                {
                    pbutton->loadTextures("JDGswz/setting_res/bt_check_yes.png", "JDGswz/setting_res/bt_check_no.png");
                    auto pbutton1 = (Button *)pbg->getChildByTag(Tag_Shack_Close);
                    pbutton1->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionShake = true;
                }
                    break;
                case Tag_Shack_Close:
                {
                    pbutton->loadTextures("JDGswz/setting_res/bt_check_yes.png", "JDGswz/setting_res/bt_check_no.png");
                    auto pbutton1 = (Button *)pbg->getChildByTag(Tag_Shack_Open);
                    pbutton1->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionShake = false;
                }
                    break;
                case Tag_Sound_Off:
                {
                    HallDataMgr::getInstance()->m_bOptionMute = !HallDataMgr::getInstance()->m_bOptionMute;
                    auto music_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Music);
                    auto effect_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Effect);
                    if (HallDataMgr::getInstance()->m_bOptionMute) {
                        effect_slider->setPercent(0);
                        music_slider->setPercent(0);
                        
                        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.f);
                        SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);
                        pbutton->loadTextures("JDGswz/setting_res/bt_check_yes.png", "JDGswz/setting_res/bt_check_no.png");
                    }
                    else
                    {
                        effect_slider->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
                        music_slider->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
                        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(music_slider->getPercent()/100.f);
                        SimpleAudioEngine::getInstance()->setEffectsVolume(effect_slider->getPercent()/100.f);
                        
                        pbutton->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
                    }
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
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
        auto bg = this->getChildByTag(Tag_Bg);
        auto sound_off = (Button *)bg->getChildByTag(Tag_Sound_Off);
        sound_off->loadTextures("JDGswz/setting_res/bt_check_no.png", "JDGswz/setting_res/bt_check_yes.png");
        HallDataMgr::getInstance()->m_bOptionMute = false;
    }
}