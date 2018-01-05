//
//  Plazz.cpp
//  MyGame
//
//  Created by Tang Miao on 1/7/16.
//
//

#include "Plaza.h"
#include "CocosHeader.h"
#include "cocostudio/CocoStudio.h"

#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../../DataMgrSrc/GameConfigMgr.h"

#include "../Bank/BankShowScene.h"
#include "../Task/TaskScene.h"
#include "../Personal/PersonalScene.h"
#include "../Shop/ShopScene.h"

#include "../../Scene/SceneMgr/SceneMgr.h"
#include "../../Scene/SceneHeader.h"

using namespace cocos2d::ui;

enum ENUM_PLAZA
{
    TAG_MainLayout = 50,
    TAG_LockMachine,
    TAG_UnlockMachine
};

//MARK::PROGRESS
Progress::Progress():_percent(nullptr)
{
}

Progress::~Progress()
{
    
    CC_SAFE_RELEASE(_percent);
}

Progress* Progress::create(const std::string &res)
{

    Progress * ret = new (std::nothrow) Progress();
    if (ret && ret->initProgress(res))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

bool Progress::initProgress(const std::string &res)
{
    if (!ImageView::init())
        return false;
   
    this->setContentSize(Size(257, 367));
    this->loadTexture(res);
    
    //加载圈
    auto circle = Sprite::create("common_res/spinner_circle.png");
    circle->setPosition(this->getContentSize()/2);
    circle->setTag(TAG_Circle);
    this->addChild(circle);
    
    //进度
    auto percent = Label::createWithSystemFont("0％", FONT_TREBUCHET_MS_BOLD, 18);
    setPercent(percent);
    _percent->setPosition(this->getContentSize()/2);
    this->addChild(_percent);
    
    this->runAction();

    return true;
    
}
void Progress::runAction()
{
    auto circle = static_cast<Sprite *>(this->getChildByTag(TAG_Circle));
    assert(circle);
    
    //旋转动画
    RotateBy *rote = RotateBy::create(1.0, 360);
    auto forever = RepeatForever::create(rote);
    
    circle->runAction(forever);
    
}

void Progress::updatePercent(int percent)
{
    
    if (_percent == nullptr)
        return;
    
    _percent->setString(__String::createWithFormat("%d％",percent)->getCString());
    
}

//MARK::GAMELIST
GameList::GameList():
m_pProgress(nullptr)
{
    
    memset(&DownInfo, 0, sizeof(ST_DOWNLOAD_INFO));
}

GameList::~GameList()
{
    if (m_pProgress != nullptr)
    {
        m_pProgress = nullptr;
    }
    
    memset(&DownInfo, 0, sizeof(ST_DOWNLOAD_INFO));
}
GameList *GameList::create()
{
    
    GameList* widget = new (std::nothrow) GameList();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
    
}

bool GameList::init()
{
    if (Widget::init())
    {
        return true;
    }
    return false;
}
void GameList::setInfo(ST_DOWNLOAD_INFO &info)
{
    memcpy(&DownInfo, &info, sizeof(ST_DOWNLOAD_INFO));
}

void GameList::addProgress()
{
    
    m_pProgress = new Progress();
    m_pProgress->autorelease();
    
    m_pProgress->initProgress("plazz/list_alpha.png");
    m_pProgress->setTouchEnabled(true);
    m_pProgress->setAnchorPoint(Vec2(.5, .5));
    m_pProgress->setPosition(this->getContentSize()/2);
    this->addChild(m_pProgress);

}

void GameList::removeProgress()
{
    
    if (m_pProgress != nullptr)
    {
        m_pProgress->removeFromParent();
        m_pProgress = nullptr;
    }
}
//MARK::PLAZZ

Scene* Plazz::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Plazz::create();
    layer->setTag(10);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Plazz::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    m_btnBindMachine = nullptr;
    
    auto layout = Layout::create();
    layout->setTag(TAG_MainLayout);
    layout->setContentSize(Size(1136, 640));
    
    auto rootNode = CSLoader::createNode("plazz/PlazzScene.csb");
    rootNode->setPosition(0, 0);
    layout->addChild(rootNode);
    
    Director::getInstance()->getTextureCache()->addImageAsync("plazz/animate.png", [=](Texture2D *){
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plazz/animate.plist");
        readAnimation("%d.png", "plazzAnimate", 14, 0.2);
        
        auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName("0.png");
        auto sprite = Sprite::createWithSpriteFrame(pframe);
        sprite->setAnchorPoint(Vec2(.5, .5));
        sprite->setPosition(Vec2(116, 585));
        layout->addChild(sprite,10);
        auto paction = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("plazzAnimate")));
        sprite->runAction(paction);
        
        auto signBtn = Button::create();
        signBtn->setScale9Enabled(true);
        signBtn->setContentSize(Size(100, 100));
        signBtn->setPosition(Vec2(116, 585));
        signBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithReward, this));
        rootNode->addChild(signBtn);
    });
    
    _list = ListView::create();
    _list->setBounceEnabled(true);
    _list->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _list->setContentSize(Size(1136, 376));
    _list->setAnchorPoint(Vec2(.5, .5));
    _list->setPosition(Vec2(568, 345));
    _list->setItemsMargin(25.0f);
    layout->addChild(_list);
    
    int nGameCount = INSTANCE(GameConfigMgr)->getGameCount();
    for (int i = 0; i < nGameCount; ++i)
    {
        bool bHave = false;
        _stGameConfig config = INSTANCE(GameConfigMgr)->getGameConfigByIdx(i, bHave);
        if (!bHave)
        {
            continue;
        }
        GameList *list = (GameList*)(this->initOneListByKind(config.nGameKindId));
        
        if (nullptr == list)
        {
            continue;
        }
        EM_GAME eTag = (EM_GAME)list->getTag();
        
        list->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type)
                                    {
                                        if (cocos2d::ui::Widget::TouchEventType::ENDED == type)
                                        {
                                            if (DownloadMgr::getInstance()->getDownInfo(eTag))
                                            {
                                                list->DownInfo.emDownloadStatus = DownLoadDone;
                                                //加载游戏
                                                this->loadingGame(eTag);
                                            }
                                            else
                                            {
                                                std::string storagePath = FileUtils::getInstance()->getWritablePath();
                                                if (config.bGameActive)
                                                {
                                                    std::string path = "http://jd.foxuc.net" + config.strGameResPath;
                                                    DownloadMgr::getInstance()->createTaskforDown(path,
                                                                                                  storagePath,
                                                                                                  eTag);
                                                    
                                                    list->DownInfo.emDownloadStatus = DownLoading;
                                                    list->addProgress();
                                                }
                                                else
                                                {
                                                    HallDataMgr::getInstance()->AddpopLayer("提示", "游戏暂未开通", Type_Ensure);
                                                }
                                            }
                                        }
                                    });
        _list->pushBackCustomItem(list);
    }

    Button *headBtn = static_cast<Button *>(rootNode->getChildByName("head_back_btn"));
    if(nullptr != headBtn)
    {
        
        headBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithTouchUser, this));
    }
    
    //用户昵称
    Label *nick = Label::createWithSystemFont(HallDataMgr::getInstance()->m_pNickName, FONT_TREBUCHET_MS_BOLD, 24);
    setUserNickName(nick);
    _UserNikcName->setAnchorPoint(Vec2(.0, .5));
    _UserNikcName->setDimensions(129, _UserNikcName->getContentSize().height + 1);
    _UserNikcName->setPosition(Vec2(200, 100));
    layout->addChild(_UserNikcName);
    
    //用户分数
    Label *score = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","), FONT_TREBUCHET_MS_BOLD, 24);
    setUserScore(score);
    _UserScore->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_UserScore, 135);
    _UserScore->setAnchorPoint(Vec2(.0, .5));
    _UserScore->setPosition(Vec2(180, 60));
    layout->addChild(_UserScore);
    
    auto level = Label::createWithCharMap("plazz/level_num.png", 10, 13, '0');
    setUserLevel(level);
    _UserLevel->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
    _UserLevel->setPosition(Vec2(170, 100));
    layout->addChild(_UserLevel);
    
  

    layout->setScaleX(JUDGE_SCALE);
    this->addChild(layout);
    
    //购买
    Button *purchaseBtn = static_cast<Button*>(rootNode->getChildByName("btn_purchase"));
    if (nullptr != purchaseBtn)
    {
        
        purchaseBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithShop, this));
    }
 
    //银行
    Button *bank = static_cast<Button *>(rootNode->getChildByName("bank_btn"));
    if (nullptr != bank)
    {
        
        bank->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithBank, this));
    }
    
    //签到
    Button *rewardBtn = static_cast<Button*>(rootNode->getChildByName("baseen_btn"));
    if (nullptr != rewardBtn)
    {
        
        rewardBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithReward, this));
    }
    
    //任务
    Button *taskBtn = static_cast<Button*>(rootNode->getChildByName("task_btn"));
    if (nullptr != taskBtn)
    {
        
        taskBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithTask, this));
    }
    
    //商城
    Button *shopBtn = static_cast<Button*>(rootNode->getChildByName("shop_btn"));
    if (nullptr != shopBtn)
    {
        shopBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithShop, this));
    }
    
    //设置
    Button *setBtn = static_cast<Button*>(rootNode->getChildByName("set_btn"));
    if (nullptr != setBtn)
    {
        setBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithSet, this));
    }
    
    //排行榜
    Button *rankBtn = static_cast<Button*>(rootNode->getChildByName("rank_btn"));
    if (nullptr != rankBtn)
    {
        rankBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithRank, this));
    }
    
    //分享
    Button *shareBtn = static_cast<Button*>(rootNode->getChildByName("share_btn"));
    if (nullptr != shareBtn)
    {
        shareBtn->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithShare, this));
    }
    
    //锁定机器
    std::string file1 = "plazz/bt_unlock_0.png";
    std::string file2 = "plazz/bt_unlock_1.png";
    int nTag = TAG_LockMachine;
    if (HallDataMgr::getInstance()->m_cbMoorMachine)    //已经锁定机器
    {
        file1 = "plazz/bt_lock_0.png";
        file2 = "plazz/bt_lock_1.png";
        nTag = TAG_UnlockMachine;
    }
    m_btnBindMachine = Button::create(file1,file2,file1);
    m_btnBindMachine->setPosition(Vec2(1084, 590));
    m_btnBindMachine->setTag(nTag);
    m_btnBindMachine->addTouchEventListener(CC_CALLBACK_2(Plazz::buttonEventWithLock, this));
    rootNode->addChild(m_btnBindMachine);
    
    return true;
}

Plazz::Plazz()
{
    _headSprite = nullptr;
    _UserScore = nullptr;
    _UserNikcName = nullptr;
    _UserLevel = nullptr;
    
    //_resURL = nullptr;
}

Plazz::~Plazz()
{
    
    CC_SAFE_RELEASE(_headSprite);
    CC_SAFE_RELEASE(_UserScore);
    CC_SAFE_RELEASE(_UserNikcName);
    CC_SAFE_RELEASE(_UserLevel);
    
    /*
    if (_resURL)
    {
        delete  _resURL;
        _resURL = nullptr;
    }
     */
}

Widget* Plazz::initOneList(int index)
{
    auto button = GameList::create();
    button->loadTextures("plazz/oneList_back.png","plazz/oneList_back.png");
    button->setTag(index);
    
    //游戏类型
    ImageView *pName = ImageView::create(__String::createWithFormat("plazz/game_type_%d.png",index)->getCString());
    pName->setAnchorPoint(Vec2(.5, .0));
    pName->setPosition(Vec2(button->getContentSize().width/2, 40));
    button->addChild(pName);
    
    //游戏icon
    ImageView *icon = ImageView::create(__String::createWithFormat("plazz/icon_%d.png",index)->getCString());
    icon->setAnchorPoint(Vec2(.5, .5));
    icon->setPosition(Vec2(button->getContentSize().width/2 + 15, button->getContentSize().height/2 + 40));
    button->addChild(icon);
    
    return button;
}

Widget* Plazz::initOneListByKind(const int &nKind)
{
    auto button = GameList::create();
    button->loadTextures("plazz/oneList_back.png","plazz/oneList_back.png");
    button->setTag(nKind);
    char buf[32] = "";
    sprintf(buf, "game_kind_%d", nKind);
    button->setName(buf);
    
    //游戏类型
    ImageView *pName = ImageView::create(__String::createWithFormat("plazz/game_type_%d.png",nKind)->getCString());
    pName->setAnchorPoint(Vec2(.5, .0));
    pName->setPosition(Vec2(button->getContentSize().width/2, 40));
    button->addChild(pName);
    
    //游戏icon
    ImageView *icon = ImageView::create(__String::createWithFormat("plazz/icon_%d.png",nKind)->getCString());
    icon->setAnchorPoint(Vec2(.5, .5));
    icon->setPosition(Vec2(button->getContentSize().width/2 + 15, button->getContentSize().height/2 + 40));
    button->addChild(icon);
    
    return button;
}

void Plazz::onEnter()
{
    
    Layer::onEnter();
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Plazz::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_PARAMETER, CC_CALLBACK_2(Plazz::LevelInfo, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_UPGRADE, CC_CALLBACK_2(Plazz::LevelUpgrade, this));

    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(Plazz::notifyFreshInfo, this)), 1);
    
   Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Download_Update, CC_CALLBACK_1(Plazz::notifyDownRefresh, this)), 1);
 
    
}

void Plazz::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    auto layout = this->getChildByTag(TAG_MainLayout);
    
    //用户头像
    HeaderRequest *pHead = nullptr;
    if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_NORMAL || HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_VISITOR)
    {
        
        pHead = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                HallDataMgr::getInstance()->m_wCustom,
                                                HallDataMgr::getInstance()->m_dwUserID,
                                                HallDataMgr::getInstance()->m_cbGender);
    }
    else if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_RENREN || HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_SINA)
    {
        pHead = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
    }
    
    
    if(nullptr != pHead)
    {
        
        pHead->setHeadSize(70.0);
        setHeadSprite(pHead);
        _headSprite->setPosition(Vec2(102, 80));
        
        layout->addChild(_headSprite);
    }
    
    //公告
    Notice *notice = Notice::CreateNotice();
    notice->setAnchorPoint(Vec2(.5, .5));
    notice->setContentSize(Size(475, 40));
    notice->setPosition(Vec2(568, 630));
    layout->addChild(notice);
    
    HallDataMgr::getInstance()->m_dwKindID = EM_GAME_DEFALUT;
    
    DownloadMgr::getInstance()->onTaskSuccess = CC_CALLBACK_1(Plazz::onTaskSuccess, this);
    DownloadMgr::getInstance()->onDecompressSuccess = CC_CALLBACK_1(Plazz::onDecompressSuccess, this);
    
    
    if (HallDataMgr::getInstance()->m_loadtype != EM_LOAD_TYPE_VISITOR)
    {
        //查询等级
        this->sendPacketWithUserLevelInfo();
        
    }
}
void Plazz::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_UPGRADE);
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Download_Update);
    
    Layer::onExit();
}

void Plazz::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    /*switch (keycode)
    {
        case EventKeyboard::KeyCode::KEY_ESCAPE:   //返回键听
        {
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
            auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("提示", "确定要退出有乐棋牌吗 ?", Type_Ensure_Cancel));
            player->setEnsureCallback([=](){
                
                Director::getInstance()->end();
                
            });
        }
            break;
        default:
            break;
    }*/
}

void Plazz::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
    
}

void Plazz::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}

void Plazz::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void Plazz::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void Plazz::sendPacketWithUserLevelInfo()
{
    CMD_GP_GROW_LEVEL_QUERY_INFO levelQueryInfo;
    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GROW_LEVEL_QUERY_INFO));
    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine.c_str(), levelQueryInfo.szMachineID);
    Utf8ToUtf16(HallDataMgr::getInstance()->m_pPassword.c_str(), levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}
//MARK::等级
//等级升级
void Plazz::LevelUpgrade(void* pData, WORD wSize)
{
    
    CMD_GP_GROW_LEVEL_UPGRADE *level = (CMD_GP_GROW_LEVEL_UPGRADE *)pData;
    HallDataMgr::getInstance()->m_UserScore = level->lCurrScore;
    HallDataMgr::getInstance()->m_Ingot = level->lCurrIngot;
    
    _UserScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","));
    
    std::string str = WHConverUnicodeToUtf8WithArray(level->szNotifyContent);
    HallDataMgr::getInstance()->AddpopLayer("", str, Type_Ensure);
    
}

//等级信息
void Plazz::LevelInfo(void* pData, WORD wSize)
{
    
    memcpy(&HallDataMgr::getInstance()->m_levelData, pData, sizeof(HallDataMgr::getInstance()->m_levelData));
  
    _UserLevel->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
    
    
    auto action = CallFunc::create([]{
        NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

//MARK::Notify
void Plazz::notifyFreshInfo(cocos2d::EventCustom *event)
{
    
    auto type = (__Integer*)event->getUserData();
    switch (type->getValue())
    {
        case User_Change_Name:
        {
            
            _UserNikcName->setString(HallDataMgr::getInstance()->m_pNickName);
            
        }
            break;
        case User_Change_Head:
        {
            auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(HallDataMgr::getInstance()->m_dwUserID);
            if (ptexture)
            {
                _headSprite->setTexture(ptexture);
            }
        }
        break;

        case User_Change_Score:
        case User_Change_Bean:
        {
            _UserScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","));
            
        }
            break;
        default:
            break;
    }
}

//下载进度刷新
void Plazz::notifyDownRefresh(cocos2d::EventCustom *event)
{
    
    void * data = event->getUserData();
    ST_DOWNLOAD_REFRESH refresh;
    memset(&refresh, 0, sizeof(ST_DOWNLOAD_REFRESH));
    memcpy(&refresh, data, sizeof(ST_DOWNLOAD_REFRESH));
    
    EM_GAME eCurrentKind = refresh.emGame;
    int  nPercent = refresh.dDownloadPercent;
    
    char buf[32] = "";
    sprintf(buf, "game_kind_%d", eCurrentKind);
    //GameList *list = static_cast<GameList *>(_list->getChildByTag(eCurrentKind));
    GameList *list = static_cast<GameList *>(_list->getChildByName(buf));
    if (nullptr != list)
    {
        if (nPercent == 100)
        {
            list->removeProgress();
            ST_DOWNLOAD_INFO info;
            info.emGame = eCurrentKind;
            info.emDownloadStatus = DownLoadDone;
            list->setInfo(info);
            return;
        }
        
        if (list->m_pProgress != nullptr)
        {
            list->m_pProgress->updatePercent(nPercent);
        }
    }
}

void Plazz::initBindMachine(const std::string &title)
{
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_SUCCESS, CC_CALLBACK_2(Plazz::operatesuccessResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_FAILURE, CC_CALLBACK_2(Plazz::operatefailureResult, this));
    
    Size winSize = Director::getInstance()->getWinSize();
    auto color = Layout::create();
    color->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    color->setBackGroundColor(Color3B::BLACK);
    color->setOpacity(125);
    color->setContentSize(winSize);
    color->setTouchEnabled(true);
    this->addChild(color);
    
    //底板
    auto imageBack = ImageView::create("public_res/pop_frame.png");
    imageBack->setScale9Enabled(true);
    imageBack->setContentSize(Size(500, 300));
    imageBack->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    color->addChild(imageBack);
    Size bgSize = imageBack->getContentSize();
    
    //提示文本
    auto m_textNotice = Text::create(title, "Arial", 20);
    m_textNotice->setPosition(Vec2(bgSize.width * 0.5, bgSize.height * 0.8));
    imageBack->addChild(m_textNotice);
    
    //
    auto text = Text::create("银行密码:", "Arial", 20);
    text->setPosition(Vec2(103, bgSize.height * 0.65));
    imageBack->addChild(text);
    
    //输入
    auto fieldInput = ui::EditBox::create(Size(238,42), "public_res/blank.png");
    CC_ASSERT(fieldInput != nullptr);
    fieldInput->setPosition(Vec2(bgSize.width * 0.57, bgSize.height * 0.65));
    fieldInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    fieldInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    fieldInput->setDelegate(this);
    fieldInput->setFontColor(Color3B::WHITE);
    fieldInput->setPlaceHolder("请输入密码");
    imageBack->addChild(fieldInput);
    
    //确定按钮
    auto btn = Button::create("public_res/bt_ensure.png");
    btn->setPosition(Vec2(bgSize.width * 0.28, 130));
    btn->addTouchEventListener([=](cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
                               {
                                   if (ui::Widget::TouchEventType::ENDED == type)
                                   {
                                       BYTE cbBind = 0;
                                       if (HallDataMgr::getInstance()->m_cbMoorMachine)
                                       {
                                           cbBind = 0;
                                       }
                                       else
                                       {
                                           cbBind = 1;
                                       }
                                       std::string str = fieldInput->getText();
                                       NetworkMgr::getInstance()->bindingMachine(cbBind, str);
                                       HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                                       
                                       color->removeFromParentAndCleanup(true);
                                   }
                               });
    imageBack->addChild(btn);
    
    //取消按钮
    btn = Button::create("public_res/bt_cancle.png");
    btn->setPosition(Vec2(bgSize.width * 0.72, 130));
    btn->addTouchEventListener([=](cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
                               {
                                   if (ui::Widget::TouchEventType::ENDED == type)
                                   {
                                       color->removeFromParentAndCleanup(true);
                                       NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
                                       NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
                                   }
                               });
    imageBack->addChild(btn);
    
    //提示
    text = Text::create("注意:", "Arial", 15);
    text->setPosition(Vec2(82, 70));
    imageBack->addChild(text);
    
    //提示
    auto str = "绑定机器后，此帐号只能在此设备上进行游戏。 \n绑定机器后，可在此机器上进行解除绑定操作。";
    auto lab = Label::createWithSystemFont(str, "Arial", 15, Size(350, 0));
    lab->setPosition(Vec2(280, 60));
    imageBack->addChild(lab);
}

void Plazz::operatesuccessResult(void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    std::string normalfile = "plazz/bt_unlock_0.png";
    std::string prefile = "plazz/bt_unlock_1.png";
    int nTag = TAG_LockMachine;
    if (HallDataMgr::getInstance()->m_cbMoorMachine)
    {
        HallDataMgr::getInstance()->m_cbMoorMachine = 0;
    }
    else
    {
        HallDataMgr::getInstance()->m_cbMoorMachine = 1;
        
        normalfile = "plazz/bt_lock_0.png";
        prefile = "plazz/bt_lock_1.png";
        nTag = TAG_UnlockMachine;
    }
    if (nullptr != m_btnBindMachine)
    {
        m_btnBindMachine->loadTextureNormal(normalfile);
        m_btnBindMachine->loadTexturePressed(prefile);
        m_btnBindMachine->loadTextureDisabled(normalfile);
        m_btnBindMachine->setTag(nTag);
    }
    
    auto presult = (CMD_GP_OPERATE_SUCC *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
}

void Plazz::operatefailureResult(void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    auto presult = (CMD_GP_OPERATE_FAILURE *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
}

//MARK::按钮事件
void Plazz::buttonEventWithTouchUser(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        if (this->getChildByTag(PERSONAL))
            return;
        
        PersonalScene *personal = PersonalScene::create();
        personal->setTag(PERSONAL);
        personal->setPosition(Vec2(1136, 0));
        this->addChild(personal);
        
        personal->popPersonal();
    }
}


void Plazz::buttonEventWithBank(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(BANK))
            return;
        
        BankShowScene *bank = BankShowScene::create();
        bank->setTag(BANK);
        bank->setPosition(Vec2(1136, 0));
        this->addChild(bank);
        bank->popBank();
    }
    
}
void Plazz::buttonEventWithReward(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
      
        RewardScene *reward = RewardScene::create();
        reward->setScale(0.5f);
        this->addChild(reward);
        
        Sequence *quence = Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.05), ScaleTo::create(0.2f, 1.0));
        reward->runAction(quence);
    }

}

void Plazz::buttonEventWithShop(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(SHOP))
            return;
        
        ShopScene *shop = ShopScene::create();
        shop->setTag(SHOP);
        shop->setPosition(Vec2(1136, 0));
        this->addChild(shop);
        
        shop->popShop();
        
    }
    
}

void Plazz::buttonEventWithTask(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(TASK))
            return;
        
        TaskScene *task = TaskScene::create();
        task->setTag(TASK);
        task->setPosition(Vec2(1136, 0));
        this->addChild(task);
        task->popTask();
    }
    
}
void Plazz::buttonEventWithSet(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        SettingScene *set = SettingScene::create();
        
        this->addChild(set);
        
    }
    
}
void Plazz::buttonEventWithRank(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(RANK))
            return;
        
        RankScene *rank = RankScene::create();
        rank->setTag(RANK);
        rank->setPosition(Vec2(1136, 0));
        this->addChild(rank);
        rank->popRank();
        
    }
}
void Plazz::buttonEventWithShare(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        auto root = static_cast<Layout *>(this->getChildByTag(TAG_MainLayout));
        auto pbg = Button::create();
        pbg->setScale9Enabled(true);
        pbg->setContentSize(Size(1136, 640));
        pbg->setPosition(Vec2(568, 320));
        pbg->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                pbg->removeFromParent();
            }
        });
        root->addChild(pbg);
        
        
        auto shareRoot = CSLoader::createNode("share_res/share.csb");
        pbg->addChild(shareRoot);
    }
    
}

void Plazz::buttonEventWithLock(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType tType)
{
    Widget *pWidget = static_cast<Widget*>(target);
    if (Widget::TouchEventType::ENDED == tType)
    {
        switch (pWidget->getTag())
        {
            case TAG_LockMachine:
            {
                initBindMachine("请输入银行密码绑定机器");
            }
                break;
            case TAG_UnlockMachine:
            {
                initBindMachine("请输入银行密码解除绑定");
            }
                break;
            default:
                break;
        }
    }
}
//MARK::CallBack
void Plazz::onTaskSuccess(EM_GAME kind)
{
    
    if (HallDataMgr::getInstance()->m_dwKindID != EM_GAME_DEFALUT)
        return;
    
    HallDataMgr::getInstance()->AddpopLayer("", "正在加载资源...", Type_Wait_Text);
    
}
void Plazz::ontaskError(EM_GAME kind)
{
    
      HallDataMgr::getInstance()->AddpopLayer("", "资源下载失败", Type_Info_Reminder);
    
}
void Plazz::onDecompressSuccess(EM_GAME kind)
{
    
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    //加载游戏
    this->loadingGame(kind);
    
}


//MARK::游戏入口
void Plazz::loadingGame(EM_GAME game)
{
    if (HallDataMgr::getInstance()->m_dwKindID != EM_GAME_DEFALUT)
        return;
    bool bHaveGame = false;
    _stGameConfig config = INSTANCE(GameConfigMgr)->getGameConfigByKind(game, bHaveGame);
    if (!bHaveGame)
    {
        return;
    }
    
    std::vector<_stGameRoomServer*>().swap(HallDataMgr::getInstance()->m_subRoomList);
    
    HallDataMgr::getInstance()->m_dwKindID = (DWORD)game;
    
    //配置分系统房间列表
    for (size_t i = 0; i < HallDataMgr::getInstance()->m_roomList.size(); ++i)
    {
        _stGameRoomServer *pServer = HallDataMgr::getInstance()->m_roomList[i];
        if (pServer->wKindID == HallDataMgr::getInstance()->m_dwKindID
            && pServer->wServerType != GAME_GENRE_MATCH)
        {
            HallDataMgr::getInstance()->m_subRoomList.push_back(pServer);
        }
    }
    
    switch (game)
    {
        case EM_GAME_NIUNIU:
        {
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDNiuNiu/hd";
            FileUtils::getInstance()->addSearchPath(path);
            path = FileUtils::getInstance()->getWritablePath() + "JDNiuNiu";
            FileUtils::getInstance()->addSearchPath(path);
            
            //分系统游戏参数
            tagSubSystemParam tagParam = tagSubSystemParam();
            tagParam.m_nPlayerCount = config.nGamePlayerCount;
            HallDataMgr::getInstance()->m_tagSubParam = tagParam;
            
            //透明纹理图
            SpriteFrame* frame = SpriteFrame::create("blank.png", Rect(0, 0, 60, 60));
            SpriteFrameCache::getInstance()->addSpriteFrame(frame, "blank.png");            
            
            INSTANCE(HallDataMgr)->toSubSystemScale();
            auto scene = NN_NAMESPACE_FUN(PlazaScene)::create();
            auto reScene = SCENE_TRANSITION(scene);
            Director::getInstance()->replaceScene(reScene);
        }
            break;
        case EM_GAME_BAIJIALE:
        {
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDBaiJiaLe/hd";
            FileUtils::getInstance()->addSearchPath(path);
            path = FileUtils::getInstance()->getWritablePath() + "JDBaiJiaLe";
            FileUtils::getInstance()->addSearchPath(path);
            
            //分系统游戏参数
            tagSubSystemParam tagParam = tagSubSystemParam();
            tagParam.m_nPlayerCount = config.nGamePlayerCount;
            HallDataMgr::getInstance()->m_tagSubParam = tagParam;
            
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("public_res/public_res.plist");
            INSTANCE(SceneMgr)->transitionScene(BJL_SCENE(PLAZA_SCENE), false);
        }
            break;
        case EM_GAME_LKPY:
        {
            
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDLkpy/";
            FileUtils::getInstance()->addSearchPath(path);
            tagSubSystemParam tagParam = tagSubSystemParam();
            INSTANCE(HallDataMgr)->toSubSystemScale();
            tagParam.m_nPlayerCount = config.nGamePlayerCount;
            HallDataMgr::getInstance()->m_tagSubParam = tagParam;
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f,lkpy_game::RoomLayer::createScene()));
        }
            break;
        case EM_GAME_BRNN:   //百人牛牛
        {
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDBRNN/";
            FileUtils::getInstance()->addSearchPath(path);
            tagSubSystemParam tagParam = tagSubSystemParam();
            
            tagParam.m_nPlayerCount = config.nGamePlayerCount;
            INSTANCE(HallDataMgr)->toSubSystemScale();
            HallDataMgr::getInstance()->m_tagSubParam = tagParam;
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f,brnn_game::RoomLayer::createScene()));
        }
            break;
        case EM_GAME_ZJH:      //扎金花
        {
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDZjh/";
            std::string path2 = FileUtils::getInstance()->getWritablePath();
            FileUtils::getInstance()->addSearchPath(path);
            FileUtils::getInstance()->addSearchPath(path2);
            tagSubSystemParam tagParam = tagSubSystemParam();
            
            tagParam.m_nPlayerCount = config.nGamePlayerCount;
            INSTANCE(HallDataMgr)->toSubSystemScale();
            HallDataMgr::getInstance()->m_tagSubParam = tagParam;
            zjh_game::RoomLayer::loadRes();
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f,zjh_game::RoomLayer::createScene()));
        }
            break;
        case EM_GAME_TBNN:
        {
            std::string path = FileUtils::getInstance()->getWritablePath() + "OxSixEx";
            FileUtils::getInstance()->addSearchPath(path);
            
            //分系统游戏参数
            tagSubSystemParam tagParam = tagSubSystemParam();
            tagParam.m_nPlayerCount = config.nGamePlayerCount;
            HallDataMgr::getInstance()->m_tagSubParam = tagParam;
            
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("public_res/public_res.plist");
            INSTANCE(SceneMgr)->transitionScene(TBNN_SCENE(PLAZA_SCENE), false);
        }
            break;
    default:
        break;
    }
    
}
//MARK::android方法
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    void Java_org_cocos2dx_cpp_AppActivity_UserHeadChange(JNIEnv *env,jobject thiz, jstring headFile)
    {
        std::string imagepath = JniHelper::jstring2string(headFile);
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([imagepath](){
            log("头像地址%s",imagepath.c_str());
            cocos2d::Image * pimage = new cocos2d::Image();
            if (pimage->initWithImageFile(imagepath))
            {
                //log("path:%s  length:%zd",imagepath->getCString(), pimage->getDataLen());
                if (pimage->getDataLen() < 9216)
                {
                    HallDataMgr::getInstance()->AddpopLayer("", "选择图片太小...", Type_Ensure);
                    pimage->release();
                    return;
                }
                HallDataMgr::getInstance()->m_completecallback(pimage);
            }
            pimage->release();
            
        });
    }
}
#endif

