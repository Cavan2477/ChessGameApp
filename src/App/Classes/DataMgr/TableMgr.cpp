/************************************************************************************
 * file: 		TableMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:48:30
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "TableMgr.h"

TableMgr* TableMgr::s_pTableMgrInstance = nullptr;

using namespace std;

void TableMgr::init()
{
	m_nCurrentGroup = -1;
    m_nPageGroupCount = 1;
    m_funTableCallBack = nullptr;
}

void TableMgr::clear()
{
	resetList();
}

bool TableMgr::haveTableList()
{
    DWORD dwKind = HallDataMgr::getInstance()->m_dwKindID;
    bool bRes = false;
    switch (dwKind)
    {
        case EM_GAME_BAIJIALE:
            bRes = false;
            break;
        case EM_GAME_NIUNIU:
            bRes = true;
            break;
        case EM_GAME_TBNN:
            bRes = true;
            break;
    }
    return bRes;
}

void TableMgr::addTableItem(const _stTableStatus *pStatus, const WORD &tableId)
{
    _stTableItem *item = getTableItemById(tableId);
    if (nullptr == item)
    {
        item = new _stTableItem();
        item->wTableId = tableId;
        item->tagStatus.cbPlayStatus = pStatus->cbPlayStatus;
        item->tagStatus.cbTableLock = pStatus->cbTableLock;
        item->tagStatus.nCellScore = pStatus->nCellScore;
        
        m_vecTables.push_back(item);
    }
}

bool TableMgr::updateTableItem(const _stTableStatus *pStatus,const WORD &tableId)
{
	bool bRes = false;
    _stTableItem *item = nullptr;
	do 
	{
		item = getTableItemById(tableId);
		if (nullptr == item)
		{
            return false;
		}
		item->wTableId = tableId;
		item->tagStatus.cbPlayStatus = pStatus->cbPlayStatus;
		item->tagStatus.cbTableLock = pStatus->cbTableLock;
		item->tagStatus.nCellScore = pStatus->nCellScore;
		
		bRes = true;
	} while (false);
    
    //用户是否在当前显示的table组中
    if (INSTANCE(TableMgr)->isTableInGroup(tableId,m_nCurrentGroup))
    {
        if (nullptr != m_funTableCallBack)
        {
            m_funTableCallBack(enTableCode::kUpdateTable, item, sizeof(_stTableItem));
        }
    }
    
	return bRes;
}

bool TableMgr::updateTableItem( const _stTableUserInfo &tagInfo )
{
	WORD updateTable = 0;
    _stTableItem *item = nullptr;
    _stTableUserInfo userInfo = _stTableUserInfo();

	bool bRes = false;
	do 
	{		
		//先找到该用户之前坐过的桌椅信息
		std::map<DWORD,_stTableUserInfo>::iterator ite = m_mapUserSit.find(tagInfo.wUserId);
		
		//用户未坐过（更新信息）
		if (ite == m_mapUserSit.end())
		{
			item = getTableItemById(tagInfo.tagStatus.wTableID);
			CC_BREAK_IF(item == nullptr);
			item->tableUsers[tagInfo.tagStatus.wChairID] = tagInfo;

			_stTableUserInfo tmpInfo = tagInfo;
			m_mapUserSit.insert(std::make_pair(tmpInfo.wUserId,tmpInfo));
			updateTable = tagInfo.tagStatus.wTableID;
            
            userInfo = tagInfo;
		}
		else
		{
			WORD sitTable = ite->second.tagStatus.wTableID;
			WORD sitChair = ite->second.tagStatus.wChairID;
			WORD userTable = tagInfo.tagStatus.wTableID;
			WORD userChair = tagInfo.tagStatus.wChairID;
			updateTable = sitTable;
            userInfo.wUserId = tagInfo.wUserId;
            userInfo.tagStatus.wTableID = sitTable;
            userInfo.tagStatus.wChairID = sitChair;
            userInfo.tagStatus.cbUserStatus = tagInfo.tagStatus.cbUserStatus;

			item = getTableItemById(sitTable);
			CC_BREAK_IF(item == nullptr);

			//已有用户信息，判断是更新游戏状态还是换桌
			if (sitTable == userTable)	//同一桌，更新状态信息
			{
                int max_player = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
				//参数检验
				CC_BREAK_IF(tagInfo.tagStatus.wChairID > max_player);

				if (sitChair == userChair) //同座位
				{
					item->tableUsers[sitChair] = tagInfo;
				}
				else //非同座位：清除原有座位信息，再更新新的信息
				{
					item->tableUsers[sitChair] = _stTableUserInfo();
					item->tableUsers[userChair] = tagInfo;
				}
			}
			else //非同桌
			{
				//清除原有桌信息
				item->tableUsers[sitChair] = _stTableUserInfo();
				m_mapUserSit.erase(ite);
				bRes = true;

				//新的桌子
				item = getTableItemById(userTable);
				CC_BREAK_IF(item == nullptr);
				item->tableUsers[userChair] = tagInfo;
				_stTableUserInfo tmpInfo = tagInfo;
				m_mapUserSit.insert(std::make_pair(tmpInfo.wUserId,tmpInfo));
			}
		}		
		bRes = true;
	} while (false);
    
    //用户是否在当前显示的table组中
    if (INSTANCE(TableMgr)->isTableInGroup(updateTable,m_nCurrentGroup))
    {
        if (nullptr != m_funTableCallBack)
        {
            m_funTableCallBack(enTableCode::kUpdateTableUser, (void*)&userInfo, sizeof(_stTableUserInfo));
        }
    }
    
	return bRes;
}

void TableMgr::grouping(const int &game_kind /*const int groupCount*/)
{
    m_nPageGroupCount = 6;
    switch (game_kind)
    {
        case EM_GAME_TBNN:
            m_nPageGroupCount = 3;
            break;
    }
	size_t all = m_vecTables.size();
	int group = int(all / m_nPageGroupCount);
	
	int idx = 0;
	for (int i = 0; i < group; ++i)
	{
		std::vector<_stTableItem*> vecGroup = std::vector<_stTableItem*>();
		for (int j = 0; j < m_nPageGroupCount; ++j)
		{
			vecGroup.push_back(m_vecTables[idx]);
			++idx;
		}
		m_vecGroup.push_back(vecGroup);
	}
    
    std::vector<_stTableItem*> vecGroup = std::vector<_stTableItem*>();
    int left = (int)(all - idx);
    for (int i = 0; i < left; ++i)
    {
        vecGroup.push_back(m_vecTables[idx]);
        ++idx;
    }
    if (!vecGroup.empty())
    {
        m_vecGroup.push_back(vecGroup);
    }
    
    //通知ui更新界面
    if (m_funTableCallBack)
    {
        m_funTableCallBack(kGetTableInfo, nullptr, 0);
    }
}

_stTableItem* TableMgr::getTableItemById(const WORD &tableId)
{
	_stTableItem *res = nullptr;
	for (int i = 0; i < m_vecTables.size(); ++i)
	{
		if (m_vecTables[i]->wTableId == tableId)
		{
			res = m_vecTables[i];
			break;
		}
	}
	return res;
}

std::vector<_stTableItem*> TableMgr::getTableGroupByIndex(const int &nIdx)
{
	std::vector<_stTableItem*> vec = std::vector<_stTableItem*>();
	if (nIdx >= m_vecGroup.size())
	{
		return vec;
	}
	return m_vecGroup[nIdx];
}

void TableMgr::resetList()
{
    //清理分组
    std::vector<std::vector<_stTableItem*>>().swap(m_vecGroup);
    
	vector<_stTableItem*>::iterator ite = m_vecTables.begin();
	for (; ite != m_vecTables.end(); )
	{
		_stTableItem *pItem = *ite;
		delete pItem;
		pItem = nullptr;
		
		ite = m_vecTables.erase(ite);
	}
    
    m_mapUserSit.clear();
}

int TableMgr::getTableCount()
{
	return (int)m_vecTables.size();
}

int TableMgr::getTableGroupCount()
{
	return (int)m_vecGroup.size();
}

bool TableMgr::isTableInGroup(const int &tableidx,const int &group)
{
	bool bRes = false;
	std::vector<_stTableItem*> groupData = getTableGroupByIndex(group);
	for (size_t i = 0; i < groupData.size(); ++i)
	{
		if (groupData[i]->wTableId == tableidx)
		{
			bRes = true;
			break;
		}
	}
	return bRes;
}

bool TableMgr::isTableLocked(const int &tableidx)
{
    _stTableItem *pItem = getTableItemById(tableidx);
    if (nullptr != pItem)
    {
        return pItem->tagStatus.cbTableLock == 1;
    }
    return false;
}

bool TableMgr::isTableValid(const int &tableidx, bool &isLocked, bool &isPlaying)
{
    isLocked = false;
    isPlaying = false;
    _stTableItem *pItem = getTableItemById(tableidx);
    if (nullptr != pItem)
    {
        isLocked = pItem->tagStatus.cbTableLock == 1;
        isPlaying = pItem->tagStatus.cbPlayStatus == 1;
        return true;
    }
    return false;
}