//
//  BankerNode.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/1/16.
//
//

#include "BankerNode.h"
#include "HallDataMgr.h"

USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

BankerNode::BankerNode():
m_wChair(0)
{
    m_pUserHead = nullptr;
}

BankerNode::~BankerNode()
{
    m_actTransition->release();
}

bool BankerNode::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF( !Node::init() );
        
        LOAD_CSB("game/BankerNode.csb");
        
        m_spBg = static_cast<Sprite*>(m_root->getChildByName("banker_sp"));
        CC_ASSERT(m_spBg != nullptr);
        
        m_textScore = static_cast<Text*>(m_spBg->getChildByName("score_text"));
        CC_ASSERT(m_textScore != nullptr);
        
        m_clipBankerName = ClipText::createClipText(Size(160, 23), "");
        CC_ASSERT(m_clipBankerName != nullptr);
        m_clipBankerName->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_clipBankerName->setPosition(Vec2(204,23));
        m_spBg->addChild(m_clipBankerName);
        
        m_spHeadBg = static_cast<Sprite*>(m_spBg->getChildByName("head_bg"));
        CC_ASSERT(m_spHeadBg != nullptr);
        
        auto sca = ScaleTo::create(0.2f, 0.0001f, 1.0f);
        auto sca2 = ScaleTo::create(0.2f, 1.0f, 1.0f);
        m_actTransition = Sequence::create(sca, sca2, NULL);
        m_actTransition->retain();
        
        bRes = true;
    } while (false);
    return bRes;
}

void BankerNode::refreshBanker(const WORD &wChair,  LONGLONG lScore, const bool &bSystemBanker)
{
    UserData *pUser = HallDataMgr::getInstance()->getUserDataByChair(wChair);
    if (nullptr == pUser && !bSystemBanker)
    {
        return;
    }
    m_wChair = wChair;
    if (nullptr != m_pUserHead)
    {
        m_pUserHead->removeFromParentAndCleanup(true);
        m_pUserHead = nullptr;
    }
    std::string nick = "";
    if (INVALID_CHAIR == wChair && bSystemBanker)
    {
        nick = "系统坐庄";
        m_pUserHead = HeaderRequest::createwithFaceID(0, 0, 0, 0);
    }
    else
    {
        nick = pUser->m_nickname;
        
        //头像
        if (pUser->m_date.dwUserID == HallDataMgr::getInstance()->m_dwUserID)
        {
            if (HallDataMgr::getInstance()->m_loadtype == Load_Normal || HallDataMgr::getInstance()->m_loadtype == Load_Visitor)
            {
                m_pUserHead = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                              HallDataMgr::getInstance()->m_wCustom,
                                                              HallDataMgr::getInstance()->m_dwUserID,
                                                              HallDataMgr::getInstance()->m_cbGender);
            }
            else if (HallDataMgr::getInstance()->m_loadtype == Load_RenRen || HallDataMgr::getInstance()->m_loadtype == Load_Sina)
            {
                m_pUserHead = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
            }
        }
        else
        {
            m_pUserHead = HeaderRequest::createwithFaceID(pUser->m_date.wFaceID,
                                                          pUser->m_date.dwCustomID,
                                                          pUser->m_date.dwUserID,
                                                          pUser->m_date.cbGender);
        }
    }
    
    m_pUserHead->setHeadSize(65);
    m_pUserHead->setPosition(m_spHeadBg->getPosition());
    m_spBg->addChild(m_pUserHead);
    
    //名字
    m_clipBankerName->setString(nick);
    
    //积分
    refreshBankerScore(lScore);
    
    //切换动画
    m_spBg->stopAllActions();
    m_spBg->runAction(m_actTransition);
}

void BankerNode::refreshBankerScore( LONGLONG lScore)
{
    char buf[64] = "";
    sprintf(buf, "%lld", lScore);
    std::string str = buf;
    size_t len = str.length();
    for (int i = (int)len - 3; i > 0; i -= 3)
    {
        str.insert(i, ",");
    }
    if (str.length() > 14)
    {
        str = str.substr(0, 14);
        str.append("...");
    }
    m_textScore->setString(str);
}