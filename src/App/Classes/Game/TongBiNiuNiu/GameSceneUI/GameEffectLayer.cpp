//
//  GameEffectLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#include "GameEffectLayer.h"
USING_NS_CC;
using namespace ui;
using namespace std;
USING_TBNN_NAMESPACE;

GameEffectLayer::GameEffectLayer()
{
    
}

GameEffectLayer::~GameEffectLayer()
{
    
}

bool GameEffectLayer::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("game/GameEffectLayer.csb");
        
        m_spGameTip = static_cast<Sprite*>(m_root->getChildByName("game_tip"));
        CC_ASSERT(m_spGameTip != nullptr);
        
        bRes = true;
    } while (false);
    return bRes;
}

void GameEffectLayer::reSet()
{
    m_spGameTip->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("blank.png"));
}

void GameEffectLayer::showGameTip(const GameEffectLayer::emGameTip &tip)
{
    SpriteFrame *pFrame = nullptr;
    
    switch (tip) {
        case emGameTip::kGAME_TIP_BLANK:
            pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("blank.png");
            break;
        case emGameTip::kGAME_TIP_ADDSCORE:
            break;
        case emGameTip::kGAME_TIP_OUTCARD:
            pFrame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("tips_wait_opencard.png");
            break;
        default:
            break;
    }
    if (nullptr == pFrame)
    {
        return;
    }
    m_spGameTip->setSpriteFrame(pFrame);
}