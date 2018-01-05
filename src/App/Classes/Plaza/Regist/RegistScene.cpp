//
//  Regist.cpp
//  MyGame
//
//  Created by Tang Miao on 1/12/16.
//
//

#include "RegistScene.h"
#include"cocostudio/CocoStudio.h"
#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../Plaza/LoginScene.h"
#include "../Plaza/FramList.h"
#include "../Loading/LoadingScene.h"

using namespace cocos2d::ui;

Scene* RegistScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RegistScene::create();
    layer->setTag(10);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool RegistScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    auto layout = Layout::create();
    layout->setContentSize(Size(1136, 640));
    
    auto rootNode = CSLoader::createNode("load_res/RegistScene.csb");
    layout->addChild(rootNode);
    layout->setScaleX(JUDGE_SCALE);
    this->addChild(layout);
    
    //文本edit
    auto account = static_cast<Sprite*>(rootNode->getChildByName("regist_account"));
    m_pAccountEdit = EditBox::create(Size(318, 58), "common_res/common_edit_textIcon.png");
    m_pAccountEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_pAccountEdit->setPosition(cocos2d::Point(account->getPositionX()+80, account->getPositionY()));
    m_pAccountEdit->setMaxLength(LEN_ACCOUNT);
    m_pAccountEdit->setFontSize(28);
    m_pAccountEdit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    m_pAccountEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    m_pAccountEdit->setPlaceholderFontColor(cocos2d::Color3B(214,246,255));
    m_pAccountEdit->setPlaceHolder("6-31位英文字母,数字,下划线组合");
    m_pAccountEdit->setDelegate(this);
    m_pAccountEdit->setFontColor(Color3B::YELLOW);
    layout->addChild(m_pAccountEdit);
    
    auto password = static_cast<Sprite*>(rootNode->getChildByName("reigist_password"));
    m_pPasswordEdit = EditBox::create(Size(318, 58), "common_res/common_edit_textIcon.png");
    m_pPasswordEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_pPasswordEdit->setPosition(cocos2d::Point(password->getPositionX()+80, password->getPositionY()));
    m_pPasswordEdit->setMaxLength(LEN_PASSWORD);
    m_pPasswordEdit->setFontSize(28);
    m_pPasswordEdit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    m_pPasswordEdit->setPlaceholderFontColor(cocos2d::Color3B(214,246,255));
    m_pPasswordEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    m_pPasswordEdit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    m_pPasswordEdit->setPlaceHolder("6-31位英文字母,数字,下划线组合");
    m_pPasswordEdit->setDelegate(this);
    m_pPasswordEdit->setFontColor(Color3B::YELLOW);
    layout->addChild(m_pPasswordEdit);
    
    auto confirm = static_cast<Sprite*>(rootNode->getChildByName("regist_confirm_4"));
    m_pPWDConfirmEdit = EditBox::create(Size(318, 58), "common_res/common_edit_textIcon.png");
    m_pPWDConfirmEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_pPWDConfirmEdit->setPosition(cocos2d::Point(confirm->getPositionX()+80, confirm->getPositionY()));
    m_pPWDConfirmEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    m_pPWDConfirmEdit->setMaxLength(LEN_PASSWORD);
    m_pPWDConfirmEdit->setFontSize(28);
    m_pPWDConfirmEdit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    m_pPWDConfirmEdit->setPlaceholderFontColor(cocos2d::Color3B(214,246,255));
    m_pPWDConfirmEdit->setPlaceHolder("6-31位英文字母,数字,下划线组合");
    m_pPWDConfirmEdit->setDelegate(this);
    m_pPWDConfirmEdit->setFontColor(Color3B::YELLOW);
    m_pPWDConfirmEdit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    layout->addChild(m_pPWDConfirmEdit);
    
    
    //注册按钮
    Button *registBtn = static_cast<Button*>(rootNode->getChildByName("Button_regist"));
    registBtn->addTouchEventListener(CC_CALLBACK_2(RegistScene::buttonEventWithRegist, this));
    
    //返回按钮
    Button *backBtn = static_cast<Button*>(rootNode->getChildByName("Button_return"));
    backBtn->addTouchEventListener(CC_CALLBACK_2(RegistScene::buttonEventWithBack, this));
    
    
    return true;
}

bool RegistScene::checkSpaceStr(const std::string str) const
{
    
    if(str.find(" ") != std::string::npos)
    {
        //有空格
        HallDataMgr::getInstance()->AddpopLayer("系统提示", "帐号或密码中不能有空格", Type_Ensure);
        return true;
        
    }
    
    return false;
}
bool RegistScene::checkLength(int len) const
{
    
    if (len == 0)
    {
        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码不能为空，请重新输入", Type_Ensure);
        return false;
    }
    else if (len<6)
    {
        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码长度小于6位，请重新输入", Type_Ensure);
        return  false;
    }
    else if (len > LEN_PASSWORD)
    {
        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码长度超过32位，请重新输入", Type_Ensure);
        return false;
    }
    
    return true;
}


void RegistScene::onEnter()
{
    Layer::onEnter();

    
}

void RegistScene::onEnterTransitionDidFinish()
{
    
    Layer::onEnterTransitionDidFinish();

}

void RegistScene::onExit()
{

    Layer::onExit();
    
}
void RegistScene::loadFinish(Ref *ref)
{

     Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Plazz::createScene()));

    
}
void RegistScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void RegistScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

void RegistScene::buttonEventWithBack(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_LOGON);
        NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_SERVER_LIST);
        
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Socket_Connect_Failure);
        
        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Login::createScene()));
    }
    
}

void RegistScene::buttonEventWithRegist(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_LOGON, CC_CALLBACK_3(RegistScene::loginResult, this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(RegistScene::roomlistResult, this));
    //联网失败通知
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Socket_Connect_Failure, CC_CALLBACK_1(RegistScene::notifyConnectFailure, this)), 1);
    
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        std::string account = std::string(m_pAccountEdit->getText());
        std::string password = std::string(m_pPasswordEdit->getText());
        std::string confirmPwd = std::string(m_pPWDConfirmEdit->getText());
        
        if (checkSpaceStr(account) || checkSpaceStr(password) || checkSpaceStr(confirmPwd))
        {
            return;
        }
     
        //取帐号长度
        int length = 0;
        int len = (int)account.length();
        int i = 0;
        while (i < len)
        {
            if (~(account.at(i) >> 8) == 0)
            {
                i = i + 3;
                length += 2;
            } else
            {
                i = i + 1;
                length += 1;
            }
        }
     
        if (checkLength(length) && checkLength(strLength(password))&&checkLength(strLength(confirmPwd)))
        {
            if (password.compare(confirmPwd) == 0)
            {
                HallDataMgr::getInstance()->m_loadtype = EM_LOAD_TYPE_NORMAL;
                HallDataMgr::getInstance()->m_pAccounts = account;
                HallDataMgr::getInstance()->m_pPassword = MD5Encrypt(password);
                HallDataMgr::getInstance()->m_cbGender  = 1;
                HallDataMgr::getInstance()->m_loadtype = EM_LOAD_TYPE_NORMAL;
                
                NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
                NetworkMgr::getInstance()->sendRegister();
                
                LoadingScene *loading = LoadingScene::create();
                loading->setTag(EM_LOADING);
                loading->setPosition(Vec2(0, 0));
                loading->setFinishCallFunc(CC_CALLBACK_1(RegistScene::loadFinish, this));
                this->addChild(loading);
            }else
            {
                
                HallDataMgr::getInstance()->AddpopLayer("错误提示", "两次输入的密码不一致,请重新输入", Type_Ensure);
            }
            
        }

    }
    
}


//MARK::网络消息模块
void RegistScene::notifyConnectFailure(cocos2d::EventCustom *event)
{
    LoadingScene *loading = static_cast<LoadingScene*>(this->getChildByTag(EM_LOADING));
    if (nullptr != loading)
    {
        loading->removeFromParent();
    }
}


void RegistScene::loginResult(WORD  wSubCmdID, void* pData, WORD wSize)
{
 
    if (wSubCmdID == SUB_MB_LOGON_SUCCESS)
    {
        
        DebugLog("注册成功");
      
        CMD_MB_LOGON_SUCC *success = (CMD_MB_LOGON_SUCC *)pData;
        HallDataMgr::getInstance()->m_wFaceID = success->wFaceID;
        HallDataMgr::getInstance()->m_cbGender = success->cbGender;
        HallDataMgr::getInstance()->m_wCustom = success->cbCustomID;
        HallDataMgr::getInstance()->m_dwUserID = success->dwUserID;
        HallDataMgr::getInstance()->m_dwGameID = success->dwGameID;
        HallDataMgr::getInstance()->m_pNickName = WHConverUnicodeToUtf8WithArray(success->szNickName);
        HallDataMgr::getInstance()->m_dynamicpass = WHConverUnicodeToUtf8WithArray(success->szDynamicPasswd);
        HallDataMgr::getInstance()->m_Ingot = success->lUserIngot;
        HallDataMgr::getInstance()->m_Bean = success->dUserBeans;
        HallDataMgr::getInstance()->m_UserInsure = success->lUserInsure;
        HallDataMgr::getInstance()->m_UserScore = success->lUserScore;
        HallDataMgr::getInstance()->m_cbInsureEnable = success->cbInsureEnable;
        
        if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_NORMAL)
        {
            CFramList::getInstance()->addAccountListWithString(HallDataMgr::getInstance()->m_pAccounts, HallDataMgr::getInstance()->m_pPassword);
            HallDataMgr::getInstance()->saveConfig();
        }
        if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_VISITOR)
        {
            HallDataMgr::getInstance()->m_pAccounts = HallDataMgr::getInstance()->m_pNickName;
        }
        
        HallDataMgr::getInstance()->m_Logintime = getsystemtomillisecond();
        
        
    }
    else if (wSubCmdID == SUB_MB_LOGON_FAILURE)
    {
        DebugLog("注册失败");
        
        LoadingScene *loading = static_cast<LoadingScene*>(this->getChildByTag(EM_LOADING));
        loading->removeFromParent();
        
     
        CMD_MB_LOGON_FAILURE* failuer = (CMD_MB_LOGON_FAILURE *)pData;
        auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
        
        //昵称错误
        if (failuer->lErrorCode == 30)
        {
          
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
        }
        else
        {
            std::string str = WHConverUnicodeToUtf8WithArray(failuer->szDescription);
            HallDataMgr::getInstance()->AddpopLayer("", str, Type_Ensure);
        }
      
    }
    else if (wSubCmdID == SUB_MB_UPDATE_NOTIFY)
    {
        LoadingScene *loading = static_cast<LoadingScene*>(this->getChildByTag(EM_LOADING));
        loading->removeFromParent();
        
        HallDataMgr::getInstance()->AddpopLayer("系统提示", "当前版本不是最新版本，请升级到最新版本", Type_Ensure);
        auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
    }
}

void RegistScene::roomlistResult(WORD  wSubCmdID, void* pData, WORD wSize)
{
    
    switch (wSubCmdID)
    {
        case SUB_MB_LIST_KIND:				//种类列表	100
        {
            
        }
            break;
        case SUB_MB_LIST_SERVER:			//房间列表	101
        {
            int size = sizeof(_stGameRoomServer);
            int count = wSize/size;
            HallDataMgr::getInstance()->roomlistclear();
            for (int index=0 ; index<count ; index++)
            {
                _stGameRoomServer *pServer = new _stGameRoomServer();
                memset(pServer, 0, sizeof(_stGameRoomServer));
                char *pVector = (char *)pData + index*size;
                memcpy(pServer, pVector, size);
                HallDataMgr::getInstance()->m_roomList.push_back(pServer);
                std::sort(HallDataMgr::getInstance()->m_roomList.begin(), HallDataMgr::getInstance()->m_roomList.end(), [](const _stGameRoomServer *s1,const _stGameRoomServer *s2){
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
       
            LoadingScene *loading = static_cast<LoadingScene *>(this->getChildByTag(EM_LOADING));
            loading->runFinish();
            
            DebugLog("列表完成");
            
        }
            break;
        default:
            break;
    }

    
    
    
}
