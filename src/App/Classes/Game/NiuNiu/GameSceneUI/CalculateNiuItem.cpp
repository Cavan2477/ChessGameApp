//
//  CalculateNiuItem.cpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#include "CalculateNiuItem.h"
USING_NS_CC;
using namespace ui;
using namespace std;
USING_NN_NAMESPACE;

CalculateNiuItem::CalculateNiuItem()
{
    
}

CalculateNiuItem::~CalculateNiuItem()
{
    
}

bool CalculateNiuItem::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Node::init());
        LOAD_CSB("game/AddNiuNiuItem.csb");
        
        char buf[64] = "";
        for (int i = 0; i < 3; ++i)
        {
            memset(buf, 0, 64);
            sprintf(buf, "text%d",i);
            m_text[i] = static_cast<Text*>(m_root->getChildByName(buf));
            CC_ASSERT(m_text[i] != nullptr);
        }
        //牛牛结果
        m_niuniu = NiuNiuNode::create();
        m_niuniu->setPosition(Vec2(195.0f,7.0f));
        m_root->addChild(m_niuniu);
        
        bRes = true;
    } while (false);
    return bRes;
}

void CalculateNiuItem::reSet()
{
    for (int i = 0; i < 3; ++i)
    {
        m_text[i]->setString("");
        m_text[i]->setTag(0);
    }
    m_niuniu->reSet();
}

void CalculateNiuItem::refreshNumber(const BYTE &num1, const BYTE &num2, const BYTE &num3)
{
    BYTE tmp = num1;
    if (tmp > 10)
    {
        tmp = 10;
    }
    char buf[12];
    sprintf(buf, "%d",tmp);
    m_text[0]->setString(buf);
    m_text[0]->setTag(num1);
    
    tmp = num2;
    if (tmp > 10)
    {
        tmp = 10;
    }
    sprintf(buf, "%d",tmp);
    m_text[1]->setString(buf);
    m_text[1]->setTag(num2);
    
    tmp = num3;
    if (tmp > 10)
    {
        tmp = 10;
    }
    sprintf(buf, "%d",tmp);
    m_text[2]->setString(buf);
    m_text[2]->setTag(num3);
}

void CalculateNiuItem::addNumber(const BYTE &num)
{
    Text *pText = nullptr;
    for (int i = 0; i < 3; ++i)
    {
        if (m_text[i]->getTag() == 0)
        {
            pText = m_text[i];
            break;
        }
    }
    //都有数字，将末尾更换
    if (nullptr == pText)
    {
        pText = m_text[2];
    }
    
    BYTE tmp = num;
    if (num > 10)
    {
        tmp = 10;
    }
    char buf[12];
    sprintf(buf, "%d",tmp);
    pText->setString(buf);
    pText->setTag(num);
}

void CalculateNiuItem::cutNumber(const BYTE &num)
{
    Text *pText = static_cast<Text*>(m_root->getChildByTag(num));
    if (nullptr != pText)
    {
        pText->setString("");
        pText->setTag(0);
    }
}

void CalculateNiuItem::showResult(const emNiuNiuType &cardType)
{
    m_niuniu->refreshNiuNiu(cardType);
}

void CalculateNiuItem::hideResult()
{
    m_niuniu->reSet();
}