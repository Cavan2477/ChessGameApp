//
//  ApplyListLayer.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/2/16.
//
//

#ifndef bjl_header_ApplyListLayer_hpp
#define bjl_header_ApplyListLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "ApplyItemNode.h"

BJL_NAMESPACE_BEGIN

enum enApplyState
{
    kCancelState,           //取消、未申请状态
    kApplyState,            //申请状态
    kApplyedState,          //已申请状态
};

class ApplyListLayer:public UILayer,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
public:
    UILAYER_WITHNOTIFY(ApplyListLayer);
    
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView *table) override;
    
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    virtual void onEnterTransitionDidFinish() override;
    
    virtual void onExit() override;
public:
    /*
     * @brief 刷新申请列表
     * @param[vecInfo] 列表信息
     * @param[bMyApply] 自己是否申请
     */
    void refresh(const std::vector<tagApplyInfo> &vecInfo, const enApplyState &enState);
    
    void removeApplyItemByIdx(const int &nIdx);
    
    void reSet();
private:
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
private:
    cocos2d::EventListenerTouchOneByOne *m_listener;
    
    cocos2d::extension::TableView *m_applyList;
    cocos2d::Sprite *m_spBg;
    cocos2d::ui::Text *m_textApplyNum, *m_textTips;
    cocos2d::ui::Button *m_btnApply;
    //申请列表信息
    std::vector<tagApplyInfo> m_vecApplyInfo;
};

BJL_NAMESPACE_END
#endif /* ApplyListLayer_hpp */
