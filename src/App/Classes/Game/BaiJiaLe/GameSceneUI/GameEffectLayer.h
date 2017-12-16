//
//  GameEffectLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#ifndef bjl_header_GameEffectLayer_hpp
#define bjl_header_GameEffectLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameDefine.h"

BJL_NAMESPACE_BEGIN

class GameEffectLayer:public UILayer
{
public:
    GameEffectLayer();
    ~GameEffectLayer();
    
    enum emGameTip
    {
        kGAME_TIP_BLANK,
        kGAME_TIP_ADDSCORE,
        kGAME_TIP_RODBANKER,
        kGAME_TIP_OUTCARD,
        kGAME_TIP_PASS,
        kGAME_FREE_TIP,
        kGAME_JETTON_TIP,
        kGAME_CHANGEBANKER_TIP,
        kGAME_TEXT_TIP,
    };
    
    CREATE_FUNC(GameEffectLayer);
    virtual bool init();
    
    void reSet();
    
    void showGameTip(const emGameTip& tip = kGAME_TIP_BLANK, const std::string &str = "");
    
    void showRocketShootEffect();
    
    void showAirplaneEffect();
    
    void showBombEffect(const enGameRoleDir &dir);
    
    void showAlarmEffect(const enGameRoleDir &dir);
private:
    void loadEffect();
    void unloadEffect();
private:
    cocos2d::Sprite *m_spRocket;
    cocos2d::Sprite *m_spAirplane;
    //cocos2d::Sprite *m_spAlarms[100];
    cocos2d::Sprite *m_spBomb;
    //cocos2d::Vec2 m_alarmPos[100];
private:
    cocos2d::Sprite *m_spGameTip;
    cocos2d::ui::ImageView *m_imageTextBg;
    cocos2d::ui::Text *m_textGameTip;
private:
    cocos2d::Action *m_actShootRepeat;
    cocos2d::Action *m_actRocktShoot;
    
    cocos2d::Action *m_actFlyRepeat;
    cocos2d::Action *m_actPlaneFly;
    
    cocos2d::Action *m_actDelayHide;
};

BJL_NAMESPACE_END
#endif /* GameEffectLayer_hpp */
