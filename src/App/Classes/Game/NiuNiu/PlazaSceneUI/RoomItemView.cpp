//
//  RoomItemView.cpp
//  DouDiZhu
//
//  Created by zhong on 1/13/16.
//
//

#include "RoomItemView.h"
#include "GameDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_NN_NAMESPACE;

CRoomItemView::CRoomItemView():
m_spRoomBg(nullptr),
m_spRoomType(nullptr),
m_spAntiCheating(nullptr)
{
    auto s = ScaleTo::create(0.1f, 1.05f);
    auto s_ = ScaleTo::create(0.1f, 1.0f);
    auto call = CallFuncN::create([this](Node* pSender)
                                  {
                                      int tag = this->getTag();
                                      if (tag >= 0 && tag < HallDataMgr::getInstance()->m_subRoomList.size())
                                      {
                                          tagGameServer *dataitem = HallDataMgr::getInstance()->m_subRoomList[tag];
                                          if (nullptr != dataitem)
                                          {
                                              INSTANCE(GameDataMgr)->setEnterRoom(tag);
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

CRoomItemView::~CRoomItemView()
{
    CC_SAFE_RELEASE(m_actSelect);
}

bool CRoomItemView::init()
{
    bool result = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        LOAD_CSB_ITEM("plaza/RoomItem.csb");
        
        //m_spRoomBg = static_cast<Sprite*>(m_root->getChildByName("roombg"));
        //CC_ASSERT(m_spRoomBg != nullptr);
        m_btnRoomBg = static_cast<Button*>(m_root->getChildByName("roombg"));
        CC_ASSERT(m_btnRoomBg != nullptr);
        m_btnRoomBg->setSwallowTouches(false);
        m_btnRoomBg->addTouchEventListener(CC_CALLBACK_2(CRoomItemView::touchEvent, this));
        
        m_spRoomType = static_cast<Sprite*>(m_root->getChildByName("roomtype"));
        CC_ASSERT(m_spRoomType != nullptr);
        
        m_spAntiCheating = static_cast<Sprite*>(m_root->getChildByName("anticheating"));
        CC_ASSERT(m_spAntiCheating != nullptr);
        
        //m_labDiFen = static_cast<TextAtlas*>(m_root->getChildByName("difen"));
        m_textDiFen = static_cast<Text*>(m_root->getChildByName("difen"));
        CC_ASSERT(m_textDiFen != nullptr);
        
        //m_labRuChang = static_cast<TextAtlas*>(m_root->getChildByName("ruchang"));
        m_textRuChang = static_cast<Text*>(m_root->getChildByName("ruchang"));
        CC_ASSERT(m_textRuChang != nullptr);
        
        //m_labZaiXian = static_cast<TextAtlas*>(m_root->getChildByName("zaixian"));
        m_textZaiXian = static_cast<Text*>(m_root->getChildByName("zaixian"));
        CC_ASSERT(m_textZaiXian != nullptr);
        
        result = true;
    } while (0);
    
    return result;
}

void CRoomItemView::refresh(const int &nRoom, const int &nType, const SCORE &lScore, const SCORE &lRuChang, const int &nZaiXian)
{
    m_btnRoomBg->setEnabled(true);
    
    this->setRoom(nRoom);
    this->setType(nType);
    this->setDifen(lScore);
    this->setRuChang(lRuChang);
    this->setZaiXian(nZaiXian);
}

void CRoomItemView::setRoom(const int &nRoom)
{
    memset(m_buf,0,64);
    //SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_buf);
    //CC_ASSERT(frame != nullptr);
    //m_spRoomBg->setSpriteFrame(frame);
    
    if ((DWORD)kind_niuniu == HallDataMgr::getInstance()->m_dwKindID)
    {
        sprintf(m_buf,"plaza/room_%d.png",nRoom);
        m_btnRoomBg->loadTextureNormal(m_buf);
        m_btnRoomBg->loadTextureDisabled(m_buf);
        m_btnRoomBg->loadTexturePressed(m_buf);
    }
    else
    {
        sprintf(m_buf,"%sroom_%d.png",PLAZA_RES_SUFIX,nRoom);
        m_btnRoomBg->loadTextureNormal(m_buf, Widget::TextureResType::PLIST);
        m_btnRoomBg->loadTextureDisabled(m_buf, Widget::TextureResType::PLIST);
        m_btnRoomBg->loadTexturePressed(m_buf, Widget::TextureResType::PLIST);
    }
}

void CRoomItemView::setType(const int &nType)
{
    memset(m_buf,0,64);
    if ((DWORD)kind_niuniu == HallDataMgr::getInstance()->m_dwKindID)
    {
        sprintf(m_buf,"plaza/room_type_%d.png",nType);
        auto sp = Sprite::create(m_buf);
        if (nullptr != sp)
        {
            m_spRoomType->setSpriteFrame(sp->getSpriteFrame());
        }
    }
    else
    {
        sprintf(m_buf,"%sroom_type_%d.png",PLAZA_RES_SUFIX,nType);
        SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_buf);
        if (nullptr != frame)
        {
            m_spRoomType->setSpriteFrame(frame);
        }
    }
    
    m_spAntiCheating->setVisible(nType == 10);
}

void CRoomItemView::setDifen(const SCORE &difen)
{
    memset(m_buf,0,64);
    sprintf(m_buf,"%lld倍",difen);
    //m_labDiFen->setString(m_buf);
    m_textDiFen->setString(m_buf);
}

void CRoomItemView::setRuChang(const SCORE &ruchang)
{
    memset(m_buf,0,64);
    if (ruchang >= 10000 && ruchang < 100000000)
    {
        sprintf(m_buf,"%.2f万游戏币",(float)ruchang / 10000);
    }
    else if ( ruchang >= 100000000 )
    {
        sprintf(m_buf,"%.2f亿游戏币",(float)ruchang / 100000000);
    }
    else
    {
        sprintf(m_buf,"%lld游戏币",ruchang);
    }
    //m_labDiFen->setString(m_buf);
    m_textRuChang->setString(m_buf);
}

void CRoomItemView::setZaiXian(const int &zaixian)
{
    memset(m_buf,0,64);
    sprintf(m_buf,"%d人",zaixian);
    //m_labZaiXian->setString(m_buf);
    m_textZaiXian->setString(m_buf);
}

void CRoomItemView::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == tType)
    {
        Vec2 beginPos = pWidget->getTouchBeganPosition();
        Vec2 pos = pWidget->getTouchEndPosition();
        if (fabs(beginPos.x - pos.x) > DIS_BTN_ENABLE
            || fabs(beginPos.y - pos.y) > DIS_BTN_ENABLE)
        {
            return;
        }
        
        //pWidget->setEnabled(false);
        this->stopAllActions();
        this->runAction(m_actSelect);
    }
}