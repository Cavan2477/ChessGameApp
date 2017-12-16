//
//  BJLSimpleWallBill.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/4/16.
//
//

#include "SimpleWallBillLayer.h"
#include "GameDataMgr.h"
#include "GameRecordMgr.h"

USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

enum enSimpleWallBillTag
{
    kTagDefault = 100,
    kLastRecord_btn,
    kNextRecord_btn,
};

SimpleWallBillLayer::SimpleWallBillLayer():
m_notify(nullptr),
m_nBeginIdx(0)
{
    
}

SimpleWallBillLayer::~SimpleWallBillLayer()
{
    
}

bool SimpleWallBillLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF( !Layer::init() );
        
        LOAD_CSB("game/SimpleWallBill.csb");
        
        m_btnLast = static_cast<Button*>(m_root->getChildByName("left_btn"));
        CC_ASSERT( m_btnLast != nullptr );
        m_btnLast->setTag(kLastRecord_btn);
        m_btnLast->addTouchEventListener(CC_CALLBACK_2(SimpleWallBillLayer::touchEvent, this));
        
        m_btnNext = static_cast<Button*>(m_root->getChildByName("right_btn"));
        CC_ASSERT( m_btnNext != nullptr );
        m_btnNext->setTag(kNextRecord_btn);
        m_btnNext->addTouchEventListener(CC_CALLBACK_2(SimpleWallBillLayer::touchEvent, this));
        
        m_spRecordBg = static_cast<Sprite*>(m_root->getChildByName("record_sp"));
        CC_ASSERT(m_spRecordBg != nullptr);
        
        m_textMasterWin = static_cast<Text*>(m_spRecordBg->getChildByName("master_text"));
        CC_ASSERT(m_textMasterWin != nullptr);
        
        m_textIdleWin = static_cast<Text*>(m_spRecordBg->getChildByName("idle_text"));
        CC_ASSERT(m_textIdleWin != nullptr);
        
        m_textDraw = static_cast<Text*>(m_spRecordBg->getChildByName("draw_text"));
        CC_ASSERT(m_textDraw != nullptr);
        
        m_vec2Pos[kSelectDraw] = m_textDraw->getPosition();
        m_vec2Pos[kSelectIdleWin] = m_textIdleWin->getPosition();
        m_vec2Pos[kSelectMasterWin] = m_textMasterWin->getPosition();
        
        char buf[32] = "";
        for (int i = 0; i < 15; ++i)
        {
            m_spRecrod[i] = nullptr;
            memset(buf,0, 32);
            sprintf(buf, "record%d_sp",i);
            m_spRecrod[i] = static_cast<Sprite*>(m_spRecordBg->getChildByName(buf));
            CC_ASSERT(m_spRecrod[i] != nullptr);
        }
        
        reSet();
        
        bRes = true;
        
    } while (false);
    return bRes;
}

void SimpleWallBillLayer::reSet()
{
    for (int i = 0; i < 15; ++i)
    {
        m_spRecrod[i]->setSpriteFrame(INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png"));
    }
    m_textDraw->setString("");
    m_textIdleWin->setString("");
    m_textMasterWin->setString("");
}

void SimpleWallBillLayer::refreshRecordList()
{
    std::vector<tagGameRecord> vec = INSTANCE(GameRecordMgr)->getRecords();
    m_nBeginIdx = vec.size() - 1;
    refreshList();
    
    //统计数据
    int nMasterWin = 0;
    int nIdleWin = 0;
    int nDraw = 0;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        tagGameRecord rec = vec[i];
        if (rec.m_pServerRecord->cbBankerCount > rec.m_pServerRecord->cbPlayerCount) //庄赢
        {
            ++nMasterWin;
        }
        else if (rec.m_pServerRecord->cbBankerCount < rec.m_pServerRecord->cbPlayerCount) //闲赢
        {
            ++nIdleWin;
        }
        else //平局
        {
            ++nDraw;
        }
    }
    
    char buf[32] = "";
    if (nMasterWin < 10)
    {
        sprintf(buf, "0%d",nMasterWin);
    }
    else
    {
        sprintf(buf, "%d",nMasterWin);
    }
    m_textMasterWin->setString(buf);
    
    if (nIdleWin < 10)
    {
        sprintf(buf, "0%d",nIdleWin);
    }
    else
    {
        sprintf(buf, "%d",nIdleWin);
    }
    m_textIdleWin->setString(buf);
    
    if (nDraw < 10)
    {
        sprintf(buf, "0%d",nDraw);
    }
    else
    {
        sprintf(buf, "%d",nDraw);
    }
    m_textDraw->setString(buf);
}

void SimpleWallBillLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == tType)
    {
        switch (pWidget->getTag())
        {
            case kLastRecord_btn:
            {
                --m_nBeginIdx;
                if (m_nBeginIdx < 15)
                {
                    m_nBeginIdx = 15;
                }
                else
                {
                    refreshList();
                }
            }
                break;
            case kNextRecord_btn:
            {
                ++m_nBeginIdx;
                if (m_nBeginIdx > INSTANCE(GameRecordMgr)->getRecords().size() - 1)
                {
                    m_nBeginIdx = (int)(INSTANCE(GameRecordMgr)->getRecords().size() - 1);
                }
                else
                {
                    refreshList();
                }
            }
                break;
            default:
                break;
        }
    }
}

void SimpleWallBillLayer::refreshList()
{
    std::vector<tagGameRecord> vec = INSTANCE(GameRecordMgr)->getRecords();
    std::string str = "";
    float fYPos = 0.0f;
    int nCount = 0;
    
    for (int i = m_nBeginIdx; i >= 0; --i, ++nCount)
    {
        //显示15个数据
        if (nCount > 14)
        {
            break;
        }
        
        tagGameRecord rec = vec[i];
        if (rec.m_pServerRecord->cbBankerCount > rec.m_pServerRecord->cbPlayerCount) //庄赢
        {
            fYPos = m_vec2Pos[kSelectMasterWin].y;
            if (rec.m_tagUserRecord.m_bJoin)
            {
                if (rec.m_tagUserRecord.m_bWin)
                {
                    str = "game/room_single_win_1.png";
                }
                else
                {
                    str = "game/room_single_lose.png";
                }
            }
            else
            {
                str = "game/room_single_win.png";
            }
        }
        else if (rec.m_pServerRecord->cbBankerCount < rec.m_pServerRecord->cbPlayerCount) //闲赢
        {
            fYPos = m_vec2Pos[kSelectIdleWin].y;
            if (rec.m_tagUserRecord.m_bJoin)
            {
                if (rec.m_tagUserRecord.m_bWin)
                {
                    str = "game/room_single_win_1.png";
                }
                else
                {
                    str = "game/room_single_lose.png";
                }
            }
            else
            {
                str = "game/room_single_win.png";
            }
        }
        else //平局
        {
            fYPos = m_vec2Pos[kSelectDraw].y;
            if (rec.m_tagUserRecord.m_bJoin)
            {
                if (rec.m_tagUserRecord.m_bWin)
                {
                    str = "game/room_single_win_1.png";
                }
                else
                {
                    str = "game/room_single_lose.png";
                }
            }
            else
            {
                str = "game/room_single_win.png";
            }
        }
        m_spRecrod[nCount]->setSpriteFrame(str);
        m_spRecrod[nCount]->setPositionY(fYPos);
    }
}