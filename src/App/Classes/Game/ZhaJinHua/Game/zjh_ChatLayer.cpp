#include "zjh_ChatLayer.h"
#include "define.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "CMD_GameServer.h"
#include "lkpy_Help.h"
#include "NetworkMgr.h"
#include "GameDataMgr.h"
#include "zjh_GameLayer.h"
#include "SimpleAudioEngine.h"
#include "HallDataMgr.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
USING_ZJH_NAMESPACE;
enum ButtonTag
{
    Tag_BT_Change_Texts,
    Tag_BT_Change_Brows,
    Tag_BT_Change_Records,
    Tag_BT_Send_Texts,
    Tag_BT_Send_Brows,
    Tag_BT_Send_Records,
    Tag_BT_Send_Chat,
    Tag_BT_Send_Exit,
    Tag_UserName = 1000,//聊天记录 玩家昵称
    Tag_Content = 2000,//聊天记录 聊天内容
    Tag_Line = 3000,//聊天记录 下划线
    
};

bool ChatLayer::init()
{
    if(!Layer::init())
        return false;
    m_layout= static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/chatLayout.json"));
    this->addChild(m_layout);
    
    ImageView* background = (ui::ImageView*)Helper::seekWidgetByName(m_layout, "background");
    
    
    Button* exitBtn = (ui::Button*)Helper::seekWidgetByName(background, "exitBtn");
    exitBtn->setTag(Tag_BT_Send_Exit);
    exitBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
    
    Button* sendChatBtn = (ui::Button*)Helper::seekWidgetByName(background, "sendChatBtn");
    sendChatBtn->setTag(Tag_BT_Send_Chat);
    sendChatBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
    
    m_inputEditBox = EditBox::create(Size(370,49), "studioUi/chat/chat_textField.png");
    m_inputEditBox->setPosition(Point(sendChatBtn->getPositionX()-sendChatBtn->getContentSize().width/2 - m_inputEditBox->getContentSize().width/2 -5,sendChatBtn->getPositionY()));
    m_inputEditBox->setFontSize(22);
    m_inputEditBox->setPlaceHolder("请输入聊天内容");
    m_inputEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_inputEditBox->setMaxLength(42);
    background->addChild(m_inputEditBox);
    
    m_textScrollView = (ui::ScrollView*)Helper::seekWidgetByName(background, "textScrollView");
    Vector<Node*> textChildren = m_textScrollView->getChildren();
    
    for(int i = 0;i < textChildren.size();++i)
    {
        Button* textBtn = (ui::Button*)textChildren.at(i);
        textBtn->setTag(Tag_BT_Send_Texts);
        textBtn->setZOrder(i);
        textBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
    }
    
    m_browScrollView = (ui::ScrollView*)Helper::seekWidgetByName(background, "browScrollView");
    m_browScrollView->setVisible(false);
    
    int countY = 0;
    int countX = 0;
    for(int i = 0;i < 80;++i)
    {
        
        Button* browBtn = Button::create(__String::createWithFormat("studioUi/brow/%d.png",i)->getCString());
        countY = i / 6;
        countX = i % 6;
        browBtn->setPosition(Point(countX * (browBtn->getContentSize().width+35)+52,
                             m_browScrollView->getContentSize().height- countY*(browBtn->getContentSize().height+13) +630));
        
        browBtn->setTag(Tag_BT_Send_Brows);
        browBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
        
        m_browScrollView->addChild(browBtn,i);
    }
    
    m_recordScrollView = (ui::ScrollView*)Helper::seekWidgetByName(background, "recordScrollView");
    m_recordScrollView->setVisible(false);
    m_textsBtn = (ui::Button*)Helper::seekWidgetByName(background, "textsBtn");
    m_textsBtn->setTag(Tag_BT_Change_Texts);
    m_textsBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
    
    m_browBtn = (ui::Button*)Helper::seekWidgetByName(background, "browBtn");
    m_browBtn->setTag(Tag_BT_Change_Brows);
    m_browBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
    
    m_recordBtn = (ui::Button*)Helper::seekWidgetByName(background, "recordBtn");
    m_recordBtn->setTag(Tag_BT_Change_Records);
    m_recordBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent, this));
    
    return true;
}

void ChatLayer::initRecordUi()
{
    GameLayer* gameLayer = (GameLayer*)this->getParent();
    //gameLayer
    int nRecordCount = gameLayer->m_recordUserVe.size();
    int nPadding = 39;
    //nRecordCount= 66;
    int totalHeight = 10;
    float fAddPosY = 0;
    float innerHeight = 0;
    
    innerHeight = m_recordScrollView->getInnerContainerSize().height;
    float posNameY = 0;m_recordScrollView->getInnerContainerSize().height- nPadding +12;
    
    for(int i = 0; i < 2; ++i)
    {
        if(i == 1)
            posNameY = m_recordScrollView->getInnerContainerSize().height- nPadding +12;
        for(int index = nRecordCount - 1;index >= 0;--index)
        {
            Text* pUserName = (Text*)m_recordScrollView->getChildByTag(Tag_UserName + index);
            Size nameSize;
            if(!pUserName)
            {
                pUserName = Text::create(__String::createWithFormat("%s:",gameLayer->m_recordUserVe[index].c_str())->getCString(), FontNormal, 22);
                nameSize = pUserName->getContentSize();
                pUserName->setColor(Color3B::RED);
                pUserName->setAnchorPoint(Point(0,0.5f));
                
                m_recordScrollView->addChild(pUserName,1,Tag_UserName + index);
                totalHeight += nameSize.height;
            }
            else
            {
                nameSize = pUserName->getContentSize();
                pUserName->setPosition(Point(10,posNameY));
            }
            
            //+ nRecordCount+8;
            
            float posLineY = 0;
            
            if(gameLayer->m_recordTypeVe[index] == 0)
            {
                Label* contentText = (Label*)m_recordScrollView->getChildByTag(Tag_Content + index);
                if(!contentText)
                {
                    contentText = Label::create(__String::createWithFormat("%s",gameLayer->m_recordContentVe[index].c_str())->getCString(), FontNormal, 22);
                    contentText->setDimensions(500,0);
                    
                    //contentText->setColor(Color3B(0,0,0));
                    contentText->setAnchorPoint(Point(0,0.5f));
                    
                    m_recordScrollView->addChild(contentText,1,Tag_Content + index);
                    totalHeight += (13 + contentText->getContentSize().height);
                }
                else
                {
                    contentText->setPosition(Point(10,posNameY - nameSize.height/2 - 18));
                    posLineY = contentText->getPositionY() - contentText->getContentSize().height/2 -8;
                }
                
            }
            else if (gameLayer->m_recordTypeVe[index] == 1)
            {
                Sprite* pBrow = (Sprite*)m_recordScrollView->getChildByTag(Tag_Content + index);
                if(!pBrow)
                {
                    pBrow = Sprite::createWithSpriteFrameName(__String::createWithFormat("brow%s.png",gameLayer->m_recordContentVe[index].c_str())->getCString());
                    
                    pBrow->setScale(0.75f);
                    pBrow->setAnchorPoint(Point(0,0.5f));
                    
                    m_recordScrollView->addChild(pBrow,1,Tag_Content + index);
                    totalHeight += (2 + pBrow->getContentSize().height);
                }
                else
                {
                    pBrow->setPosition(Point(10,posNameY - nameSize.height/2 - 18));
                    posLineY = pBrow->getPositionY() - pBrow->getContentSize().height/2 -1;
                }
            }
            
            Sprite*  line = (Sprite*)m_recordScrollView->getChildByTag(Tag_Line + index);
            if(!line)
            {
                line = Sprite::create("studioUi/chat/line.png");
                m_recordScrollView->addChild(line,1,Tag_Line + index);
            }
            else
            {
                line->setPosition(Point(m_recordScrollView->getContentSize().width/2,
                                        posLineY));
            }
           // m_recordScrollView->getInnerContainerSize().height- (nRecordCount - index)*nPadding +12 -19; //+ nRecordCount -11;
            totalHeight += (line->getContentSize().height + 3);
            if(i == 1)
            posNameY = posLineY - 16;
        }
        if(i == 0 && totalHeight > m_recordScrollView->getContentSize().height)
        {
            innerHeight = nRecordCount*nPadding +10;
            m_recordScrollView->setInnerContainerSize(Size(m_recordScrollView->getContentSize().width,totalHeight));
        }

    }
   
}

void ChatLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto pbutton = (Button *)pSender;
        auto ntag = pbutton->getTag();
        switch (ntag) {
            case Tag_BT_Change_Texts:
            {
                m_textsBtn->loadTextures("studioUi/chat/chat_texts_0.png", "studioUi/chat/chat_texts_0.png");
                m_browBtn->loadTextures("studioUi/chat/chat_brow_1.png", "studioUi/chat/chat_brow_1.png");
                m_recordBtn->loadTextures("studioUi/chat/chat_record_1.png", "studioUi/chat/chat_record_1.png");
                m_textScrollView->setVisible(true);
                m_browScrollView->setVisible(false);
                m_recordScrollView->setVisible(false);
                
                
            }
                break;
            case Tag_BT_Change_Brows:
            {
                m_textsBtn->loadTextures("studioUi/chat/chat_texts_1.png", "studioUi/chat/chat_texts_1.png");
                m_browBtn->loadTextures("studioUi/chat/chat_brow_0.png", "studioUi/chat/chat_brow_0.png");
                m_recordBtn->loadTextures("studioUi/chat/chat_record_1.png", "studioUi/chat/chat_record_1.png");
                m_browScrollView->setVisible(true);
                m_textScrollView->setVisible(false);
                m_recordScrollView->setVisible(false);
            }
                break;
            case Tag_BT_Change_Records:
            {
                m_textsBtn->loadTextures("studioUi/chat/chat_texts_1.png", "studioUi/chat/chat_texts_1.png");
                m_browBtn->loadTextures("studioUi/chat/chat_brow_1.png", "studioUi/chat/chat_brow_1.png");
                m_recordBtn->loadTextures("studioUi/chat/chat_record_0.png", "studioUi/chat/chat_record_0.png");
                m_recordScrollView->setVisible(true);
                m_textScrollView->setVisible(false);
                m_browScrollView->setVisible(false);
            }
                break;
            case Tag_BT_Send_Texts:
            {
                auto textLabel = (ui::Text*)Helper::seekWidgetByName(pbutton, "textLabel");
                string strText = textLabel->getString();
                
                int length = lkpy_game::getStringLength(strText);

                if (length > (128/3)) {
                    HallDataMgr::getInstance()->AddpopLayer("系统提示", "内容长度不符合规范,请输入0～128位字符", Type_Ensure);
                    return;
                }
                CMD_GF_C_UserChat cmd_gf_cuc;
                cmd_gf_cuc.wChatLength = strText.length();
                cmd_gf_cuc.dwChatColor = 255+(255<<8)+(255<<16);
                cmd_gf_cuc.dwTargerUserID = INVALID_USERID;
                memset(cmd_gf_cuc.szChatString, 0, sizeof(cmd_gf_cuc.szChatString));
                UTF8Str_To_UTF16Str_BYTE((BYTE *)strText.c_str(), cmd_gf_cuc.szChatString);
                int nSize = sizeof(cmd_gf_cuc)-sizeof(cmd_gf_cuc.szChatString)+cmd_gf_cuc.wChatLength*2;
                NetworkMgr::getInstance()->sendData(MDM_GF_FRAME, SUB_GF_USER_CHAT, &cmd_gf_cuc, nSize);
                this->removeFromParentAndCleanup(true);
                int zOrder = pbutton->getZOrder();
                string effectName = __String::createWithFormat("sound_res/speak_%d_%d.wav",!HallDataMgr::getInstance()->m_cbGender+1,zOrder)->getCString();
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectName.c_str());
                //m_inputEditBox->setText("");
            }
                break;
            case Tag_BT_Send_Brows:
            {
                int zorder = pbutton->getZOrder();
                CMD_GF_C_UserExpression cmd_gf_cue;
                cmd_gf_cue.dwTargerUserID = INVALID_USERID;
                cmd_gf_cue.wItemIndex = zorder;
                NetworkMgr::getInstance()->sendData(MDM_GF_FRAME, SUB_GF_USER_EXPRESSION, &cmd_gf_cue, sizeof(cmd_gf_cue));
                this->removeFromParentAndCleanup(true);
                
            }
                break;
            case Tag_BT_Send_Records:
            {
                
            }
                break;
            case Tag_BT_Send_Chat:
            {
                auto textLabel = (ui::Text*)Helper::seekWidgetByName(pbutton, "textLabel");
                string strText = m_inputEditBox->getText();
                
                int length = lkpy_game::getStringLength(strText);

                if (length > (128/3)) {
                    HallDataMgr::getInstance()->AddpopLayer("系统提示", "内容长度不符合规范,请输入0～128位字符", Type_Ensure);
                    return;
                }
                
                CMD_GF_C_UserChat cmd_gf_cuc;
                cmd_gf_cuc.wChatLength = strText.length();
                cmd_gf_cuc.dwChatColor = 255+(255<<8)+(255<<16);
                cmd_gf_cuc.dwTargerUserID = INVALID_USERID;
                memset(cmd_gf_cuc.szChatString, 0, sizeof(cmd_gf_cuc.szChatString));
                UTF8Str_To_UTF16Str_BYTE((BYTE *)strText.c_str(), cmd_gf_cuc.szChatString);
                int nSize = sizeof(cmd_gf_cuc)-sizeof(cmd_gf_cuc.szChatString)+cmd_gf_cuc.wChatLength*2;
                NetworkMgr::getInstance()->sendData(MDM_GF_FRAME, SUB_GF_USER_CHAT, &cmd_gf_cuc, nSize);
                this->removeFromParentAndCleanup(true);
                m_inputEditBox->setText("");
            }
                break;
            case Tag_BT_Send_Exit:
            {
                this->removeFromParentAndCleanup(true);
            }
                break;
        }
    }

}

void ChatLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

