//
//  BankSaveLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#include "BankSaveLayer.h"
#include "HallDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;
BankSaveLayer::BankSaveLayer():
m_notify(nullptr)
{
    
}

BankSaveLayer::~BankSaveLayer()
{
    
}

bool BankSaveLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("bank/BankSaveLayer.csb");
        
        m_textGold = static_cast<Text*>(m_root->getChildByName("gold_text"));
        CC_ASSERT(m_textGold != nullptr);
        m_textGold->setString("");
        
        m_textSave = static_cast<Text*>(m_root->getChildByName("save_text"));
        CC_ASSERT(m_textSave != nullptr);
        m_textSave->setString("");
        
        auto tmp = m_root->getChildByName("bank_num_bg");
        CC_ASSERT(tmp != nullptr);
        m_fieldNumber = ui::EditBox::create(tmp->getBoundingBox().size, "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldNumber != nullptr);
        m_fieldNumber->setPlaceholderFontName("Arial");
        m_fieldNumber->setFontName("Arial");
        m_fieldNumber->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldNumber->setPlaceHolder("请输入存取款金额");
        m_fieldNumber->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldNumber->setPosition(tmp->getPosition());
        m_fieldNumber->setInputMode(ui::EditBox::InputMode::NUMERIC);
        m_fieldNumber->setDelegate(this);
        m_root->addChild(m_fieldNumber);
        
        tmp = m_root->getChildByName("bank_pw_bg");
        CC_ASSERT(tmp != nullptr);
        m_fieldPasswd = ui::EditBox::create(tmp->getBoundingBox().size, "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldPasswd != nullptr);
        m_fieldPasswd->setPlaceholderFontName("Arial");
        m_fieldPasswd->setFontName("Arial");
        m_fieldPasswd->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldPasswd->setPlaceHolder("请输入6-32位字符或数字");
        m_fieldPasswd->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldPasswd->setPosition(tmp->getPosition());
        m_fieldPasswd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        m_fieldPasswd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        m_fieldPasswd->setDelegate(this);
        m_root->addChild(m_fieldPasswd);
        
        Button *csbBtn = static_cast<Button*>(m_root->getChildByName("save_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(BankSaveLayer::touchEvent, this));
        
        csbBtn = static_cast<Button*>(m_root->getChildByName("out_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(BankSaveLayer::touchEvent, this));
        bRes = true;
    } while (false);
    return bRes;
}

void BankSaveLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    bool bEnable = false;
    this->stopAllActions();
    if (m_notify)
    {
        m_notify->uiMsg(UI_MSG_BASE::UI_MSG_COMMON, COMMON_UI::ENABLE_BTN, &bEnable, sizeof(bool));
    }
}

void BankSaveLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto seq = Sequence::create(DelayTime::create(0.5f), CallFunc::create([this]()
                                                                          {
                                                                              bool bEnable = true;
                                                                              if (m_notify)
                                                                              {
                                                                                  m_notify->uiMsg(UI_MSG_BASE::UI_MSG_COMMON, COMMON_UI::ENABLE_BTN, &bEnable, sizeof(bool));
                                                                              }
                                                                          }), NULL);
    this->stopAllActions();
    this->runAction(seq);
#else
    bool bEnable = true;
    if (m_notify)
    {
        m_notify->uiMsg(UI_MSG_BASE::UI_MSG_COMMON, COMMON_UI::ENABLE_BTN, &bEnable, sizeof(bool));
    }
#endif
}

void BankSaveLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void BankSaveLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void BankSaveLayer::refreshScore()
{
    std::string scorestring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString();
    size_t len = scorestring.length();
    for (int i = (int)len - 3; i > 0; i -= 3)
    {
        scorestring.insert(i, ",");
    }
    scorestring = "$:" + scorestring;
    if (scorestring.length() > 14)
    {
        scorestring = scorestring.substr(0,14);
        scorestring.append("...");
    }
    m_textGold->setString(scorestring);
    
    std::string insuerstring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserInsure)->getCString();
    len = insuerstring.length();
    for (int i = (int)len - 3; i > 0; i -= 3)
    {
        insuerstring.insert(i, ",");
    }
    insuerstring = "存款:" + insuerstring;
    if (insuerstring.length() > 16)
    {
        insuerstring = insuerstring.substr(0,16);
        insuerstring.append("...");
    }
    m_textSave->setString(insuerstring);
}

void BankSaveLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    Widget *pWidget = (Widget*)pSender;
    if (nullptr == pWidget)
    {
        return;
    }
    
    std::string widgetname = pWidget->getName();
    if (Widget::TouchEventType::ENDED == tType)
    {
        INSTANCE(AudioMgr)->playClickAudio();
        std::string numstr = m_fieldNumber->getText();
        
        if (0 == strcmp("save_btn", widgetname.c_str()))
        {
            if (!numstr.size())
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "请输入存入金额", Type_Ensure);
                return;
            }
            SCORE score;
            sscanf(numstr.c_str(), "%lld", &score);
            if (score == 0)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "存款不能为0，请重新输入", Type_Ensure);
                return;
            }
            if (score > HallDataMgr::getInstance()->m_UserScore)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "存款不能大于你的金币值", Type_Ensure);
                return;
            }
            tagSaveParam param = {};
            param.score = score;
            param.passwd = "";
            if (m_notify)
            {
                m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::SAVE_GOLD, &param, sizeof(tagSaveParam));
            }
        }
        else if (0 == strcmp("out_btn", widgetname.c_str()))
        {
            if (!numstr.size())
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "请输入取出金额", Type_Ensure);
                return;
            }
            SCORE score;
            sscanf(numstr.c_str(), "%lld", &score);
            if (score == 0)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "取款不能为0，请重新输入", Type_Ensure);
                return;
            }
            if (score > HallDataMgr::getInstance()->m_UserInsure)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "取款不能大于你的银行金币值", Type_Ensure);
                return;
            }
            std::string passwd = m_fieldPasswd->getText();
            if (m_notify)
            {
                if (!m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::PASSWD_CHECK, &passwd, (int)passwd.length()))
                {
                    return;
                }
                tagSaveParam param = {};
                param.score = score;
                param.passwd = passwd;
                m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::OUT_GOLD, &param, sizeof(tagSaveParam));
            }
        }
    }
}