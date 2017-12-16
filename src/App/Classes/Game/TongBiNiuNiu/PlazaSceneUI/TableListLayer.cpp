#include "TableListLayer.h"
#include "TableMgr.h"

USING_NS_CC;
using namespace ui;
USING_TBNN_NAMESPACE;

TableListLayer::TableListLayer():
m_tablePageView(nullptr),
m_notify(nullptr),
m_currentPage(nullptr)
{
    m_szTablePagaSize = Size(1136,500);
}

TableListLayer::~TableListLayer()
{
    
}

TableListLayer* TableListLayer::create(UILayer *notify /* = nullptr */)
{
    TableListLayer* pLayer = new TableListLayer();
    if (nullptr != pLayer && pLayer->init(notify))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return pLayer;
}

bool TableListLayer::init(UILayer *notify /*= nullptr*/)
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        m_notify = notify;
        
        bRes = true;
    } while (false);
    return bRes;
}

void TableListLayer::didScrollToPage(cocos2d::Node *sender,const size_t &page)
{
    m_currentPage = (TablePageItem*)sender;
    if (nullptr == m_currentPage)
    {
        return;
    }
    /*if (nullptr != m_dotNode)
     {
     m_dotNode->moveToDot((int)page);
     }*/
    WORD curGroup = page;
    //请求当前页桌子用户信息
    int nGroupCount = INSTANCE(TableMgr)->getPageGroupCount();
    NetworkMgr::getInstance()->sendRequestUserInfo(INVALID_TABLE, curGroup * nGroupCount);
    
    CCLOG("page:%d,%d",m_currentPage->getPageIdx(),(int)page);
    INSTANCE(TableMgr)->setCurrentGroup((int)page);
    std::vector<tagTableItem*> groupData = INSTANCE(TableMgr)->getTableGroupByIndex((int)page);
    if (!groupData.empty())
    {
        m_currentPage->refreshTablePage(groupData);
    }
}

cocos2d::Node* TableListLayer::getPageAt(const size_t &page)
{
    log("create :%zu",page);
    //创建指定页面的分组
    std::vector<tagTableItem*> groupData = INSTANCE(TableMgr)->getTableGroupByIndex((int)page);
    TablePageItem *pItem = TablePageItem::createTablePage((int)groupData.size(),m_notify);
    CC_ASSERT(pItem != nullptr);
    pItem->setPageIdx((int)page);
    return pItem;
}

void TableListLayer::refreshPageAt(cocos2d::Node *node,const size_t &page)
{
    
}

cocos2d::Size TableListLayer::pageSizeAtIndex(LCPageView2 *view, const size_t &idx)
{
    return m_szTablePagaSize;
}

size_t TableListLayer::numberOfPageInPageView(LCPageView2 *view)
{
    return INSTANCE(TableMgr)->getTableGroupCount();
}

void TableListLayer::refreshTableList()
{
    if (nullptr != m_tablePageView)
    {
        m_tablePageView->removeFromParentAndCleanup(true);
        m_tablePageView = nullptr;
    }
    /*
     if (nullptr != m_dotNode)
     {
     m_dotNode->removeAllChildrenWithCleanup(true);
     m_dotNode = nullptr;
     }
     m_dotNode = DotNode::createDotNode("game_page_0.png", "game_page_1.png");
     INSTANCE(UIMgr)->setNodePosition(m_dotNode, Vec2(pageSize.width * 0.5, -10.0f));
     m_dotNode->refreshDot(INSTANCE(TableMgr)->getTableGroupCount());
     this->addChild(m_dotNode);
     */
    
    Size pageSize = m_szTablePagaSize;
    Vec2 tablePos = s_vec2TableListPos;
    
    auto touchArea = Rect(tablePos.x, tablePos.y, pageSize.width,pageSize.height);
    auto viewArea = Rect(tablePos.x,tablePos.y,pageSize.width,pageSize.height);
    
    m_tablePageView = LCPageView2::createPageView(this,viewArea,touchArea,pageSize.width);
    m_tablePageView->setPageDelegate(this);
    m_tablePageView->setInvalide(true);
    this->addChild(m_tablePageView);
    m_tablePageView->scrollToPage(m_tablePageView->getCurPage());
}

void TableListLayer::refreshCurrentPageInfo()
{
    if (nullptr == m_currentPage)
    {
        return;
    }
    
    int currentPage = INSTANCE(TableMgr)->getCurrentGroup();
    std::vector<tagTableItem*> groupData = INSTANCE(TableMgr)->getTableGroupByIndex(currentPage);
    if (!groupData.empty())
    {
        m_currentPage->refreshTablePage(groupData);
    }
}

void TableListLayer::refreshTableInfo(tagTableItem *pItem)
{
    if (nullptr == m_currentPage)
    {
        return;
    }
    
    m_currentPage->refreshTableInfo(pItem);
}

void TableListLayer::refreshTableUser(const tagTableUserInfo &tagUser)
{
    if (nullptr == m_currentPage)
    {
        return;
    }
    
    m_currentPage->refreshTableUserInfo(tagUser);
}

void TableListLayer::showTableList(bool bShow)
{
    this->setVisible(bShow);
    if (m_tablePageView)
    {
        m_tablePageView->setInvalide(bShow);
    }    
}

void TableListLayer::enableTableList(bool bEnable)
{
    if (m_tablePageView)
    {
        m_tablePageView->setInvalide(bEnable);
    }
}