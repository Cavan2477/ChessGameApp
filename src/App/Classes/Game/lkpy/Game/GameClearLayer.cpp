//
//  GameClearLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/24.
//
//
//#include "CocoStudio/Reader/GUIReader.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "GameClearLayer.h"
#include "HallDataMgr.h"
#include "lkpy_GameDataMgr.h"
#include "NetworkMgr.h"
#include "GameLayer.h"
#include "lkpy_Help.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
USING_LKPY_NAMESPACE;
enum ButtonTag
{
    Tag_Quit = 0,
    Tag_Return,
};

bool GameClearLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    m_layout= static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/GameClearLayer.json"));
    this->addChild(m_layout,100);
    
    ImageView *gameBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "gameBg");
    
    
    Button* exitBtn = (ui::Button*)ui::Helper::seekWidgetByName(gameBg, "exitBtn");
    exitBtn->addTouchEventListener(this, toucheventselector(GameClearLayer::exitCallBack));
    
    Button* backBtn = (ui::Button*)ui::Helper::seekWidgetByName(gameBg, "backBtn");
    backBtn->addTouchEventListener(this, toucheventselector(GameClearLayer::backCallBack));
    

    
    for(int index = 0;index < 21; ++index)
    {
        auto labelStr = __String::createWithFormat("fishCatchNum_%d",index+1)->getCString();
        TextAtlas * catchNum = (ui::TextAtlas*)ui::Helper::seekWidgetByName(gameBg, labelStr);
        auto pstring = __String::createWithFormat(".%d",GameDataMgr::getInstance()->m_sceneData.lPlayFishCount[HallDataMgr::getInstance()->m_wChairID][index])->getCString();
        catchNum->setString(string(pstring));
    }
    
    
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    
    TextAtlas* bulletValue = (ui::TextAtlas*)ui::Helper::seekWidgetByName(gameBg, "bulletValue");
    
    TextAtlas* totalGoldValue = (ui::TextAtlas*)ui::Helper::seekWidgetByName(gameBg, "totalGoldValue");
    
    TextAtlas* fishCatchValue = (ui::TextAtlas*)ui::Helper::seekWidgetByName(gameBg, "fishCatchValue");
   
    if (puser)
    {
        auto ngetnum = puser->m_date.lScore - GameDataMgr::getInstance()->m_sceneData.lPlayStartScore[puser->m_date.wChairID] + GameDataMgr::getInstance()->m_sceneData.lBulletConsume[puser->m_date.wChairID];
        string fishValueStr = getGoldStr(__String::createWithFormat("%lld",ngetnum)->getCString(), '/');
        fishCatchValue->setString(fishValueStr);
        
        string bulletValueStr = getGoldStr(
                                           __String::createWithFormat("%lld",GameDataMgr::getInstance()->m_sceneData.lBulletConsume[puser->m_date.wChairID])->getCString()
                                           , '/');
        bulletValue->setString(bulletValueStr);
        
        string totalValueStr = getGoldStr(__String::createWithFormat("%lld",puser->m_date.lScore)->getCString(), '/');
        totalGoldValue->setString(totalValueStr);

    }
    
    return true;
}

void GameClearLayer::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = CC_CALLBACK_2(GameClearLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameClearLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameClearLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -100);
    _touchListener = listener;
}

void GameClearLayer::onExit()
{
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    Layer::onExit();
}

bool GameClearLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    GameLayer* layer = (GameLayer*)getParent();
    layer->setSecondCount(60);
    return false;
}

void GameClearLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    GameLayer* pLayer = (GameLayer*)this->getParent();
    pLayer->setSecondCount(60);
}

void GameClearLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    GameLayer* pLayer = (GameLayer*)this->getParent();
    pLayer->setSecondCount(60);
}


Widget::ccWidgetTouchCallback GameClearLayer::onLocateTouchCallback(const string &callBackName)
{
    if(callBackName == "onTouch")
    {
        return CC_CALLBACK_2(GameClearLayer::onTouch,this);
    }
    return nullptr;
}

Widget::ccWidgetClickCallback GameClearLayer::onLocateClickCallback(const string &callBackName)
{
    if (callBackName == "onClick")
    {
        return CC_CALLBACK_1(GameClearLayer::onClick, this);
    }
    return nullptr;
}


Widget::ccWidgetEventCallback GameClearLayer::onLocateEventCallback(const string &callBackName)
{
    if (callBackName == "onEvent")
    {
        return CC_CALLBACK_2(GameClearLayer::onEvent, this);
    } 
    return nullptr; 
}

void GameClearLayer::onTouch(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    CCLOG("onTouch");
}


void GameClearLayer::onClick(cocos2d::Ref* sender)
{
    CCLOG("onClick");
}


void GameClearLayer::onEvent(cocos2d::Ref* sender, int eventType)
{
    CCLOG("onEvent");
}

void GameClearLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto ntag = static_cast<Button *>(pSender)->getTag();
            switch (ntag) {
                case Tag_Quit:
                {
                    CMD_GR_UserStandUp request;
                    memset(&request, 0, sizeof(request));
                    
                    request.wTableID = HallDataMgr::getInstance()->m_wTableID;
                    request.wChairID = HallDataMgr::getInstance()->m_wChairID;
                    request.cbForceLeave = true;
                    
                    NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));
                    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                    this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]{
                        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
                    })));
                }
                    break;
                case Tag_Return:
                {
                    GameLayer* pLayer = (GameLayer*)this->getParent();
                    pLayer->setSecondCount(60);
                    this->removeFromParent();
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}

void GameClearLayer::exitCallBack(CCObject *pSender, ui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        CMD_GR_UserStandUp request;
        memset(&request, 0, sizeof(request));
        
        request.wTableID = HallDataMgr::getInstance()->m_wTableID;
        request.wChairID = HallDataMgr::getInstance()->m_wChairID;
        request.cbForceLeave = true;
        HallDataMgr::getInstance()->m_bStartGame = false;
        
        NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));
        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]{
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
        })));
    }
}

void GameClearLayer::backCallBack(CCObject *pSender, ui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        GameLayer* layer = (GameLayer*)getParent();
        layer->setSecondCount(60);
        this->removeFromParent();
    }
}