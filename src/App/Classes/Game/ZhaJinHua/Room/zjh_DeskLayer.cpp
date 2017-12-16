#include "Project.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "zjh_DeskLayer.h"
#include "zjh_DeskCell.h"
#include "ModeLayer.h"
#include "zjh_RoomLayer.h"
#include "CocosHeader.h"
#include "lkpy_help.h"
#include "CommonPlazaLayer.h"
USING_NS_CC;
using namespace ui;
USING_ZJH_NAMESPACE;
#define GSWZ_PAGE_TABLE_COUNT 3
enum ButtonTag
{
    Tag_BT_Begin = 0,//快速开始
    Tag_BT_Encrypt,//桌子加密
    Tag_Bt_Room,
    Tag_Bt_Mission,
    Tag_BT_Return,
    Tag_BT_Head,
    Tag_BT_Excharge,//兑换
    Tag_BT_Shop,//商城
    
    Tag_BT_Rewards,//奖励
    Tag_BT_Bank,//银行
    Tag_BT_Help,//帮助
    Tag_BT_Setting,//设置
    
    Tag_Rewards,
    Tag_Bank,
    Tag_Help,
    Tag_Exchage,
    Tag_PersonData,
    Tag_Mission,
    Tag_LevelData,
    Tag_PageView,//翻页
    
    Tag_Pop_Layout,
    
    Tag_Encrypt,
    
    Tag_Encrypt_Ensure,
    
    
    
    Tag_Dot = 20,
};

Scene* DeskLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DeskLayer::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool DeskLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto plazaLayer = CommonPlazaLayer::create();
    this->addChild(plazaLayer);
    plazaLayer->hideUserInfo(true);
    HallDataMgr::getInstance()->m_isFirstSendGameOption = false;
    HallDataMgr::getInstance()->m_RoomType = Data_Room;
    cocos2d::Size viewsize = WinSize;
    
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(80, viewsize.height-55));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f,RoomLayer::createScene()));
            NetworkMgr::getInstance()->Disconnect(Data_Room);
            memset(&HallDataMgr::getInstance()->m_Tableinfo, 0, sizeof(CMD_GR_TableInfo));
        }
    });
    plazaLayer->addChild(preturn);
    
    auto pbegin = Button::create("plazz/desk_quickEnter.png");
    pbegin->setTag(Tag_BT_Begin);
    pbegin->addTouchEventListener(CC_CALLBACK_2(DeskLayer::touchEvent, this));
    pbegin->setPosition(Vec2(viewsize.width-160, viewsize.height-78));
    plazaLayer->addChild(pbegin);
    
    auto pencrypt = Button::create("plazz/desk_pwd.png");
    pencrypt->setTag(Tag_BT_Encrypt);
    pencrypt->addTouchEventListener(CC_CALLBACK_2(DeskLayer::touchEvent, this));
    pencrypt->setPosition(Vec2(viewsize.width-380, viewsize.height-78));
    plazaLayer->addChild(pencrypt);
    
    
    m_pageIndex = 0;
    m_pageView = PageView::create();
    m_pageView->setContentSize(cocos2d::Size(WinSize.width, 550));
    m_pageView->setAnchorPoint(Vec2(0.5f, 0.5f));
    m_pageView->setTag(Tag_PageView);
    m_pageView->setPosition(Vec2(viewsize.width/2, viewsize.height/2-55));
    m_pageView->setCustomScrollThreshold(200);
    m_pageView->addEventListenerPageView(this,pagevieweventselector(DeskLayer::pageViewEvent));
    this->addChild(m_pageView);
    
    int judgevalue = HallDataMgr::getInstance()->m_TableCount%GSWZ_PAGE_TABLE_COUNT;
    int pagenum = HallDataMgr::getInstance()->m_TableCount/GSWZ_PAGE_TABLE_COUNT;
    pagenum = judgevalue?(pagenum+1):pagenum;
    for (int index=0; index<pagenum; ++index)
    {
        auto playout = Layout::create();
        playout->setContentSize(cocos2d::Size(m_pageView->getContentSize()));
        m_pageView->addPage(playout);
        
        for (int deskid=0; deskid<GSWZ_PAGE_TABLE_COUNT; ++deskid) {
            int number = index*GSWZ_PAGE_TABLE_COUNT+deskid;
            auto pdesk = DeskCell::create("room_res/desk_bg.png", number);
            pdesk->setTag(deskid);
            pdesk->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-(pdesk->getContentSize().height+30)*(deskid+0.5f)));
            playout->addChild(pdesk);
        }
    }
    return true;}



void DeskLayer::encryptinit()
{
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    pbg->setTag(Tag_Encrypt);
    this->addChild(pbg,3);
    
    auto ppop = ImageView::create("public_res/pop_frame.png");
    ppop->setScale9Enabled(true);
    ppop->setContentSize(cocos2d::Size(500,300));
    ppop->setPosition(viewsize/2);
    pbg->addChild(ppop);
    
    auto pclose = Button::create("public_res/bt_close_0.png");
    pclose->cocos2d::Node::setScale(0.95f);
    pclose->setPosition(Vec2(ppop->getContentSize().width-31, ppop->getContentSize().height-31));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                          pbg->removeFromParent();
                                      }
                                  });
    ppop->addChild(pclose);
    
    auto ptext = Label::createWithSystemFont("你已设置了桌子密码，如要更改，请输入密码！如要解锁，请清空密码。", FontBold, 22);
    ptext->setPosition(Vec2(70, ppop->getContentSize().height-80));
    ptext->setAnchorPoint(Vec2(0.f, 0.5f));
    ptext->setDimensions(380, 55);
    ppop->addChild(ptext);
    
    auto ptitle = Text::create("桌子密码：", FontBold, 22);
    ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
    ptitle->setPosition(Vec2(70, ppop->getContentSize().height-140));
    ppop->addChild(ptitle);
    
    m_Encrpytedit = EditBox::create(cocos2d::Size(238,42), "public_res/desk_pwd_text.png");
    m_Encrpytedit->setFontColor(Color3B::BLACK);
    m_Encrpytedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_Encrpytedit->setPosition(Vec2(ptitle->getPositionX()+100, ptitle->getPositionY()));
    m_Encrpytedit->setInputFlag(EditBox::InputFlag::PASSWORD);
    m_Encrpytedit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_Encrpytedit->setText(HallDataMgr::getInstance()->m_Tablepass.c_str());
    m_Encrpytedit->setFontSize(28);
    ppop->addChild(m_Encrpytedit);
    
    auto pensure = Button::create("public_res/desk_sure.png");
    pensure->addTouchEventListener(CC_CALLBACK_2(DeskLayer::touchEvent, this));
    pensure->setTag(Tag_Encrypt_Ensure);
    pensure->setPosition(Point(ppop->getContentSize().width/2, ppop->getContentSize().height - 225));
    ppop->addChild(pensure);
}


void DeskLayer::onEnter()
{
    Layer::onEnter();
    //    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(DeskLayer::notifyRefreshStatus), whEvent_Desk_Status, nullptr);
}

void DeskLayer::onEnterTransitionDidFinish()
{
    NetworkMgr::getInstance()->m_Userdelegate = this;
    this->userinfoUpdate(0);
    Layer::onEnterTransitionDidFinish();
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(DeskLayer::notifyRefreshScore, this)), 1);
    //this->sendPacketWithUserLevelInfo();
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_PARAMETER, CC_CALLBACK_2(DeskLayer::levelInfo, this));
    
}

void DeskLayer::onExit()
{
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    m_UserList.clear();
    __NotificationCenter::getInstance()->removeObserver(this, whEvent_Desk_Status);
    //Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_PARAMETER);
    Layer::onExit();
}

void DeskLayer::sendPacketWithUserLevelInfo()
{
    CMD_GP_GrowLevelQueryInfo levelQueryInfo;
    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GrowLevelQueryInfo));
    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), levelQueryInfo.szMachineID);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}

void DeskLayer::levelInfo(void *pData, WORD wSize)
{
    memcpy(&HallDataMgr::getInstance()->m_levelData, pData, sizeof(HallDataMgr::getInstance()->m_levelData));
    this->levelchange();
    auto action = CallFunc::create([]{
        NetworkMgr::getInstance()->Disconnect(Data_Load);
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}



void DeskLayer::notifyRefreshStatus(cocos2d::Ref *pSender)
{
    auto deskID = (__Integer*)pSender;
    auto playout = m_pageView->getPage(deskID->getValue()/PAGE_TABLE_COUNT);
    if (playout) {
        auto pcell = (DeskCell *)playout->getChildByTag(deskID->getValue()%PAGE_TABLE_COUNT);
        if (pcell) {
            pcell->updateStatus(HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[deskID->getValue()].cbTableLock,
                                HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[deskID->getValue()].cbPlayStatus);
        }
    }
}

void DeskLayer::notifyRefreshScore(cocos2d::EventCustom *event)
{
    auto type = (__Integer*)event->getUserData();
    switch (type->getValue()) {
        case User_Change_Name:
        {
            if (m_labelNickName) {
                string tempStr = lkpy_game::getStringByWidth(HallDataMgr::getInstance()->m_pNickName, 18,134);
                m_labelNickName->setString(tempStr);
            }
        }
            break;
        case User_Change_Head:
        {
            auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(HallDataMgr::getInstance()->m_dwUserID);
            if (ptexture) {
                m_head->setTexture(ptexture);
                m_head->setTextureRect(cocos2d::Rect(0, 0, FACE_CX, FACE_CY));
            }
        }
            break;
        case User_Change_Level:
        {
            this->levelchange();
        }
            break;
        case User_Change_Score:
        case User_Change_Bean:
        {
            this->scorechange();
        }
            break;
        default:
            break;
    }
    
}

void DeskLayer::levelchange()
{
    auto str = __String::createWithFormat("%d", HallDataMgr::getInstance()->m_levelData.wCurrLevelID);
    if (m_levelNum) {
        m_levelNum->setString(str->getCString());
    }
    float Experience = HallDataMgr::getInstance()->m_levelData.dwExperience;
    float percent = Experience/HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience;
    if (m_ProBar) {
        m_ProBar->setPercent(percent*100);
    }
}

void DeskLayer::scorechange()
{
    std::string scorestr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestr = lkpy_game::getGoldStr(scorestr,';');
    if (scorestr.length() > 16) {
        scorestr = scorestr.substr(0,14);
        scorestr.append("::");
    }
    if (m_goldNum) {
        m_goldNum->setString(scorestr);
    }
    
    std::string beanstr = __String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_Bean)->getCString();
    auto pos = beanstr.find(".");
    beanstr = beanstr.replace(pos, 1, ":");
    beanstr = lkpy_game::getGoldStr(beanstr,';');
    if (beanstr.length() > 16) {
        beanstr = beanstr.substr(0,14);
        beanstr.append("::");
    }
    if (m_beanNum) {
        m_beanNum->setString(beanstr);
    }
}

void DeskLayer::userinfoUpdate(float t)
{
    //HallDataMgr::getInstance()->AddpopLayer("", "请求数据中。。。", Type_Wait_Text);
    auto player = ModeLayer::createBytype("", "请求数据中。。。", Type_Wait_Text);
    player->setTag(Tag_Pop_Layout);
    this->addChild(player,15);
    player->retain();
    auto paction = CallFunc::create([=]{
        if (player->getReferenceCount() == 2) {
            player->removeFromParent();
        }
        player->release();
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), paction));
    NetworkMgr::getInstance()->sendRequestUserInfo(INVALID_TABLE, m_pageIndex*GSWZ_PAGE_TABLE_COUNT);
}

void DeskLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_BT_Begin:
                {
                    HallDataMgr::getInstance()->AddpopLayer("", "正在进入游戏。。。", Type_Wait_Text);
                    NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_CHAIR_REQ, NULL, NULL);
                    //NetworkMgr::getInstance()->sendSitDown(INVALID_DWORD, INVALID_DWORD, "");
                }
                    break;
                case Tag_BT_Encrypt:
                {
                    this->encryptinit();
                }
                    break;
                case Tag_Encrypt_Ensure:
                {
                    std::string passstr = m_Encrpytedit->getText();
                    if (strLength(passstr) > LEN_PASSWORD - 1) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "密码过长，请重新输入", Type_Ensure);
                        return;
                    }
                    if (passstr.find(" ") != std::string::npos) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "密码中不能有空格，请重新输入", Type_Ensure);
                        return;
                    }
                    this->removeChildByTag(Tag_Encrypt);
                    NetworkMgr::getInstance()->sendEncrypt(passstr.c_str());
                    //                    this->removeChildByTag(100);
                    //                    auto ptext = Text::create(passstr, FontBold, 28);
                    //                    ptext->setPosition(Vec2(WinSize.width/2, WinSize.height - 50));
                    //                    ptext->setTag(100);
                    //                    this->addChild(ptext);
                    HallDataMgr::getInstance()->m_Tablepass = passstr;
                    HallDataMgr::getInstance()->saveTablepass(passstr);
                    m_Encrpytedit = nullptr;
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

void DeskLayer::pageViewEvent(cocos2d::Ref *pSender, cocos2d::ui::PageView::EventType type)
{
    auto pageview = (PageView *)pSender;
    int pageindex = (int)pageview->getCurPageIndex();
    if (m_pageIndex == pageindex) {
        return;
    }
    //    auto pdot1 = (ImageView *)this->getChildByTag(Tag_Dot+pageindex);
    //    auto pdot2 = (ImageView *)this->getChildByTag(Tag_Dot+m_pageIndex);
    //    pdot1->loadTexture("room_res/im_page_0.png");
    //    pdot2->loadTexture("room_res/im_page_1.png");
    
    auto playout = pageview->getPage(m_pageIndex);
    auto playout1 = pageview->getPage(pageindex);
    for (int index=0; index<GSWZ_PAGE_TABLE_COUNT; ++index) {
        auto pcell = (DeskCell *)playout->getChildByTag(index);
        pcell->clearUser();
        auto pcell1 = static_cast<DeskCell *>(playout1->getChildByTag(index));
        int pageID = pageindex*GSWZ_PAGE_TABLE_COUNT + index;
        pcell1->updateStatus(HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[pageID].cbTableLock,
                             HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[pageID].cbPlayStatus);    }
    m_UserList.clear();
    
    m_pageIndex = pageindex;
    this->userinfoUpdate(0);
}

void DeskLayer::OnUserEnter(void *pUser)
{
    auto puserData = (UserData *)pUser;
    this->removeChildByTag(Tag_Pop_Layout);
    if (puserData->m_date.cbUserStatus == US_FREE) {
        return;
    }
    if (puserData->m_date.wTableID == INVALID_TABLE) {
        return;
    }
    if (puserData->m_date.wTableID/GSWZ_PAGE_TABLE_COUNT != m_pageIndex) {
        return;
    }
    this->OnUserStatus(pUser);
}

void DeskLayer::OnUserStatus(void *pUser)
{
    auto puserData = (UserData *)pUser;
    if (puserData->m_date.cbUserStatus >= US_SIT) {
        auto playout = m_pageView->getPage(puserData->m_date.wTableID/GSWZ_PAGE_TABLE_COUNT);
        if (!playout) {
            return;
        }
        auto pcell = (DeskCell *)playout->getChildByTag(puserData->m_date.wTableID%GSWZ_PAGE_TABLE_COUNT);
        if (!pcell) {
            return;
        }
        pcell->userEnter(puserData, puserData->m_date.wChairID);
        auto pdeskuser = new DeskUserData(puserData->m_date.dwUserID, puserData->m_date.wTableID, puserData->m_date.wChairID);
        m_UserList.insert(puserData->m_date.dwUserID, pdeskuser);
    }
    else if (puserData->m_date.cbUserStatus == US_FREE)
    {
        auto deskuser = m_UserList.at(puserData->m_date.dwUserID);
        if (!deskuser) {
            return;
        }
        auto playout = m_pageView->getPage(deskuser->m_tableID/GSWZ_PAGE_TABLE_COUNT);
        if (!playout) {
            return;
        }
        auto pcell = (DeskCell *)playout->getChildByTag(deskuser->m_tableID%GSWZ_PAGE_TABLE_COUNT);
        if (!pcell) {
            return;
        }
        pcell->userLeave(deskuser->m_chairID);
        m_UserList.erase(deskuser->m_userID);
    }
}