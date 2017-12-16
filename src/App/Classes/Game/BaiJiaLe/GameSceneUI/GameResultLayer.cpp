//
//  GameResultLayer.cpp
//  DouDiZhu
//
//  Created by zhong on 1/19/16.
//
//

#include "GameResultLayer.h"
#include "GameDataMgr.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;
USING_BJL_NAMESPACE;

enum RESULT_TAG
{
    TAG_RESULT_BASE = 1000,
    TAG_CONTINUE_MENU,
    TAG_QUIT_MENU,
    TAG_CLOSE_MENU
};

GameResultLayer::GameResultLayer():
m_root(nullptr),
m_notify(nullptr)
{
    for (int i = 0; i < AREA_MAX; ++i)
    {
        m_atlasScore[i] = nullptr;
    }
}

GameResultLayer::~GameResultLayer()
{
    this->removeAllChildrenWithCleanup(true);
}

bool GameResultLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        //读取csb
        LOAD_CSB("game/GameResultLayer.csb");
        
        auto csbBg = m_root->getChildByName("bg");
        CC_ASSERT ( csbBg != nullptr );
        
        auto scorelayout = csbBg->getChildByName("userscore_layout");
        CC_ASSERT(scorelayout != nullptr);
        
        char buf[32] = "";
        for (int i = 0; i < AREA_MAX; ++i)
        {
            memset(buf, 0, 32);
            sprintf(buf, "score%d_atlas",i);
            m_atlasScore[i] = static_cast<TextAtlas*>(scorelayout->getChildByName(buf));
            CC_ASSERT(m_atlasScore[i] != nullptr);
        }
        m_atlasTotal = static_cast<TextAtlas*>(csbBg->getChildByName("scroetotal_atlas"));
        CC_ASSERT(m_atlasTotal != nullptr);
        
        hideGameResult();
        bRes = true;
    } while (0);
    return bRes;
}

void GameResultLayer::hideGameResult()
{
    reSet();
    this->setVisible(false);
}

void GameResultLayer::showGameResult(const tagGameResult &rs)
{
    reSet();
    this->setVisible(true);
    
    char buf[64] = "";
    for (int i = 0; i < AREA_MAX; ++i)
    {
        memset(buf, 0, 64);
        LONGLONG lScore = rs.m_pAreaScore[i];
        float scale = abs(lScore) > 1000000 ? 0.8f : 1.0f;
        if (lScore > 0)
        {
            sprintf(buf, ".%lld", lScore);
        }
        else if (lScore < 0)
        {
            sprintf(buf, "/%lld", lScore);
        }
        m_atlasScore[i]->setScale(scale);
        m_atlasScore[i]->setString(buf);
    }
    
    memset(buf, 0, 64);
    if (rs.m_llTotal > 0)
    {
        sprintf(buf, ".%lld", rs.m_llTotal);
    }
    else if (rs.m_llTotal < 0)
    {
        sprintf(buf, "/%lld", rs.m_llTotal);
    }
    m_atlasTotal->setString(buf);
}

void GameResultLayer::onEnter()
{
    Layer::onEnter();
    
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    m_listener->onTouchBegan = [=](Touch *touch,Event *event) -> bool
    {
        return this->isVisible();
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener,this);
}

void GameResultLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    
    Layer::onExit();
}

void GameResultLayer::reSet()
{
    for (int i = 0; i < AREA_MAX; ++i)
    {
        m_atlasScore[i]->setScale(1.0f);
        m_atlasScore[i]->setString("");
    }
    m_atlasTotal->setString("");
}