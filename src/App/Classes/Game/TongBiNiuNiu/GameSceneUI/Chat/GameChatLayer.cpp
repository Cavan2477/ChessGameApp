//
//  GameChatLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#include "GameChatLayer.h"
#include "GameDataMgr.h"
#include "UtilEx.h"
#include "HallDataMgr.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif

USING_NS_CC;
using namespace ui;
using namespace std;
USING_TBNN_NAMESPACE;

const int TAG_TEXT_TABLEVIEW = 100;
const int TAG_RECORD_TABLEVIEW = 101;
const int TAG_BROW_TABLEVIEW = 102;

GameChatLayer::GameChatLayer():
    m_textChat(nullptr),
    m_browChat(nullptr),
    m_recordChat(nullptr),
    m_notify(nullptr)
{
    m_group = new ButtonGroup();
    m_group->setGroupListen(this);
    m_group->setGroupName("chat_group");
}

GameChatLayer::~GameChatLayer()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //closeEditTextDialogJNI();
#endif
    m_group->removeFromGroup("text_check");
    m_group->removeFromGroup("brow_check");
    m_group->removeFromGroup("record_check");
    delete m_group;
    m_group = nullptr;
    
    m_mapBrowItem.clear();
    m_mapRecordItem.clear();
    m_mapTextItem.clear();
}

bool GameChatLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        //TODO
        //LAYER_LOAD("UILayerReader",UILayerReader,"GameChatLayer.csb");
        LOAD_CSB("game/GameChatLayer.csb");
        
        m_spChatArea = static_cast<Sprite*>(m_root->getChildByName("chat_area"));
        CC_ASSERT(m_spChatArea != nullptr);
        
        m_checkText = static_cast<CheckBox*>(m_root->getChildByName("text_check"));
        CC_ASSERT(m_checkText != nullptr);
        m_group->addToGroup(m_checkText, "text_check");        
        
        m_checkBrow = static_cast<CheckBox*>(m_root->getChildByName("brow_check"));
        CC_ASSERT(m_checkBrow != nullptr);
        m_group->addToGroup(m_checkBrow, "brow_check");
        
        m_checkRecord = static_cast<CheckBox*>(m_root->getChildByName("record_check"));
        CC_ASSERT(m_checkRecord != nullptr);
        m_group->addToGroup(m_checkRecord, "record_check");
        
        m_group->setSelecte("text_check");
        
        m_btnSend = static_cast<Button*>(m_root->getChildByName("send_btn"));
        CC_ASSERT(m_btnSend != nullptr);
        m_btnSend->addTouchEventListener(CC_CALLBACK_2(GameChatLayer::touchEvent, this));
        
        m_btnSend = static_cast<Button*>(m_root->getChildByName("close_btn"));
        CC_ASSERT(m_btnSend != nullptr);
        m_btnSend->addTouchEventListener(CC_CALLBACK_2(GameChatLayer::touchEvent, this));
        
        m_fieldChat = ui::EditBox::create(Size(370.0f, 50.0f), "blank.png",Widget::TextureResType::PLIST);
        CC_ASSERT(m_fieldChat != nullptr);
        m_fieldChat->setDelegate(this);
        m_fieldChat->setPlaceHolder("请输入聊天信息");
        m_fieldChat->setPosition(Vec2(488.0f, 173.0f));
        m_root->addChild(m_fieldChat);
        m_fieldChat->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        
        this->setVisible(false);
        bRes = true;
    } while (false);
    return bRes;
}

bool GameChatLayer::preButtonChecked(cocos2d::Node *pSender, bool bSelected)
{
    return true;
}

void GameChatLayer::onButtonChecked(cocos2d::Node *pSender)
{
    std::string btnname = pSender->getName();
    log("on: check :%s,select",btnname.c_str());
    
    if (0 == strcmp(btnname.c_str(), "text_check"))
    {
        
    }
    else if (0 == strcmp(btnname.c_str(), "brow_check"))
    {
        
    }
    else if (0 == strcmp(btnname.c_str(), "record_check"))
    {
        
    }
}

void GameChatLayer::onCheckStatuChange(cocos2d::Node *pSender, bool bSelected)
{
    std::string btnname = pSender->getName();
    log("change: check :%s,select",btnname.c_str());
    
    if (0 == strcmp(btnname.c_str(), "text_check"))
    {
        showTextChat(bSelected);
    }
    else if (0 == strcmp(btnname.c_str(), "brow_check"))
    {
        showBrowChat(bSelected);
    }
    else if (0 == strcmp(btnname.c_str(), "record_check"))
    {
        showRecord(bSelected);
    }
}

cocos2d::Node *GameChatLayer::getDataItemAt(ChatTableView *chatTable, const size_t &idx)
{
    log("get idx %zu",idx);
    int nTag = chatTable->getTag();
    
    if (TAG_TEXT_TABLEVIEW == nTag)
    {
        ChatItem *pItem  = ChatItem::create();
        return pItem;
        /*
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getStaticTextAt(idx,record))
        {
            ChatItem *pItem = ChatItem::createTextItem(record.strChat);
            Size itemSize = pItem->getContentSize();
            pItem->setPosition(Vec2(itemSize.width * 0.5, 0));
            
            m_mapTextItem.insert(make_pair(idx,pItem));
            return pItem;
        }*/
    }
    else if (TAG_RECORD_TABLEVIEW == nTag)
    {
        ChatItem *pItem  = ChatItem::create();
        return pItem;/*
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getRecordAt(idx,record))
        {
            if (record.bBrow)
            {
                ChatItem *pItem = ChatItem::createBrowItem(record.nIdx,record.dwChatUserId);
                Size itemSize = pItem->getContentSize();
                pItem->setPosition(Vec2(itemSize.width * 0.5, 0));
                
                m_mapRecordItem.insert(make_pair(idx,pItem));
                return pItem;
            }
            else
            {
                ChatItem *pItem = ChatItem::createTextItem(record.strChat,record.dwChatUserId);
                Size itemSize = pItem->getContentSize();
                pItem->setPosition(Vec2(itemSize.width * 0.5, 0));
                
                m_mapRecordItem.insert(make_pair(idx,pItem));
                return pItem;
            }
        }*/
    }
    else if (TAG_BROW_TABLEVIEW == nTag)
    {
        if (1 == idx)
        {
            //表情
            std::vector<int> vecidx = std::vector<int>();
            vecidx.reserve(40);
            for (int i = 1; i < 41; ++i)
            {
                vecidx.push_back(i);
            }
            BrowPanel *panel = BrowPanel::createPanel(vecidx);
            panel->setNotifyNode(this);
            m_mapBrowItem.insert(make_pair(idx, panel));
            return panel;
        }
        else if (0 == idx)
        {
            //表情
            std::vector<int> vecidx = std::vector<int>();
            vecidx.reserve(40);
            for (int i = 41; i < 81; ++i)
            {
                vecidx.push_back(i);
            }
            BrowPanel *panel = BrowPanel::createPanel(vecidx);
            panel->setNotifyNode(this);
            m_mapBrowItem.insert(make_pair(idx, panel));
            return panel;
        }
    }
    return nullptr;
}

void GameChatLayer::refreshDataItemAt(ChatTableView *chatTable, cocos2d::Node *item,const size_t &idx)
{
    log("refresh idx %zu",idx);
    
    ChatItem *pItem = static_cast<ChatItem*>(item);
    if (nullptr == pItem)
    {
        return;
    }
    
    int nTag = chatTable->getTag();
    if (TAG_TEXT_TABLEVIEW == nTag)
    {
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getStaticTextAt(idx,record))
        {
            pItem->refreshTextItem(record.strChat);
            Size itemSize = pItem->getContentSize();
            pItem->setPosition(Vec2(itemSize.width * 0.5, 0));
            
            map<size_t,Node*>::iterator ite = m_mapTextItem.find(idx);
            if (ite != m_mapTextItem.end())
            {
                ite->second = pItem;
            }
            else
            {
                m_mapTextItem.insert(make_pair(idx,pItem));
            }
        }
    }
    else if (TAG_RECORD_TABLEVIEW == nTag)
    {
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getRecordAt(idx,record))
        {
            if (record.bBrow)
            {
                pItem->refreshBrowItem(record.nIdx,record.dwChatUserId);
                Size itemSize = pItem->getContentSize();
                pItem->setPosition(Vec2(itemSize.width * 0.5, 0));
                
                map<size_t,Node*>::iterator ite = m_mapRecordItem.find(idx);
                if (ite != m_mapRecordItem.end())
                {
                    ite->second = pItem;
                }
                else
                {
                    m_mapRecordItem.insert(make_pair(idx,pItem));
                }
            }
            else
            {
                pItem->refreshTextItem(record.strChat,record.dwChatUserId);
                Size itemSize = pItem->getContentSize();
                pItem->setPosition(Vec2(itemSize.width * 0.5, 0));
                
                map<size_t,Node*>::iterator ite = m_mapRecordItem.find(idx);
                if (ite != m_mapRecordItem.end())
                {
                    ite->second = pItem;
                }
                else
                {
                    m_mapRecordItem.insert(make_pair(idx,pItem));
                }
            }
        }
    }
}

size_t GameChatLayer::getDataItemSize(ChatTableView *chatTable)
{
    int nTag = chatTable->getTag();
    if (TAG_TEXT_TABLEVIEW == nTag)
    {
        return INSTANCE(GameDataMgr)->staticRecordCount();
    }
    else if (TAG_RECORD_TABLEVIEW == nTag)
    {
        return INSTANCE(GameDataMgr)->recordCount();
    }
    else if (TAG_BROW_TABLEVIEW == nTag)
    {
        return 2;
    }
    return 0;
}

cocos2d::Size GameChatLayer::getItemSize(ChatTableView *chatTable, const size_t &idx)
{
    int nTag = chatTable->getTag();
    if (TAG_TEXT_TABLEVIEW == nTag)
    {
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getStaticTextAt(idx,record))
        {/*
            auto count = cocos2d::StringUtils::getCharacterCountInUTF8String(record.strChat);
            auto nRows = count / 16;
            nRows = nRows < 1 ? 1 : nRows;
            return Size(530, 28 * nRows + 5);*/
            
            int nTotal = 0;
            CUtilEx::parseUTF8Helper(record.strChat, 28, nTotal);
            if (nTotal > 490)
            {
                int nRows = nTotal / 490 + 1;
                return Size(530, 30 * nRows + 15);
            }
        }
        
        return Size(530, 40);
    }
    else if (TAG_RECORD_TABLEVIEW == nTag)
    {
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getRecordAt(idx,record))
        {
            if (record.bBrow)
            {
                return Size(530,80);
            }
            else
            {
                int nTotal = 0;
                CUtilEx::parseUTF8Helper(record.strChat, 28, nTotal);
                if (nTotal > 490)
                {
                    int nRows = (int)(nTotal / 490) + 2;
                    return Size(530, 30 * nRows + 20);
                }
            }
            
        }
        return Size(530,60);
    }
    else if (TAG_BROW_TABLEVIEW == nTag)
    {
        return Size(500,200);
    }
    return Size::ZERO;
}

void GameChatLayer::onTouchItem(ChatTableView *chatTable, cocos2d::extension::TableViewCell* cell)
{
    if (m_bEditChat)
    {
        m_bEditChat = false;
        return;
    }
    
    int nTag = chatTable->getTag();
    if (TAG_TEXT_TABLEVIEW == nTag)
    {
        int idx = cell->getTag();
        log("touch %d",idx);
        tagChatRecord record;
        if (INSTANCE(GameDataMgr)->getStaticTextAt(idx,record))
        {
            INSTANCE(GameDataMgr)->textChat((int)record.strChat.length(), record.strChat);
            
            INSTANCE(AudioMgr)->playTextAudio(HallDataMgr::getInstance()->m_dwUserID,idx);
            
            if (m_notify)
            {
                m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::TEXT_CHAT, &record.strChat, (int)record.strChat.length());
            }
            tagChatRecord staticrecord = {0};
            staticrecord.dwChatUserId = HallDataMgr::getInstance()->m_dwUserID;
            staticrecord.strChat = record.strChat;
            staticrecord.bBrow = false;
            INSTANCE(GameDataMgr)->addRecord(staticrecord);
        }
        
        m_fieldChat->setText("");
        showGameChat(false);
        m_bEditChat = false;
    }
    else if (TAG_RECORD_TABLEVIEW == nTag)
    {
        
    }
}

bool GameChatLayer::uiMsg(const int &main, const int &sub, void *pData, const int &nLen)
{
    switch (main) {
        case UI_MSG_BASE::UI_MSG_GAME:
            uiGameMsg(sub, pData, nLen);
            break;
            
        default:
            break;
    }
    return true;
}

void GameChatLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    m_bEditChat = true;
    log("begin");
}

void GameChatLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    log("end");
}

void GameChatLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void GameChatLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void GameChatLayer::showGameChat(bool bShow)
{
    this->setVisible(bShow);
    enableBrowItem(bShow);
}

void GameChatLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    Button *pBtn = static_cast<Button*>(pSender);
    if (nullptr == pBtn)
    {
        return;
    }
    
    if (Widget::TouchEventType::ENDED == type)
    {
        INSTANCE(AudioMgr)->playClickAudio();
        string btnName = pBtn->getName();
        if (0 == strcmp(btnName.c_str(), "send_btn"))
        {
            string msg = m_fieldChat->getText();
            if (CUtilEx::getUTF8StringCount(msg.c_str()) > 100)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "聊天内容过长，请重新输入", Type_Ensure);
                return;
            }
            if (!msg.empty())
            {
                //聊天
                INSTANCE(GameDataMgr)->textChat((int)msg.length(), msg);
                
                if (m_notify)
                {
                    m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::TEXT_CHAT, &msg, (int)msg.length());
                }
                tagChatRecord record = {0};
                record.dwChatUserId = HallDataMgr::getInstance()->m_dwUserID;
                record.strChat = msg;
                record.bBrow = false;
                INSTANCE(GameDataMgr)->addRecord(record);
            }
            m_fieldChat->setText("");
            showGameChat(false);
            m_bEditChat = false;
        }
        else if (0 == strcmp(btnName.c_str(), "close_btn"))
        {
            showGameChat(false);
        }
    }
}

void GameChatLayer::showTextChat(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_textChat)
        {
            m_textChat = ChatTableView::create(this, Size(530, 210));
            m_textChat->setTag(TAG_TEXT_TABLEVIEW);
            m_textChat->setPositionY(5.0f);
            m_spChatArea->addChild(m_textChat);
        }
        m_textChat->reload();
    }
    if (nullptr != m_textChat)
    {
        m_textChat->setVisible(bShow);
    }
}

void GameChatLayer::showBrowChat(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_browChat)
        {
            m_browChat = ChatTableView::create(this, Size(480, 200));
            m_browChat->setTag(TAG_BROW_TABLEVIEW);
            m_browChat->setPosition(Vec2(25.0f, 13.0f));
            m_spChatArea->addChild(m_browChat);
        }
        m_browChat->reload();
    }
    if (nullptr != m_browChat)
    {
        m_browChat->setVisible(bShow);
        enableBrowItem(bShow);
    }
}

void GameChatLayer::showRecord(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_recordChat)
        {
            m_recordChat = ChatTableView::create(this, Size(530, 210));
            m_recordChat->setTag(TAG_RECORD_TABLEVIEW);
            m_recordChat->setPositionY(5.0f);
            m_spChatArea->addChild(m_recordChat);
        }
        
        m_recordChat->reload();
    }
    
    if (nullptr != m_recordChat)
    {
        m_recordChat->setVisible(bShow);
    }
}

void GameChatLayer::uiGameMsg(const int &sub, void *pData, const int &nLen)
{
    switch (sub) {
        case GAME_UI::BROW_CHAT:
        {
            if (nullptr != pData && 0 != nLen)
            {                
                int nIdx = *((int*)pData);
                log("face e%d",nIdx);
                INSTANCE(GameDataMgr)->browChat(nIdx);
                
                if (m_notify)
                {
                    m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::BROW_CHAT, &nIdx, sizeof(int));
                }
                tagChatRecord record = {0};
                record.dwChatUserId = HallDataMgr::getInstance()->m_dwUserID;
                record.nIdx = nIdx + 1;
                record.bBrow = true;
                INSTANCE(GameDataMgr)->addRecord(record);
            }
            showGameChat(false);
        }
            break;
            
        default:
            break;
    }
}

void GameChatLayer::enableBrowItem(bool bEnable)
{
    std::map<size_t,BrowPanel*>::iterator ite = m_mapBrowItem.begin();
    for (; ite != m_mapBrowItem.end(); ++ite)
    {
        ite->second->setBrowTouchable(bEnable);
    }
}