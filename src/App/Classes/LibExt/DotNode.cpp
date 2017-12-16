//
//  DotNode.cpp
//  NiuNiu
//
//  Created by zhong on 12/28/15.
//
//

#include "DotNode.h"
const float DOT_DISTANCE = 50.0f;
USING_NS_CC;
DotNode::DotNode():
m_nCurrentDot(0)
{
    
}

DotNode::~DotNode()
{
    SpriteFrameCache::getInstance()->removeSpriteFrameByName(m_strNormal);
    SpriteFrameCache::getInstance()->removeSpriteFrameByName(m_strSelect);
}

DotNode* DotNode::createDotNode(const std::string &normalfile, const std::string &selectfile)
{
    DotNode *p = new DotNode();
    if (nullptr != p && p->initDot(normalfile,selectfile))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool DotNode::initDot(const std::string &normalfile, const std::string &selectfile)
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        m_strNormal = normalfile;
        m_strSelect = selectfile;
        
        /*
        auto tmp = Sprite::create(m_strNormal);
        CC_ASSERT(tmp != nullptr);
        SpriteFrame *normal = SpriteFrame::create(m_strNormal, Rect(0, 0, tmp->getContentSize().width, tmp->getContentSize().height));
        SpriteFrameCache::getInstance()->addSpriteFrame(normal, m_strNormal);
        
        tmp = Sprite::create(m_strSelect);
        CC_ASSERT(tmp != nullptr);
        SpriteFrame *select = SpriteFrame::create(m_strSelect, Rect(0, 0, tmp->getContentSize().width, tmp->getContentSize().height));
        SpriteFrameCache::getInstance()->addSpriteFrame(select, m_strSelect);
         */
        bRes = true;
    } while (false);
    return bRes;
}

void DotNode::refreshDot(const int &nCount)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strNormal);
    if (nullptr == frame)
    {
        return;
    }
    for (int i = 0; i < nCount; ++i)
    {
        Sprite* dot = Sprite::createWithSpriteFrame(frame);
        this->addChild(dot);
        m_vecDot.push_back(dot);
    }
    arrangePos();
}

void DotNode::moveToDot(const int &nCount)
{
    if (nCount > m_vecDot.size())
    {
        return;
    }
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strSelect);
    SpriteFrame *norframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strNormal);
    if (nullptr == frame || nullptr == norframe)
    {
        return;
    }
    m_vecDot[m_nCurrentDot]->setSpriteFrame(norframe);
    m_nCurrentDot = nCount;
    m_vecDot[m_nCurrentDot]->setSpriteFrame(frame);
}

void DotNode::arrangePos()
{
    float center = m_vecDot.size() * 0.5;
    int idx = 0;
    for (size_t i = 0; i < m_vecDot.size(); ++i,++idx)
    {
        auto dot = m_vecDot[i];
        dot->setPositionX((idx - center) * DOT_DISTANCE);
    }
}