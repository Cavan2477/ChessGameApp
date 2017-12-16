#include "TablePageItem.h"

USING_NS_CC;
USING_NN_NAMESPACE;

TablePageItem::TablePageItem():
	m_nPageIdx(0),
	m_notify(nullptr)
{
	for (int i = 0; i < 4; ++i)
	{
		m_pTables[i] = nullptr;
	}
	m_pos[0] = Vec2(300,400);
	m_pos[1] = Vec2(800,400);
	m_pos[2] = Vec2(300,250);
	m_pos[3] = Vec2(800,250);
    m_pos[4] = Vec2(300,100);
    m_pos[5] = Vec2(800,100);
}

TablePageItem::~TablePageItem()
{

}

TablePageItem* TablePageItem::createTablePage(const int &tableCount,UILayer *notify /*= nullptr*/)
{
	TablePageItem *item = new TablePageItem();
	if (nullptr != item && item->init(tableCount,notify))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool TablePageItem::init(const int &tableCount,UILayer *notify /*= nullptr*/)
{
	bool bRes = false;
	do 
	{
		CC_BREAK_IF(tableCount > 6);
		m_nTableCount = tableCount;
		m_notify = notify;

        Size contentSize = Size(900,500);
        this->setContentSize(contentSize);
        
        Vec2 tablePos = s_vec2TableListPos;
		for (int i = 0; i < tableCount; ++i)
		{
			m_pTables[i] = TableItemView::create();
			CC_ASSERT(m_pTables[i] != nullptr);
			m_pTables[i]->setNotifyNode(m_notify);
            m_pTables[i]->setValidTouchArea(Rect(tablePos.x, tablePos.y, WinSize.width, WinSize.height));
            m_pTables[i]->setPosition(Vec2(m_pos[i].x, m_pos[i].y));
			this->addChild(m_pTables[i]);
		}

		bRes = true;
	} while (false);
	return bRes;
}

void TablePageItem::refreshTablePage(const std::vector<tagTableItem*> vecData)
{
	if (vecData.size() > m_nTableCount)
	{
		return;
	}

	for (int i = 0; i < vecData.size(); ++i)
	{
		m_pTables[i]->refreshTableItem(vecData[i]);
	}
}

void TablePageItem::refreshTableInfo(tagTableItem *pItem)
{
    if (nullptr == pItem || pItem->wTableId > m_nTableCount)
        return;
    std::vector<tagTableItem*> groupData = INSTANCE(TableMgr)->getTableGroupByIndex(INSTANCE(TableMgr)->getCurrentGroup());
    if (!groupData.empty())
    {
        for (int i = 0; i < groupData.size(); ++i)
        {
            if (groupData[i]->wTableId == pItem->wTableId)
            {
                m_pTables[i]->refreshTableItem(pItem);
                break;
            }
        }
    }
}

void TablePageItem::refreshTableUserInfo(const tagTableUserInfo &tagUser)
{
    UserData *pUser = HallDataMgr::getInstance()->getUserData(tagUser.wUserId);
    if (nullptr == pUser)
        return;
    
    std::vector<tagTableItem*> groupData = INSTANCE(TableMgr)->getTableGroupByIndex(INSTANCE(TableMgr)->getCurrentGroup());
    for (int i = 0; i < groupData.size(); ++i)
    {
        if (tagUser.tagStatus.wTableID == groupData[i]->wTableId)
        {
            if (pUser->m_date.wTableID == INVALID_TABLE)
            {
                m_pTables[i]->removeUser(tagUser.tagStatus.wChairID);
                break;
            }
            else
            {
                m_pTables[i]->refreshTableUser(pUser);
                break;
            }
        }
    }
}