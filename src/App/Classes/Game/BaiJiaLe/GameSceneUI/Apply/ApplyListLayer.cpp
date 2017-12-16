//
//  ApplyListLayer.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/2/16.
//
//

#include "ApplyListLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
USING_BJL_NAMESPACE;

ApplyListLayer::ApplyListLayer():
m_notify(nullptr)
{
    
}

ApplyListLayer::~ApplyListLayer()
{
    
}

bool ApplyListLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        LOAD_CSB("game/ApplyListLayer.csb");
        m_spBg = static_cast<Sprite*>(m_root->getChildByName("room_list_bg"));
        CC_ASSERT(m_spBg != nullptr);
        
        m_textApplyNum = static_cast<Text*>(m_spBg->getChildByName("apply_text"));
        CC_ASSERT(m_textApplyNum != nullptr);
        m_textApplyNum->setString("");
        
        m_textTips = static_cast<Text*>(m_spBg->getChildByName("tips_text"));
        CC_ASSERT(m_textTips != nullptr);
        m_textTips->setString("");
        
        m_btnApply = static_cast<Button*>(m_spBg->getChildByName("apply_btn"));
        CC_ASSERT(m_btnApply != nullptr);
        m_btnApply->addTouchEventListener(CC_CALLBACK_2(ApplyListLayer::touchEvent, this));
        
        m_applyList = TableView::create(this,Size(460,280));
        m_applyList->setDirection(TableView::Direction::VERTICAL);
        m_applyList->setDelegate(this);
        m_applyList->setPosition(Vec2(38, 155));
        m_spBg->addChild(m_applyList);
        
        bRes = true;
    } while (false);
    return bRes;
}

cocos2d::extension::TableViewCell* ApplyListLayer::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    ApplyItemNode *item = nullptr;
    
    if (nullptr == cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        item = ApplyItemNode::create();
        item->setPosition(table->getViewSize().width * 0.5, 0);
        item->setName("apply_item_view");
        cell->addChild(item);
    }
    else
    {
        item = static_cast<ApplyItemNode*>(cell->getChildByName("apply_item_view"));
    }
    
    if (nullptr != item && idx < m_vecApplyInfo.size())
    {
        tagApplyInfo info = m_vecApplyInfo[idx];
        item->refresh(info);
    }
    return cell;
}

ssize_t ApplyListLayer::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return m_vecApplyInfo.size();
}

cocos2d::Size ApplyListLayer::cellSizeForTable(cocos2d::extension::TableView *table)
{
    return Size(460, 30);
}

void ApplyListLayer::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

void ApplyListLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    
    m_listener->onTouchBegan = [this](Touch *pTouch,Event *event)
    {
        return this->isVisible();
    };
    m_listener->onTouchEnded = [this](Touch *pTouch,Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        pos = m_spBg->convertToNodeSpace(pos);
        Rect rec = Rect(0, 0, m_spBg->getContentSize().width, m_spBg->getContentSize().height);
        if (!rec.containsPoint(pos))
        {
            this->setVisible(false);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void ApplyListLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Layer::onExit();
}

void ApplyListLayer::refresh(const std::vector<tagApplyInfo> &vecInfo, const enApplyState &enState)
{
    m_vecApplyInfo = vecInfo;
    m_applyList->reloadData();
    
    //申请人数
    char buf[64] = "";
    sprintf(buf, "%lu", m_vecApplyInfo.size());
    std::string str = buf;
    m_textApplyNum->setString(str);
    std::string tmp = "";
    for (size_t i = 0; i < str.length() + 2; ++i)
    {
        tmp.append(" ");
    }
    sprintf(buf, "现有%s个人申请上庄",tmp.c_str());
    m_textTips->setString(buf);
    
    str = "";
    switch (enState)
    {
        case enApplyState::kCancelState:
        {
            str = "room_list_button_application.png";
        }
            break;
        case enApplyState::kApplyState:
        {
            str = "room_list_button_cancelapply.png";
        }
            break;
        case enApplyState::kApplyedState:
        {
            str = "room_list_button_cancel.png";
        }
            break;
        default:
            break;
    }
    m_btnApply->loadTextureDisabled(str,Widget::TextureResType::PLIST);
    m_btnApply->loadTextureNormal(str,Widget::TextureResType::PLIST);
    m_btnApply->loadTexturePressed(str,Widget::TextureResType::PLIST);
    
    m_btnApply->setTag(enState);
    
    if (HallDataMgr::getInstance()->m_UserScore > 10000000)
    {
        m_btnApply->setEnabled(true);
        m_btnApply->setOpacity(255);
    }
    else
    {
        m_btnApply->setEnabled(false);
        m_btnApply->setOpacity(125);
    }
}

void ApplyListLayer::removeApplyItemByIdx(const int &nIdx)
{
    m_applyList->reloadData();
}

void ApplyListLayer::reSet()
{
    this->setVisible(false);
    
    m_applyList->reloadData();
    
    std::vector<tagApplyInfo>().swap(m_vecApplyInfo);
}

void ApplyListLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == tType)
    {
        if (m_notify)
        {
            enApplyState enState = (enApplyState)pWidget->getTag();
            m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::APPLY_BANKER, (void*)&enState, sizeof(pWidget->getTag()));
        }
    }
}