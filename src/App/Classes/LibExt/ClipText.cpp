/************************************************************************************
 * file: 		ClipText.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:10:59
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "ClipText.h"
#include "../Utils/UtilEx.h"

USING_NS_CC;

using namespace ui;

enum CLIPNODE_TAG
{
    kTEXT_CHILD,
};

ClipText::ClipText(const cocos2d::Size &visibleSize):
m_szVisibleSize(visibleSize),
m_actMoveAct(nullptr),
m_emClipType(emClipType::kClipReplace),
m_strReplaceStr(".")
{
    
}

ClipText::~ClipText()
{
    CC_SAFE_RELEASE(m_actMoveAct);
}

ClipText* ClipText::createClipText(const cocos2d::Size &visibleSize,
                                   const std::string &str,
                                   const std::string &fontName /*= "Arial"*/,
                                   const int &fontSize /*= 20*/)
{
    ClipText *p = new ClipText(visibleSize);
    if (nullptr != p && p->initText(str,fontName,fontSize))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void ClipText::onEnterTransitionDidFinish()
{
    Layout::onEnterTransitionDidFinish();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(false);
    m_listener->onTouchBegan = [this](Touch *pTouch, Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        auto child = this->getClipText();
        if (child)
        {
            pos = child->convertToNodeSpace(pos);
            Rect rect = Rect(0, 0, child->getContentSize().width, child->getContentSize().height);
            
            if (rect.containsPoint(pos))
            {
                return true;
            }
        }
        return false;
    };
    m_listener->onTouchEnded = [this](Touch *pTouch, Event *event)
    {
        auto child = this->getClipText();
        if (child)
        {
            if (0 == child->getNumberOfRunningActions())
            {
                if (child->getContentSize().width >= m_szVisibleSize.width && emClipType::kClipRunAct == m_emClipType)
                {
                    child->stopAction(m_actMoveAct);
                    child->setPosition(Vec2(m_szVisibleSize.width * child->getAnchorPoint().x, m_szVisibleSize.height * child->getAnchorPoint().y));
                    child->runAction(m_actMoveAct);
                }
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void ClipText::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Layout::onExit();
}

void ClipText::setAnchorPoint(const cocos2d::Vec2 &anchorPoint)
{
    Layout::setAnchorPoint(anchorPoint);
    setTextAnchorPoint(anchorPoint);
}

void ClipText::setClipMode(const ClipText::emClipType &mode, const std::string &replacestr /*= "."*/)
{
    m_emClipType = mode;
    m_strReplaceStr = replacestr;
    
    Node *pNode = getClipText();
    if (nullptr == pNode)
    {
        return;
    }
    
    if (pNode->getContentSize().width >= m_szVisibleSize.width)
    {
        if (emClipType::kClipRunAct == m_emClipType)
        {
            pNode->stopAllActions();
            pNode->setPosition(Vec2(pNode->getContentSize().width * pNode->getAnchorPoint().x + m_szVisibleSize.width, m_szVisibleSize.height * pNode->getAnchorPoint().y));
            pNode->runAction(m_actMoveAct);
        }
        else
        {
            threadParseStr(m_strText);
        }
    }
}

Node* ClipText::getClipText()
{
    return this->getChildByTag(kTEXT_CHILD);
}

void ClipText::setTextAnchorPoint(const cocos2d::Vec2 &anchor)
{
    Node *pNode = getClipText();
    if (nullptr != pNode)
    {
        pNode->setAnchorPoint(anchor);
        updateMoveAction(pNode);
        
        if (emClipType::kClipRunAct == m_emClipType)
        {
            pNode->stopAllActions();
            pNode->setPosition(Vec2(pNode->getContentSize().width * pNode->getAnchorPoint().x + m_szVisibleSize.width, m_szVisibleSize.height * pNode->getAnchorPoint().y));
            pNode->runAction(m_actMoveAct);
        }
        else
        {
            pNode->setPosition(m_szVisibleSize.width * anchor.x, m_szVisibleSize.height * anchor.y);
        }
    }
}

void ClipText::setTextPosition(const cocos2d::Vec2 &pos)
{
    Node *pNode = getClipText();
    if (nullptr != pNode)
    {
        pNode->setPosition(pos);
        
        updateMoveAction(pNode);
    }
}

void ClipText::setString(const std::string &str)
{
    m_strText = str;
    Node *pNode = getClipText();
    if (nullptr == pNode)
    {
        return;
    }
    
    switch (m_emTextType)
    {
        case emTextType::kCLIP_TEXT:
        {
            Text *pText = static_cast<Text*>(pNode);
            if (nullptr != pText)
            {
                pText->setString(str);
            }
        }
            break;
        case emTextType::kCLIP_TEXTATLAS:
        {
            TextAtlas *pTextAtlas = static_cast<TextAtlas*>(pNode);
            if (nullptr != pTextAtlas)
            {
                pTextAtlas->setString(str);
            }
        }
            break;
        default:
            break;
    }
    updateMoveAction(pNode);
    
    if (pNode->getContentSize().width >= m_szVisibleSize.width)
    {
        if (emClipType::kClipRunAct == m_emClipType)
        {
            pNode->stopAllActions();
            pNode->setPosition(Vec2(pNode->getContentSize().width * pNode->getAnchorPoint().x + m_szVisibleSize.width, m_szVisibleSize.height * pNode->getAnchorPoint().y));
            pNode->runAction(m_actMoveAct);
        }
        else
        {
            threadParseStr(str);
        }
    }
}

void ClipText::setTextColor(const cocos2d::Color4B &color)
{
    Node *pNode = getClipText();
    if (nullptr == pNode)
    {
        return;
    }
    
    switch (m_emTextType)
    {
        case emTextType::kCLIP_TEXT:
        {
            Text *pText = static_cast<Text*>(pNode);
            if (nullptr != pText)
            {
                pText->setTextColor(color);
            }
        }
            break;
        case emTextType::kCLIP_TEXTATLAS:
        {
            
        }
            break;
        default:
            break;
    }
}

void ClipText::setTextFontSize(const float &fSize)
{
    Node *pNode = getClipText();
    if (nullptr == pNode)
    {
        return;
    }
    
    switch (m_emTextType)
    {
        case emTextType::kCLIP_TEXT:
        {
            Text *pText = static_cast<Text*>(pNode);
            if (nullptr != pText)
            {
                pText->setFontSize(fSize);
            }
        }
            break;
        case emTextType::kCLIP_TEXTATLAS:
        {
            
        }
            break;
        default:
            break;
    }
}

bool ClipText::initText(const std::string &str,
                        const std::string &fontName /*= "Arial"*/,
                        const int &fontSize /*= 20*/)
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layout::init());
        
        this->setClippingEnabled(true);
        this->setContentSize(m_szVisibleSize);
        
        m_emTextType = emTextType::kCLIP_TEXT;
        m_strText = str;
        m_nFontSize = fontSize;
        
        auto text = Text::create(str, fontName, m_nFontSize);
        CC_BREAK_IF(text == nullptr);
        text->setTag(kTEXT_CHILD);
        this->addChild(text);
        
        updateMoveAction(text);
        
        setTextAnchorPoint(this->getAnchorPoint());
        if (text->getContentSize().width >= m_szVisibleSize.width)
        {
            if (emClipType::kClipRunAct == m_emClipType)
            {
                text->stopAllActions();
                text->setPosition(Vec2(text->getContentSize().width * text->getAnchorPoint().x + m_szVisibleSize.width, m_szVisibleSize.height * text->getAnchorPoint().y));
                text->runAction(m_actMoveAct);
            }
            else //字符替换
            {
                threadParseStr(str);
            }
        }
        bRes = true;
    } while (false);
    return bRes;
}

void ClipText::updateMoveAction(cocos2d::Node *text)
{
    CC_SAFE_RELEASE(m_actMoveAct);
    Size textSize = text->getContentSize();
    Vec2 pos = Vec2(-textSize.width, 0);
    auto moveBy = MoveTo::create(5.0f + (textSize.width / m_szVisibleSize.width) * 2.0f, pos);
    auto delay = DelayTime::create(1.5f);
    auto back = CallFunc::create([text,this]()
                                 {
                                     text->setPosition(Vec2(text->getContentSize().width * text->getAnchorPoint().x + m_szVisibleSize.width, m_szVisibleSize.height * text->getAnchorPoint().y));
                                 });
    auto seq = Sequence::create(moveBy, delay, back, NULL);
    auto repeat = Repeat::create(seq, 3);
    auto backOrigin = MoveTo::create(3.0f, Vec2(0, 0));
    m_actMoveAct = Sequence::create(repeat, backOrigin, NULL);
    CC_SAFE_RETAIN(m_actMoveAct);
}

void ClipText::threadParseStr(const std::string &str)
{
    /*
     std::thread t([this,str]()
     {
     int nLen = m_nFontSize;
     
     int nTotal = m_szVisibleSize.width - m_nFontSize;
     bool bSplite = false;
     std::string *pStr = new std::string(CUtilEx::spliteUTF8Str(str, nLen, nTotal,bSplite));
     
     if (bSplite)
     {
     pStr->append(m_strReplaceStr);
     pStr->append(m_strReplaceStr);
     pStr->append(m_strReplaceStr);
     }
     INSTANCE(MsgDispatcher)->addDataToCallBackQueue(QUEUE_MSG_BASE::QUEUE_MSG_CLIP, CLIP_QUEUE::STRING_PARSED, pStr, (int)pStr->length());
     });
     t.detach();
     */
    
    int nLen = m_nFontSize;
    
    int nTotal = m_szVisibleSize.width - m_nFontSize;
    bool bSplite = false;
    std::string pStr = CUtilEx::spliteUTF8Str(str, nLen, nTotal,bSplite);
    
    if (bSplite)
    {
        pStr.append(m_strReplaceStr);
        pStr.append(m_strReplaceStr);
        pStr.append(m_strReplaceStr);
    }
    
    Node *pNode = getClipText();
    if (nullptr == pNode)
    {
        return;
    }
    
    //更新文本
    switch (m_emTextType)
    {
        case emTextType::kCLIP_TEXT:
        {
            Text *pText = static_cast<Text*>(pNode);
            if (nullptr != pText)
            {
                pText->setString(pStr);
            }
        }
            break;
        case emTextType::kCLIP_TEXTATLAS:
        {
            TextAtlas *pTextAtlas = static_cast<TextAtlas*>(pNode);
            if (nullptr != pTextAtlas)
            {
                pTextAtlas->setString(pStr);
            }
        }
            break;
        default:
            break;
    }
}