//
//  GameLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#include "brnn_GameLayer.h"
#include "Project.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "SimpleAudioEngine.h"
#include "GameLogic.h"
#include "ModeLayer.h"
#include "../RoomSrc/BankLayer.h"
#include "../RoomSrc/brnn_RoomLayer.h"
#include "NameLabel.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace brnn_game;

#define ScoreColor  Color3B(249, 222, 167)//分数颜色值
#define ButtonLigntAnim   "lightanim"//按钮发亮动画
const int scorelist[5] = {100,1000,10000,100000,1000000};//筹码加注按钮对应值
const int goldbasenum[5] = {1,2,3,4,5};//每个加注筹码对应金币个数
const int goldmaxnum[5] = {3,6,9,12,15};//每个加注筹码对应最多金币个数
#define HornShow  "hornshow" //喇叭定时器Tag
#define MaxTimes   10        //最大赔率

enum ButtonTag
{
    Tag_BT_Return = 0,//返回按钮
    Tag_BT_SZ,//上庄按钮
    Tag_BT_Bank,
    Tag_BT_Horn,//喇叭
    Tag_BT_Small_Horn,//小喇叭
    Tag_BT_Player,//玩家列表
    Tag_BT_Trend,//游戏走势
    Tag_BT_SelfHead,//自己头像
    Tag_BT_BankerHead,//庄家头像
    
    Tag_BT_Area = 20,//游戏下注区 20-23
    
    Tag_BT_Score = 25,//游戏下注按钮 25-29
    
    Tag_Area_Score = 30,//区域总下注
    
    Tag_Area_Self_Score = 35,//区域自己下注分数
    
    Tag_Area_Niu_Dot = 40,//牛的点数显示
    
    Tag_Txt_Time = 45,//时间显示
    Tag_Txt_Horn,//喇叭内容显示框
    Tag_Self_Score,//自己分数显示
    
    Tag_Banker_Head,//庄家头像
    Tag_Banker_Txt,//庄这个字
    Tag_Banker_Name,//庄家名称
    Tag_Banker_Score,//庄家分数
    Tag_Banker_Gold,//庄家金币
    
    Tag_CountDown_Txt,//倒计时文字
    Tag_CountDown_Num,//倒计时数字
    
    Tag_SendCard_Action,//发牌动画
    Tag_CountDown_Action,//倒计时
    
    Tag_End_Layer,//结算层
    Tag_UserList_Layer,//用户列表层
    Tag_Record_Lyaer,//游戏走势层
    Tag_Bank_Layer,//银行层
    Tag_Apply_Banker_Layer,//请求上庄层
    Tag_Horn_Layer,//喇叭内容输入层
};


Scene* GameLayer::createScene()
{
    auto scene = Scene::createWithPhysics();
    
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto layer = GameLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    HallDataMgr::getInstance()->m_RoomType = Data_Room;
    HallDataMgr::getInstance()->m_bStartGame = true;
    auto viewsize = WinSize;
    NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameLayer::onGameScene, this);
    NetworkMgr::getInstance()->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(GameLayer::onGameMessage, this));
    NetworkMgr::getInstance()->m_Userdelegate = this;
    this->loadlightAnim();
    m_scoreIndex = -1;
    m_ApplyBankerStatus = 0;
    m_gamelogic = new CGameLogic;
    m_chatindex = 0;
    
    m_cardVec2[0] = Vec2(viewsize.width/2-106, viewsize.height-60);
    m_cardVec2[1] = Vec2(viewsize.width/2-470, 188);
    m_cardVec2[2] = Vec2(viewsize.width/2-470+244, 188);
    m_cardVec2[3] = Vec2(viewsize.width/2-470+244*2, 188);
    m_cardVec2[4] = Vec2(viewsize.width/2-470+244*3, 188);
    
    auto pbg = ImageView::create("game_res/game_bg.png");
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    this->addChild(pbg);
    
    auto ptop = ImageView::create("game_res/im_top.png");
    ptop->setAnchorPoint(Vec2(0.5f, 1.f));
    ptop->setPosition(Vec2(viewsize.width/2, viewsize.height));
    this->addChild(ptop);
    
    auto pbottom = ImageView::create("game_res/im_bottom.png");
    pbottom->setAnchorPoint(Vec2(0.5f, 0.f));
    pbottom->setPosition(Vec2(viewsize.width/2-40, 0));
    this->addChild(pbottom);
    
    //银行
    auto pbank = Button::create("game_res/bt_bank_0.png", "game_res/bt_bank_1.png");
    pbank->setTag(Tag_BT_Bank);
    pbank->setPosition(Vec2(viewsize.width/2+340, viewsize.height-40));
    pbank->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    this->addChild(pbank);
    
    //喇叭
    auto phorn = Button::create("game_res/bt_horn_0.png", "game_res/bt_horn_1.png");
    phorn->setTag(Tag_BT_Horn);
    phorn->setPosition(Vec2(viewsize.width/2+440, viewsize.height-40));
    phorn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    this->addChild(phorn);
    
    //趋势走向
    auto ptrend = Button::create("game_res/bt_trend_0.png", "game_res/bt_trend_1.png");
    ptrend->setTag(Tag_BT_Trend);
    ptrend->setPosition(Vec2(viewsize.width/2+480, 34));
    ptrend->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    this->addChild(ptrend);
    if (viewsize.width == 960) {
        ptrend->setPosition(Vec2(viewsize.width/2+400, 108));
    }
    
    //玩家列表
    auto pplayer = Button::create("game_res/bt_player_0.png", "game_res/bt_player_1.png");
    pplayer->setTag(Tag_BT_Player);
    pplayer->setPosition(Vec2(viewsize.width/2+400, 34));
    pplayer->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    this->addChild(pplayer);
    
    //上庄按钮
    auto pSZ = Button::create("game_res/bt_sz_0_0.png", "game_res/bt_sz_0_1.png");
    pSZ->setTag(Tag_BT_SZ);
    pSZ->setPosition(Vec2(viewsize.width/2+172, viewsize.height-30));
    pSZ->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    this->addChild(pSZ);
    
    //返回按钮
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setTag(Tag_BT_Return);
    preturn->setPosition(Vec2(80, viewsize.height-70));
    preturn->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    this->addChild(preturn);
    
    //时间显示
    auto ptimebg = ImageView::create("game_res/im_time_bg.png");
    ptimebg->setPosition(Vec2(viewsize.width/2-408, viewsize.height-26));
    this->addChild(ptimebg);
    
    auto ptime = Label::createWithCharMap("game_res/num_time.png", 13, 19, '/');
    ptime->setTag(Tag_Txt_Time);
    ptime->setPosition(Vec2(ptimebg->getPositionX(), viewsize.height-26));
    ptime->setString("00/00");
    this->addChild(ptime);
    
    //下注按钮
    for (int index=0; index<5; ++index) {
        std::string normalstr = __String::createWithFormat("game_res/bt_score_%d_0.png", index)->getCString();
        std::string selectstr = __String::createWithFormat("game_res/bt_score_%d_1.png", index)->getCString();
        std::string disstr = __String::createWithFormat("game_res/bt_score_%d_3.png", index)->getCString();
        auto pscore = Button::create(normalstr, selectstr, disstr);
        pscore->setTag(Tag_BT_Score+index);
        pscore->setPosition(Vec2(viewsize.width/2-188+112*index, 42));
        pscore->setEnabled(false);
        pscore->setBright(false);
        pscore->addTouchEventListener(CC_CALLBACK_2(GameLayer::scoretouchEvent, this));
        this->addChild(pscore);
    }
    
    //头像框
    auto pZheadbg = Button::create("game_res/im_head_bg.png", "game_res/im_head_bg.png");
    pZheadbg->setPosition(Vec2(viewsize.width/2-286, viewsize.height-56));
    pZheadbg->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pZheadbg->setTag(Tag_BT_BankerHead);
    this->addChild(pZheadbg);
    
    auto pzscorebg = ImageView::create("game_res/im_score_bg.png");
    pzscorebg->setPosition(Vec2(viewsize.width/2-166, viewsize.height-48));
    this->addChild(pzscorebg);
    
    auto pselfheadbg = Button::create("game_res/im_head_bg.png", "game_res/im_head_bg.png");
    pselfheadbg->setPosition(Vec2(viewsize.width/2-440, 64));
    pselfheadbg->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pselfheadbg->setTag(Tag_BT_SelfHead);
    this->addChild(pselfheadbg);
    
    //自己头像
    auto headimage = HeaderRequest::createwithFaceID(
                                                     HallDataMgr::getInstance()->m_wFaceID,
                                                     HallDataMgr::getInstance()->m_wCustom,
                                                     HallDataMgr::getInstance()->m_dwUserID,
                                                     HallDataMgr::getInstance()->m_cbGender
                                                     );
    headimage->setHeadSize(100);
    
    auto clipnode = ClippingNode::create();
    clipnode->addChild(headimage);
    auto circleNode = DrawNode::create();
    const int maxTrangle = 360;
    Vec2 circleVec2[maxTrangle];
    for (int i = 0; i < maxTrangle; i ++)
    {
        float x = cosf( i * (M_PI/180.f)) * 50;
        float y = sinf( i * (M_PI/180.f)) * 50;
        circleVec2[i] = Vec2(x, y);
    }
    auto circleColor = Color4F(0, 1, 0, 1);
    circleNode->drawPolygon(circleVec2, maxTrangle, circleColor, 1, circleColor);
    clipnode->setStencil(circleNode);
    clipnode->setPosition(Vec2(pselfheadbg->getPositionX(), 64));
    this->addChild(clipnode);
    
    //名称
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    auto pName = NameLabel::create(puser->m_nickname, FontNormal, 20);
    pName->setFontColor(Color3B(50, 17, 0));
    pName->setAnchorPoint(Vec2(0.5f, 0.5f));
    pName->setPosition(Vec2(pselfheadbg->getPositionX()+127, 58));
    pName->setSize(cocos2d::Size(130, 25));
    this->addChild(pName);
    
    auto pscorebg = ImageView::create("game_res/im_score_bg.png");
    pscorebg->setPosition(Vec2(pselfheadbg->getPositionX()+114, 22));
    this->addChild(pscorebg);
    
    //金币
    auto pSmallGold = ImageView::create("game_res/im_small_gold.png");
    pSmallGold->setPosition(Vec2(pselfheadbg->getPositionX()+64, 22));
    this->addChild(pSmallGold);
    
    m_showscore = puser->m_date.lScore;
    auto pScore = Label::createWithSystemFont(getScoreString(m_showscore), FontNormal, 25);
    pScore->setAnchorPoint(Vec2(0.f, 0.5f));
    pScore->setColor(ScoreColor);
    pScore->setPosition(Vec2(pSmallGold->getPositionX()+15, pSmallGold->getPositionY()));
    pScore->setTag(Tag_Self_Score);
    this->addChild(pScore);
    
    //喇叭显示
    auto psmallhorn = Button::create("game_res/bt_small_horn_0.png", "game_res/bt_small_horn_1.png");
    psmallhorn->setTag(Tag_BT_Small_Horn);
    psmallhorn->setEnabled(false);
    psmallhorn->setPosition(Vec2(250, viewsize.height-142));
    this->addChild(psmallhorn);
    
    auto phornframe = ImageView::create("game_res/im_horn_frame.png");
    phornframe->setAnchorPoint(Vec2(0.f, 0.5f));
    //phornframe->setTag(Tag_Txt_Horn);
    phornframe->setPosition(Vec2(psmallhorn->getPositionX()+30, psmallhorn->getPositionY()));
    this->addChild(phornframe);
    
    auto txtnode = ui::ScrollView::create();
    txtnode->setContentSize(cocos2d::Size(540, 48));
    txtnode->setAnchorPoint(Vec2(0.f, 0.5f));
    txtnode->setPosition(Vec2(290, psmallhorn->getPositionY()));
    txtnode->setTag(Tag_Txt_Horn);
    txtnode->setEnabled(false);
    this->addChild(txtnode);
    
    //下注区
    for (int index=0; index<4; ++index) {
        std::string imagestr = __String::createWithFormat("game_res/bt_area_%d.png", index)->getCString();
        auto parea = Button::create(imagestr, imagestr);
        parea->setTag(Tag_BT_Area+index);
        parea->setAnchorPoint(Vec2(0.5f, 1.f));
        parea->setPosition(Vec2(viewsize.width/2-370+244*index, viewsize.height-168));
        parea->addTouchEventListener(CC_CALLBACK_2(GameLayer::areatouchEvent, this));
        this->addChild(parea);
    }
    

    this->scheduleUpdate();
    this->schedule(CC_CALLBACK_1(GameLayer::hornUpdate, this), 4.f, HornShow);
    
    return true;
};

void GameLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_BT_SZ:
                {
                    if (m_ApplyBankerStatus == 0 || m_ApplyBankerStatus==1) {
                        this->showApplyBankerList();
                    }
                    else if (m_ApplyBankerStatus == 2)
                    {
                        if (m_cbGameStatus != GAME_SCENE_FREE) {
                            HallDataMgr::getInstance()->AddpopLayer("系统提示", "只有在休息状态下，才能下庄", Type_Ensure);
                            return;
                        }
                        NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_CANCEL_BANKER, NULL, 0);
                        m_ApplyBankerStatus = 0;
                        this->updateApplyBankerButton();
                    }
                }
                    break;
                case Tag_BT_Bank:
                {
//                    if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
//                        HallDataMgr::getInstance()->AddpopLayer("系统提示", "游客禁止操作，请注册帐号", Type_Ensure);
//                        return;
//                    }
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
                    BankLayer *player = BankLayer::createWithGame();
                    player->setTag(Tag_Bank_Layer);
                    this->addChild(player,3);
                    bool save = m_cbGameStatus==GAME_SCENE_FREE?true:false;
                    player->updategamesave(save);
                }
                    break;
                case Tag_BT_Return:
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/back.wav");
                    if (m_cbGameStatus != GAME_STATUS_FREE) {
                        LONGLONG score = 0;
                        for (int index=0; index<5; ++index) {
                            score += m_lUserJettonScore[index];
                        }
                        if (score) {
                            auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("系统提示", "您正在游戏中，强行退出会被扣分，确实要强退吗？", Type_Ensure_Cancel));
                            player->setEnsureCallback([=]{NetworkMgr::getInstance()->sendLeaveGame();});
                            return;
                        }
                    }
                    NetworkMgr::getInstance()->sendLeaveGame();
                }
                    break;
                case Tag_BT_Trend:
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
                    this->showGameRecord();
                }
                    break;
                case Tag_BT_Player:
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
                    this->showUserList();
                }
                    break;
                case Tag_BT_Horn:
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
                    this->showEditHorn();
                }
                    break;
                case Tag_BT_SelfHead:
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
                    this->showUserInfo(m_UserList.at(HallDataMgr::getInstance()->m_dwUserID));
                }
                    break;
                case Tag_BT_BankerHead:
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_alert.wav");
                    //this->showUserInfo(getUserByChairID(m_wBankerUser));
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

void GameLayer::scoretouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag()-Tag_BT_Score;
            if (ntag == m_scoreIndex) {
                return;
            }
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/choice_score.wav");
            auto pbutton1 = static_cast<Button *>(this->getChildByTag(Tag_BT_Score + m_scoreIndex));
            if (pbutton1) {
                pbutton1->loadTextureNormal(__String::createWithFormat("game_res/bt_score_%d_0.png", m_scoreIndex)->getCString());
                pbutton1->removeAllChildren();
            }
            pbutton->loadTextureNormal(__String::createWithFormat("game_res/bt_score_%d_2.png", ntag)->getCString());
            auto pimage = Sprite::create("game_res/anim_light_0.png");
            pimage->setPosition(Vec2(pbutton->getContentSize().width/2, pbutton->getContentSize().height/2));
            pbutton->addChild(pimage);
            auto lightanim = Animate::create(AnimationCache::getInstance()->getAnimation(ButtonLigntAnim));
            auto paction = RepeatForever::create(lightanim);
            pimage->runAction(paction);
            m_scoreIndex = ntag;
        }
            break;
            
        default:
            break;
    }

}

void GameLayer::areatouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    //不在下注状态
    if (m_cbGameStatus != GAME_SCENE_PLACE_JETTON || m_scoreIndex == -1 || m_ApplyBankerStatus==2) {
        return;
    }
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag()-Tag_BT_Area;
            auto jettonscore = scorelist[m_scoreIndex];
            //自己以下注金币
            LONGLONG selfscore = m_lUserJettonScore[1] + m_lUserJettonScore[2] + m_lUserJettonScore[3] + m_lUserJettonScore[4] + jettonscore;
            if (selfscore > m_lUserMaxScore && m_lUserMaxScore) {
                HallDataMgr::getInstance()->AddpopLayer("", "已超过个人最大下注值", Type_Info_Reminder);
                return;
            }
            LONGLONG areascore = m_lAllJettonScore[ntag+1] + jettonscore;
            if (areascore > m_lAreaLimitScore && m_lAreaLimitScore) {
                HallDataMgr::getInstance()->AddpopLayer("", "已超过该区域最大下注值", Type_Info_Reminder);
                return;
            }
            
            if (m_wBankerUser != INVALID_CHAIR) {
                LONGLONG allscore = m_lAllJettonScore[1] + m_lAllJettonScore[2] + m_lAllJettonScore[3] + m_lAllJettonScore[4];
                auto pbanker = this->getUserByChairID(m_wBankerUser);
                if (pbanker) {
                    LONGLONG maxscore = pbanker->m_date.lScore/MaxTimes;
                    if (maxscore < (allscore + jettonscore)) {
                        HallDataMgr::getInstance()->AddpopLayer("", "总下注已超过庄家下注上限", Type_Info_Reminder);
                        this->updateScoreButton(maxscore-allscore);
                        return;
                    }
                }
            }
            
            
            //发送加注消息
            CMD_C_PlaceJetton placejetton;
            memset(&placejetton, 0, sizeof(placejetton));
            placejetton.lJettonScore = jettonscore;
            placejetton.cbJettonArea = ntag+1;
            NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_PLACE_JETTON, &placejetton, sizeof(placejetton));
            
            m_showscore -= jettonscore;
            m_lUserJettonScore[ntag+1] += jettonscore;
            m_lAllJettonScore[ntag+1] += jettonscore;
            this->updateAreaScore(true);
            this->updateAreaSelfScore(true);
            this->showgoldmove(jettonscore, ntag, true);
            this->updateScoreButton(true);
            this->updateselfscore();
        }
            break;
            
        default:
            break;
    }

}

void GameLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/ingameBGMMono.mp3",true);
    NetworkMgr::getInstance()->sendRequestChairUserInfo(HallDataMgr::getInstance()->m_wTableID, INVALID_CHAIR);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Room_LeaveGame, CC_CALLBACK_1(GameLayer::notifyLeaveGame, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(GameLayer::notifyScoreChange, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Horn_Message, CC_CALLBACK_1(GameLayer::notifyHorn, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Sys_Chat, CC_CALLBACK_1(GameLayer::notifySysChat, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_EnterForeground, CC_CALLBACK_1(GameLayer::notifyEnterForeground, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Socket_Disconnect, CC_CALLBACK_1(GameLayer::notifySocketDisconnect, this)), 1);
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(GameLayer::roomloginResult, this));
}

void GameLayer::onExit()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/menuBGMMono.mp3",true);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Room_LeaveGame);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Horn_Message);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Sys_Chat);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_EnterForeground);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Socket_Disconnect);
    HallDataMgr::getInstance()->m_bStartGame = false;
    NetworkMgr::getInstance()->m_GameScenecallback = nullptr;
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GF_GAME);
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    NetworkMgr::getInstance()->Disconnect(Data_Room);
    m_UserList.clear();
    m_hornlist.clear();
    m_bankwaitlist.clear();
    this->clearRecord();
    this->clearGold();
    this->clearCardList();
    CC_SAFE_DELETE(m_gamelogic);
    Layer::onExit();
}

void GameLayer::update(float delta)
{
    auto ptime = static_cast<Label *>(this->getChildByTag(Tag_Txt_Time));
    if (ptime) {
        ptime->setString(getCurrentTime());
    }
}

void GameLayer::hornUpdate(float t)
{
    auto txtnode = static_cast<ui::ScrollView *>(this->getChildByTag(Tag_Txt_Horn));
    if (!txtnode) {
        return;
    }
    txtnode->removeAllChildren();
    int chatnum = (int)HallDataMgr::getInstance()->m_chatlist.size();
    if (m_hornlist.size()==0)
    {
        if (chatnum == 0) {
            this->unschedule(HornShow);
        }
        else
        {
            m_chatindex++;
            m_chatindex %= chatnum;
            auto pstr = HallDataMgr::getInstance()->m_chatlist.at(m_chatindex);
            auto plabel = Text::create(pstr->getCString(), FontBold, 24);
            plabel->setColor(Color3B::RED);
            plabel->setAnchorPoint(Vec2(0.f, 0.5f));
            plabel->setPosition(Vec2(30, 20));
            
            auto phornlayout = Layout::create();
            phornlayout->setContentSize(cocos2d::Size(plabel->getContentSize().width+60, 40));
            phornlayout->addChild(plabel);
            phornlayout->setAnchorPoint(Vec2(0.f, 0.5f));
            phornlayout->setPosition(Vec2(0, 24));
            if (phornlayout->getContentSize().width>560) {
                auto length = phornlayout->getContentSize().width-560;
                float time = length*0.02f;
                auto paction = Sequence::createWithTwoActions(MoveBy::create(time, Vec2(-length, 0)), MoveBy::create(time, Vec2(length, 0)));
                phornlayout->runAction(RepeatForever::create(paction));
            }
            txtnode->addChild(phornlayout);
        }
        return;
    }
    
    auto phorntext = m_hornlist.at(0);
    auto plabel1 = Label::createWithSystemFont(phorntext->sendnickname, FontBold, 24);
    plabel1->setColor(Color3B::WHITE);
    plabel1->setAnchorPoint(Vec2(0.f, 0.5f));
    
    auto plabel2 = Label::createWithSystemFont(phorntext->sendtext, FontBold, 24);
    plabel2->setColor(Color3B::YELLOW);
    plabel2->setAnchorPoint(Vec2(0.f, 0.5f));
    
    float length = 60;
    plabel1->setPosition(Vec2(length-30, 20));
    length += plabel1->getContentSize().width;
    plabel2->setPosition(Vec2(length-30, 20));
    length += plabel2->getContentSize().width;
    
    auto phornlayout = Layout::create();
    phornlayout->setContentSize(cocos2d::Size(length, 40));
    phornlayout->addChild(plabel1);
    phornlayout->addChild(plabel2);
    m_hornlist.erase(0);
    phornlayout->setAnchorPoint(Vec2(0.f, 0.5f));
    phornlayout->setPosition(Vec2(0, 24));
    if (phornlayout->getContentSize().width>560) {
        auto length = phornlayout->getContentSize().width-560;
        float time = length*0.02f;
        auto paction = Sequence::createWithTwoActions(MoveBy::create(time, Vec2(-length, 0)), MoveBy::create(time, Vec2(length, 0)));
        phornlayout->runAction(RepeatForever::create(paction));
    }
    txtnode->addChild(phornlayout);
}

void GameLayer::sendCard()
{
    if (m_cbGameStatus != GAME_SCENE_GAME_END) {
        return;
    }
    this->clearCardList();
    this->sendCard(0);
    m_sendIndex = 0;
    auto pAction = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(0.6f), CallFunc::create([=]{
        m_sendIndex++;
        this->sendCard(m_sendIndex);
    })), 4);
    pAction->setTag(Tag_SendCard_Action);
    this->runAction(pAction);
}

void GameLayer::sendCard(int pos)
{
    if (m_cbGameStatus != GAME_SCENE_GAME_END) {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/CARD_SELECTED_SOUND.wav");
    auto function = [=]{
        auto pcardbg = Sprite::create("game_res/im_card.png", cocos2d::Rect(180, 119*4, 90, 119));
        pcardbg->setOpacity(0);
        pcardbg->setAnchorPoint(Vec2(0.f, 0.5f));
        pcardbg->runAction(FadeTo::create(0.04f, 255));
        pcardbg->setPosition(Vec2(548, WinSize.height-228));
        this->addChild(pcardbg);
        m_cardlist[pos].pushBack(pcardbg);
        pcardbg->runAction(MoveTo::create(0.33f, m_cardVec2[pos]));
    };
    
    auto endfunction = [=]{
        int listsize = (int)m_cardlist[pos].size();
        if (!listsize) {
            return;
        }
        for (int index=0; index<5; ++index) {
            auto pcardbg = m_cardlist[pos].at(index);
            if (pcardbg) {
                pcardbg->runAction(MoveBy::create(0.04*index, Vec2(26*index, 0)));
            }
        }
    };
    auto pAction = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(0.05f), CallFunc::create(function)), 5);
    this->runAction(pAction);
    auto pEndAction = Sequence::createWithTwoActions(DelayTime::create(0.6f), CallFunc::create(endfunction));
    this->runAction(pEndAction);
}

void GameLayer::sendCardNoAnim()
{
    for (int index=0; index<5; ++index) {
        for (int num=0; num<5; ++num) {
            auto pcardbg = Sprite::create("game_res/im_card.png", cocos2d::Rect(180, 119*4, 90, 119));
            pcardbg->setOpacity(0);
            pcardbg->setAnchorPoint(Vec2(0.f, 0.5f));
            pcardbg->runAction(FadeTo::create(0.04f, 255));
            pcardbg->setPosition(Vec2(m_cardVec2[index].x+26*num, m_cardVec2[index].y));
            this->addChild(pcardbg);
            m_cardlist[index].pushBack(pcardbg);
        }
        this->showCard(index);
    }
}

void GameLayer::clearCardList()
{
    for (int index=0; index<5; ++index) {
        for (auto iter:m_cardlist[index]) {
            iter->removeFromParent();
        }
        m_cardlist[index].clear();
    }
}

void GameLayer::roomloginResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_LOGON_SUCCESS:
        {
            
        }
            break;
        case SUB_GR_LOGON_FAILURE:
        {
            CMD_GR_LogonFailure *failure = (CMD_GR_LogonFailure *)pData;
            auto str = WHConverUnicodeToUtf8WithArray(failure->szDescribeString);
            auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure));
            player->setEnsureCallback([=]{
                this->notifyLeaveGame(nullptr);
            });
            NetworkMgr::getInstance()->Disconnect(Data_Room);
        }
            break;
        case SUB_GR_LOGON_FINISH:
        {
            auto status = HallDataMgr::getInstance()->m_cbStatus;
            //跳到桌子界面
            if (status==US_FREE || status==US_NULL) {
                NetworkMgr::getInstance()->sendSitDown(0, 100, HallDataMgr::getInstance()->m_pPassword);
                NetworkMgr::getInstance()->sendGameOption();
            }
            //游戏状态启动游戏
            else if(status==US_PLAYING || status==US_READY || status==US_OFFLINE)
            {
                NetworkMgr::getInstance()->sendGameOption();
            }
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
            this->clearData();
            this->clearCardList();
            this->updateAreaScore(false);
            this->updateAreaSelfScore(false);
            this->showniudot(false);
            this->clearGold();
        }
            break;
        default:
            break;
    }

}

void GameLayer::onGameScene(void *pData, WORD wSize)
{
    m_scoreIndex = -1;
    if (HallDataMgr::getInstance()->m_cbGameStatus == GAME_SCENE_FREE) {
        this->onGameSceneFree(pData, wSize);
    }
    else if (HallDataMgr::getInstance()->m_cbGameStatus==GAME_SCENE_PLACE_JETTON || HallDataMgr::getInstance()->m_cbGameStatus==GAME_SCENE_GAME_END)
    {
        this->onGameScenePlaying(pData, wSize);
    }
    this->showBanker();
}

void GameLayer::onGameSceneFree(void *pData, WORD wSize)
{
    auto pScene = static_cast<CMD_S_StatusFree *>(pData);
    this->clearData();
    m_lApplyBankerCondition = pScene->lApplyBankerCondition;
    m_lAreaLimitScore = pScene->lAreaLimitScore;
    m_cbTimeLeave = pScene->cbTimeLeave;
    m_wBankerUser = pScene->wBankerUser;
    m_bEnableSysBanker = pScene->bEnableSysBanker;
    m_lUserMaxScore = pScene->lUserMaxScore/MaxTimes;
    m_lAreaLimitScore = pScene->lAreaLimitScore;
    this->showBanker();
    this->showCountDown(true);
    this->updateScoreButton(true);
    if (m_wBankerUser == HallDataMgr::getInstance()->m_wChairID) {
        m_ApplyBankerStatus = 2;
        this->updateApplyBankerButton();
    }
}

void GameLayer::onGameScenePlaying(void *pData, WORD wSize)
{
    auto pScene = static_cast<CMD_S_StatusPlay *>(pData);
    this->clearData();
    m_cbGameStatus = pScene->cbGameStatus;
    m_lApplyBankerCondition = pScene->lApplyBankerCondition;
    m_lAreaLimitScore = pScene->lAreaLimitScore;
    m_cbTimeLeave = pScene->cbTimeLeave;
    m_wBankerUser = pScene->wBankerUser;
    m_bEnableSysBanker = pScene->bEnableSysBanker;
    m_lUserScore = pScene->lEndUserScore;
    m_lBankerScore = pScene->lEndBankerScore;
    m_lUserMaxScore = pScene->lUserMaxScore/MaxTimes;
    m_lAreaLimitScore = pScene->lAreaLimitScore;
    memcpy(m_lAllJettonScore, pScene->lAllJettonScore, sizeof(LONGLONG)*5);
    memcpy(m_lUserJettonScore, pScene->lUserJettonScore, sizeof(LONGLONG)*5);
    this->showBanker();
    if (m_wBankerUser == HallDataMgr::getInstance()->m_wChairID) {
        m_ApplyBankerStatus = 2;
        this->updateApplyBankerButton();
    }
    for (int index=0; index<5; ++index)
    {
        //设置手牌
        memcpy(m_cbTableCardArray[index],pScene->cbTableCardArray[index],sizeof(BYTE)*5);
    }
    if (m_cbGameStatus == GAME_SCENE_PLACE_JETTON) {
        this->showCountDown(true);
        this->updateAreaScore(true);
        this->updateAreaSelfScore(true);
        this->addgoldtoArea();
    }
    else
    {
        LONGLONG score = 0;
        for (int index=1; index<5; ++index)
        {
            score += m_lUserJettonScore[index];
        }
        if (score) {
            this->updateAreaScore(true);
            this->updateAreaSelfScore(true);
            this->sendCardNoAnim();
        }
        else
        {
            m_cbGameStatus = GAME_SCENE_FREE;
            m_cbTimeLeave += 5;
            this->showCountDown(true);
            this->updateScoreButton(true);
        }
    }
}

void GameLayer::onGameMessage(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_S_GAME_FREE:
        {
            this->onGameMessageFree(pData, wSize);
        }
            break;
        case SUB_S_GAME_START:
        {
            this->onGameMessageStart(pData, wSize);
        }
            break;
        case SUB_S_GAME_END:
        {
            this->onGameMessageEnd(pData, wSize);
        }
            break;
        case SUB_S_PLACE_JETTON:
        {
            this->onGameMessagePlaceJetton(pData, wSize);
        }
            break;
        case SUB_S_APPLY_BANKER:
        {
            this->onGameMessageApplyBanker(pData, wSize);
        }
            break;
        case SUB_S_CHANGE_BANKER:
        {
            this->onGameMessageChangeBanker(pData, wSize);
        }
            break;
        case SUB_S_CANCEL_BANKER:
        {
            this->onGameMessageCancelBanker(pData, wSize);
        }
            break;
            
        case SUB_S_CHANGE_USER_SCORE:
        {
            this->onGameMessageChangeScore(pData, wSize);
        }
            break;
        case SUB_S_SEND_RECORD:
        {
            this->onGameMessageRecord(pData, wSize);
        }
            break;
        case SUB_S_PLACE_JETTON_FAIL:
        {
            this->onGameMessagePlaceJettonFailure(pData, wSize);
        }
            break;
        default:
            break;
    }
}

void GameLayer::onGameMessageFree(void *pData, WORD wSize)
{
    //int size = sizeof(CMD_S_GameFree);
    auto pfree = static_cast<CMD_S_GameFree *>(pData);
    m_cbTimeLeave = pfree->cbTimeLeave;
    m_cbGameStatus = GAME_SCENE_FREE;
    this->showCountDown(true);
    this->updateScoreButton(true);
    this->clearData();
    this->clearCardList();
    this->updateAreaScore(false);
    this->updateAreaSelfScore(false);
    this->showniudot(false);
    this->clearGold();
    this->removeChildByTag(Tag_End_Layer);
    //银行可以存款
    auto pbank = static_cast<BankLayer *>(this->getChildByTag(Tag_Bank_Layer));
    if (pbank) {
        pbank->updategamesave(true);
    }
}

//游戏开始，开始下注
void GameLayer::onGameMessageStart(void *pData, WORD wSize)
{
    //int size = sizeof(CMD_S_GameStart);
    auto pstart = static_cast<CMD_S_GameStart *>(pData);
    m_cbTimeLeave = pstart->cbTimeLeave;
    m_wBankerUser = pstart->wBankerUser;
    m_lUserMaxScore = pstart->lUserMaxScore/MaxTimes;
    auto puser = this->getUserByChairID(m_wBankerUser);
    if (puser) {
        puser->m_date.lScore = pstart->lBankerScore;
        this->showBanker();
    }
    m_cbGameStatus = GAME_SCENE_PLACE_JETTON;
    this->showCountDown(true);
    this->updateScoreButton(true);
    
    //银行不可以存款
    auto pbank = static_cast<BankLayer *>(this->getChildByTag(Tag_Bank_Layer));
    if (pbank) {
        pbank->updategamesave(false);
    }
}

//游戏结束，开始发牌，亮牌
void GameLayer::onGameMessageEnd(void *pData, WORD wSize)
{
    //int size = sizeof(CMD_S_GameEnd);
    auto pend = static_cast<CMD_S_GameEnd *>(pData);
    for (int index=0; index<5; ++index) {
        memcpy(m_cbTableCardArray[index], pend->cbTableCardArray[index], sizeof(BYTE)*5);
    }
    m_lUserScore = pend->lUserScore;
    m_lBankerScore = pend->lBankerScore;
    m_cbGameStatus = GAME_SCENE_GAME_END;
    this->showCountDown(false);
    this->updateScoreButton(false);
    this->sendCard();
    //显示牌点数
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(4.f), CallFunc::create([=]{
        if (m_cbGameStatus != GAME_SCENE_GAME_END) {
            return;
        }
        this->showniudot(true);
    })));
    //金币移到庄家处
    this->runAction(Sequence::create(DelayTime::create(10.f), CallFunc::create([=]{
        if (m_cbGameStatus != GAME_SCENE_GAME_END) {
            return;
        }
        int winnum = 0;
        LONGLONG winscore = 0;
        for (int index=0; index<AREA_COUNT; ++index) {
            if (!m_bUserOxCard[index+1]) {
                winnum++;
                winscore += m_lAllJettonScore[index+1];
                this->showgoldmoveToZ(index+1);
            }
        }
        //金币从庄家移到下注区域
        if (!winnum || winscore==0) {
            this->showgoldmoveToArea();
        }
        else
        {
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.6f), CallFunc::create([=]{this->showgoldmoveToArea();})));
        }
    }),nullptr));
}

//用户下注
void GameLayer::onGameMessagePlaceJetton(void *pData, WORD wSize)
{
    auto pjetton = static_cast<CMD_S_PlaceJetton *>(pData);
    if (pjetton->wChairID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    m_lAllJettonScore[pjetton->cbJettonArea] += pjetton->lJettonScore;
    this->updateAreaScore(true);
    this->showgoldmove(pjetton->lJettonScore, pjetton->cbJettonArea-1);
    
    if (m_wBankerUser != INVALID_CHAIR) {
        LONGLONG allscore = m_lAllJettonScore[1] + m_lAllJettonScore[2] + m_lAllJettonScore[3] + m_lAllJettonScore[4];
        auto pbanker = this->getUserByChairID(m_wBankerUser);
        if (pbanker) {
            LONGLONG maxscore = pbanker->m_date.lScore/MaxTimes;
            if (maxscore < (allscore)) {
                //HallDataMgr::getInstance()->AddpopLayer("", "总下注已超过庄家下注上限", Type_Info_Reminder);
                this->updateScoreButton(false);
            }
        }
    }
}

//申请上庄
void GameLayer::onGameMessageApplyBanker(void *pData, WORD wSize)
{
    auto papply = static_cast<CMD_S_ApplyBanker *>(pData);
    m_bankwaitlist.insert(std::make_pair(papply->wApplyUser, papply->wApplyUser));
}

//切换庄家
void GameLayer::onGameMessageChangeBanker(void *pData, WORD wSize)
{
    auto pchange = static_cast<CMD_S_ChangeBanker *>(pData);
    m_bankwaitlist.erase(pchange->wBankerUser);
    if (m_wBankerUser == HallDataMgr::getInstance()->m_wChairID) {
        m_ApplyBankerStatus = 0;
        this->updateApplyBankerButton();
    }
    m_wBankerUser = pchange->wBankerUser;
    auto puser = this->getUserByChairID(m_wBankerUser);
    if (puser) {
        puser->m_date.lScore = pchange->lBankerScore;
    }
    this->showBanker();
    if (m_wBankerUser == HallDataMgr::getInstance()->m_wChairID) {
        m_ApplyBankerStatus = 2;
        this->updateApplyBankerButton();
    }
}

//取消申请上庄
void GameLayer::onGameMessageCancelBanker(void *pData, WORD wSize)
{
    auto pcancle = static_cast<CMD_S_CancelBanker *>(pData);
    m_bankwaitlist.erase(pcancle->wCancelUser);
}

//更新积分
void GameLayer::onGameMessageChangeScore(void *pData, WORD wSize)
{
//    auto pchangescore = static_cast<CMD_S_ChangeUserScore *>(pData);
//    auto puser = this->getUserByChairID(pchangescore->wChairID);
//    if (puser) {
//        puser->m_date.lScore = pchangescore->lScore;
//        
//    }
}

//游戏记录
void GameLayer::onGameMessageRecord(void *pData, WORD wSize)
{
    int num = wSize/sizeof(tagServerGameRecord);
    if(num < 10)
        return;
    for (int index=num-10; index<num; ++index) {
        auto precord = new tagServerGameRecord;
        char *pbegin = (char *)pData + index*4;
        memcpy(precord, pbegin, 4);
        m_recordlist.push_back(precord);
    }
}

//下注失败
void GameLayer::onGameMessagePlaceJettonFailure(void *pData, WORD wSize)
{
    auto pfailure = static_cast<CMD_S_PlaceJettonFail *>(pData);
    if (pfailure->wPlaceUser == HallDataMgr::getInstance()->m_wChairID) {
        m_lUserJettonScore[pfailure->lJettonArea] -= pfailure->lPlaceScore;
        m_lAllJettonScore[pfailure->lJettonArea] -= pfailure->lPlaceScore;
        this->updateAreaSelfScore(true);
        this->updateAreaScore(true);
        m_showscore += pfailure->lPlaceScore;
        this->updateselfscore();
    }
}

void GameLayer::OnUserEnter(void *pUser)
{
    auto puser = static_cast<UserData *>(pUser);
    m_UserList.insert(puser->m_date.dwUserID, puser);
    if (m_wBankerUser != INVALID_CHAIR) {
        if (puser->m_date.wChairID == m_wBankerUser) {
            this->showBanker();
        }
    }
//    if (puser->m_date.wChairID == m_wBankerUser) {
//        this->showBanker();
//    }
}

void GameLayer::OnUserStatus(void *pUser)
{
    auto puser = static_cast<UserData *>(pUser);
    m_UserList.insert(puser->m_date.dwUserID, puser);
    if (puser->m_date.cbUserStatus == US_FREE) {
        m_UserList.erase(puser->m_date.dwUserID);
    }
}


void GameLayer::OnUserScore(void *pUser)
{
    auto puser = static_cast<UserData *>(pUser);
    m_UserList.insert(puser->m_date.dwUserID, puser);
    if (puser->m_date.dwUserID == HallDataMgr::getInstance()->m_dwUserID) {
        m_showscore = puser->m_date.lScore;
        this->updateselfscore();
    }
}

void GameLayer::clearData()
{
    memset(m_lAllJettonScore, 0, sizeof(LONGLONG)*(AREA_COUNT+1));
    memset(m_lUserJettonScore, 0, sizeof(LONGLONG)*(AREA_COUNT+1));
    memset(m_bUserOxCard, 0, sizeof(bool)*5);
    for (int index=0; index<(AREA_COUNT+1); ++index) {
        memset(m_cbTableCardArray[index], 0, sizeof(BYTE)*(AREA_COUNT+1));
    }
    m_cbGameStatus = GAME_SCENE_FREE;
    m_lUserScore = 0;
    m_lBankerScore = 0;
}

void GameLayer::clearGold()
{
    for (int index=0; index<AREA_COUNT+1; ++index) {
        for (auto iter:m_goldlist[index]) {
            iter->removeFromParent();
        }
        m_goldlist[index].clear();
    }
}

void GameLayer::produceRecordList()
{
    //如果没有记录，随机产生记录
    if (!m_recordlist.size()) {
        for (int index=0; index<10; ++index)
        {
            tagServerGameRecord *precord = new tagServerGameRecord;
            precord->bWinShunMen = std::rand()%2;
            precord->bWinDuiMen = std::rand()%2;
            precord->bWinDaoMen = std::rand()%2;
            precord->bWinHuang = std::rand()%2;
            m_recordlist.push_back(precord);
        }
    }
}

void GameLayer::clearRecord()
{
    for(std::vector<tagServerGameRecord *>::iterator iter=m_recordlist.begin(); iter != m_recordlist.end(); iter++)
    {
        delete *iter;
    }
    m_recordlist.clear();
}

void GameLayer::notifyLeaveGame(cocos2d::EventCustom *event)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, RoomLayer::createScene()));
}

void GameLayer::notifyScoreChange(cocos2d::EventCustom *event)
{
    auto puser = m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser) {
        puser->m_date.lScore = HallDataMgr::getInstance()->m_UserScore;
        m_showscore = puser->m_date.lScore;
        this->updateselfscore();
    }
}

void GameLayer::notifySysChat(cocos2d::EventCustom *event)
{
    if (!this->isScheduled(HornShow)) {
        this->hornUpdate(0);
        this->schedule(CC_CALLBACK_1(GameLayer::hornUpdate, this), 4.f, HornShow);
    }
}

void GameLayer::notifyHorn(cocos2d::EventCustom *event)
{
    auto phorn = static_cast<TrumpetData *>(event->getUserData());
    m_hornlist.pushBack(phorn);
    if (!this->isScheduled(HornShow)) {
        this->hornUpdate(0);
        this->schedule(CC_CALLBACK_1(GameLayer::hornUpdate, this), 4.f, HornShow);
    }
}

void GameLayer::notifyEnterForeground(cocos2d::EventCustom *event)
{
    m_cbGameStatus = GAME_SCENE_FREE;
    this->clearData();
    this->clearCardList();
    this->updateAreaScore(false);
    this->updateAreaSelfScore(false);
    this->showniudot(false);
    this->clearGold();
    this->removeChildByTag(Tag_End_Layer);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (!NetworkMgr::getInstance()->getSocketData()) {
        this->gameConect();
    }
#endif
}

void GameLayer::notifySocketDisconnect(cocos2d::EventCustom *event)
{
    NetworkMgr::getInstance()->Disconnect(Data_Room);
    this->gameConect();
}


void GameLayer::gameConect()
{
    HallDataMgr::getInstance()->AddpopLayer("", "正在重新连接游戏", Type_Wait_Text);
    NetworkMgr::getInstance()->doConnect(HallDataMgr::getInstance()->m_roomaddress.c_str(), HallDataMgr::getInstance()->m_roomPort, Data_Room);
    NetworkMgr::getInstance()->sendRoomLogin(HallDataMgr::getInstance()->m_roomPass);
    
}

int GameLayer::getgoldnumbyScore(LONGLONG score)
{
    if (score < 100) {
        return 0;
    }
    std::string scorestr = __String::createWithFormat("%lld",score)->getCString();
    int length = (int)scorestr.length();
    scorestr = scorestr.substr(0,length-2);
    length = (int)scorestr.length();
    if (length>5) {
        scorestr = scorestr.substr(length-5,5);
    }
    length = (int)scorestr.length();
    int goldnum = 0;
    for (int index=length; index!=0; --index) {
        std::string bytestr = scorestr.substr(length-index,1);
        int value = __String::create(bytestr)->intValue();
        int num = goldbasenum[index-1]*value;
        num = MIN(num, goldmaxnum[index-1]);
        goldnum += num;
    }
    return goldnum;
}

UserData * GameLayer::getUserByChairID(WORD chairID)
{
    for (auto iter:m_UserList) {
        if (iter.second->m_date.wChairID == chairID) {
            return  iter.second;
        }
    }
    return nullptr;
}

void GameLayer::loadlightAnim()
{
    cocos2d::Vector<SpriteFrame *> animarray;
    for (int index=0; index<11; ++index)
    {
            SpriteFrame *frame = SpriteFrame::create(__String::createWithFormat("game_res/anim_light_%d.png",index)->getCString(), cocos2d::Rect(0, 0, 150, 100));
            animarray.pushBack(frame);
    }
    auto fishanim = Animation::createWithSpriteFrames(animarray, 0.23f);
    AnimationCache::getInstance()->addAnimation(fishanim, ButtonLigntAnim);
}

void GameLayer::showBanker()
{
    this->removeChildByTag(Tag_Banker_Head);
    
    cocos2d::Size viewsize = WinSize;
    std::string scorestr;
    std::string namestr;
    if (m_wBankerUser == INVALID_CHAIR) {
        auto phead = ImageView::create("game_res/im_sys_head.png");
        phead->setTag(Tag_Banker_Head);
        phead->setPosition(Vec2(viewsize.width/2-286, viewsize.height-56));
        this->addChild(phead);
        scorestr = getScoreString(8888800);
        namestr = "小怪兽系统";
    }
    else
    {
        auto puser = this->getUserByChairID(m_wBankerUser);
        if (!puser) {
            return;
        }
        auto headimage = HeaderRequest::createwithFaceID(puser->m_date.wFaceID,
                                                         puser->m_date.dwCustomID,
                                                         puser->m_date.dwUserID,
                                                         puser->m_date.cbGender);
        headimage->setHeadSize(100);
        
        auto clipnode = ClippingNode::create();
        clipnode->addChild(headimage);
        auto circleNode = DrawNode::create();
        const int maxTrangle = 360;
        Vec2 circleVec2[maxTrangle];
        for (int i = 0; i < maxTrangle; i ++)
        {
            float x = cosf( i * (M_PI/180.f)) * 50;
            float y = sinf( i * (M_PI/180.f)) * 50;
            circleVec2[i] = Vec2(x, y);
        }
        auto circleColor = Color4F(0, 1, 0, 1);
        circleNode->drawPolygon(circleVec2, maxTrangle, circleColor, 1, circleColor);
        clipnode->setStencil(circleNode);
        clipnode->setTag(Tag_Banker_Head);
        clipnode->setPosition(Vec2(viewsize.width/2-286, viewsize.height-56));
        this->addChild(clipnode);
        scorestr = getScoreString(puser->m_date.lScore);
        namestr = puser->m_nickname;
    }
    
    if (!this->getChildByTag(Tag_Banker_Txt)) {
        auto pzhuang = ImageView::create("game_res/im_zhuang.png");
        pzhuang->setPosition(Vec2(viewsize.width/2-256, viewsize.height-34));
        pzhuang->setTag(Tag_Banker_Txt);
        this->addChild(pzhuang,1);
    }
    auto pname = static_cast<NameLabel *>(this->getChildByTag(Tag_Banker_Name));
    if (pname) {
        pname->setstring(namestr);
    }
    else
    {
        auto pName = NameLabel::create(namestr, FontNormal, 20);
        pName->setFontColor(Color3B(50, 17, 0));
        pName->setSize(cocos2d::Size(130, 25));
        pName->setAnchorPoint(Vec2(0.5f, 0.5f));
        pName->setPosition(Vec2(viewsize.width/2-166, viewsize.height-17));
        pName->setTag(Tag_Banker_Name);
        this->addChild(pName);
    }
    
    if (!this->getChildByTag(Tag_Banker_Gold)) {
        auto pgold = ImageView::create("game_res/im_small_gold.png");
        pgold->setTag(Tag_Banker_Gold);
        pgold->setPosition(Vec2(viewsize.width/2-216, viewsize.height-48));
        this->addChild(pgold);
    }
    auto pscore = static_cast<Text *>(this->getChildByTag(Tag_Banker_Score));
    if (pscore) {
        pscore->setString(scorestr);
    }
    else
    {
        pscore = Text::create(scorestr, FontNormal, 25);
        pscore->setColor(ScoreColor);
        pscore->setPosition(Vec2(viewsize.width/2-196, viewsize.height-48));
        pscore->setTag(Tag_Banker_Score);
        pscore->setAnchorPoint(Vec2(0.f, 0.5f));
        this->addChild(pscore);
    }
}

void GameLayer::updateselfscore()
{
    auto pscore = static_cast<Label *>(this->getChildByTag(Tag_Self_Score));
    auto puser = m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pscore&&puser) {
        pscore->setString(getScoreString(m_showscore));
    }
}

void GameLayer::updateScoreButton(bool enable)
{
    if (m_wBankerUser == HallDataMgr::getInstance()->m_wChairID) {
        enable = false;//庄家不可以下注
    }
    if (!enable) {
        for (int index=0; index<5; ++index) {
            auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score+index));
            if (pbutton) {
                pbutton->setEnabled(false);
                auto normalstr = __String::createWithFormat("game_res/bt_score_%d_0.png", index)->getCString();
                auto selectstr = __String::createWithFormat("game_res/bt_score_%d_1.png", index)->getCString();
                auto disstr = __String::createWithFormat("game_res/bt_score_%d_3.png", index)->getCString();
                pbutton->loadTextures(normalstr, selectstr,disstr);
                pbutton->removeAllChildren();
            }
        }
        return;
    }
    auto puser = m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser) {
        //最大下注数
        auto pmaxscore = puser->m_date.lScore/MaxTimes;
        if (m_lUserMaxScore) {
            pmaxscore = MIN(pmaxscore, m_lUserMaxScore);
        }
        auto pjettonscore = m_lUserJettonScore[1] + m_lUserJettonScore[2] + m_lUserJettonScore[3] + m_lUserJettonScore[4];
        pmaxscore = pmaxscore - pjettonscore;
        int maxindex = -1;//记录最大加注按钮
        for (int index=0; index<5; ++index)
        {
            LONGLONG paddscore = 0;
            for (int num=0; num<=index; ++num) {
                paddscore += scorelist[num];
            }
            auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score+index));
            if (pbutton)
            {
                bool b = (paddscore<=pmaxscore);
                pbutton->setBright(b);
                pbutton->setEnabled(b);
                maxindex = b?index:maxindex;
            }
        }
        Button *pselectbutton = nullptr;
        if (m_scoreIndex == -1) {
            m_scoreIndex = maxindex;
            auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score + m_scoreIndex));
            if (pbutton) {
                pselectbutton = pbutton;
            }
        }
        else if (maxindex == -1)
        {
            auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score + m_scoreIndex));
            if (pbutton) {
                pbutton->loadTextureNormal(__String::createWithFormat("game_res/bt_score_%d_0.png", m_scoreIndex)->getCString());
                pbutton->removeAllChildren();
                pbutton->setEnabled(false);
                pbutton->setBright(false);
            }
            m_scoreIndex = maxindex;
        }
        else if(m_scoreIndex>maxindex)
        {
            auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score + m_scoreIndex));
            if (pbutton) {
                pbutton->loadTextureNormal(__String::createWithFormat("game_res/bt_score_%d_0.png", m_scoreIndex)->getCString());
                pbutton->removeAllChildren();
                pbutton->setEnabled(false);
                pbutton->setBright(false);
            }
            auto pbutton1 = static_cast<Button *>(this->getChildByTag(Tag_BT_Score + maxindex));
            if (pbutton1) {
                pselectbutton = pbutton1;
            }
            m_scoreIndex = maxindex;
        }
        else
        {
            pselectbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score+m_scoreIndex));
        }
        //选中按钮动画添加
        if (pselectbutton) {
            pselectbutton->loadTextureNormal(__String::createWithFormat("game_res/bt_score_%d_2.png", m_scoreIndex)->getCString());
            auto pimage = Sprite::create("game_res/anim_light_0.png");
            pimage->setPosition(Vec2(pselectbutton->getContentSize().width/2, pselectbutton->getContentSize().height/2));
            pselectbutton->addChild(pimage);
            auto lightanim = Animate::create(AnimationCache::getInstance()->getAnimation(ButtonLigntAnim));
            auto paction = RepeatForever::create(lightanim);
            pimage->runAction(paction);
        }
    }
}

void GameLayer::updateScoreButton(SCORE socre)
{
    LONGLONG paddscore = 0;
    for (int index=0; index<5; ++index) {
        paddscore += scorelist[index];
        auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_Score+index));
        if (pbutton) {
            if (paddscore > socre) {
                pbutton->setEnabled(false);
                pbutton->setBright(false);
            }
        }
    }
}

void GameLayer::updateApplyBankerButton()
{
    auto pbutton = static_cast<Button *>(this->getChildByTag(Tag_BT_SZ));
    if (pbutton) {
        switch (m_ApplyBankerStatus) {
            case 0:
                pbutton->loadTextures("game_res/bt_sz_0_0.png", "game_res/bt_sz_0_1.png","game_res/bt_sz_0_0.png");
                break;
            case 1:
                pbutton->loadTextures("game_res/bt_sz_1_0.png", "game_res/bt_sz_1_1.png","game_res/bt_sz_1_0.png");
                break;
            case 2:
                pbutton->loadTextures("game_res/bt_sz_2_0.png", "game_res/bt_sz_2_1.png","game_res/bt_sz_2_0.png");
                break;
            default:
                break;
        }
    }
}

void GameLayer::showCountDown(bool show)
{
    this->removeChildByTag(Tag_CountDown_Num);
    this->removeChildByTag(Tag_CountDown_Txt);
    if (!show) {
        this->stopActionByTag(Tag_CountDown_Action);
        return;
    }
    std::string txtstr = m_cbGameStatus==GAME_SCENE_FREE?"休息一下...":"请下注";
    auto ptxt = Text::create(txtstr, FontBold, 28);
    ptxt->setAnchorPoint(Vec2(1.f, 0.5f));
    ptxt->setPosition(Vec2(WinSize.width/2+20, 110));
    ptxt->setTag(Tag_CountDown_Txt);
    this->addChild(ptxt);
    
    std::string numstr = __String::createWithFormat("%d", m_cbTimeLeave)->getCString();
    auto pnum = Text::create(numstr, FontBold, 28);
    pnum->setPosition(Vec2(ptxt->getPositionX()+30, 110));
    pnum->setTag(Tag_CountDown_Num);
    this->addChild(pnum);
    
    int repeattime = m_cbTimeLeave;
    auto paction = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]{
        auto pnum = static_cast<Text *>(this->getChildByTag(Tag_CountDown_Num));
        if (pnum) {
            if (m_cbTimeLeave>0) {
                m_cbTimeLeave -= 1;
            }
            pnum->setString(__String::createWithFormat("%d",m_cbTimeLeave)->getCString());
        }
    })), repeattime);
    pnum->runAction(paction);
}

void GameLayer::updateAreaScore(bool show)
{
    if (!show) {
        for (int index=0; index<AREA_COUNT; ++index) {
            this->removeChildByTag(Tag_Area_Score+index);
        }
        return;
    }
    for (int index=0; index<AREA_COUNT; ++index) {
        if (m_lAllJettonScore[index+1] == 0) {
            continue;
        }
        auto ptxt = static_cast<Text *>(this->getChildByTag(Tag_Area_Score+index));
        if (ptxt) {
            ptxt->setString(getScoreString(m_lAllJettonScore[index+1]));
        }
        else
        {
            ptxt = Text::create(getScoreString(m_lAllJettonScore[index+1]), FontBold, 20);
            ptxt->setColor(ScoreColor);
            ptxt->setPosition(Vec2(WinSize.width/2-370+244*index, WinSize.height-194));
            ptxt->setTag(Tag_Area_Score+index);
            this->addChild(ptxt);
        }
    }
}

void GameLayer::updateAreaSelfScore(bool show)
{
    if (!show) {
        for (int index=0; index<AREA_COUNT; ++index) {
            this->removeChildByTag(Tag_Area_Self_Score+index);
        }
        return;
    }
    for (int index=0; index<AREA_COUNT; ++index) {
        auto pbg = static_cast<ImageView *>(this->getChildByTag(Tag_Area_Self_Score+index));
        if (m_lUserJettonScore[index+1]==0 && !pbg) {
            continue;
        }
        if (m_lUserJettonScore[index+1]<=0 && pbg) {
            pbg->removeFromParent();
            continue;
        }
        if (pbg) {
            auto ptxt = static_cast<Text *>(pbg->getChildByTag(10));
            ptxt->setString(getScoreString(m_lUserJettonScore[index+1]));
        }
        else
        {
            auto pbg = ImageView::create("game_res/im_self_score_bg.png");
            pbg->setTag(Tag_Area_Self_Score+index);
            pbg->setPosition(Vec2(WinSize.width/2-370+244*index, WinSize.height-362));
            this->addChild(pbg);
            
            auto ptxt = Text::create(getScoreString(m_lUserJettonScore[index+1]), FontBold, 20);
            ptxt->setColor(ScoreColor);
            ptxt->setPosition(pbg->getContentSize()/2);
            ptxt->setTag(10);
            pbg->addChild(ptxt);
        }
    }
}

void GameLayer::showniudot(bool show)
{
    if (!show) {
        for (int index=0; index<(AREA_COUNT+1); ++index) {
            this->removeChildByTag(Tag_Area_Niu_Dot+index);
        }
        return;
    }
    this->showCard(0);
    m_sendIndex = 0;
    auto paction = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(1.2f), CallFunc::create([=]{
        m_sendIndex++;
        this->showCard(m_sendIndex);
    })), AREA_COUNT);
    this->runAction(paction);
}

void GameLayer::showCard(int pos)
{
    BYTE sortlist[5];
    memset(sortlist, 0, 5);
    int cardtype = m_gamelogic->GetCardType(m_cbTableCardArray[pos], 5, sortlist);
    memcpy(m_cbTableCardArray[pos], sortlist, 5);
    if (cardtype == BRNN_CT_ERROR || m_cbGameStatus != GAME_SCENE_GAME_END) {
        return;
    }
//    if (cardtype==CT_SPECIAL_NIUYING || cardtype==BRNN_CT_SPECIAL_NIUKING) {
//        cardtype -= 2;
//    }
    
    int effecttype = cardtype-1;
    effecttype = effecttype>10?10:effecttype;
    std::string effectstr = __String::createWithFormat("sound_res/OX_%d_BANKER_SOUND.wav", effecttype)->getCString();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectstr.c_str());
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/open_card.wav");
    bool win = false;
    BYTE multiple = 0;
    if (pos) {
        int value = m_gamelogic->CompareCard(m_cbTableCardArray[0], 5, m_cbTableCardArray[pos], 5, multiple);
        win = value==1?true:false;
    }
    log("游戏倍数_%d", multiple);
    m_bUserOxCard[pos] = win;
    multiple = multiple==0?1:multiple;
    m_Multiple[pos-1] = multiple;
    for (int index=0; index<5; ++index) {
        int listsize = (int)m_cardlist[pos].size();
        if (!listsize) {
            return;
        }
        auto pcard = m_cardlist[pos].at(index);
        int value = m_cbTableCardArray[pos][index];
        int xindex = value%16-1;
        int yindex = value/16;
        pcard->setTextureRect(cocos2d::Rect(xindex*90, yindex*119, 90, 119));
    }
    log("yonghubeishu %d__%d", pos, multiple);
    
    auto pniubg = ImageView::create("game_res/im_frame.png");
    pniubg->setPosition(Vec2(m_cardVec2[pos].x, m_cardVec2[pos].y+6));
    pniubg->setAnchorPoint(Vec2(0.f, 1.f));
    pniubg->setTag(Tag_Area_Niu_Dot+pos);
    this->addChild(pniubg);
    
    Layout *pniudot = nullptr;
    if (!pos) {
        pniudot = this->getspriteByType("game_res/txt_niu_1.png", cardtype);
        pniudot->setPosition(Vec2(pniubg->getContentSize().width/2, 36));
    }
    else
    {
        std::string imagestr = win?"game_res/txt_niu_0.png":"game_res/txt_niu_2.png";
        pniudot = this->getspriteByType(imagestr, cardtype);
        pniudot->setPosition(Vec2(pniubg->getContentSize().width/2, 48));
    }
    pniubg->addChild(pniudot);
    
    if (pos) {
        if (m_lUserJettonScore[pos] == 0) {
            auto ptxt = Text::create("没有下注", FontBold, 20);
            ptxt->setPosition(Vec2(pniubg->getContentSize().width/2, 12));
            pniubg->addChild(ptxt);
        }
        else
        {
            std::string scorestr = getScoreString(multiple*m_lUserJettonScore[pos]);
            scorestr.insert(0, win?"+":"-");
            auto ptxt = Text::create(scorestr, FontBold, 20);
            ptxt->setColor(win?Color3B::YELLOW:Color3B::WHITE);
            ptxt->setPosition(Vec2(pniubg->getContentSize().width/2, 12));
            pniubg->addChild(ptxt);
        }
    }
}

cocos2d::ui::Layout *GameLayer::getspriteByType(const std::string &imagefile, int type)
{
    auto playout = Layout::create();
    playout->setAnchorPoint(Vec2(0.5f, 0.5f));
    if (type==BRNN_CT_SPECIAL_NIUNIUXW||type==BRNN_CT_SPECIAL_NIUNIUDW) {
        playout->setContentSize(cocos2d::Size(47*3, 42));
    }
    else
    {
        playout->setContentSize(cocos2d::Size(47*2, 42));
    }
    auto pniu = Sprite::create(imagefile, cocos2d::Rect(470, 0, 47, 42));
    pniu->setAnchorPoint(Vec2(0.f, 0.5f));
    
    if (type<=BRNN_CT_SPECIAL_NIUNIU) {
        auto pstr = Sprite::create(imagefile, cocos2d::Rect((type-1)*47, 0, 47, 42));
        pstr->setAnchorPoint(Vec2(0.f, 0.5f));
        if (type==BRNN_CT_POINT) {
            pstr->setPosition(Vec2(0, 21));
            playout->addChild(pstr);
            pniu->setPosition(Vec2(47, 21));
            playout->addChild(pniu);
            return playout;
        }
        pstr->setPosition(Vec2(47, 21));
        playout->addChild(pstr);
        pniu->setPosition(Vec2(0, 21));
        playout->addChild(pniu);
        return playout;
    }
    
    if (type>=BRNN_CT_SPECIAL_NIUYING&&type<=BRNN_CT_SPECIAL_NIUKING) {
        auto pstr = Sprite::create(imagefile, cocos2d::Rect((type-3)*47, 0, 47, 42));
        pstr->setAnchorPoint(Vec2(0.f, 0.5f));
        pstr->setPosition(Vec2(0, 21));
        playout->addChild(pstr);
        pniu->setPosition(Vec2(47, 21));
        playout->addChild(pniu);
        return playout;
    }
    
    if (type>=BRNN_CT_SPECIAL_NIUNIUXW&&type<=BRNN_CT_SPECIAL_NIUNIUDW) {
        int index = type==BRNN_CT_SPECIAL_NIUNIUDW?611:658;
        auto pstr = Sprite::create(imagefile, cocos2d::Rect(index, 0, 47, 42));
        pstr->setAnchorPoint(Vec2(0.f, 0.5f));
        auto pwan = Sprite::create(imagefile, cocos2d::Rect(705, 0, 47, 42));
        pwan->setAnchorPoint(Vec2(0.f, 0.5f));
        pstr->setPosition(Vec2(0, 21));
        playout->addChild(pstr);
        pwan->setPosition(Vec2(47, 21));
        playout->addChild(pwan);
        pniu->setPosition(Vec2(94, 21));
        playout->addChild(pniu);
        return playout;
    }
    
    if (type == BRNN_CT_SPECIAL_BOMEBOME) {
        auto pstr = Sprite::create(imagefile, cocos2d::Rect(752, 0, 94, 42));
        pstr->setAnchorPoint(Vec2(0.f, 0.5f));
        pstr->setPosition(Vec2(0, 21));
        playout->addChild(pstr);
        return playout;
    }
    return nullptr;
}

void GameLayer::addgoldtoArea()
{
    for (int index=0; index<AREA_COUNT; ++index) {
        int goldnum = this->getgoldnumbyScore(m_lAllJettonScore[index+1]);
        for (int num=0; num<goldnum; ++num) {
            auto pgold = Sprite::create("game_res/im_gold.png");
            auto xmove = std::rand()%160;
            auto ymove = std::rand()%100;
            auto target = Vec2(WinSize.width/2-450+xmove+244*index, WinSize.height-228-ymove);
            pgold->setPosition(target);
            this->addChild(pgold);
            m_goldlist[index+1].pushBack(pgold);
        }
    }
}

void GameLayer::showgoldmove(LONGLONG score, int areaID, bool isself)
{
    int goldnum = 1;
    if (score>100) {
        goldnum = 2;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/coins_fly_in.wav");
    cocos2d::Size viewsize = WinSize;
    for (int index=0; index<goldnum; ++index) {
        auto pgold = Sprite::create("game_res/im_gold.png");
        pgold->setPosition(Vec2(viewsize.width/2-440, 68));
        if (!isself) {
            pgold->setPosition(Vec2(viewsize.width/2+400, 34));
        }
        pgold->setOpacity(0);
        pgold->runAction(FadeTo::create(0.1f, 255));
        auto xmove = std::rand()%160;
        auto ymove = std::rand()%100;
        auto target = Vec2(viewsize.width/2-450+xmove+244*areaID, viewsize.height-228-ymove);
        ccBezierConfig config;
        config.controlPoint_1 = Vec2(pgold->getPositionX(), pgold->getPositionY()*2.1f);
        config.controlPoint_2 = target;
        config.endPosition = target;
        FiniteTimeAction * paction = EaseOut::create(BezierTo::create(0.42f, config), 1);
        if (index) {
            paction = Sequence::createWithTwoActions(DelayTime::create(rand_0_1()/5.f), paction);
        }
        pgold->runAction(paction);
        this->addChild(pgold);
        m_goldlist[areaID+1].pushBack(pgold);
    }
}

void GameLayer::showgoldmoveToZ(int pos)
{
    int num = (int)m_goldlist[pos].size();
    if (!num) {
        return;
    }
    int movetime = 11;
    int cellnum = num/(movetime-1);
    if (!cellnum) {
        cellnum = 1;
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/coinCollide.wav");
    for (int index=0; index<=movetime; ++index) {
        for (int cellindex=0; cellindex<cellnum; ++cellindex) {
            int goldindex = index*cellnum+cellindex;
            if (goldindex >= num) {
                return;
            }
            auto pgold = m_goldlist[pos].at(num-goldindex-1);
            if (pgold) {
                pgold->removeFromParent();
                this->addChild(pgold,1);
                m_goldlist[pos].eraseObject(pgold);
                m_goldlist[0].pushBack(pgold);
                auto peaseAction = EaseIn::create(MoveTo::create(0.3f, Vec2(WinSize.width/2-286, WinSize.height-56)), 1);
                auto paction = Sequence::create(DelayTime::create(index*0.03f), peaseAction, CallFunc::create([=]{pgold->setVisible(false);}), NULL);
                pgold->runAction(paction);
            }
        }
    }
    int endnum = (int)m_goldlist[pos].size();
    if (endnum) {
        for (auto iter:m_goldlist[pos]) {
            if (iter) {
                iter->removeFromParent();
                this->addChild(iter,1);
                m_goldlist[0].pushBack(iter);
                auto peaseAction = EaseIn::create(MoveTo::create(0.3f, Vec2(WinSize.width/2-286, WinSize.height-56)), 1);
                auto paction = Sequence::create(DelayTime::create(12*0.03f), peaseAction, CallFunc::create([=]{iter->setVisible(false);}), NULL);
                iter->runAction(paction);
            }
        }
        m_goldlist[pos].clear();
    }
}

void GameLayer::showgoldmoveToArea()
{
    if (m_cbGameStatus != GAME_SCENE_GAME_END) {
        return;
    }
    //失败区域
    int failurenum = 0;
    int needgoldnum[4] = {0,0,0,0};//需要金币数
    
    for (int index=0; index<AREA_COUNT; ++index) {
        if (m_bUserOxCard[index+1] && m_lAllJettonScore[index+1]!=0) {
            failurenum++;
            int goldnum = this->getgoldnumbyScore(m_lAllJettonScore[index+1]*m_Multiple[index]);
            if (goldnum == 0) {
                goldnum = 2;
            }
            needgoldnum[index] = goldnum;
            log("需要金币%d", goldnum);
        }
    }
    //全赢
    if (failurenum == 0) {
        for (auto iter:m_goldlist[0]) {
            iter->removeFromParent();
        }
        m_goldlist[0].clear();
        this->showGameEndScore();
        return;
    }
    for (int index=0; index<AREA_COUNT; ++index) {
        if (needgoldnum[index]) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/coinCollide.wav");
            for (int num=0; num<needgoldnum[index]; ++num) {
                Sprite *pgold = nullptr;
                if (m_goldlist[0].size()) {
                    pgold = m_goldlist[0].at(0);
                    pgold->removeFromParent();
                    this->addChild(pgold,2);
                    m_goldlist[0].erase(0);
                    
                }
                else
                {
                    pgold = Sprite::create("game_res/im_gold.png");
                    pgold->setPosition(Vec2(WinSize.width/2-286, WinSize.height-56));
                    this->addChild(pgold,2);
                    pgold->setVisible(false);
                }
                m_goldlist[index+1].pushBack(pgold);
                auto xmove = std::rand()%160;
                auto ymove = std::rand()%100;
                auto target = Vec2(WinSize.width/2-450+xmove+244*index, WinSize.height-228-ymove);
                ccBezierConfig config;
                auto movex = (target.x - pgold->getPositionX())*1.1f;
                config.controlPoint_1 = Vec2(pgold->getPositionX()+movex, pgold->getPositionY());
                config.controlPoint_2 = target;
                config.endPosition = target;
                FiniteTimeAction * paction = EaseOut::create(BezierTo::create(0.42f, config), 1);
                auto delaytime = num*0.01f;
                pgold->runAction(Sequence::createWithTwoActions(DelayTime::create(delaytime), paction));
                pgold->runAction(Sequence::createWithTwoActions(DelayTime::create(delaytime), CallFunc::create([=]{pgold->setVisible(true);})));
            }
        }
    }
    
    //移动金币到玩家区
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.85f), CallFunc::create([=]{
        if (m_cbGameStatus != GAME_SCENE_GAME_END) {
            return;
        }
        for (int index=0; index<AREA_COUNT; ++index) {
            if (needgoldnum[index]) {
                this->showgoldmoveToUser(index+1);
            }
        }
    })));
    
    //显示结算界面
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.f), CallFunc::create([=]{
        if (m_cbGameStatus != GAME_SCENE_GAME_END) {
            return;
        }
        this->showGameEndScore();
    })));
}

void GameLayer::showgoldmoveToUser(int areaID)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/coinCollide.wav");
    bool allmovetoself = false;
    int selfgoldnum = 0;
    int listnum = (int)m_goldlist[areaID].size();
    if (m_lUserJettonScore[areaID] == m_lAllJettonScore[areaID]) {
        allmovetoself = true;
        selfgoldnum = listnum;
    }
    else
    {
        selfgoldnum = this->getgoldnumbyScore(m_lUserJettonScore[areaID]*m_Multiple[areaID-1]);
    }
    selfgoldnum = MIN(listnum, selfgoldnum);
    for (int index=selfgoldnum; index!=0; --index) {
        auto pgold = m_goldlist[areaID].at(0);
        pgold->removeFromParent();
        this->addChild(pgold,2);
        m_goldlist[areaID].erase(0);
        float delaytime = index*0.01f;
        ccBezierConfig config;
        auto target = Vec2(WinSize.width/2-440, 64);
        auto movex = (target.x - pgold->getPositionX())*1.1f;
        config.controlPoint_1 = Vec2(pgold->getPositionX()+movex, pgold->getPositionY());
        config.controlPoint_2 = target;
        config.endPosition = target;
        FiniteTimeAction * paction = EaseIn::create(BezierTo::create(0.42f, config), 1);
        pgold->runAction(Sequence::createWithTwoActions(DelayTime::create(delaytime), paction));
        pgold->runAction(Sequence::createWithTwoActions(DelayTime::create(delaytime+0.42f), CallFunc::create([=]{pgold->removeFromParent();})));
    }
    
    listnum = (int)m_goldlist[areaID].size();
    for (int index=0; index<listnum; ++index) {
        auto pgold = m_goldlist[areaID].at(listnum-index-1);
        pgold->removeFromParent();
        this->addChild(pgold,2);
        m_goldlist[areaID].eraseObject(pgold);
        float delaytime = index*0.01f;
        ccBezierConfig config;
        auto target = Vec2(WinSize.width/2+400, 34);
        auto movex = (target.x - pgold->getPositionX())*1.1f;
        config.controlPoint_1 = Vec2(pgold->getPositionX()+movex, pgold->getPositionY());
        config.controlPoint_2 = target;
        config.endPosition = target;
        FiniteTimeAction * paction = EaseIn::create(BezierTo::create(0.42f, config), 1);
        pgold->runAction(Sequence::createWithTwoActions(DelayTime::create(delaytime), paction));
        pgold->runAction(Sequence::createWithTwoActions(DelayTime::create(delaytime+0.42f), CallFunc::create([=]{pgold->removeFromParent();})));
    }
}

void GameLayer::showGameEndScore()
{
    //分数结算
    auto puser = m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser) {
        puser->m_date.lScore += m_lUserScore;
        m_showscore = puser->m_date.lScore;
        this->updateselfscore();
    }
    if (m_wBankerUser != HallDataMgr::getInstance()->m_wChairID && m_wBankerUser!=INVALID_CHAIR) {
        auto pbankeruser = this->getUserByChairID(m_wBankerUser);
        if (pbankeruser) {
            pbankeruser->m_date.lScore += m_lBankerScore;
        }
    }
    this->showBanker();
    
    int recordnum = (int)m_recordlist.size();
    //如果没有记录，随机产生记录
    if (recordnum == 0) {
        this->produceRecordList();
    }
    auto precord = new tagServerGameRecord;
    precord->bWinShunMen = m_bUserOxCard[1];
    precord->bWinDuiMen = m_bUserOxCard[2];
    precord->bWinDaoMen = m_bUserOxCard[3];
    precord->bWinHuang = m_bUserOxCard[4];
    m_recordlist.push_back(precord);
    
    precord = m_recordlist.at(0);
    for(std::vector<tagServerGameRecord *>::iterator iter=m_recordlist.begin(); iter != m_recordlist.end(); iter++)
    {
        if (*iter == precord) {
            m_recordlist.erase(iter);
        }
    }
    CC_SAFE_DELETE(precord);
    
    LONGLONG addscore = 0;
    for (int index=0; index<AREA_COUNT; ++index) {
        addscore += m_lUserJettonScore[index+1];
    }
    //未下注，不显示结算节目
    if (addscore == 0 && m_ApplyBankerStatus!=2) {
        return;
    }
    
    //结算界面显示
    cocos2d::Size viewsize = WinSize;
    bool iswin = m_lUserScore>=0?true:false;
    std::string endeffectstr = iswin?"sound_res/gameWin.wav":"sound_res/gameLose.wav";
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(endeffectstr.c_str());
    
    std::string bgstr = iswin?"game_res/im_win_bg.png":"game_res/im_failure_bg.png";
    std::string txtstr = iswin?"game_res/txt_win.png":"game_res/txt_failure.png";
    auto pcolorlayer = LayerColor::create(Color4B(0,0,0,110), viewsize.width, viewsize.height);
    pcolorlayer->setTag(Tag_End_Layer);
    this->addChild(pcolorlayer,3);
    
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_End_Layer);
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    pcolorlayer->addChild(pbuttonbg);
    
    auto pbgimage = ImageView::create(bgstr);
    pbgimage->setPosition(Vec2(viewsize.width/2, 335));
    pbgimage->setScale(0.5f);
    pbgimage->runAction(Sequence::create(ScaleTo::create(0.17f, 1.1f), ScaleTo::create(0.03f, 1.f), NULL));
    pcolorlayer->addChild(pbgimage);
    
    auto pheadbg = Sprite::create("game_res/im_head_bg.png");
    pheadbg->setPosition(Vec2(pbgimage->getContentSize().width/2, 350));
    pbgimage->addChild(pheadbg);
    
    auto headimage = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                     HallDataMgr::getInstance()->m_wCustom,
                                                     HallDataMgr::getInstance()->m_dwUserID,
                                                     HallDataMgr::getInstance()->m_cbGender);
    headimage->setHeadSize(100);
    
    auto clipnode = ClippingNode::create();
    clipnode->addChild(headimage);
    auto circleNode = DrawNode::create();
    const int maxTrangle = 360;
    Vec2 circleVec2[maxTrangle];
    for (int i = 0; i < maxTrangle; i ++)
    {
        float x = cosf( i * (M_PI/180.f)) * 50;
        float y = sinf( i * (M_PI/180.f)) * 50;
        circleVec2[i] = Vec2(x, y);
    }
    auto circleColor = Color4F(0, 1, 0, 1);
    circleNode->drawPolygon(circleVec2, maxTrangle, circleColor, 1, circleColor);
    clipnode->setStencil(circleNode);
    clipnode->setPosition(Vec2(pbgimage->getContentSize().width/2, 350));
    pbgimage->addChild(clipnode);
    
    auto ptxtimage = ImageView::create(txtstr);
    ptxtimage->setPosition(Vec2(pbgimage->getContentSize().width/2, 280));
    pbgimage->addChild(ptxtimage);
    
    auto scorestr = getScoreString(std::abs(m_lUserScore));
    scorestr.insert(0, "/");
    auto pos = scorestr.find("万");
    if (pos != std::string::npos) {
        auto length = scorestr.length();
        scorestr = scorestr.substr(0,length-3);
        scorestr.append(":");
    }
    pos = scorestr.find("亿");
    if (pos != std::string::npos) {
        auto length = scorestr.length();
        scorestr = scorestr.substr(0,length-3);
        scorestr.append(";");
    }
    std::string numimage = iswin?"game_res/num_win.png":"game_res/num_failure.png";
    auto pscorestr = Label::createWithCharMap(numimage, 33, 48, '.');
    pscorestr->setString(scorestr);
    pscorestr->setPosition(Vec2(pbgimage->getContentSize().width/2, 198));
    pbgimage->addChild(pscorestr);
    
    std::string zname = "小怪兽系统";
    if (m_wBankerUser != INVALID_CHAIR) {
        auto puser = this->getUserByChairID(m_wBankerUser);
        if (puser) {
            zname = puser->m_nickname;
        }
    }
    
    auto pzstr = NameLabel::create(zname, FontBold, 24);
    pzstr->setFontColor(Color3B::GREEN);
    pzstr->setAnchorPoint(Vec2(1.f, 0.5f));
    pzstr->setSize(cocos2d::Size(150, 25));
    pzstr->setPosition(Vec2(pbgimage->getContentSize().width/2-10, 120));
    pbgimage->addChild(pzstr);
    
    std::string zscorestr = __String::createWithFormat("%lld", std::abs(m_lBankerScore))->getCString();
    zscorestr.insert(0, m_lBankerScore>=0?"+":"-");
    zscorestr.insert(0, ":");
    auto pzscorestr = Text::create(zscorestr, FontBold, 24);
    pzscorestr->setAnchorPoint(Vec2(0.f, 0.5f));
    pzscorestr->setPosition(Vec2(pbgimage->getContentSize().width/2, 120));
    pzscorestr->setColor(Color3B::GREEN);
    pbgimage->addChild(pzscorestr);
    
    if (m_wBankerUser != HallDataMgr::getInstance()->m_wChairID) {
        auto pname = HallDataMgr::getInstance()->m_pNickName;
        auto pselfname = NameLabel::create(pname, FontBold, 24);
        pselfname->setFontColor(Color3B::GREEN);
        pselfname->setAnchorPoint(Vec2(1.f, 0.5f));
        pselfname->setSize(cocos2d::Size(150, 25));
        pselfname->setPosition(Vec2(pbgimage->getContentSize().width/2-10, 90));
        pbgimage->addChild(pselfname);
        
        std::string selfscorestr = __String::createWithFormat("%lld", std::abs(m_lUserScore))->getCString();
        selfscorestr.insert(0, m_lUserScore>=0?"+":"-");
        selfscorestr.insert(0, ":");
        auto pselfscorestr = Text::create(selfscorestr, FontBold, 24);
        pselfscorestr->setAnchorPoint(Vec2(0.f, 0.5f));
        pselfscorestr->setPosition(Vec2(pbgimage->getContentSize().width/2, 90));
        pselfscorestr->setColor(Color3B::GREEN);
        pbgimage->addChild(pselfscorestr);
    }
    
    auto pcontinue = Button::create("game_res/bt_continue_0.png", "game_res/bt_continue_1.png");
    pcontinue->setPosition(Vec2(pbgimage->getContentSize().width/2, 45));
    pcontinue->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_End_Layer);
        }
    });
    pbgimage->addChild(pcontinue);
}

void GameLayer::showGameRecord()
{
    cocos2d::Size viewsize = WinSize;
    auto pcolorlayer = LayerColor::create(Color4B(0,0,0,110), viewsize.width, viewsize.height);
    pcolorlayer->setTag(Tag_Record_Lyaer);
    this->addChild(pcolorlayer,3);
    
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_Record_Lyaer);
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    pcolorlayer->addChild(pbuttonbg);
    
    auto pbg = Button::create("game_res/trend_bg.png", "game_res/trend_bg.png");
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    pbg->setScale(0.5f);
    pbg->runAction(Sequence::create(ScaleTo::create(0.17f, 1.1f), ScaleTo::create(0.03f, 1.f), NULL));
    pcolorlayer->addChild(pbg);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(pbg->getContentSize().width-40, pbg->getContentSize().height-45));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                          this->removeChildByTag(Tag_Record_Lyaer);
                                      }
                                  });
    pbg->addChild(pclose);
    
    auto ptitlebg = ImageView::create("game_res/im_title_bg.png");
    ptitlebg->setPosition(Vec2(pbg->getContentSize().width/2, 360));
    pbg->addChild(ptitlebg);
    
    auto ptitle = ImageView::create("game_res/txt_trend.png");
    ptitle->setPosition(ptitlebg->getContentSize()/2);
    ptitlebg->addChild(ptitle);
    
    auto ptxtnew = ImageView::create("game_res/txt_new.png");
    ptxtnew->setPosition(Vec2(pbg->getContentSize().width-124, pbg->getContentSize().height-60));
    pbg->addChild(ptxtnew);
    
    for (int index=0; index<AREA_COUNT; ++index) {
        auto pcardcolor = ImageView::create(__String::createWithFormat("game_res/im_card_color_%d.png", index)->getCString());
        pcardcolor->setPosition(Vec2(74, 286-67*index));
        pbg->addChild(pcardcolor);
        
        if (index<(AREA_COUNT-1)) {
            auto pline = ImageView::create("game_res/im_line.png");
            pline->setAnchorPoint(Vec2(0.f, 0.5f));
            pline->setPosition(Vec2(75, 253-67*index));
            pbg->addChild(pline);
        }
    }
    
    int recordnum = (int)m_recordlist.size();
    //如果没有记录，随机产生记录
    if (recordnum == 0) {
        this->produceRecordList();
    }
    for (int index=0; index<10; ++index) {
        auto pdata = m_recordlist.at(index);
        bool value[4] = {pdata->bWinShunMen, pdata->bWinDuiMen, pdata->bWinDaoMen, pdata->bWinHuang};
        for (int i=0; i<4; ++i) {
            std::string imagestr = value[i]?"game_res/im_win_circle.png":"game_res/im_failure_circle.png";
            auto pimage = ImageView::create(imagestr);
            pimage->setPosition(Vec2(138+index*56, 286-67*i));
            pbg->addChild(pimage);
        }
    }
}

void GameLayer::showUserList()
{
    cocos2d::Size viewsize = WinSize;
    auto pcolorlayer = LayerColor::create(Color4B(0,0,0,110), viewsize.width, viewsize.height);
    pcolorlayer->setTag(Tag_UserList_Layer);
    this->addChild(pcolorlayer,3);
    
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_UserList_Layer);
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    pcolorlayer->addChild(pbuttonbg);
    
    auto pbg = Button::create("game_res/trend_bg.png", "game_res/trend_bg.png");
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    pbg->setScale(0.5f);
    pbg->runAction(Sequence::create(ScaleTo::create(0.17f, 1.1f), ScaleTo::create(0.03f, 1.f), NULL));
    pcolorlayer->addChild(pbg);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(pbg->getContentSize().width-40, pbg->getContentSize().height-40));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                          this->removeChildByTag(Tag_UserList_Layer);
                                      }
                                  });
    pbg->addChild(pclose);
    
    auto ptitlebg = ImageView::create("game_res/im_title_bg.png");
    ptitlebg->setPosition(Vec2(pbg->getContentSize().width/2, 360));
    pbg->addChild(ptitlebg);
    
    auto ptitle = ImageView::create("game_res/txt_player.png");
    ptitle->setPosition(ptitlebg->getContentSize()/2);
    ptitlebg->addChild(ptitle);
    
    auto ptestlist = TableView::create(this, cocos2d::Size(650,280));
    ptestlist->setDirection(extension::ScrollView::Direction::VERTICAL);
    //ptestlist->setAnchorPoint(Vec2(0.5f, 1.f));
    ptestlist->setPosition(Vec2(65,38));
    pbg->addChild(ptestlist);
//    auto userlist = ListView::create();
//    userlist->setBounceEnabled(true);
//    userlist->setDirection(ui::ScrollView::Direction::VERTICAL);
//    userlist->setContentSize(cocos2d::Size(650,280));
//    userlist->setAnchorPoint(cocos2d::Point(0.5f,1.f));
//    userlist->setPosition(cocos2d::Point(pbg->getContentSize().width/2,pbg->getContentSize().height-85));
//    userlist->setItemsMargin(6.f);
//    pbg->addChild(userlist);
//    
//    typedef std::pair<int, UserData *> PAIR;
//    std::vector<PAIR> name_score_vec(m_UserList.begin(), m_UserList.end());
//    std::sort(name_score_vec.begin(), name_score_vec.end(), [](const PAIR& l1, const PAIR& l2) {
//        return l1.second->m_date.lScore > l2.second->m_date.lScore;});
//    
//    for (auto iter : name_score_vec) {
//        auto puserbg = Button::create("game_res/im_player_bg.png", "game_res/im_player_bg.png");
//        userlist->pushBackCustomItem(puserbg);
//        
//        auto pheadimage = HeaderRequest::createwithFaceID(iter.second->m_date.wFaceID, iter.second->m_date.dwCustomID, iter.second->m_date.dwUserID);
//        pheadimage->setHeadSize(35);
//        pheadimage->setPosition(Vec2(60, puserbg->getContentSize().height/2));
//        puserbg->addChild(pheadimage);
//        
//        auto pname = Label::createWithSystemFont(iter.second->m_nickname, FontNormal, 24);
//        pname->setAnchorPoint(Vec2(0.f, 0.5f));
//        pname->setPosition(Vec2(90, puserbg->getContentSize().height/2));
//        pname->setDimensions(270, 30);
//        pname->setColor(Color3B(122, 4, 5));
//        puserbg->addChild(pname);
//        
//        auto pimagescore = ImageView::create("game_res/im_score.png");
//        pimagescore->setPosition(Vec2(476, puserbg->getContentSize().height/2));
//        puserbg->addChild(pimagescore);
//        
//        auto pscore = Text::create(getScoreString(iter.second->m_date.lScore), FontNormal, 24);
//        pscore->setAnchorPoint(Vec2(0.f, 0.5f));
//        pscore->setPosition(Vec2(512, puserbg->getContentSize().height/2));
//        pscore->setColor(Color3B(122, 4, 5));
//        puserbg->addChild(pscore);
//    }
}

void GameLayer::showApplyBankerList()
{
    cocos2d::Size viewsize = WinSize;
    auto pcolorlayer = LayerColor::create(Color4B(0,0,0,110), viewsize.width, viewsize.height);
    pcolorlayer->setTag(Tag_Apply_Banker_Layer);
    this->addChild(pcolorlayer,3);
    
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_Apply_Banker_Layer);
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    pcolorlayer->addChild(pbuttonbg);
    
    auto pbg = Button::create("game_res/im_sz_bg.png", "game_res/im_sz_bg.png");
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    pbg->setScale(0.5f);
    pbg->runAction(Sequence::create(ScaleTo::create(0.17f, 1.1f), ScaleTo::create(0.03f, 1.f), NULL));
    pcolorlayer->addChild(pbg);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(pbg->getContentSize().width-60, pbg->getContentSize().height-50));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                          this->removeChildByTag(Tag_Apply_Banker_Layer);
                                      }
                                  });
    pbg->addChild(pclose);
    
    auto applylist = ListView::create();
    applylist->setBounceEnabled(true);
    applylist->setDirection(ui::ScrollView::Direction::VERTICAL);
    applylist->setContentSize(cocos2d::Size(500,175));
    applylist->setAnchorPoint(cocos2d::Point(0.5f,1.f));
    applylist->setPosition(cocos2d::Point(pbg->getContentSize().width/2,pbg->getContentSize().height-85));
    applylist->setItemsMargin(6.f);
    pbg->addChild(applylist);
    
    //当前庄家
    auto pzbg = Button::create("game_res/im_sz_frame.png", "game_res/im_sz_frame.png");
    auto pztxt = ImageView::create("game_res/im_zhuang.png");
    pztxt->setAnchorPoint(Vec2(0.f, 0.5f));
    pztxt->setPosition(Vec2(30, pzbg->getContentSize().height/2));
    pztxt->setScale(0.57f);
    pzbg->addChild(pztxt);
    
    std::string namestr = "小怪兽系统";
    std::string scorestr = getScoreString(8888888);
    if (m_wBankerUser != INVALID_CHAIR) {
        auto puser = this->getUserByChairID(m_wBankerUser);
        if (puser) {
            namestr = puser->m_nickname;
            scorestr = getScoreString(puser->m_date.lScore);
        }
    }
    
    auto pname = Label::createWithSystemFont(namestr, FontNormal, 24);
    pname->setAnchorPoint(Vec2(0.f, 0.5f));
    pname->setPosition(Vec2(64, pzbg->getContentSize().height/2));
    pname->setDimensions(270, 30);
    pname->setColor(Color3B(122, 4, 5));
    pzbg->addChild(pname);
    
    auto pscore = Text::create(scorestr, FontNormal, 24);
    pscore->setAnchorPoint(Vec2(0.f, 0.5f));
    pscore->setPosition(Vec2(318, pzbg->getContentSize().height/2));
    pscore->setColor(Color3B(122, 4, 5));
    pzbg->addChild(pscore);
    applylist->pushBackCustomItem(pzbg);
    
    //申请列表
    auto listfunction = [=](UserData *puser){
        auto puserbg = Button::create("game_res/im_sz_frame.png", "game_res/im_sz_frame.png");
        
        auto pname = Label::createWithSystemFont(puser->m_nickname, FontNormal, 24);
        pname->setAnchorPoint(Vec2(0.f, 0.5f));
        pname->setPosition(Vec2(64, puserbg->getContentSize().height/2));
        pname->setDimensions(270, 30);
        pname->setColor(Color3B(122, 4, 5));
        puserbg->addChild(pname);
        
        auto pscore = Text::create(getScoreString(puser->m_date.lScore), FontNormal, 24);
        pscore->setAnchorPoint(Vec2(0.f, 0.5f));
        pscore->setPosition(Vec2(318, puserbg->getContentSize().height/2));
        pscore->setColor(Color3B(122, 4, 5));
        puserbg->addChild(pscore);
        
        applylist->pushBackCustomItem(puserbg);
    };
    
    for (auto iter : m_bankwaitlist) {
        auto puser = this->getUserByChairID(iter.second);
        if (puser) {
            listfunction(puser);
        }
    }
    
    //取消申请按钮
    auto canclefunction = [=]{
        auto papply = Button::create("game_res/bt_sz_1_0.png", "game_res/bt_sz_1_1.png", "game_res/bt_sz_1_1.png");
        papply->setPosition(Vec2(pbg->getContentSize().width/2, 54));
        papply->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_CANCEL_BANKER, nullptr, 0);
                m_ApplyBankerStatus = 0;
                this->updateApplyBankerButton();
                m_bankwaitlist.erase(HallDataMgr::getInstance()->m_wChairID);
                this->removeChildByTag(Tag_Apply_Banker_Layer);
            }
        });
        pbg->addChild(papply);
    };
    
    if (m_ApplyBankerStatus == 0) {
        std::string popstring = "上庄至少需要";
        popstring += getScoreString(m_lApplyBankerCondition);
        auto ptips = Text::create(popstring, FontBold, 20);
        //ptips->setColor(Color3B(122, 4, 5));
        ptips->setPosition(Vec2(pbg->getContentSize().width/2, 95));
        pbg->addChild(ptips);
        
        auto puser = m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
        auto papply = Button::create("game_res/bt_sz_0_0.png", "game_res/bt_sz_0_1.png", "game_res/bt_sz_0_1.png");
        papply->setPosition(Vec2(pbg->getContentSize().width/2, 54));
        papply->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_APPLY_BANKER, nullptr, 0);
                m_ApplyBankerStatus = 1;
                this->updateApplyBankerButton();
                listfunction(puser);
                m_bankwaitlist.insert(std::make_pair(puser->m_date.wChairID, puser->m_date.wChairID));
                papply->removeFromParent();
                canclefunction();
            }
        });
        if (puser->m_date.lScore < m_lApplyBankerCondition)
        {
            papply->setEnabled(false);
            papply->setColor(Color3B::GRAY);
        }
        pbg->addChild(papply);
    }
    else
    {
        canclefunction();
    }
}

void GameLayer::showEditHorn()
{
    cocos2d::Size viewsize = WinSize;
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->setTag(Tag_Horn_Layer);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_Horn_Layer);
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    this->addChild(pbuttonbg, 3);
    
    auto pbg = Button::create("game_res/im_horn_bg.png", "game_res/im_horn_bg.png");
    pbg->setAnchorPoint(Vec2(1.f, 1.f));
    pbg->setPosition(Vec2(viewsize.width-50, viewsize.height-65));
    pbg->setTag(10);
    pbg->setScale(0.5f);
    pbg->runAction(Sequence::create(ScaleTo::create(0.17f, 1.1f), ScaleTo::create(0.03f, 1.f), NULL));
    pbuttonbg->addChild(pbg);
    
    auto psend = Button::create("game_res/bt_send_0.png", "game_res/bt_send_1.png");
    psend->setPosition(Vec2(pbg->getContentSize().width-100, pbg->getContentSize().height/2));
    psend->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            auto pbgbutton = static_cast<Button *>(this->getChildByTag(Tag_Horn_Layer)->getChildByTag(10));
            auto pedit = static_cast<EditBox *>(pbgbutton->getChildByTag(10));
            if (!pedit) {
                this->removeChildByTag(Tag_Horn_Layer);
                return;
            }
            std::string psendtxt = pedit->getText();
            this->removeChildByTag(Tag_Horn_Layer);
            if (psendtxt.length() == 0) {
                HallDataMgr::getInstance()->AddpopLayer("系统提示", "喇叭发送内容不能为空", Type_Ensure);
                return;
            }
            if (strLength(psendtxt) > 40) {
                HallDataMgr::getInstance()->AddpopLayer("系统提示", "喇叭发送内容长度不能超过40个字，请重新输入", Type_Ensure);
                return;
            }
            CMD_GR_C_SendTrumpet trumpet;
            memset(&trumpet , 0, sizeof(CMD_GR_C_SendTrumpet));
            trumpet.cbRequestArea = PT_ISSUE_AREA_SERVER;
            trumpet.TrumpetColor = GLCOLOR_RGB(255, 255, 255);
            trumpet.wPropertyIndex = PROPERTY_ID_TYPHON;
            UTF8Str_To_UTF16Str(psendtxt, trumpet.szTrumpetContent);
            NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_PROPERTY_TRUMPET, &trumpet, sizeof(trumpet));
        }
    });
    pbg->addChild(psend);
    
    auto pedit = ui::EditBox::create(cocos2d::Size(438, 65), "game_res/im_horn_edit.png");
    pedit->setAnchorPoint(Vec2(0.f, 0.5f));
    pedit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    pedit->setPosition(Vec2(20, pbg->getContentSize().height/2));
    pedit->setTag(10);
    pedit->setPlaceHolder("发送1条喇叭1万金币");
    pedit->setFontSize(28);
    pedit->setPlaceholderFont(FontNormal, 26);
    pbg->addChild(pedit);
}

void GameLayer::showUserInfo(UserData *puser)
{
    if (!puser && m_wBankerUser!=INVALID_CHAIR) {
        return;
    }
    cocos2d::Size viewsize = WinSize;
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->setTag(Tag_Horn_Layer);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeChildByTag(Tag_Horn_Layer);
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    this->addChild(pbuttonbg, 3);
    
    auto pbg = Button::create("public_res/pop_frame.png", "public_res/pop_frame.png");
    pbg->setScale9Enabled(true);
    pbg->setContentSize(cocos2d::Size(500, 320));
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    pbg->setScale(0.5f);
    pbg->runAction(Sequence::create(ScaleTo::create(0.17f, 1.1f), ScaleTo::create(0.03f, 1.f), NULL));
    pbuttonbg->addChild(pbg);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(pbg->getContentSize().width-45, pbg->getContentSize().height-45));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                           this->removeChildByTag(Tag_Horn_Layer);
                                      }
                                  });
    pbg->addChild(pclose);
    
    const char *titlelist[4] = {"昵称：", /*"ID：" ,*/ "性别：", "携带游戏币："};
    for (int index=0; index<3; ++index) {
        auto ptitle = Text::create(titlelist[index], FontBold, 30);
        ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
        ptitle->setPosition(Vec2(50, pbg->getContentSize().height-80-60*index));
        pbg->addChild(ptitle);
    }
    std::string nickname;
    //std::string idstr;
    std::string sexstr;
    std::string scorestr;
    if (!puser) {
        nickname = "小怪兽系统";
        //idstr = "000000";
        sexstr = "女";
        scorestr = "888.8万";
    }
    else
    {
        nickname = puser->m_nickname;
        //idstr = __String::createWithFormat("%d", puser->m_date.dwGameID)->getCString();
        sexstr = puser->m_date.cbGender==0?"女":"男";
        scorestr = __String::createWithFormat("%lld", puser->m_date.lScore)->getCString();
    }
    
    auto pnickname = NameLabel::create(nickname, FontBold, 30);
    pnickname->setFontColor(Color3B(250, 234, 123));
    pnickname->setAnchorPoint(Vec2(0.f, 0.5f));
    pnickname->setSize(cocos2d::Size(320, 25));
    pnickname->setPosition(Vec2(130, pbg->getContentSize().height-80));
    pbg->addChild(pnickname);
    /*
    auto pID = Text::create(idstr, FontBold, 30);
    pID->setAnchorPoint(Vec2(0.f, 0.5f));
    pID->setPosition(Vec2(pnickname->getPositionX(), pnickname->getPositionY()-60));
    pID->setColor(Color3B(250, 234, 123));
    pbg->addChild(pID);
     */
    
    auto psex = Text::create(sexstr, FontBold, 30);
    psex->setAnchorPoint(Vec2(0.f, 0.5f));
    psex->setPosition(Vec2(pnickname->getPositionX(), pnickname->getPositionY()-60));
    psex->setColor(Color3B(250, 234, 123));
    pbg->addChild(psex);
    
    auto pscore = Text::create(scorestr, FontBold, 30);
    pscore->setAnchorPoint(Vec2(0.f, 0.5f));
    pscore->setPosition(Vec2(pnickname->getPositionX()+86, pnickname->getPositionY()-60*2));
    pscore->setColor(Color3B(250, 234, 123));
    pbg->addChild(pscore);
}

TableViewCell* GameLayer::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    typedef std::pair<int, UserData *> PAIR;
    std::vector<PAIR> name_score_vec(m_UserList.begin(), m_UserList.end());
    std::sort(name_score_vec.begin(), name_score_vec.end(), [](const PAIR& l1, const PAIR& l2) {
        return l1.second->m_date.lScore < l2.second->m_date.lScore;});
    auto pdata = name_score_vec.at(idx).second;
    if (!cell) {
        cell = new (std::nothrow) TableViewCell();
        cell->autorelease();
        auto puserbg = Sprite::create("game_res/im_player_bg.png");
        puserbg->setPosition(325, 31);
        puserbg->setTag(10);
        cell->addChild(puserbg);
        
        auto pheadimage = HeaderRequest::createwithFaceID(pdata->m_date.wFaceID,
                                                          pdata->m_date.dwCustomID,
                                                          pdata->m_date.dwUserID,
                                                          pdata->m_date.cbGender);
        pheadimage->setHeadSize(35);
        pheadimage->setTag(10);
        pheadimage->setPosition(Vec2(60, puserbg->getContentSize().height/2));
        puserbg->addChild(pheadimage);
        
        auto pname = Label::createWithSystemFont(pdata->m_nickname, FontNormal, 24);
        pname->setAnchorPoint(Vec2(0.f, 0.5f));
        pname->setPosition(Vec2(90, puserbg->getContentSize().height/2));
        pname->setDimensions(270, 30);
        pname->setColor(Color3B(122, 4, 5));
        pname->setTag(11);
        puserbg->addChild(pname);
        
        auto pimagescore = ImageView::create("game_res/im_score.png");
        pimagescore->setPosition(Vec2(476, puserbg->getContentSize().height/2));
        puserbg->addChild(pimagescore);
        
        auto pscore = Label::createWithSystemFont(getScoreString(pdata->m_date.lScore), FontNormal, 24);
        pscore->setAnchorPoint(Vec2(0.f, 0.5f));
        pscore->setPosition(Vec2(512, puserbg->getContentSize().height/2));
        pscore->setColor(Color3B(122, 4, 5));
        pscore->setTag(12);
        puserbg->addChild(pscore);
    }
    else
    {
        auto puserbg = cell->getChildByTag(10);
        puserbg->removeChildByTag(10);
        auto pheadimage = HeaderRequest::createwithFaceID(pdata->m_date.wFaceID,
                                                          pdata->m_date.dwCustomID,
                                                          pdata->m_date.dwUserID,
                                                          pdata->m_date.cbGender);
        pheadimage->setHeadSize(35);
        pheadimage->setTag(10);
        pheadimage->setPosition(Vec2(60, puserbg->getContentSize().height/2));
        puserbg->addChild(pheadimage);
        
        auto pname = static_cast<Label *>(puserbg->getChildByTag(11));
        pname->setString(pdata->m_nickname);
        
        auto pscore = static_cast<Label *>(puserbg->getChildByTag(12));
        pscore->setString(getScoreString(pdata->m_date.lScore));
    }
    
    
    return cell;
}
