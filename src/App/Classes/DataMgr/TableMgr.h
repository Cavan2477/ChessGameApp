/************************************************************************************
 * file: 		TableMgr.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:45:41
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __TABLE_MGR_H__
#define __TABLE_MGR_H__

#include "CocosHeader.h"
#include "../Pattern/Singleton.h"
#include "../Common/MacroDefine.h"
#include "../Common/Struct.h"
//#include "CMD_Game.h"

enum enTableCode
{
    kGetTableInfo = 100,            //获取到桌子信息
    kUpdateTable,					//更新桌子信息
    kUpdateTableUser,               //更新桌子用户
};

typedef struct _stTableUserInfo
{
	DWORD				wUserId;
	_stUserStatus		tagStatus;

	_stTableUserInfo()
	{
		wUserId = INVALID_USERID;
		tagStatus.wChairID = INVALID_CHAIR;
		tagStatus.wTableID = INVALID_TABLE;
	}
}ST_TABLE_USER_INFO, *PST_TABLE_USER_INFO;

typedef struct _stTableItem
{
	WORD				wTableId;				//桌子id
	_stTableStatus		tagStatus;				//桌子状态
	_stTableUserInfo		tableUsers[6];          //桌子用户id
}ST_TABLE_ITEM, *PST_TABLE_ITEM;

class TableMgr
{
	CREATE_SINGLETON_MUTEX_CLEAR(TableMgr,s_pTableMgrInstance,nullptr);

	void init();
	void clear();

public:
    /*
     * @brief 是否拥有桌子界面
     * @return 返回结果
     */
    bool haveTableList();
    
    //增加桌子信息
    void addTableItem(const _stTableStatus *pStatus,const WORD &tableId);
    
	//更新桌子状态信息
	bool updateTableItem(const _stTableStatus *pStatus,const WORD &tableId);
    
	//更新桌子用户信息(桌子状态信息必须已存在)
	bool updateTableItem( const _stTableUserInfo &tagInfo );

	/*
	 * @brief 将所有的桌子进行分组
	 * @param[LIST_Kind] 游戏类型(根据游戏类型确定分组数量)
	 */
	void grouping(const int &LIST_Kind);

	_stTableItem* getTableItemById(const WORD &tableId);

	std::vector<_stTableItem*> getTableGroupByIndex(const int &nIdx);

	void resetList();

	int getTableCount();

	//获取所有的桌子分组后的数量
	int getTableGroupCount();

	//当前显示的桌子分页
	CC_SYNTHESIZE(int,m_nCurrentGroup,CurrentGroup);
    
    //桌子分组人数
    CC_SYNTHESIZE(int, m_nPageGroupCount, PageGroupCount);

	//桌子是否在该组中
	bool isTableInGroup(const int &tableidx,const int &group);
    
    //桌子是否加密状态
    bool isTableLocked(const int &tableidx);
    
    //桌子是否可以坐下
    bool isTableValid(const int &tableidx, bool &isLocked, bool &isPlaying);
    
    //回调函数
    std::function<void(const enTableCode& nCode, void *pData, const int& nSize)> m_funTableCallBack;

private:
	static TableMgr*						s_pTableMgrInstance;

	std::vector<_stTableItem*>				m_vecTables;
	std::vector<std::vector<_stTableItem*>> m_vecGroup;

	std::map<DWORD,_stTableUserInfo>		m_mapUserSit;			//存储坐下的用户的桌椅信息
};
#endif