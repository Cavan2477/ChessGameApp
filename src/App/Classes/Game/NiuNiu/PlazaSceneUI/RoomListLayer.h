//
//  RoomListLayer.hpp
//  DouDiZhu
//
//  Created by zhong on 1/26/16.
//
//

#ifndef nn_RoomListLayer_hpp
#define nn_RoomListLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

NN_NAMESPACE_BEGIN

struct tagGameServerParam
{
    tagGameServer*  m_pServer;
    int             m_nIdx;
};
class RoomListLayer:public UILayer,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
public:
    UILAYER_WITHNOTIFY(RoomListLayer);
    
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView *table) override;
    
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
public:
    virtual void setVisible(bool visible) override;
private:
    /*
     * @brief 获取房间组
     * @param[groupCount] 房间组内的房间数
     */
    cocos2d::ui::Widget* getRoomGroup(const int &groupCount);
private:
    cocos2d::extension::TableView *m_roomList;
    //cocos2d::ui::ListView *m_roomListView;
    float m_fCount;
    cocos2d::Size m_szCellSize;
    //房间选中效果
    cocos2d::Action *m_actSelect;
    std::vector<std::vector<tagGameServerParam>> m_vecRoomGroup;
};

NN_NAMESPACE_END

#endif /* RoomListLayer_hpp */
