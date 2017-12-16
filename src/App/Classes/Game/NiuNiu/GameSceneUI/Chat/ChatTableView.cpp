//
//  ChatTableView.cpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#include "ChatTableView.h"
USING_NS_CC;
USING_NS_CC_EXT;
USING_NN_NAMESPACE;

ChatTableView::ChatTableView():
    m_tableview(nullptr),
    m_spBg(nullptr)
{
    
}

ChatTableView::~ChatTableView()
{
    
}

ChatTableView* ChatTableView::create()
{
    CC_ASSERT(true);
    return nullptr;
}

bool ChatTableView::init()
{
    return false;
}

ChatTableView* ChatTableView::create(ChatTableViewDataSource *pData, cocos2d::Size size, const char *bgfile /*= nullptr*/)
{
    ChatTableView *ptmp = new ChatTableView();
    if (nullptr != ptmp && ptmp->initChatTableView(pData, size,bgfile))
    {
        ptmp->autorelease();
        return ptmp;
    }
    CC_SAFE_DELETE(ptmp);
    return nullptr;
}

bool ChatTableView::initChatTableView(ChatTableViewDataSource *pData, cocos2d::Size size, const char *bgfile /*= nullptr*/)
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF( !Layer::init() );
        m_dataSource = pData;
        
        m_tableview = TableView::create(this, size);
        CC_ASSERT(m_tableview != nullptr);
        m_tableview->setDirection(TableView::Direction::VERTICAL);
        m_tableview->setDelegate(this);
        m_tableview->setPosition(0,0);
        this->addChild(m_tableview);
        
        if (nullptr != bgfile )
        {
            m_spBg = Sprite::create(bgfile);
            m_spBg->setAnchorPoint(Vec2(0,0));
            this->addChild(m_spBg);
        }
        
        bRes = true;
    }while(false);
    return bRes;
}

TableViewCell* ChatTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    
    Node *dataitem = nullptr;
    if (nullptr == cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
        dataitem = m_dataSource->getDataItemAt(this,idx);
        if (nullptr != dataitem)
        {
            dataitem->setName("chat_item");
            cell->addChild(dataitem);
        }
    }
    else
    {
        dataitem = cell->getChildByName("chat_item");        
    }
    if (nullptr != dataitem)
    {
        m_dataSource->refreshDataItemAt(this,dataitem,idx);
    }
    cell->setTag((int)idx);
    
    return cell;
}

ssize_t ChatTableView::numberOfCellsInTableView(TableView *table)
{
    return m_dataSource->getDataItemSize(this);
}

Size ChatTableView::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{    
    return m_dataSource->getItemSize(this,idx);
}

void ChatTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{    
    m_dataSource->onTouchItem(this,cell);
}