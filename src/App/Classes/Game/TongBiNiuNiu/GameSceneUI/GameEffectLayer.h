//
//  GameEffectLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#ifndef tbnn_header_GameEffectLayer_hpp
#define tbnn_header_GameEffectLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

TBNN_NAMESPACE_BEGIN

class GameEffectLayer:public UILayer
{
public:
    GameEffectLayer();
    ~GameEffectLayer();
    
    enum emGameTip
    {
        kGAME_TIP_BLANK,
        kGAME_TIP_ADDSCORE,
        kGAME_TIP_OUTCARD,
    };
    
    CREATE_FUNC(GameEffectLayer);
    virtual bool init();
    
    void reSet();
    
    void showGameTip(const emGameTip& tip = kGAME_TIP_BLANK);
private:
    cocos2d::Sprite *m_spGameTip;
};

TBNN_NAMESPACE_END

#endif /* GameEffectLayer_hpp */
