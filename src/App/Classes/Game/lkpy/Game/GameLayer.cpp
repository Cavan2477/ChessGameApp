//
//  GameLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#include "GameLayer.h"
#include "Project.h"
#include "lkpy_GameDataMgr.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "DeskLayer.h"
#include "DeskLayer.h"
#include "CannonLayer.h"
#include "GameMessage.h"
#include "CoinMove.h"
#include "GameSettingLayer.h"
#include "GameBankLayer.h"
#include "GameClearLayer.h"
#include "GameHelpLayer.h"
#include "SimpleAudioEngine.h"
#include "HallDataMgr.h"
#include "UserData.h"
#include "HallDataMgr.h"
#include "CMD_GameServer.h"
#include "CircleBy.h"
//#include "SettingLayer.h"
#include "CocosHeader.h"
#include "lkpy_GameDefine.h"
#include "lkpy_Help.h"
#include "Help.h"
#include "lkpy_CMD_NotifyDefine.h"
USING_NS_CC;
using namespace ui;
USING_LKPY_NAMESPACE;
//鱼出现银币效果的数量 1-5
static int silverNumArrary[5] = {2,2,3,4,4};

//5-
static int goldNumArrary[20] = {1,1,1,2,2,3,3,4,5,6,8,16,16,16,18,18};

enum ButtonTag
{
    Tag_BG = 0,
    
    Tag_Fish_Layer = 3,
    
    Tag_Cannon_Layer=8,
    
    Tag_UI_Layer = 20,
    Tag_UserScore = 9,
    Tag_Game_Multiple,//当前倍数
    Tag_BT_Multiple,//倍数选择按钮
    Tag_Multiple_Tip,//倍数选择提示
    Tag_Shoot,
    Tag_Lock,
    Tag_BT_Menu,
    Tag_BT_Bank,
    Tag_BT_Setting,
    Tag_BT_Help,
    Tag_Bt_Clearing,
    Tag_BT_Chat_OnOff,
    Tag_Bt_Send_Chat,
    Tag_Gold_Circle = 3000,
    Tag_Gold_Circle_Text = 4000
};

enum layerTag
{
    TAG_Clear_Layer =10000,
    TAG_Help_Layer,
    TAG_Second_Tip
};

//"获得一个补给箱！击中可能获得大量奖励哟！赶快击杀！"

const char *FishName[25] = {"小黄刺鱼","小草鱼","热带黄鱼","大眼金鱼","热带紫鱼","小丑鱼","河豚鱼","狮头鱼","灯笼鱼","海龟","神仙鱼","蝴蝶鱼","铃铛鱼","剑鱼","魔鬼鱼","大白鲨","大金鲨","双头企鹅"
    ,"巨型黄金鲨","金龙","李逵","水浒传","忠义堂","爆炸飞镖","宝箱"};

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
    //NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameLayer::onGameScene, this);
    auto viewsize = WinSize;
    auto bgname = __String::createWithFormat("game_res/game_bg_%d.png", GameDataMgr::getInstance()->m_sceneData.cbBackIndex)->getCString();
    auto pbg = ImageView::create(bgname);
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    pbg->setTag(Tag_BG);
    this->addChild(pbg);
    
    auto pwater = Sprite::createWithSpriteFrameName("water_0.png");
    pwater->setScale(WinSize.width/512.f);
    pwater->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    pwater->setBlendFunc(blendFunc);
    pwater->setOpacity(120);
    this->addChild(pwater,1);
    
    auto panim = Animate::create(AnimationCache::getInstance()->getAnimation(WaterAnim));
    auto panim1 = panim->reverse();
    auto paction = RepeatForever::create(Sequence::createWithTwoActions(panim, panim1));
    pwater->runAction(paction);
    
    m_fishLayer = Layer::create();
    this->addChild(m_fishLayer,Tag_Fish_Layer);
    if (GameDataMgr::getInstance()->m_reversal) {
        m_fishLayer->setRotation(180);
    }
    
    m_CannonLayer = CannonLayer::create();
    this->addChild(m_CannonLayer, Tag_Cannon_Layer);
    
    m_uiLayer = Layer::create();
    this->addChild(m_uiLayer,20);

    this->scheduleUpdate();
    this->uiLayerinit();
    m_nGoldBannerNum = 0;
    m_nSecondCount = 60;
    this->schedule(schedule_selector(GameLayer::secondSchedule),1.0f);
    return true;
};

void GameLayer::uiLayerinit()
{
    auto viewsize = WinSize;
    
    auto pbottom = Button::create("game_res/game_buttom.png", "game_res/game_buttom.png");
    pbottom->setScaleX(JudgeScale);
    pbottom->setTag(1);
    pbottom->setPosition(Vec2(viewsize.width/2, pbottom->getContentSize().height/2));
    m_uiLayer->addChild(pbottom);
    
    std::string scorestr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestr = getGoldStr(scorestr, ',');
    if (scorestr.length() > 16) {
        scorestr = scorestr.substr(0,14);
        scorestr.append("::");
    }
    auto m_score = Label::create(scorestr, FontBold,23);
    m_score->setString(scorestr);
    //m_score->setDimensions(170, 20);
    m_score->setTag(Tag_UserScore);
    m_score->setHorizontalAlignment(TextHAlignment::CENTER);
    m_score->setPosition(Vec2(145, 26));
    pbottom->addChild(m_score);
    
//    auto bt_multiple = Button::create();
//    bt_multiple->setScale9Enabled(true);
//    bt_multiple->setContentSize(cocos2d::Size(186,30));
//    bt_multiple->setPosition(Vec2(365, 18));
//    bt_multiple->setTag(Tag_BT_Multiple);
//    bt_multiple->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
//    pbottom->addChild(bt_multiple);
    GameDataMgr *gd = GameDataMgr::getInstance();
    auto multiplestr = __String::createWithFormat("%d:", GameDataMgr::getInstance()->m_sceneData.nMultipleValue[GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[HallDataMgr::getInstance()->m_wChairID]])->getCString();
    auto txt_multiple = Label::createWithCharMap("game_res/num_multiple.png", 19, 20, '0');
    txt_multiple->setString(multiplestr);
    txt_multiple->setTag(Tag_Game_Multiple);
    txt_multiple->setPosition(Vec2(365, 26));
    txt_multiple->setHorizontalAlignment(TextHAlignment::CENTER);
    pbottom->addChild(txt_multiple);
    
    auto multipletip = Button::create("game_res/im_multiple_tip_0.png");
    multipletip->loadTextures("game_res/im_multiple_tip_0.png", "game_res/im_multiple_tip_1.png","game_res/im_multiple_tip_0.png");
    multipletip->setPosition(Vec2(390, 40));
    multipletip->setScale(0.8f);
    multipletip->setAnchorPoint(Vec2(0.5f, 0.f));
    multipletip->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    multipletip->setTag(Tag_BT_Multiple);
    pbottom->addChild(multipletip);
    
//    auto fadeaction = Sequence::create(FadeTo::create(1.7f, 80), DelayTime::create(0.17f), FadeTo::create(1.7f, 255), DelayTime::create(0.17f),NULL);
//    auto paction = Sequence::createWithTwoActions(Repeat::create(fadeaction, 10), CallFunc::create([=]{pbottom->removeChildByTag(Tag_Multiple_Tip);}));
//    multipletip->runAction(paction);
    
    auto autoshoot = Button::create("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    autoshoot->setTag(Tag_Shoot);
    autoshoot->setPosition(Vec2(536, 24));
    autoshoot->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pbottom->addChild(autoshoot);
    
    auto autolock = Button::create("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    autolock->setTag(Tag_Lock);
    autolock->setPosition(Vec2(762, 24));
    autolock->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pbottom->addChild(autolock);
    
    auto pmenu = Button::create("game_res/bt_menu_0.png", "game_res/bt_menu_1.png");
    pmenu->setPosition(Vec2(1044, 25));
    pmenu->setTag(Tag_BT_Menu);
    pmenu->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pbottom->addChild(pmenu);
    
//    auto trumpet = Button::create("game_res/trumpet_0.png", "game_res/trumpet_1.png");
//    trumpet->setPosition(Vec2(80, 225));
//    trumpet->setTag(Tag_BT_Chat_OnOff);
//    trumpet->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
//    this->addChild(trumpet,10);
//    
//    auto bt_send = Button::create("game_res/bt_send_0.png", "game_res/bt_send_1.png");
//    bt_send->setPosition(Vec2(350, 225));
//    bt_send->setTag(Tag_Bt_Send_Chat);
//    bt_send->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
//    this->addChild(bt_send,11);
//    
//    inputField = EditBox::create(Size(225,48), "game_res/imputfield.png");
//    inputField->setAnchorPoint(cocos2d::Point(0.f,0.5f));
//    inputField->setPosition(cocos2d::Point(130,225));
//    //m_LoginEdit[0]->setMaxLength(LEN_ACCOUNTS);
//    inputField->setFontSize(16);
//    inputField->setPlaceHolder("");
//    inputField->setDelegate(this);
//    this->addChild(inputField,10);
}

void GameLayer::multipleinit()
{
    
}

void GameLayer::onGameScene(void *pData, WORD wSize)
{
    if(GameDataMgr::getInstance()->m_gameMessage)
        return;
    HallDataMgr::getInstance()->m_bStartGame = true;
    int nsize = sizeof(GameScene);
    //if(wSize!=sizeof(GameScene)) return;
    auto presult = (GameScene *)pData;
    GameDataMgr::getInstance()->m_sceneData = *presult;
    int multipeArray[6] = {1,10,100,500,1000,5000};
    for (int i = 0; i < 6; i++) {
        GameDataMgr::getInstance()->m_sceneData.nMultipleValue[i] = multipeArray[i];
    }
    GameScene gs = GameDataMgr::getInstance()->m_sceneData;
    GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
    GameDataMgr::getInstance()->initgameMessage();
}

void GameLayer::menuinit()
{
    m_menu = Layout::create();
    m_menu->setContentSize(cocos2d::Size(182, 380));
    m_menu->setPosition(Vec2(WinSize.width-92, 54));
    m_menu->setAnchorPoint(Vec2(0.5f, 1.f));
    this->addChild(m_menu,Tag_UI_Layer-1);
    m_menu->runAction(MoveBy::create(0.17f, Vec2(0.f, 380)));
    
    auto pbuttonbg = Button::create("game_res/im_bt_frame.png", "game_res/im_bt_frame.png");
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(cocos2d::Size(pbuttonbg->getContentSize().width, 380));
    pbuttonbg->setPosition(m_menu->getContentSize()/2);
    
    m_menu->addChild(pbuttonbg);
    
    auto pbank = Button::create("game_res/bt_bank_0.png", "game_res/bt_bank_1.png");
    pbank->setPosition(Vec2(m_menu->getContentSize().width/2, m_menu->getContentSize().height-52));
    pbank->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    pbank->setTag(Tag_BT_Bank);
    m_menu->addChild(pbank);
    
    auto phelp = Button::create("game_res/bt_help_0.png", "game_res/bt_help_1.png");
    phelp->setPosition(Vec2(m_menu->getContentSize().width/2, pbank->getPositionY()-85));
    phelp->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    phelp->setTag(Tag_BT_Help);
    m_menu->addChild(phelp);
    
    auto psetting = Button::create("game_res/bt_setting_0.png", "game_res/bt_setting_1.png");
    psetting->setPosition(Vec2(m_menu->getContentSize().width/2, pbank->getPositionY()-85*2));
    psetting->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    psetting->setTag(Tag_BT_Setting);
    m_menu->addChild(psetting);
    
    auto pclear = Button::create("game_res/bt_clearing_0.png", "game_res/bt_clearing_1.png");
    pclear->setPosition(Vec2(m_menu->getContentSize().width/2, pbank->getPositionY()-85*3));
    pclear->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    pclear->setTag(Tag_Bt_Clearing);
    m_menu->addChild(pclear);
}

void GameLayer::helplayerinit()
{
    auto player = GameHelpLayer::create();
    this->addChild(player,42+1,TAG_Help_Layer);
}

void GameLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
    m_nSecondCount = 60;
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_BT_Menu:
                {
                    m_nSecondCount = 60;
                    if (m_menu) {
                        m_menu->removeFromParent();
                        m_menu = nullptr;
                    }
                    else
                    {
                        this->menuinit();
                    }
                }
                    break;
                case Tag_Shoot:
                {
                    
                    GameDataMgr::getInstance()->m_autoshoot = !GameDataMgr::getInstance()->m_autoshoot;
                    if (GameDataMgr::getInstance()->m_autoshoot) {
                        GameDataMgr::getInstance()->m_autolock = false;
                    }
                    GameDataMgr::getInstance()->m_fishIndex = INT_MAX;
                    this->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot);
                    this->setAutoLock(!GameDataMgr::getInstance()->m_autolock);
                    
                    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
                    if (pcannon) {
                        pcannon->removeChildByTag(CANNON_LOCK_TAG);
                        pcannon->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot|GameDataMgr::getInstance()->m_autolock);
                    }
                }
                    break;
                case Tag_Lock:
                {
                    GameDataMgr::getInstance()->m_autolock = !GameDataMgr::getInstance()->m_autolock;
                    if (GameDataMgr::getInstance()->m_autolock) {
                        GameDataMgr::getInstance()->m_autoshoot = false;
                    }
                    else
                    {
                       
                        
                        GameDataMgr::getInstance()->m_fishIndex = INT_MAX;
                    }
                    this->setAutoLock(GameDataMgr::getInstance()->m_autolock);
                    this->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot);
                    
                    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
                    if (pcannon) {
                        pcannon->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot|GameDataMgr::getInstance()->m_autolock);
                        if(GameDataMgr::getInstance()->m_autolock)
                        {
                            
                        }
                        else
                        {
                            pcannon->removeChildByTag(CANNON_LOCK_TAG);
                        }
                    }
                }
                    break;
                case Tag_BT_Multiple:
                {
                    GameDataMgr *gdm = GameDataMgr::getInstance();
                    WORD myChiarId = HallDataMgr::getInstance()->m_wChairID;
                    GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[myChiarId]++;
                    GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[myChiarId] %= 6;
                    //GameDataMgr::getInstance()->m_sceneData.nMultipleValue[GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[0]]);
                    auto pmultiple = static_cast<Label *>(m_uiLayer->getChildByTag(1)->getChildByTag(Tag_Game_Multiple));
                    pmultiple->setString(__String::createWithFormat("%d:", GameDataMgr::getInstance()->m_sceneData.nMultipleValue[GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[myChiarId]])->getCString());
                    CMD_C_Multiple multiple;
                    multiple.nMultipleIndex = GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[myChiarId];
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_MULTIPLE, &multiple, sizeof(CMD_C_Multiple));
                    if (m_uiLayer) {
                        m_uiLayer->getChildByTag(1)->removeChildByTag(Tag_Multiple_Tip);
                    }
                }
                    break;
                case Tag_Bt_Send_Chat:
                {
                    CMD_GF_C_UserChat cmd_gf_cuc;
                    
                    std::string inputString(inputField->getText());
                    cmd_gf_cuc.wChatLength = inputString.length();
                    cmd_gf_cuc.dwChatColor = 0;
                    cmd_gf_cuc.dwTargerUserID = HallDataMgr::getInstance()->m_dwUserID;
                    memset(cmd_gf_cuc.szChatString, 0, sizeof(cmd_gf_cuc.szChatString));
                    //WHConverUft8ToUnicodeWithArray(cmd_gf_cuc.szChatString,inputString.c_str());
                    UTF8Str_To_UTF16Str_BYTE((BYTE *)inputString.c_str(), cmd_gf_cuc.szChatString);
                    //NetworkMgr::getInstance()->sendChat(cmd_gf_cuc);
                    int nSize = sizeof(cmd_gf_cuc)-sizeof(cmd_gf_cuc.szChatString)+cmd_gf_cuc.wChatLength*2;
                    NetworkMgr::getInstance()->sendData(MDM_GF_FRAME, SUB_GF_USER_CHAT, &cmd_gf_cuc, nSize);
                    inputField->setText("");
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

void GameLayer::menutouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
        m_nSecondCount = 60;
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_Bt_Clearing:
                {
                    if(!this->getChildByTag(TAG_Clear_Layer))
                    {
                        auto player = GameClearLayer::create();
                        this->addChild(player,Tag_UI_Layer+1,TAG_Clear_Layer);
                    }
                    
//                    CMD_GR_UserStandUp request;
//                    memset(&request, 0, sizeof(request));
//                    
//                    request.wTableID = HallDataMgr::getInstance()->m_wTableID;
//                    request.wChairID = HallDataMgr::getInstance()->m_wChairID;
//                    request.cbForceLeave = true;
//                    
//                    NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));

                }
                    break;
                case Tag_BT_Bank:
                {
                    auto player = GameBankLayer::create();
                    this->addChild(player,Tag_UI_Layer+1);
                }
                    break;
                case Tag_BT_Setting:
                {
                    auto player = GameSettingLayer::create();
                    this->addChild(player,Tag_UI_Layer+1);
                }
                    break;
                case Tag_BT_Help:
                {
                    this->helplayerinit();
                }
                    break;
                default:
                    break;
            }
            m_menu->removeFromParent();
            m_menu = nullptr;
        }
            break;
        default:
            break;
    }
    
}

void GameLayer::setAutoShoot(bool b)
{
    
    auto pbutton = (Button *)m_uiLayer->getChildByTag(1)->getChildByTag(Tag_Shoot);
    if (GameDataMgr::getInstance()->m_autoshoot) {
        pbutton->loadTextures("game_res/bt_check_yes.png", "game_res/bt_check_no.png");
    }
    else
    {
        pbutton->loadTextures("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    }
}

void GameLayer::setAutoLock(bool b)
{
    auto pbutton = (Button *)m_uiLayer->getChildByTag(1)->getChildByTag(Tag_Lock);
    if (GameDataMgr::getInstance()->m_autolock) {
        pbutton->loadTextures("game_res/bt_check_yes.png", "game_res/bt_check_no.png");
    }
    else
    {
        pbutton->loadTextures("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    }
}

void GameLayer::onEnterTransitionDidFinish()
{
    NetworkMgr::getInstance()->sendRequestChairUserInfo(HallDataMgr::getInstance()->m_wTableID, INVALID_CHAIR);
    Layer::onEnterTransitionDidFinish();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Room_LeaveGame, CC_CALLBACK_1(GameLayer::eventLeaveGame, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(GameLayer::eventScoreChange, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Auto_Change, CC_CALLBACK_1(GameLayer::eventAutoChange, this)), 1);
    m_contact = EventListenerPhysicsContact::create();
    m_contact->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    m_contact->onContactPreSolve = CC_CALLBACK_2(GameLayer::onContactPreSolve, this);
    m_contact->onContactPostSolve = CC_CALLBACK_2(GameLayer::onContactPostSolve, this);
    m_contact->onContactSeparate = CC_CALLBACK_1(GameLayer::onContactSeperate, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_contact, this);
    GameDataMgr* gdm = GameDataMgr::getInstance();

    GameDataMgr::getInstance()->m_gameMessage->setLayer(this);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(__String::createWithFormat("sound_res/MUSIC_BACK_0%d.mp3", GameDataMgr::getInstance()->m_sceneData.cbBackIndex+1)->getCString(), true);
}

void GameLayer::onExit()
{
    NetworkMgr::getInstance()->m_GameScenecallback = nullptr;
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/LOAD_BACK.mp3",true);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Room_LeaveGame);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Auto_Change);
    GameDataMgr::getInstance()->clearGameData();
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_contact);
    HallDataMgr::getInstance()->m_bStartGame = false;
    AnimationCache::getInstance()->destroyInstance();
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    Layer::onExit();
}

void GameLayer::update(float delta)
{
    GameDataMgr *gdm = GameDataMgr::getInstance();
    cocos2d::Vector<Fish *> fishlist;
    for (auto iter : GameDataMgr::getInstance()->m_waitList) {
        if (iter.second->isCanAddtoScene()) {
            fishlist.pushBack(iter.second);
        }
    }
    int addnum = 0;
    for (auto iter : fishlist) {
        addnum++;
        if (addnum>10) {
            break;
        }
        GameDataMgr::getInstance()->m_waitList.erase(iter->getFishData()->nFishKey);
        iter->initAnim();
        if (false/*iter->getFishData()->bKiller || iter->getFishData()->bSpecial*/) {
            m_fishLayer->addChild(iter,iter->getFishData()->nFishType+1);
        }
        else
        {
            m_fishLayer->addChild(iter,iter->getFishData()->nFishType);
        }
        iter->initphysicsBody();
        GameDataMgr *gdm = GameDataMgr::getInstance();
        GameDataMgr::getInstance()->m_fishList.insert(iter->getFishData()->nFishKey, iter);
        Fish * pFish = &(*iter);
       GameDataMgr::getInstance()->m_FishArray->addObject(pFish);
        //iter->scheduleUpdate();
    }
    fishlist.clear();
    
    if (GameDataMgr::getInstance()->m_autolock) {
        if (GameDataMgr::getInstance()->m_fishIndex == INT_MAX) {
            GameDataMgr::getInstance()->m_fishIndex = GameDataMgr::getInstance()->selectMaxFish();
        }
        else
        {
            auto pfish = GameDataMgr::getInstance()->m_fishList.at(GameDataMgr::getInstance()->m_fishIndex);
            auto rect = cocos2d::Rect(0, 0, WinSize.width, WinSize.height);
            if (!pfish || !rect.containsPoint(pfish->getPosition())) {
                GameDataMgr::getInstance()->m_fishIndex = GameDataMgr::getInstance()->selectMaxFish();
            }
        }
    }
}


void GameLayer::showCoin(LONGLONG score, DWORD wchairID, cocos2d::Vec2 vec2, BYTE nfishtype)
{
    //return;
    int myChiarId = HallDataMgr::getInstance()->m_wChairID;
    int nMyNum = myChiarId/3;//自己所在的一排
    int nPlayerNum = wchairID/3;
    
    __String *str = NULL;
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(COINFLY);
    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(wchairID);
    if (!pcannon) {
        return;
    }
    Animation *panim = nullptr;
    int coinnum = 0;
    std::string framstr;
    float distant = 50.f;
    
    if(nfishtype<5)
    {
        panim = AnimationCache::getInstance()->getAnimation(SilverAnim);
        framstr = "silver_coin_0.png";
        coinnum = silverNumArrary[nfishtype];
    }
    else if(nfishtype>=5&&nfishtype<17)
    {
        panim = AnimationCache::getInstance()->getAnimation(GoldAnim);
        framstr = "gold_coin_0.png";
        coinnum = goldNumArrary[nfishtype];
    }
  
    float posXArray[10];
    float initX = -105;
    posXArray[0] = initX;
    for (int i = 1; i<10 ; ++i) {
        posXArray[i] = initX - i*39;
    }
    //if(false)
    if(panim)
    {
        auto paction = RepeatForever::create(Animate::create(panim));
        
        if (coinnum == 0) {
            coinnum = 1;
        }
        if (coinnum > 10) {
            coinnum = 10;
        }
        //coinnum =10+rand()%7;
        auto playout = Layout::create();
        playout->setPosition(vec2);
        playout->setContentSize(cocos2d::Size(distant*5,distant+40));
        if (coinnum>5) {
            playout->setContentSize(cocos2d::Size(distant*5,distant*2+40));
        }
        playout->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_CannonLayer->addChild(playout,1);
        
        auto ptext = TextAtlas::create(__String::createWithFormat("%lld",score)->getCString(), "game_res/num_game_gold.png", 37, 34, "0");
        ptext->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-20));
        playout->addChild(ptext);
        ptext->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]{ptext->removeFromParent();})));
        
        int secondnum = coinnum>5?coinnum/2:coinnum;
        auto playout1 = Layout::create();
        playout1->setContentSize(cocos2d::Size(distant*secondnum,distant));
        playout1->setAnchorPoint(Vec2(0.5f, 0.5f));
        playout1->setPosition(Vec2(playout->getContentSize().width/2, distant/2));
        playout->addChild(playout1);
        for (int index=0; index<secondnum; ++index) {
            auto pcoin = Sprite::createWithSpriteFrameName(framstr);
            auto pscale = distant/pcoin->getContentSize().width;
            pcoin->setScale(pscale);
            pcoin->runAction(paction->clone());
            pcoin->setPosition(Vec2(distant/2+index*distant, distant/2));
            playout1->addChild(pcoin);
        }
        
        if (coinnum > 5) {
            int firstnum = coinnum-secondnum;//coinnum-5;
            auto playout2 = Layout::create();
            playout2->setContentSize(cocos2d::Size(distant*firstnum,distant));
            playout2->setAnchorPoint(Vec2(0.5f, 0.5f));
            playout2->setPosition(Vec2(playout->getContentSize().width/2, distant*3/2));
            playout->addChild(playout2);
            for (int index=0; index<firstnum; ++index) {
                auto pcoin = Sprite::createWithSpriteFrameName(framstr);
                auto pscale = distant/pcoin->getContentSize().width;
                pcoin->setScale(pscale);
                pcoin->runAction(paction->clone());
                pcoin->setPosition(Vec2(distant/2+index*distant, distant/2));
                playout2->addChild(pcoin);
            }
        }
        
        auto position = pcannon->getCannonPoint();
        
        playout->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 40.f)), MoveTo::create(0.5f, position),
                                            CallFunc::create([=]{
            playout->removeFromParent();
            //加分
//            auto pcannon1 = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(wchairID);
//            if (pcannon1) {
//                HallDataMgr* hdm = HallDataMgr::getInstance();
//                auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
//                if (puser) {
//                    puser->m_date.lScore += score;
//                    pcannon->setScore(puser->m_date.lScore);
//                }
//            }
        }),  NULL));
        auto paction1 = Sequence::createWithTwoActions(DelayTime::create(1.2f), FadeTo::create(0.3f, 0));
        for (auto iter : playout->getChildren()) {
            for (auto iter1 : iter->getChildren()) {
                iter1->runAction(paction1->clone());
            }
        }
    }
    //else
    //if(wchairID == HallDataMgr::getInstance()->m_wChairID)
    {
        m_bBannerColor = !m_bBannerColor;
        m_nGoldBannerNum++;
        
        auto pcannon1 = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(wchairID);
        if (pcannon1)
        {
            float Angle = 70.0f;
            float time = 0.12f;
            float moveY = 30.0f;
            Vec2 pos = pcannon1->getPosition();
            
            if(nfishtype >= FishType_JianYu && nfishtype <= FishType_LiKui)
            {
                auto goldCircle = (Sprite*)this->getChildByTag(Tag_Gold_Circle+wchairID);
                if(!goldCircle)
                {
                goldCircle = Sprite::create("game_res/goldCircle.png");
                 this->addChild(goldCircle,6,Tag_Gold_Circle+wchairID);
                 goldCircle->runAction(Sequence::createWithTwoActions(RotateBy::create(time*18,360*1.3f)
                                                                 ,CallFunc::create([=]{goldCircle->removeFromParentAndCleanup(true);})
                                                                 ));
                }
                 
                auto pCircleText = (LabelAtlas*)this->getChildByTag(Tag_Gold_Circle_Text+wchairID);

                if(!pCircleText)
                {
                    pCircleText = LabelAtlas::create(__String::createWithFormat("%lld",score)->getCString(),"game_res/mutipleNum.png",14,17,'0');
                    pCircleText->setAnchorPoint(Vec2(0.5f, 0.5f));           
                    this->addChild(pCircleText,7,Tag_Gold_Circle_Text+wchairID);
                    auto actionSe1 =  Sequence::create(RotateTo::create(time*2,Angle)
                                               ,RotateTo::create(time*4,-Angle)
                                               ,RotateTo::create(time*2, 0)
                                               , NULL);
                    pCircleText->runAction(Sequence::createWithTwoActions(Repeat::create(actionSe1, 4)
                                                                  ,CallFunc::create([=]{pCircleText->removeFromParentAndCleanup(true);})                                                                  ));
                }
                if(nMyNum == nPlayerNum)
                {
                    goldCircle->setPosition(Vec2(pos.x-80,pos.y+158));
                    pCircleText->setPosition(Vec2(pos.x-80,pos.y+153));
                }
                else
                {
                    goldCircle->setPosition(Vec2(pos.x-75,pos.y-160));
                    pCircleText->setPosition(Vec2(pos.x-75,pos.y-155));
                }
            }
        

            
            
            
            auto ptext = LabelAtlas::create(__String::createWithFormat("%lld",score)->getCString(),"game_res/mutipleNum.png",14,17,'0');
            ptext->setTag(1);
            ptext->setAnchorPoint(Vec2(0.5f, 0.5f));
           
            
            auto goldBanner = Scale9Sprite::create("game_res/gold_banner.png"
                                                   ,Rect(0,18*m_bBannerColor,75,18)
                                                   ,Rect(5,5,65,8));
            
            goldBanner->setContentSize(Size(ptext->getContentSize().width+10,goldBanner->getContentSize().height));
            
            goldBanner->setTag(BANNER_TAG);
            //this->addChild(goldBanner,19);
            //ptext->setPosition(goldBanner->getContentSize()/2);
            //goldBanner->addChild(ptext);
            
            auto golds = Sprite::create("game_res/golds.png", Rect(0,0,38,4*7));
            
            golds->setTag(GOLDS_TAG);
                        goldBanner->addChild(golds,19);
            
            if(nMyNum == nPlayerNum)
            {
                goldBanner->setPosition(Vec2(pos.x+posXArray[pcannon1->m_goldlist.size()+1],pos.y-27));
                golds->setPosition(Vec2(goldBanner->getContentSize().width/2,-15));
            }
            else
            {
                moveY = -30.0f;
                goldBanner->setPosition(Vec2(pos.x+posXArray[pcannon1->m_goldlist.size()+1],pos.y+30));
                golds->setPosition(Vec2(goldBanner->getContentSize().width/2,27));
            }
            
            auto seAction = Sequence::create(MoveBy::create(0.5f, Vec2(0,moveY)),DelayTime::create(0.3f)/*,FadeOut::create(0.2f)*/, NULL);
            
//            goldBanner->runAction(Sequence::createWithTwoActions((FiniteTimeAction*)seAction->clone()
//                                                                 ,CallFunc::create([=]{
//                
//                if(goldBanner->getTag()==BANNER_TAG+1)
//                {
//                    goldBanner->removeFromParentAndCleanup(true);
//                    moveAllBannerAndGolds();
//                }
//                
//                
//                
//            })
//                                                                 ));
            
//            golds->runAction(Sequence::createWithTwoActions((FiniteTimeAction*)seAction->clone()
//                                                            ,CallFunc::create([=]{
//                if(golds->getTag()==GOLDS_TAG+1)
//                {
//                    golds->removeFromParentAndCleanup(true);
//                }
//            })
//                                                            ));

            int index = pcannon1->m_goldlist.size();
            //while (index >= 3)
            //if(!m_goldlist.empty())
            if(index >= 3)
            {
                char str[255];
                auto pnode = pcannon1->m_goldlist.at(2);
                LabelAtlas* pText = (LabelAtlas*)pnode->getChildByTag(GOLDS_TAG)->getChildByTag(1);
                LONGLONG value = 0;
                if(pText)
                {
                    sprintf(str,"%s",pText->getString().c_str());
                    scanf(str,"%lld",&value);
                    value += score;
                    sprintf(str,"%lld",value);
                    pText->setString(str);
                }
                
//                for (auto iter : pcannon1->m_goldlist) {
//                    iter->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(39,0)), CallFunc::create([=]{
//                        pcannon1->moveAllBannerAndGolds();
//                    }),NULL));
//
//                    
//                }
                //index = pcannon1->m_goldlist.size();
            }
            else
            {
                this->addChild(goldBanner,19);
                ptext->setPosition(goldBanner->getContentSize()/2);
                goldBanner->addChild(ptext);

                pcannon1->m_goldlist.pushBack(goldBanner);
                
                goldBanner->runAction(Sequence::create((FiniteTimeAction*)seAction->clone(), CallFunc::create([=]{
                    if(pcannon1->m_goldlist.size()&&(pcannon1->m_goldlist.size() == 1 || goldBanner == pcannon1->m_goldlist.at(0)))
                    {
                        goldBanner->removeFromParent();
                        pcannon1->m_goldlist.eraseObject(goldBanner);
                    }
                    
                    if (pcannon1->m_goldlist.size()) {
                        for (auto iter : pcannon1->m_goldlist) {
                            iter->runAction(Sequence::create(MoveBy::create(0.35f, Vec2(39,0)), CallFunc::create([=]{
                                Node* pNode = &(*iter);
                                pcannon1->moveAllBannerAndGolds(pNode);
                            }),NULL));
                        }
                    }}), NULL));
                

            }
                        //m_nGoldBannerNum++;
           
            
            //加分
            HallDataMgr* hdm = HallDataMgr::getInstance();
            auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
            if (puser) {
                puser->m_date.lScore += score;
                pcannon->setScore(puser->m_date.lScore);
            }
        }
    }
    
}

void GameLayer::showLight(BYTE fishType, DWORD fishIndex)
{
    cocos2d::Vector<Fish *>fishlist;
    auto pfish = GameDataMgr::getInstance()->m_fishList.at(fishIndex);
    if (!pfish) {
        return;
    }
    auto rect = Rect(0, 0, WinSize.width, WinSize.height);
    for (auto iter : GameDataMgr::getInstance()->m_fishList) {
        if (iter.second->getFishData()->nFishType==fishType && iter.second->getFishData()->nFishKey!=fishIndex) {
            if (rect.containsPoint(iter.second->getPosition())) {
                fishlist.pushBack(iter.second);
            }
        }
    }
    if (!fishlist.size()) {
        return;
    }
    auto fishpoint = pfish->getPosition();
    if (GameDataMgr::getInstance()->m_reversal) {
        fishpoint = Vec2(WinSize.width, WinSize.height) - fishpoint;
    }
    this->showBall(fishpoint);
    for (auto iter : fishlist) {
        auto point = iter->getPosition();
        if (GameDataMgr::getInstance()->m_reversal) {
            point = Vec2(WinSize.width, WinSize.height) - point;
        }
        this->showBall(point);
        this->showLight(fishpoint, point);
    }
}

void GameLayer::showBall(cocos2d::Vec2 &point)
{
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    auto pball1 = Sprite::create("game_res/fish_bomb_ball.png", cocos2d::Rect(0, 0, 70, 70));
    pball1->setPosition(point);
    pball1->setColor(Color3B(255, 255, 0));
    pball1->setOpacity(255);
    pball1->setBlendFunc(blendFunc);
    this->addChild(pball1,Tag_Cannon_Layer);
    
    auto pball2 = Sprite::create("game_res/fish_bomb_ball.png", cocos2d::Rect(0, 0, 70, 70));
    pball2->setPosition(point);
    pball2->setColor(Color3B(0,215,255));
    pball2->setOpacity(150);
    pball2->setBlendFunc(blendFunc);
    this->addChild(pball2,Tag_Cannon_Layer);
    
    auto panim = Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation(FishBall)), 3);
    auto paction = Sequence::createWithTwoActions(panim, CallFuncN::create([=](Node *pnode){pnode->removeFromParent();}));
    pball1->runAction(paction);
    pball2->runAction(paction->clone());
}

void GameLayer::showLight(cocos2d::Vec2 &point1, cocos2d::Vec2 &point2)
{
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    auto distant = point1.distance(point2);
    auto pscaley = distant/256.f;
    auto angle = GameDataMgr::getInstance()->getAngleByTwoPoint(point1, point2);
    auto plight1 = Sprite::create("game_res/fish_bomb_light.png", cocos2d::Rect(0, 0, 60, 256));
    plight1->setPosition((point1+point2)/2);
    plight1->setColor(Color3B(255, 255, 0));
    plight1->setOpacity(255);
    plight1->setRotation(angle);
    plight1->setScaleY(pscaley);
    plight1->setBlendFunc(blendFunc);
    this->addChild(plight1,Tag_Cannon_Layer);
    
    auto plight2 = Sprite::create("game_res/fish_bomb_light.png", cocos2d::Rect(0, 0, 60, 256));
    plight2->setPosition((point1+point2)/2);
    plight2->setColor(Color3B(0,215,255));
    plight2->setOpacity(150);
    plight2->setRotation(angle);
    plight1->setScaleY(pscaley);
    plight2->setBlendFunc(blendFunc);
    this->addChild(plight2,Tag_Cannon_Layer);
    
    auto panim = Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation(FishLight)), 5);
    auto paction = Sequence::createWithTwoActions(panim, CallFuncN::create([=](Node *pnode){pnode->removeFromParent();}));
    plight1->runAction(paction);
    plight2->runAction(paction->clone());
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

void GameLayer::showTips(CMD_S_AwardTip *ptip)
{
    cocos2d::Vector<Label *>labellist;
    std::string tipstr;
    std::string tipstr1;
    std::string tipstr2;
    if (ptip->nFishMultiple >= 50) {
        if (ptip->nScoreType == EST_Cold) {
            tipstr = __String::createWithFormat("捕中了%s,获得", FishName[ptip->nFishType])->getCString();
        }
        else if(ptip->nScoreType == EST_Laser)
        {
            tipstr = "使用激光，获得";
        }
//        else if (ptip->nScoreType == EST_Bomb)
//        {
//            tipstr = "击中宝箱炸弹，获得";
//        }
//        else if (ptip->nScoreType == EST_Kill)
//        {
//            tipstr = "击中炸弹鱼，获得";
//        }
        tipstr1 = __String::createWithFormat("%d倍 %lld分数！",ptip->nFishMultiple, ptip->lFishScore)->getCString();
        if (ptip->nFishMultiple > 500) {
            tipstr2 = "超神了！！！";
        }
        else if (ptip->nFishType == 19)
        {
            tipstr2 = "运气爆表！！！";
        }
        else
        {
            tipstr2 = "实力超群！！！";
        }
        auto pname = WHConverUnicodeToUtf8WithArray(ptip->szPlayName);
        char text[255];
        string tableStr;
        if(ptip->wTableID == HallDataMgr::getInstance()->m_wTableID)
            tableStr = "本桌玩家";
        else
        {
            sprintf(text,"第%d桌玩家",ptip->wTableID+1);
            tableStr = text;
        }
        auto plabel1 = Label::createWithSystemFont(tableStr.c_str(), FontNormal, 20);
        plabel1->setColor(Color3B::YELLOW);
        plabel1->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel2 = Label::createWithSystemFont(pname.c_str(), FontNormal, 20);
        plabel2->setColor(Color3B::RED);
        plabel2->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel3 = Label::createWithSystemFont(tipstr, FontNormal, 20);
        plabel3->setColor(Color3B::YELLOW);
        plabel3->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel4 = Label::createWithSystemFont(tipstr1, FontNormal, 20);
        plabel4->setColor(Color3B::RED);
        plabel4->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel5 = Label::createWithSystemFont(tipstr2, FontNormal, 20);
        plabel5->setColor(Color3B::YELLOW);
        plabel5->setAnchorPoint(Vec2(0.f, 0.5f));
        
        labellist.pushBack(plabel1);
        labellist.pushBack(plabel2);
        labellist.pushBack(plabel3);
        labellist.pushBack(plabel4);
        labellist.pushBack(plabel5);
    }
    else
    {
        auto plabel1 = Label::createWithSystemFont("恭喜你捕中了补给箱，获得", FontNormal, 20);
        plabel1->setColor(Color3B::YELLOW);
        plabel1->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel2 = Label::createWithSystemFont(__String::createWithFormat("%d倍 %lld分数！",ptip->nFishMultiple, ptip->lFishScore)->getCString(), FontNormal, 20);
        plabel2->setColor(Color3B::RED);
        plabel2->setAnchorPoint(Vec2(0.f, 0.5f));
        
        labellist.pushBack(plabel1);
        labellist.pushBack(plabel2);
    }
    
    float length = 60;
    for (auto iter : labellist) {
        iter->setPosition(Vec2(length-30, 20));
        length += iter->getContentSize().width + 5;
    }
    
    auto pbg = ImageView::create("game_res/clew_box.png");
    pbg->setScale9Enabled(true);
    pbg->setContentSize(cocos2d::Size(length, 40));
    pbg->setScaleX(0.1f);
    for (auto iter : labellist)
    {
        pbg->addChild(iter);
    }
    this->showTips(pbg);
    
    labellist.clear();
    
    //auto plabel3 = Label::createWithSystemFont(<#const std::string &text#>, <#const std::string &font#>, <#float fontSize#>)
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

void GameLayer::showBomb(Cannon *pcannon)
{
    auto position = pcannon->getPosition();
    auto function = [=]{
        auto positionx = random(-150.f, 150.f);
        auto positiony = random(-150.f, 150.f);
        auto pbomb = Sprite::createWithSpriteFrameName("bomb_0.png");
        pbomb->setPosition(position + Vec2(positionx, positiony));
        pbomb->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(BombAnim)));
        this->addChild(pbomb,Tag_Fish_Layer);
    };
    auto funcaction = CallFunc::create(function);
    this->runAction(Repeat::create(Sequence::createWithTwoActions(funcaction, DelayTime::create(0.048)), 8));
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

void GameLayer::eventLeaveGame(cocos2d::EventCustom *event)
{
    Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, DeskLayer::createScene()));
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
        scorestr = getGoldStr(scorestr,',');
        if (scorestr.length() > 16) {
            scorestr = scorestr.substr(0,14);
            scorestr.append("::");
        }
        pscore->setString(scorestr);
    }
}

void GameLayer::eventAutoChange(cocos2d::EventCustom *event)
{
    this->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot);
    this->setAutoLock(!GameDataMgr::getInstance()->m_autolock);
}

void GameLayer::messageFire(CMD_S_Fire *fire)
{
    if (fire->wChairID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto pcannonlayer = (CannonLayer *)m_CannonLayer;
    auto pcannon = pcannonlayer->getCannonbypos(fire->wChairID);
    if (pcannon) {
        pcannon->shoot(fire);
    }
}

void GameLayer::messageMULTIPLE(void *pData, WORD wSize)
{
    int structSize=sizeof(CMD_S_Multiple);
    CMD_S_Multiple *cmd_sm = (CMD_S_Multiple*)pData;
    int count=wSize/structSize;
    auto pcannonlayer = (CannonLayer *)m_CannonLayer;
    auto pcannon = pcannonlayer->getCannonbypos(cmd_sm->wChairID);
    int nScore = GameDataMgr::getInstance()->m_sceneData.nMultipleValue[cmd_sm->nMultipleIndex];
    if(pcannon)
    {
        pcannon->setMultiple(nScore);
    }
    GameDataMgr::getInstance()->m_sceneData.nMultipleIndex[cmd_sm->wChairID] = cmd_sm->nMultipleIndex;
}
    
void GameLayer::messageCatchFish(void *pData, WORD wSize)
{
    int structSize=sizeof(CMD_S_CatchFish);
    int count=wSize/structSize;
    bool showlight = false;
    
    for(int index=0; index<count; ++index)
    {
        auto pdata1 = (char *)pData+index*structSize;
        auto fishdata = (CMD_S_CatchFish *)pdata1;
        
        auto pfish = GameDataMgr::getInstance()->m_fishList.at(fishdata->nFishIndex);
        if (pfish)
        {
            //水浒传 等待鱼出场时间也延迟5秒
            if(pfish->getFishData()->nFishType == FishType_ShuiHuZhuan)
            {
                for (auto iter : GameDataMgr::getInstance()->m_waitList) {
                    iter.second->setProductTime(iter.second->getProductTime()+5000);
                }
            }
            BYTE nfishtype = pfish->getFishData()->nFishType;
            GameDataMgr::getInstance()->m_sceneData.lPlayFishCount[fishdata->wChairID][nfishtype] += 1;
//            if (pfish->getFishData()->bKiller && !showlight) {
//                showlight = true;
//                this->showLight(nfishtype, fishdata->nFishIndex);
//            }
//            if (pfish->getFishData()->bSpecial) {
//                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/CoinLightMove.wav");
//            }
            if (nfishtype == FISH_ALL_COUNT-1 ) {
                auto m_data = pfish->getFishData();
                GameDataMgr::getInstance()->m_FishArray->removeObject(pfish);
                pfish->removeFromParent();
                GameDataMgr::getInstance()->m_fishList.erase(fishdata->nFishIndex);
                if(m_data->nFishState == FishState_King)
                {
                    GameDataMgr::getInstance()->m_fishKingList.erase(m_data->nFishKey);
                }
               
                return;
            }
            
            string strFormat1;
            string strFormat2;
            string strFormat3;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            strFormat1 = "sound_res/small_%d.ogg";
            strFormat2 = "sound_res/beauty_%d.ogg";
            strFormat3 = "sound_res/big_%d.ogg";
#else
            strFormat1 = "sound_res/small_%d.wav";
            strFormat2 = "sound_res/beauty_%d.wav";
            strFormat3 = "sound_res/big_%d.wav";
#endif
            
            
            
            if (nfishtype < FISH_KING_MAX) {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat(strFormat1.c_str(),rand()%6)->getCString());
            }
            else if (nfishtype == FISH_MEI_REN_YU)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat(strFormat2.c_str(),rand()%4)->getCString());
            }
            else
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat(strFormat3.c_str(),nfishtype)->getCString());
            }
            
            auto position = pfish->getPosition();
            if (GameDataMgr::getInstance()->m_reversal) {
                position = Vec2(WinSize.width, WinSize.height) - position;
            }
//            if (pfish->getFishData()->bSpecial) {
//                auto function = [=]{auto particleSystem = ParticleSystemQuad::create("game_res/particles_test1.plist");
//                    particleSystem->setPosition(position);
//                    particleSystem->setPositionType(ParticleSystem::PositionType::GROUPED);
//                    this->addChild(particleSystem,Tag_Fish_Layer);
//                };
//                this->runAction(Sequence::create(CallFunc::create(function), DelayTime::create(0.2f), CallFunc::create(function), DelayTime::create(0.25f), CallFunc::create(function), NULL));
//            }
            
            if (pfish->getFishData()->nFishType > 13) {
                auto particleSystem = ParticleSystemQuad::create("game_res/particles_test2.plist");
                particleSystem->setPosition(position);
                particleSystem->setPositionType(ParticleSystem::PositionType::GROUPED);
                this->addChild(particleSystem, Tag_Fish_Layer-1);
                
                auto funcaction = CallFunc::create([=]{
                    auto pcoin = CoinMove::create(position, Vec2(position.x, -100), rand()%9+3, ((rand()%((3)/2*100))+50.0)/100.0, rand()%2);
                    this->addChild(pcoin,Tag_Fish_Layer-1);
                });
                this->runAction(Repeat::create(Sequence::createWithTwoActions(funcaction, DelayTime::create(0.016f)), 36));
            }
            pfish->deadDeal();
            
            SCORE score = fishdata->lScoreCount;
            DWORD chairID = fishdata->wChairID;
            BYTE  fishtype = pfish->getFishData()->nFishType;
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]{this->showCoin(score, chairID, position, fishtype);})));
        }
        else
        {
            auto cannonLayer = (CannonLayer *)m_CannonLayer;
            auto pcannon = cannonLayer->getCannonbypos(fishdata->wChairID);
            if (pcannon) {
                auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
                if (puser) {
                    CCLOG("messageCatchFish lScore %lld lScoreCount %lld",puser->m_date.lScore,fishdata->lScoreCount);
                    puser->m_date.lScore += fishdata->lScoreCount;
                    pcannon->setScore(puser->m_date.lScore);
                }
                
            }
        }
        
        
    }
    
}

void GameLayer::messageFastMove()
{
    GameDataMgr::getInstance()->m_waitList.clear();
    for (auto iter : GameDataMgr::getInstance()->m_fishList) {
        iter.second->setFastMove();
    }
    GameDataMgr::getInstance()->m_fishList.clear();
    
    auto cannonLayer = (CannonLayer *)m_CannonLayer;
    auto pcannon = cannonLayer->m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        pcannon->fastDeal();
    }
//    if (GameDataMgr::getInstance()->m_autoshoot) {
//        
//    }
}

void GameLayer::messageExchangeScene(int index)
{
    GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
    // - GameDataMgr::getInstance()->getEnterTime());
    auto pbg = this->getChildByTag(Tag_BG);
    pbg->setTag(-1);
    pbg->runAction(Sequence::createWithTwoActions(FadeTo::create(2.5f, 0), CallFunc::create([=]{pbg->removeFromParent();})));
    
    GameDataMgr::getInstance()->m_sceneData.cbBackIndex = index;
    auto pbg1 = Sprite::create(__String::createWithFormat("game_res/game_bg_%d.png", index)->getCString());
    pbg1->setPosition(Vec2(WinSize.width/2, WinSize.height/2));
    pbg1->setOpacity(0);
    pbg1->setTag(Tag_BG);
    pbg1->runAction(FadeTo::create(5.f, 255));
    this->addChild(pbg1);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(__String::createWithFormat("sound_res/MUSIC_BACK_0%d.mp3", GameDataMgr::getInstance()->m_sceneData.cbBackIndex)->getCString(), true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHANGE_SCENE);
}


void GameLayer::messageSupplyTip(DWORD chairID)
{
    std::string tipstr;
    if (chairID == HallDataMgr::getInstance()->m_wChairID) {
        tipstr = "获得一个补给箱！击中可能获得大量奖励哟！赶快击杀！";
    }
    else
    {
        auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(chairID);
        if (pcannon) {
            auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
            if (puser) {
                tipstr = puser->m_nickname + " 获得了一个补给箱！羡慕吧，继续努力，你也可能得到！";
            }
        }
    }
    this->showTips(tipstr);
}

void GameLayer::messageSupply(void *pData, WORD wSize)
{
    auto presult = (CMD_S_Supply *)pData;
    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(presult->wChairID);
    if (pcannon) {
        pcannon->showSupply(presult);
        auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
        if (presult->nSupplyType == EST_Gift) {
            if (puser) {
                puser->m_date.lScore += presult->lSupplyCount;
                pcannon->setScore(puser->m_date.lScore);
            }
        }
        std::string tipstr("");
        switch (presult->nSupplyType) {
                //this->showBomb(pcannon);
            case EST_Laser:
            {
                if (presult->wChairID == HallDataMgr::getInstance()->m_wChairID) {
                    tipstr = puser->m_nickname + " 击中补给箱打出了激光！秒杀利器！赶快使用！";
                }
                else
                {
                    tipstr = puser->m_nickname + " 击中补给箱打出了激光！秒杀利器!";
                }
            }
                break;
//            case EST_Bomb:
//            {
//               tipstr = puser->m_nickname + " 击中补给箱打出了炸弹！轰！！！！！";
//                this->showBomb(pcannon);
//                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/combo.wav");
//            }
//                break;
            case EST_Speed:
            {
                tipstr = puser->m_nickname + " 击中补给箱打出了加速！所有子弹速度翻倍！";
            }
                break;
//            case EST_Strong:
//            {
//                tipstr = puser->m_nickname + " 击中补给箱打出了强化！所有鱼网范围扩大！";
//            }
//                break;
            case EST_Null:
            {
                if (presult->wChairID == HallDataMgr::getInstance()->m_wChairID)
                {
                    tipstr = "很遗憾！补给箱里面什么都没有！";
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SMASHFAIL);
                }
            }
                break;
            default:
                break;
        }
        if (tipstr != "") {
            this->showTips(tipstr);
        }
    }
}

void GameLayer::messageChat(void *pData, WORD wSize)
{
    CMD_GF_S_UserChat* cmd_gf_cuc = (CMD_GF_S_UserChat*) pData;
    int chatLength = cmd_gf_cuc->wChatLength;
    CannonLayer* cannonLayer = (CannonLayer*) m_CannonLayer;
    Cannon* pCannon = cannonLayer->m_cannonlist.at(cmd_gf_cuc->dwSendUserID);
    if(pCannon)
    {
        TCHAR ChatStr[LEN_USER_CHAT];
        memset(ChatStr, 0, sizeof(ChatStr));
        memcpy(ChatStr,(char *)cmd_gf_cuc->szChatString,chatLength);
        string chatString = WHConverUnicodeToUtf8WithArray(ChatStr);
        pCannon->setChatString(chatString);
    }
    
    //s->set
    
}

void GameLayer::messageSpeech(void *pData,WORD wSize)
{
    auto presult = (CMD_S_Speech *)pData;
    WORD wChiarId = presult->wChairID;
}

void GameLayer::messageAwardTip(void *pData, WORD wSize)
{
    auto presult = (CMD_S_AwardTip *)pData;
    //if (presult->wTableID != HallDataMgr::getInstance()->m_wTableID) {
    //    return;
    //}
    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(presult->wChairID);
    if (pcannon) {
        pcannon->showAwardTip(presult);
    }
    auto nscore = presult->nFishMultiple;
    if (nscore >= 50 || (presult->nFishType==19&&presult->nScoreType==EST_Cold&&presult->wChairID==HallDataMgr::getInstance()->m_wChairID) ) {
        this->showTips(presult);
    }
    
}

void GameLayer::messageBeginLaster(int charID)
{
    if (charID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto pcannon= static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(charID);
    if (pcannon) {
        pcannon->setCannonType(Laser_Cannon, 0);
    }
}

void GameLayer::messageLaster(CMD_S_Laser *presult)
{
    if (presult->wChairID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto point = Vec2(presult->ptPos.x, presult->ptPos.y);
    point = GameDataMgr::getInstance()->convertCoordinateSystem(point, 1, GameDataMgr::getInstance()->m_reversal);
    auto pcannon= static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(presult->wChairID);
    if (pcannon) {
        pcannon->shoot(point, false);
        //pcannon->setCannonType(Normal_Cannon, 0);
    }
}

void GameLayer::messageStayFish(CMD_S_StayFish *presult)
{
    if (!presult) {
        return;
    }
    UINT	nFishKey = presult->nFishKey;
    UINT	nStayStart = presult->nStayStart;
    UINT	nStayTime = presult->nStayTime;
    Fish* pFish = GameDataMgr::getInstance()->m_fishList.at(nFishKey);
    if(pFish)
        pFish->stay(nStayTime);
}

void GameLayer::secondSchedule(float dt)
{
    m_nSecondCount--;
    if(m_nSecondCount <= 10)
    {
        Sprite* se_tip = (Sprite*)this->getChildByTag(TAG_Second_Tip);
        if(!se_tip)
            addSecondTip();
        if(se_tip)
        {
            char text[255];
            sprintf(text,"%d秒",m_nSecondCount);
            Label* pLabel = (Label*)se_tip->getChildByTag(1);
            pLabel->setString(text);
        }
        if(m_nSecondCount <= 5 && !this->getChildByTag(TAG_Clear_Layer))
        {
            auto player = GameClearLayer::create();
            this->addChild(player,Tag_UI_Layer+1,TAG_Clear_Layer);
            
        }
        if(m_nSecondCount <= 0)
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
    }
    else
    {
        //this->removeChildByTag(TAG_Clear_Layer);
        this->removeChildByTag(TAG_Second_Tip);
    }
    
}

void GameLayer::setSecondCount(int count)
{
    m_nSecondCount = count;
}

void GameLayer::addSecondTip()
{
    auto secondTipBG = Sprite::create("game_res/secondTip.png");
    secondTipBG->setPosition(Point(568,550));
    this->addChild(secondTipBG,100,TAG_Second_Tip);
    
    auto watch = Sprite::createWithSpriteFrameName("watch_0.png");
    watch->setPosition(Point(60,45));
    secondTipBG->addChild(watch);
    Animation* pAniamation = AnimationCache::getInstance()->getAnimation(watchAnim);
    if(pAniamation)
    {
        watch->runAction(
                         RepeatForever::create(Animate::create(pAniamation))
                         );
    }
    char text[255];
    sprintf(text,"%d秒",m_nSecondCount);
    
    Label* secondLabel = Label::create(text,FontNormal,18);
    secondLabel->setColor(Color3B(246,120,26));
    secondLabel->setAnchorPoint(Point(0,0.5f));
    secondLabel->setPosition(117,55);
    secondTipBG->addChild(secondLabel,1,1);
    
    Label* buttomTip = Label::create("60秒未开炮，即将退出游戏。",FontNormal,17);
    buttomTip->setColor(Color3B::YELLOW);
    buttomTip->setAnchorPoint(Point(0,0.5f));
    buttomTip->setPosition(Point(117,30));
    secondTipBG->addChild(buttomTip);
    
}

bool GameLayer::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto pnode1 = contact.getShapeA()->getBody()->getNode();
    auto pnode2 = contact.getShapeB()->getBody()->getNode();
    if (pnode1 && pnode2) {
        Bullet *pbullet;
        if (pnode1->getTag() == Tag_Bullet) {
            pbullet = (Bullet *)pnode1;
        }
        else if (pnode2->getTag() == Tag_Bullet)
        {
            pbullet = (Bullet *)pnode2;
        }
        //pbullet->getBbullet()->fallingnet();
        pbullet->fallingnet();
        
        
        //pbullet->getBbullet()->removeFromParent();
        pbullet->removeFromParent();
        
    }
    
    return false;
}

bool GameLayer::onContactPreSolve(cocos2d::PhysicsContact &contact, cocos2d::PhysicsContactPreSolve &solve)
{
    solve.ignore();
    log("%s", "碰撞处理");
    return false;
}

void GameLayer::onContactPostSolve(cocos2d::PhysicsContact &contact, const cocos2d::PhysicsContactPostSolve &solve)
{
    
}

void GameLayer::onContactSeperate(cocos2d::PhysicsContact &contact)
{

}
