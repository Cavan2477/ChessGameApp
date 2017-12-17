/************************************************************************************
 * file: 		Personal.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:05:58
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include"cocostudio/CocoStudio.h"
#include "PersonalScene.h"
#include "../../Common/Project.h"
#include "../../DataMgr/HallDataMgr.h"
#include "../../Public/HeaderRequest.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../Plaza/FramList.h"
#include "../Shop/ShopScene.h"
#include "../Bank/BankShowScene.h"

using namespace ui;

PersonalScene::PersonalScene():
m_eType(Type_PersonalInfo),
m_eModify(modify_sex),
_personalLayout(nullptr),
_modifyLoginLayout(nullptr),
_modifyBankLayout(nullptr),
_headSprite(nullptr),
m_photo(nullptr)

{
    
    

}

PersonalScene::~PersonalScene()
{
    
    CC_SAFE_RELEASE(_headSprite);
    CC_SAFE_RELEASE(m_photo);
    
}

bool PersonalScene::init()
{
    
    if (!Layer::init())
        return false;
    
    auto pbg = ImageView::create();
    pbg->setTouchEnabled(true);
    pbg->setScale9Enabled(true);
    pbg->setContentSize(WIN_SIZE);
    pbg->setPosition(WIN_SIZE/2);
    this->addChild(pbg);
 
    auto layout = Layout::create();
    layout->setTag(INFO_MAINLAYOUT);
    layout->setContentSize(Size(1136, 640));
    
    auto rootNode = CSLoader::createNode("personal_res/PersonalMain.csb");
    rootNode->setTag(INFO_MAINROOT);
    layout->addChild(rootNode);
    
    //基本信息
    _personalLayout = Layout::create();
    _personalLayout->setContentSize(Size(1136, 640));
    layout->addChild(_personalLayout);
    
    auto personalRoot = CSLoader::createNode("personal_res/Personal.csb");
    personalRoot->setTag(1);
    _personalLayout->addChild(personalRoot);
    
    //修改登录密码
    _modifyLoginLayout = Layout::create();
    _modifyLoginLayout->setContentSize(Size(1136, 640));
    layout->addChild(_modifyLoginLayout);
    _modifyLoginLayout->setVisible(false);
    
    auto LoginModifyRoot = CSLoader::createNode("personal_res/Modify.csb");
    LoginModifyRoot->setTag(2);
    _modifyLoginLayout->addChild(LoginModifyRoot);
    
    auto loginPassModifyBtn = static_cast<Button *>(LoginModifyRoot->getChildByName("Button_sureModify"));

    if (loginPassModifyBtn != nullptr)
    {
        if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_VISITOR || HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_SINA)
        {
            loginPassModifyBtn->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type)
			{    
                if (type == Widget::TouchEventType::ENDED)
                {       
                    HallDataMgr::getInstance()->AddpopLayer("系统提示", "游客或第三方帐号不支持修改登录密码功能", EM_MODE_TYPE_ENSURE);
                }
            });
        }
		else
        {
            loginPassModifyBtn->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithSureModify, this));
        }
    }
    
    //修改银行密码
    _modifyBankLayout = Layout::create();
    _modifyBankLayout->setContentSize(Size(1136, 640));
    layout->addChild(_modifyBankLayout);
    _modifyBankLayout->setVisible(false);
    
    

    auto bankModifyRoot = CSLoader::createNode("personal_res/Modify.csb");
    bankModifyRoot->setTag(3);
    _modifyBankLayout->addChild(bankModifyRoot);
    auto bankPassModifyBtn = static_cast<Button *>(bankModifyRoot->getChildByName("Button_sureModify"));
    if  (bankPassModifyBtn != nullptr)
    {
        bankPassModifyBtn->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithSureModify, this));
    }
    
    

    layout->setScaleX(JUDGE_SCALE);
    this->addChild(layout);
    
    //返回按钮
    auto returnBtn = static_cast<Button *>(rootNode->getChildByName("Button_return"));
    returnBtn->addTouchEventListener([this](cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type){
        
    
        CallFunc *func = CallFunc::create([this]{this->removeFromParent();});
        Sequence *quence = Sequence::createWithTwoActions(MoveTo::create(0.4f, Vec2(1136, 0)), func);
        
        this->runAction(quence);
        
    });
    
    //个人信息
    auto personal = static_cast<Button *>(rootNode->getChildByName("Button_personal"));
    personal->setTag(INFO_PERSONAL);
    if (nullptr != personal)
    {
        
        personal->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithMenu, this));
    }
    
    //帐号
    auto account = static_cast<Text *>(personalRoot->getChildByName("Text_account"));
    if (account != nullptr)
    {
        account->setString(HallDataMgr::getInstance()->m_pAccounts);
        account->setTextColor(cocos2d::Color4B(36,236,250,255));
        account->setTextAreaSize(Size(200, account->getContentSize().height ));
    }
    
    //昵称
    auto nick = static_cast<Text *>(personalRoot->getChildByName("Text_nick"));
    if (nick != nullptr)
    {
        nick->setString(HallDataMgr::getInstance()->m_pNickName);
        nick->setTextColor(cocos2d::Color4B(36,236,250,255));
        nick->setTextAreaSize(Size(200, account->getContentSize().height));
        nick->setTag(INFO_NICK);
    }
    
    //修改昵称
    EditBox *edit = EditBox::create(Size(318, 58), "personal_res/info_commonText.png");
    edit->setDelegate(this);
    edit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    edit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    edit->setPosition(cocos2d::Point(nick->getPositionX(), nick->getPositionY()));
    edit->setMaxLength(LEN_ACCOUNT);
    edit->setFontSize(28);
    edit->setReturnType(EditBox::KeyboardReturnType::DONE);
    edit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
    edit->setPlaceHolder("");
    edit->setTag(INFO_NICKEDIT);
    edit->setFontColor(Color3B::YELLOW);
    edit->setVisible(false);
    personalRoot->addChild(edit);
    
    //修改按钮
    Button *modifyBtn = Button::create("personal_res/userinfo_modiyNick_0.png","personal_res/userinfo_modiyNick_1.png");
    modifyBtn->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithModifyNick, this));
    modifyBtn->setAnchorPoint(Vec2(.0, .5));
    modifyBtn->setTag(INFO_MODIFYBTN);
    modifyBtn->setPosition(Vec2(nick->getPositionX() + 230, nick->getPositionY()));
    personalRoot->addChild(modifyBtn);
    modifyBtn->setVisible(false);
    modifyBtn->setEnabled(false);
    
    //id
    auto ID = static_cast<Text *>(personalRoot->getChildByName("Text_id"));
    if (ID != nullptr)
    {
        ID->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_dwGameID)->getCString());
        ID->setTextColor(cocos2d::Color4B(36,236,250,255));
    }
    
    //元宝
    auto ingot = static_cast<Text *>(personalRoot->getChildByName("Text_ingot"));
    if (ingot != nullptr)
    {
        ingot->setString(__String::createWithFormat("%lld",HallDataMgr::getInstance()->m_Ingot)->getCString());
        ingot->setTextColor(cocos2d::Color4B(228,235,55,255));
        
    }
    
    //游戏币
    auto score = static_cast<Text *>(personalRoot->getChildByName("Text_coin"));
    if (score != nullptr)
    {
        score->setString(__String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString());
        score->setTextColor(cocos2d::Color4B(228,235,55,255));
    }
    
    //游戏豆
    auto bean = static_cast<Text *>(personalRoot->getChildByName("Text_bean"));
    if (bean != nullptr)
    {
        bean->setString(__String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_Bean)->getCString());
        bean->setTextColor(cocos2d::Color4B(228,235,55,255));
    }
    
    //兑换
    auto exchange = static_cast<Button *>(personalRoot->getChildByName("Button_exchange"));
    if (exchange != nullptr)
    {
        
        exchange->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
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
        });
    }
    
    //取款
    auto bank = static_cast<Button *>(personalRoot->getChildByName("Button_takeCoin"));
    if (nullptr != bank)
    {
       bank->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                if (this->getChildByTag(BANK))
                    return ;
                
                BankShowScene *bank = BankShowScene::create();
                bank->setTag(BANK);
                bank->setPosition(Vec2(1136, 0));
                this->addChild(bank);
                bank->popBank();
            }
        });
        
        
    }
    
    //充值
    auto purchase = static_cast<Button *>(personalRoot->getChildByName("Button_purchase"));
    if (nullptr != purchase)
    {
        purchase->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                if (this->getChildByTag(SHOP))
                    return;
                
                
                ShopScene *shop = ShopScene::create();
                shop->setTag(SHOP);
                shop->setPosition(Vec2(1136, 0));
                this->addChild(shop);
                shop->setIndex(1);
                shop->popShop();
            }
        });
        
    }
    
    //性别
    BYTE  cbGender = HallDataMgr::getInstance()->m_cbGender;
    
    auto manBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_1"));
  
    auto womanBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_0"));
    
    if (nullptr != manBtn && nullptr != womanBtn)
    {
        manBtn->setTag(INFO_MAN);
        womanBtn->setTag(INFO_WOMAN);
        
        if (cbGender == 1)
        {
            manBtn->loadTextures("personal_res/info_sex_1.png", "personal_res/info_sex_0.png");
            womanBtn->loadTextures("personal_res/info_sex_0.png", "personal_res/info_sex_1.png");
        }else
        {
            manBtn->loadTextures("personal_res/info_sex_0.png", "personal_res/info_sex_1.png");
            womanBtn->loadTextures("personal_res/info_sex_1.png", "personal_res/info_sex_0.png");
            
        }
        
        manBtn->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithGender, this));
        womanBtn->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithGender, this));
        
    }

    //修改登录密码
    auto modifyLogin = static_cast<Button *>(rootNode->getChildByName("Button_modifyLoginPass"));
    modifyLogin->setTag(INFO_LOGINPASS);
    if (nullptr != modifyLogin)
    {
        modifyLogin->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithMenu, this));
    }
    
    
    //修改银行密码
    auto modifyBank = static_cast<Button *>(rootNode->getChildByName("Button_modifyBankPass"));
    modifyBank->setTag(INFO_BANKPASS);
    if (nullptr != modifyBank)
    {
        modifyBank->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithMenu, this));
    }
    
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
        pHead->setHeadSize(190.0);
        setHeadSprite(pHead);
        _headSprite->setPosition(Vec2(269, 358));
        _personalLayout->addChild(_headSprite);
        
        //上传头像
        auto  modify = Button::create("personal_res/info_pen.png");
        modify->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithUploadRes, this));
        modify->setPosition(Vec2(340,285));
        _personalLayout->addChild(modify);
    }

    //等级信息
    auto levelIcon = static_cast<Sprite *>(personalRoot->getChildByName("level_icon"));
    if (nullptr != levelIcon)
    {
        auto level = Label::createWithCharMap("plazz/level_num.png", 10, 13, '0');
        level->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
        level->setPosition(levelIcon->getContentSize()/2);
        levelIcon->addChild(level);
    }
    
    //进度条
    auto barFull = static_cast<Sprite *>(personalRoot->getChildByName("level_bar_full"));
    if (nullptr != barFull)
    {
        float percent =  (HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience > 0) ?
        HallDataMgr::getInstance()->m_levelData.dwExperience/(float) HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience : 0;
        percent = MIN(percent, 1.0);
        
        barFull->setScaleX(percent);
        
        //百分比
        auto percentText = static_cast<Text *>(personalRoot->getChildByName("Text_percent"));
        if (nullptr != percentText)
        {
            
            percentText->setTextColor(cocos2d::Color4B::BLACK);
            int _percent = ((int)(percent*100));
            percentText->setString(__String::createWithFormat("%d％",_percent)->getCString());
        }
    }
    
    //升级与奖励
    auto upgrade = static_cast<Text *>(personalRoot->getChildByName("Text_0"));
    auto reward = static_cast<Text *>(personalRoot->getChildByName("Text_2"));
    if (nullptr != upgrade && nullptr != reward)
    {
        if (HallDataMgr::getInstance()->m_levelData.wCurrLevelID == 60)
        {
            
            upgrade->setString("您已经是满级");
            upgrade->setTextColor(cocos2d::Color4B::YELLOW);
            reward->setVisible(false);
            
        }else
        {
            upgrade->setString(__String::createWithFormat("下次升级还需要%d经验",HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience -
                                                          HallDataMgr::getInstance()->m_levelData.dwExperience)->getCString());
            upgrade->setTextColor(cocos2d::Color4B::YELLOW);
            
            reward->setString(__String::createWithFormat("奖励%lld游戏币+%lld元宝",HallDataMgr::getInstance()->m_levelData.lUpgradeRewardGold,
                                                         HallDataMgr::getInstance()->m_levelData.lUpgradeRewardIngot)->getCString());
            reward->setTextColor(cocos2d::Color4B::YELLOW);
            
        }
    }
    return true;
}


void PersonalScene::onEnter()
{
    Layer::onEnter();
    
}

void PersonalScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_FACE_INFO, CC_CALLBACK_2(PersonalScene::userFaceinfoResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_SUCCESS, CC_CALLBACK_2(PersonalScene::operatesuccessResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_FAILURE, CC_CALLBACK_2(PersonalScene::operatefailureResult, this));
    
}

void PersonalScene::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_USER_FACE_INFO);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
    
    Layer::onExit();
    
}

void PersonalScene::popPersonal()
{
    
    auto run = MoveTo::create(0.2f, Vec2(.0,.0));
    this->runAction(run);
    
}
void PersonalScene::initModify()
{
    
    auto modify = (m_eType == Type_ModifyLoginPass)  ? static_cast<Node *>(_modifyLoginLayout->getChildByTag(2)) :
                                                       static_cast<Node *>(_modifyBankLayout->getChildByTag(3))  ;
    
    if (modify->getChildByTag(100))
    {
        return;
    }
    for (int i = 0 ; i < 3 ; i++)
    {
        __String str;
        
        if ( 0 == i)
        {
            
            if (m_eType == Type_ModifyLoginPass)
            {
               str = "请输入您的登录密码";
            }else
            {
                str = "请输入您的银行密码";
            }
        }else if (1 == i)
        {
            
           str = "请输入您的新密码";
        }else
        {
            
            str = "请重复您刚才输入的新密码";
        }
        
        assert(modify);
        Text *text = static_cast<Text *>(modify->getChildByName(__String::createWithFormat("Text_%d",i)->getCString()));
        
        EditBox *edit = EditBox::create(Size(318, 58), "personal_res/info_commonText.png");
        edit->setDelegate(this);
        edit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
        edit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
        edit->setPosition(cocos2d::Point(text->getPositionX() + 60, text->getPositionY()));
        edit->setMaxLength(LEN_PASSWORD);
        edit->setFontSize(28);
        edit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
        edit->setReturnType(EditBox::KeyboardReturnType::DONE);
        edit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
        edit->setPlaceholderFontColor(cocos2d::Color3B(36,236,250));
        edit->setPlaceHolder(str.getCString());
        edit->setTag(100 + i);
        edit->setFontColor(Color3B::YELLOW);
        modify->addChild(edit);
    }
}

//MARK::Notify
void PersonalScene::userFaceinfoResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserFaceInfo *)pData;
    HallDataMgr::getInstance()->m_wFaceID = result->wFaceID;
    HallDataMgr::getInstance()->m_wCustom = result->dwCustomID;
    
    HallDataMgr::getInstance()->m_Headlist.insert(HallDataMgr::getInstance()->m_dwUserID, _headSprite->getTexture());
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    
    HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
    
    //更新
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(EM_USER_DATA_CHANGE_HEAD);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void PersonalScene::operatesuccessResult(void *pData, WORD wSize)
{
    auto presult = (CMD_GP_OperateSuccess *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, EM_MODE_TYPE_ENSURE);
    HallDataMgr::getInstance()->m_cbGender = m_cbGender;
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    if (m_eType == Type_PersonalInfo)
    {
        
        auto personalRoot = static_cast<Node *>(_personalLayout->getChildByTag(1));
        auto manBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_1"));
        
        auto womanBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_0"));
        
        if (m_eModify == modify_sex)
        {
            if (m_cbGender == MAN)
            {
                manBtn->loadTextures("personal_res/info_sex_1.png", "personal_res/info_sex_0.png");
                womanBtn->loadTextures("personal_res/info_sex_0.png", "personal_res/info_sex_1.png");
            }else
            {
                manBtn->loadTextures("personal_res/info_sex_0.png", "personal_res/info_sex_1.png");
                womanBtn->loadTextures("personal_res/info_sex_1.png", "personal_res/info_sex_0.png");
                
            }
        }else if (m_eModify == modify_nick)
        {
            auto editBox = static_cast<EditBox *>(personalRoot->getChildByTag(INFO_NICKEDIT));
            HallDataMgr::getInstance()->m_pNickName = editBox->getText();
            
            auto nick = static_cast<Text *>(personalRoot->getChildByTag(INFO_NICK));
            nick->setString(editBox->getText());
            
            //更新大厅显示
            EventCustom event(whEvent_User_Data_Change);
            auto value = __Integer::create(EM_USER_DATA_CHANGE_NAME);
            event.setUserData(value);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
        
    }
    else if (m_eType == Type_ModifyLoginPass)
    {
        auto modify = (m_eType == Type_ModifyLoginPass)  ? static_cast<Node *>(_modifyLoginLayout->getChildByTag(2)) :
        static_cast<Node *>(_modifyBankLayout->getChildByTag(3))  ;
        
 
        EditBox *orignalPass = static_cast<EditBox *>(modify->getChildByTag(100));
        EditBox *newPass = static_cast<EditBox *>(modify->getChildByTag(101));
        EditBox *confirmPass = static_cast<EditBox *>(modify->getChildByTag(102));
      
        
        HallDataMgr::getInstance()->m_pPassword = MD5Encrypt(newPass->getText());
        CFramList::getInstance()->addAccountListWithString(HallDataMgr::getInstance()->m_pAccounts, HallDataMgr::getInstance()->m_pPassword);
        
        orignalPass->setText("");
        newPass->setText("");
        confirmPass->setText("");
        
    }
    else if (m_eType == Type_ModifyBankPass)
    {
        
        
        auto modify = (m_eType == Type_ModifyLoginPass)  ? static_cast<Node *>(_modifyLoginLayout->getChildByTag(2)) :
        static_cast<Node *>(_modifyBankLayout->getChildByTag(3))  ;
        
        
        EditBox *orignalPass = static_cast<EditBox *>(modify->getChildByTag(100));
        EditBox *newPass = static_cast<EditBox *>(modify->getChildByTag(101));
        EditBox *confirmPass = static_cast<EditBox *>(modify->getChildByTag(102));
        
        HallDataMgr::getInstance()->m_pBankword = MD5Encrypt(newPass->getText());
        
        orignalPass->setText("");
        newPass->setText("");
        confirmPass->setText("");
    }
    
}

void PersonalScene::operatefailureResult(void *pData, WORD wSize)
{
    auto presult = (CMD_GP_OperateFailure *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("错误提示", str, EM_MODE_TYPE_ENSURE);
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
}


//MARK::按钮事件
void PersonalScene::buttonEventWithMenu(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        auto btn = static_cast<Button *>(target);
        auto mainLayout = static_cast<Layout *>(this->getChildByTag(INFO_MAINLAYOUT));
        auto mainRoot = static_cast<Node *>(mainLayout->getChildByTag(INFO_MAINROOT));
        auto personal = static_cast<Button *>(mainRoot->getChildByTag(INFO_PERSONAL));
        auto loginPass = static_cast<Button *>(mainRoot->getChildByTag(INFO_LOGINPASS));
        auto bankPass = static_cast<Button *>(mainRoot->getChildByTag(INFO_BANKPASS));
        
        int tag = btn->getTag();
        if(tag == INFO_PERSONAL )
        {
            if (m_eType == Type_PersonalInfo)
            {
                
                return;
            }
            
            m_eType = Type_PersonalInfo;
            _personalLayout->setVisible(true);
            _modifyLoginLayout->setVisible(false);
            _modifyBankLayout->setVisible(false);
            
            if (personal && loginPass && bankPass)
            {
                
                personal->loadTextures("personal_res/info_personalIcon.png", "personal_res/info_personalBtn.png");
                loginPass->loadTextures("personal_res/info_loginPassModifyBtn.png", "personal_res/info_pwdIcon.png");
                bankPass->loadTextures("personal_res/info_bankPassModifyBtn.png", "personal_res/info_bankpwdIcon.png");
            }
            
        }
        if(tag == INFO_LOGINPASS )
        {
            if (m_eType == Type_ModifyLoginPass)
            {
                return;
            }
            
            m_eType = Type_ModifyLoginPass;
            _personalLayout->setVisible(false);
            _modifyLoginLayout->setVisible(true);
            _modifyBankLayout->setVisible(false);
            
            if (personal && loginPass && bankPass)
            {
                
                personal->loadTextures("personal_res/info_personalBtn.png", "personal_res/info_personalIcon.png");
                loginPass->loadTextures("personal_res/info_pwdIcon.png", "personal_res/info_loginPassModifyBtn.png");
                bankPass->loadTextures("personal_res/info_bankPassModifyBtn.png", "personal_res/info_bankpwdIcon.png");
            }
            
            this->initModify();
        }
        if(tag == INFO_BANKPASS)
        {
            if (m_eType == Type_ModifyBankPass)
            {
                return;
            }
            
            m_eType = Type_ModifyBankPass;
            _personalLayout->setVisible(false);
            _modifyLoginLayout->setVisible(false);
            _modifyBankLayout->setVisible(true);
            
            if (personal && loginPass && bankPass)
            {
                
                personal->loadTextures("personal_res/info_personalBtn.png", "personal_res/info_personalIcon.png");
                loginPass->loadTextures("personal_res/info_loginPassModifyBtn.png", "personal_res/info_pwdIcon.png");
                bankPass->loadTextures("personal_res/info_bankpwdIcon.png", "personal_res/info_bankPassModifyBtn.png");
            }
            
            this->initModify();
        }
        
        
    }
    
}

void PersonalScene::buttonEventWithGender(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        Button *btn = static_cast<Button *>(target);
        
        auto personalRoot = static_cast<Node *>(_personalLayout->getChildByTag(1));
        auto manBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_1"));
        
        auto womanBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_0"));
        
        if (btn == manBtn)
        {
            if (HallDataMgr::getInstance()->m_cbGender == MAN)
            {
                return;
            }
            
            HallDataMgr::getInstance()->AddpopLayer("", "发送资料中...", EM_MODE_TYPE_WAIT_TEXT);
            m_cbGender = 1;
            
            this->sendAlterIndividual(HallDataMgr::getInstance()->m_pNickName, m_cbGender);
        }
        
        if (btn == womanBtn)
        {
            if (HallDataMgr::getInstance()->m_cbGender == WOMAN)
            {
                return;
            }
            HallDataMgr::getInstance()->AddpopLayer("", "发送资料中...", EM_MODE_TYPE_WAIT_TEXT);
            m_cbGender = 0;
            
            this->sendAlterIndividual(HallDataMgr::getInstance()->m_pNickName, m_cbGender);
        }
        

         m_eModify = modify_sex;
    }
}

//更换头像
void PersonalScene::buttonEventWithUploadRes(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (_personalLayout->getChildByTag(INFO_PICBG))
            return;
        
        auto image = ImageView::create();
        image->setContentSize(Size(1136, 640));
        image->setScale9Enabled(true);
        image->setTouchEnabled(true);
        image->setPosition(Vec2(568, 320));
        image->setTag(INFO_PICBG);
        _personalLayout->addChild(image);
        
        auto loadBG = ImageView::create("personal_res/info_loadPic_bg.png");
        loadBG->setPosition(Vec2(568, 380));
        image->addChild(loadBG);
        
        
        auto listView = ListView::create();
        listView->setContentSize(Size(960, 160));
        listView->setBounceEnabled(true);
        listView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        listView->setAnchorPoint(Vec2(0.0, 0.0));
        listView->setPosition(Vec2(10, 0));
        listView->setItemsMargin(60.0f);
        loadBG->addChild(listView);
        
        for (int i = 0 ; i < 9 ; i++)
        {
            
            //头像框
            auto headRect = ImageView::create("personal_res/info_system_headBG.png");
            
            
            __String *str = (HallDataMgr::getInstance()->m_cbGender == MAN) ? __String::createWithFormat("public_res/im_head_man_%d.png",i) :
                                                                              __String::createWithFormat("public_res/im_head_woman_%d.png",i);
            auto head = Button::create(str->getCString(),str->getCString());
            head->setTag(i);
            head->addTouchEventListener([=](Ref *ref ,cocos2d::ui::Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED)
                {
                    HallDataMgr::getInstance()->AddpopLayer("", "正在上传图片", EM_MODE_TYPE_WAIT_TEXT);
    
                    this->sendSystemFaceInfo(head->getTag());
                    std::string file =  head->getNormalFile().file;
                    auto _sprite = Sprite::create(file);
                    _headSprite->setTexture(_sprite->getTexture());
//
                    image->removeFromParent();
                }
            });
            head->setScale(.5f);
            head->setAnchorPoint(Vec2(.5, .5));
            head->setPosition(headRect->getContentSize()/2);
            
            headRect->addChild(head);
            listView->pushBackCustomItem(headRect);
            
        }
        
        listView->setVisible(false);
        
        
        //自定义头像
        auto custom = Button::create("personal_res/info_customHead.png");
        custom->setAnchorPoint(Vec2(1.0, .5));
        custom->setPosition(Vec2(927, loadBG->getContentSize().height/2));
        custom->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                image->removeFromParent();
                if (!m_photo)
                {
                    m_photo = new PhotoBridge;
                }
                m_photo->openPhoto();
                m_photo->setChoiceType(0);
                m_photo->m_completecallback = CC_CALLBACK_1(PersonalScene::photocomplete, this);
                HallDataMgr::getInstance()->m_completecallback = CC_CALLBACK_1(PersonalScene::photocomplete, this);
                
            }
        });
        loadBG->addChild(custom);
        

        
        //系统头像
        auto system = Button::create("personal_res/info_systemHead.png");
        system->setAnchorPoint(Vec2(.0, .5));
        system->setPosition(Vec2(120, loadBG->getContentSize().height/2));
        system->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                custom->setVisible(false);
                system->setVisible(false);
                
                listView->setVisible(true);
            }
        });
        loadBG->addChild(system);
    
        //关闭按钮
        auto close = Button::create("personal_res/info_close.png");
        close->setPosition(Vec2(1015, 140));
        close->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED)
            {
               if (listView->isVisible())
               {
                   
                   custom->setVisible(true);
                   system->setVisible(true);
                   
                   listView->setVisible(false);
                   return ;
               }
                image->removeFromParent();
                
            }
        });
        
        loadBG->addChild(close);
    
        
    }
}
void PersonalScene::buttonEventWithModifyNick(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        Button *modify = static_cast<Button *>(target);
        modify->setVisible(false);
        
        auto personalRoot = static_cast<Node *>(_personalLayout->getChildByTag(1));
        Text    *nick = static_cast<Text *>(personalRoot->getChildByTag(INFO_NICK));
        nick->setVisible(false);
        
        
        auto editbox = static_cast<EditBox *>(personalRoot->getChildByTag(INFO_NICKEDIT));
        editbox->setVisible(true);
        editbox->touchDownAction(editbox, cocos2d::ui::Widget::TouchEventType::ENDED);
    }
    
}

void PersonalScene::buttonEventWithSureModify(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        if (m_eType == Type_ModifyBankPass)
        {
            
            if (!HallDataMgr::getInstance()->m_cbInsureEnable)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "您暂未开通银行密码,请先开通银行密码", EM_MODE_TYPE_ENSURE);
                return;
            }
        }
        
        auto modify = (m_eType == Type_ModifyLoginPass)  ? static_cast<Node *>(_modifyLoginLayout->getChildByTag(2)) :
                                                           static_cast<Node *>(_modifyBankLayout->getChildByTag(3))  ;
        
        EditBox *orignalPass = static_cast<EditBox *>(modify->getChildByTag(100));
        EditBox *newPass = static_cast<EditBox *>(modify->getChildByTag(101));
        EditBox *confirmPass = static_cast<EditBox *>(modify->getChildByTag(102));
        
        
        int len_orignal = strLength(orignalPass->getText());
        int len_new = strLength(newPass->getText());
        if (0 == len_orignal)
        {
            
            HallDataMgr::getInstance()->AddpopLayer("提示", "原始密码不能变为空", EM_MODE_TYPE_ENSURE);
            return;
            
        }else if (len_orignal > 32 || len_orignal < 6)
        {
            HallDataMgr::getInstance()->AddpopLayer("提示", "原始密码请输入6-32位字符", EM_MODE_TYPE_ENSURE);
             return;
        }
        
        if (m_eType == Type_ModifyLoginPass)
        {
        
            if (std::strcmp(MD5Encrypt(std::string(orignalPass->getText())).c_str() , HallDataMgr::getInstance()->m_pPassword.c_str()) != 0)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "您输入的原始密码有误", EM_MODE_TYPE_ENSURE);
               return;
                
            }
        }else
        {
            if (std::strcmp(MD5Encrypt(std::string(orignalPass->getText())).c_str() , HallDataMgr::getInstance()->m_pBankword.c_str()) != 0)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "您输入的原始密码有误", EM_MODE_TYPE_ENSURE);
                return;
                
            }
            
        }
        
        if (0 == len_new)
        {
            
            HallDataMgr::getInstance()->AddpopLayer("提示", "新密码不能变为空", EM_MODE_TYPE_ENSURE);
            return;
            
        }else if (len_new > 32 || len_new < 6)
        {
            HallDataMgr::getInstance()->AddpopLayer("提示", "新密码请输入6-32位字符", EM_MODE_TYPE_ENSURE);
            return;
        }
        
        std::string newPassStr = std::string(newPass->getText());
        
        if(newPassStr.find(" ") != std::string::npos)
        {
            //有空格
            HallDataMgr::getInstance()->AddpopLayer("提示", "新密码不能输入空格字符,请重新输入", EM_MODE_TYPE_ENSURE);
            return;
        }
        
        if (std::strcmp(orignalPass->getText() , newPassStr.c_str()) == 0)
        {
            
            HallDataMgr::getInstance()->AddpopLayer("提示", "新密码与原始密码一致,请重新输入", EM_MODE_TYPE_ENSURE);
            return;
        }
        
         std::string confirmPassStr = std::string(confirmPass->getText());
        
        if (std::strcmp(newPassStr.c_str(), confirmPassStr.c_str()) != 0)
        {
            
            HallDataMgr::getInstance()->AddpopLayer("提示", "两次输入的密码不一致,请重新输入", EM_MODE_TYPE_ENSURE);
            return;
        }
        
        if  (m_eType == Type_ModifyLoginPass)
        {
            sendAlterloginPass(std::string(orignalPass->getText()), newPassStr);
            
        }else if (m_eType == Type_ModifyBankPass)
        {
            
            
            
            sendAlterBankPass(std::string(orignalPass->getText()), newPassStr);
        }
        
    }
    
}
//MARK::CALLBACK
void PersonalScene::photocomplete(cocos2d::Image *pimage)
{
    auto ptexture = new Texture2D;
    ptexture->initWithImage(pimage);
    _headSprite->setTexture(ptexture);
    this->sendCustomFaceInfo(pimage);
    ptexture->release();
    HallDataMgr::getInstance()->AddpopLayer("", "正在提交......", EM_MODE_TYPE_WAIT_TEXT);
}

//MARK::NetWork
void PersonalScene::sendSystemFaceInfo(WORD wface)
{
    CMD_GP_SystemFaceInfo SystemFaceInfo;
    memset(&SystemFaceInfo, 0, sizeof(CMD_GP_SystemFaceInfo));
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, SystemFaceInfo.szPassword);
    SystemFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    SystemFaceInfo.wFaceID = wface;
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_SYSTEM_FACE_INFO, &SystemFaceInfo, sizeof(CMD_GP_SystemFaceInfo),NetworkMgr::getInstance()->getSocketOnce());
    
    
}
void PersonalScene::sendCustomFaceInfo(cocos2d::Image *pImage)
{
    auto pdate = pImage->getData();
    int length = (int)pImage->getDataLen();
    
    char byte[length];
    memset(byte, 0, length);

    for(int i=0; i<length/4;i++)
    {
        byte[i*4]=pdate[i*4+2];
        byte[i*4+1]=pdate[i*4+1];
        byte[i*4+2]=pdate[i*4];
        byte[i*4+3]=255;
    }
    
    CMD_GP_CustomFaceInfo CustomFaceInfo;
    memset(&CustomFaceInfo, 0, sizeof(CustomFaceInfo));
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, CustomFaceInfo.szPassword);
    CustomFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(CustomFaceInfo.dwCustomFace, byte, length);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CUSTOM_FACE_INFO, &CustomFaceInfo, sizeof(CMD_GP_CustomFaceInfo),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::sendAlterIndividual(const std::string &name, BYTE cbgerder, int type)
{
    BYTE buffer[256];
    memset(buffer, 0, sizeof(buffer));
    
    CMD_GP_ModifyIndividual modifyindividual;
    memset(&modifyindividual, 0, sizeof(CMD_GP_ModifyIndividual));
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, modifyindividual.szPassword);
    modifyindividual.cbGender = cbgerder;
    modifyindividual.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(buffer, &modifyindividual, sizeof(CMD_GP_ModifyIndividual));
    int size = sizeof(CMD_GP_ModifyIndividual);
    //if (type)
    {
        int wsize = (int)name.size()+1;
        
        _stUserDataExt describe;
        memset(&describe, 0, sizeof(_stUserDataExt));
        describe.wDataSize = wsize*sizeof(TCHAR);
        describe.wDataDesc = DTP_GP_UI_NICKNAME;
        memcpy(buffer+size, &describe, sizeof(_stUserDataExt));
        size += sizeof(_stUserDataExt);
        
        TCHAR tname[wsize];
        memset(tname, 0, sizeof(tname));
        UTF8Str_To_UTF16Str(name,tname);
        memcpy(buffer + size, tname, describe.wDataSize);
        size += describe.wDataSize;
    }
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INDIVIDUAL, &buffer, size,NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::sendAlterloginPass(const std::string &oldpass, const std::string &newpass)
{
    CMD_GP_ModifyLogonPass request;
    memset(&request, 0, sizeof(request));
    request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(MD5Encrypt(oldpass), request.szScrPassword);
    UTF8Str_To_UTF16Str(MD5Encrypt(newpass), request.szDesPassword);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_LOGON_PASS, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::sendAlterBankPass(const std::string &oldpass, const std::string &newpass)
{
    CMD_GP_ModifyInsurePass request;
    memset(&request, 0, sizeof(request));
    request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(MD5Encrypt(oldpass), request.szScrPassword);
    UTF8Str_To_UTF16Str(MD5Encrypt(newpass), request.szDesPassword);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INSURE_PASS, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
}

//MARK::EditBoxDelegate
void PersonalScene::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
   
    
    
}

void PersonalScene::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}

void PersonalScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void PersonalScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    if (m_eType == Type_PersonalInfo)
    {
        m_cbGender = HallDataMgr::getInstance()->m_cbGender;
        
        auto personalRoot = static_cast<Node *>(_personalLayout->getChildByTag(1));

        Button *modify = static_cast<Button *>(personalRoot->getChildByTag(INFO_MODIFYBTN));
        modify->setVisible(true);
        
        Text    *_nick = static_cast<Text *>(personalRoot->getChildByTag(INFO_NICK));
        _nick->setVisible(true);
        
        editBox->setVisible(false);

        std::string nick = std::string(editBox->getText());
       
        //取帐号长度
        int length = 0;
        int len = (int)nick.length();
        int i = 0;
        while (i < len)
        {
            if (~(nick.at(i) >> 8) == 0)
            {
                i = i + 3;
                length += 2;
            } else
            {
                i = i + 1;
                length += 1;
            }
        }
        
        bool success = true;
        if (0 == length)
        {
            
            HallDataMgr::getInstance()->AddpopLayer("提示", "昵称不能为空,请输入昵称", EM_MODE_TYPE_ENSURE);
            success = false;

        }else if (length > 32 || length < 6)
        {
             HallDataMgr::getInstance()->AddpopLayer("提示", "请输入6-32位字符", EM_MODE_TYPE_ENSURE);
             success = false;
        }
        
        if(nick.find(" ") != std::string::npos)
        {
            //有空格
            HallDataMgr::getInstance()->AddpopLayer("提示", "昵称中不能有空格", EM_MODE_TYPE_ENSURE);
            success = false;
        }
        
        if (!success)
        {
            
            return;
        }
        
        m_eModify = modify_nick;
        HallDataMgr::getInstance()->AddpopLayer("", "修改昵称中...", EM_MODE_TYPE_WAIT_TEXT);
        this->sendAlterIndividual(nick, HallDataMgr::getInstance()->m_cbGender);
    }
    
    
    
}

