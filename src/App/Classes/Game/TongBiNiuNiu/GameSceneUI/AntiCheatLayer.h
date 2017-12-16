//
//  AntiCheatLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/24/15.
//
//

#ifndef tbnn_header_AntiCheatLayer_hpp
#define tbnn_header_AntiCheatLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

TBNN_NAMESPACE_BEGIN

class AntiCheatLayer:public UILayer
{
    UILAYER_WITHNOTIFY(AntiCheatLayer);
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
private:
    cocos2d::ui::Button *m_btnJoin;
    cocos2d::ui::Button *m_btnExit;
};

TBNN_NAMESPACE_END
#endif /* AntiCheatLayer_hpp */
