//
//  NameLabel.cpp
//  
//
//  Created by wh on 16/3/21.
//
//

#include "NameLabel.h"
#include "Project.h"
USING_NS_CC;
using namespace ui;

NameLabel * NameLabel::create(const std::string &text, const std::string &font, float fontSize)
{
    auto plabel = new NameLabel();
    if (plabel->init()) {
        plabel->initwithstr(text, font, fontSize);
        plabel->autorelease();
        return plabel;
    }
    CC_SAFE_DELETE(plabel);
    return nullptr;
}

bool NameLabel::init()
{
    if (!Layout::init()) {
        return false;
    }
    
    m_label = nullptr;
    return true;
}

void NameLabel::initwithstr(const std::string &text, const std::string &font, float fontSize)
{
    m_label = Label::createWithSystemFont(text, font, fontSize);
    m_label->setAnchorPoint(Vec2(0.f, 0.5f));
    m_label->setPosition(Vec2(5, fontSize/2));
    this->addChild(m_label);
}

void NameLabel::setstring(const std::string &str)
{
    m_label->setString(str);
    //float flength = this->getContentSize().width;
    //float labellength = m_label->getContentSize().width;
    if (m_label->getContentSize().width > this->getContentSize().width) {
        float length = this->getContentSize().width-m_label->getSystemFontSize();
        m_label->setDimensions(length, m_label->getSystemFontSize()+5);
        if (!m_dotlabel) {
            m_dotlabel = Label::createWithSystemFont("..", m_label->getSystemFontName(), m_label->getSystemFontSize());
            m_dotlabel->setColor(m_label->getColor());
            m_dotlabel->setAnchorPoint(Vec2(0.f, 0.5f));
            this->addChild(m_dotlabel);
        }
        m_dotlabel->setVisible(true);
        if (m_label->getTextAlignment() == TextHAlignment::CENTER && m_label->getAnchorPoint().x == 0.5f) {
            m_dotlabel->setPosition(Vec2(m_label->getContentSize().width/2+m_label->getPositionX(), m_label->getPositionY()));
        }
        else
        {
            m_dotlabel->setPosition(Vec2(m_label->getContentSize().width+m_label->getPositionX(), m_label->getPositionY()));
        }
    }
    else
    {
        if (m_dotlabel)
        {
            m_dotlabel->setVisible(false);
        }
        m_label->setDimensions(0, 0);
    }
}

void NameLabel::setFontColor(const cocos2d::Color3B &color)
{
    m_label->setColor(color);
    if (m_dotlabel) {
        m_dotlabel->setColor(color);
    }
}

void NameLabel::setFont(const std::string &font, float fontSize)
{
    m_label->setSystemFontName(font);
    m_label->setSystemFontSize(fontSize);
    this->setstring(m_label->getString());
}

void NameLabel::setSize(const cocos2d::Size &contentSize)
{
    Layout::setContentSize(contentSize);
    if (m_label) {
        m_label->setPositionY(this->getContentSize().height/2);
        this->setstring(m_label->getString());
    }
}