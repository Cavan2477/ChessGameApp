/************************************************************************************
 * file: 		CommonPlazaLayer.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/28 22:36:04
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/28			
 *
 ************************************************************************************/

#include "CommonPlazaLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../Public/Help.h"
#include "../DataMgr/HallDataMgr.h"
#include "../DataMgr/NetworkMgr.h"
#include "Notice/Notice.h"
#include "Bank/BankShowScene.h"
#include "Task/TaskScene.h"
#include "Personal/PersonalScene.h"
#include "Shop/ShopScene.h"
#include "Rank/RankScene.h"
#include "Setting/SettingScene.h"
#include "Reward/RewardScene.h"

USING_NS_CC;

using namespace ui;

CommonPlazaLayer::CommonPlazaLayer():
m_root(nullptr),
m_headerRequestSprite(nullptr),
m_pLabelUserLevel(nullptr),
m_pLabelUserScore(nullptr),
m_pLabelUserNickname(nullptr)
{
    
}

CommonPlazaLayer::~CommonPlazaLayer()
{
    
}

bool CommonPlazaLayer::init()
{
    bool bRes = false;

    do
    {
        CC_BREAK_IF(!Layer::init());
        m_root = Layout::create();
        m_root->setContentSize(Size(1136, 640));
        
        auto rootNode = CSLoader::createNode("plazz/CommonPlazz.csb");
        rootNode->setPosition(0, 0);
        m_root->addChild(rootNode);
        
        //大厅标题
        m_pImageViewTitle = static_cast<ImageView*>(rootNode->getChildByName("Image_3"));
        CC_ASSERT(m_pImageViewTitle != nullptr);
        char tmpBuf[32] = "";
        
        sprintf(tmpBuf, "%d-title.png",HallDataMgr::getInstance()->m_dwKindID);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(tmpBuf);
        if (nullptr != frame)
        {
            m_pImageViewTitle->loadTexture(tmpBuf,Widget::TextureResType::PLIST);
            m_pImageViewTitle->setContentSize(Size(frame->getRect().size.width, frame->getRect().size.height));
        }
        else
        {
            sprintf(tmpBuf, "plaza/%d-title.png",HallDataMgr::getInstance()->m_dwKindID);
            auto sp = Sprite::create(tmpBuf);
            if (nullptr != sp)
            {
                m_pImageViewTitle->loadTexture(tmpBuf);
                m_pImageViewTitle->setContentSize(Size(sp->getContentSize().width, sp->getContentSize().height));
            }
        }
        
        //背景图片
        m_pSpriteImageListBack = static_cast<Sprite*>(rootNode->getChildByName("list_back"));
        CC_ASSERT(m_pSpriteImageListBack != nullptr);
        
        //大厅用户信息
        m_pLayoutUserInfo = static_cast<Layout*>(rootNode->getChildByName("info_panel"));
        CC_ASSERT(m_pLayoutUserInfo != nullptr);
        
        //奖励按钮动画
        Director::getInstance()->getTextureCache()->addImageAsync("plazz/animate.png", [=](Texture2D *)
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plazz/animate.plist");
            readAnimation("%d.png", "plazzAnimate", 14, 0.2);

            auto pos = Vec2(160, 585);

			if ((DWORD)EM_GAME_DEFALUT == HallDataMgr::getInstance()->m_dwKindID)
            {
                pos = Vec2(116, 585);
            }
            
            auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName("0.png");
            auto sprite = Sprite::createWithSpriteFrame(pframe);

            sprite->setAnchorPoint(Vec2(.5, .5));
            sprite->setPosition(pos);

            m_pLayoutUserInfo->addChild(sprite,10);

            auto paction = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("plazzAnimate")));

            sprite->runAction(paction);
            
            auto signBtn = Button::create();

            signBtn->setScale9Enabled(true);
            signBtn->setContentSize(Size(100, 100));
            signBtn->setPosition(pos);
            signBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithReward, this));

            m_pLayoutUserInfo->addChild(signBtn);
        });
        
        Button *pBtnHead = static_cast<Button *>(m_pLayoutUserInfo->getChildByName("head_back_btn"));

        if(nullptr != pBtnHead)
        {            
            pBtnHead->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithTouchUser, this));
        }
        
        //用户昵称
        m_pLabelUserNickname = Label::createWithSystemFont(HallDataMgr::getInstance()->m_pNickName, FONT_TREBUCHET_MS_BOLD, 24);
        m_pLabelUserNickname->setAnchorPoint(Vec2(.0, .5));
        m_pLabelUserNickname->setDimensions(129, m_pLabelUserNickname->getContentSize().height + 1);
        m_pLabelUserNickname->setPosition(Vec2(200, 100));
        m_pLayoutUserInfo->addChild(m_pLabelUserNickname);
        
        //用户分数
        m_pLabelUserScore = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","), FONT_TREBUCHET_MS_BOLD, 24);
        m_pLabelUserScore->setTextColor(cocos2d::Color4B::YELLOW);

        Labellengthdeal(m_pLabelUserScore, 135);
        m_pLabelUserScore->setAnchorPoint(Vec2(.0, .5));
        m_pLabelUserScore->setPosition(Vec2(180, 60));
        m_pLayoutUserInfo->addChild(m_pLabelUserScore);
        
        m_pLabelUserLevel = Label::createWithCharMap("plazz/level_num.png", 10, 13, '0');
        m_pLabelUserLevel->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
        m_pLabelUserLevel->setPosition(Vec2(170, 100));
        m_pLayoutUserInfo->addChild(m_pLabelUserLevel);
        
        m_root->setScaleX(JUDGE_SCALE);
        this->addChild(m_root);
        
        //购买
        Button *pBtnPurchase = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("btn_purchase"));

        if (nullptr != pBtnPurchase)
        {
            pBtnPurchase->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithShop, this));
        }
        
        //银行
        Button *pBtnBank = static_cast<Button *>(m_pLayoutUserInfo->getChildByName("bank_btn"));

        if (nullptr != pBtnBank)
        {
            pBtnBank->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithBank, this));
        }
        
        //任务
        Button *pBtnTask = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("task_btn"));

        if (nullptr != pBtnTask)
        {
            pBtnTask->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithTask, this));
        }
        
        //商城
        Button *pBtnShop = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("shop_btn"));

        if (nullptr != pBtnShop)
        {
            pBtnShop->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithShop, this));
        }
        
        //设置
        Button *pBtnSetting = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("set_btn"));

        if (nullptr != pBtnSetting)
        {
            pBtnSetting->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithSet, this));
        }
        
        //排行榜
        Button *pBtnRank = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("rank_btn"));

        if (nullptr != pBtnRank)
        {
            pBtnRank->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithRank, this));
        }
        
        //分享
        Button *pBtnShare = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("share_btn"));

        if (nullptr != pBtnShare)
        {
            pBtnShare->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithShare, this));
        }
        
        //锁定机器
        std::string file1 = "plazz/bt_unlock_0.png";
        std::string file2 = "plazz/bt_unlock_1.png";

        int nTag = EM_COMMON_PLAZA_LOCK_MACHINE;

        if (HallDataMgr::getInstance()->m_cbMoorMachine)    //已经锁定机器
        {
            file1 = "plazz/bt_lock_0.png";
            file2 = "plazz/bt_lock_1.png";
            nTag = EM_COMMON_PLAZA_UNLOCK_MACHINE;
        }

        Button *pBtnLock = Button::create(file1,file2,file1);
        pBtnLock->setName("bind_machine");
        pBtnLock->setPosition(Vec2(1084, 590));
        pBtnLock->setTag(nTag);
        pBtnLock->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithLock, this));

        m_pLayoutUserInfo->addChild(pBtnLock);
        
        bRes = true;
    } while (false);

    return bRes;
}

void CommonPlazaLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    //用户头像
	if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_NORMAL || 
        HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_VISITOR)
    {
        m_headerRequestSprite = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                      HallDataMgr::getInstance()->m_wCustom,
                                                      HallDataMgr::getInstance()->m_dwUserID,
                                                      HallDataMgr::getInstance()->m_cbGender);
    }
	else if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_RENREN ||
             HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_SINA)
    {
        m_headerRequestSprite = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl,
                                                   HallDataMgr::getInstance()->m_dwUserID);
    }

    if(nullptr != m_headerRequestSprite)
    {
        m_headerRequestSprite->setHeadSize(70.0);
        m_headerRequestSprite->setPosition(Vec2(102, 80));
        m_pLayoutUserInfo->addChild(m_headerRequestSprite);
    }
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(CommonPlazaLayer::notifyFreshInfo, this)), 1);
    
    //公告
    Notice *pNotice = Notice::CreateNotice();

    pNotice->setAnchorPoint(Vec2(.5, .5));
    pNotice->setContentSize(Size(475, 40));
    pNotice->setPosition(Vec2(568, 630));

    m_root->addChild(pNotice);
}

void CommonPlazaLayer::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    Layer::onExit();
}

void CommonPlazaLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
    
}

void CommonPlazaLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}

void CommonPlazaLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}

void CommonPlazaLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}

//MARK::按钮事件
void CommonPlazaLayer::buttonEventWithTouchUser(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(PERSONAL))
            return;
        
		PersonalScene *pPersonalScene = PersonalScene::create();

        pPersonalScene->setTag(PERSONAL);
        pPersonalScene->setPosition(Vec2(1136, 0));

        this->addChild(pPersonalScene);
        
        pPersonalScene->popPersonal();
    }
}

void CommonPlazaLayer::buttonEventWithBank(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(BANK))
            return;
        
		BankShowScene *pBankShowScene = BankShowScene::create();

        pBankShowScene->setTag(BANK);
        pBankShowScene->setPosition(Vec2(1136, 0));

        this->addChild(pBankShowScene);

        pBankShowScene->popBank();
    }
}
void CommonPlazaLayer::buttonEventWithReward(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
		RewardScene *pRewardScene = RewardScene::create();

        pRewardScene->setScale(0.5f);

        this->addChild(pRewardScene);
        
        Sequence *quence = Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.05), ScaleTo::create(0.2f, 1.0));

        pRewardScene->runAction(quence);
    }
}

void CommonPlazaLayer::buttonEventWithShop(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(SHOP))
            return;
        
		ShopScene *pShopScene = ShopScene::create();

        pShopScene->setTag(SHOP);
        pShopScene->setPosition(Vec2(1136, 0));

        this->addChild(pShopScene);
        
        pShopScene->popShop();
    }
}

void CommonPlazaLayer::buttonEventWithTask(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(TASK))
            return;
        
		TaskScene *pTaskScene = TaskScene::create();
        pTaskScene->setTag(TASK);
        pTaskScene->setPosition(Vec2(1136, 0));

        this->addChild(pTaskScene);

        pTaskScene->popTask();
    }
}
void CommonPlazaLayer::buttonEventWithSet(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
		SettingScene *pSettingScene = SettingScene::create();
        
        this->addChild(pSettingScene);
    }
}
void CommonPlazaLayer::buttonEventWithRank(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(RANK))
            return;
        
		RankScene *pRankScene = RankScene::create();
        pRankScene->setTag(RANK);
        pRankScene->setPosition(Vec2(1136, 0));

        this->addChild(pRankScene);

        pRankScene->popRank();
    }
}
void CommonPlazaLayer::buttonEventWithShare(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        auto pbg = Button::create();

        pbg->setScale9Enabled(true);
        pbg->setContentSize(Size(1136, 640));
        pbg->setPosition(Vec2(568, 320));

        pbg->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type)
        {
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                pbg->removeFromParent();
            }
        });

        m_root->addChild(pbg);
        
        auto shareRoot = CSLoader::createNode("share_res/share.csb");

        pbg->addChild(shareRoot);
    }
}

void CommonPlazaLayer::buttonEventWithLock(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType tType)
{
    Widget *pWidget = static_cast<Widget*>(target);

    if (Widget::TouchEventType::ENDED == tType)
    {
        switch (pWidget->getTag())
        {
            case EM_COMMON_PLAZA_LOCK_MACHINE:
            {
                initBindMachine("请输入银行密码绑定机器");
            }
                break;

            case EM_COMMON_PLAZA_UNLOCK_MACHINE:
            {
                initBindMachine("请输入银行密码解除绑定");
            }
                break;

            default:
                break;
        }
    }
}

//MARK:: 等级信息
void CommonPlazaLayer::LevelUpgrade(void* pData, WORD wSize)
{
    CMD_GP_GROW_LEVEL_UPGRADE *pLevelUpgrade = (CMD_GP_GROW_LEVEL_UPGRADE *)pData;

    HallDataMgr::getInstance()->m_UserScore = pLevelUpgrade->lCurrScore;
    HallDataMgr::getInstance()->m_Ingot = pLevelUpgrade->lCurrIngot;
    
    m_pLabelUserScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","));
    
    std::string strNotifyContent = WHConverUnicodeToUtf8WithArray((WORD*)pLevelUpgrade->szNotifyContent);

	HallDataMgr::getInstance()->AddpopLayer("", strNotifyContent, EM_MODE_TYPE_ENSURE);
}

//等级信息
void CommonPlazaLayer::LevelInfo(void* pData, WORD wSize)
{
    memcpy(&HallDataMgr::getInstance()->m_levelData, pData, sizeof(HallDataMgr::getInstance()->m_levelData));
    
    m_pLabelUserLevel->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
    
    auto action = CallFunc::create([]
    {
        NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

void CommonPlazaLayer::notifyFreshInfo(cocos2d::EventCustom *event)
{    
    auto type = (__Integer*)event->getUserData();
    switch (type->getValue())
    {
		// 修改用户名
		case EM_USER_DATA_CHANGE_NAME:
        {
            m_pLabelUserNickname->setString(HallDataMgr::getInstance()->m_pNickName);
            
        }
            break;

		// 修改用户头像
		case EM_USER_DATA_CHANGE_HEAD:
        {
            auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(HallDataMgr::getInstance()->m_dwUserID);

            if (ptexture)
                m_headerRequestSprite->setTexture(ptexture);
        }
            break;            

		// 变更
		case EM_USER_DATA_CHANGE_SCORE:
		case EM_USER_DATA_CHANGE_BEAN:
        {
            m_pLabelUserScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","));
        }

            break;

        default:
            break;
    }
}

void CommonPlazaLayer::hideUserInfo(bool bHide)
{
    m_pLayoutUserInfo->setVisible(!bHide);
    
    SpriteFrame *frame = nullptr;
    if (bHide)
    {
        auto sp = Sprite::create("plazz/tablelist_bg.png");
        if (nullptr != sp)
        {
            frame = sp->getSpriteFrame();
        }
    }
    else
    {
        auto sp = Sprite::create("plazz/list_back.png");
        if (nullptr != sp)
        {
            frame = sp->getSpriteFrame();
        }
    }
    
    if (nullptr != frame)
    {
        m_pSpriteImageListBack->setSpriteFrame(frame);
    }
}

void CommonPlazaLayer::initBindMachine(const std::string &title)
{
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_SUCCESS, CC_CALLBACK_2(CommonPlazaLayer::operatesuccessResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_FAILURE, CC_CALLBACK_2(CommonPlazaLayer::operatefailureResult, this));
    
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
									   HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_WAIT);
                                       
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

void CommonPlazaLayer::operatesuccessResult(void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    auto btn = static_cast<Button*>(m_pLayoutUserInfo->getChildByName("bind_machine"));
    std::string normalfile = "plazz/bt_unlock_0.png";
    std::string prefile = "plazz/bt_unlock_1.png";
    int nTag = EM_COMMON_PLAZA_LOCK_MACHINE;
    if (HallDataMgr::getInstance()->m_cbMoorMachine)
    {
        HallDataMgr::getInstance()->m_cbMoorMachine = 0;
    }
    else
    {
        HallDataMgr::getInstance()->m_cbMoorMachine = 1;
        
        normalfile = "plazz/bt_lock_0.png";
        prefile = "plazz/bt_lock_1.png";
        nTag = EM_COMMON_PLAZA_UNLOCK_MACHINE;
    }
    if (nullptr != btn)
    {
        btn->loadTextureNormal(normalfile);
        btn->loadTexturePressed(prefile);
        btn->loadTextureDisabled(normalfile);
        btn->setTag(nTag);
    }
    
    auto pResult = (CMD_GP_OPERATE_SUCC *)pData;

    std::string strResultDes = WHConverUnicodeToUtf8WithArray((WORD*)pResult->szDescription);

	HallDataMgr::getInstance()->AddpopLayer("系统提示", strResultDes, EM_MODE_TYPE_ENSURE);
    
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
}

void CommonPlazaLayer::operatefailureResult(void *pData, WORD wSize)
{
	HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);

    if (nullptr == pData || 0 == wSize)
        return;

    auto pResult = (CMD_GP_OPERATE_FAILURE *)pData;

    std::string strDes = WHConverUnicodeToUtf8WithArray((WORD*)pResult->szDescription);

	HallDataMgr::getInstance()->AddpopLayer("系统提示", strDes, EM_MODE_TYPE_ENSURE);
    
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
}