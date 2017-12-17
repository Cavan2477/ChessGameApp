//
//  Reward.cpp
//  GameProject
//
//  Created by Tang Miao on 1/29/16.
//
//

#include "RewardScene.h"
#include "cocostudio/CocoStudio.h"

#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"

using namespace cocos2d::ui;

typedef enum
{
    SIGN_DAYNUM = 0,
    SIGN_NODE	= 1,
    SIGN_PERNUM = 2,
    SIGN_BTN	= 20
}ENUM_REWARD;

typedef enum
{
    LOW_LIMIT_NUM	= 0,
    BASE_NODE		= 1,
    TAKE_TIMES		= 2,
    GET_NUMS		= 3,
    TAKE_BTN		= 20
}ENUM_BASE_EN;

RewardScene::~RewardScene()
{
    _touchListener = nullptr;
    _RewardLayout = nullptr;
    _BaseenLayout = nullptr;
}

RewardScene::RewardScene():
_touchListener(nullptr)
,_RewardLayout(nullptr)
,_BaseenLayout(nullptr)
,m_eRewardType(type_reward)
,m_lowbase(0)
,m_wSeriesDate(0)
{
    
    
}
bool RewardScene::init()
{
    
    if (!Layer::init())
    {
        return false;
    }


    _mainLayout = Layout::create();
    _mainLayout->setContentSize(Size(1136, 960));
    
    auto rootNode = CSLoader::createNode("reward_res/MainScene.csb");
    _mainLayout->addChild(rootNode);

    initReward();
    initBaseen();
    
    _BaseenLayout->setVisible(false);
    
    _mainLayout->setScaleX(JUDGE_SCALE);
    this->addChild(_mainLayout);
    
    //关闭
    Button *closeBtn = static_cast<Button *>(rootNode->getChildByName("Button_close"));
    if (nullptr != closeBtn)
    {
        closeBtn->addTouchEventListener(CC_CALLBACK_2(RewardScene::buttonEventWithClose, this));
    }
    
    //切换按钮
    Button *rewardShow = static_cast<Button *>(rootNode->getChildByName("changebtn_reward"));
    if (nullptr != rewardShow)
    {
        rewardShow->addTouchEventListener(CC_CALLBACK_2(RewardScene::buttonEventWithShowReward, this));
    }
    
    Button *baseenShow = static_cast<Button *>(rootNode->getChildByName("changeBtn_takeBaseen"));
    if (nullptr != baseenShow)
    {
        baseenShow->addTouchEventListener(CC_CALLBACK_2(RewardScene::buttonEventWithShowBaseen, this));
    }
    
    return true;
}

void RewardScene::onEnter()
{
    
    Layer::onEnter();
    
    //签到信息
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_CHECKIN_INFO, CC_CALLBACK_2(RewardScene::checkininfo, this));
    //签到结果
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_CHECKIN_RESULT, CC_CALLBACK_2(RewardScene::checkinresult, this));
    //低保参数
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_BASEENSURE_PARAMETER, CC_CALLBACK_2(RewardScene::baseensureparamter, this));
    //低保结果
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_BASEENSURE_RESULT, CC_CALLBACK_2(RewardScene::baseensureresult, this));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = CC_CALLBACK_2(RewardScene::onTouchBegan, this);
    _touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
  
}

void RewardScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->addImageAsync("coin_res/coins.png", [=](Texture2D *){
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("coin_res/coins.plist");
    });
    
}

void RewardScene::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_CHECKIN_INFO);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_CHECKIN_RESULT);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_BASEENSURE_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_BASEENSURE_RESULT);
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    Layer::onExit();
   
}

void RewardScene::initReward()
{
    _RewardLayout = Layout::create();
    _RewardLayout->setContentSize(Size(1136, 640));
    _mainLayout->addChild(_RewardLayout);
    
    
    auto rewardNode = CSLoader::createNode("reward_res/Reward.csb");
    rewardNode->setTag(SIGN_NODE);
    _RewardLayout->addChild(rewardNode);
    
    //连续天数
    Label *dayNum = Label::createWithCharMap("reward_res/reward_num.png", 23, 30, '0');
    dayNum->setTag(SIGN_DAYNUM);
    dayNum->setString("0");
    dayNum->setPosition(Vec2(635, 445));
    _RewardLayout->addChild(dayNum);
    
    //签到奖励
    for (int i=0 ; i<WEEK ; i++)
    {
        
        Label *perNum = Label::createWithCharMap("reward_res/sign_perNum.png", 11, 13, '0');
        perNum->setTag(SIGN_PERNUM + i);
        perNum->setString("0");
        perNum->setPosition(Vec2(290 + i*95, 233));
        _RewardLayout->addChild(perNum);
    }
    
    //签到
    Button *signbtn = static_cast<Button *>(rewardNode->getChildByName("reward_takeBtn"));
   
    if (nullptr != signbtn)
    {
        signbtn->setTag(SIGN_BTN);
        signbtn->addTouchEventListener(CC_CALLBACK_2(RewardScene::buttonEventWithSign, this));
        signbtn->setColor(cocos2d::Color3B::GRAY);
        signbtn->setEnabled(false);
    }
    
    sendCheckinQueryInfo();
    
}

void RewardScene::initBaseen()
{
    
    _BaseenLayout = Layout::create();
    _BaseenLayout->setContentSize(Size(1136, 640));
    _mainLayout->addChild(_BaseenLayout);
    
    auto BaseenNode = CSLoader::createNode("reward_res/Baseen.csb");
    BaseenNode->setTag(BASE_NODE);
    _BaseenLayout->addChild(BaseenNode);
    
    //低保限制金币
    auto limitNum = static_cast<Text *>(BaseenNode->getChildByName("Text_less"));
    limitNum->setTag(LOW_LIMIT_NUM);
    
    //领取次数
    auto takeTimes = static_cast<Text *>(BaseenNode->getChildByName("Text_times"));
    takeTimes->setTag(TAKE_TIMES);
    
    //领取金额
    auto takeNum = static_cast<Text *>(BaseenNode->getChildByName("Text_9"));
    takeNum->setTag(GET_NUMS);
    
    //领取低保
    Button *baseenbtn = static_cast<Button *>(BaseenNode->getChildByName("take_basebtn"));
    if (nullptr != baseenbtn)
    {
        baseenbtn->addTouchEventListener(CC_CALLBACK_2(RewardScene::buttonEventWithTakeBaseen, this));
        baseenbtn->setColor(cocos2d::Color3B::GRAY);
        baseenbtn->setTag(TAKE_BTN);
        baseenbtn->setEnabled(false);
    }
}

void RewardScene::updateReward(void *data)
{
    
    auto info = (CMD_GP_CheckInInfo *)data;
    
    this->m_wSeriesDate = info->wSeriesDate;
    
    //更新连续天数
    Label *daynum = static_cast<Label *>(_RewardLayout->getChildByTag(SIGN_DAYNUM));
    if (nullptr != daynum)
    {
        daynum->setString(__String::createWithFormat("%d",info->wSeriesDate)->getCString());
    }
    
    //每日奖励数目
    for (int i =0; i<WEEK; i++)
    {
        Label *per = static_cast<Label *>(_RewardLayout->getChildByTag(SIGN_PERNUM + i));
        per->setString(__String::createWithFormat("%lld",info->lRewardGold[i])->getCString());
        
        if (i < this->m_wSeriesDate)
        {
            auto day = static_cast<Button *>(_RewardLayout->getChildByTag(SIGN_NODE)->getChildByName(__String::createWithFormat("day_%d",i+1)->getCString()));
            
            if (nullptr != day)
            {
                //遮罩
                auto image = ImageView::create("reward_res/day_alpha.png");
                image->setPosition(day->getContentSize()/2);
                day->addChild(image);
                
                //签到标记
                auto already = ImageView::create("reward_res/sign_already.png");
                already->setPosition(day->getContentSize()/2);
                day->addChild(already);
                
            }
            
        }
    }
    
    //签到按钮状态
    Button *signBtn = static_cast<Button *>(_RewardLayout->getChildByTag(SIGN_NODE)->getChildByTag(SIGN_BTN));
    if (nullptr != signBtn)
    {

        signBtn->setEnabled(!info->bTodayChecked);
        
        if (info->bTodayChecked)
        {
            signBtn->loadTextures("reward_res/sign_tomorrow.png","reward_res/reward_signBtn_0.png");
        }else
        {
            signBtn->setColor(cocos2d::Color3B::YELLOW);
            
        }
    }
}

void RewardScene::updateBaseen(void *data)
{
    
    auto result = (CMD_GP_BaseEnsureParamter *)data;
    
    //低保限制金币
    auto limitNum = static_cast<Text *>(_BaseenLayout->getChildByTag(BASE_NODE)->getChildByTag(LOW_LIMIT_NUM));
    limitNum->setString(__String::createWithFormat("%lld",result->lScoreCondition)->getCString());
    
    //领取次数
    auto takeTimes = static_cast<Text *>(_BaseenLayout->getChildByTag(BASE_NODE)->getChildByTag(TAKE_TIMES));
    takeTimes->setString(__String::createWithFormat("%d",result->cbTakeTimes)->getCString());
    
    //领取金额
    auto takeNum = static_cast<Text *>(_BaseenLayout->getChildByTag(BASE_NODE)->getChildByTag(GET_NUMS));
    takeNum->setString(__String::createWithFormat("%lld",result->lScoreAmount)->getCString());
    
    
}
void RewardScene::sendCheckinQueryInfo()
{
    CMD_GP_CheckInQueryInfo  info;
    memset(&info, 0, sizeof(info));
    
    info.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), info.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_QUERY, &info, sizeof(info),NetworkMgr::getInstance()->getSocketOnce());
    
}

void RewardScene::sendCheckinDone()
{
    CMD_GP_CheckInDone  CheckInDone;
    memset(&CheckInDone, 0, sizeof(CMD_GP_CheckInDone));
    
    CheckInDone.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), CheckInDone.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), CheckInDone.szMachineID);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_DONE,
                                        &CheckInDone,
                                        sizeof(CheckInDone),
                                        NetworkMgr::getInstance()->getSocketOnce());
}

void RewardScene::sendEnsureTake()
{
    CMD_GP_BaseEnsureTake  BaseEnsureTake;
    memset(&BaseEnsureTake, 0, sizeof(CMD_GP_BaseEnsureTake));
    
    BaseEnsureTake.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), BaseEnsureTake.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), BaseEnsureTake.szMachineID);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_TAKE, &BaseEnsureTake,
                                        sizeof(BaseEnsureTake),NetworkMgr::getInstance()->getSocketOnce());
}

void RewardScene::checkininfo(void *pData, WORD wSize)
{

    //更新显示
    updateReward(pData);
    
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
}

void RewardScene::checkinresult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_CheckInResult *)pData;
    if (!result->bSuccessed)
    {
        auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
        HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
        return;
    }

    m_wSeriesDate ++;
    
    //更新连续天数
    Label *daynum = static_cast<Label *>(_RewardLayout->getChildByTag(SIGN_DAYNUM));
    if (nullptr != daynum)
    {
        daynum->setString(__String::createWithFormat("%d",this->m_wSeriesDate)->getCString());
    }
    

    auto day = static_cast<Button *>(_RewardLayout->getChildByTag(SIGN_NODE)->getChildByName(__String::createWithFormat("day_%d",m_wSeriesDate)->getCString()));
    
    if (nullptr != day)
    {
        //遮罩
        auto image = ImageView::create("reward_res/day_alpha.png");
        image->setPosition(day->getContentSize()/2);
        day->addChild(image);
        
        //签到标记
        auto already = ImageView::create("reward_res/sign_already.png");
        already->setPosition(day->getContentSize()/2);
        day->addChild(already);
        
    }
    
    //签到按钮状态
    Button *signBtn = static_cast<Button *>(_RewardLayout->getChildByTag(SIGN_NODE)->getChildByTag(SIGN_BTN));
    if (nullptr != signBtn)
    {
        signBtn->loadTextures("reward_res/sign_tomorrow.png","reward_res/reward_signBtn_0.png");
        signBtn->setEnabled(false);
    }
    
    
    SCORE lRewardScore = result->lScore - HallDataMgr::getInstance()->m_UserScore;
    HallDataMgr::getInstance()->m_UserScore = result->lScore;
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    
    //金币掉落动画
    CallFunc *createMove = CallFunc::create([=](){
        
        int rand = std::rand()%17;
        
        
        auto goldDrop = GoldDrop::create(__String::createWithFormat("jinbi_%d.png",rand)->getCString());
        goldDrop->setBeginVec(Vec2(32, 680));
        this->addChild(goldDrop);
        
    });
    

    this->runAction(Repeat::create(Sequence::createWithTwoActions(createMove, DelayTime::create(0.03f)), 30));
    
    ImageView *tipsIMG = ImageView::create("reward_res/reward_tipsBG.png");
    tipsIMG->setAnchorPoint(Vec2(.5, .5));
    tipsIMG->setPosition(this->getContentSize()/2);
    this->addChild(tipsIMG);
    
    auto tips = Label::createWithSystemFont(__String::createWithFormat("恭喜您,获取%lld金币",lRewardScore)->getCString(), FONT_DEFAULT, 28);
    tips->setAnchorPoint(Vec2(.5, .5));
    tips->setTextColor(Color4B::YELLOW);
    tips->setPosition(tipsIMG->getContentSize()/2);
    tipsIMG->addChild(tips);
    
    tipsIMG->setVisible(false);
    
    auto delay = DelayTime::create(1.5);
    auto func = CallFunc::create([=](){
        
        tipsIMG->setVisible(true);
        
    });
    auto quence = Sequence::createWithTwoActions(delay, func);
    
    auto _delay = DelayTime::create(3.0f);
    auto _func = CallFunc::create([=](){
        
        tipsIMG->removeFromParent();
        
    });
    auto _quence = Sequence::createWithTwoActions(_delay, _func);
    this->runAction(Sequence::createWithTwoActions(quence,_quence));

}

void RewardScene::baseensureparamter(void *pData, WORD wSize)
{
    auto result = (CMD_GP_BaseEnsureParamter *)pData;

    m_lowbase = result->lScoreCondition;
    if (HallDataMgr::getInstance()->m_UserScore + HallDataMgr::getInstance()->m_UserInsure > m_lowbase)
    {
        
        Button *baseenbtn = static_cast<Button *>(_BaseenLayout->getChildByTag(BASE_NODE)->getChildByTag(TAKE_BTN));
        baseenbtn->setColor(cocos2d::Color3B::GRAY);
        baseenbtn->setEnabled(false);

    }else
    {
        Button *baseenbtn = static_cast<Button *>(_BaseenLayout->getChildByTag(BASE_NODE)->getChildByTag(TAKE_BTN));
        baseenbtn->setColor(cocos2d::Color3B::YELLOW);
        baseenbtn->setEnabled(true);
        
    }
    
     updateBaseen(pData);

     NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);

}

void RewardScene::baseensureresult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_BaseEnsureResult *)pData;
    auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);

    HallDataMgr::getInstance()->m_UserScore = result->lGameScore;
    
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    
    if (m_lowbase < HallDataMgr::getInstance()->m_UserScore) {
        auto BaseenNode = _BaseenLayout->getChildByTag(BASE_NODE);
        if (BaseenNode) {
            Button *baseenbtn = static_cast<Button *>(BaseenNode->getChildByTag(TAKE_BTN));
            if (baseenbtn) {
                baseenbtn->setColor(cocos2d::Color3B::GRAY);
                baseenbtn->setEnabled(false);
            }
        }
        
    }
    
}
bool RewardScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    
    return true;
}
void RewardScene::buttonEventWithClose(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
    
}
void RewardScene::buttonEventWithSign(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
     if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
     {
         
         sendCheckinDone();
         
         
     }
    
}
void RewardScene::buttonEventWithTakeBaseen(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
     if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
     {
         //领取低保
         this->sendEnsureTake();
     }

}

void RewardScene::buttonEventWithShowReward(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
     if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
     {
         if (m_eRewardType == type_reward)
             return;
         
         m_eRewardType = type_reward;
         
         _RewardLayout->setVisible(true);
         _BaseenLayout->setVisible(false);
         
     }
}
void RewardScene::buttonEventWithShowBaseen(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
     if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
     {
         if (m_eRewardType == type_baseen)
             return;
         
         m_eRewardType = type_baseen;
    
         _RewardLayout->setVisible(false);
         _BaseenLayout->setVisible(true);
         
         if (m_lowbase == 0)
         {
             NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
             NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_LOAD, NULL, 0, NetworkMgr::getInstance()->getSocketOnce());
         }
         
         
     }
}


