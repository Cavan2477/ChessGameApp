//
//  BankSendLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#include "BankSendLayer.h"
#include "HallDataMgr.h"
#include "ModeLayer.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

BankSendLayer::BankSendLayer():
m_notify(nullptr)
{
    m_group = new ButtonGroup();
    m_group->setGroupListen(this);
    m_group->setGroupName("send_group");
}

BankSendLayer::~BankSendLayer()
{
    m_group->removeFromGroup("id_check");
    m_group->removeFromGroup("nickname_check");
    delete m_group;
    m_group = nullptr;
}

bool BankSendLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("bank/BankSendLayer.csb");
        
        m_textSave = static_cast<Text*>(m_root->getChildByName("save_text"));
        CC_ASSERT(m_textSave != nullptr);
        m_textSave->setString("");
        
        auto tmp = m_root->getChildByName("bank_user_bg");
        CC_ASSERT(tmp != nullptr);
        m_fieldGetUser = ui::EditBox::create(tmp->getBoundingBox().size, "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldGetUser != nullptr);
        m_fieldGetUser->setFontName("Arial");
        m_fieldGetUser->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldGetUser->setPlaceholderFontName("Arial");
        m_fieldGetUser->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldGetUser->setPosition(tmp->getPosition());
        m_fieldGetUser->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        m_fieldGetUser->setDelegate(this);
        m_root->addChild(m_fieldGetUser);
        
        tmp = m_root->getChildByName("bank_num_bg");
        CC_ASSERT(tmp != nullptr);
        m_fieldSendNumber = ui::EditBox::create(tmp->getBoundingBox().size, "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldSendNumber != nullptr);
        m_fieldSendNumber->setPosition(tmp->getPosition());
        m_fieldSendNumber->setFontName("Arial");
        m_fieldSendNumber->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldSendNumber->setPlaceholderFontName("Arial");
        m_fieldSendNumber->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldSendNumber->setPlaceHolder("请输入赠送金额");
        m_fieldSendNumber->setDelegate(this);
        m_fieldSendNumber->setInputMode(ui::EditBox::InputMode::NUMERIC);
        m_root->addChild(m_fieldSendNumber);
        
        m_fieldPasswd = ui::EditBox::create(Size(520.0f, 50.0f), "blank.png", Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldPasswd != nullptr);
        tmp = m_root->getChildByName("bank_pw_bg");
        CC_ASSERT(tmp != nullptr);
        m_fieldPasswd->setPosition(tmp->getPosition());
        m_fieldPasswd->setFontName("Arial");
        m_fieldPasswd->setFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldPasswd->setPlaceHolder("请输入银行密码");
        m_fieldPasswd->setPlaceholderFontName("Arial");
        m_fieldPasswd->setPlaceholderFontSize(tmp->getBoundingBox().size.height * 2 / 3);
        m_fieldPasswd->setDelegate(this);
        m_fieldPasswd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        m_fieldPasswd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        m_root->addChild(m_fieldPasswd);
        
        m_checkByID = static_cast<CheckBox*>(m_root->getChildByName("id_check"));
        CC_ASSERT(m_checkByID != nullptr);
        m_group->addToGroup(m_checkByID, "id_check");
        
        m_checkByNick = static_cast<CheckBox*>(m_root->getChildByName("nickname_check"));
        CC_ASSERT(m_checkByNick != nullptr);
        m_group->addToGroup(m_checkByNick, "nickname_check");
        m_group->setSelecte("id_check");
        
        //确认赠送
        auto csbBtn = static_cast<Button*>(m_root->getChildByName("send_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(BankSendLayer::touchEvent, this));
        
        bRes = true;
    } while (false);
    return bRes;
}

bool BankSendLayer::preButtonChecked(cocos2d::Node *pSender, bool bSelected)
{
    return true;
}

void BankSendLayer::onButtonChecked(cocos2d::Node *pSender)
{
    std::string btnname = pSender->getName();
    log("on: check :%s,select",btnname.c_str());
}

void BankSendLayer::onCheckStatuChange(cocos2d::Node *pSender, bool bSelected)
{
    std::string btnname = pSender->getName();
    log("change: check :%s,select",btnname.c_str());
    
    if (0 == strcmp(btnname.c_str(), "id_check"))
    {
        switchSendByID(bSelected);
    }
    else if (0 == strcmp(btnname.c_str(), "nickname_check"))
    {
        switchSendByNick(bSelected);
    }
}

void BankSendLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    bool bEnable = false;
    this->stopAllActions();
    if (m_notify)
    {
        m_notify->uiMsg(UI_MSG_BASE::UI_MSG_COMMON, COMMON_UI::ENABLE_BTN, &bEnable, sizeof(bool));
    }
}

void BankSendLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
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

void BankSendLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void BankSendLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void BankSendLayer::refreshScore()
{
    std::string insuerstring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserInsure)->getCString();
    size_t len = insuerstring.length();
    for (int i = (int)len - 3; i > 0; i -= 3)
    {
        insuerstring.insert(i, ",");
    }
    insuerstring = "当前存款:" + insuerstring;
    if (insuerstring.length() > 30)
    {
        insuerstring = insuerstring.substr(0,30);
        insuerstring.append("...");
    }
    m_textSave->setString(insuerstring);
}

void BankSendLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
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
        if (0 == strcmp("send_btn", widgetname.c_str()))
        {
            if (m_notify)
            {
                //赠送用户
                std::string user = m_fieldGetUser->getText();
                if (user.empty())
                {
                    HallDataMgr::getInstance()->AddpopLayer("提示", "赠送玩家不能为空", Type_Ensure);
                    return;
                }
                if (strLength(user)  > LEN_ACCOUNTS-1)
                {
                    auto title = 1 == m_sendMode ? "输入昵称过长，请重新输入":"输入ID过长，请重新输入";
                    HallDataMgr::getInstance()->AddpopLayer("提示", title, Type_Ensure);
                    return;
                }
                //赠送金额
                std::string scorestr = m_fieldSendNumber->getText();
                if (!scorestr.size())
                {
                    HallDataMgr::getInstance()->AddpopLayer("提示", "请输入赠送金额", Type_Ensure);
                    return;
                }
                
                //银行密码
                std::string ps = m_fieldPasswd->getText();
                if (!m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, BANK_UI::PASSWD_CHECK, &ps, (int)ps.length()))
                {
                    return ;
                }
                SCORE score;
                sscanf(scorestr.c_str(), "%lld", &score);
                
                BANK_UI subCode = 1 == m_sendMode ? BANK_UI::SEND_GOLD_NICK : BANK_UI::SEND_GOLD_ID;
                std::string mestr = 1 == m_sendMode ? "昵称为:" : "ID为:";
                char buf[1024] = "";
                memset(buf, 0, 1024);
                sprintf(buf, "是否赠送%lld游戏币给%s%s的用户",score,mestr.c_str(),user.c_str());
                auto player = static_cast<ModeLayer*>(HallDataMgr::getInstance()->AddpopLayer("温馨提示", buf, Type_Ensure_Cancel));
                player->setEnsureCallback([this,subCode,score,user,ps]()
                                          {
                                              tagSendParam param = {};
                                              param.sendScore = score;
                                              param.targetUser = user;
                                              param.passwd = ps;
                                              
                                              m_notify->uiMsg(UI_MSG_BASE::UI_MSG_BANK, subCode, &param, sizeof(tagSendParam));
                                          });
            }
        }
    }
}

void BankSendLayer::switchSendByID(bool bSelect)
{
    if (bSelect)
    {
        m_fieldGetUser->setPlaceHolder("请输入接收ID");
        m_sendMode = 0;
    }
}

void BankSendLayer::switchSendByNick(bool bSelect)
{
    if (bSelect)
    {
        m_fieldGetUser->setPlaceHolder("请输入接收昵称");
        
        m_sendMode = 1;
    }
}