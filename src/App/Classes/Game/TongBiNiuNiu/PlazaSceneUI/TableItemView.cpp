#include "TableItemView.h"
#include "HallDataMgr.h"

USING_NS_CC;
using namespace ui;
USING_TBNN_NAMESPACE;

static const char* CHAIR_NAME = "table_chair";
TableItemView::TableItemView():
m_textTableId(nullptr),
m_notify(nullptr)
{
    int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
    for (int i = 0; i < max; ++i)
    {
        m_btnChairs[i] = nullptr;
        m_spReady[i] = nullptr;
    }
}

TableItemView::~TableItemView()
{
    
}

bool TableItemView::init()
{
    bool bRes = false;
    do
    {
        LOAD_CSB_ITEM("plaza/TableItem.csb");
        
        m_textTableId = static_cast<Text*>(m_root->getChildByName("tableId"));
        CC_ASSERT(m_textTableId != nullptr);
        
        char buf[64] = "";
        Node *tmp = nullptr;
        int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
        for (int i = 0; i < max; ++i)
        {
            sprintf(buf,"user%d",i + 1);
            m_btnChairs[i] = static_cast<Button*>(m_root->getChildByName(buf));
            CC_ASSERT(m_btnChairs[i] != nullptr);
            m_btnChairs[i]->loadTextureNormal("desk_noBody.png",Widget::TextureResType::PLIST);
            m_btnChairs[i]->setSwallowTouches(false);
            m_btnChairs[i]->setTag(i);
            m_btnChairs[i]->setName(CHAIR_NAME);
            m_btnChairs[i]->addTouchEventListener(CC_CALLBACK_2(TableItemView::touchEvent,this));
            
            sprintf(buf,"ready%d",i + 1);
            m_spReady[i] = static_cast<Sprite*>(m_root->getChildByName(buf));
            CC_ASSERT(m_spReady[i] != nullptr);
            m_spReady[i]->setLocalZOrder(1);
            m_spReady[i]->setVisible(false);
            
            sprintf(buf, "name%d",i + 1);
            /*
             m_textUserName[i] = static_cast<Text*>(m_root->getChildByName(buf));
             CC_ASSERT(m_textUserName[i] != nullptr);
             */
            tmp = m_root->getChildByName(buf);
            CC_ASSERT(tmp != nullptr);
            Size nameSize = Size(87, 23);
            m_clipUserName[i] = ClipText::createClipText(nameSize, "");
            CC_ASSERT(m_clipUserName[i] != nullptr);
            m_clipUserName[i]->setTextFontSize(14.0f);
            m_clipUserName[i]->setTextColor(Color4B::GREEN);
            m_clipUserName[i]->setAnchorPoint(Vec2(0, 0));
            m_clipUserName[i]->setPosition(tmp->getPosition());
            m_root->addChild(m_clipUserName[i]);
            
            m_pChairs[i] = nullptr;
        }
        
        m_spLock = static_cast<Sprite*>(m_root->getChildByName("lock_sp"));
        CC_ASSERT(m_spLock != nullptr);
        
        m_spPlaying = static_cast<Sprite*>(m_root->getChildByName("playing_sp"));
        CC_ASSERT(m_spPlaying != nullptr);
        m_spPlaying->setLocalZOrder(2);
        
        bRes = true;
    } while (false);
    return bRes;
}

void TableItemView::refreshTableItem(const tagTableItem* tableItem)
{
    char buf[64] = "";
    sprintf(buf,"%d",tableItem->wTableId + 1);
    m_textTableId->setString(buf);
    
    //更新头像状态等信息
    int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
    for (int i = 0; i < max; ++i)
    {
        m_spReady[i]->setVisible(false);
        //座位有用户
        if (tableItem->tableUsers[i].wUserId != INVALID_USERID)
        {
            //m_btnChairs[i]->loadTextureNormal("plaza/desk_yesbody.png");
            if (tableItem->tableUsers[i].tagStatus.cbUserStatus == US_READY)
            {
                m_spReady[i]->setVisible(true);
            }
            
            //昵称
            UserData *pUser = HallDataMgr::getInstance()->getUserData(tableItem->tableUsers[i].wUserId);
            if (nullptr != pUser)
            {
                m_clipUserName[i]->setString(pUser->m_nickname);
                //更新头像
                if (nullptr == m_pChairs[i])
                {
                    if (pUser->m_date.dwUserID == HallDataMgr::getInstance()->m_dwUserID)
                    {
                        if (HallDataMgr::getInstance()->m_loadtype == Load_Normal || HallDataMgr::getInstance()->m_loadtype == Load_Visitor)
                        {
                            m_pChairs[i] = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                                           HallDataMgr::getInstance()->m_wCustom,
                                                                           HallDataMgr::getInstance()->m_dwUserID,
                                                                           HallDataMgr::getInstance()->m_cbGender);
                        }
                        else if (HallDataMgr::getInstance()->m_loadtype == Load_RenRen || HallDataMgr::getInstance()->m_loadtype == Load_Sina)
                        {
                            m_pChairs[i] = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
                        }
                    }
                    else
                    {
                        m_pChairs[i] = HeaderRequest::createwithFaceID(pUser->m_date.wFaceID,
                                                                       pUser->m_date.dwCustomID,
                                                                       pUser->m_date.dwUserID,
                                                                       pUser->m_date.cbGender);
                    }
                    Sprite *pSen = Sprite::createWithSpriteFrameName("desk_noBody.png");
                    ClippingNode *pNode = ClippingNode::create();
                    pNode->setStencil(pSen);
                    pNode->setPosition(m_btnChairs[i]->getPosition());
                    pNode->setAlphaThreshold(0);
                    m_root->addChild(pNode);
                    
                    m_pChairs[i]->setHeadSize(90.0f);
                    pNode->addChild(m_pChairs[i]);
                }
            }
        }
        else
        {
            if (nullptr != m_pChairs[i])
            {
                if (nullptr != m_pChairs[i]->getParent())
                {
                    m_pChairs[i]->getParent()->removeFromParentAndCleanup(true);
                }
                m_pChairs[i] = nullptr;
            }
            m_clipUserName[i]->setString("");
        }
    }
    m_nTableId = tableItem->wTableId;
    //锁定
    m_spLock->setVisible(tableItem->tagStatus.cbTableLock == 1);
    //游戏进行中
    m_spPlaying->setVisible(tableItem->tagStatus.cbPlayStatus == 1);
}

void TableItemView::refreshTableUser(UserData *pUser)
{
    if (nullptr == pUser)
        return;
    
    int i = pUser->m_date.wChairID;
    
    m_clipUserName[i]->setString(pUser->m_nickname);
    //更新头像
    if (nullptr == m_pChairs[i])
    {
        if (pUser->m_date.dwUserID == HallDataMgr::getInstance()->m_dwUserID)
        {
            if (HallDataMgr::getInstance()->m_loadtype == Load_Normal || HallDataMgr::getInstance()->m_loadtype == Load_Visitor)
            {
                m_pChairs[i] = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                               HallDataMgr::getInstance()->m_wCustom,
                                                               HallDataMgr::getInstance()->m_dwUserID,
                                                               HallDataMgr::getInstance()->m_cbGender);
            }
            else if (HallDataMgr::getInstance()->m_loadtype == Load_RenRen || HallDataMgr::getInstance()->m_loadtype == Load_Sina)
            {
                m_pChairs[i] = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
            }
        }
        else
        {
            m_pChairs[i] = HeaderRequest::createwithFaceID(pUser->m_date.wFaceID,
                                                           pUser->m_date.dwCustomID,
                                                           pUser->m_date.dwUserID,
                                                           pUser->m_date.cbGender);
        }
        Sprite *pSen = Sprite::createWithSpriteFrameName("desk_noBody.png");
        ClippingNode *pNode = ClippingNode::create();
        pNode->setStencil(pSen);
        pNode->setPosition(m_btnChairs[i]->getPosition());
        pNode->setAlphaThreshold(0);
        m_root->addChild(pNode);
        
        m_pChairs[i]->setHeadSize(90.0f);
        pNode->addChild(m_pChairs[i]);
    }
    
    //准备
    m_spReady[i]->setVisible(pUser->m_date.cbUserStatus == US_READY);
}

void TableItemView::removeUser(const WORD &wChair)
{
    int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;
    if (wChair < max)
    {
        if (nullptr != m_pChairs[wChair])
        {
            if (nullptr != m_pChairs[wChair]->getParent())
            {
                m_pChairs[wChair]->getParent()->removeFromParentAndCleanup(true);
            }
            m_pChairs[wChair] = nullptr;
        }
        
        m_clipUserName[wChair]->setString("");
        //准备
        m_spReady[wChair]->setVisible(false);
    }
}

void TableItemView::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    Button *pBtn = static_cast<Button*>(pSender);
    if (nullptr == pBtn)
    {
        return;
    }
    
    if (type == Widget::TouchEventType::ENDED)
    {
        Vec2 beginPos = pBtn->getTouchBeganPosition();
        Vec2 pos = pBtn->getTouchEndPosition();
        if (fabs(beginPos.x - pos.x) > DIS_BTN_ENABLE
            || fabs(beginPos.y - pos.y) > DIS_BTN_ENABLE)
        {
            return;
        }
        
        bool bCanTouch = true;
        if (m_notify)
        {
            //检测是否可点击
            bCanTouch = m_notify->uiMsg(UI_MSG_BASE::UI_MSG_PLAZA, PLAZA_UI::CLICK_DESK, nullptr, 0);
        }
        
        if (0 == strcmp(CHAIR_NAME,pBtn->getName().c_str())
            && m_rectValidArea.containsPoint(pos))
        {
            tagTableParam param = {0};
            param.wTableId = m_nTableId;
            param.nChair = pBtn->getTag();
            
            if (nullptr != m_notify)
            {
                m_notify->uiMsg(UI_MSG_BASE::UI_MSG_PLAZA,PLAZA_UI::SIT_DOWN,&param,sizeof(tagTableItem));
            }
        }		
    }
}