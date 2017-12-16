//
//  WallBillLayer.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/7/16.
//
//

#ifndef bjl_header_WallBillLayer_hpp
#define bjl_header_WallBillLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameDefine.h"

BJL_NAMESPACE_BEGIN

class WallBillLayer:public UILayer
{
public:
    UILAYER_WITHNOTIFY(WallBillLayer);
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
public:
    void reSet();
    
    /*
     * @brief 刷新记录列表
     */
    void refreshWallBillList();
    
    void showLayer(bool bShow);
private:
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
    
    void refreshList();
private:
    cocos2d::ui::Layout *m_layoutBillSp;
    cocos2d::Sprite *m_spRecrod[29][6];
    cocos2d::Vec2 m_vec2Pos[6];
    
    cocos2d::ui::Layout *m_layoutBillSp2;
    cocos2d::Sprite *m_spRecord2[14][6];
    cocos2d::Vec2 m_vec2Pos2[6];
    
    cocos2d::ui::Layout *m_layoutSettle;
    cocos2d::ui::Text *m_textXianSettle, *m_textPingSettle, *m_textZhuangSettle;
    cocos2d::ui::Text *m_textXianDoubleCount, *m_textZhuangDoubleCount, *m_textXianTianCount, *m_textZhuangTianCount;
    //索引下标
    int m_nBeginIdx;
    
    cocos2d::Sprite *m_spBg;
    cocos2d::EventListenerTouchOneByOne *m_listener;
};

BJL_NAMESPACE_END
#endif /* WallBillLayer_hpp */
