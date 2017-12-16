//
//  AntiCheatLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/24/15.
//
//

#ifndef bjl_header_AntiCheatLayer_hpp
#define bjl_header_AntiCheatLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

BJL_NAMESPACE_BEGIN

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
BJL_NAMESPACE_END

#endif /* AntiCheatLayer_hpp */
