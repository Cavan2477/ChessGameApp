//
//  GameEffectLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#ifndef nn_GameEffectLayer_hpp
#define nn_GameEffectLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

NN_NAMESPACE_BEGIN

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

NN_NAMESPACE_END

#endif /* GameEffectLayer_hpp */
