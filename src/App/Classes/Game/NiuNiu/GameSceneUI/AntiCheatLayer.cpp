//
//  AntiCheatLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/24/15.
//
//

#include "AntiCheatLayer.h"
#include "GameDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_NN_NAMESPACE;

AntiCheatLayer::AntiCheatLayer():
m_notify(nullptr)
{
    
}

AntiCheatLayer::~AntiCheatLayer()
{
    
}

bool AntiCheatLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("game/AntiCheatLayer.csb");
        
        m_btnJoin = static_cast<Button*>(m_root->getChildByName("join_btn"));
        CC_ASSERT(m_btnJoin != nullptr);
        m_btnJoin->addTouchEventListener(CC_CALLBACK_2(AntiCheatLayer::touchEvent, this));
        
        m_btnExit = static_cast<Button*>(m_root->getChildByName("exit_btn"));
        CC_ASSERT(m_btnExit != nullptr);
        m_btnExit->addTouchEventListener(CC_CALLBACK_2(AntiCheatLayer::touchEvent, this));
        bRes = true;
    } while (false);
    return bRes;
}

void AntiCheatLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType)
{
    TOUCH_EVENT
    
    if (Widget::TouchEventType::ENDED == tType)
    {
        if (0 == strcmp("join_btn", nodename.c_str()))
        {
            //INSTANCE(GameDataMgr)->randomSit();
            INSTANCE(GameDataMgr)->requestSitdown(INVALID_TABLE, INVALID_CHAIR);
            
            INSTANCE(GameDataMgr)->setAntiCheatJoin(true);
            //this->removeFromParentAndCleanup(true);
        }
        else if (0 == strcmp("exit_btn", nodename.c_str()))
        {
            //通知离开界面
            /*INSTANCE(MsgDispatcher)->addDataToCallBackQueue(QUEUE_MSG_BASE::QUEUE_MSG_USER,USER_QUEUE::USER_LEAVE,nullptr,0);*/
            this->removeFromParentAndCleanup(true);
        }
    }
}