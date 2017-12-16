//
//  ChatTableView.hpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#ifndef nn_ChatTableView_hpp
#define nn_ChatTableView_hpp

#include <stdio.h>
#include "CocosHeader.h"

NN_NAMESPACE_BEGIN

class ChatTableView;
class ChatTableViewDataSource
{
public:
    virtual cocos2d::Node *getDataItemAt(ChatTableView *chatTable, const size_t &idx) = 0;
    virtual void refreshDataItemAt(ChatTableView *chatTable, cocos2d::Node *item,const size_t &idx) = 0;
    virtual size_t getDataItemSize(ChatTableView *chatTable) = 0;
    virtual cocos2d::Size getItemSize(ChatTableView *chatTable,const size_t &idx) = 0;
    virtual void onTouchItem(ChatTableView *chatTable, cocos2d::extension::TableViewCell* cell) = 0;
};

class ChatTableView:public cocos2d::Layer,
    public cocos2d::extension::TableViewDataSource,
    public cocos2d::extension::TableViewDelegate
{
public:
    ChatTableView();
    ~ChatTableView();
    
    static ChatTableView* create();
    virtual bool init();
    
    static ChatTableView* create(ChatTableViewDataSource *pData, cocos2d::Size size, const char *bgfile = nullptr);
    bool initChatTableView(ChatTableViewDataSource *pData, cocos2d::Size size, const char *bgfile = nullptr);
    
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    //ui消息通知
    CC_SYNTHESIZE(UILayer*,m_notify,NotifyNode);
    
    //
    void reload(){ m_tableview->reloadData(); };
private:
    ChatTableViewDataSource *m_dataSource;
    cocos2d::extension::TableView *m_tableview;
    cocos2d::Sprite *m_spBg;
};

NN_NAMESPACE_END

#endif /* ChatTableView_hpp */
