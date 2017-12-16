//
//  GameRoleInfoLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/31/15.
//
//

#include "GameRoleInfoLayer.h"
USING_NS_CC;
using namespace ui;
USING_TBNN_NAMESPACE;

GameRoleInfoLayer::GameRoleInfoLayer():
m_pHead(nullptr)
{
    
}

GameRoleInfoLayer::~GameRoleInfoLayer()
{
    
}

bool GameRoleInfoLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("game/GameRoleInfo.csb");
        
        m_spBg = static_cast<Sprite*>(m_root->getChildByName("bg_kuang"));
        CC_ASSERT(m_spBg != nullptr);
        
        m_spHeadBg = static_cast<Sprite*>(m_root->getChildByName("head_bg"));
        CC_ASSERT(m_spHeadBg != nullptr);
        
        m_spGender = static_cast<Sprite*>(m_root->getChildByName("gender_sp"));
        CC_ASSERT(m_spGender != nullptr);
        
        m_textName = static_cast<Text*>(m_root->getChildByName("name_text"));
        CC_ASSERT(m_textName != nullptr);
        
        m_textId = static_cast<Text*>(m_root->getChildByName("id_text"));
        CC_ASSERT(m_textId != nullptr);
        
        m_textMoney = static_cast<Text*>(m_root->getChildByName("money_text"));
        CC_ASSERT(m_textMoney != nullptr);
        
        m_textWin = static_cast<Text*>(m_root->getChildByName("win_text"));
        CC_ASSERT(m_textWin != nullptr);
        
        bRes = true;
    } while (false);
    return bRes;
}

void GameRoleInfoLayer::onEnterTransitionDidFinish()
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
            this->setVisible(false);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void GameRoleInfoLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Layer::onExit();
}

void GameRoleInfoLayer::refresh(UserData *pUser)
{
    this->setVisible(true);
    //头像
    if (nullptr == m_pHead)
    {
        m_pHead = HeaderRequest::createwithFaceID(pUser->m_date.wFaceID,
                                                  pUser->m_date.dwCustomID,
                                                  pUser->m_date.dwUserID,
                                                  pUser->m_date.cbGender);
        //m_pHead->setHeadSize(80.0f);
        m_pHead->setPosition(m_spHeadBg->getPosition());
        m_root->addChild(m_pHead);
    }
    else
    {
        if (0 != pUser->m_date.dwCustomID)
        {
            auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(pUser->m_date.dwUserID);
            if (ptexture)
            {
                m_pHead->setTexture(ptexture);
                m_pHead->setTextureRect(cocos2d::Rect(0, 0, FACE_CX, FACE_CY));
            }
        }
        else
        {
            auto faceID = pUser->m_date.wFaceID%10;
            std::string headstr = 1 == pUser->m_date.cbGender? "man" : "woman";
            headstr = __String::createWithFormat("public_res/im_head_%s_%d.png", headstr.c_str(), faceID)->getCString();
            m_pHead->initWithFile(headstr);
        }
    }
    m_pHead->setHeadSize(160);
    
    //性别图标
    char buf[64] = "";
    sprintf(buf, "game/player_gender_%d.png",pUser->m_date.cbGender);
    auto frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName(buf);
    if (nullptr != frame)
    {
        m_spGender->setSpriteFrame(frame);
    }
    
    //姓名
    m_textName->setString(pUser->m_nickname);
    
    //id
    sprintf(buf, "%d",pUser->m_date.dwGameID);
    m_textId->setString(buf);
    
    //金币
    sprintf(buf, "%lld",pUser->m_date.lScore);
    m_textMoney->setString(buf);
    
    //胜负
    DWORD win = pUser->m_date.dwWinCount;
    DWORD lost = pUser->m_date.dwLostCount;
    DWORD total = win + lost + pUser->m_date.dwDrawCount + pUser->m_date.dwFleeCount;
    DWORD per = ((float)win / total) * 100;
    sprintf(buf, "%d%% %d胜 %d负",per,win,lost);
    m_textWin->setString(buf);
}