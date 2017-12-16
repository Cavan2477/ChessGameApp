//
//  GameJettonNode.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/2/16.
//
//

#include "GameJettonNode.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

GameJettonNode::GameJettonNode()
{
    m_llMyTotal = 0;
    m_llAreaTotal = 0;
}

GameJettonNode::~GameJettonNode()
{
    
}

bool GameJettonNode::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        
        LOAD_CSB("game/JettonNode.csb");
        
        m_imageBg = static_cast<ImageView*>(m_root->getChildByName("jetton_bg"));
        CC_ASSERT(m_imageBg != nullptr);
        
        m_textMyJetton = static_cast<Text*>(m_imageBg->getChildByName("jetton_my"));
        CC_ASSERT(m_textMyJetton != nullptr);
        
        m_textTotalJetton = static_cast<Text*>(m_imageBg->getChildByName("jetton_total"));
        CC_ASSERT(m_textTotalJetton != nullptr);
        
        bRes = true;
    } while (false);
    return bRes;
}

void GameJettonNode::refreshJetton( LONGLONG my,  LONGLONG total, bool bMyJetton)
{
    if (bMyJetton)
    {
        m_llMyTotal += my;
    }
    m_llAreaTotal += total;
    this->setVisible(0 != m_llAreaTotal);
    
    char buf[64] = "";
    sprintf(buf, "%lld",m_llMyTotal);
    std::string tmp = buf;
    size_t len = tmp.length();
    for (int i = (int)len - 3; i > 0; i -= 3)
    {
        tmp.insert(i, ",");
    }
    tmp.append(" /");
    if (tmp.length() > 15)
    {
        tmp = tmp.substr(0,12);
        tmp.append("... /");
    }
    m_textMyJetton->setString(tmp);
    
    sprintf(buf, "%lld",m_llAreaTotal);
    std::string tmps = buf;
    len = tmps.length();
    for (int i = (int)len - 3; i > 0; i -= 3)
    {
        tmps.insert(i, ",");
    }
    tmps = " " + tmps;
    if (tmp.length() > 15)
    {
        if (tmps.length() > 15)
        {
            tmps = tmps.substr(0,12);
            tmps.append("...");
        }
    }
    else
    {
        size_t l = 15 + tmp.length();
        if (tmps.length() > l)
        {
            tmps = tmps.substr(0,l - 3);
            tmps.append("...");
        }
    }
    m_textTotalJetton->setString(tmps);
    
    Size mySize = m_textMyJetton->getContentSize();
    Size tSize = m_textTotalJetton->getContentSize();
    Size totalSize = Size(mySize.width + tSize.width + 18, 32);
    m_imageBg->setContentSize(totalSize);
    
    m_textTotalJetton->setPositionX(6 + mySize.width );
}

void GameJettonNode::reSet()
{
    this->setVisible(false);
    
    m_textMyJetton->setString("");
    m_textTotalJetton->setString("");
    m_imageBg->setContentSize(Size(34,32));
    
    m_llMyTotal = 0;
    m_llAreaTotal = 0;
}