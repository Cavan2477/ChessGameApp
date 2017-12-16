//
//  PasswdLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/22/15.
//
//

#include "PasswdLayer.h"
#include "ModeLayer.h"
#include "UtilEx.h"
#include "GameDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_NN_NAMESPACE;

PasswdLayer::PasswdLayer():
m_notify(nullptr)
{
    
}

PasswdLayer::~PasswdLayer()
{
    
}

bool PasswdLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("plaza/PasswdLayer.csb");
        
        //确定按钮
        Button *csbBtn = static_cast<Button*>(m_root->getChildByName("sure_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(PasswdLayer::touchEvent,this));
        
        //关闭按钮
        csbBtn = static_cast<Button*>(m_root->getChildByName("close_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(PasswdLayer::touchEvent,this));
        
        //提示文本
        m_textNotice = static_cast<Text*>(m_root->getChildByName("notice_text"));
        CC_ASSERT(m_textNotice != nullptr);
        
        //输入
        m_fieldInput = ui::EditBox::create(Size(238,42), "blank.png",Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldInput != nullptr);
        auto sp = m_root->getChildByName("text_bg_rect");
        CC_ASSERT(sp != nullptr);
        m_fieldInput->setPosition(sp->getPosition());
        m_fieldInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        m_fieldInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        m_fieldInput->setDelegate(this);
        m_fieldInput->setFontColor(Color3B::BLACK);
        m_fieldInput->setPlaceHolder("请输入密码:");
        m_root->addChild(m_fieldInput);
        
        bRes = true;
        
        showPasswd(false);
    } while (false);
    return bRes;
}

void PasswdLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
    
}

void PasswdLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}

void PasswdLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void PasswdLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void PasswdLayer::showPasswd(bool bShow)
{
    this->setVisible(bShow);
}

void PasswdLayer::refreshNotice(const std::string &notice,const emPasswdType &passwdType)
{
    m_textNotice->setString(notice);
    m_emPasswdType = passwdType;
}

void PasswdLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    Node *pNode = static_cast<Node*>(pSender);
    if (nullptr == pNode)
    {
        return;
    }
    
    std::string name = pNode->getName();
    if (Widget::TouchEventType::ENDED == type)
    {
        if ( 0 == strcmp("sure_btn", name.c_str()))
        {
            std::string passstr = m_fieldInput->getText();
            
            if (strLength(passstr) > LEN_PASSWORD - 1) {
                HallDataMgr::getInstance()->AddpopLayer("提示", "密码过长，请重新输入", Type_Ensure);
                return;
            }
            if (passstr.find(" ") != std::string::npos) {
                HallDataMgr::getInstance()->AddpopLayer("提示", "密码中不能有空格，请重新输入", Type_Ensure);
                return;
            }
            this->showPasswd(false);
            switch (m_emPasswdType)
            {
                case kENTER_LIANXIROOM:     //练习房间密码
                {
                    if ("" == passstr)
                    {
                        HallDataMgr::getInstance()->AddpopLayer("系统提示", "密码不能为空", Type_Ensure);
                        return;
                    }
                    INSTANCE(GameDataMgr)->enterRoom(passstr);
                }
                    break;
                case kENTER_ANTICHEAT:      //防作弊
                {
                    if ("" == passstr)
                    {
                        HallDataMgr::getInstance()->AddpopLayer("系统提示", "密码不能为空", Type_Ensure);
                        return;
                    }
                    INSTANCE(GameDataMgr)->enterRoom(passstr);
                }
                    break;
                case kSET_PASSWD:           //设置桌子密码
                {
                    NetworkMgr::getInstance()->sendEncrypt(passstr.c_str());
                    HallDataMgr::getInstance()->m_Tablepass = passstr;
                    HallDataMgr::getInstance()->saveTablepass(passstr);
                }
                    break;
                case kINPUT_TABLE_PASS:
                {
                    if ("" == passstr)
                    {
                        HallDataMgr::getInstance()->AddpopLayer("系统提示", "密码不能为空", Type_Ensure);
                        return;
                    }
                    WORD wTable = INSTANCE(GameDataMgr)->getSitTable();
                    WORD wChair = INSTANCE(GameDataMgr)->getSitChair();
                    INSTANCE(GameDataMgr)->requestSitdown(wTable, wChair,passstr);
                    HallDataMgr::getInstance()->AddpopLayer("", "正在进入游戏。。。。", Type_Wait_Text);
                }
                    break;
                default:
                    break;
            }
            showPasswd(false);
        }
        else if (0 == strcmp("close_btn", name.c_str()))
        {
            if (kENTER_LIANXIROOM == m_emPasswdType || kENTER_ANTICHEAT == m_emPasswdType)
            {
                NetworkMgr::getInstance()->Disconnect(Data_Room);
                HallDataMgr::getInstance()->m_RoomType = Data_Load;
            }
            showPasswd(false);
        }
    }
}