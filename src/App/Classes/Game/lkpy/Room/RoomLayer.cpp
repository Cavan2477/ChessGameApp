//
//  RoomLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/5.
//
//大厅界面

#include "RoomLayer.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "DeskLayer.h"
#include "GameClearLayer.h"
#include "Plazz.h"
#include "Help.h"
#include "CommonPlazaLayer.h"
USING_NS_CC;
using namespace ui;
using namespace std;
USING_LKPY_NAMESPACE;
enum ButtonTag
{
    Tag_Room_Pass = 0,//房间密码层
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
    //GameClearLayer* layer = GameClearLayer::create();
    //this->addChild(layer,100);
    
    HallDataMgr::getInstance()->m_isFirstSendGameOption = false;
    HallDataMgr::getInstance()->m_RoomType = Data_Load;
    cocos2d::Size viewsize = WinSize;
//    auto pbg = ImageView::create("room_res/room_bg.png");
//    pbg->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
//    this->addChild(pbg);
    
    auto plazaLayer = CommonPlazaLayer::create();
    this->addChild(plazaLayer);
    
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(80, viewsize.height-55));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            
            std::string path = FileUtils::getInstance()->getWritablePath() + "JDLkpy/";
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
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Plazz::createScene()));
        }
    });
    plazaLayer->addChild(preturn);
    

    //m_subRoomList
    float listWidth = viewsize.width;
    size_t roomCount = HallDataMgr::getInstance()->m_subRoomList.size();
    //roomCount = 3;
    if(roomCount > 4)
    {
        if(roomCount % 4 == 0)
            listWidth = viewsize.width * (roomCount / 4);
        else
            listWidth = viewsize.width * (roomCount / 4 + 1);
    }
    
    
    auto roomlist = ScrollView::create();
    roomlist->setBounceEnabled(true);
    roomlist->setDirection(ScrollView::Direction::HORIZONTAL);
    roomlist->setContentSize(cocos2d::Size(viewsize.width,350));
    roomlist->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    roomlist->setPosition(cocos2d::Point(viewsize.width/2,170));
    roomlist->setInnerContainerSize(cocos2d::Size(listWidth,350));
    //roomlist->setItemsMargin(20.f);
    plazaLayer->addChild(roomlist);
    
//    auto layout = Layout::create();
//    layout->setContentSize(cocos2d::Size(20,170));
//    roomlist->pushBackCustomItem(layout);
    float fSpace = 515;
    float fStartPosX = 90;
    float fStartPosY = 90;
    int modNum = 0;
    int divideNum = 0;
    int subModNum = 0;
    for(int i = 1;i <= roomCount;i++)
    {
        
        modNum = i % 4;
        divideNum = i / 4;
        
        if(modNum  ==  0)
        {
            modNum = 4;
            divideNum -=1;
        }
        subModNum = modNum % 2;
        if(subModNum == 0)
            subModNum = 2;
        fStartPosX = 330 + divideNum * viewsize.width;

        fStartPosY = 260 - (modNum / 3) * 180;
        auto room = this->roominitwithinfo(HallDataMgr::getInstance()->m_subRoomList[i-1]);
        Vec2 pos(fStartPosX + (subModNum - 1) * fSpace,fStartPosY);
        room->setPosition(pos);
        roomlist->addChild(room);
    }
    
    
    return true;
}

void RoomLayer::roompassinit()
{
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    this->addChild(pbg);
    
    auto ppop = ImageView::create("public_res/pop_frame.png");
    ppop->setScale9Enabled(true);
    ppop->setContentSize(cocos2d::Size(500,280));
    ppop->setPosition(viewsize/2);
    pbg->addChild(ppop);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(ppop->getContentSize().width-20, ppop->getContentSize().height-20));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                    {
                                        if (type == Widget::TouchEventType::ENDED) {
                                            pbg->removeFromParent();
                                        }
                                    });
    ppop->addChild(pclose);
    
    auto ptext = Text::create("该房间已设置进入密码，请输入密码", FontBold, 22);
    ptext->setPosition(Vec2(70, ppop->getContentSize().height-60));
    ptext->setAnchorPoint(Vec2(0.f, 0.5f));
    ppop->addChild(ptext);
    
    auto ptitle = Text::create("房间密码：", FontBold, 22);
    ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
    ptitle->setPosition(Vec2(70, ppop->getContentSize().height-120));
    ppop->addChild(ptitle);
    
    auto pedit = EditBox::create(cocos2d::Size(250,59), "login_res/register_frame.png");
    pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    pedit->setPosition(Vec2(ptitle->getPositionX()+100, ptitle->getPositionY()));
    pedit->setInputFlag(EditBox::InputFlag::PASSWORD);
    pedit->setFontSize(28);
    ppop->addChild(pedit);
    
    auto pensure = Button::create("login_res/bt_ensure_0.png", "login_res/bt_ensure_1.png");
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
                                            this->sendRoomLogin(passstr);
                                            HallDataMgr::getInstance()->AddpopLayer("", "正在进入房间。。。。", Type_Wait_Text);
                                        }
                                    });
    pensure->setPosition(Point(ppop->getContentSize().width/2, ppop->getContentSize().height - 220));
    ppop->addChild(pensure);
}

Widget * RoomLayer::roominitwithinfo(tagGameServer *server)
{
    srand(time(NULL));
    int index = rand()%3;
    auto im_name0 = __String::createWithFormat("room_res/bt_room%d_0.png", index);
    auto im_name1 = __String::createWithFormat("room_res/bt_room%d_1.png", index);
    auto pbutton = Button::create(im_name0->getCString());
    pbutton->addTouchEventListener(CC_CALLBACK_2(RoomLayer::touchEvent, this));
    pbutton->setUserData(server);
    pbutton->loadTextures(im_name0->getCString(), im_name0->getCString());
    auto txtroom = ImageView::create("room_res/txt_room.png");
    txtroom->setPosition(cocos2d::Point(pbutton->getContentSize().width/2, 150));
    pbutton->addChild(txtroom);
    
    auto cellTip = Text::create("倍数:", FontBold, 18);
    cellTip->setColor(Color3B(59,144,183));
    cellTip->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    cellTip->setPosition(cocos2d::Point(176, 100));
    pbutton->addChild(cellTip);
    
    auto cellscorestr = __String::createWithFormat("%lld倍",server->lCellScore);
    auto cellscore = Text::create(cellscorestr->getCString(), FontBold, 18);
    cellscore->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    cellscore->setColor(Color3B(179,124,77));
    cellscore->setPosition(cocos2d::Point(cellTip->getPositionX() + cellTip->getContentSize().width +2, cellTip->getPositionY()));
    pbutton->addChild(cellscore);
    
    auto interTip = Text::create("入场:", FontBold, 18);
    interTip->setColor(Color3B(59,144,183));
    interTip->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    interTip->setPosition(cocos2d::Point(176, 75));
    pbutton->addChild(interTip);
    
    auto interscorestr = __String::createWithFormat("%lld游戏币",server->lEnterScore);
    auto interscore = Text::create(interscorestr->getCString() , FontBold, 18);
    interscore->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    interscore->setColor(Color3B(151,145,57));
    interscore->setPosition(cocos2d::Point(interTip->getPositionX() + interTip->getContentSize().width +2, interTip->getPositionY()));
    pbutton->addChild(interscore);
    
    
    
    
    
    auto onlineTip = Text::create("在线人数:", FontBold, 18);
    onlineTip->setColor(Color3B(59,144,183));
    onlineTip->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    onlineTip->setPosition(cocos2d::Point(176, 50));
    pbutton->addChild(onlineTip);
    
    auto onlinestr = __String::createWithFormat("%d人", server->dwOnLineCount+server->dwAndroidCount);
    auto online = Text::create(onlinestr->getCString(), FontBold, 18);
    online->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    online->setColor(Color3B(89,242,119));
    online->setPosition(cocos2d::Point(onlineTip->getPositionX() + onlineTip->getContentSize().width +2, onlineTip->getPositionY()));
    pbutton->addChild(online);
    
    int roomlevel = server->wServerLevel;
    if (roomlevel>6 || roomlevel==0) {
        roomlevel = 1;
    }
    auto roomnamestr = __String::createWithFormat("room_res/txt_room_%d.png", roomlevel);
    auto roomname = ImageView::create(roomnamestr->getCString());
    roomname->setPosition(Vec2(pbutton->getContentSize().width/2+18, 155));
    pbutton->addChild(roomname);
    return pbutton;
}

void RoomLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_loadtype != Load_Visitor) {
        this->sendPacketWithUserLevelInfo();
    }
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(RoomLayer::roomloginResult, this));
}

void RoomLayer::onExit()
{
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    Layer::onExit();
}

void RoomLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto button = (Button *)pSender;
            auto roomdata = (tagGameServer *)button->getUserData();
            std::string ipaddress = WHConverUnicodeToUtf8WithArray(roomdata->szServerAddr);
            //NetworkMgr::getInstance()->Disconnect(Data_Load);
            NetworkMgr::getInstance()->doConnect(ipaddress.c_str(), roomdata->wServerPort, Data_Room);
            HallDataMgr::getInstance()->m_Gametype = roomdata->wServerType;
            HallDataMgr::getInstance()->m_RoomRule = roomdata->dwServerRule;
            if ((roomdata->wServerKind&SERVER_GENRE_PASSWD) != 0) {
                this->roompassinit();
            }
            else
            {
                this->sendRoomLogin("");
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
//    CMD_GP_GrowLevelQueryInfo levelQueryInfo;
//    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GrowLevelQueryInfo));
//    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
//    UTF8Str_To_UTF16Str((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), levelQueryInfo.szMachineID);
//    UTF8Str_To_UTF16Str((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), levelQueryInfo.szPassword);
//    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
//    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
    
    CMD_GP_GrowLevelQueryInfo levelQueryInfo;
    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GrowLevelQueryInfo));
    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), levelQueryInfo.szMachineID);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}

void RoomLayer::sendRoomLogin(const std::string &roompass)
{
    CMD_GR_LogonMobile lm;
    memset(&lm, 0, sizeof(lm));
    
    lm.wGameID = HallDataMgr::getInstance()->m_dwKindID;

    lm.dwProcessVersion = PROCESS_VERSION(6,0,3);

    lm.wBehaviorFlags = HallDataMgr::getInstance()->m_bStartType?(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_ALL):(BEHAVIOR_LOGON_NORMAL|VIEW_MODE_ALL);
    lm.wPageTableCount = PAGE_TABLE_COUNT;
    lm.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    lm.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), lm.szMachineID);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_dynamicpass.c_str(), lm.szDynamicPassword);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)roompass.c_str(), lm.szServerPasswd);
    
    NetworkMgr::getInstance()->sendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &lm, sizeof(lm));
    log("%s","房间登录");
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
                Director::getInstance()->replaceScene(TransitionFade::create(0.3f, DeskLayer::createScene()));
            }
            //游戏状态启动游戏
            else if(status==US_PLAYING || status==US_READY)
            {
                if(!HallDataMgr::getInstance()->m_isFirstSendGameOption)
                {
                    NetworkMgr::getInstance()->sendGameOption();
                }
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
