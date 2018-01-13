//
//  Login.cpp
//  MyGame
//
//  Created by Tang Miao on 1/12/16.
//
//

#include "Login.h"
#include "HallDataMgr.h"
#include "cocostudio/CocoStudio.h"
#include "NetworkMgr.h"
#include "FramList.h"
#include "Regist.h"
#include "Loading.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "UMSocial.h"
#import "AppController.h"
#import "RootViewController.h"
#endif

#include "GameDataMgr.h"
using namespace cocos2d::ui;
USING_NS_CC_EXT;
USING_NS_CC;

Scene* Login::createScene()
{
    if (nullptr != INSTANCE(Director)->getTextureCache()->getTextureForKey("public_res/public_res.plist"))
    {
        INSTANCE(SpriteFrameCache)->removeSpriteFramesFromFile("public_res/public_res.plist");
        INSTANCE(Director)->getTextureCache()->removeTextureForKey("public_res/public_res.plist");
    }
    INSTANCE(Director)->getTextureCache()->removeUnusedTextures();
    INSTANCE(SpriteFrameCache)->removeUnusedSpriteFrames();
    
    FileUtils::getInstance()->setSearchPaths(std::vector<std::string>());
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Login::create();
    layer->setTag(10);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Login::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
   
    m_bSelectSave = false;
    

    return true;
 
}

Login::~Login()
{
    
    DebugLog("Login Release");
}
void Login::initLogin()
{
    INSTANCE(GameDataMgr)->setEnterNormalRoom(false);
    
    auto layout = Layout::create();
    layout->setContentSize(cocos2d::Size(1136, 640));

    auto rootNode = CSLoader::createNode("load_res/MainScene.csb");
    layout->addChild(rootNode);

    //登录
    Button* loginBtn = static_cast<Button*>(rootNode->getChildByName("Button_login"));
    loginBtn->addTouchEventListener(CC_CALLBACK_2(Login::buttonEventWithLogin, this));
    
    //游客登录
    auto visitor = static_cast<Button *>(rootNode->getChildByName("Button_visitor"));
    if (nullptr != visitor)
    {
        visitor->addTouchEventListener(CC_CALLBACK_2(Login::buttonEventWithVisitor, this));
    }
    
    //第三方登录
    auto thirdPlatform = static_cast<Button *>(rootNode->getChildByName("Button_loadMore"));
    if (nullptr != thirdPlatform)
    {
        thirdPlatform->addTouchEventListener(CC_CALLBACK_2(Login::buttonEventWithThirdPlatformLogin, this));
        
    }
    
    //注册
    Button* registBtn = static_cast<Button*>(rootNode->getChildByName("Button_regist"));
    registBtn->addTouchEventListener(CC_CALLBACK_2(Login::buttonEventWithRegist, this));

    //帐号EditBox
    auto account = static_cast<Sprite*>(rootNode->getChildByName("load_account"));
    m_pAccountEdit = EditBox::create(cocos2d::Size(370, 58), "plazz/laba_back.png");
    m_pAccountEdit->setDelegate(this);
    m_pAccountEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    m_pAccountEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_pAccountEdit->setPosition(cocos2d::Point(account->getPositionX()+90, account->getPositionY()));
    m_pAccountEdit->setMaxLength(LEN_ACCOUNTS);
    m_pAccountEdit->setFontSize(26);
    m_pAccountEdit->setPlaceholderFont(FONT_DEFAULT, 24);
    m_pAccountEdit->setPlaceholderFontColor(cocos2d::Color3B(214,246,255));
    m_pAccountEdit->setPlaceHolder("请输入您的游戏帐号");
    m_pAccountEdit->setDelegate(this);
    m_pAccountEdit->setFontColor(Color3B::YELLOW);
    rootNode->addChild(m_pAccountEdit);
    
    //密码EditBox
    auto password =static_cast<Sprite*>(rootNode->getChildByName("load_password"));
    m_pPasswordEdit = EditBox::create(cocos2d::Size(370, 58), "plazz/laba_back.png");
    m_pPasswordEdit->setDelegate(this);
    m_pPasswordEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    m_pPasswordEdit->setInputFlag(EditBox::InputFlag::PASSWORD);
    m_pPasswordEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_pPasswordEdit->setPosition(cocos2d::Point(password->getPositionX()+90, password->getPositionY() - 10));
    m_pPasswordEdit->setFontSize(26);
    m_pPasswordEdit->setPlaceholderFont(FONT_DEFAULT, 24);
    m_pPasswordEdit->setPlaceholderFontColor(cocos2d::Color3B(214,246,255));
    m_pPasswordEdit->setPlaceHolder("请输入您的游戏密码");
    m_pPasswordEdit->setDelegate(this);
    m_pPasswordEdit->setFontColor(Color3B::YELLOW);
    m_pPasswordEdit->setMaxLength(LEN_PASSWORD);
    rootNode->addChild(m_pPasswordEdit);
    
    CFramList::getInstance()->readAccountList();
    int nCount = CFramList::getInstance()->AccountListCount();
    if (nCount>0)
    {
        m_pAccountEdit->setText(CFramList::getInstance()->getAccountListWithIndex(0).accounts);
     
        m_pPasswordEdit->setText(CFramList::getInstance()->getAccountListWithIndex(0).password);
        
        m_bSelectSave = true;
    }
    
    //服务器选择
    Button* selectBtn = static_cast<Button*>(rootNode->getChildByName("Button_serverSel"));
    selectBtn->addTouchEventListener(CC_CALLBACK_2(Login::buttonEventWithServerSelect, this));
    
    //服务器列表背景
    auto list_layout = static_cast<Layout*>(rootNode->getChildByName("server_list"));
    CC_ASSERT(list_layout != nullptr);
    list_layout->setLocalZOrder(2);
    list_layout->addTouchEventListener([=](Ref* sender,Widget::TouchEventType tType)
                                       {
                                           if (Widget::TouchEventType::ENDED == tType)
                                           {
                                               Layout *pLayout = (Layout*)sender;
                                               Vec2 endPos = pLayout->getTouchBeganPosition();
                                               endPos = m_spServerListBg->convertToNodeSpace(endPos);
                                               cocos2d::Rect rect = cocos2d::Rect(0, 0, m_spServerListBg->getContentSize().width, m_spServerListBg->getContentSize().height);
                                               if (!rect.containsPoint(endPos))
                                               {
                                                   pLayout->setVisible(false);
                                               }
                                           }
                                       });
    m_spServerListBg = static_cast<Sprite*>(list_layout->getChildByName("server_bg"));
    CC_ASSERT(m_spServerListBg != nullptr);
    list_layout->setVisible(false);
    
    m_tableView = nullptr;
    
    layout->setScaleX(JudgeScale);
    this->addChild(layout);
}

void Login::onEnter()
{
    Layer::onEnter();
    HallDataMgr::getInstance()->m_loadtype = Load_Default;
}

void Login::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Login::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    //注册网络消息
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_LOGON, CC_CALLBACK_3(Login::loginResult ,this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(Login::roomlistResult, this));
    
    //联网失败通知
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Socket_Connect_Failure, CC_CALLBACK_1(Login::notifyConnectFailure, this)), 1);

    initLogin();
    
    HallDataMgr::getInstance()->_versionCheckcallback = [=](){
        //读取记录
        HallDataMgr::getInstance()->readConfig();
        
        CFramList::getInstance()->readAccountList();
        int nCount = CFramList::getInstance()->AccountListCount();
        if(/*m_bRember &&*/  nCount &&( !HallDataMgr::getInstance()->m_bcancel) && (HallDataMgr::getInstance()->m_loadtype != Load_Visitor))
        {
            //自动登录
            HallDataMgr::getInstance()->m_pAccounts = m_pAccountEdit->getText();
            HallDataMgr::getInstance()->m_pPassword = m_pPasswordEdit->getText() ;
            NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
            NetworkMgr::getInstance()->sendAccountLogin();
            
            Loading *loading = Loading::create();
            loading->setTag(TAG_LOADING);
            loading->setPosition(Vec2(0, 0));
            loading->setFinishCallFunc(CC_CALLBACK_1(Login::loadFinish, this));
            this->addChild(loading);
            
        }
    };
  
}


void Login::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
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

TableViewCell *Login::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    
    Node *item = nullptr;
    if (nullptr == cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
        item = Node::create();
        item->setPosition(table->getViewSize().width * 0.5f,0);
        item->setName("server_item_cell");
        cell->addChild(item);
        
        //线
        Sprite* spLine = Sprite::create("load_res/server_line.png");
        item->addChild(spLine);
    }
    else
    {
        item = cell->getChildByName("server_item_cell");
    }
    
    if (nullptr != item)
    {
        refreshItemCell(cell,idx);
    }
    return cell;
}

ssize_t Login::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return m_vecLoginServerList.size();
}

cocos2d::Size Login::cellSizeForTable(cocos2d::extension::TableView *table)
{
    return cocos2d::Size(1136, m_fCount);
}

void Login::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell)
{
    int idx = cell->getIdx();
    Node *item = cell->getChildByName("server_item_cell");
    if (idx < m_vecLoginServerList.size() && nullptr != item)
    {
        //调整选择状态
        int preIdx = -1;
        for (int i = 0; i < m_vecLoginServerList.size(); ++i)
        {
            if (m_vecLoginServerList[i].bSelect)
            {
                preIdx = i;
            }
            m_vecLoginServerList[i].bSelect = false;
        }
        m_vecLoginServerList[idx].bSelect = true;
        
        //已选择置未选择状态
        auto preCell = table->cellAtIndex(preIdx);
        refreshItemCell(preCell,preIdx);
        
        //更新选择状态
        refreshItemCell(cell,idx);
        
        //保存选择的服务器线路
        HallDataMgr::getInstance()->saveSelectServer(m_vecLoginServerList[idx].strServerAdress);
    }
    
    m_spServerListBg->getParent()->setVisible(false);
}

void Login::loadLoading()
{
    Loading *loading = Loading::create();
    loading->setTag(TAG_LOADING);
    loading->setPosition(Vec2(0, 0));
    loading->setFinishCallFunc(CC_CALLBACK_1(Login::loadFinish, this));
    this->addChild(loading);
}

 void Login::onExit()
{
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_LOGON);
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_SERVER_LIST);
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Socket_Connect_Failure);
    Layer::onExit();
    
}

void Login::loadFinish( cocos2d::Ref *ref)
{
    TransFadeInPlazz;
}

void Login::initServerList()
{
    m_spServerListBg->getParent()->setVisible(true);
    
    int selectIdx = 0;
    if (nullptr == m_tableView)
    {
        //服务器列表
        tagLoginServer loginServer = {};
        loginServer.bSelect = false;
        loginServer.strServerName = "线路4";
        loginServer.strServerAdress = "a.008d.net";
        m_vecLoginServerList.push_back(loginServer);
        
        loginServer = {};
        loginServer.bSelect = false;
        loginServer.strServerName = "线路3";
        loginServer.strServerAdress = "b.008d.net";
        m_vecLoginServerList.push_back(loginServer);
        
        loginServer = {};
        loginServer.bSelect = false;
        loginServer.strServerName = "线路2";
        loginServer.strServerAdress = "c.008d.net";
        m_vecLoginServerList.push_back(loginServer);
        
        loginServer = {};
        loginServer.bSelect = false;
        loginServer.strServerName = "线路1";
        loginServer.strServerAdress = "122.10.92.221";
        m_vecLoginServerList.push_back(loginServer);
        
        m_fCount = 200 / 3;
        m_tableView = cocos2d::extension::TableView::create(this, cocos2d::Size(1136,200));
        m_tableView->setDirection(cocos2d::extension::TableView::Direction::VERTICAL);
        m_tableView->setDelegate(this);
        m_tableView->setPosition(Vec2(0, 30));
        
        m_spServerListBg->addChild(m_tableView);
    }
    for (int i = 0; i < m_vecLoginServerList.size(); ++i)
    {
        if (0 == strcmp(HallDataMgr::getInstance()->m_strGameIpAdress.c_str(), m_vecLoginServerList[i].strServerAdress.c_str()))
        {
            m_vecLoginServerList[i].bSelect = true;
            selectIdx = i;
        }
    }
    
    m_tableView->reloadData();
    
    //滚动到固定行
    int cellcount = m_vecLoginServerList.size();
    int toIdx = 0;
    float viewHeight = m_tableView->getViewSize().height;
    float total = m_fCount * cellcount;
    float offset = 0;
    if (total > viewHeight)
    {
        //offset = 0.0f - (cellcount - (selectIdx - toIdx + 4)) * m_fCount;
        if (selectIdx < cellcount * 0.5)
        {
            offset = -selectIdx * m_fCount;
        }
        else
        {
            toIdx = cellcount - selectIdx - 1;
            offset = viewHeight - total + toIdx * m_fCount;
        }
        
        m_tableView->setContentOffset(Vec2(0, offset));
    }
}

void Login::loadingCoinAnimateResFinish(cocos2d::Texture2D *texture)
{

}
//MARK::通知
void Login::loginResult(WORD  wSubCmdID, void* pData, WORD wSize)
{
    
    if (wSubCmdID == SUB_MB_LOGON_SUCCESS)
    {
        
        DebugLog("登录成功");
        
        CMD_MB_LogonSuccess *success = (CMD_MB_LogonSuccess *)pData;
        HallDataMgr::getInstance()->m_wFaceID = success->wFaceID;
        HallDataMgr::getInstance()->m_cbGender = success->cbGender;
        HallDataMgr::getInstance()->m_wCustom = success->cbCustomID;
        HallDataMgr::getInstance()->m_dwUserID = success->dwUserID;
        HallDataMgr::getInstance()->m_dwGameID = success->dwGameID;
        HallDataMgr::getInstance()->m_pNickName = WHConverUnicodeToUtf8WithArray(success->szNickName);
        HallDataMgr::getInstance()->m_dynamicpass = WHConverUnicodeToUtf8WithArray(success->szDynamicPasswd);
        HallDataMgr::getInstance()->m_pAccounts = WHConverUnicodeToUtf8WithArray(success->szAccounts);
        HallDataMgr::getInstance()->m_Ingot = success->lUserIngot;
        HallDataMgr::getInstance()->m_Bean = success->dUserBeans;
        HallDataMgr::getInstance()->m_UserInsure = success->lUserInsure;
        HallDataMgr::getInstance()->m_UserScore = success->lUserScore;
        HallDataMgr::getInstance()->m_cbInsureEnable = success->cbInsureEnable;
        HallDataMgr::getInstance()->m_bcancel = false;
        HallDataMgr::getInstance()->m_cbMoorMachine = success->cbMoorMachine;
        
        if (HallDataMgr::getInstance()->m_loadtype == Load_Normal)
        {
            CFramList::getInstance()->addAccountListWithString(HallDataMgr::getInstance()->m_pAccounts, HallDataMgr::getInstance()->m_pPassword);
            HallDataMgr::getInstance()->saveConfig();
        }
        if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor)
        {
            HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
        }
        
        HallDataMgr::getInstance()->m_Logintime = getsystemtomillisecond();
        
        
    }
    else if (wSubCmdID == SUB_MB_LOGON_FAILURE)
    {
        DebugLog("登录失败");
        
        Loading *loading = static_cast<Loading*>(this->getChildByTag(TAG_LOADING));
        if (loading != nullptr)
        {
            loading->removeFromParent();
        }
        
        
        CMD_MB_LogonFailure* failuer = (CMD_MB_LogonFailure *)pData;
        auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Load);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
        
        //昵称错误
        if (failuer->lErrorCode == 30)
        {
            
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
            DebugLog("当前昵称被占用");
        }
        else
        {
            std::string str = WHConverUnicodeToUtf8WithArray(failuer->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("", str, Type_Ensure);
        }
        
    }
    else if (wSubCmdID == SUB_MB_UPDATE_NOTIFY)
    {
        Loading *loading = static_cast<Loading*>(this->getChildByTag(TAG_LOADING));
        loading->removeFromParent();
        
        HallDataMgr::getInstance()->AddpopLayer("系统提示", "当前版本不是最新版本，请升级到最新版本", Type_Ensure);
        auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Load);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
        
    }
}

void Login::roomlistResult(WORD  wSubCmdID, void* pData, WORD wSize)
{
    
    switch (wSubCmdID)
    {
        case SUB_MB_LIST_KIND:				//种类列表	100
        {
            
        }
            break;
        case SUB_MB_LIST_SERVER:			//房间列表	101
        {
            int size = sizeof(tagGameServer);
            int count = wSize/size;
            HallDataMgr::getInstance()->roomlistclear();
            for (int index=0 ; index<count ; index++)
            {
                tagGameServer *pServer = new tagGameServer();
                memset(pServer, 0, sizeof(tagGameServer));
                char *pVector = (char *)pData + index*size;
                memcpy(pServer, pVector, size);
                HallDataMgr::getInstance()->m_roomList.push_back(pServer);
                std::sort(HallDataMgr::getInstance()->m_roomList.begin(), HallDataMgr::getInstance()->m_roomList.end(), [](const tagGameServer *s1,const tagGameServer *s2){
                    return s1->wSortID < s2->wSortID;
                });
            }
            
            DebugLog("列表信息");
            
        }
            break;
        case SUB_MB_MATCH_SERVER:           //比赛信息
        {
            
        }
            break;
        case SUB_MB_LIST_FINISH:			//列表完成	200
        {
            
            Loading *loading = static_cast<Loading *>(this->getChildByTag(TAG_LOADING));
            loading->runFinish();
            
            DebugLog("列表完成");
            
        }
            break;
        default:
            break;
    }
}

//登录失败处理
void Login::loginFailureEvent()
{
    
}

void Login::notifyConnectFailure(cocos2d::EventCustom *event)
{
    
    Loading *loading = static_cast<Loading*>(this->getChildByTag(TAG_LOADING));
    loading->removeFromParent();
    
    
}
//MARK::EditBoxDelegate

void Login::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    if(editBox == m_pPasswordEdit)
    {
        
        m_pPasswordEdit->setText("");
        m_bSelectSave = false;

    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (editBox == m_pAccountEdit)
    {
        m_pPasswordEdit->setText("");
        m_bSelectSave = false;
        
    }
#endif
}

void Login::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}
void Login::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (editBox == m_pAccountEdit)
    {
        m_pPasswordEdit->setText("");
        m_bSelectSave = false;
        
    }
#endif
    
}
void Login::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
   
    
}

//MARK::按钮回调
void Login::buttonEventWithThirdPlatformLogin(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == Widget::TouchEventType::ENDED)
    {
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
        UMSocialSnsPlatform *snsplatform = [UMSocialSnsPlatformManager getSocialPlatformWithName:UMShareToSina];
        snsplatform.loginClickHandler(pApp.viewController, [UMSocialControllerService defaultControllerService], YES, ^(UMSocialResponseEntity *response){
            [[UMSocialDataService defaultDataService] requestSocialAccountWithCompletion:^(UMSocialResponseEntity *response) {
                 if (response.responseCode == UMSResponseCodeSuccess) {
                 NSMutableDictionary *ptest = [[response.data objectForKey:@"accounts"] objectForKey:UMShareToSina ];
                 if (!ptest)
                 {
                    return;
                 }
                 //头像url
                HallDataMgr::getInstance()->m_MethodHeadUrl = [[ptest objectForKey:@"icon"] UTF8String];
                 //用户昵称
                HallDataMgr::getInstance()->m_pNickName = [[ptest objectForKey:@"username"] UTF8String];
                HallDataMgr::getInstance()->m_cbGender = [[ptest objectForKey:@"gender"] intValue];
                 
                 //设置用户信息
                 HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
                 HallDataMgr::getInstance()->m_pAccounts = [[ptest objectForKey:@"usid"] UTF8String];
                 //性别  女0 男1
                 HallDataMgr::getInstance()->m_loadtype = Load_Sina;
                 this->loadLoading();    
                 NetworkMgr::getInstance()->sendMethodLogin(PLATFORM_Sina);
                     
         
                 }
                 else
                 {
                    log("%s_%d", "第三方登录失败", response.responseCode);
                 }
             }];
        });
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        DebugLog("第三方登录");
        JniMethodInfo minfo;
        jobject jobj;
        bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity","sinalogin","()V");
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
            minfo.env->DeleteLocalRef(minfo.classID);
        }
        DebugLog("第三方登录");
#endif
     }
}
void Login::buttonEventWithRemember(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
     Button *rememberBtn = (Button*)target;
    if(type == cocos2d::ui::Widget::TouchEventType::BEGAN)
    {
     
        Sprite *dot = (Sprite*)(rememberBtn->getChildByTag(1));
        dot->setVisible(false);
    }
    
    if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        m_bRember = !m_bRember;
        Sprite *dot = (Sprite*)(rememberBtn->getChildByTag(1));
        dot->setVisible(m_bRember);
    }
    
     HallDataMgr::getInstance()->m_bRemember = m_bRember;
     HallDataMgr::getInstance()->saveConfig();
}
//注册
void Login::buttonEventWithRegist(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
      if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
      {
          
         Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Regist::createScene()));
      }
}
//登录
void Login::buttonEventWithLogin(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
     if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
     {
        
        
         std::string account = std::string(m_pAccountEdit->getText());
         std::string password = std::string(m_pPasswordEdit->getText());
         
         if(account.length()==0)
         {
             
             HallDataMgr::getInstance()->AddpopLayer("提示", "请输入帐号", Type_Ensure);
             return;
         }
         
         if(password.length() == 0)
         {
             HallDataMgr::getInstance()->AddpopLayer("提示", "请输入密码", Type_Ensure);
             return;
             
         }
         
         HallDataMgr::getInstance()->m_loadtype = Load_Normal;
         HallDataMgr::getInstance()->m_pAccounts = account;
         HallDataMgr::getInstance()->m_pPassword =m_bSelectSave ? password : MD5Encrypt(password);
         NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
         NetworkMgr::getInstance()->sendAccountLogin();
         
         Loading *loading = Loading::create();
         loading->setTag(TAG_LOADING);
         loading->setPosition(Vec2(0, 0));
         loading->setFinishCallFunc(CC_CALLBACK_1(Login::loadFinish, this));
         this->addChild(loading);
         
     }
}
//游客登录
void Login::buttonEventWithVisitor(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
    
        HallDataMgr::getInstance()->m_loadtype = Load_Visitor;
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        CMD_MB_LogonVisitor visitor;
        memset(&visitor, 0, sizeof(visitor));
        visitor.wModuleID = INVALID_WORD/*HallDataMgr::getInstance()->m_dwKindID*/;
        visitor.dwPlazaVersion = VERSION_MOBILE;
        visitor.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, visitor.szMachineID);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPhoneNum, visitor.szMobilePhone);
        NetworkMgr::getInstance()->sendData(MDM_MB_LOGON, SUB_MB_LOGON_VISITOR, &visitor, sizeof(visitor), NetworkMgr::getInstance()->getSocketOnce());
        
        Loading *loading = Loading::create();
        loading->setTag(TAG_LOADING);
        loading->setPosition(Vec2(0, 0));
        loading->setFinishCallFunc(CC_CALLBACK_1(Login::loadFinish, this));
        this->addChild(loading);
    }
}

void Login::buttonEventWithServerSelect(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        initServerList();
    }
}

void Login::refreshItemCell(cocos2d::extension::TableViewCell *cell,const int& idx)
{
    if (idx >= 0 && idx < m_vecLoginServerList.size() && nullptr != cell)
    {
        tagLoginServer loginServer = m_vecLoginServerList[idx];
        Node *item = cell->getChildByName("server_item_cell");
        if (nullptr != item)
        {
            tagLoginServer loginServer = m_vecLoginServerList[idx];
            ClipText* pServerName = static_cast<ClipText*>(item->getChildByName("server_name"));
            if (nullptr != pServerName)
            {
                pServerName->setString(loginServer.strServerName);
                pServerName->setTextColor(Color4B(255, 255, 255, 255));
            }
            else
            {
                //服务器名字
                pServerName = ClipText::createClipText(cocos2d::Size(200,30), loginServer.strServerName,"Arial",30);
                pServerName->setTextColor(Color4B(255, 255, 255, 255));
                pServerName->setName("server_name");
                pServerName->setAnchorPoint(Vec2(0.5, 0.5));
                pServerName->setPosition(Vec2(-100, 20));
                item->addChild(pServerName);
            }
            
            Sprite *spSelect = static_cast<Sprite*>(item->getChildByName("server_sel"));
            if (nullptr != spSelect)
            {
                spSelect->setVisible(loginServer.bSelect);
            }
            
            if (loginServer.bSelect)
            {
                if (nullptr != pServerName)
                {
                    pServerName->setTextColor(Color4B(254, 236, 82, 255));
                }
                
                if (nullptr == spSelect)
                {
                    spSelect = Sprite::create("load_res/server_select.png");
                    spSelect->setPosition(Vec2(75, 25));
                    spSelect->setName("server_sel");
                    item->addChild(spSelect);
                }
            }
        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    void Java_org_cocos2dx_cpp_AppActivity_sinaLoginSuccess(JNIEnv *env,jobject thiz,jstring useid,jstring headurl,jstring nickname,jbyte gender)
    {
        
        cocos2d::log("新浪授权成功----------");
        
        HallDataMgr::getInstance()->m_pAccounts = JniHelper::jstring2string(useid);;
        HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
        HallDataMgr::getInstance()->m_bRemember = false;
        HallDataMgr::getInstance()->m_bAutomatic = false;
        HallDataMgr::getInstance()->m_pNickName = JniHelper::jstring2string(nickname);
        HallDataMgr::getInstance()->m_cbGender = gender;
        HallDataMgr::getInstance()->m_MethodHeadUrl = JniHelper::jstring2string(headurl);
        HallDataMgr::getInstance()->m_loadtype = Load_Sina;
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](){
            NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
            NetworkMgr::getInstance()->sendMethodLogin(PLATFORM_Sina);
            auto player = static_cast<Login *>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(10));
            if (player)
            {
                player->loadLoading();
                cocos2d::log("发送微博登录");
            }
       
        });
    }
    
    void Java_org_cocos2dx_cpp_AppActivity_Login(JNIEnv *env,jobject thiz,jint platform, jstring account, jstring password, jstring nickname)
    {
        if (HallDataMgr::getInstance()->m_IsLogin == false) {
            cocos2d::Director::getInstance()->replaceScene(Login::createScene());
        }
        HallDataMgr::getInstance()->m_pAccounts = JniHelper::jstring2string(account);
        HallDataMgr::getInstance()->m_pPassword = MD5Encrypt(JniHelper::jstring2string(password));
        HallDataMgr::getInstance()->m_pNickName = JniHelper::jstring2string(nickname);
        HallDataMgr::getInstance()->m_loadtype = Load_Normal;
        
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](){
            NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
            NetworkMgr::getInstance()->sendAccountLogin();
            
            auto player = static_cast<Login *>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(10));
            if (player) {
                player->loadLoading();
            }
        });
    }

}
#endif

