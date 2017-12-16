#ifndef __bjl_header_TableListLayer_H__
#define __bjl_header_TableListLayer_H__

#include "CocosHeader.h"
#include "LCPageView.h"
#include "LCPageView2.h"
#include "TablePageItem.h"
#include "DotNode.h"

BJL_NAMESPACE_BEGIN

class TableListLayer:public cocos2d::Layer,
	public LCPageView2Delegate,
    public LCPageView2DataSource
{
public:
	TableListLayer();
	~TableListLayer();

	static TableListLayer* create(UILayer *notify = nullptr);
	bool init(UILayer *notify = nullptr);
    
	virtual void didScrollToPage(cocos2d::Node *sender,const size_t &page);
    virtual cocos2d::Node* getPageAt(const size_t &page);
    virtual void refreshPageAt(cocos2d::Node *node,const size_t &page);
    virtual cocos2d::Size pageSizeAtIndex(LCPageView2 *view, const size_t &idx);
    virtual size_t numberOfPageInPageView(LCPageView2 *view);
	//ui消息通知
	CC_SYNTHESIZE(UILayer*,m_notify,NotifyNode);
public:
	void refreshTableList();
	void refreshCurrentPageInfo();
    void refreshTableInfo(tagTableItem *pItem);
    void refreshTableUser(const tagTableUserInfo &tagUser);
    void showTableList(bool bHide);
    void enableTableList(bool bEnable);
private:
	LCPageView2 *m_tablePageView;
	TablePageItem *m_currentPage;
    DotNode *m_dotNode;
    cocos2d::Size m_szTablePagaSize;
};

BJL_NAMESPACE_END
#endif