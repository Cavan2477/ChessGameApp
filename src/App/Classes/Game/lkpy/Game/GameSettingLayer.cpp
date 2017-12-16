#include "GameSettingLayer.h"
#include "HallDataMgr.h"
#include "lkpy_GameDataMgr.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
USING_LKPY_NAMESPACE;
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

bool GameSettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    cocos2d::Size viewsize = WinSize;
    //    auto pbg = Button::create("login_res/method_bg.png","login_res/method_bg.png");
    //    pbg->setPosition(cocos2d::Point(viewsize.width/2, viewsize.height/2));
    //    this->addChild(pbg);
    
    auto set_bg = ImageView::create("setting_res/setting_bg.png");
    Size bgSize = set_bg->getContentSize();
    set_bg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    set_bg->setTag(Tag_Bg);
    this->addChild(set_bg);
    set_bg->setOpacity(0);
    
    auto set_bg2 = ImageView::create("login_res/register_bg.png");
    set_bg2->setPosition(Vec2(bgSize.width/2,bgSize.height/2+10));
    //Scale9 (57x66)
    set_bg->addChild(set_bg2);
    set_bg2->setScale(0.85f);
    auto set_title = ImageView::create("setting_res/im_setting_title.png");
    set_title->setPosition(Vec2(set_bg->getContentSize().width/2, set_bg->getContentSize().height));
    set_bg->addChild(set_title);
    
    
    auto close = Button::create("setting_res/bt_close_0.png", "setting_res/bt_close_1.png");
    close->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParent();
        }});
    close->setScale(0.85f);
    close->setPosition(Vec2(set_bg->getContentSize().width+28, set_bg->getContentSize().height-12));
    //close->setPosition(Vec2(set_bg->getContentSize().width+88, set_bg->getContentSize().height+32));
    set_bg->addChild(close);
    
    
    //音乐
    auto txt_music = ImageView::create("setting_res/txt_music.png");
    txt_music->setPosition(Vec2(180, set_bg->getContentSize().height-88));
    set_bg->addChild(txt_music);
    
    auto music_slider = Slider::create();
    music_slider->loadBarTexture("setting_res/slider_bg.png");
    music_slider->loadSlidBallTextures("setting_res/slider_thumb.png", "setting_res/slider_thumb.png", "");
    music_slider->loadProgressBarTexture("setting_res/slider_progress.png");
    music_slider->setPosition(Vec2(set_bg->getContentSize().width/2, set_bg->getContentSize().height-136));
    music_slider->addEventListener(CC_CALLBACK_2(GameSettingLayer::sliderEvent, this));
    music_slider->setTag(Tag_Sl_Music);
    music_slider->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
    set_bg->addChild(music_slider);
    
    auto music_plus = Button::create("setting_res/bt_plus.png");
    music_plus->addTouchEventListener(CC_CALLBACK_2(GameSettingLayer::touchEvent, this));
    music_plus->setTag(Tag_BT_Musicplus);
    music_plus->setPosition(Vec2(635, music_slider->getPositionY()));
    set_bg->addChild(music_plus);
    
    auto music_minus = Button::create("setting_res/bt_minus.png");
    music_minus->addTouchEventListener(CC_CALLBACK_2(GameSettingLayer::touchEvent, this));
    music_minus->setTag(Tag_BT_Musicminus);
    music_minus->setPosition(Vec2(64, music_slider->getPositionY()));
    set_bg->addChild(music_minus);
    
    //音效
    auto txt_effect = ImageView::create("setting_res/txt_effect.png");
    txt_effect->setPosition(Vec2(180, set_bg->getContentSize().height-190));
    set_bg->addChild(txt_effect);
    
    auto effect_slider = Slider::create();
    effect_slider->loadBarTexture("setting_res/slider_bg.png");
    effect_slider->loadSlidBallTextures("setting_res/slider_thumb.png", "setting_res/slider_thumb.png", "");
    effect_slider->loadProgressBarTexture("setting_res/slider_progress.png");
    effect_slider->setPosition(Vec2(set_bg->getContentSize().width/2, set_bg->getContentSize().height-238));
    effect_slider->addEventListener(CC_CALLBACK_2(GameSettingLayer::sliderEvent, this));
    effect_slider->setTag(Tag_Sl_Effect);
    effect_slider->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
    set_bg->addChild(effect_slider);
    
    auto effect_plus = Button::create("setting_res/bt_plus.png");
    effect_plus->addTouchEventListener(CC_CALLBACK_2(GameSettingLayer::touchEvent, this));
    effect_plus->setTag(Tag_BT_Effectplus);
    effect_plus->setPosition(Vec2(635, effect_slider->getPositionY()));
    set_bg->addChild(effect_plus);
    
    auto effect_minus = Button::create("setting_res/bt_minus.png");
    effect_minus->addTouchEventListener(CC_CALLBACK_2(GameSettingLayer::touchEvent, this));
    effect_minus->setTag(Tag_BT_Effectminus);
    effect_minus->setPosition(Vec2(64, effect_slider->getPositionY()));
    set_bg->addChild(effect_minus);
    
 
    
    //全部静音
    auto sound_off = Button::create();
    HallDataMgr::getInstance()->m_bOptionMute?sound_off->loadTextures("setting_res/bt_check_yes.png", "setting_res/bt_check_no.png"):sound_off->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
    sound_off->setPosition(Vec2(415,set_bg->getContentSize().height-326));
    sound_off->setTag(Tag_Sound_Off);
    sound_off->addTouchEventListener(CC_CALLBACK_2(GameSettingLayer::touchEvent, this));
    set_bg->addChild(sound_off);
    if (HallDataMgr::getInstance()->m_bOptionMute) {
        music_slider->setPercent(0);
        effect_slider->setPercent(0);
    }
    
    auto txt_sound_off = ImageView::create("setting_res/txt_sound_off.png");
    txt_sound_off->setPosition(Vec2(210, set_bg->getContentSize().height-326));
    txt_sound_off->setAnchorPoint(Vec2(0.f, 0.5f));
    set_bg->addChild(txt_sound_off);
    
    return true;
}

bool GameSettingLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}
void GameSettingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void GameSettingLayer::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameSettingLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void GameSettingLayer::onExit()
{
    HallDataMgr::getInstance()->saveConfig();
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    Layer::onExit();
}

void GameSettingLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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
                }
                    break;
                case Tag_BT_Musicplus:
                {
                    auto music_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Music);
                    music_slider->setPercent(music_slider->getPercent()+10);
                    HallDataMgr::getInstance()->m_fOptionMusic = music_slider->getPercent();
                    
                    auto sound_off = (Button *)pbg->getChildByTag(Tag_Sound_Off);
                    sound_off->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionMute = false;
                }
                    break;
                case Tag_BT_Effectplus:
                {
                    auto effect_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Effect);
                    effect_slider->setPercent(effect_slider->getPercent()+10);
                    HallDataMgr::getInstance()->m_fOptionSound = effect_slider->getPercent();
                    
                    auto sound_off = (Button *)pbg->getChildByTag(Tag_Sound_Off);
                    sound_off->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionMute = false;
                }
                    break;
                case Tag_BT_Effectminus:
                {
                    auto effect_slider = (Slider *)pbg->getChildByTag(Tag_Sl_Effect);
                    effect_slider->setPercent(effect_slider->getPercent()-10);
                    HallDataMgr::getInstance()->m_fOptionSound = effect_slider->getPercent();
                }
                    break;
                case Tag_Shack_Open:
                {
                    pbutton->loadTextures("setting_res/bt_check_yes.png", "setting_res/bt_check_no.png");
                    auto pbutton1 = (Button *)pbg->getChildByTag(Tag_Shack_Close);
                    pbutton1->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_bOptionShake = true;
                }
                    break;
                case Tag_Shack_Close:
                {
                    pbutton->loadTextures("setting_res/bt_check_yes.png", "setting_res/bt_check_no.png");
                    auto pbutton1 = (Button *)pbg->getChildByTag(Tag_Shack_Open);
                    pbutton1->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
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
                        pbutton->loadTextures("setting_res/bt_check_yes.png", "setting_res/bt_check_no.png");
                    }
                    else
                    {
                        effect_slider->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
                        music_slider->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
                        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(music_slider->getPercent()/100.f);
                        SimpleAudioEngine::getInstance()->setEffectsVolume(effect_slider->getPercent()/100.f);
                        
                        pbutton->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
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

void GameSettingLayer::sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
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
        sound_off->loadTextures("setting_res/bt_check_no.png", "setting_res/bt_check_yes.png");
        HallDataMgr::getInstance()->m_bOptionMute = false;
    }
}