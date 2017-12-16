//
//  RoomListLayer.cpp
//  DouDiZhu
//
//  Created by zhong on 1/26/16.
//
//

#include "RoomListLayer.h"
#include "RoomItemView.h"
#include "GameDataMgr.h"

USING_NN_NAMESPACE;
USING_NS_CC;
USING_NS_CC_EXT;

RoomListLayer::RoomListLayer(void):
m_notify(nullptr)
{
    m_fCount = 1000 / 2;
    auto s = ScaleTo::create(0.1f, 1.05f);
    auto s_ = ScaleTo::create(0.1f, 1.0f);
    auto call = CallFuncN::create([this](Node* pSender)
                                  {
                                      TableViewCell *pCell = (TableViewCell*)pSender;
                                      if (nullptr != pCell)
                                      {
                                          tagGameServer *dataitem = HallDataMgr::getInstance()->m_subRoomList[pCell->getIdx()];
                                          if (nullptr != dataitem)
                                          {
                                              INSTANCE(GameDataMgr)->setEnterRoom((int)pCell->getIdx());
                                              if (nullptr != m_notify)
                                              {			
                                                  m_notify->uiMsg(UI_MSG_BASE::UI_MSG_PLAZA,PLAZA_UI::ENTER_ROOM,dataitem,sizeof(tagGameServer));
                                              }
                                          }
                                      }
                                  });
    m_actSelect = Sequence::create(s,s_,call, NULL);
    CC_SAFE_RETAIN(m_actSelect);
}

RoomListLayer::~RoomListLayer()
{
    CC_SAFE_RELEASE(m_actSelect);
}

bool RoomListLayer::init()
{
    Size visible = Director::getInstance()->getWinSize();
    bool bRes = false;
    do
    {
        Size viewSize = Size(1000, 400);
        m_szCellSize = Size(0,400);
        m_szCellSize.width = m_fCount;
        
        size_t all = HallDataMgr::getInstance()->m_subRoomList.size();
        int group = (int)all / 2;
        int idx = 0;
        //整除
        for (int i = 0; i < group; ++i)
        {
            std::vector<tagGameServerParam> vecGroup = std::vector<tagGameServerParam>();
            for (int j = 0; j < 2; ++j)
            {
                tagGameServerParam param = tagGameServerParam();
                param.m_pServer = HallDataMgr::getInstance()->m_subRoomList[idx];
                param.m_nIdx = idx;
                vecGroup.push_back(param);
                ++idx;
            }
            m_vecRoomGroup.push_back(vecGroup);
        }
        std::vector<tagGameServerParam> vecGroup = std::vector<tagGameServerParam>();
        int left = (int)(all - idx);
        //剩余
        for (int i = 0; i < left; ++i)
        {
            tagGameServerParam param = tagGameServerParam();
            param.m_pServer = HallDataMgr::getInstance()->m_subRoomList[idx];
            param.m_nIdx = idx;
            vecGroup.push_back(param);
            ++idx;
        }
        if (!vecGroup.empty())
        {
            m_vecRoomGroup.push_back(vecGroup);
        }
        m_roomList = TableView::create(this,viewSize);
        m_roomList->setDirection(TableView::Direction::HORIZONTAL);
        m_roomList->setDelegate(this);
        m_roomList->setPosition(0,0);
        this->addChild(m_roomList);
        m_roomList->reloadData();
        
        bRes = true;
    } while (false);
    return bRes;
}

TableViewCell* RoomListLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    std::vector<tagGameServerParam> vecGroup = m_vecRoomGroup[idx];
    Node* item = nullptr;
    
    if (nullptr == cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        item = getRoomGroup((int)vecGroup.size());
        item->setPosition(m_fCount * 0.5f,table->getViewSize().height * 0.5f);
        item->setName("room_item_view");
        cell->addChild(item);
    }
    else
    {
        item = static_cast<CRoomItemView*>(cell->getChildByName("room_item_view"));
    }
    
    if (!vecGroup.empty() && nullptr != item)
    {
        char buf[16] = "";
        for (size_t i = 0; i < vecGroup.size(); ++i)
        {
            tagGameServer* dataitem = vecGroup[i].m_pServer;
            int nType = (dataitem->dwServerRule & SR_ALLOW_AVERT_CHEAT_MODE) == 1 ? 10 : dataitem->wServerLevel;
            int nRoom = nType % 4;
            SCORE lScore = dataitem->lCellScore;
            SCORE lRuChang = dataitem->lEnterScore;
            int nZaiXian = dataitem->dwOnLineCount + dataitem->dwAndroidCount;
            
            sprintf(buf, "room_%d",(int)i);
            CRoomItemView *room = static_cast<CRoomItemView*>(item->getChildByName(buf));
            if (nullptr != room)
            {
                room->setTag(vecGroup[i].m_nIdx);
                room->refresh(nRoom,nType,lScore,lRuChang,nZaiXian);
            }
        }
    }
    return cell;
}

ssize_t RoomListLayer::numberOfCellsInTableView(TableView *table)
{
    return m_vecRoomGroup.size();
}

Size RoomListLayer::cellSizeForTable(TableView *table)
{
    return m_szCellSize;
}

void RoomListLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    //cell->stopAllActions();
    //cell->runAction(m_actSelect);
}

void RoomListLayer::setVisible(bool visible)
{
    UILayer::setVisible(visible);
    if (visible && m_roomList)
    {
        m_roomList->reloadData();
    }
}

cocos2d::ui::Widget* RoomListLayer::getRoomGroup(const int &groupCount)
{
    Widget *node = Widget::create();
    
    float posY[2] = {90.0f, -90.0f};
    char buf[32] = "";
    for (int i = 0; i < groupCount; ++i)
    {
        sprintf(buf, "room_%d",i);
        
        CRoomItemView* item1 = CRoomItemView::create();
        item1->setNotify(m_notify);
        item1->setPositionY(posY[i]);
        item1->setName(buf);
        node->addChild(item1);
    }
    
    return node;
}