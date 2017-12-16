//
//  GameHelpLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/30.
//
//

#include "GameHelpLayer.h"
#include "GameDataMgr.h"
#include "CMD_Stdafx.h"
USING_NS_CC;
using namespace ui;
USING_LKPY_NAMESPACE;
bool GameHelpLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_layout= static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/GameHelpLayer.json"));
    this->addChild(m_layout,100);
    
    Button* exitBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "exitBtn");
    exitBtn->addTouchEventListener(this, toucheventselector(GameHelpLayer::exitCallBack));
    
    Button* fishRewardBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "fishRewardBtn");
    fishRewardBtn->addTouchEventListener(this, toucheventselector(GameHelpLayer::openFishCallBack));
    
    Button* supplyBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "supplyBtn");
    supplyBtn->addTouchEventListener(this, toucheventselector(GameHelpLayer::openSupplyCallBack));
    
    Button* operationBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "operationBtn");
    operationBtn->addTouchEventListener(this, toucheventselector(GameHelpLayer::openHelpCallBack));
    
    supply = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "supply");
    help = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "help");
    fish = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "fish");
    
    return true;
}

void GameHelpLayer::exitCallBack(Ref *pSender, ui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        this->removeFromParentAndCleanup(true);
    }
}

void GameHelpLayer::openFishCallBack(Ref *pSender, ui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        fish->setVisible(true);
        help->setVisible(false);
        supply->setVisible(false);
    }
}

void GameHelpLayer::openHelpCallBack(Ref *pSender, ui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        fish->setVisible(false);
        help->setVisible(true);
        supply->setVisible(false);
    }
}

void GameHelpLayer::openSupplyCallBack(Ref *pSender, ui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        fish->setVisible(false);
        help->setVisible(false);
        supply->setVisible(true);
    }
}



