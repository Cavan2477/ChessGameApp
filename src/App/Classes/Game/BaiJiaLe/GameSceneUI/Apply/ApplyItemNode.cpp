//
//  ApplyItemNode.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/2/16.
//
//

#include "ApplyItemNode.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

ApplyItemNode::ApplyItemNode()
{
    
}

ApplyItemNode::~ApplyItemNode()
{
    
}

bool ApplyItemNode::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        
        LOAD_CSB("game/ApplyItemNode.csb");
        
        m_spCurrentBg = static_cast<Sprite*>(m_root->getChildByName("current_bg"));
        CC_ASSERT(m_spCurrentBg != nullptr);
        
        m_spApplyNumBg = static_cast<Sprite*>(m_root->getChildByName("listnum_bg"));
        CC_ASSERT(m_spApplyNumBg != nullptr);
        
        m_textApplyNum = static_cast<Text*>(m_root->getChildByName("apply_text"));
        CC_ASSERT(m_textApplyNum != nullptr);
        
        m_clipUserName = ClipText::createClipText(Size(250, 23), "");
        CC_ASSERT(m_clipUserName != nullptr);
        m_clipUserName->setTextAnchorPoint(Vec2(0, 0));
        m_clipUserName->setPosition(Vec2(-220, 0));
        m_root->addChild(m_clipUserName);
        bRes = true;
    } while (false);
    return bRes;
}

void ApplyItemNode::refresh(const tagApplyInfo &tagInfo)
{
    //用户昵称
    m_clipUserName->setString(tagInfo.m_strUserName);
    m_clipUserName->setTextColor(Color4B::BLACK);
    
    //排号
    int nNum = tagInfo.m_cbApplyNum;
    char buf[32] = "";
    if (nNum < 10)
    {
        sprintf(buf, "00%d",nNum);
    }
    else if (nNum >= 10 && nNum < 100)
    {
        sprintf(buf, "0%d",nNum);
    }
    else
    {
        sprintf(buf, "%d",nNum);
    }
    m_textApplyNum->setString(buf);
    
    SpriteFrame *frame = nullptr;
    //是否当前
    if (tagInfo.m_bCurrent)
    {
        m_spCurrentBg->setVisible(true);
        frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("room_list_current.png");
        m_textApplyNum->setTextColor(Color4B::BLACK);
    }
    else
    {
        m_spCurrentBg->setVisible(false);
        frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("room_list_other.png");
        m_textApplyNum->setTextColor(Color4B::WHITE);
    }
    if (nullptr != frame)
    {
        m_spApplyNumBg->setSpriteFrame(frame);
    }
}