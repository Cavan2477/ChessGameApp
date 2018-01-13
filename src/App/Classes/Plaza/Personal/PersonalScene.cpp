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
m_emPersonalType(EM_PERSONAL_TYPE_INFO),
m_emPersonalModifyType(EM_PERSONAL_MODIFY_TYPE_SEX),
m_pUiLayoutPersonal(nullptr),
m_pUiLayoutModifyLogon(nullptr),
m_pUiLayoutModifyBank(nullptr),
_headSprite(nullptr),
m_pPhotoBridge(nullptr)
{
}

PersonalScene::~PersonalScene()
{
    CC_SAFE_RELEASE(_headSprite);
	CC_SAFE_RELEASE(m_pPhotoBridge);
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
    layout->setTag(EM_INFO_LAYOUT_MAIN);
    layout->setContentSize(Size(1136, 640));
    
    auto rootNode = CSLoader::createNode("personal_res/PersonalMain.csb");
    rootNode->setTag(EM_INFO_ROOT_MAIN);
    layout->addChild(rootNode);
    
    //基本信息
    m_pUiLayoutPersonal = Layout::create();
    m_pUiLayoutPersonal->setContentSize(Size(1136, 640));
    layout->addChild(m_pUiLayoutPersonal);
    
    auto personalRoot = CSLoader::createNode("personal_res/Personal.csb");
    personalRoot->setTag(1);
    m_pUiLayoutPersonal->addChild(personalRoot);
    
    //修改登录密码
    m_pUiLayoutModifyLogon = Layout::create();
    m_pUiLayoutModifyLogon->setContentSize(Size(1136, 640));
    layout->addChild(m_pUiLayoutModifyLogon);
    m_pUiLayoutModifyLogon->setVisible(false);
    
    auto LoginModifyRoot = CSLoader::createNode("personal_res/Modify.csb");
    LoginModifyRoot->setTag(2);
    m_pUiLayoutModifyLogon->addChild(LoginModifyRoot);
    
    auto btnModifyLogonPwd = static_cast<Button *>(LoginModifyRoot->getChildByName("Button_sureModify"));

    if (btnModifyLogonPwd != nullptr)
    {
        if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_VISITOR || HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_SINA)
        {
            btnModifyLogonPwd->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type)
			{    
                if (type == Widget::TouchEventType::ENDED)
                {       
                    HallDataMgr::getInstance()->AddpopLayer("系统提示", "游客或第三方帐号不支持修改登录密码功能", EM_MODE_TYPE_ENSURE);
                }
            });
        }
		else
            btnModifyLogonPwd->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithSureModify, this));
    }
    
    //修改银行密码
    m_pUiLayoutModifyBank = Layout::create();
    m_pUiLayoutModifyBank->setContentSize(Size(1136, 640));
    layout->addChild(m_pUiLayoutModifyBank);
    m_pUiLayoutModifyBank->setVisible(false);
    
    auto bankModifyRoot = CSLoader::createNode("personal_res/Modify.csb");
    bankModifyRoot->setTag(3);
    m_pUiLayoutModifyBank->addChild(bankModifyRoot);
    auto bankPassModifyBtn = static_cast<Button *>(bankModifyRoot->getChildByName("Button_sureModify"));

    if  (bankPassModifyBtn != nullptr)
        bankPassModifyBtn->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithSureModify, this));
    
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
    personal->setTag(EM_INFO_TEXT_PERSONAL);

    if (nullptr != personal)
        personal->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithMenu, this));
    
    //帐号
    auto account = static_cast<Text *>(personalRoot->getChildByName("Text_account"));

    if (account != nullptr)
    {
        account->setString(HallDataMgr::getInstance()->m_strAccounts);
        account->setTextColor(cocos2d::Color4B(36,236,250,255));
        account->setTextAreaSize(Size(200, account->getContentSize().height ));
    }
    
    //昵称
    auto nick = static_cast<Text *>(personalRoot->getChildByName("Text_nick"));

    if (nick != nullptr)
    {
        nick->setString(HallDataMgr::getInstance()->m_strNickName);
        nick->setTextColor(cocos2d::Color4B(36,236,250,255));
        nick->setTextAreaSize(Size(200, account->getContentSize().height));
        nick->setTag(EM_INFO_TEXT_NICKNAME);
    }
    
    //修改昵称
    EditBox *edit = EditBox::create(Size(318, 58), "personal_res/info_commonText.png");

    edit->setDelegate(this);
    edit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    edit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    edit->setPosition(cocos2d::Point(nick->getPositionX(), nick->getPositionY()));
    edit->setMaxLength(LEN_MAX_ACCOUNT);
    edit->setFontSize(28);
    edit->setReturnType(EditBox::KeyboardReturnType::DONE);
    edit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
    edit->setPlaceHolder("");
    edit->setTag(EM_INFO_EDIT_NICKNAME);
    edit->setFontColor(Color3B::YELLOW);
    edit->setVisible(false);

    personalRoot->addChild(edit);
    
    //修改按钮
    Button *pBtnModify = Button::create("personal_res/userinfo_modiyNick_0.png","personal_res/userinfo_modiyNick_1.png");

    pBtnModify->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithModifyNick, this));
    pBtnModify->setAnchorPoint(Vec2(.0, .5));
    pBtnModify->setTag(EM_INFO_BTN_MODIFY);
    pBtnModify->setPosition(Vec2(nick->getPositionX() + 230, nick->getPositionY()));
    personalRoot->addChild(pBtnModify);
    pBtnModify->setVisible(false);
    pBtnModify->setEnabled(false);
    
    //id
    auto pTextId = static_cast<Text *>(personalRoot->getChildByName("Text_id"));

    if (pTextId != nullptr)
    {
        pTextId->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_dwGameID)->getCString());
        pTextId->setTextColor(cocos2d::Color4B(36,236,250,255));
    }
    
    //金币
    auto pTextGold = static_cast<Text *>(personalRoot->getChildByName("Text_ingot"));

    if (pTextGold != nullptr)
    {
        pTextGold->setString(__String::createWithFormat("%lld",HallDataMgr::getInstance()->m_llGold)->getCString());
        pTextGold->setTextColor(cocos2d::Color4B(228,235,55,255));
    }
    
    //游戏币
    auto pTextGameCoin = static_cast<Text *>(personalRoot->getChildByName("Text_coin"));

    if (pTextGameCoin != nullptr)
    {
        pTextGameCoin->setString(__String::createWithFormat("%lld",HallDataMgr::getInstance()->m_llUserGold)->getCString());
        pTextGameCoin->setTextColor(cocos2d::Color4B(228,235,55,255));
    }
    
    //游戏豆
    auto pTextBean = static_cast<Text *>(personalRoot->getChildByName("Text_bean"));

    if (pTextBean != nullptr)
    {
        pTextBean->setString(__String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_dBean)->getCString());
        pTextBean->setTextColor(cocos2d::Color4B(228,235,55,255));
    }
    
    //兑换
    auto pBtnExchange = static_cast<Button *>(personalRoot->getChildByName("Button_exchange"));

    if (pBtnExchange != nullptr)
    {
        pBtnExchange->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type)
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
        });
    }
    
    //取款
    auto pBtnBank = static_cast<Button *>(personalRoot->getChildByName("Button_takeCoin"));

    if (nullptr != pBtnBank)
    {
       pBtnBank->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
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
    auto pBtnPurchase = static_cast<Button *>(personalRoot->getChildByName("Button_purchase"));

    if (nullptr != pBtnPurchase)
    {
        pBtnPurchase->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                if (this->getChildByTag(SHOP))
                    return;

                ShopScene *pShopScene = ShopScene::create();

                pShopScene->setTag(SHOP);
                pShopScene->setPosition(Vec2(1136, 0));

                this->addChild(pShopScene);

                pShopScene->setIndex(1);
                pShopScene->popShop();
            }
        });
    }
    
    //性别
    BYTE  cbGender = HallDataMgr::getInstance()->m_cbGender;
    
    auto pBtnMale = static_cast<Button *>(personalRoot->getChildByName("Button_sex_1"));
    auto pBtnFemale = static_cast<Button *>(personalRoot->getChildByName("Button_sex_0"));
    
    if (nullptr != pBtnMale && nullptr != pBtnFemale)
    {
        pBtnMale->setTag(EM_INFO_TEXT_FALE);
        pBtnFemale->setTag(EM_INFO_TEXT_FEMALE);
        
        if (cbGender == 1)
        {
            pBtnMale->loadTextures("personal_res/info_sex_1.png", "personal_res/info_sex_0.png");
            pBtnFemale->loadTextures("personal_res/info_sex_0.png", "personal_res/info_sex_1.png");
        }else
        {
            pBtnMale->loadTextures("personal_res/info_sex_0.png", "personal_res/info_sex_1.png");
            pBtnFemale->loadTextures("personal_res/info_sex_1.png", "personal_res/info_sex_0.png");
            
        }
        
        pBtnMale->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithGender, this));
        pBtnFemale->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithGender, this));
        
    }

    //修改登录密码
    auto modifyLogin = static_cast<Button *>(rootNode->getChildByName("Button_modifyLoginPass"));
    modifyLogin->setTag(EM_INFO_EDIT_LOGON_PWD);
    if (nullptr != modifyLogin)
    {
        modifyLogin->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithMenu, this));
    }
    
    
    //修改银行密码
    auto modifyBank = static_cast<Button *>(rootNode->getChildByName("Button_modifyBankPass"));
    modifyBank->setTag(EM_INFO_EDIT_BANK_PWD);
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
        m_pUiLayoutPersonal->addChild(_headSprite);
        
        //上传头像
        auto  modify = Button::create("personal_res/info_pen.png");
        modify->addTouchEventListener(CC_CALLBACK_2(PersonalScene::buttonEventWithUploadRes, this));
        modify->setPosition(Vec2(340,285));
        m_pUiLayoutPersonal->addChild(modify);
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
        float percent =  (HallDataMgr::getInstance()->m_levelData.dwNextLevelExp > 0) ?
        HallDataMgr::getInstance()->m_levelData.dwCurrExp/(float) HallDataMgr::getInstance()->m_levelData.dwNextLevelExp : 0;
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
            
            upgrade->setString("您已经达到最高等级");
            upgrade->setTextColor(cocos2d::Color4B::YELLOW);
            reward->setVisible(false);
            
        }else
        {
            upgrade->setString(__String::createWithFormat("下次升级还需要%d经验",HallDataMgr::getInstance()->m_levelData.dwNextLevelExp -
                                                          HallDataMgr::getInstance()->m_levelData.dwCurrExp)->getCString());
            upgrade->setTextColor(cocos2d::Color4B::YELLOW);
            
            reward->setString(__String::createWithFormat("奖励%lld游戏币+%lld金币",HallDataMgr::getInstance()->m_levelData.lUpgradeRewardGameCoin,
                                                         HallDataMgr::getInstance()->m_levelData.lUpgradeRewardGold)->getCString());
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
    auto modify = (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)  ? static_cast<Node *>(m_pUiLayoutModifyLogon->getChildByTag(2)) :
                                                       static_cast<Node *>(m_pUiLayoutModifyBank->getChildByTag(3))  ;
    
    if (modify->getChildByTag(100))
        return;

    for (int i = 0 ; i < 3 ; i++)
    {
        __String strResult;
        
        if ( 0 == i)
        {
            if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)
               strResult = "请输入您的登录密码";
            else
                strResult = "请输入您的银行密码";
        }
		else if (1 == i)
			strResult = "请输入您的新密码";
        else
            strResult = "请重复您刚才输入的新密码";
        
        assert(modify);

        Text *text = static_cast<Text *>(modify->getChildByName(__String::createWithFormat("Text_%d",i)->getCString()));
        
        EditBox *edit = EditBox::create(Size(318, 58), "personal_res/info_commonText.png");
        
		edit->setDelegate(this);
        edit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
        edit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
        edit->setPosition(cocos2d::Point(text->getPositionX() + 60, text->getPositionY()));
        edit->setMaxLength(LEN_PWD);
        edit->setFontSize(28);
        edit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
        edit->setReturnType(EditBox::KeyboardReturnType::DONE);
        edit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
        edit->setPlaceholderFontColor(cocos2d::Color3B(36,236,250));
        edit->setPlaceHolder(strResult.getCString());
        edit->setTag(100 + i);
        edit->setFontColor(Color3B::YELLOW);

        modify->addChild(edit);
    }
}

//MARK::Notify
void PersonalScene::userFaceinfoResult(void *pData, WORD wSize)
{
    auto result = (_stCmdGpUserFaceInfo *)pData;
    HallDataMgr::getInstance()->m_wFaceID = result->wFaceID;
    HallDataMgr::getInstance()->m_wCustom = result->dwCustomID;
    
    HallDataMgr::getInstance()->m_Headlist.insert(HallDataMgr::getInstance()->m_dwUserID, _headSprite->getTexture());
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    
    HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
    
    //更新
    EventCustom event(STR_EVENT_USER_DATA_CHANGE);

    auto value = __Integer::create(EM_USER_DATA_CHANGE_HEAD);
    event.setUserData(value);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void PersonalScene::operatesuccessResult(void *pData, WORD wSize)
{
    auto presult = (ST_CMD_GP_OPERATE_SUCC *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray((WORD*)presult->szDes);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, EM_MODE_TYPE_ENSURE);
    HallDataMgr::getInstance()->m_cbGender = m_cbGender;
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);

    if (m_emPersonalType == EM_PERSONAL_TYPE_INFO)
    {
        
        auto personalRoot = static_cast<Node *>(m_pUiLayoutPersonal->getChildByTag(1));
        auto manBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_1"));
        
        auto womanBtn = static_cast<Button *>(personalRoot->getChildByName("Button_sex_0"));
        
        if (m_emPersonalModifyType == EM_PERSONAL_MODIFY_TYPE_SEX)
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
        }else if (m_emPersonalModifyType == EM_PERSONAL_MODIFY_TYPE_NICK_NAME)
        {
            auto editBox = static_cast<EditBox *>(personalRoot->getChildByTag(EM_INFO_EDIT_NICKNAME));
            HallDataMgr::getInstance()->m_strNickName = editBox->getText();
            
            auto nick = static_cast<Text *>(personalRoot->getChildByTag(EM_INFO_TEXT_NICKNAME));
            nick->setString(editBox->getText());
            
            //更新大厅显示
            EventCustom event(STR_EVENT_USER_DATA_CHANGE);
            auto value = __Integer::create(EM_USER_DATA_CHANGE_NAME);
            event.setUserData(value);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
        
    }
    else if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)
    {
        auto modify = (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)  ? static_cast<Node *>(m_pUiLayoutModifyLogon->getChildByTag(2)) :
        static_cast<Node *>(m_pUiLayoutModifyBank->getChildByTag(3))  ;
        
 
        EditBox *orignalPass = static_cast<EditBox *>(modify->getChildByTag(100));
        EditBox *newPass = static_cast<EditBox *>(modify->getChildByTag(101));
        EditBox *confirmPass = static_cast<EditBox *>(modify->getChildByTag(102));
      
        
        HallDataMgr::getInstance()->m_strPwd = MD5Encrypt(newPass->getText());
        CFramList::getInstance()->addAccountListWithString(HallDataMgr::getInstance()->m_strAccounts, HallDataMgr::getInstance()->m_strPwd);
        
        orignalPass->setText("");
        newPass->setText("");
        confirmPass->setText("");
        
    }
    else if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_BANK_PWD)
    {
        
        
        auto modify = (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)  ? static_cast<Node *>(m_pUiLayoutModifyLogon->getChildByTag(2)) :
        static_cast<Node *>(m_pUiLayoutModifyBank->getChildByTag(3))  ;
        
        
        EditBox *orignalPass = static_cast<EditBox *>(modify->getChildByTag(100));
        EditBox *newPass = static_cast<EditBox *>(modify->getChildByTag(101));
        EditBox *confirmPass = static_cast<EditBox *>(modify->getChildByTag(102));
        
        HallDataMgr::getInstance()->m_strBankPwd = MD5Encrypt(newPass->getText());
        
        orignalPass->setText("");
        newPass->setText("");
        confirmPass->setText("");
    }
}

void PersonalScene::operatefailureResult(void *pData, WORD wSize)
{
    auto presult = (ST_CMD_GP_OPERATE_FAILURE *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray((WORD*)presult->szDes);
    HallDataMgr::getInstance()->AddpopLayer("错误提示", str, EM_MODE_TYPE_ENSURE);
    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
}

//MARK::按钮事件
void PersonalScene::buttonEventWithMenu(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        auto btn = static_cast<Button *>(target);
        auto mainLayout = static_cast<Layout *>(this->getChildByTag(EM_INFO_LAYOUT_MAIN));
        auto mainRoot = static_cast<Node *>(mainLayout->getChildByTag(EM_INFO_ROOT_MAIN));
        auto personal = static_cast<Button *>(mainRoot->getChildByTag(EM_INFO_TEXT_PERSONAL));
        auto loginPass = static_cast<Button *>(mainRoot->getChildByTag(EM_INFO_EDIT_LOGON_PWD));
        auto bankPass = static_cast<Button *>(mainRoot->getChildByTag(EM_INFO_EDIT_BANK_PWD));
        
        int tag = btn->getTag();
        if(tag == EM_INFO_TEXT_PERSONAL )
        {
            if (m_emPersonalType == EM_PERSONAL_TYPE_INFO)
            {
                
                return;
            }
            
            m_emPersonalType = EM_PERSONAL_TYPE_INFO;
            m_pUiLayoutPersonal->setVisible(true);
            m_pUiLayoutModifyLogon->setVisible(false);
            m_pUiLayoutModifyBank->setVisible(false);
            
            if (personal && loginPass && bankPass)
            {
                
                personal->loadTextures("personal_res/info_personalIcon.png", "personal_res/info_personalBtn.png");
                loginPass->loadTextures("personal_res/info_loginPassModifyBtn.png", "personal_res/info_pwdIcon.png");
                bankPass->loadTextures("personal_res/info_bankPassModifyBtn.png", "personal_res/info_bankpwdIcon.png");
            }
            
        }
        if(tag == EM_INFO_EDIT_LOGON_PWD )
        {
            if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)
            {
                return;
            }
            
            m_emPersonalType = EM_PERSONAL_TYPE_MODIFY_LOGON_PWD;
            m_pUiLayoutPersonal->setVisible(false);
            m_pUiLayoutModifyLogon->setVisible(true);
            m_pUiLayoutModifyBank->setVisible(false);
            
            if (personal && loginPass && bankPass)
            {
                
                personal->loadTextures("personal_res/info_personalBtn.png", "personal_res/info_personalIcon.png");
                loginPass->loadTextures("personal_res/info_pwdIcon.png", "personal_res/info_loginPassModifyBtn.png");
                bankPass->loadTextures("personal_res/info_bankPassModifyBtn.png", "personal_res/info_bankpwdIcon.png");
            }
            
            this->initModify();
        }
        if(tag == EM_INFO_EDIT_BANK_PWD)
        {
            if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_BANK_PWD)
            {
                return;
            }
            
            m_emPersonalType = EM_PERSONAL_TYPE_MODIFY_BANK_PWD;
            m_pUiLayoutPersonal->setVisible(false);
            m_pUiLayoutModifyLogon->setVisible(false);
            m_pUiLayoutModifyBank->setVisible(true);
            
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
        
        auto personalRoot = static_cast<Node *>(m_pUiLayoutPersonal->getChildByTag(1));
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
            
            this->sendAlterIndividual(HallDataMgr::getInstance()->m_strNickName, m_cbGender);
        }
        
        if (btn == womanBtn)
        {
            if (HallDataMgr::getInstance()->m_cbGender == WOMAN)
            {
                return;
            }
            HallDataMgr::getInstance()->AddpopLayer("", "发送资料中...", EM_MODE_TYPE_WAIT_TEXT);
            m_cbGender = 0;
            
            this->sendAlterIndividual(HallDataMgr::getInstance()->m_strNickName, m_cbGender);
        }
        

         m_emPersonalModifyType = EM_PERSONAL_MODIFY_TYPE_SEX;
    }
}

//更换头像
void PersonalScene::buttonEventWithUploadRes(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (m_pUiLayoutPersonal->getChildByTag(EM_INFO_IMAGE_BG))
            return;
        
        auto image = ImageView::create();
        image->setContentSize(Size(1136, 640));
        image->setScale9Enabled(true);
        image->setTouchEnabled(true);
        image->setPosition(Vec2(568, 320));
        image->setTag(EM_INFO_IMAGE_BG);
        m_pUiLayoutPersonal->addChild(image);
        
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

                if (!m_pPhotoBridge)
                    m_pPhotoBridge = new PhotoBridge;

                m_pPhotoBridge->openPhoto();
                m_pPhotoBridge->setChoiceType(0);

				m_pPhotoBridge->setPhotoCallback(CC_CALLBACK_1(PersonalScene::photocomplete, this));

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
        
        auto personalRoot = static_cast<Node *>(m_pUiLayoutPersonal->getChildByTag(1));
        Text    *nick = static_cast<Text *>(personalRoot->getChildByTag(EM_INFO_TEXT_NICKNAME));
        nick->setVisible(false);
        
        
        auto editbox = static_cast<EditBox *>(personalRoot->getChildByTag(EM_INFO_EDIT_NICKNAME));
        editbox->setVisible(true);
        editbox->touchDownAction(editbox, cocos2d::ui::Widget::TouchEventType::ENDED);
    }
    
}

void PersonalScene::buttonEventWithSureModify(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_BANK_PWD)
        {
            
            if (!HallDataMgr::getInstance()->m_cbInsureEnable)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "您暂未开通银行密码,请先开通银行密码", EM_MODE_TYPE_ENSURE);
                return;
            }
        }
        
        auto modify = (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)  ? static_cast<Node *>(m_pUiLayoutModifyLogon->getChildByTag(2)) :
                                                           static_cast<Node *>(m_pUiLayoutModifyBank->getChildByTag(3))  ;
        
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
        
        if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)
        {
        
            if (std::strcmp(MD5Encrypt(std::string(orignalPass->getText())).c_str() , HallDataMgr::getInstance()->m_strPwd.c_str()) != 0)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "您输入的原始密码有误", EM_MODE_TYPE_ENSURE);
               return;
                
            }
        }else
        {
            if (std::strcmp(MD5Encrypt(std::string(orignalPass->getText())).c_str() , HallDataMgr::getInstance()->m_strBankPwd.c_str()) != 0)
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
        
        if  (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_LOGON_PWD)
        {
            SendModifyLogonPwd(std::string(orignalPass->getText()), newPassStr);
            
        }else if (m_emPersonalType == EM_PERSONAL_TYPE_MODIFY_BANK_PWD)
        {
            
            
            
            SendModifyBankPwd(std::string(orignalPass->getText()), newPassStr);
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
    _stCmdGpSysFaceInfo SystemFaceInfo;
    memset(&SystemFaceInfo, 0, sizeof(_stCmdGpSysFaceInfo));
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPwd, (WORD*)SystemFaceInfo.szPwd);
    SystemFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    SystemFaceInfo.wFaceID = wface;
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_SYSTEM_FACE_INFO, &SystemFaceInfo, sizeof(_stCmdGpSysFaceInfo),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::sendCustomFaceInfo(cocos2d::Image *pImage)
{
    auto pdate = pImage->getData();
    int length = (int)pImage->getDataLen();
    
	// 图片大小是否有问题
	char szImage[FACE_SCX * FACE_SCY] = { 0 };

    for(int i=0; i<length/4;i++)
    {
        szImage[i*4]=pdate[i*4+2];
        szImage[i*4+1]=pdate[i*4+1];
        szImage[i*4+2]=pdate[i*4];
        szImage[i*4+3]=255;
    }
    
    _stCmdGpCustomFaceInfo CustomFaceInfo;
    memset(&CustomFaceInfo, 0, sizeof(CustomFaceInfo));
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPwd, (WORD*)CustomFaceInfo.szPwd);
    CustomFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(CustomFaceInfo.dwCustomFace, szImage, length);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CUSTOM_FACE_INFO, &CustomFaceInfo, sizeof(_stCmdGpCustomFaceInfo),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::sendAlterIndividual(const std::string &name, BYTE cbgerder, int type)
{
	BYTE buffer[256] = { 0 };
	_stCmdGpModifyIndividual modifyindividual = { 0 };

	Utf8ToUtf16(HallDataMgr::getInstance()->m_strPwd, (WORD*)modifyindividual.szPwd);

    modifyindividual.cbGender = cbgerder;
    modifyindividual.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(buffer, &modifyindividual, sizeof(_stCmdGpModifyIndividual));

    int nSize = sizeof(_stCmdGpModifyIndividual);

    //if (type)
    {
        int nSizeTemp = (int)name.size()+1;
        
		_stUserDataExt stUserDataExt = { 0 };

        stUserDataExt.wDataSize = nSizeTemp*sizeof(TCHAR);
        stUserDataExt.wDataDesc = DTP_GP_UI_NICKNAME;

        memcpy(buffer+nSize, &stUserDataExt, sizeof(_stUserDataExt));

        nSize += sizeof(_stUserDataExt);
        
		TCHAR szName[LEN_MAX_ACCOUNT] = { 0 };

        Utf8ToUtf16(name, (WORD*)szName);

        memcpy(buffer+nSize, szName, stUserDataExt.wDataSize);
		nSize += (INT)stUserDataExt.wDataSize;
    }
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INDIVIDUAL, &buffer, nSize,NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::SendModifyLogonPwd(const std::string &oldpass, const std::string &newpass)
{
	_stCmdGpModifyLogonPwd stCmdGpModifyLogonPwd = { 0 };

    stCmdGpModifyLogonPwd.dwUserID = HallDataMgr::getInstance()->m_dwUserID;

	Utf8ToUtf16(MD5Encrypt(oldpass), (WORD*)stCmdGpModifyLogonPwd.szOldPwd);
	Utf8ToUtf16(MD5Encrypt(newpass), (WORD*)stCmdGpModifyLogonPwd.szNewPwd);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_LOGON_PASS, &stCmdGpModifyLogonPwd, sizeof(stCmdGpModifyLogonPwd),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonalScene::SendModifyBankPwd(const std::string &strOldPwd, const std::string &strNewPwd)
{
	_stCmdGpModifyBankPwd request = { 0 };

    request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;

	Utf8ToUtf16(MD5Encrypt(strOldPwd), (WORD*)request.szOldPwd);
	Utf8ToUtf16(MD5Encrypt(strNewPwd), (WORD*)request.szNewPwd);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_BANK_PWD, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
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
    if (m_emPersonalType == EM_PERSONAL_TYPE_INFO)
    {
        m_cbGender = HallDataMgr::getInstance()->m_cbGender;
        
        auto personalRoot = static_cast<Node *>(m_pUiLayoutPersonal->getChildByTag(1));

        Button *modify = static_cast<Button *>(personalRoot->getChildByTag(EM_INFO_BTN_MODIFY));
        modify->setVisible(true);
        
        Text    *_nick = static_cast<Text *>(personalRoot->getChildByTag(EM_INFO_TEXT_NICKNAME));
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
            } 
			else
            {
                i = i + 1;
                length += 1;
            }
        }
        
        bool bSucc = true;

        if (0 == length)
        {
            HallDataMgr::getInstance()->AddpopLayer("提示", "昵称不能为空,请输入昵称", EM_MODE_TYPE_ENSURE);
            bSucc = false;

        }
		else if (length > LEN_MAX_ACCOUNT || length < 6)
        {
             HallDataMgr::getInstance()->AddpopLayer("提示", "请输入6-32位字符", EM_MODE_TYPE_ENSURE);
             bSucc = false;
        }
        
        if(nick.find(" ") != std::string::npos)
        {
            //有空格
            HallDataMgr::getInstance()->AddpopLayer("提示", "昵称中不能有空格", EM_MODE_TYPE_ENSURE);
            bSucc = false;
        }
        
        if (!bSucc)
        {
            
            return;
        }
        
        m_emPersonalModifyType = EM_PERSONAL_MODIFY_TYPE_NICK_NAME;
        HallDataMgr::getInstance()->AddpopLayer("", "修改昵称中...", EM_MODE_TYPE_WAIT_TEXT);
        this->sendAlterIndividual(nick, HallDataMgr::getInstance()->m_cbGender);
    }
    
    
    
}

