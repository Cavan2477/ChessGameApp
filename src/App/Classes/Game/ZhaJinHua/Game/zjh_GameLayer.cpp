//
//  GameLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#include "zjh_GameLayer.h"
#include "Project.h"
#include "GameDataMgr.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "zjh_DeskLayer.h"
#include "SimpleAudioEngine.h"
#include "HallDataMgr.h"
#include "UserData.h"
#include "GameDataMgr.h"
#include "HallDataMgr.h"
#include "CMD_GameServer.h"
#include "zjh_GameSettingLayer.h"
#include "zjh_CardSprite.h"
#include "zjh_GameLogic.h"
#include "zjh_RoomLayer.h"
#include "zjh_ChatLayer.h"
#include "CMD_GameServer.h"
#include "zjh_DistributeLayer.h"
#include "lkpy_help.h"
#include "ModeLayer.h"
#include "define.h"
#include "zjh_GameDefine.h"
USING_NS_CC;
using namespace ui;
USING_ZJH_NAMESPACE;
#define USERINFO_MOVE_TIME 0.45f
#define SEND_CARD_INTERVAL_TIME 0.12f
#define CARD_SCALE 0.6f         //其他玩家牌大小
#define VS_MOVE_TIME 0.8f
#define CHIP_MOVE_TIME 0.25f
//鱼出现银币效果的数量 1-5
static int silverNumArrary[5] = {2,2,3,4,4};
static int addTimesArrary[3] = {1,2,5};
//5-
static int goldNumArrary[20] = {1,1,1,2,2,3,3,4,5,6,8,16,16,16,18,18};

enum ButtonTag
{
    Tag_BG = 0,
    
    Tag_Option,
    
    Tag_Fish_Layer = 3,
    
    Tag_ChatLabel=8,
    
    Tag_UI_Layer = 20,
    Tag_Card = 1000,
    Tag_UserScore = 9,
    Tag_BT_UserInfo,
    Tag_BT_Start,
    Tag_BT_ChangeDesk,
    Tag_Shoot,
    Tag_Lock,
    Tag_BT_Menu,
    Tag_BT_openAddGold,//显示4个加注按钮
    Tag_BT_FollowGold,
    Tag_BT_DoNotAddGold,
    Tag_BT_Check,
    Tag_BT_Compare,
    Tag_BT_GiveUp,
    Tag_BT_AddGold,//加注按钮
    Tag_BT_Setting,
    Tag_BT_Help,
    Tag_Bt_Clearing,
    Tag_BT_Chat_OnOff,
    Tag_Bt_Send_Chat,
    Tag_Bt_Card_Type,
    Tag_Bt_leave,
    Tag_head,//个人头像
    Tag_proBar = 98765,//玩家倒计时进度条
};

enum layerTag
{
    TAG_Clear_Layer =10000,
    TAG_Help_Layer,
    TAG_Second_Tip
};

//"获得一个补给箱！击中可能获得大量奖励哟！赶快击杀！"

Scene* GameLayer::createScene()
{
    auto scene = Scene::createWithPhysics();
    
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto layer = GameLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

//GameLayer::GameLayer():
//m_bBannerColor(false)
//{
//    
//}

bool GameLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    HallDataMgr::getInstance()->m_bStartGame = true;
    m_slideLayout = NULL;
    m_chipVe.clear();
    //HallDataMgr::getInstance()->m_bIsChangeDesk = false;
    m_bOpenUserInfo = true;
    m_bIsGameBegin = false;
    m_bOpenScondCount = false;
    m_countBg = nullptr;
    m_bHasLookCard = true;
    m_bCardTypeMoveing = false;
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    m_wMyChiarId = HallDataMgr::getInstance()->m_wChairID;
 
    m_llTotalScore = 0;
    
    m_gameLogic = CGameLogic::create();
    addChild(m_gameLogic);
   
    GameDataMgr* gdm = GameDataMgr::getInstance();
//    gdm->initgameMessage();
//    GameDataMgr::getInstance()->m_gameMessage->setLayer(this);
    m_layout= static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/gameLayer.json"));
    this->addChild(m_layout,1);
    this->uiLayerinit();
    selfInfoInit();
    

    m_nSecondCount = 30;
    m_bIsCompare = false;
    NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameLayer::onGameScene, this);
    NetworkMgr::getInstance()->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(GameLayer::networkGRGame, this));
    return true;
    this->scheduleUpdate();
    
    m_nGoldBannerNum = 0;
    
    //this->schedule(schedule_selector(GameLayer::secondSchedule),1.0f);
    
    return true;
};

void GameLayer::secondSchedule(float dt)
{
    if(m_bOpenScondCount)
    {
        m_countBg->setVisible(true);
        m_nSecondCount--;
        TextAtlas* countNum = (ui::TextAtlas*)Helper::seekWidgetByName(m_countBg, "countNum");
        countNum->setString(__String::createWithFormat("%d",m_nSecondCount)->getCString());
        if(m_nSecondCount <= 0)
        {
            closeScondCount();
        }
    }
}

void GameLayer::openScondCountByChairId(BYTE wChairId)
{
    closeScondCount();
    Button* infoBtn = getInfoBtnByChairId(wChairId);
    m_countBg = (ui::ImageView*)Helper::seekWidgetByName(infoBtn, "countBg");
    TextAtlas* countNum = (ui::TextAtlas*)Helper::seekWidgetByName(m_countBg, "countNum");
    countNum->setString("30");
    m_nSecondCount = 30;
    m_countBg->setVisible(true);
    m_bOpenScondCount = true;
    schedule(schedule_selector(GameLayer::secondSchedule),1.0f);
}

void GameLayer::closeScondCount()
{
    if(!m_countBg)
        return;
    m_countBg->setVisible(false);
    TextAtlas* countNum = (ui::TextAtlas*)Helper::seekWidgetByName(m_countBg, "countNum");
    countNum->setString("30");
    m_nSecondCount = 30;
    m_bOpenScondCount = false;
    unschedule(schedule_selector(GameLayer::secondSchedule));
}

void GameLayer::presentButtomBg()
{
    m_buttomBg->setVisible(true);
}

void GameLayer::hideButtomBg()
{
    m_buttomBg->setVisible(false);
}

void GameLayer::presentButtomBtn()
{
    //presentButtomBg();
    //hideButtomAddBtn();
    Button *addGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "addGoldBtn");
    bool is = m_lTurnMaxScore>m_lTurnLessScore && m_lTurnMaxScore - m_lTurnLessScore != 0&&m_lTurnMaxScore-m_lTurnLessScore>m_cmd_GameStart.lCellScore;
    addGoldBtn->setVisible(true);
    Button *followGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "followGoldBtn");
    followGoldBtn->setVisible(true);
    
    Button *compareBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "compareBtn");
    compareBtn->setVisible(true);
    
    LONGLONG lScore = 0;
    
    for(int i = 0;i < GAME_PLAYER;++i)
    {
        lScore+=m_lTableScore[i];
    }
    
    if(lScore == m_nPlayerNum*m_cmd_GameStart.lCellScore)
    {
        followGoldBtn->setColor(Color3B::GRAY);
        followGoldBtn->setEnabled(false);
        
        addGoldBtn->setColor(Color3B::WHITE);
        addGoldBtn->setEnabled(true);
    }
    else
    {
        followGoldBtn->setColor(Color3B::WHITE);
        followGoldBtn->setEnabled(true);
    }
    
    LONGLONG lTemp=(m_bHasLookCard)?7:5;
    if((m_cmd_GameStart.lUserMaxScore-m_lTableScore[m_wMyChiarId])>=m_cmd_GameStart.lMaxScore*lTemp)
    {
        if(m_lCurrentTimes * m_cmd_GameStart.lCellScore < m_cmd_GameStart.lMaxScore)
        {
            addGoldBtn->setColor(Color3B::WHITE);
            addGoldBtn->setEnabled(true);
        }
        else
        {
            addGoldBtn->setColor(Color3B::GRAY);
            addGoldBtn->setEnabled(false);
            
        }
        
        
        if(m_wMyChiarId==m_wBankUser||m_lTableScore[m_wMyChiarId]>=2*m_cmd_GameStart.lCellScore)
        {
            compareBtn->setTouchEnabled(true);
            compareBtn->setColor(Color3B::WHITE);
        }
        else
        {
            compareBtn->setTouchEnabled(false);
            compareBtn->setColor(Color3B::GRAY);
        }
        
    }
    else
    {
        addGoldBtn->setColor(Color3B::GRAY);
        addGoldBtn->setEnabled(false);
        
        followGoldBtn->setColor(Color3B::GRAY);
        followGoldBtn->setEnabled(false);
        
        compareBtn->setTouchEnabled(false);
        compareBtn->setColor(Color3B::GRAY);
    }
    
    
    
    
    Button *checkBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "checkBtn");
    checkBtn->setVisible(true);
    if(m_bHasLookCard)
    {
        checkBtn->setTouchEnabled(false);
        checkBtn->setColor(Color3B::GRAY);
    }
    else
    {
        checkBtn->setTouchEnabled(true);
        checkBtn->setColor(Color3B::WHITE);
    }
    
    
    Button *giveUpBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "giveUpBtn");
    giveUpBtn->setVisible(true);
}

void GameLayer::hideButtomBtn()
{
    Button *addGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "addGoldBtn");
    addGoldBtn->setVisible(false);
    
    Button *followGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "followGoldBtn");
    followGoldBtn->setVisible(false);
    
    Button *checkBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "checkBtn");
    checkBtn->setVisible(false);
    
    Button *compareBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "compareBtn");
    compareBtn->setVisible(false);
    
    Button *giveUpBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "giveUpBtn");
    giveUpBtn->setVisible(false);
    
    
}

void GameLayer::presentButtomAddBtn()
{
    presentButtomBg();
    hideButtomBtn();
    Button *infoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
    Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
    LONGLONG lDrawUserScore = atoi(addNum->getString().c_str());
    LONGLONG lMinBet = ( m_lTurnLessScore==lDrawUserScore)?m_cmd_GameStart.lCellScore:0;
    SetBetCount( m_lTurnMaxScore - m_lTurnLessScore, lMinBet );
    m_lMinTurnScore = lMinBet;
    m_lMaxTurnScore = m_lTurnMaxScore - m_lTurnLessScore;
//    if(!m_slideLayout)
//    {
//        m_slideLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/slideGoldLayout.json"));
//        this->addChild(m_slideLayout,2);
//        
//        ImageView * slideBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_slideLayout, "slideBg");
//        
//        m_slideLine = (ui::Slider*)ui::Helper::seekWidgetByName(slideBg, "slideLine");
//        m_slideLine->addEventListener(CC_CALLBACK_2(GameLayer::slideEvent, this));
//        
//        
//        m_slideProBar = (ui::LoadingBar*)ui::Helper::seekWidgetByName(slideBg, "proBar");
//        
//        m_slideAddGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(slideBg, "addGoldBtn");
//        m_slideAddGoldBtn->setTag(Tag_BT_AddGold);
//        m_slideAddGoldBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
//        
//        m_slideGoldNum = (ui::TextAtlas*)ui::Helper::seekWidgetByName(m_slideAddGoldBtn, "addNum");
//        
//        
//    }
//    m_slideGoldNum->setString(__String::createWithFormat("%lld",lMinBet)->getCString());
//        m_slideLine->setPercent(0);
//        m_slideProBar->setPercent(0);
//        m_slideLayout->setVisible(true);
//
    LONGLONG lMaxTurnScore = m_cmd_GameStart.lMaxScore;
    LONGLONG lAddNum = m_cmd_GameStart.lCellScore;
    for(int i = 0;i < 3;++i)
    {
        Button *addBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_buttomBg,__String::createWithFormat("addBtn%d",i)->getCString());
        Text* addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBtn,"addNum");
        lAddNum = atoi(addNum->getString().c_str());
        if(lAddNum * m_lCurrentTimes >lMaxTurnScore)
        {
            addBtn->setEnabled(false);
            addBtn->setColor(Color3B::GRAY);
        }
        else
        {
            addBtn->setEnabled(true);
            addBtn->setColor(Color3B::WHITE);
        }
    }
}

void GameLayer::hideButtomAddBtn()
{
//    for(int i = 0;i < 3;++i)
//    {
//        Button *addBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_buttomBg,__String::createWithFormat("addBtn%d",i)->getCString());
//        addBtn->setVisible(false);
//    }
    hideButtomBg();
    Button *infoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
    Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
    LONGLONG lDrawUserScore = atoi(addNum->getString().c_str());
    LONGLONG lMinBet = ( m_lTurnLessScore==lDrawUserScore)?m_cmd_GameStart.lCellScore:0;
    m_lMinTurnScore = lMinBet;
    m_lMaxTurnScore = m_lTurnMaxScore - m_lTurnLessScore;
//    if(!m_slideLayout)
//    {
//        m_slideLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("studioUi/slideGoldLayout.json"));
//        
//        this->addChild(m_slideLayout,2);
//        
//        ImageView * slideBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_slideLayout, "slideBg");
//        
//        
//        m_slideLine = (ui::Slider*)ui::Helper::seekWidgetByName(slideBg, "slideLine");
//        m_slideLine->addEventListener(CC_CALLBACK_2(GameLayer::slideEvent, this));
//        
//        m_slideProBar = (ui::LoadingBar*)ui::Helper::seekWidgetByName(slideBg, "proBar");
//        
//        m_slideAddGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(slideBg, "addGoldBtn");
//        m_slideAddGoldBtn->setTag(Tag_BT_AddGold);
//        m_slideAddGoldBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
//        
//        m_slideGoldNum = (ui::TextAtlas*)ui::Helper::seekWidgetByName(m_slideAddGoldBtn, "addNum");
//       
//    }
//     //m_slideGoldNum->setString(__String::createWithFormat("%lld",lMinBet)->getCString());
//        m_slideLayout->setVisible(false);
//        m_slideLine->setPercent(0);
//        m_slideProBar->setPercent(0);
}

void GameLayer::presentStartBtns()
{
    m_startBtn->setVisible(true);
    //m_changeDeskBtn->setVisible(true);
}

void GameLayer::hideStartBtns()
{
    m_startBtn->setVisible(false);
    m_changeDeskBtn->setVisible(false);
}

void GameLayer::selfInfoInit()
{
    Button *selfInfoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
    UserData *puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    selfInfoBtn->setUserData(puser);
    Size infoSize = selfInfoBtn->getContentSize();
    HeaderRequest * headimage = NULL;
    if (HallDataMgr::getInstance()->m_loadtype == Load_Normal || HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
        headimage = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID, HallDataMgr::getInstance()->m_wCustom, HallDataMgr::getInstance()->m_dwUserID,HallDataMgr::getInstance()->m_cbGender);
    }
    else if (HallDataMgr::getInstance()->m_loadtype == Load_RenRen || HallDataMgr::getInstance()->m_loadtype == Load_Sina)
    {
        headimage = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
    }
    headimage->setHeadSize(90);
    
    headimage->setPosition(Vec2(infoSize.width/2+2,infoSize.height/2+19));
    
    selfInfoBtn->addChild(headimage);
    
    //昵称
    Text *nickName = (ui::Text*)ui::Helper::seekWidgetByName(selfInfoBtn, "nickName");
    string tempStr = lkpy_game::getStringByWidth(HallDataMgr::getInstance()->m_pNickName, 18,selfInfoBtn->getContentSize().width-38);
    nickName->setString(tempStr);
    
    Text *goldNum = (ui::Text*)ui::Helper::seekWidgetByName(selfInfoBtn, "goldNum");
    std::string scorestr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestr = lkpy_game::getGoldStr(scorestr,',');
    scorestr = lkpy_game::getStringByWidth(scorestr,18,selfInfoBtn->getContentSize().width-10);
    goldNum->setString(scorestr);
    
}

void GameLayer::uiLayerinit()
{
    //return;
    for (int i = 0;i < GAME_PLAYER ; ++i){
        ImageView* chatBg = (ImageView*)getWidgetByChairId(i, "chatBg%d");
        chatBg->setVisible(false);
    }
    
    
    m_topBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "topBg");
    
    
    m_userInfoBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "userInfoBg");
    m_userInfoBg->setVisible(false);
    m_userInfoBg->setScale(0.01f);
    
    m_buttomBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "buttomBg");
    
    m_cardType = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "cardType");
    
    Button *m_leaveBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_topBg, "leaveBtn");
    m_leaveBtn->setTag(Tag_Bt_leave);
    m_leaveBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    Button *m_setBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_topBg, "setBtn");
    m_setBtn->setTag(Tag_BT_Setting);
    m_setBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    Button *m_chatBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_topBg, "chatBtn");
    m_chatBtn->setTag(Tag_Bt_Send_Chat);
    m_chatBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    Button *typeBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "typeBtn");
    typeBtn->setTag(Tag_Bt_Card_Type);
    typeBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent,this));
    
    
    m_currentTotalNum = (ui::Text*)ui::Helper::seekWidgetByName(m_topBg, "totalBet");
    
    //bool m_bIsSecondOpenClear;
    m_startBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "startBtn");
    m_startBtn->setTag(Tag_BT_Start);
    m_startBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
   
    m_changeDeskBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "changeDeskBtn");
    m_changeDeskBtn->setVisible(false);
    m_changeDeskBtn->setTag(Tag_BT_ChangeDesk);
    m_changeDeskBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    
    
    Button *addGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "addGoldBtn");
    addGoldBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    addGoldBtn->setTag(Tag_BT_openAddGold);
    
    Button *followGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "followGoldBtn");
    followGoldBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    followGoldBtn->setTag(Tag_BT_FollowGold);
    
    Button *checkBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "checkBtn");
    checkBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    checkBtn->setTag(Tag_BT_Check);
    
    Button *compareBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "compareBtn");
    compareBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    compareBtn->setTag(Tag_BT_Compare);
    
    Button *giveUpBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "giveUpBtn");
    giveUpBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    giveUpBtn->setTag(Tag_BT_GiveUp);
    
    for(int i = 0;i < 3;++i)
    {
        Button *addBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_buttomBg,__String::createWithFormat("addBtn%d",i)->getCString());
        addBtn->setTag(Tag_BT_AddGold);
        addBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    }
    
    for(int i  = 0; i < GAME_PLAYER;++i)
    {
        Button * infoBtn = getInfoBtnByChairId(i);
        infoBtn->setTag(Tag_BT_UserInfo);
        infoBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::userInfoEvent, this));
        
        ImageView* chatBg = (ImageView*)getWidgetByChairId(i,"chatBg%d");
        Text *chatText = (ui::Text*)ui::Helper::seekWidgetByName(chatBg, "chatText");
        chatText->setVisible(false);
        Label *chatLabel = Label::create("", FontNormal, chatText->getFontSize());
        //chatLabel->setDimensions(230,0);
        chatLabel->setVisible(false);
        chatLabel->setPosition(chatText->getPosition());
        chatBg->addChild(chatLabel,2,Tag_ChatLabel);
        chatBg->setVisible(false);
    }
    

}

bool GameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if(!m_bCardTypeMoveing && !(m_cardType->boundingBox().containsPoint(touch->getLocation()))
       && m_cardType->getPositionX()>=(m_cardType->getContentSize().width/2 -1))
    {
        m_bCardTypeMoveing = true;
        Vec2 pos = m_cardType->getPosition();
        float moveX = -m_cardType->getContentSize().width;
        m_cardType->runAction(Sequence::createWithTwoActions(MoveBy::create(0.5f,Vec2(moveX,0)),CallFunc::create([=]{m_bCardTypeMoveing = false;})));
    }
    if(!m_userInfoBg->boundingBox().containsPoint(touch->getLocation()) && m_bOpenUserInfo)
    {
        int tag = m_userInfoBg->getTag();
        if(tag>=5 || !m_userInfoBg->isVisible())
        {
            int i = 0;
        }
        else{
            m_bOpenUserInfo = false;
            Button *infoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, __String::createWithFormat("infoBtn%d",tag)->getCString());
            m_userInfoBg->runAction(Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(USERINFO_MOVE_TIME-0.05f, 0.01f), MoveTo::create(USERINFO_MOVE_TIME, infoBtn->getPosition())), CallFunc::create([=]{
                m_userInfoBg->setVisible(false);
                m_bOpenUserInfo = true;
            })));
            return true;
        }
        
    }
    else if(m_userInfoBg->boundingBox().containsPoint(touch->getLocation()))
    {
        return true;
    }
//
//    if(m_pScureCard)
//    {
//        Rect cardRect = m_pScureCard->boundingBox();
//        Point cardPos = m_pScureCard->getPosition();
//        cardRect.setRect(0,0, cardRect.size.width / 4, cardRect.size.height);
//        Point pos = m_pScureCard->convertToNodeSpace(touch->getLocation()) ;
//        if(cardRect.containsPoint(pos))
//        {
//            m_pScureCard->changeDisplay(m_pScureCard->m_nValue);
//            return true;
//        }
//    }
        return false;
    
}

void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //if(m_pScureCard)
    //m_pScureCard->changeDisplay(0x50);
}

void GameLayer::userInfoEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto pbutton = (Button *)pSender;
        UserData * pUserData = (UserData*)pbutton->getUserData();
        ImageView* arrowImg = (ImageView*)Helper::seekWidgetByName(pbutton, "arrowImg");
        if(arrowImg->isVisible())
        {
            //隐藏所有箭头
            for(int i = 0;i < GAME_PLAYER;++i)
            {
                if(m_giveUpPlayer[i])
                {
                    Button* infoBtn = getInfoBtnByChairId(i);
                    ImageView* pArrowImg = (ImageView*)Helper::seekWidgetByName(pbutton, "arrowImg");
                    pArrowImg->setVisible(false);
                }
                    
                
            }
            CMD_C_CompareCard cmd;
            cmd.wCompareUser = pUserData->m_date.wChairID;
            NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_COMPARE_CARD, &cmd, sizeof(cmd));
            hideButtomBtn();
        }
        else
        {
            if(!m_bOpenUserInfo)
                return;
            UserData * pUserData = (UserData*)pbutton->getUserData();
            //只显示自己信息，屏蔽其余玩家 2016-5-5 by zhong
            if (pUserData->m_date.dwUserID != HallDataMgr::getInstance()->m_dwUserID)
            {
                return;
            }
            setUserInfo(pUserData);
            
            string strName = pbutton->getName();
            char cNum = strName.back();
            int tag = atoi(&cNum);
            m_userInfoBg->setPosition(pbutton->getPosition());
            m_userInfoBg->setScale(0.01f);
            m_userInfoBg->setTag(tag);
            m_userInfoBg->setVisible(true);
            m_bOpenUserInfo = false;
            m_userInfoBg->runAction(Sequence::createWithTwoActions(Spawn::create(MoveTo::create(USERINFO_MOVE_TIME,Point(568,320))
                                                                                 ,ScaleTo::create(USERINFO_MOVE_TIME-0.05f, 1.f)
                                                                                 ,NULL),CallFunc::create([=]{
                m_bOpenUserInfo = true;
            })));
            
        }
        
    }
}

void GameLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_Bt_leave:
                {
                    
                   if(m_bIsGameBegin)
                   {
                       auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("系统提示", "您正在游戏中,强行退出会被扣分,确定要强退吗?", Type_Ensure_Cancel));
                       player->setEnsureCallback([=]{
                           CMD_GR_UserStandUp request;
                           memset(&request, 0, sizeof(request));
                           
                           request.wTableID = HallDataMgr::getInstance()->m_wTableID;
                           request.wChairID = HallDataMgr::getInstance()->m_wChairID;
                           request.cbForceLeave = true;
                           
                           NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));
                           
                           this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]{
                               Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
                           })));
                           
                       });
                       player->setCancelCallback([=]
                                                 {
                                                     player->removeFromParent();
                                                 });
                   }
                   else
                   {
                       CMD_GR_UserStandUp request;
                       memset(&request, 0, sizeof(request));
                       
                       request.wTableID = HallDataMgr::getInstance()->m_wTableID;
                       request.wChairID = HallDataMgr::getInstance()->m_wChairID;
                       request.cbForceLeave = true;
                       
                       NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));
                       
                       this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]{
                           Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
                       })));
                   }
                   
                }
                    break;
                case Tag_BT_Menu:
                {
                    
                    if (m_btBg->isVisible()) {
                        m_btBg->setVisible(false);
                    }
                    else
                    {
                        m_btBg->setVisible(true);
                    }
                }
                    break;
                
                case Tag_Bt_Send_Chat:
                {
                    ChatLayer *cl = ChatLayer::create();
                    this->addChild(cl,100);
                    cl->initRecordUi();
                }
                    break;
                case Tag_Bt_Card_Type:
                {
                    if(m_bCardTypeMoveing)
                        break;
                    m_bCardTypeMoveing = true;
                    Vec2 pos = m_cardType->getPosition();
                    float moveX = -m_cardType->getContentSize().width;
                    if(pos.x < m_cardType->getContentSize().width/2)
                        moveX = -moveX;
                    m_cardType->runAction(Sequence::createWithTwoActions(MoveBy::create(0.5f,Vec2(moveX,0)),CallFunc::create([=]{m_bCardTypeMoveing = false;})));

                }
                    break;
                    
                case Tag_BT_Setting:
                {
                    auto player = SettingLayer::create();//GameSettingLayer::create();
                    this->addChild(player,Tag_UI_Layer+1);
                }
                    break;
                case Tag_BT_Start:
                {
                    NetworkMgr::getInstance()->sendData(MDM_GF_FRAME, SUB_GF_USER_READY, NULL, 0);
                }
                    break;
                    
                                case  Tag_BT_openAddGold:
                {
                    presentButtomAddBtn();
                    hideButtomBtn();
                }
                    break;
                    
                case Tag_BT_AddGold:
                {
                    hideButtomBg();
                    hideButtomBtn();
                    //hideButtomAddBtn();
                    Button *selfInfoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
                    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(selfInfoBtn, "addBg");
                    Text *tAddNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
                    string str1 = tAddNum->getString();
                    LONGLONG currentScore = atoi(str1.c_str());
                    //hideButtomAddBtn();
                    Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(pbutton, "addNum");
                    string str2 = addNum->getString();
                    LONGLONG addScore = atoi(str2.c_str());// + m_llLastScore;
                    addScore += m_cmd_GameStart.lCellScore * m_lCurrentTimes +m_llLastScore;
                    if(m_bHasLookCard)
                        addScore*=2;
                    CMD_C_AddScore cmd_cas;
                    cmd_cas.lScore = addScore;
                    cmd_cas.wState = 0;
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_ADD_SCORE, &cmd_cas, sizeof(CMD_C_AddScore));
                }
                    break;
                case  Tag_BT_FollowGold:
                {
                    //hideButtomBg();
                    hideButtomBtn();
                    CMD_C_AddScore cmd_cas;
                    cmd_cas.lScore = m_llStartScore;
                    cmd_cas.lScore = m_llLastScore;
                    LONGLONG addScore = m_cmd_GameStart.lCellScore * m_lCurrentTimes;
                    if(m_bHasLookCard)
                        addScore*=2;
                    cmd_cas.lScore = addScore;
                    cmd_cas.wState = 0;
                    log("Follow Score %lld,CurrentTimes %lld lCellScore %lld",addScore,m_lCurrentTimes,m_cmd_GameStart.lCellScore);
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_ADD_SCORE, &cmd_cas, sizeof(CMD_C_AddScore));
                }
                    
                    break;
                case  Tag_BT_DoNotAddGold:
                {
                    //hideButtomBg();
                    hideButtomBtn();
                    CMD_C_AddScore cmd_cas;
                    cmd_cas.lScore = 0;
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_ADD_SCORE, &cmd_cas, sizeof(CMD_C_AddScore));
                }
                    break;
                case  Tag_BT_Check:
                {
                    //hideButtomBg();
                    
                    //playVsAnimation();
                    
                    //cmd_cas.lScore = m_cmd_GameStart.lDrawMaxScore - lAddNum;
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_LOOK_CARD, NULL, 0);
                    pbutton->setTouchEnabled(false);
                    pbutton->setColor(Color3B::GRAY);
                    m_bHasLookCard = true;
                }
                    break;
                    
                case  Tag_BT_Compare:
                {
                    m_bIsCompare = true;
                    hideButtomBtn();
                    //判断明注
                    LONGLONG lCurrentScore=m_bHasLookCard?(m_lCurrentTimes*m_cmd_GameStart.lCellScore*4):(m_lCurrentTimes*m_cmd_GameStart.lCellScore*2);
                    CMD_C_AddScore cmd_cas;
                    cmd_cas.lScore = lCurrentScore;
                    cmd_cas.wState = 1;
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_ADD_SCORE, &cmd_cas, sizeof(CMD_C_AddScore));
                    // 发送加注消息
                    if((m_wBankUser==m_wMyChiarId&&(m_lTableScore[m_wMyChiarId]-lCurrentScore)==m_cmd_GameStart.lCellScore)||m_nPlayerNum==2)
                    {
                        //构造变量
                        CMD_C_CompareCard cmd;
                        
                        //查找上家
                        for(int i=m_wMyChiarId-1;;i--)
                        {
                            if(i==-1)
                                i=GAME_PLAYER-1;
                            if(m_giveUpPlayer[i])
                            {
                                //playVsAnimation();
                                cmd.wCompareUser=(WORD)i;
                                NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_COMPARE_CARD, &cmd, sizeof(cmd));
                                break;
                            }
                        }
                        
                    }
                    else//选择比牌玩家
                    {
                        
                        for(int i = 0;i < GAME_PLAYER;++i)
                        {
                            if(m_giveUpPlayer[i] && i != HallDataMgr::getInstance()->m_wChairID)
                            {
                                Button* infoBtn = getInfoBtnByChairId(i);
                                ImageView* arrowImg = (ImageView*)Helper::seekWidgetByName(infoBtn, "arrowImg");
                                arrowImg->setVisible(true);
                            }
                        }
                    }
                    
                }
                    break;
                case  Tag_BT_GiveUp:
                {
                    //hideButtomBg();
                    hideButtomBtn();
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_GIVE_UP, NULL, NULL);
                }
                    break;
                case Tag_BT_UserInfo:
                {
                    UserData * pUserData = (UserData*)pbutton->getUserData();
                    ImageView* arrowImg = (ImageView*)Helper::seekWidgetByName(pbutton, "arrowImg");
                    if(arrowImg->isVisible())
                    {
                        CMD_C_CompareCard cmd;
                        cmd.wCompareUser = pUserData->m_date.wChairID;
                        NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_COMPARE_CARD, &cmd, sizeof(cmd));
                    }
                    else
                    {
                        if(!m_bOpenUserInfo)
                            return;
                        UserData * pUserData = (UserData*)pbutton->getUserData();
                        setUserInfo(pUserData);
                        
                        string strName = pbutton->getName();
                        char cNum = strName.back();
                        int tag = atoi(&cNum);
                        m_userInfoBg->setPosition(pbutton->getPosition());
                        m_userInfoBg->setScale(0.01f);
                        m_userInfoBg->setTag(tag);
                        m_userInfoBg->setVisible(true);
                        m_bOpenUserInfo = false;
                        m_userInfoBg->runAction(Sequence::createWithTwoActions(Spawn::create(MoveTo::create(USERINFO_MOVE_TIME,Point(568,320))
                                                                                             ,ScaleTo::create(USERINFO_MOVE_TIME-0.05f, 1.f)
                                                                                             ,NULL),CallFunc::create([=]{
                            m_bOpenUserInfo = true;
                        })));
                        
                    }
                    
                }
                    break;
                default:
                    break;
            }
        }
}


void GameLayer::slideEvent(cocos2d::Ref *pSender, Slider::EventType type)
{
    if(type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider * slider = dynamic_cast<Slider *>(pSender);
        int percent = slider->getPercent();
        
        m_slideProBar->setPercent(percent);
        Button *selfInfoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
        ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(selfInfoBtn, "addBg");
        Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
        string str = addNum->getString();
        LONGLONG currentScore = atoi(str.c_str());
        LONGLONG minScore = m_lTurnLessScore - currentScore;
        LONGLONG maxScore = m_lTurnMaxScore - currentScore;
        LONGLONG goldNum = minScore + (maxScore - minScore) / 100 * percent;
        m_slideGoldNum->setString(__String::createWithFormat("%lld",goldNum)->getCString());
    }
}

void GameLayer::playVsAnimation()
{
    auto pVs = Sprite::createWithSpriteFrameName("vs_01.png");
    auto pAnimation = AnimationCache::getInstance()->getAnimation(VS_ANIM);
    if(pAnimation)
    {
        auto animate = Animate::create(pAnimation);
        pVs->setPosition(568,340);
        m_layout->addChild(pVs,10);
        pVs->runAction(Sequence::create(DelayTime::create(VS_MOVE_TIME),animate, CallFunc::create([=]{
            pVs->removeFromParentAndCleanup(true);
        }),NULL));
    }
    
}

void GameLayer::setUserInfo(UserData* pUserData)
{
    
    HeaderRequest *pHead = HeaderRequest::createwithFaceID(pUserData->m_date.wFaceID, pUserData->m_date.dwCustomID, pUserData->m_date.dwUserID,pUserData->m_date.cbGender);;
    m_userInfoBg->removeChildByTag(Tag_head);
    m_userInfoBg->addChild(pHead,2,Tag_head);
    pHead->setPosition(185,248);
    pHead->setHeadSize(210);
    
    ImageView* genderImg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_userInfoBg, "genderImg");
    genderImg->loadTexture(__String::createWithFormat("player_gender_%d.png",pUserData->m_date.cbGender)->getCString());
    
    Text* idNum = (ui::Text*)ui::Helper::seekWidgetByName(m_userInfoBg, "idNum");
    idNum->setString(__String::createWithFormat("%d",pUserData->m_date.dwUserID)->getCString());
    
    Text* userName = (ui::Text*)ui::Helper::seekWidgetByName(m_userInfoBg, "userName");
    string strNickName = pUserData->m_nickname;
    strNickName = lkpy_game::getStringByWidth(strNickName,userName->getFontSize(),270);
    userName->setString(strNickName);
    
    Text* goldNum = (ui::Text*)ui::Helper::seekWidgetByName(m_userInfoBg, "goldNum");
    string strGoldNum = __String::createWithFormat("%lld",pUserData->m_date.lScore)->getCString();
    strGoldNum = lkpy_game::getGoldStr(strGoldNum, ',');
    goldNum->setString(strGoldNum);
    
    Text* winContent = (ui::Text*)ui::Helper::seekWidgetByName(m_userInfoBg, "winContent");
    float fPercent = float(pUserData->m_date.dwWinCount) / float(pUserData->m_date.dwWinCount + pUserData->m_date.dwLostCount + pUserData->m_date.dwDrawCount);
    int nPercent = int(fPercent * 100);
    winContent->setString(__String::createWithFormat("%d%% %d胜 %d负 %d平",nPercent,pUserData->m_date.dwWinCount,pUserData->m_date.dwLostCount,pUserData->m_date.dwDrawCount)->getCString());
    
}

void GameLayer::OnUserStatus(void *pUser)
{
    auto puserData = (UserData *)pUser;
    if(puserData->m_date.wTableID == INVALID_TABLE || puserData->m_date.wChairID == INVALID_CHAIR)
    {
        Button *infoBtn = (Button*)m_layout->getChildByTag(puserData->m_date.dwGameID);
        if(infoBtn)//用户离开
        {
            infoBtn->setVisible(false);
            infoBtn->removeChildByTag(Tag_head);
            infoBtn->setTag(0);
            
            ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
            addBg->setVisible(false);
        }
        return;
    }
    if(puserData->m_date.wTableID != HallDataMgr::getInstance()->m_wTableID)
        return;
    WORD myChairId = HallDataMgr::getInstance()->m_wChairID;
    
    Button *infoBtn = getInfoBtnByChairId(puserData->m_date.wChairID);
    auto readyImg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "readyImg");
    if (puserData->m_date.cbUserStatus == US_READY)
    {
        readyImg->setVisible(true);
        if(myChairId == puserData->m_date.wChairID)
        {
            closeScondCount();
            m_startBtn->setVisible(false);
            m_changeDeskBtn->setVisible(false);
        }
    }
    else
    {
        readyImg->setVisible(false);
    }
    if(infoBtn->isVisible())
        return;
    Size infoSize = infoBtn->getContentSize();
    if(puserData->m_date.cbUserStatus >= US_SIT)
    {
        auto bankImg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "bankImg");
        bankImg->setVisible(false);
        infoBtn->setUserData(pUser);
        infoBtn->setVisible(true);
        if(infoBtn->getTag() != puserData->m_date.dwGameID)
        {
            HeaderRequest * headimage = NULL;
            
            headimage = HeaderRequest::createwithFaceID(puserData->m_date.wFaceID, puserData->m_date.dwCustomID, puserData->m_date.dwUserID,puserData->m_date.cbGender);
            
            headimage->setHeadSize(90);
            
            headimage->setPosition(Vec2(infoSize.width/2+2,infoSize.height/2+19));
            
            infoBtn->addChild(headimage,1,Tag_head);
            
            //昵称
            Text *nickName = (ui::Text*)ui::Helper::seekWidgetByName(infoBtn, "nickName");
            string tempStr = lkpy_game::getStringByWidth(puserData->m_nickname, 18,infoBtn->getContentSize().width-38);
            nickName->setString(tempStr);
            
            Text *goldNum = (ui::Text*)ui::Helper::seekWidgetByName(infoBtn, "goldNum");
            std::string scorestr = __String::createWithFormat("%lld",puserData->m_date.lScore)->getCString();
            scorestr = lkpy_game::getGoldStr(scorestr,',');
            scorestr = lkpy_game::getStringByWidth(scorestr,18,infoBtn->getContentSize().width-10);
            goldNum->setString(scorestr);
            
            infoBtn->setTag(puserData->m_date.dwGameID);
        }
        
        
        
    }
    
}



void GameLayer::OnUserEnter(void *pUser)
{
    auto puserData = (UserData *)pUser;
//    if(puserData->m_date.wTableID == INVALID_TABLE || puserData->m_date.wChairID == INVALID_CHAIR)
//    {
//      
//        return;
//    }
    if(puserData->m_date.wTableID == INVALID_TABLE || puserData->m_date.wChairID == INVALID_CHAIR)
    {
        Button *infoBtn = (Button*)m_layout->getChildByTag(puserData->m_date.dwGameID);
        if(infoBtn)//用户离开
        {
            infoBtn->setVisible(false);
            infoBtn->removeChildByTag(Tag_head);
            infoBtn->setTag(0);
            
            ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
            addBg->setVisible(false);
        }
        return;
    }
    if(puserData->m_date.wTableID != HallDataMgr::getInstance()->m_wTableID)
        return;
    WORD myChairId = HallDataMgr::getInstance()->m_wChairID;
    
    Button *infoBtn = getInfoBtnByChairId(puserData->m_date.wChairID);
    auto readyImg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "readyImg");
    if (puserData->m_date.cbUserStatus == US_READY)
    {
        readyImg->setVisible(true);
        if(myChairId == puserData->m_date.wChairID)
        {
            openScondCountByChairId(myChairId);
            m_startBtn->setVisible(false);
            m_changeDeskBtn->setVisible(false);
        }
    }
    else
    {
        readyImg->setVisible(false);
    }
    if(infoBtn->isVisible())
        return;
    Size infoSize = infoBtn->getContentSize();
    if(puserData->m_date.cbUserStatus >= US_SIT)
    {
        auto bankImg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "bankImg");
        bankImg->setVisible(false);
        infoBtn->setUserData(pUser);
        infoBtn->setVisible(true);
        if(infoBtn->getTag() != puserData->m_date.dwGameID)
        {
            HeaderRequest * headimage = NULL;
            
            headimage = HeaderRequest::createwithFaceID(puserData->m_date.wFaceID, puserData->m_date.dwCustomID, puserData->m_date.dwUserID,puserData->m_date.cbGender);
            
            headimage->setHeadSize(90);
            
            headimage->setPosition(Vec2(infoSize.width/2+2,infoSize.height/2+19));
            
            infoBtn->addChild(headimage,1,Tag_head);
            
            //昵称
            Text *nickName = (ui::Text*)ui::Helper::seekWidgetByName(infoBtn, "nickName");
            string tempStr = lkpy_game::getStringByWidth(puserData->m_nickname, 18,infoBtn->getContentSize().width-25);
            nickName->setString(tempStr);
            
            Text *goldNum = (ui::Text*)ui::Helper::seekWidgetByName(infoBtn, "goldNum");
            std::string scorestr = __String::createWithFormat("%lld",puserData->m_date.lScore)->getCString();
            scorestr = lkpy_game::getGoldStr(scorestr,',');
            scorestr = lkpy_game::getStringByWidth(scorestr,18,infoBtn->getContentSize().width-10);
            goldNum->setString(scorestr);
            
            infoBtn->setTag(puserData->m_date.dwGameID);
        }
        
        
        
    }

}

void GameLayer::OnUserScore(void *pData)
{
    return;
    auto result = (CMD_GR_MobileUserScore *)pData;
    auto pUser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);
    if(pUser->m_date.wTableID != HallDataMgr::getInstance()->m_wTableID)
        return;
    Button* infoBtn = (Button*)getInfoBtnByChairId(pUser->m_date.wChairID);
    Text *goldNum = (Text *)ui::Helper::seekWidgetByName(infoBtn, "goldNum");
    goldNum->setText(__String::createWithFormat("%lld", pUser->m_date.lScore)->getCString());
    //string gol
    
}

void GameLayer::onEnterTransitionDidFinish()
{
    NetworkMgr::getInstance()->m_Userdelegate = this;
    for(int i = 0;i<GAME_PLAYER;++i)
    {
        if(i != HallDataMgr::getInstance()->m_wChairID)
        NetworkMgr::getInstance()->sendRequestChairUserInfo(HallDataMgr::getInstance()->m_wTableID,i);
    }
    //sendRequestChairUserInfo
    Layer::onEnterTransitionDidFinish();
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Room_LeaveGame, CC_CALLBACK_1(GameLayer::eventLeaveGame, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(GameLayer::eventScoreChange, this)), 1);
    GameDataMgr* gdm = GameDataMgr::getInstance();

    //GameDataMgr::getInstance()->m_gameMessage->setLayer(this);
    //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(__String::createWithFormat("sound_res/MUSIC_BACK_0%d.mp3", GameDataMgr::getInstance()->m_sceneData.cbBackIndex+1)->getCString(), true);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);;
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -128);
    _touchListener = listener;
}

void GameLayer::onExit()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/LOAD_BACK.wav",true);
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_GF_GAME);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Room_LeaveGame);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    //GameDataMgr::getInstance()->clearGameData();
    HallDataMgr::getInstance()->m_bStartGame = false;
    //AnimationCache::getInstance()->destroyInstance();
    //SpriteFrameCache::getInstance()->removeSpriteFrames();
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
//    if(GameDataMgr::getInstance()->m_gameMessage)
//        GameDataMgr::getInstance()->m_gameMessage->setLayer(nullptr);
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;

    Layer::onExit();
}

void GameLayer::update(float delta)
{
    
}
void GameLayer::SetBetCount(LONGLONG lMaxBetCount, LONGLONG lMinBetCount)
{
    return;
    m_lMaxScore = lMaxBetCount;
    m_lMinScore = lMinBetCount;
    
    if ( m_lMaxScore < 0 )
        m_lMaxScore = 0;
    
    if ( m_lMinScore < 0 )
        m_lMinScore = 0;
    
    if ( m_cmd_GameStart.lCellScore > m_lMaxScore )
        m_lMinScore = m_lMaxScore;
    Button *selfInfoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(selfInfoBtn, "addBg");
    
    for(LONGLONG i = 2;i>=0;--i)
    {
        Button *addBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_buttomBg,__String::createWithFormat("addBtn%d",i)->getCString());
        TextAtlas *addNum = (ui::TextAtlas*)ui::Helper::seekWidgetByName(addBtn,"addNum");
        addBtn->setVisible(true);
        if(m_cmd_GameStart.lCellScore * (4-i) > lMaxBetCount )
        {
            addBtn->setColor(Color3B::GRAY);
            addBtn->setEnabled(false);
        }
        else
        {
            //addNum->setString(__String::createWithFormat("%lld",m_cmd_GameStart.lCellScore*(i+1))->getCString());
            addBtn->setColor(Color3B::WHITE);
            addBtn->setEnabled(true);
        }
        
    }
    
}
void GameLayer::eventLeaveGame(cocos2d::EventCustom *event)
{
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
//    if(GameDataMgr::getInstance()->m_gameMessage)
//        GameDataMgr::getInstance()->m_gameMessage->setLayer(nullptr);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/LOAD_BACK.wav",true);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Room_LeaveGame);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    //GameDataMgr::getInstance()->clearGameData();
    HallDataMgr::getInstance()->m_bStartGame = false;
    //AnimationCache::getInstance()->destroyInstance();
    //SpriteFrameCache::getInstance()->removeSpriteFrames();
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    //if(!m_bIsChangeDesk)
    if((HallDataMgr::getInstance()->m_RoomRule & SR_ALLOW_AVERT_CHEAT_MODE) != 0)
        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, DistributeLayer::createScene()));
    else
        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, DeskLayer::createScene()));
    //else
    {
       // Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, GameLayer::createScene()));
    }
    //Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, RoomLayer::createScene()));
}

void GameLayer::onGameScenePlay(CMD_S_StatusPlay* cmd_ssp)
{
    m_nChipCount = 0;
    m_bIsCompare = false;
    //显示庄家
    Button* zInfoBtn = getInfoBtnByChairId(cmd_ssp->wBankerUser);
    auto bankImg = (ui::Button*)Helper::seekWidgetByName(zInfoBtn, "bankImg");
    bankImg->setVisible(true);
    m_lUserMaxScore = cmd_ssp->lUserMaxScore;
    m_lCurrentTimes = cmd_ssp->lCurrentTimes;
    m_cmd_GameStart.lMaxScore = cmd_ssp->lMaxCellScore;
    m_cmd_GameStart.lCellScore = cmd_ssp->lCellScore;
    m_cmd_GameStart.lUserMaxScore = cmd_ssp->lUserMaxScore;;
    updateOperatorText(cmd_ssp->wCurrentUser);
    m_wMyChiarId = HallDataMgr::getInstance()->m_wChairID;
    if(cmd_ssp->lTableScore[m_wMyChiarId]>0)
        m_bIsGameBegin = true;
    
    
    memset(m_lTotalScore,0,GAME_PLAYER*sizeof(LONGLONG));

    

    //m_llLastScore = cmd_ssp->lTurnLessScore;
    memset(m_giveUpPlayer, 0,GAME_PLAYER);
    hideStartBtns();
    for(int index = 0; index < m_chipVe.size();++index)
    {
        auto pCard =(Sprite*)m_chipVe.at(index);
        pCard->removeFromParentAndCleanup(true);
    }
    m_chipVe.clear();

    m_llStartScore = 0;

    for (int i = 0; i < GAME_PLAYER; i++) {
        
        
    }
    m_topBg->setVisible(true);
    Button *selfInfoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(selfInfoBtn, "addBg");
    Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
    if(cmd_ssp->bMingZhu)
    {
        ImageView* eye = (ui::ImageView*)ui::Helper::seekWidgetByName(selfInfoBtn, "eye");
        eye->setVisible(true);
    }
    
    
    ImageView *totalNumBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "totalNumBg");
    //totalNumBg->setVisible(true);
    

    m_llTotalScore = 0;
    m_nPlayerNum = 0;
    for(int i = 0;i < GAME_PLAYER;++i)
    {
        m_llTotalScore += cmd_ssp->lTableScore[i];
        m_lTableScore[i] = cmd_ssp->lTableScore[i];
        Layout *cardLayout = getCardLayoutByChairId(i);
        cardLayout->removeAllChildren();
        if(m_lTableScore[i])
            m_nPlayerNum++;
    }
    m_currentTotalNum->setString(__String::createWithFormat("%lld",m_llTotalScore)->getCString());
    
    
    m_cmd_GameStart.lCurrentTimes = cmd_ssp->lCurrentTimes;
    LONGLONG baseScore = cmd_ssp->lCellScore;
    LONGLONG maxUserScore = 0 ;//找出最大用户下注

    for(int i = 0;i < 3;++i)
    {
        Button *addBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_buttomBg,__String::createWithFormat("addBtn%d",i)->getCString());
        Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBtn,"addNum");
        addNum->setString(__String::createWithFormat("%lld",baseScore*addTimesArrary[i])->getCString());
    }
    
    
    float delayTime = 0;
    int nCount = 0;
    int totalCount = 0;
    for(int i  = 0; i < GAME_PLAYER;++i)
    {
        m_giveUpPlayer[i] = cmd_ssp->cbPlayStatus[i];
        m_cmd_GameStart.cbPlayStatus[i] = cmd_ssp->cbPlayStatus[i];
         if(m_cmd_GameStart.cbPlayStatus[i])
            totalCount++;
        
    }
    //算出当前正在玩游戏玩家的押注总和
    for(int i  = 0; i < GAME_PLAYER;++i)
    {
        //赋值当前还在游戏中的玩家，结算时明牌
        m_cmd_GameStart.cbPlayStatus[i] = cmd_ssp->cbPlayStatus[i];
        if(cmd_ssp->lTableScore[i])
        {
            Button * infoBtn = getInfoBtnByChairId(i);
            //显示庄家
            ImageView* bankImg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "bankImg");
            if(i == cmd_ssp->wBankerUser)
                bankImg->setVisible(true);
            else
                bankImg->setVisible(false);
            
            ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
            addBg->setVisible(true);
            Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
            addNum->setString(__String::createWithFormat("%lld",cmd_ssp->lTableScore[i])->getCString());
            if(cmd_ssp->bMingZhu[i])
            {
                ImageView *eye = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "eye");
                eye->setVisible(true);
            }
            CardSprite *pCard = NULL;
            float fScale;
            if(i == HallDataMgr::getInstance()->m_wChairID)
            {
                m_bHasLookCard = cmd_ssp->bMingZhu[i];
                fScale = 1;
            }
            else
            {
                fScale = CARD_SCALE;
            }
            Card_Value cv;
            if(m_cmd_GameStart.cbPlayStatus[i])
                cv = Value_Back;
            else
                cv = Value_Break;
            for(int index = 0;index < 3;++index)
            {
                addCard(i,cv,index,nCount * SEND_CARD_INTERVAL_TIME*0.8 + SEND_CARD_INTERVAL_TIME*0.8 * totalCount * index,false,false,true,fScale);
            }
            
            nCount++;
        }
    }
    
    if(cmd_ssp->wCurrentUser == HallDataMgr::getInstance()->m_wChairID)
    {
        presentButtomBtn();
    }
    if(cmd_ssp->wCurrentUser != INVALID_CHAIR)
    {
        openScondCountByChairId(cmd_ssp->wCurrentUser);
    }
}


void GameLayer::onGameSceneFree(CMD_S_StatusFree* cmd_ssf)
{
//    ImageView *totalNumBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "totalNumBg");
//    totalNumBg->setVisible(false);
//    

    m_startBtn->setVisible(true);
    m_currentTotalNum->setString("0");
    
    openScondCountByChairId(HallDataMgr::getInstance()->m_wChairID);
}

void GameLayer::onGameSceneStart(CMD_S_GameStart* cmd_sgs)
{
    m_nChipCount = 0;
    m_bIsCompare = false;
    //playVsAnimation();
    m_bHasLookCard = false;
    
    openScondCountByChairId(cmd_sgs->wCurrentUser);
    m_lCurrentTimes = cmd_sgs->lCurrentTimes;
    m_bIsGameBegin = true;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_START.wav");
    updateOperatorText(cmd_sgs->wCurrentUser);
    
    m_wMyChiarId = HallDataMgr::getInstance()->m_wChairID;
    //状态置零
    memset(m_lTotalScore,0, sizeof(m_lUserScore));
    memset(m_lUserScore,0, sizeof(m_lUserScore));
    memset(m_lTableScore,0, sizeof(m_lTableScore));
    //状态初始化
    for(int index = 0; index < m_chipVe.size();++index)
    {
        auto pCard =(Sprite*)m_chipVe.at(index);
        pCard->removeFromParentAndCleanup(true);
    }
    m_chipVe.clear();
    m_llStartScore = 0;
    m_llLastScore = 0;
    for (int i = 0; i < GAME_PLAYER; i++) {
        Layout *cardLayout = getCardLayoutByChairId(i);
        cardLayout->removeAllChildren();
        
    }
    memset(m_handCardArray, 0, 3 * GAME_PLAYER);
    memset(m_giveUpPlayer, 0,GAME_PLAYER);
    
    
 
    
    m_startBtn->setVisible(false);
    m_changeDeskBtn->setVisible(false);
    m_cmd_GameStart = *cmd_sgs;
    Button *selfInfoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "infoBtn0");
    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(selfInfoBtn, "addBg");
    Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
    addNum->setVisible(true);
    addNum->setString("0");
    LONGLONG baseScore = cmd_sgs->lCellScore;
    m_llTotalScore = 0;
    m_nPlayerNum = 0;
    //算出当前正在玩游戏玩家的押注总和
    int nCount = 0 ;//计算发牌时间
    int totalCount = 0;
    for(int i  = 0; i < GAME_PLAYER;++i)
    {
        if(cmd_sgs->cbPlayStatus[i])
        {
            totalCount++;
        }
        
    }
    float fScale = CARD_SCALE;
    int chipIndex = rand()%3+1;
    for(int i  = 0; i < GAME_PLAYER;++i)
    {
        if(cmd_sgs->cbPlayStatus[i])
        {
            m_nPlayerNum++;
            m_lTotalScore[i] = cmd_sgs->lCellScore;
            //参与游戏的玩家
            m_giveUpPlayer[i] = 1;
            
            m_lTableScore[i] = cmd_sgs->lCellScore;
            
            Button * infoBtn = getInfoBtnByChairId(i);
            ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
            addChip(infoBtn->convertToWorldSpace(addBg->getPosition()),chipIndex);
            addBg->setVisible(true);
            Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
            addNum->setString(__String::createWithFormat("%lld",baseScore)->getCString());
            m_llTotalScore += baseScore;
            
            ImageView* bankImg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "bankImg");
            if(i == cmd_sgs->wBankerUser)
                bankImg->setVisible(true);
            else
                bankImg->setVisible(false);
            ImageView* eye = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "eye");
            eye->setVisible(false);
       
            if(i == HallDataMgr::getInstance()->m_wChairID)
            {
                fScale = 1.f;
            }
            else
            {
                fScale = CARD_SCALE;
            }
            for(int index = 0;index < 3;index++)
            {
                addCard(i,Value_Back,index,nCount * SEND_CARD_INTERVAL_TIME*0.8 + SEND_CARD_INTERVAL_TIME*0.8 * totalCount * index,false,false,true,fScale);
            }
            nCount++;
        }
    }
    //设置加注按钮的分数
    for(int i = 0;i < 3;++i)
    {
        Button *addBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_buttomBg,__String::createWithFormat("addBtn%d",i)->getCString());
        Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBtn,"addNum");
        addNum->setString(__String::createWithFormat("%lld",baseScore*addTimesArrary[i])->getCString());
    }
    

    m_currentTotalNum->setString(__String::createWithFormat("%lld",totalCount*cmd_sgs->lCellScore*cmd_sgs->lCurrentTimes)->getCString());
    
    if(cmd_sgs->wCurrentUser == HallDataMgr::getInstance()->m_wChairID)
    {
        presentButtomBtn();
    }

    
    //加钱币
    srand(time(NULL));
    //判断放几个钱币
    LONGLONG LCount  = 2;
    for(int index = 0 ;index<GAME_PLAYER;index++)
    {
        if(m_cmd_GameStart.cbPlayStatus[index])
        {
//            Button * infoBtn = getInfoBtnByChairId(index);
//            for(int i =0; i < LCount;++i)
//            {
//                float posX = rand()%400 + 350;
//                float posY = rand()%150 + 300;
//                //钱币
//                auto pChip = Sprite::create(__String::createWithFormat("studioUi/gameMain/%dchip.png",rand()%3+1)->getCString());
//                pChip->setPosition(infoBtn->getPosition());
//                m_layout->addChild(pChip,1);
//                pChip->runAction(MoveTo::create(0.15f,Point(posX,posY)));
//                m_chipVe.pushBack(pChip);
//                
//            }
        }
        
    }
    
}

void GameLayer::onGameSceneAddScore(CMD_S_AddScore* cmd_sas)
{
//    updateOperatorText(cmd_sas->wCurrentUser);
//    m_lTotalScore[cmd_sas->wAddScoreUser] += cmd_sas->lUserScoreCount;
//    m_lTotalScore[cmd_sas->wAddScoreUser] = cmd_sas->lTurnLessScore;
//    m_lUserScore[cmd_sas->wAddScoreUser] = cmd_sas->lUserScoreCount;
//
    
    m_wLastAddUser = cmd_sas->wAddScoreUser;
    m_lCurrentTimes = cmd_sas->lCurrentTimes;
    Button * infoBtn = getInfoBtnByChairId(cmd_sas->wAddScoreUser);
    
    openScondCountByChairId(cmd_sas->wCurrentUser);
    ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
    int chipIndex = rand()%3+1;
    addChip(infoBtn->convertToWorldSpace(addBg->getPosition()),chipIndex);
    Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
    string strAddNum = addNum->getString();
    LONGLONG llAddNum = atoi(strAddNum.c_str())+cmd_sas->lAddScoreCount;
    addNum->setString(__String::createWithFormat("%lld",llAddNum)->getCString());
    m_lTableScore[cmd_sas->wAddScoreUser] = llAddNum;
//    //保存上一玩家押注
    m_llLastScore = cmd_sas->lAddScoreCount;

//    
    if(cmd_sas->wCurrentUser == HallDataMgr::getInstance()->m_wChairID)
    {
        
        Button *followGoldBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout, "followGoldBtn");
        if(m_llLastScore)
        {
              followGoldBtn->setColor(Color3B::WHITE);
        }
        else
        {
            followGoldBtn->setColor(Color3B::GRAY);
        }
        if(!m_bIsCompare)
        {
            log("presentButtomBtn addScore");
            presentButtomBtn();
        }
        
        else
        {
            log("m_bIsCompare");
        }
    }
    
    
   
    m_llTotalScore += cmd_sas->lAddScoreCount;
    ImageView *totalNumBg = (ui::ImageView*)ui::Helper::seekWidgetByName(m_layout, "totalNumBg");
    //totalNumBg->setVisible(true);
    
    TextAtlas *totalNum = (ui::TextAtlas*)ui::Helper::seekWidgetByName(totalNumBg, "totalNum");
    m_currentTotalNum->setString(__String::createWithFormat("%lld",m_llTotalScore)->getCString());
    
    if(cmd_sas->wCurrentUser != INVALID_CHAIR)
    {
//        Button * curInfoBtn = getInfoBtnByChairId(cmd_sas->wCurrentUser);

    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/ADD_SCORE.wav");
}

void GameLayer::onGameSceneGiveUp(CMD_S_GiveUp *cmd_gu)
{
    updateOperatorText(cmd_gu->wGiveUpUser);
    m_giveUpPlayer[cmd_gu->wGiveUpUser] = 0;
    m_cmd_GameStart.cbPlayStatus[cmd_gu->wGiveUpUser] = 0;

    //addOperateTip(cmd_gu->wGiveUpUser,operator_giveUp);
    if(cmd_gu->wGiveUpUser == HallDataMgr::getInstance()->m_wChairID)
    {

        hideButtomBtn();
        hideButtomAddBtn();
    }
    
    Button * infoBtn = getInfoBtnByChairId(cmd_gu->wGiveUpUser);

    
    
    Layout *cardLayout = getCardLayoutByChairId(cmd_gu->wGiveUpUser);
    Vector<Node*> veChild= cardLayout->getChildren();
    //放弃的用户牌全部盖住
    for(int i = 0;i<veChild.size();++i)
    {
        CardSprite* pCard = (CardSprite*)veChild.at(i);
        pCard->changeDisplay(Value_Break);
    }
    m_nPlayerNum--;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GIVE_UP.wav");
}

void GameLayer::onGameSceneSendCard(CMD_S_SendCard* cmd_ssc)
{

    m_llStartScore = 0;
    m_llLastScore = 0;

}


void GameLayer::onGameSceneEnd(CMD_S_GameEnd *cmd_ge)
{
    m_nChipCount = 0;
    m_bIsGameBegin = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/GAME_END.wav");
    hideButtomBg();
    presentStartBtns();
    Image * img = new Image;
    img->initWithImageFile("studioUi/game/jinhua_cardtype.png");
    Texture2D* pTypeTex = new Texture2D;
    pTypeTex->initWithImage(img);
    img->release();
    
    for(int i  = 0; i < GAME_PLAYER;++i)
    {
        Button * infoBtn = getInfoBtnByChairId(i);
        if(cmd_ge->lGameScore[i]>0)
            m_winnerChairId = i;
        if(cmd_ge->lGameScore[i] != 0)
        {
            
            Text *goldNum = (Text *)ui::Helper::seekWidgetByName(infoBtn, "goldNum");
            UserData *pUserData = (UserData*)infoBtn->getUserData();
            if(pUserData)
            {
                LONGLONG curScore = pUserData->m_date.lScore;
                //curScore += cmd_ge->lGameScore[i];
                pUserData->m_date.lScore = curScore;
                if(i == HallDataMgr::getInstance()->m_wChairID)
                    HallDataMgr::getInstance()->m_UserScore = pUserData->m_date.lScore;
                string scorestr = __String::createWithFormat("%lld",curScore)->getCString();
                scorestr = lkpy_game::getGoldStr(scorestr,',');
                scorestr = lkpy_game::getStringByWidth(scorestr,18,infoBtn->getContentSize().width-10);
                goldNum->setText(scorestr);
            }
           
        }
        
        closeScondCount();
        Layout *cardLayout = getCardLayoutByChairId(i);
        cardLayout->setOpacity(0);
        Vector<Node*> ve_children = cardLayout->getChildren();
        //显示用户得分
        //if(m_cmd_GameStart.cbCardData[i])
        {
            ImageView *addBg = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "addBg");
            Text *addNum = (ui::Text*)ui::Helper::seekWidgetByName(addBg, "addNum");
            if(cmd_ge->lGameScore[i]>0)
            {
                addNum->setString(__String::createWithFormat("+%lld",cmd_ge->lGameScore[i])->getCString());
            }
            else
            {
                addNum->setString(__String::createWithFormat("%lld",cmd_ge->lGameScore[i])->getCString());
            }
        }
        if(cmd_ge->lGameScore[i] && (m_giveUpPlayer[i]|| i == m_wMyChiarId) )
        {
            for(int index = 0 ;index < ve_children.size(); ++index)
            {
                 m_handCardArray[i][index] = cmd_ge->cbCardData[i][index];
                CardSprite *pScureCard = (CardSprite *)ve_children.at(index);
                pScureCard->changeDisplay(m_handCardArray[i][index]);
                
            }
          
            
            float fScale = 1;
            
            if(i == HallDataMgr::getInstance()->m_wChairID)
                fScale = 1;
            else
                fScale = CARD_SCALE;
            
            
            m_gameLogic->SortCardList(m_handCardArray[i],ve_children.size());
            int type = m_gameLogic->GetCardType(m_handCardArray[i],ve_children.size());
            
            if(type > 1)
            {
                Rect texRect((type-2)*70,0,70,35);
                Sprite * spTyep = Sprite::createWithTexture(pTypeTex,texRect);
                
                
                spTyep->setPosition(160 * fScale /2,110.f/2 * fScale);
                
                cardLayout->addChild(spTyep,2);
            }
            
        }
    }
    pTypeTex->release();
    Button * winnerBtn = getInfoBtnByChairId(m_winnerChairId);
    //if(m_cmd_GameStart.cbCardData[HallDataMgr::getInstance()->m_wChairID])
    {
        string soundName = "";
        if(m_winnerChairId == HallDataMgr::getInstance()->m_wChairID)
        {
            soundName = "sound_res/GAME_WIN.wav";
        }
        else
        {
            soundName = "sound_res/GAME_LOSE.wav";
        }
        
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2f), CallFunc::create([=]{
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundName.c_str());
        })));
    }
    
    float ftime = 0;
    for(int index = m_chipVe.size() - 1; index >=0;--index)
    {
        auto pChip =(Sprite*)m_chipVe.at(index);
        pChip->runAction(Sequence::create(DelayTime::create(ftime),MoveTo::create(CHIP_MOVE_TIME, winnerBtn->getPosition()), CallFunc::create([=]{
            pChip->setVisible(false);
        }),NULL));
        ftime+=0.05f;
    }
    hideButtomBtn();
    openScondCountByChairId(m_wMyChiarId);
}

void GameLayer::onGameSceneGetWinner(CMD_S_OpenCard *cmd)
{
    m_winnerChairId = cmd->wWinner;
   
}

void GameLayer::onGameSceneCompareCard(CMD_S_CompareCard *cmd)
{
    //m_bIsCompare = false;
    closeScondCount();
    playVsAnimation();
    m_nPlayerNum--;
    WORD wLoseUser = cmd->wLostUser;
    WORD wCurUser = cmd->wCurrentUser;
    log("CompareCard wCurUser %d",wCurUser);
    for(int index =0;index < GAME_PLAYER;++index)
    {
        Button* infoBtn = getInfoBtnByChairId(index);
        ImageView* arrowImg = (ImageView*)Helper::seekWidgetByName(infoBtn, "arrowImg");
        arrowImg->setVisible(false);
    }
    Vec2 resPos[2];
    Vec2 desPos[2] = {Vec2(368,320),Vec2(768,320)};
    WORD tempCompareUser[2];
    memcpy(tempCompareUser,cmd->wCompareUser,sizeof(cmd->wCompareUser)*sizeof(cmd->wCompareUser[0]));
    for(int i = 0;i < 2;++i)
    {
        if(cmd->wCompareUser[i] == m_wMyChiarId)
            desPos[i].y-=22;
        Layout* CardLayout = (Layout*)getWidgetByChairId(cmd->wCompareUser[i], "cardLayout%d");
        resPos[i] = CardLayout->getPosition();
        CardLayout->runAction(Sequence::create(
                                               Spawn::createWithTwoActions(MoveTo::create(VS_MOVE_TIME, desPos[i])
                                                                                          ,CallFunc::create([=]{
                                                   Vector<Node*> cardVe = CardLayout->getChildren();
                                                   
                                                   for(int index = 0;index < cardVe.size();++index)
                                                   {
                                                       CardSprite* pCard = (CardSprite*)cardVe.at(index);
                                                       pCard->runAction(Sequence::createWithTwoActions(
                                                                                                       ScaleTo::create(VS_MOVE_TIME,1.f)
                                                                                                       ,CallFunc::create([=]{
                                                           for(int index = 0;index < cardVe.size();++index)
                                                           {
                                                               CardSprite* pCard = (CardSprite*)cardVe.at(index);
                                                               
                                                               if(tempCompareUser[i] == wLoseUser)
                                                                   pCard->changeDisplay(Value_Break);
                                                           }
                                                       })));
                                                   }
            
        }))
                                               
                                               ,DelayTime::create(2.f)
                                               ,Spawn::createWithTwoActions(MoveTo::create(VS_MOVE_TIME, resPos[i])
                                                                            ,CallFunc::create([=]{
                                                   m_bIsCompare = false;
                                                   Vector<Node*> cardVe = CardLayout->getChildren();
                                                   float fScale = CARD_SCALE;
                                                   if(tempCompareUser[i] == m_wMyChiarId)
                                                       fScale = 1.0f;
                                                   for(int index = 0;index < cardVe.size();++index)
                                                   {
                                                       CardSprite* pCard = (CardSprite*)cardVe.at(index);
                                                
                                                       pCard->runAction(ScaleTo::create(VS_MOVE_TIME,fScale));
                                                   }
                                                   if(wCurUser == m_wMyChiarId)
                                                   {
                                                       log("presentButtomBtn compareCard");
                                                       presentButtomBtn();
                                                   }
                                                   if(wCurUser <= 5)
                                                   openScondCountByChairId(wCurUser);
                                               }))

                                               ,NULL));
    }
    
    m_giveUpPlayer[wLoseUser] = 0;
//    if(cmd->wCurrentUser == m_wMyChiarId)
//        presentButtomBtn();
}

void GameLayer::onGameSceneLookCard(CMD_S_LookCard *cmd)
{
    Button* infoBtn = getInfoBtnByChairId(cmd->wLookCardUser);
    ImageView *eye = (ui::ImageView*)ui::Helper::seekWidgetByName(infoBtn, "eye");
    eye->setVisible(true);
    if(cmd->wLookCardUser == HallDataMgr::getInstance()->m_wChairID)
    {
        Layout* cardLayout = (Layout*)getWidgetByChairId(HallDataMgr::getInstance()->m_wChairID, "cardLayout%d");
        Vector<Node*> ve_children = cardLayout->getChildren();
        for(int i = 0;i < ve_children.size();++i)
        {
            CardSprite* pCard = (CardSprite*)ve_children.at(i);
            pCard->changeDisplay(cmd->cbCardData[i]);
        }
    }
}

void GameLayer::onGameScenePlayerExit(CMD_S_PlayerExit* cmd)
{
    Layout *cardLayout = getCardLayoutByChairId(cmd->wPlayerID);
    Vector<Node*> veChild= cardLayout->getChildren();
    //放弃的用户牌全部盖住
    for(int i = 0;i<veChild.size();++i)
    {
        CardSprite* pCard = (CardSprite*)veChild.at(i);
        pCard->changeDisplay(Value_Break);
    }
    m_nPlayerNum--;
    m_cmd_GameStart.cbPlayStatus[cmd->wPlayerID] = 0;
    m_giveUpPlayer[cmd->wPlayerID] = 0;
}

void GameLayer::updateOperatorText(WORD _chairId)
{
    return;
    if(_chairId == INVALID_CHAIR)
        return;
    presentButtomBg();
    Text *operatorText = (ui::Text*)ui::Helper::seekWidgetByName(m_buttomBg,"operatorText");
    if(_chairId == HallDataMgr::getInstance()->m_wChairID)
    {
        operatorText->setVisible(false);
    }
    else{
        //operatorText->setVisible(true);
        Button *infoBtn = getInfoBtnByChairId(_chairId);
        UserData * pUserData = (UserData *)infoBtn->getUserData();
        if(pUserData)
        {
            operatorText->setString(__String::createWithFormat("等待玩家 %s 下注",pUserData->m_nickname.c_str())->getCString());
            operatorText->setVisible(true);
        }
        
    }
    
}


CardSprite* GameLayer::addCard(WORD _chairId,BYTE bValue,int cardIndex,float delayTime,bool bChangeValue,bool bDoNotMove,bool bPlayEffect,float fScale)
{
//    Button * infoBtn = (Button*)pNode;
//    string strName = infoBtn->getName();
//    char cNum = strName.back();
//    
//    int tag = atoi(&cNum);
//    //infoBtn->getTag();
//    
//    Layout *cardLayout = (ui::Layout*)ui::Helper::seekWidgetByName(m_layout,__String::createWithFormat("cardLayout%d",tag)->getCString());
    Layout *cardLayout = (Layout *)getWidgetByChairId(_chairId, "cardLayout%d");
    Size layoutSize = cardLayout->getContentSize();
    CardSprite *pCard1 = CardSprite::createWithCardSize(bValue);
    //pCard1->setScale(fScale);
    Size cardSize = pCard1->getContentSize();
    
    pCard1->setPosition(layoutSize.width/2 - pCard1->getContentSize().width/2 +pCard1->getContentSize().width/2*cardIndex ,layoutSize.height/2);
    pCard1->setTag(Tag_Card+cardIndex);
    //infoBtn->addChild(pCard1);
    cardLayout->addChild(pCard1);
    
    
    Point endPos = pCard1->getPosition();
    if(bDoNotMove)
    {
        pCard1->setPosition(endPos);
        if(bChangeValue)
            pCard1->changeDisplay(pCard1->m_nValue);
    }
    
    else{
        Point centerPos = cardLayout->convertToNodeSpace(Point(568,320));
        pCard1->setPosition(centerPos);
        pCard1->runAction(Sequence::create(DelayTime::create(delayTime),
                                           Spawn::create(MoveTo::create(SEND_CARD_INTERVAL_TIME,endPos)
                                                         ,ScaleTo::create(SEND_CARD_INTERVAL_TIME, fScale)
                                                         ,NULL)
                                           
                                           ,CallFunc::create([=]{
            if(bChangeValue)
                pCard1->changeDisplay(pCard1->m_nValue);
        }),NULL));
    }
    if(bPlayEffect)
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/SEND_CARD.wav");
    return  pCard1;
}

void GameLayer::addChip(Vec2 pos,int chipIndex,LONGLONG lScore)
{
    //    float posX = rand()%400 + 350;
    //    float posY = rand()%150 + 300;
    //钱币
    auto pChip = Sprite::create(__String::createWithFormat("studioUi/game/%dchip.png",chipIndex)->getCString());
    //pChip->setScale(0.5f);
    pChip->setPosition(pos);
    m_layout->addChild(pChip,1);
    pChip->runAction(Sequence::createWithTwoActions(MoveTo::create(CHIP_MOVE_TIME-0.1f,Vec2(572,322 +m_nChipCount*3)),CallFunc::create([=]{
        //pChip->removeFromParent();
    }))
                     );
    m_chipVe.pushBack(pChip);
    
    m_nChipCount++;
    //    LONGLONG realCount = getIntergarNum(lScore);
    //    string scoreStr;
    //    if(realCount < 10000)
    //        scoreStr =  __String::createWithFormat("%lld",realCount)->getCString();
    //    else
    //    {
    //        scoreStr =  __String::createWithFormat("%lld万",realCount/10000)->getCString();
    //    }
    //    Text* scoreText = Text::create(scoreStr,FontBold,22);
    //    scoreText->setColor(Color3B::YELLOW);
    //    scoreText->setPosition(Vec2(pChip->getContentSize().width/2,pChip->getContentSize().height/2+2));
    //    pChip->addChild(scoreText,1);
    
}

Button* GameLayer::getInfoBtnByChairId(WORD _chairId)
{
    WORD myChairId = HallDataMgr::getInstance()->m_wChairID;
    int userIndex = 0;
    WORD startInfoBgIndex = myChairId;
    //计算出游戏中的图片位置
    while (true) {
        if(startInfoBgIndex == _chairId)
            break;
        userIndex++;
        startInfoBgIndex++;
        if(startInfoBgIndex >= 5)
            startInfoBgIndex = 0;
        
    }
    Button *infoBtn = (ui::Button*)ui::Helper::seekWidgetByName(m_layout,__String::createWithFormat("infoBtn%d",userIndex)->getCString());
    return infoBtn;
}

Layout* GameLayer::getCardLayoutByChairId(WORD _chairId)
{
    WORD myChairId = HallDataMgr::getInstance()->m_wChairID;
    int userIndex = 0;
    WORD startInfoBgIndex = myChairId;
    //计算出游戏中的图片位置
    while (true) {
        if(startInfoBgIndex == _chairId)
            break;
        userIndex++;
        startInfoBgIndex++;
        if(startInfoBgIndex >= 5)
            startInfoBgIndex = 0;
        
    }
    Layout *layout = (ui::Layout*)ui::Helper::seekWidgetByName(m_layout,__String::createWithFormat("cardLayout%d",userIndex)->getCString());
    return layout;
}

Widget* GameLayer::getWidgetByChairId(WORD _chairId,const char *format)
{
    WORD myChairId = HallDataMgr::getInstance()->m_wChairID;
    int userIndex = 0;
    WORD startInfoBgIndex = myChairId;
    //计算出游戏中的图片位置
    while (true) {
        if(startInfoBgIndex == _chairId)
            break;
        userIndex++;
        startInfoBgIndex++;
        if(startInfoBgIndex >= 5)
            startInfoBgIndex = 0;
        
    }
    Widget *widget = ui::Helper::seekWidgetByName(m_layout,__String::createWithFormat(format,userIndex)->getCString());
    return widget;
}

void GameLayer::showTips(const std::string &str)
{
    auto pstr = Label::createWithSystemFont(str, FontNormal, 20);
    pstr->setColor(Color3B::YELLOW);
    
    auto pbg = ImageView::create("game_res/clew_box.png");
    pbg->setScale9Enabled(true);
    pbg->setContentSize(cocos2d::Size(pstr->getContentSize().width+60,40));
    pbg->setScaleX(0.1f);
    pstr->setPosition(Vec2(pbg->getContentSize().width/2, 20));
    pbg->addChild(pstr);
    this->showTips(pbg);
}


void GameLayer::showTips(cocos2d::Node *pNode)
{
    int index = m_infolist.size();
    while (index >= 3) {
        auto pnode = m_infolist.at(0);
        pnode->removeFromParent();
        m_infolist.erase(0);
        for (auto iter : m_infolist) {
            iter->runAction(MoveBy::create(0.2f, Vec2(0.f, 60.f)));
        }
        index = m_infolist.size();
    }
    pNode->setPosition(Vec2(WinSize.width/2, WinSize.height-120-60*index));
    this->addChild(pNode,Tag_UI_Layer);
    m_infolist.pushBack(pNode);
    
    pNode->runAction(Sequence::create(ScaleTo::create(0.17f, 1.f), DelayTime::create(5.f), ScaleTo::create(0.17f, 0.1f, 1.f), CallFunc::create([=]{
        pNode->removeFromParent();
        m_infolist.eraseObject(pNode);
        if (m_infolist.size()) {
            for (auto iter : m_infolist) {
                iter->runAction(MoveBy::create(0.2f, Vec2(0.f, 60.f)));
            }
        }}), NULL));

}



void GameLayer::cannonlayerCallback()
{
    if (m_menu) {
        m_menu->removeFromParent();
        m_menu = nullptr;
    }
    
    if (m_multiple) {
        m_multiple->removeFromParent();
        m_multiple = nullptr;
    }
}

void GameLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}
void GameLayer::eventScoreChange(cocos2d::EventCustom *event)
{
    auto pscore = (Label *)m_uiLayer->getChildByTag(1)->getChildByTag(Tag_UserScore);
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser && pscore) {
        std::string scorestr = __String::createWithFormat("%lld",puser->m_date.lScore)->getCString();
        scorestr = lkpy_game::getGoldStr(scorestr,',');
        if (scorestr.length() > 16) {
            scorestr = scorestr.substr(0,14);
            scorestr.append("::");
        }
        pscore->setString(scorestr);
    }
}


void GameLayer::messageChat(void *pData, WORD wSize)
{
    CMD_GF_S_UserChat* cmd_gf_cuc = (CMD_GF_S_UserChat*) pData;
    int chatLength = cmd_gf_cuc->wChatLength;
    TCHAR ChatStr[LEN_USER_CHAT];
    memset(ChatStr, 0, sizeof(ChatStr));
    memcpy(ChatStr,(char *)cmd_gf_cuc->szChatString,chatLength);
    string chatString = WHConverUnicodeToUtf8WithArray(ChatStr);
    int strSize = strLength(chatString);
    int _chairId = 0;
    
    for (int i = 0;i < GAME_PLAYER ; ++i){
        Button* infoBtn = (Button*)getWidgetByChairId(i, "infoBtn%d");
        UserData* pUserData = (UserData*)infoBtn->getUserData();
        if(pUserData && pUserData->m_date.dwUserID == cmd_gf_cuc->dwSendUserID)
        {
            _chairId = i;
            break;
        }
    }
    
    ImageView* chatBg = (ImageView*)getWidgetByChairId(_chairId, "chatBg%d");
    
    string strName = chatBg->getName();
    char cNum = strName.back();
    int nPos = atoi(&cNum);
    
    chatBg->stopAllActions();
    chatBg->setOpacity(255);
    chatBg->setVisible(true);
    chatBg->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), FadeOut::create(1.3f)));
    Size bgSize(20*strSize+30,48);
    chatBg->setContentSize(bgSize);
    
    Text* chatText = (Text*)Helper::seekWidgetByName(chatBg, "chatText");
    
    Label *chatLabel = (Label*)chatBg->getChildByTag(Tag_ChatLabel);
    chatLabel->setVisible(true);
    //    int r = cmd_gf_cuc->dwChatColor&255;
    //    int g = (cmd_gf_cuc->dwChatColor>>8)&255;
    //    int b = (cmd_gf_cuc->dwChatColor>>16)&255;
    //    chatText->setColor(Color3B(r,g,b));
    chatLabel->setString(chatString);
    chatText->setPositionX(bgSize.width/2);
    chatLabel->setVisible(true);
    chatLabel->setColor(Color3B(0,0,0));
    chatLabel->stopAllActions();
    chatLabel->setOpacity(255);
    
    //chatLabel->setDimensions(0,0);
    float width = chatLabel->getContentSize().width;
    if(width>230)
    {
        chatLabel->setDimensions(230,0);
        width = chatLabel->getContentSize().width;
    }
    
    //int width = getTextWidth(chatString,chatText->getFontSize());
    chatBg->setContentSize(Size(width+24,chatLabel->getContentSize().height+10));
    chatLabel->setPosition(chatBg->getContentSize()/2);
    chatLabel->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), FadeOut::create(1.3f)));
    ImageView* browImg = (ImageView*)Helper::seekWidgetByName(chatBg, "browImg");
    browImg->setVisible(false);
    
    UserData* pUd = HallDataMgr::getInstance()->m_UserList.at(cmd_gf_cuc->dwSendUserID);
    m_recordUserVe.push_back(pUd->m_nickname);
    m_recordTypeVe.push_back(0);
    m_recordContentVe.push_back(chatString);
}

void GameLayer::messageBrow(void *pData, WORD wSize)
{
    CMD_GF_S_UserExpression* cmd_gf_cue = (CMD_GF_S_UserExpression*) pData;
    int _chairId = 0;
    for (int i = 0;i < GAME_PLAYER ; ++i){
        Button* infoBtn = (Button*)getWidgetByChairId(i, "infoBtn%d");
        UserData* pUserData = (UserData*)infoBtn->getUserData();
        if(pUserData && pUserData->m_date.dwUserID == cmd_gf_cue->dwSendUserID)
        {
            _chairId = i;
            break;
        }
    }
    ImageView* chatBg = (ImageView*)getWidgetByChairId(_chairId, "chatBg%d");
    
    string strName = chatBg->getName();
    char cNum = strName.back();
    int nPos = atoi(&cNum);
    
    chatBg->stopAllActions();
    chatBg->setOpacity(255);
    chatBg->setVisible(true);
    chatBg->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), FadeOut::create(1.3f)));
    chatBg->setVisible(true);
    Size bgSize(120,48);
    chatBg->setContentSize(bgSize);
    //Text* chatText = (Text*)Helper::seekWidgetByName(chatBg, "chatText");
    Label *chatLabel = (Label*)chatBg->getChildByTag(Tag_ChatLabel);
    chatLabel->setVisible(false);
    ImageView* browImg = (ImageView*)Helper::seekWidgetByName(chatBg, "browImg");
    browImg->loadTexture(__String::createWithFormat("studioUi/brow/%d.png",cmd_gf_cue->wItemIndex)->getCString());
    //if(nPos >1 && nPos < 3)
        browImg->setPositionX(bgSize.width/2);
    //else
        //browImg->setPositionX(-bgSize.width/2);
    browImg->stopAllActions();
    browImg->setVisible(true);
    browImg->setOpacity(255);
    browImg->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), FadeOut::create(1.3f)));
    
    UserData* pUd = HallDataMgr::getInstance()->m_UserList.at(cmd_gf_cue->dwSendUserID);
    m_recordUserVe.push_back(pUd->m_nickname);
    m_recordTypeVe.push_back(1);
    m_recordContentVe.push_back(__String::createWithFormat("%d",cmd_gf_cue->wItemIndex)->getCString());

}

Button* GameLayer::getInfoBtnByUserId(DWORD dwUserId)
{
    for(int i = 0;i < GAME_PLAYER;++i)
    {
        Button* infoBtn = getInfoBtnByChairId(i);
        UserData * pUserData = (UserData*)infoBtn->getUserData();
        if(pUserData && pUserData->m_date.dwUserID == dwUserId)
        {
            return infoBtn;
        }
    }
}

void GameLayer::onGameScene(void *pData, WORD wSize)
{
    if (wSize == sizeof(CMD_S_StatusFree))
    {
        CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
        onGameSceneFree(pStatusFree);
        //设置界面
    }
    else if (wSize == sizeof(CMD_S_GameStart))
    {
        CMD_S_GameStart * pStatusStart=(CMD_S_GameStart *)pData;
        onGameSceneStart(pStatusStart);
    }
    else if (wSize == sizeof(CMD_S_StatusPlay))
    {
        CMD_S_StatusPlay* cmd_ss = (CMD_S_StatusPlay*)pData;
        onGameScenePlay(cmd_ss);
        
    }
}

void GameLayer::networkGRGame(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case zjh_SUB_S_GAME_START:
        {
            CMD_S_GameStart *cmd_sgs = (CMD_S_GameStart *)pData;
            onGameSceneStart(cmd_sgs);
        }
            break;
        case zjh_SUB_S_ADD_SCORE:
        {
            CMD_S_AddScore *cmd_sas = (CMD_S_AddScore *)pData;
            onGameSceneAddScore(cmd_sas);
            
        }
            break;
        case zjh_SUB_S_GIVE_UP:
        {
            CMD_S_GiveUp *cmd_gu = (CMD_S_GiveUp *)pData;
            onGameSceneGiveUp(cmd_gu);
        }
            break;
        case zjh_SUB_S_SEND_CARD:
        {
            CMD_S_SendCard *cmd_ssc = (CMD_S_SendCard *)pData;
            onGameSceneSendCard(cmd_ssc);
            
        }
            break;
        case zjh_SUB_S_GAME_END:
        {
            CMD_S_GameEnd *cmd_ge = (CMD_S_GameEnd *)pData;
            onGameSceneEnd(cmd_ge);
        }
            break;
        case zjh_SUB_S_COMPARE_CARD:
        {
            CMD_S_CompareCard *cmd = (CMD_S_CompareCard *)pData;
            onGameSceneCompareCard(cmd);
            
        }
            break;
        case zjh_SUB_S_LOOK_CARD:
        {
            CMD_S_LookCard *cmd = (CMD_S_LookCard *)pData;
            onGameSceneLookCard(cmd);
            
        }
            break;
        case zjh_SUB_S_PLAYER_EXIT:
        {
            CMD_S_PlayerExit *cmd = (CMD_S_PlayerExit *)pData;
            onGameScenePlayerExit(cmd);
        }
            break;
        case zjh_SUB_S_OPEN_CARD:
        {
            
        }
            break;
        case zjh_SUB_S_WAIT_COMPARE:
        {
            
        }
            break;
        case zjh_SUB_S_ANDROID_CARD:
        {
            
        }
            break;
        case zjh_SUB_S_CHEAT_CARD:
        {
            
        }
            break;
            
            break;
    }

}

void GameLayer::OnUserChat(WORD wSubCode, void *pData, WORD wSize)
{
    switch (wSubCode)
    {
        case SUB_GF_USER_CHAT:
        {
            this->messageChat(pData,wSize);
        }
            break;
        case SUB_GF_USER_EXPRESSION:
        {
            this->messageBrow(pData,wSize);
        }
            break;
        default:
            break;
    }
}



