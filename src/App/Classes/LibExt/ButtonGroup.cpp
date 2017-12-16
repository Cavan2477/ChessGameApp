//
//  GroupItem.cpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#include "ButtonGroup.h"

USING_NS_CC;
using namespace ui;
using namespace std;

ButtonGroup::ButtonGroup():
    m_strGroup(""),
    m_listen(nullptr),
    m_bEnable(true)
{
    m_mapGroupButton.clear();
}

ButtonGroup::~ButtonGroup()
{
    m_mapGroupButton.clear();
    m_listen = nullptr;
}

void ButtonGroup::addToGroup(cocos2d::ui::CheckBox *btn, const std::string &btnname)
{
    btn->setName(btnname);
    btn->addTouchEventListener(CC_CALLBACK_2(ButtonGroup::touchEvent, this));
    btn->addEventListener((CC_CALLBACK_2(ButtonGroup::checkEvent, this)));
    
    //btn->loadTextureNormal(<#const std::string &normal#>)
    m_mapGroupButton.insert(std::make_pair(btnname, btn));
}

void ButtonGroup::removeFromGroup(const std::string &btnname)
{
    std::map<string, CheckBox*>::iterator ite = m_mapGroupButton.find(btnname);
    if (ite != m_mapGroupButton.end())
    {
        ite->second->setName("");
        m_mapGroupButton.erase(ite);
    }
}

bool ButtonGroup::isBtnInGroup(const std::string &btnname)
{
    return m_mapGroupButton.find(btnname) != m_mapGroupButton.end();
}

void ButtonGroup::setSelecte(const std::string &btnname)
{
    if (!isBtnInGroup(btnname))
    {
        return;
    }
    std::map<string, CheckBox*>::iterator ite = m_mapGroupButton.begin();
    for (; ite != m_mapGroupButton.end(); ++ite)
    {
        bool bCheck = 0 == strcmp(ite->second->getName().c_str(), btnname.c_str());
        ite->second->setSelected(bCheck);
        if (m_listen)
        {
            m_listen->onCheckStatuChange(ite->second, bCheck);
        }
    }
}

void ButtonGroup::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (nullptr == pSender || !m_bEnable)
    {
        return;
    }
    
    CheckBox *pBtn = static_cast<CheckBox*>(pSender);
    string btnname = pBtn->getName();
    if (!isBtnInGroup(btnname))
    {
        return;
    }
    
    if (Widget::TouchEventType::ENDED == type)
    {
        //check box的touchEnd之后，会再判断check的状态
        if (m_listen)
        {
            if (!m_listen->preButtonChecked(pBtn, true))
            {
                pBtn->setSelected(true);
                return;
            }
        }
        std::map<string, CheckBox*>::iterator ite = m_mapGroupButton.begin();
        for (; ite != m_mapGroupButton.end(); ++ite)
        {
            bool bCheck = 0 == strcmp(ite->second->getName().c_str(), btnname.c_str());
            if (!bCheck)
            {
                ite->second->setSelected(bCheck);
            }
            else
            {
                ite->second->setSelected(false);
            }
            
            if (m_listen)
            {
                m_listen->onCheckStatuChange(ite->second, bCheck);
            }
        }
    }
}

void ButtonGroup::checkEvent(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
    if (nullptr == pSender || !m_bEnable)
    {
        return;
    }
    CheckBox *pBtn = static_cast<CheckBox*>(pSender);
    string btnname = pBtn->getName();
    if (!isBtnInGroup(btnname))
    {
        return;
    }
    
    bool bCheck = type == CheckBox::EventType::SELECTED;
    if (bCheck)
    {
        if (m_listen->preButtonChecked(pBtn, bCheck))
        {
            INSTANCE(AudioMgr)->playClickAudio();
            m_listen->onButtonChecked(pBtn);
        }        
    }
}