//
//  RoomLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/5.
//
//大厅界面

#include "brnn_RoomLayer.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "SimpleAudioEngine.h"

#include "Plazz.h"
#include "CommonPlazaLayer.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;
using namespace brnn_game;

enum ButtonTag
{
    Tag_Room_Pass = 0,//房间密码层
    Tag_Room_List,//房间列表
    Tag_Room_Online_Txt,//房间在线文字
};

Scene* RoomLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RoomLayer::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool RoomLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    HallDataMgr::getInstance()->m_RoomType = Data_Load;
    cocos2d::Size viewsize = WinSize;
    
    m_commonPlaza = CommonPlazaLayer::create();
    this->addChild(m_commonPlaza);
    
    auto preturn = Button::create("common_res/common_return_0.png", "common_res/common_return_1.png");
    preturn->setPosition(cocos2d::Point(60, viewsize.height-50));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDBRNN/";
            std::vector<std::string> paths = FileUtils::getInstance()->getSearchPaths();
            for (std::vector<std::string>::iterator ite = paths.begin(); ite != paths.end(); )
            {
                if (0 == strcmp((*ite).c_str(), path.c_str()))
                {
                    ite = paths.erase(ite);
                }
                else
                {
                    ++ite;
                }
            }
            FileUtils::getInstance()->setSearchPaths(paths);
            
            NetworkMgr::getInstance()->Disconnect(Data_Load);
            //Director::getInstance()->replaceScene(TransitionFade::create(0.3f, LoginLayer::createScene()));
            TransFadeInPlazz;
        }
    });
    m_commonPlaza->addChild(preturn);
    
    auto roomlist = ListView::create();
    roomlist->setBounceEnabled(true);
    roomlist->setDirection(ScrollView::Direction::HORIZONTAL);
    roomlist->setContentSize(cocos2d::Size(viewsize.width,360));
    roomlist->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    roomlist->setPosition(cocos2d::Point(viewsize.width/2,160));
    roomlist->setItemsMargin(50.f);
    roomlist->setTag(Tag_Room_List);
    m_commonPlaza->addChild(roomlist);
    
    auto layout = Layout::create();
    layout->setContentSize(cocos2d::Size(50,roomlist->getContentSize().height));
    roomlist->pushBackCustomItem(layout);
    
    size_t all = HallDataMgr::getInstance()->m_subRoomList.size();
    int group = (int)all / 2;
    int idx = 0;
    std::vector<std::vector<tagGameServer*>> m_vecRoomGroup = std::vector<std::vector<tagGameServer*>>();
    for (int i = 0; i < group; ++i)
    {
        std::vector<tagGameServer *> vecGroup = std::vector<tagGameServer *>();
        for (int j = 0; j < 2; ++j)
        {
            vecGroup.push_back(HallDataMgr::getInstance()->m_subRoomList[idx]);
            ++idx;
        }
        m_vecRoomGroup.push_back(vecGroup);
    }
    std::vector<tagGameServer *> vecGroup = std::vector<tagGameServer *>();
    int left = (int)(all - idx);
    for (int i = 0; i < left; ++i)
    {
        vecGroup.push_back(HallDataMgr::getInstance()->m_subRoomList[idx]);
        ++idx;
    }
    if (!vecGroup.empty())
    {
        m_vecRoomGroup.push_back(vecGroup);
    }
    
    float posY[2] = {180.0f, 0.0f};
    for (size_t i = 0; i < m_vecRoomGroup.size(); ++i)
    {
        auto widget = Layout::create();
        for (size_t j = 0; j < m_vecRoomGroup[i].size(); ++j)
        {
            auto room = this->roominitwithinfo(m_vecRoomGroup[i][j]);
            widget->setContentSize(Size(room->getContentSize().width, room->getContentSize().height * 2));
            room->setPosition(Vec2(widget->getContentSize().width, widget->getContentSize().height * 0.5 + posY[j]));
            widget->addChild(room);
        }
        roomlist->pushBackCustomItem(widget);
    }
    //this->schedule(CC_CALLBACK_1(RoomLayer::updateOnline, this), 40.f, "updateonline");
    return true;
}

void RoomLayer::roompassinit()
{
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    m_commonPlaza->addChild(pbg);
    
    auto ppop = ImageView::create("public_res/pop_frame.png");
    ppop->setScale9Enabled(true);
    ppop->setContentSize(cocos2d::Size(500,300));
    ppop->setPosition(viewsize/2);
    pbg->addChild(ppop);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(ppop->getContentSize().width-45, ppop->getContentSize().height-45));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                    {
                                        if (type == Widget::TouchEventType::ENDED) {
                                            pbg->removeFromParent();
                                        }
                                    });
    ppop->addChild(pclose);
    
    auto ptext = Text::create("该房间已设置进入密码，请输入密码", FontBold, 22);
    ptext->setPosition(Vec2(70, ppop->getContentSize().height-70));
    ptext->setAnchorPoint(Vec2(0.f, 0.5f));
    ppop->addChild(ptext);
    
    auto ptitle = Text::create("房间密码：", FontBold, 22);
    ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
    ptitle->setPosition(Vec2(70, ppop->getContentSize().height-130));
    ppop->addChild(ptitle);
    
    auto pedit = EditBox::create(cocos2d::Size(250,59), "bank_res/edit_frame.png");
    pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    pedit->setPosition(Vec2(ptitle->getPositionX()+100, ptitle->getPositionY()));
    pedit->setInputFlag(EditBox::InputFlag::PASSWORD);
    pedit->setFontSize(28);
    ppop->addChild(pedit);
    
    auto pensure = Button::create("public_res/bt_ensure_0.png", "public_res/bt_ensure_1.png");
    pensure->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                    {
                                        if (type == Widget::TouchEventType::ENDED) {
                                            std::string passstr = pedit->getText();
                                            if (!passstr.size()) {
                                                HallDataMgr::getInstance()->AddpopLayer("提示", "密码为空，请输入密码", Type_Ensure);
                                                return;
                                            }
                                            if (strLength(passstr) > LEN_PASSWORD - 1) {
                                                HallDataMgr::getInstance()->AddpopLayer("提示", "密码过长，请重新输入", Type_Ensure);
                                                return;
                                            }
                                            if (passstr.find(" ") != std::string::npos) {
                                                HallDataMgr::getInstance()->AddpopLayer("提示", "密码中不能有空格，请重新输入", Type_Ensure);
                                                return;
                                            }
                                            pbg->removeFromParent();
                                            NetworkMgr::getInstance()->doConnect(HallDataMgr::getInstance()->m_roomaddress.c_str(), HallDataMgr::getInstance()->m_roomPort, Data_Room);
                                            NetworkMgr::getInstance()->sendRoomLogin(passstr);
                                            HallDataMgr::getInstance()->m_roomPass = passstr;
                                            HallDataMgr::getInstance()->AddpopLayer("", "正在进入房间。。。。", Type_Wait_Text);
                                        }
                                    });
    pensure->setPosition(Point(ppop->getContentSize().width/2, ppop->getContentSize().height - 230));
    ppop->addChild(pensure);
}

Widget * RoomLayer::roominitwithinfo(tagGameServer *server)
{
    int index = server->wServerLevel;
    index = index==0?1:index;
    index = index>10?10:index;
    
    auto im_name = __String::createWithFormat("room_res/bt_room_%d.png", index%4);
    auto widget = Widget::create();
    
    auto pbutton = Button::create(im_name->getCString(),im_name->getCString(),im_name->getCString());
    pbutton->addTouchEventListener(CC_CALLBACK_2(RoomLayer::touchEvent, this));
    pbutton->setTag(server->wServerID);
    pbutton->setUserData(server);
    widget->addChild(pbutton);
    widget->setContentSize(pbutton->getContentSize());
    
    auto text1 = Label::createWithSystemFont("底分:", FontBold, 20);
    text1->setColor(Color3B(93,199,230));
    text1->setAnchorPoint(Vec2(0, 0.5));
    text1->setPosition(Vec2(-44, 8));
    widget->addChild(text1);
    auto text2 = Label::createWithSystemFont("入场:", FontBold, 20);
    text2->setColor(Color3B(93,199,230));
    text2->setAnchorPoint(Vec2(0, 0.5));
    text2->setPosition(Vec2(-44, -16));
    widget->addChild(text2);
    auto text3 = Label::createWithSystemFont("在线人数:", FontBold, 20);
    text3->setColor(Color3B(93,199,230));
    text3->setAnchorPoint(Vec2(0, 0.5));
    text3->setPosition(Vec2(-44, -41));
    widget->addChild(text3);
    
    auto cellscore = Label::createWithSystemFont(__String::createWithFormat("%lld", server->lCellScore)->getCString(), FontBold, 20);
    cellscore->setPosition(Vec2(8, 8));
    cellscore->setAnchorPoint(Vec2(0.f, 0.5f));
    cellscore->setColor(Color3B(215,138,69));
    widget->addChild(cellscore);
    
    auto interscorestr = __String::createWithFormat("%lld", server->lEnterScore)->getCString();
    auto interscore = Label::createWithSystemFont(interscorestr, FontBold, 20);
    interscore->setColor(Color3B(215,138,69));
    interscore->setAnchorPoint(Vec2(0.f, 0.5f));
    interscore->setPosition(Vec2(8, -16));
    widget->addChild(interscore);
    
    auto onlinestr = __String::createWithFormat("%d", server->dwOnLineCount+server->dwAndroidCount);
    auto online = Label::createWithSystemFont(onlinestr->getCString(), FontBold, 20);
    online->setColor(Color3B(215,138,69));
    online->setPosition(Vec2(46, -41));
    online->setAnchorPoint(Vec2(0.f, 0.5f));
    online->setTag(Tag_Room_Online_Txt);
    widget->addChild(online);
    
    auto roomnamestr = __String::createWithFormat("room_res/txt_room_%d.png", index);
    auto roomname = Sprite::create(roomnamestr->getCString());
    roomname->setPosition(Vec2(18, 55));
    widget->addChild(roomname);
    return widget;
}

void RoomLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(RoomLayer::roomloginResult, this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(RoomLayer::roomlistResult, this));
}

void RoomLayer::onExit()
{
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_SERVER_LIST);
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    Layer::onExit();
}

void RoomLayer::updateOnline(float t)
{
    this->sendOnline();
}

void RoomLayer::updateOnlineShow()
{
    auto roomlist = static_cast<ListView *>(this->getChildByTag(Tag_Room_List));
    if (!roomlist) {
        return;
    }
    auto pItems = roomlist->getItems();
    for (auto pbutton :  pItems) {
        for (auto iter : HallDataMgr::getInstance()->m_roomList) {
            if (iter->wServerID == pbutton->getTag()){
                auto ptxt = static_cast<Text *>(pbutton->getChildByTag(Tag_Room_Online_Txt));
                ptxt->setString(__String::createWithFormat("%d", (iter->dwOnLineCount+iter->dwAndroidCount))->getCString());
            }
        }
    }
}

void RoomLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto button = (Button *)pSender;
            auto roomdata = (tagGameServer *)button->getUserData();
            std::string ipaddress = WHConverUnicodeToUtf8WithArray(roomdata->szServerAddr);
            HallDataMgr::getInstance()->m_roomaddress = ipaddress;
            HallDataMgr::getInstance()->m_roomPort = roomdata->wServerPort;
            HallDataMgr::getInstance()->m_Gametype = roomdata->wServerType;
            HallDataMgr::getInstance()->m_RoomRule = roomdata->dwServerRule;
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/enter.wav");
            if ((roomdata->wServerKind&SERVER_GENRE_PASSWD) != 0) {
                this->roompassinit();
            }
            else
            {
                NetworkMgr::getInstance()->doConnect(ipaddress.c_str(), roomdata->wServerPort, Data_Room);
                HallDataMgr::getInstance()->m_roomPass = "";
                NetworkMgr::getInstance()->sendRoomLogin("");
                HallDataMgr::getInstance()->AddpopLayer("", "正在进入房间。。。。", Type_Wait_Text);
            }
        }
            break;
            
        default:
            break;
    }
}

void RoomLayer::sendPacketWithUserLevelInfo()
{
    CMD_GP_GrowLevelQueryInfo levelQueryInfo;
    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GrowLevelQueryInfo));
    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, levelQueryInfo.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}


void RoomLayer::sendOnline()
{
    CMD_MB_GetOnline getonline;
    memset(&getonline, 0, sizeof(getonline));
    
    getonline.wServerCount = HallDataMgr::getInstance()->m_roomList.size();
    int index = 0;
    for (auto iter : HallDataMgr::getInstance()->m_roomList) {
        getonline.wOnLineServerID[index++] = iter->wServerID;
    }
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_MB_SERVER_LIST, SUB_MB_GET_ONLINE, &getonline, sizeof(WORD)*(getonline.wServerCount+1),NetworkMgr::getInstance()->getSocketOnce());
}

void RoomLayer::roomloginResult(WORD wSubCmdID, void *pData, WORD wSize)
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
            HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
            NetworkMgr::getInstance()->Disconnect(Data_Room);
        }
            break;
        case SUB_GR_LOGON_FINISH:
        {
            HallDataMgr::getInstance()->readTablepass();
            if (HallDataMgr::getInstance()->m_Tablepass != "") {
                //桌子可以加锁
                if ((HallDataMgr::getInstance()->m_RoomRule&SR_FORFEND_LOCK_TABLE) == 0) {
                    NetworkMgr::getInstance()->sendEncrypt(HallDataMgr::getInstance()->m_Tablepass);
                }
            }
            
            auto status = HallDataMgr::getInstance()->m_cbStatus;
            //跳到桌子界面
            if (status==US_FREE || status==US_NULL) {
                NetworkMgr::getInstance()->sendSitDown(0, 100, HallDataMgr::getInstance()->m_pPassword);
            }
            //游戏状态启动游戏
            else if(status==US_PLAYING || status==US_READY)
            {
                NetworkMgr::getInstance()->sendGameOption();
            }
        }
            break;
        case SUB_GR_UPDATE_NOTIFY:
        {
            HallDataMgr::getInstance()->AddpopLayer("系统提示", "当前版本不是最新版本，请升级到最新版本", Type_Ensure);
            auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Room);});
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
        }
            break;
            
        default:
            break;
    }
}

void RoomLayer::roomlistResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    if (wSubCmdID == SUB_MB_SERVER_ONLINE)
    {
        auto ponline = static_cast<CMD_MB_ServerOnline *>(pData);
        for (int index=0; index<ponline->wServerCount; ++index) {
            for (auto iter : HallDataMgr::getInstance()->m_roomList) {
                if (iter->wServerID == ponline->OnLineInfoServer[index].wServerID) {
                    iter->dwOnLineCount = ponline->OnLineInfoServer[index].dwOnLineCount;
                }
            }
        }
        this->updateOnlineShow();
        NetworkMgr::getInstance()->Disconnect(Data_Load);
    }
}
