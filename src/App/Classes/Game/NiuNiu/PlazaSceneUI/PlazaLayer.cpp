#include "PlazaLayer.h"
#include "GameDataMgr.h"
#include "NiuNiuScene.h"
/*#include "../Exchange/ExchangeLayer.h"
#include "../Person/PersonInfoLayer.h"*/

USING_NS_CC;
using namespace ui;
USING_NN_NAMESPACE;

//大厅
#include "Plazz.h"
USING_NS_CC;
using namespace ui;
USING_NN_NAMESPACE;

enum plazaTAG
{
    TAG_BASE = 100,
    TAG_BACK_BTN,
    TAG_TASK_BTN,
    TAG_BANK_BTN,
    TAG_REWARD_BTN,
    TAG_HELP_BTN,
    TAG_SET_BTN,
    TAG_ADD_COIN,
    TAG_ADD_JINDOU,
    TAG_LEVEL_BAR,
    TAG_LEVEL_DATA,
    TAG_FASTENTER,
    TAG_LOCKDESK
};
#define GameMaxLevel   60  //游戏最高等级

//z轴
const int PLAZA_CONTROL_ZORDER = 1;

PlazaLayer::PlazaLayer():
m_btnLayout(nullptr),
m_roomList(nullptr),
m_tableList(nullptr),
m_bEnterTableList(false),
m_notify(nullptr),
m_passwdLayer(nullptr),
m_settingLayer(nullptr),
m_clipNickName(nullptr)
{
    
}

PlazaLayer::~PlazaLayer()
{
    
}

bool PlazaLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        //initPlazeLayout();
        //加载通用大厅资源
        m_commonPlaza = CommonPlazaLayer::create();
        CC_ASSERT(m_commonPlaza != nullptr);
        this->addChild(m_commonPlaza);
        
        auto plazzLayout = CSLoader::createNode("plaza/PlazaLayer.csb");
        CC_ASSERT(plazzLayout != nullptr);
        m_btnBack = static_cast<Button*>(plazzLayout->getChildByName("back_btn"));
        CC_ASSERT(m_btnBack != nullptr);
        m_btnBack->setTag(TAG_BACK_BTN);
        m_btnBack->addTouchEventListener(CC_CALLBACK_2(PlazaLayer::touchEvent, this));
        
        m_tableMenuLayout = static_cast<Layout*>(plazzLayout->getChildByName("table_menu"));
        CC_ASSERT(m_tableMenuLayout != nullptr);
        m_commonPlaza->addChild(plazzLayout);
        initTableListInfo();
        
        m_roomList = RoomListLayer::createLayer(this);
        CC_BREAK_IF(nullptr == m_roomList);
        
        //m_roomList->setPosition(0,100);
        m_roomList->setPosition(68, 140);
        m_commonPlaza->addChild(m_roomList);
        
        HallDataMgr::getInstance()->m_RoomType = Data_Load;
        if (INSTANCE(GameDataMgr)->getEnterNormalRoom())
        {
            HallDataMgr::getInstance()->m_RoomType = Data_Room;
            INSTANCE(GameDataMgr)->setEnterNormalRoom(false);
            INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
            
            showTableList(true);
            refreshTableList();
            refreshCurrentTableInfo();
        }
        else
        {
            HallDataMgr::getInstance()->clearUserData();
            //确保断开房间连接
            HallDataMgr::getInstance()->m_RoomType = Data_Load;
            //断开网络连接
            NetworkMgr::getInstance()->Disconnect(Data_Room);
        }
        
        /*tagGameServer *dataitem = HallDataMgr::getInstance()->m_subRoomList[0];
         if (nullptr != dataitem)
         {
         INSTANCE(GameDataMgr)->setEnterRoom(0);
         uiMsg(UI_MSG_BASE::UI_MSG_PLAZA,PLAZA_UI::ENTER_ROOM,dataitem,sizeof(tagGameServer));
         }*/
        bRes = true;
    } while (false);
    return bRes;
}

bool PlazaLayer::uiMsg(const int &main, const int &sub,void *pData, const int &nLen)
{
    bool bRes = false;
    switch (main)
    {
        case UI_MSG_BASE::UI_MSG_PLAZA:
            bRes = handlePlazaMsg(sub,pData,nLen);
            break;
        default:
            break;
    }
    return bRes;
}

void PlazaLayer::refreshTableList()
{
    if (!INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        m_roomList->setVisible(false);
        m_commonPlaza->hideUserInfo(true);
        
        createTableList();
        
        m_tableList->refreshTableList();
    }
}

void PlazaLayer::refreshCurrentTableInfo()
{
    if (!INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        createTableList();
        
        m_tableList->refreshCurrentPageInfo();
    }
}

void PlazaLayer::refreshTableInfo(tagTableItem *pItem)
{
    if (!INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        createTableList();
        
        m_tableList->refreshTableInfo(pItem);
    }
}

void PlazaLayer::refreshTableUser(const tagTableUserInfo &tagUser)
{
    if (!INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        createTableList();
        
        m_tableList->refreshTableUser(tagUser);
    }
}

void PlazaLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    Widget *pWidget = static_cast<Widget*>(pSender);
    if (nullptr == pWidget)
    {
        return;
    }
    
    if (Widget::TouchEventType::BEGAN == type)
    {
        switch (pWidget->getTag())
        {
            case TAG_LEVEL_BAR:
                this->showLevelInfo(true);
                break;
            default:
                break;
        }
    }
    else if (Widget::TouchEventType::ENDED == type)
    {
        INSTANCE(AudioMgr)->playClickAudio();
        switch (pWidget->getTag())
        {
            case TAG_BACK_BTN:
            {
                if (m_bEnterTableList)
                {
                    m_bEnterTableList = false;
                    //退出普通房间
                    showTableList(false);
                    INSTANCE(GameDataMgr)->setEnterRoom(-1);
                    
                    HallDataMgr::getInstance()->saveTablepass("");
                    HallDataMgr::getInstance()->m_RoomType = Data_Load;
                    //断开网络连接
                    NetworkMgr::getInstance()->Disconnect(Data_Room);
                    NetworkMgr::getInstance()->Disconnect(Data_Load);
                }
                else
                {
                    INSTANCE(GameDataMgr)->setEnterNormalRoom(false);
                    
                    HallDataMgr::getInstance()->m_RoomType = Data_Load;
                    //断开网络连接
                    NetworkMgr::getInstance()->Disconnect(Data_Room);
                    NetworkMgr::getInstance()->Disconnect(Data_Load);
                    /*
                     //场景切换
                     TransitionScene *reScene = NULL;
                     Scene *s = LoginScene::create();
                     reScene = TransitionSlideInL::create(TIME_SCENE_CHANGE, s);
                     Director::getInstance()->replaceScene(reScene);
                     */
                    //INSTANCE(SceneMgr)->transitionScene(DDZ_SCENE(LOGIN_SCENE),false);
                    PlazaScene* parent = static_cast<PlazaScene*>(m_notify);
                    if (nullptr != parent)
                    {
                        parent->setBackToPlazz(true);
                    }
                    TransFadeInPlazz;
                }
                
            }
                break;
            case TAG_TASK_BTN:
            {
                /*
                 TaskScene *scene = TaskScene::create();
                 auto reScene = TransitionSlideInL::create(TIME_SCENE_CHANGE, scene);
                 Director::getInstance()->replaceScene(reScene);
                 */
                //INSTANCE(SceneMgr)->transitionScene(DDZ_SCENE(TASK_SCENE),false);
            }
                break;
            case TAG_BANK_BTN:
            {
                /*
                 BankScene *scene = BankScene::create();
                 auto reScene = TransitionSlideInL::create(TIME_SCENE_CHANGE, scene);
                 Director::getInstance()->replaceScene(reScene);
                 */
                if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor)
                {
                    //2016-1-28 解除游客登录限制
                    //HallDataMgr::getInstance()->AddpopLayer("系统提示", "游客禁止操作银行，请注册帐号", Type_Ensure);
                    //return;
                }
                if (INSTANCE(GameDataMgr)->getEnterNormalRoom()
                    && INSTANCE(GameDataMgr)->getEnterRoom() >= 0
                    && INSTANCE(GameDataMgr)->getEnterRoom() < HallDataMgr::getInstance()->m_subRoomList.size())
                {
                    tagGameServer *pServer = HallDataMgr::getInstance()->m_subRoomList.at(INSTANCE(GameDataMgr)->getEnterRoom());
                    if (nullptr != pServer)
                    {
                        if (pServer->wServerType == GAME_GENRE_SCORE || pServer->wServerType == GAME_GENRE_EDUCATE)
                        {
                            HallDataMgr::getInstance()->AddpopLayer("系统提示", "该房间禁止进行银行操作", Type_Ensure);
                            return;
                        }
                    }
                }
                //INSTANCE(SceneMgr)->transitionScene(DDZ_SCENE(BANK_SCENE),false);
            }
                break;
            case TAG_REWARD_BTN:
            {
                /*
                 CheckInScene *scene = CheckInScene::create();
                 auto reScene = TransitionSlideInL::create(TIME_SCENE_CHANGE, scene);
                 Director::getInstance()->replaceScene(reScene);
                 */
                /*
                 INSTANCE(SceneMgr)->transitionScene(CHECKIN_SCENE);
                 */
                if (m_tableList)
                {
                    m_tableList->enableTableList(false);
                }
                /*auto check = CheckInLayer::createLayerLayer(this);
                this->addChild(check);*/
            }
                break;
            case TAG_HELP_BTN:
            {
                Scene *scene = HelpLayer::createScene();
                auto reScene = SCENE_TRANSITION(scene);
                Director::getInstance()->replaceScene(reScene);
            }
                break;
            case TAG_SET_BTN:
            {
                if (nullptr == m_settingLayer)
                {
                    m_settingLayer = SettingLayer::create();
                    m_settingLayer->setNotify(this);
                    this->addChild(m_settingLayer,PLAZA_CONTROL_ZORDER);
                }
                if (m_tableList)
                {
                    m_tableList->enableTableList(false);
                }
                m_settingLayer->showLayer(true);
            }
                break;
            case TAG_ADD_COIN:
            {
                /*auto lay = ExchangeLayer::createExchangeLayer(ExchangeLayer::emExchangeType::kEXCHANGE_DUIHUAN);
                lay->setNotify(this);
                if (m_tableList)
                {
                    m_tableList->enableTableList(false);
                }
                this->addChild(lay);*/
            }
                break;
            case TAG_ADD_JINDOU:
            {
                HallDataMgr::getInstance()->AddpopLayer("提示", "商场暂未开放，敬请期待...", Type_Ensure);
                return;
            }
                break;
            case TAG_LEVEL_BAR:
                this->showLevelInfo(false);
                break;
            case TAG_FASTENTER:
            {
                if (0 == HallDataMgr::getInstance()->m_UserScore)
                {
                    HallDataMgr::getInstance()->AddpopLayer("提示", "游戏币不足", Type_Ensure);
                    return;
                }
                if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
                {
                    HallDataMgr::getInstance()->AddpopLayer("提示", "", Type_Wait);
                    //此时是进入普通房间
                    INSTANCE(GameDataMgr)->setEnterNormalRoom(true);
                    INSTANCE(GameDataMgr)->randomSit();
                    //INSTANCE(GameDataMgr)->requestSitdown(INVALID_DWORD, INVALID_DWORD);
                }
            }
                break;
            case TAG_LOCKDESK:
            {
                initPasswdLayout();
                m_passwdLayer->showPasswd(true);
                m_passwdLayer->refreshNotice("请输入桌子进入密码。", PasswdLayer::emPasswdType::kSET_PASSWD);
            }
                break;
            default:
                break;
        }
    }
    else if (Widget::TouchEventType::CANCELED == type)
    {
        switch (pWidget->getTag())
        {
            case TAG_LEVEL_BAR:
                this->showLevelInfo(false);
                break;
            default:
                break;
        }
    }
}

void PlazaLayer::levelchange(void *pData, WORD wSize)
{
    m_commonPlaza->LevelInfo(pData, wSize);
}

void PlazaLayer::scorechange(void *pData, WORD wSize)
{
    m_commonPlaza->LevelUpgrade(pData, wSize);
}

void PlazaLayer::notifyRefreshScore(cocos2d::EventCustom *event)
{
    m_commonPlaza->notifyFreshInfo(event);
}

void PlazaLayer::initPlazeLayout()
{
    //initBtn();
    
    //initUserInfo();
    
    initTableListInfo();
}

void PlazaLayer::initTableListInfo()
{
    m_tableMenuLayout->setVisible(false);
    //快速进入
    Button *csbBtn = static_cast<Button*>(m_tableMenuLayout->getChildByName("fastenter_btn"));
    CC_ASSERT(csbBtn != nullptr);
    csbBtn->setTag(TAG_FASTENTER);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(PlazaLayer::touchEvent,this));
    //桌子加密
    csbBtn = static_cast<Button*>(m_tableMenuLayout->getChildByName("locktable_btn"));
    CC_ASSERT(csbBtn != nullptr);
    csbBtn->setTag(TAG_LOCKDESK);
    csbBtn->addTouchEventListener(CC_CALLBACK_2(PlazaLayer::touchEvent,this));
}

void PlazaLayer::initPasswdLayout()
{
    if (nullptr == m_passwdLayer)
    {
        m_passwdLayer = PasswdLayer::create();
        this->addChild(m_passwdLayer,PLAZA_CONTROL_ZORDER);
    }
}

void PlazaLayer::createTableList()
{
    if (nullptr == m_tableList)
    {
        m_tableList = TableListLayer::create(this);
        m_tableList->setPosition(s_vec2TableListPos);
        m_commonPlaza->addChild(m_tableList);
    }
}

void PlazaLayer::showTableList(bool bShow)
{/*
  m_btnReward->setVisible(!bShow);
  m_btnReward->setEnabled(!bShow);
  if (bShow)
  {
  m_btnTask->setPosition(m_vcRewardPos);
  }
  else
  {
  m_btnReward->setPosition(m_vcRewardPos);
  m_btnTask->setPosition(m_vcTaskPos);
  }*/
    
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        AntiCheatLayer *pLayer = AntiCheatLayer::createLayer(this);
        this->addChild(pLayer);
    }
    else
    {
        m_roomList->setVisible(!bShow);
        m_commonPlaza->hideUserInfo(bShow);
        m_bEnterTableList = bShow;
        
        createTableList();
        m_tableList->showTableList(bShow);
        m_tableMenuLayout->setVisible(bShow);
    }
}

void PlazaLayer::enterGameScene()
{
    if (m_notify)
    {
        m_notify->uiMsg(UI_MSG_BASE::UI_MSG_PLAZA,PLAZA_UI::ENTER_GAME,nullptr,0);
    }
}

bool PlazaLayer::handlePlazaMsg(const int &sub,void *pData,const int &nLen)
{
    bool bRes = false;
    do
    {
        switch (sub)
        {
            case PLAZA_UI::ENTER_ROOM:
            {
                tagGameServer *pItem = (tagGameServer*)pData;
                
                HallDataMgr::getInstance()->m_RoomType = Data_Room;
                INSTANCE(GameDataMgr)->connectRoom(pItem);
                if ((pItem->wServerKind & SERVER_GENRE_PASSWD) != 0) //房间需要输入密码
                {
                    initPasswdLayout();
                    m_passwdLayer->showPasswd(true);
                    if ((pItem->dwServerRule & SR_ALLOW_AVERT_CHEAT_MODE) != 0)	//防作弊房间
                    {
                        INSTANCE(GameDataMgr)->setSitTable(INVALID_TABLE);
                        INSTANCE(GameDataMgr)->setSitChair(INVALID_CHAIR);
                        INSTANCE(GameDataMgr)->setEnterAntiCheat(true);
                        m_passwdLayer->refreshNotice("该房间已设置密码，请输入密码", PasswdLayer::emPasswdType::kENTER_ANTICHEAT);
                    }
                    else //普通房间
                    {
                        m_passwdLayer->refreshNotice("该房间已设置密码，请输入密码", PasswdLayer::emPasswdType::kENTER_LIANXIROOM);
                        INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
                    }
                }
                else
                {
                    if ((pItem->dwServerRule & SR_ALLOW_AVERT_CHEAT_MODE) != 0)	//防作弊房间
                    {
                        INSTANCE(GameDataMgr)->setSitTable(INVALID_TABLE);
                        INSTANCE(GameDataMgr)->setSitChair(INVALID_CHAIR);
                        INSTANCE(GameDataMgr)->setEnterAntiCheat(true);
                    }
                    else //普通房间
                    {
                        INSTANCE(GameDataMgr)->setEnterAntiCheat(false);
                    }
                    INSTANCE(GameDataMgr)->enterRoom("");
                    //INSTANCE(LoadingMgr)->progressLoading(70);
                    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                }
            }
                break;
            case PLAZA_UI::SIT_DOWN:
            {
                //此时是进入普通房间
                INSTANCE(GameDataMgr)->setEnterNormalRoom(true);
                //坐下请求
                tagTableParam *pParam = (tagTableParam*)pData;
                if (nullptr != pParam)
                {
                    INSTANCE(GameDataMgr)->setSitTable(pParam->wTableId);
                    INSTANCE(GameDataMgr)->setSitChair(pParam->nChair);
                    
                    bool bLocked = false;
                    bool bPlaying = false;
                    //判断桌子是否有效
                    if (INSTANCE(TableMgr)->isTableValid(pParam->wTableId, bLocked, bPlaying))
                    {
                        if (bPlaying)
                        {
                            log("playing");
                        }
                        if (bLocked)
                        {
                            initPasswdLayout();
                            m_passwdLayer->showPasswd(true);
                            m_passwdLayer->refreshNotice("该桌子已设置密码，请输入密码", PasswdLayer::emPasswdType::kINPUT_TABLE_PASS);
                        }
                        else
                        {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "", Type_Wait);
                            INSTANCE(GameDataMgr)->requestSitdown(pParam->wTableId, pParam->nChair);
                        }
                    }
                }
            }
                break;
            case PLAZA_UI::CLICK_DESK:
            {
                bRes = !m_roomList->isVisible();
            }
                break;
            case PLAZA_UI::REMOVE_TOP_LAYER:
            {
                if (m_tableList)
                {
                    m_tableList->enableTableList(true);
                }
            }
                break;
            default:
                break;
        }
        
        bRes = true;
    } while (false);
    return bRes;
}

void PlazaLayer::showLevelInfo(bool show)
{
    if (show && this->getChildByTag(TAG_LEVEL_DATA))
    {
        return;
    }
    if (!show)
    {
        this->removeChildByTag(TAG_LEVEL_DATA);
        return;
    }
    if (show)
    {
        auto pbg = ImageView::create("pop_frame_level.png",Widget::TextureResType::PLIST);
        pbg->setScale9Enabled(true);
        pbg->setTag(TAG_LEVEL_DATA);
        pbg->setContentSize(cocos2d::Size(500,190));
        pbg->setPosition(Vec2(300, 190));
        this->addChild(pbg,2);
        float fXpos = 20.f;
        
        auto currPerience = Text::create(__String::createWithFormat("*当前经验/升级经验：%d/%d", HallDataMgr::getInstance()->m_levelData.dwExperience, HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience)->getCString(), FontBold, 25);
        currPerience->setTextColor(Color4B::WHITE);
        currPerience->setAnchorPoint(Vec2(0.f, 0.f));
        currPerience->setPosition(Vec2(fXpos, pbg->getContentSize().height-50));
        pbg->addChild(currPerience);
        
        std::string plevelcontent;
        if (HallDataMgr::getInstance()->m_levelData.wCurrLevelID >= GameMaxLevel)
        {
            plevelcontent = __String::createWithFormat("*你现在的等级是%d,已达到最高等级", HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString();
        }
        else
        {
            plevelcontent = __String::createWithFormat("*您现在等级是%d,升级还需%d经验值", HallDataMgr::getInstance()->m_levelData.wCurrLevelID, (HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience-HallDataMgr::getInstance()->m_levelData.dwExperience))->getCString();
        }
        auto currlevel = Text::create(plevelcontent, FontBold, 25);
        currlevel->setTextColor(Color4B::WHITE);
        currlevel->setAnchorPoint(Vec2(0.f, 0.f));
        currlevel->setPosition(Vec2(fXpos, pbg->getContentSize().height-110));
        pbg->addChild(currlevel);
        
        if (HallDataMgr::getInstance()->m_levelData.wCurrLevelID < GameMaxLevel)
        {
            auto upgrade = Text::create("*升级奖励 :", FontBold, 25);
            upgrade->setTextColor(Color4B::WHITE);
            upgrade->setAnchorPoint(Vec2(0.f, 0.f));
            upgrade->setPosition(Vec2(fXpos, pbg->getContentSize().height-170));
            pbg->addChild(upgrade);
            
            auto reward = Text::create(__String::createWithFormat("%lld金币+%lld元宝", HallDataMgr::getInstance()->m_levelData.lUpgradeRewardGold, HallDataMgr::getInstance()->m_levelData.lUpgradeRewardIngot)->getCString(), FontBold, 25);
            reward->setTextColor(Color4B::GREEN);
            reward->setAnchorPoint(Vec2(0.f, 0.f));
            reward->setPosition(Vec2(upgrade->getPositionX()+upgrade->getContentSize().width+4, upgrade->getPositionY()));
            pbg->addChild(reward);
        }
    }
}