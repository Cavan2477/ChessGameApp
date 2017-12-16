//
//  DistributeLayer.cpp
//  gswz
//
//  Created by zsh on 16/3/14.
//
//

#include "zjh_DistributeLayer.h"
#include "zjh_RoomLayer.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "zjh_DeskLayer.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "zjh_GameDefine.h"
#include <functional>
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
USING_ZJH_NAMESPACE;
enum ButtonTag
{
    Tag_Bt_Distribute,
    Tag_Bt_Exit,
};

Scene* DistributeLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DistributeLayer::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool DistributeLayer::init()
{
    if(!Layer::init())
        return false;
    
    m_layout= static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/distributeLayer.json"));
    this->addChild(m_layout,1);
    
    Button *distributeBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "distributeBtn");
    distributeBtn->setTag(Tag_Bt_Distribute);
    distributeBtn->addTouchEventListener(CC_CALLBACK_2(DistributeLayer::touchEvent, this));
    
    Button *exitBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "exitBtn");
    exitBtn->setTag(Tag_Bt_Exit);
    exitBtn->addTouchEventListener(CC_CALLBACK_2(DistributeLayer::touchEvent, this));
    
    return true;
}

void DistributeLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto button = (Button *)pSender;
        int tag = button->getTag();
        switch (tag)
        {
            case Tag_Bt_Distribute:
            {
                //HallDataMgr::getInstance()->AddpopLayer("", "正在请求分组。。。", Type_Wait_Text);
                HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                NetworkMgr::getInstance()->sendSitDown(INVALID_DWORD, INVALID_DWORD, "");
            }
                break;
            case Tag_Bt_Exit:
            {
                Director::getInstance()->replaceScene(TransitionFade::create(0.3f, RoomLayer::createScene()));
                NetworkMgr::getInstance()->Disconnect(Data_Room);
                memset(&HallDataMgr::getInstance()->m_Tableinfo, 0, sizeof(CMD_GR_TableInfo));

            }
                break;
                default:
                break;
        }
    }
}