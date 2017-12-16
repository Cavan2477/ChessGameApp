//
//  OpenBankLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#include "OpenBankLayer.h"
#include "HallDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

OpenBankLayer::OpenBankLayer():
m_notify(nullptr)
{
    
}

OpenBankLayer::~OpenBankLayer()
{
    
}

OpenBankLayer* OpenBankLayer::createOpenBank(UILayer *notify)
{
    OpenBankLayer* p = new OpenBankLayer();
    if (nullptr != p && p->init())
    {
        p->m_notify = notify;
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool OpenBankLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("bank/OpenBankLayer.csb");
        
        auto tmp = m_root->getChildByName("bank_text_rect");
        CC_ASSERT(tmp != nullptr);
        //密码输入
        m_fieldPasswd = ui::EditBox::create(tmp->getBoundingBox().size, "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldPasswd != nullptr);
        m_fieldPasswd->setPosition(tmp->getPosition());
        m_fieldPasswd->setPlaceholderFontName("Arial");
        m_fieldPasswd->setFontName("Arial");
        m_fieldPasswd->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldPasswd->setPlaceHolder("请输入6-32位字符或数字");
        m_fieldPasswd->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldPasswd->setPlaceholderFontColor(Color3B::WHITE);
        m_fieldPasswd->setDelegate(this);
        m_fieldPasswd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        m_fieldPasswd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        m_fieldPasswd->setMaxLength(33);
        m_root->addChild(m_fieldPasswd);
        
        tmp = m_root->getChildByName("bank_text_rect_sure");
        CC_ASSERT(tmp != nullptr);
        //密码确认
        m_fieldSure = ui::EditBox::create(tmp->getBoundingBox().size, "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldSure != nullptr);
        m_fieldSure->setPosition(tmp->getPosition());
        m_fieldSure->setPlaceholderFontName("Arial");
        m_fieldSure->setFontName("Arial");
        m_fieldSure->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldSure->setPlaceHolder("请确认您刚才输入的密码");
        m_fieldSure->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldSure->setPlaceholderFontColor(Color3B::WHITE);
        m_fieldSure->setDelegate(this);
        m_fieldSure->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        m_fieldSure->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        m_root->addChild(m_fieldSure);
        
        //确认开通
        auto csbBtn = static_cast<Button*>(m_root->getChildByName("openbank_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(OpenBankLayer::touchEvent, this));
        bRes = true;
    } while (false);
    return bRes;
}

void OpenBankLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    
}

void OpenBankLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}

void OpenBankLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void OpenBankLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void OpenBankLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    Widget *pWidget = (Widget*)pSender;
    if (nullptr == pWidget)
    {
        return ;
    }
    std::string widgetname = pWidget->getName();
    
    if (Widget::TouchEventType::ENDED == tType)
    {
        INSTANCE(AudioMgr)->playClickAudio();
        if (0 == strcmp("openbank_btn", widgetname.c_str()))
        {
            //密码检查
            std::string ps = m_fieldPasswd->getText();
            std::string su = m_fieldSure->getText();
            if (m_notify)
            {
                if (!m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::PASSWD_CHECK, &ps, (int)ps.length()))
                {
                    return;
                }
                if (!m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::SUREPASSWD_CHECK, &su, (int)ps.length()))
                {
                    return;
                }
                
                //密码对比
                if (0 == strcmp(ps.c_str(), su.c_str()))
                {
                    m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::OPEN_BANK, &ps, (int)ps.length());
                }
                else
                {
                    HallDataMgr::getInstance()->AddpopLayer("温馨提示", "两次输入的密码不一致", Type_Ensure);
                }
            }
        }
    }
}