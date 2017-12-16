#ifndef __bjl_header_TablePageItem_H__
#define __bjl_header_TablePageItem_H__

#include "CocosHeader.h"
#include "TableItemView.h"

BJL_NAMESPACE_BEGIN

class TablePageItem:public cocos2d::Node
{
public:
	TablePageItem();
	~TablePageItem();

	/*
	 * @brief 创建一个table分页
	 * @param[tableCount] 该页table的数量
	 */
	static TablePageItem *createTablePage(const int &tableCount,UILayer *notify = nullptr);

	bool init(const int &tableCount,UILayer *notify = nullptr);

	void refreshTablePage(const std::vector<tagTableItem*> vecData);
    
    void refreshTableInfo(tagTableItem* pItem);
    
    void refreshTableUserInfo(const tagTableUserInfo &tagUser);
    
	CC_SYNTHESIZE(int,m_nPageIdx,PageIdx);
	//ui消息通知
	CC_SYNTHESIZE(UILayer*,m_notify,NotifyNode);
private:
	//一个分页4个桌子
	TableItemView *m_pTables[4];
	cocos2d::Vec2 m_pos[4];
	int m_nTableCount;
};

BJL_NAMESPACE_END
#endif