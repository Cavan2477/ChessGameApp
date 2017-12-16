//
//  BJLSimpleWallBill.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/4/16.
//
//

#ifndef bjl_header_BJLSimpleWallBill_hpp
#define bjl_header_BJLSimpleWallBill_hpp

/*
 * @brief 简易路单
 */
#include <stdio.h>
#include "CocosHeader.h"

BJL_NAMESPACE_BEGIN

class SimpleWallBillLayer:public UILayer
{
public:
    UILAYER_WITHNOTIFY(SimpleWallBillLayer);
public:
    void reSet();
    
    /*
     * @brief 刷新记录列表
     */
    void refreshRecordList();
private:
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
    
    void refreshList();
private:
    cocos2d::ui::Button *m_btnLast, *m_btnNext;
    cocos2d::Sprite *m_spRecordBg;
    cocos2d::Sprite *m_spRecrod[15];
    cocos2d::ui::Text *m_textMasterWin, *m_textIdleWin, *m_textDraw;
    cocos2d::Vec2 m_vec2Pos[3];
    
    //索引下标
    int m_nBeginIdx;
};

BJL_NAMESPACE_END
#endif /* BJLSimpleWallBill_hpp */
