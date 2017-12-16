//
//  BrowPanel.cpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#include "BrowPanel.h"
USING_NS_CC;
USING_NN_NAMESPACE;

const Vec2 BrowPanel::m_pos[40] =
{
    Vec2(0, 3),Vec2(1, 3),Vec2(2, 3),Vec2(3, 3),Vec2(4, 3),Vec2(5, 3),Vec2(6, 3),Vec2(7, 3),Vec2(8, 3),Vec2(9, 3),
    Vec2(0, 2),Vec2(1, 2),Vec2(2, 2),Vec2(3, 2),Vec2(4, 2),Vec2(5, 2),Vec2(6, 2),Vec2(7, 2),Vec2(8, 2),Vec2(9, 2),
    Vec2(0, 1),Vec2(1, 1),Vec2(2, 1),Vec2(3, 1),Vec2(4, 1),Vec2(5, 1),Vec2(6, 1),Vec2(7, 1),Vec2(8, 1),Vec2(9, 1),
    Vec2(0, 0),Vec2(1, 0),Vec2(2, 0),Vec2(3, 0),Vec2(4, 0),Vec2(5, 0),Vec2(6, 0),Vec2(7, 0),Vec2(8, 0),Vec2(9, 0),
};
BrowPanel::BrowPanel():
    m_bTouchable(true)
{
    
}

BrowPanel::~BrowPanel()
{
    
}

BrowPanel* BrowPanel::createPanel(const std::vector<int> &vecidx)
{
    BrowPanel *ptmp = new BrowPanel();
    if (nullptr != ptmp && ptmp->init(vecidx))
    {
        ptmp->autorelease();
        return ptmp;
    }
    CC_SAFE_DELETE(ptmp);
    return nullptr;
}

bool BrowPanel::init(const std::vector<int> &vecidx)
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        
        //表情底板
        auto sp = Sprite::create("game/face_bg.png");
        sp->setAnchorPoint(Vec2(0, 0));
        this->addChild(sp);
        
        char buf[24] = "";
        
        int idx = 0;
        //表情
        for (size_t i = 0; i < vecidx.size(); ++i)
        {
            idx = vecidx[i];
            if (idx < 10)
            {
                sprintf(buf, "e0%d.png",idx);
            }
            else
            {
                sprintf(buf, "e%d.png",idx);
            }
            TouchSprite *face = TouchSprite::createWith(buf);
            if (nullptr == face)
            {
                continue;
            }
            face->setTag(idx);
            face->setPosition(m_pos[i].x * 48 + 24,m_pos[i].y * 48 + 24);
            face->setTouchDelegate(this);
            sp->addChild(face);
            
        }
        
        this->setContentSize(sp->getContentSize());
        bRes = true;
    } while (false);
    return bRes;
}

bool BrowPanel::isTouchable(TouchSprite *sp)
{
    return m_bTouchable;
}

void BrowPanel::onTouchSpriteEnd(TouchSprite *sp)
{
    if (nullptr != sp && nullptr != m_notify)
    {
        int idx = sp->getTag() - 1;
        //通知界面
        m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::BROW_CHAT, (void*)&idx, sizeof(int));
    }
}