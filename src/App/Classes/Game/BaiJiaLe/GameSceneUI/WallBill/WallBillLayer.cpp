//
//  WallBillLayer.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/7/16.
//
//

#include "WallBillLayer.h"
#include "GameDataMgr.h"
#include "GameRecordMgr.h"

USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

WallBillLayer::WallBillLayer()
{
    for (int i = 0; i < 29; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            m_spRecrod[i][j] = nullptr;
        }
    }
    
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            m_spRecord2[i][j] = nullptr;
        }
    }
}

WallBillLayer::~WallBillLayer()
{
    
}

bool WallBillLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF( !Layer::init() );
        
        auto tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/game_ludan1_zhuang.png");
        CC_ASSERT(tex != nullptr);
        auto frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 19, 20));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/game_ludan1_zhuang.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/game_ludan1_xian.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 19, 20));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/game_ludan1_xian.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/game_ludan1_ping.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 19, 20));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/game_ludan1_ping.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/game_ludan2_zhuang.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 44, 45));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/game_ludan2_zhuang.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/game_ludan2_xian.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 44, 45));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/game_ludan2_xian.png");
        
        tex = INSTANCE(Director)->getTextureCache()->getTextureForKey("game/game_ludan2_ping.png");
        CC_ASSERT(tex != nullptr);
        frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, 44, 45));
        INSTANCE(SpriteFrameCache)->addSpriteFrame(frame, "game/game_ludan2_ping.png");
        
        LOAD_CSB("game/WallBill.csb");
        
        m_layoutBillSp = static_cast<Layout*>(m_root->getChildByName("bill_layout"));
        CC_ASSERT(m_layoutBillSp != nullptr);
        char buf[32] = "";
        for (int i = 0; i < 29; ++i)
        {
            memset(buf, 0, 32);
            sprintf(buf, "record%d_sp",i);
            m_spRecrod[i][0] = static_cast<Sprite*>(m_layoutBillSp->getChildByName(buf));
        }
        
        m_layoutBillSp2 = static_cast<Layout*>(m_root->getChildByName("bill2_layout"));
        CC_ASSERT(m_layoutBillSp2 != nullptr);
        for (int i =0 ; i < 14; ++i)
        {
            memset(buf, 0, 32);
            sprintf(buf, "record%d_sp",i);
            m_spRecord2[i][0] = static_cast<Sprite*>(m_layoutBillSp2->getChildByName(buf));
        }
        
        m_layoutSettle = static_cast<Layout*>(m_root->getChildByName("settle_layout"));
        CC_ASSERT(m_layoutSettle != nullptr);
        m_textXianSettle = static_cast<Text*>(m_layoutSettle->getChildByName("xian_settle"));
        CC_ASSERT(m_textXianSettle != nullptr);
        m_textZhuangSettle = static_cast<Text*>(m_layoutSettle->getChildByName("zhuang_settle"));
        CC_ASSERT(m_textZhuangSettle != nullptr);
        m_textPingSettle = static_cast<Text*>(m_layoutSettle->getChildByName("ping_settle"));
        CC_ASSERT(m_textPingSettle != nullptr);
        
        m_textXianDoubleCount = static_cast<Text*>(m_layoutSettle->getChildByName("xiandouble_count"));
        CC_ASSERT(m_textXianDoubleCount != nullptr);
        m_textZhuangDoubleCount = static_cast<Text*>(m_layoutSettle->getChildByName("zhuangdouble_count"));
        CC_ASSERT(m_textZhuangDoubleCount != nullptr);
        m_textXianTianCount = static_cast<Text*>(m_layoutSettle->getChildByName("xiantian_count"));
        CC_ASSERT(m_textXianTianCount != nullptr);
        m_textZhuangTianCount = static_cast<Text*>(m_layoutSettle->getChildByName("zhuangtian_count"));
        CC_ASSERT(m_textZhuangTianCount != nullptr);
        
        m_spBg = static_cast<Sprite*>(m_root->getChildByName("game_ludan_bg"));
        CC_ASSERT(m_spBg != nullptr);
        
        for (int i = 0; i < 6; ++i)
        {
            m_vec2Pos[i].y = 532 - i * 25;
            m_vec2Pos2[i].y = 363 - i * 52;
        }
        
        reSet();
        bRes = true;
    } while (false);
    return bRes;
}

void WallBillLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    
    m_listener->onTouchBegan = [this](Touch *pTouch,Event *event)
    {
        return this->isVisible();
    };
    m_listener->onTouchEnded = [this](Touch *pTouch,Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        pos = m_spBg->convertToNodeSpace(pos);
        Rect rec = Rect(0, 0, m_spBg->getContentSize().width, m_spBg->getContentSize().height);
        if (!rec.containsPoint(pos))
        {
            showLayer(false);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void WallBillLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Layer::onExit();
}

void WallBillLayer::reSet()
{
    for (int i = 0; i < 29; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (nullptr != m_spRecrod[i][j])
            {
                m_spRecrod[i][j]->setSpriteFrame(INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png"));
            }
        }
    }
    
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (nullptr != m_spRecord2[i][j])
            {
                m_spRecord2[i][j]->setSpriteFrame(INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png"));
            }
        }
    }
    
    m_textXianSettle->setString("");
    m_textPingSettle->setString("");
    m_textZhuangSettle->setString("");
    
    m_textXianDoubleCount->setString("");
    m_textZhuangDoubleCount->setString("");
    m_textXianTianCount->setString("");
    m_textZhuangTianCount->setString("");
}

void WallBillLayer::refreshWallBillList()
{
    std::vector<tagGameWallBill> vec = INSTANCE(GameRecordMgr)->getWallBills();
    m_nBeginIdx = vec.size() > 29 ? vec.size() - 29 : 0;
    reSet();
    
    refreshList();
    
    std::vector<tagGameRecord> vecRecord = INSTANCE(GameRecordMgr)->getRecords();
    //统计数据
    int nTotal = vec.size();
    int nXian = 0;
    int nZhuang = 0;
    int nPing = 0;
    int nXianDouble = 0;
    int nZhuangDouble = 0;
    int nXianTian = 0;
    int nZhuangTian = 0;
    for (int i = 0; i < nTotal; ++i)
    {
        tagGameRecord rec = vecRecord[i];
        switch (rec.m_cbGameResult)
        {
            case AREA_XIAN:
                ++nXian;
                break;
            case AREA_PING:
                ++nPing;
                break;
            case AREA_ZHUANG:
                ++nZhuang;
                break;
            default:
                break;
        }
        
        if (rec.m_pServerRecord->bBankerTwoPair)
        {
            ++nZhuangDouble;
        }
        if (rec.m_pServerRecord->bPlayerTwoPair)
        {
            ++nXianDouble;
        }
        if (AREA_XIAN_TIAN == rec.m_pServerRecord->cbKingWinner)
        {
            ++nXianTian;
        }
        if (AREA_ZHUANG_TIAN == rec.m_pServerRecord->cbKingWinner)
        {
            ++nZhuangTian;
        }
    }
    char buf[32] = "";
    sprintf(buf, "%d    %.2f%%",nXian, ((float)nXian/nTotal) * 100);
    m_textXianSettle->setString(buf);
    
    sprintf(buf, "%d    %.2f%%",nPing, ((float)nPing/nTotal) * 100);
    m_textPingSettle->setString(buf);
    
    sprintf(buf, "%d    %.2f%%",nZhuang, ((float)nZhuang/nTotal) * 100);
    m_textZhuangSettle->setString(buf);
    
    sprintf(buf, "%d",nXianDouble);
    m_textXianDoubleCount->setString(buf);
    
    sprintf(buf, "%d",nZhuangDouble);
    m_textZhuangDoubleCount->setString(buf);
    
    sprintf(buf, "%d",nXianTian);
    m_textXianTianCount->setString(buf);
    
    sprintf(buf, "%d",nZhuangTian);
    m_textZhuangTianCount->setString(buf);
    
    showLayer(true);
}

void WallBillLayer::showLayer(bool bShow)
{
    this->setVisible(bShow);
}

void WallBillLayer::refreshList()
{
    std::vector<tagGameWallBill> vec = INSTANCE(GameRecordMgr)->getWallBills();
    
    int nCount = 0;
    std::string str = "";
    for (int i = m_nBeginIdx; i < vec.size(); ++i, ++nCount)
    {
        //显示数量控制
        if (nCount > 29)
        {
            break;
        }
        tagGameWallBill bill = vec[i];
        for (int j = 0; j <= bill.m_cbIndex; ++j)
        {
            //数量控制
            if (j > 4)
            {
                break;
            }
            str = "";
            if (AREA_XIAN == bill.m_pRecords[j])
            {
                str = "game/game_ludan1_xian.png";
            }
            else if (AREA_ZHUANG == bill.m_pRecords[j])
            {
                str = "game/game_ludan1_zhuang.png";
            }
            else if (AREA_PING == bill.m_pRecords[j])
            {
                str = "game/game_ludan1_ping.png";
            }
            
            SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(str);
            if (nullptr != frame)
            {
                if (nullptr == m_spRecrod[nCount][j])
                {
                    m_spRecrod[nCount][j] = Sprite::createWithSpriteFrame(frame);
                    m_layoutBillSp->addChild(m_spRecrod[nCount][j]);
                }
                else
                {
                    m_spRecrod[nCount][j]->setSpriteFrame(frame);
                }
                
                m_spRecrod[nCount][j]->setPosition(m_spRecrod[nCount][0]->getPositionX(), m_vec2Pos[j].y);
            }
        }
        
        if( bill.m_bWinList)
        {
            if (AREA_XIAN == bill.m_pRecords[5])
            {
                str = "game/game_ludan1_xian.png";
            }
            else if (AREA_ZHUANG == bill.m_pRecords[5])
            {
                str = "game/game_ludan1_zhuang.png";
            }
            else if (AREA_PING == bill.m_pRecords[5])
            {
                str = "game/game_ludan1_ping.png";
            }
            
            SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(str);
            if (nullptr != frame)
            {
                if (nullptr == m_spRecrod[nCount][5])
                {
                    m_spRecrod[nCount][5] = Sprite::createWithSpriteFrame(frame);
                    m_layoutBillSp->addChild(m_spRecrod[nCount][5]);
                }
                else
                {
                    m_spRecrod[nCount][5]->setSpriteFrame(frame);
                }
                
                m_spRecrod[nCount][5]->setPosition(m_spRecrod[nCount][0]->getPositionX(), m_vec2Pos[5].y);
            }
        }
    }
    
    std::vector<tagGameRecord> vec2 = INSTANCE(GameRecordMgr)->getRecords();
    nCount = 0;
    int subCount = 0;
    int nBegin = vec2.size() > 84 ? vec2.size() - 84 : 0;
    for (int i = nBegin; i < vec2.size(); ++i)
    {
        //显示数量控制
        if (nCount > 13)
        {
            break;
        }
        tagGameRecord rec = vec2[i];
        str = "";
        if (AREA_XIAN == rec.m_cbGameResult)
        {
            str = "game/game_ludan2_xian.png";
        }
        else if (AREA_ZHUANG == rec.m_cbGameResult)
        {
            str = "game/game_ludan2_zhuang.png";
        }
        else if (AREA_PING == rec.m_cbGameResult)
        {
            str = "game/game_ludan2_ping.png";
        }
        
        SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(str);
        if (nullptr != frame)
        {
            if (nullptr == m_spRecord2[nCount][subCount])
            {
                m_spRecord2[nCount][subCount] = Sprite::createWithSpriteFrame(frame);
                m_layoutBillSp->addChild(m_spRecord2[nCount][subCount]);
            }
            else
            {
                m_spRecord2[nCount][subCount]->setSpriteFrame(frame);
            }
            m_spRecord2[nCount][subCount]->setPosition(m_spRecord2[nCount][0]->getPositionX(), m_vec2Pos2[subCount].y);
        }
        
        ++subCount;
        if (subCount > 5)
        {
            subCount = 0;
            ++nCount;
        }
    }
}