//
//  HelpLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/24/15.
//
//

#ifndef nn_HelpLayer_hpp
#define nn_HelpLayer_hpp

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
