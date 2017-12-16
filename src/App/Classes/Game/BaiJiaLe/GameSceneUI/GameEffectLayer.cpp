//
//  GameEffectLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#include "GameEffectLayer.h"
#include "AnimationMgr.h"
USING_NS_CC;
using namespace ui;
using namespace std;
USING_BJL_NAMESPACE;

GameEffectLayer::GameEffectLayer()
{
    m_spRocket = nullptr;
    m_spAirplane = nullptr;
    /*for (int i = 0; i < GAME_PLAYER; ++i)
    {
        m_spAlarms[i] = nullptr;
    }*/
    m_spBomb = nullptr;
    m_spGameTip = nullptr;
    m_actDelayHide = nullptr;
}

GameEffectLayer::~GameEffectLayer()
{
    //unloadEffect();
    CC_SAFE_RELEASE(m_actDelayHide);
}

bool GameEffectLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("game/GameEffectLayer.csb");
        
        auto winSize = Director::getInstance()->getWinSize();
        m_spGameTip = static_cast<Sprite*>(m_root->getChildByName("game_tip"));
        CC_ASSERT(m_spGameTip != nullptr);
        
        m_imageTextBg = static_cast<ImageView*>(m_root->getChildByName("text_bg"));
        CC_ASSERT(m_imageTextBg != nullptr);
        m_imageTextBg->setVisible(false);
        
        m_textGameTip = static_cast<Text*>(m_root->getChildByName("text_tip"));
        CC_ASSERT(m_textGameTip != nullptr);
        
        /*m_alarmPos[kMY_ROLE] = Vec2(winSize.width * 0.19, winSize.height * 0.15);
        m_alarmPos[kRIGHT_ROLE] = Vec2(winSize.width * 0.84, winSize.height * 0.62);
        m_alarmPos[kLEFT_ROLE] = Vec2(winSize.width * 0.19, winSize.height * 0.62);*/
        
        auto delay = DelayTime::create(1.0f);
        auto hide = CallFunc::create([this]()
                                     {
                                         m_textGameTip->setString("");
                                         m_imageTextBg->setVisible(false);
                                     });
        m_actDelayHide = Sequence::create(delay, hide, NULL);
        m_actDelayHide->retain();
        //loadEffect();
        bRes = true;
    } while (false);
    return bRes;
}

void GameEffectLayer::reSet()
{
    m_spGameTip->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("blank.png"));
    /*for (int i = 0; i < GAME_PLAYER; ++i)
    {
        if (nullptr != m_spAlarms[i])
        {
            m_spAlarms[i]->stopAllActions();
            m_spAlarms[i]->setVisible(false);
            m_spAlarms[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_alarm_01.png"));
        }
    }*/
}

void GameEffectLayer::showGameTip(const emGameTip& tip /*= kGAME_TIP_BLANK*/, const std::string &str /*= ""*/)
{
    SpriteFrame *pFrame = nullptr;
    Size si = Director::getInstance()->getWinSize();
    Vec2 pos = Vec2(si.width * 0.5, si.height * 0.5);
    
    bool bAnimation = false;
    Action *pAct = nullptr;
    switch (tip)
    {
        case emGameTip::kGAME_TIP_BLANK:
            pFrame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png");
            break;
        case emGameTip::kGAME_TIP_ADDSCORE:
            pFrame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("game_tips_01.png");
            break;
        case emGameTip::kGAME_TIP_PASS:
            pFrame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("game_tips_00.png");
            pos = Vec2(si.width * 0.5, si.height * 0.25);
            break;
        case emGameTip::kGAME_FREE_TIP:
        {
            bAnimation = true;
            //切换动画
            auto sca = ScaleTo::create(0.2f, 0.0001f, 1.0f);
            auto call = CallFunc::create([this]()
                                         {
                                             char buf[64] = "";
                                             sprintf(buf, "room_freetime.png");
                                             SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(buf);
                                             if (nullptr != frame)
                                             {
                                                 m_spGameTip->setSpriteFrame(frame);
                                             }
                                         });
            auto sca2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
            auto call2 = CallFunc::create([this]()
                                          {
                                              this->showGameTip();
                                          });
            pAct = Sequence::create(sca, call, sca2, DelayTime::create(1.0f), call2, NULL);
        }
            break;
        case emGameTip::kGAME_JETTON_TIP:
        {
            bAnimation = true;
            //切换动画
            auto sca = ScaleTo::create(0.2f, 0.0001f, 1.0f);
            auto call = CallFunc::create([this]()
                                         {
                                             char buf[64] = "";
                                             sprintf(buf, "room_bettime.png");
                                             SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(buf);
                                             if (nullptr != frame)
                                             {
                                                 m_spGameTip->setSpriteFrame(frame);
                                             }
                                         });
            auto sca2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
            auto call2 = CallFunc::create([this]()
                                          {
                                              this->showGameTip();
                                          });
            pAct = Sequence::create(sca, call, sca2, DelayTime::create(1.0f), call2, NULL);
        }
            break;
        case emGameTip::kGAME_CHANGEBANKER_TIP:
            break;
        case emGameTip::kGAME_TEXT_TIP:
        {
            m_spGameTip->setVisible(false);
            m_textGameTip->setString(str);
            m_imageTextBg->setVisible(true);
            m_imageTextBg->setContentSize(Size(m_textGameTip->getContentSize().width + 18, 32));
            
            m_imageTextBg->stopAllActions();
            m_imageTextBg->runAction(m_actDelayHide);
            return;
        }
            break;
        default:
            break;
    }
    
    m_textGameTip->setString("");
    m_imageTextBg->setVisible(false);
    m_spGameTip->setVisible(true);
    if (!bAnimation)
    {
        if (nullptr == pFrame)
        {
            return;
        }
        m_spGameTip->setPosition(pos);
        m_spGameTip->setSpriteFrame(pFrame);
    }
    else
    {
        if (nullptr != pAct)
        {
            m_spGameTip->stopAllActions();
            m_spGameTip->runAction(pAct);
        }
    }
}

void GameEffectLayer::showRocketShootEffect()
{
    /*Size vSize = Director::getInstance()->getWinSize();
    if (nullptr == m_spRocket)
    {
        m_spRocket = Sprite::createWithSpriteFrameName("rocket_0.png");
        m_spRocket->setOpacity(0);
        this->addChild(m_spRocket);
    }
    m_spRocket->setPosition(vSize.width * 0.5, vSize.height * 0.5);
    m_spRocket->setOpacity(255);
    
    m_spRocket->stopAllActions();
    m_spRocket->runAction(m_actRocktShoot);*/
}

void GameEffectLayer::showAirplaneEffect()
{
    /*Size vSize = Director::getInstance()->getWinSize();
    if (nullptr == m_spAirplane)
    {
        m_spAirplane = Sprite::createWithSpriteFrameName("plane_0.png");
        m_spAirplane->setOpacity(0);
        this->addChild(m_spAirplane);
    }
    m_spAirplane->setPosition(vSize.width, vSize.height * 0.5);
    m_spAirplane->setOpacity(255);
    
    m_spAirplane->stopAllActions();
    m_spAirplane->runAction(m_actPlaneFly);*/
}

void GameEffectLayer::showBombEffect(const enGameRoleDir &dir)
{
    /*Size vSize = Director::getInstance()->getWinSize();
    if (nullptr == m_spBomb)
    {
        m_spBomb = Sprite::createWithSpriteFrameName("game_bomb_01.png");
        m_spBomb->setOpacity(0);
        this->addChild(m_spBomb);
    }
    m_spBomb->setPosition(vSize.width * 0.5, vSize.height * 0.5);
    m_spBomb->setOpacity(255);
    
    m_spBomb->stopAllActions();
    tagAnimationParam param = tagAnimationParam();
    param.m_fDelay = 0.1f;
    param.m_strName = BOMB_ANIMATION_KEY;
    auto animate = AnimationMgr::getInstance()->getAnimate(param);
    auto fadeOut = FadeOut::create(1.0f);
    auto call = CallFunc::create([this]
                                 {
                                     m_spBomb->setOpacity(0);
                                 });
    auto seq = Sequence::create(animate, fadeOut, call, NULL);
    m_spBomb->runAction(seq);*/
}

void GameEffectLayer::showAlarmEffect(const enGameRoleDir &dir)
{
    /*if (nullptr == m_spAlarms[dir])
    {
        m_spAlarms[dir] = Sprite::createWithSpriteFrameName("game_alarm_01.png");
        m_spAlarms[dir]->setVisible(false);
        m_spAlarms[dir]->setPosition(m_alarmPos[dir]);
        this->addChild(m_spAlarms[dir]);
    }
    
    tagAnimationParam param = tagAnimationParam();
    param.m_fDelay = 0.1f;
    param.m_strName = ALARM_ANIMATION_KEY;
    auto animate = AnimationMgr::getInstance()->getAnimate(param);
    auto repeat = RepeatForever::create(animate);
    m_spAlarms[dir]->stopAllActions();
    m_spAlarms[dir]->setVisible(true);
    m_spAlarms[dir]->runAction(repeat);*/
}

void GameEffectLayer::loadEffect()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //火箭动画(Spawn Sequence 和 RepeatForever 有冲突)
    tagAnimationParam param = tagAnimationParam();
    param.m_fDelay = 0.1f;
    param.m_strName = ROCKET_ANIMATION_KEY;
    auto animate = AnimationMgr::getInstance()->getAnimate(param);
    m_actShootRepeat = RepeatForever::create(animate);
    CC_SAFE_RETAIN(m_actShootRepeat);
    auto moveDown = MoveBy::create(0.1f,Vec2(0,-20));
    auto moveBy = MoveBy::create(2.0f,Vec2(0,500));
    auto fadeOut = FadeOut::create(2.0f);
    auto seq = Sequence::create(DelayTime::create(2.0f), CallFunc::create([](){  }), fadeOut,nullptr);
    auto spawn = Spawn::create(EaseExponentialIn::create(moveBy),/*seq,*/seq,nullptr);
    auto call = CallFuncN::create([=](Node* sender){ sender->runAction(m_actShootRepeat); });
    m_actRocktShoot = Sequence::create(call,moveDown,spawn,nullptr);
    CC_SAFE_RETAIN(m_actRocktShoot);
    
    //飞机动画
    param = tagAnimationParam();
    param.m_fDelay = 0.1f;
    param.m_strName = AIRSHIP_ANIMATION_KEY;
    animate = AnimationMgr::getInstance()->getAnimate(param);
    m_actFlyRepeat = RepeatForever::create(animate);
    CC_SAFE_RETAIN(m_actFlyRepeat);
    auto moveTo = MoveTo::create(3.0f, Vec2(0, winSize.height * 0.5));
    fadeOut = FadeOut::create(1.5f);
    seq = Sequence::create(DelayTime::create(1.5f), CallFunc::create([](){  }), fadeOut,nullptr);
    spawn = Spawn::create(/*EaseExponentialIn::create(moveTo)*/moveTo,seq,nullptr);
    call = CallFuncN::create([=](Node* sender){ sender->runAction(m_actFlyRepeat); });
    m_actPlaneFly = Sequence::create(call,spawn,nullptr);
    CC_SAFE_RETAIN(m_actPlaneFly);
}

void GameEffectLayer::unloadEffect()
{
    CC_SAFE_RELEASE(m_actShootRepeat);
    CC_SAFE_RELEASE(m_actRocktShoot);
    
    CC_SAFE_RELEASE(m_actFlyRepeat);
    CC_SAFE_RELEASE(m_actPlaneFly);
}