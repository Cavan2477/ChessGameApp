//
//  CommonPlazaLayer.cpp
//  GameProject
//
//  Created by zhong on 3/25/16.
//
//

#include "CommonPlazaLayer.h"
#include"cocostudio/CocoStudio.h"
#include "Help.h"
#include "Notice.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"

#include "BankShow.h"
#include "Task.h"
#include "Personal.h"
#include "Shop.h"
#include "Rank.h"
#include "Setting.h"
#include "Reward.h"

USING_NS_CC;
using namespace ui;

enum CommonPlazaTag
{
    TAG_MainLayout = 50,
    TAG_LockMachine,
    TAG_UnlockMachine
};
CommonPlazaLayer::CommonPlazaLayer():
m_root(nullptr),
_headSprite(nullptr),
_UserLevel(nullptr),
_UserScore(nullptr),
_UserNikcName(nullptr)
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
        m_imageTitle = static_cast<ImageView*>(rootNode->getChildByName("Image_3"));
        CC_ASSERT(m_imageTitle != nullptr);
        char tmpBuf[32] = "";
        
        sprintf(tmpBuf, "%d-title.png",HallDataMgr::getInstance()->m_dwKindID);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(tmpBuf);
        if (nullptr != frame)
        {
            m_imageTitle->loadTexture(tmpBuf,Widget::TextureResType::PLIST);
            m_imageTitle->setContentSize(Size(frame->getRect().size.width, frame->getRect().size.height));
        }
        else
        {
            sprintf(tmpBuf, "plaza/%d-title.png",HallDataMgr::getInstance()->m_dwKindID);
            auto sp = Sprite::create(tmpBuf);
            if (nullptr != sp)
            {
                m_imageTitle->loadTexture(tmpBuf);
                m_imageTitle->setContentSize(Size(sp->getContentSize().width, sp->getContentSize().height));
            }
        }
        
        //背景图片
        m_imageListBack = static_cast<Sprite*>(rootNode->getChildByName("list_back"));
        CC_ASSERT(m_imageListBack != nullptr);
        
        //大厅用户信息
        m_infoLayout = static_cast<Layout*>(rootNode->getChildByName("info_panel"));
        CC_ASSERT(m_infoLayout != nullptr);
        
        //奖励按钮动画
        Director::getInstance()->getTextureCache()->addImageAsync("plazz/animate.png", [=](Texture2D *)
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plazz/animate.plist");
            readAnimation("%d.png", "plazzAnimate", 14, 0.2);
            auto pos = Vec2(160, 585);
            if ((DWORD)kind_default == HallDataMgr::getInstance()->m_dwKindID)
            {
                pos = Vec2(116, 585);
            }
            
            auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName("0.png");
            auto sprite = Sprite::createWithSpriteFrame(pframe);
            sprite->setAnchorPoint(Vec2(.5, .5));
            sprite->setPosition(pos);
            m_infoLayout->addChild(sprite,10);
            auto paction = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("plazzAnimate")));
            sprite->runAction(paction);
            
            auto signBtn = Button::create();
            signBtn->setScale9Enabled(true);
            signBtn->setContentSize(Size(100, 100));
            signBtn->setPosition(pos);
            signBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithReward, this));
            m_infoLayout->addChild(signBtn);
        });
        
        Button *headBtn = static_cast<Button *>(m_infoLayout->getChildByName("head_back_btn"));
        if(nullptr != headBtn)
        {            
            headBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithTouchUser, this));
        }
        
        //用户昵称
        _UserNikcName = Label::createWithSystemFont(HallDataMgr::getInstance()->m_pNickName, FONT_DEFAULT, 24);
        _UserNikcName->setAnchorPoint(Vec2(.0, .5));
        _UserNikcName->setDimensions(129, _UserNikcName->getContentSize().height + 1);
        _UserNikcName->setPosition(Vec2(200, 100));
        m_infoLayout->addChild(_UserNikcName);
        
        //用户分数
        _UserScore = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","), FONT_DEFAULT, 24);
        _UserScore->setTextColor(cocos2d::Color4B::YELLOW);
        Labellengthdeal(_UserScore, 135);
        _UserScore->setAnchorPoint(Vec2(.0, .5));
        _UserScore->setPosition(Vec2(180, 60));
        m_infoLayout->addChild(_UserScore);
        
        _UserLevel = Label::createWithCharMap("plazz/level_num.png", 10, 13, '0');
        _UserLevel->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
        _UserLevel->setPosition(Vec2(170, 100));
        m_infoLayout->addChild(_UserLevel);
        
        m_root->setScaleX(JudgeScale);
        this->addChild(m_root);
        
        //购买
        Button *purchaseBtn = static_cast<Button*>(m_infoLayout->getChildByName("btn_purchase"));
        if (nullptr != purchaseBtn)
        {
            purchaseBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithShop, this));
        }
        
        //银行
        Button *bank = static_cast<Button *>(m_infoLayout->getChildByName("bank_btn"));
        if (nullptr != bank)
        {
            bank->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithBank, this));
        }
        
        //任务
        Button *taskBtn = static_cast<Button*>(m_infoLayout->getChildByName("task_btn"));
        if (nullptr != taskBtn)
        {
            taskBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithTask, this));
        }
        
        //商城
        Button *shopBtn = static_cast<Button*>(m_infoLayout->getChildByName("shop_btn"));
        if (nullptr != shopBtn)
        {
            shopBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithShop, this));
        }
        
        //设置
        Button *setBtn = static_cast<Button*>(m_infoLayout->getChildByName("set_btn"));
        if (nullptr != setBtn)
        {
            setBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithSet, this));
        }
        
        //排行榜
        Button *rankBtn = static_cast<Button*>(m_infoLayout->getChildByName("rank_btn"));
        if (nullptr != rankBtn)
        {
            rankBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithRank, this));
        }
        
        //分享
        Button *shareBtn = static_cast<Button*>(m_infoLayout->getChildByName("share_btn"));
        if (nullptr != shareBtn)
        {
            shareBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithShare, this));
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
        Button *lockBtn = Button::create(file1,file2,file1);
        lockBtn->setName("bind_machine");
        lockBtn->setPosition(Vec2(1084, 590));
        lockBtn->setTag(nTag);
        lockBtn->addTouchEventListener(CC_CALLBACK_2(CommonPlazaLayer::buttonEventWithLock, this));
        m_infoLayout->addChild(lockBtn);
        
        bRes = true;
    } while (false);
    return bRes;
}

void CommonPlazaLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    //用户头像
    if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_NORMAL
        || HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_VISITOR)
    {
        _headSprite = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID,
                                                      HallDataMgr::getInstance()->m_wCustom,
                                                      HallDataMgr::getInstance()->m_dwUserID,
                                                      HallDataMgr::getInstance()->m_cbGender);
    }
    else if (HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_RENREN
             || HallDataMgr::getInstance()->m_loadtype == EM_LOAD_TYPE_SINA)
    {
        _headSprite = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl,
                                                   HallDataMgr::getInstance()->m_dwUserID);
    }
    if(nullptr != _headSprite)
    {
        _headSprite->setHeadSize(70.0);
        _headSprite->setPosition(Vec2(102, 80));
        m_infoLayout->addChild(_headSprite);
    }
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(CommonPlazaLayer::notifyFreshInfo, this)), 1);
    
    //公告
    Notice *notice = Notice::CreateNotice();
    notice->setAnchorPoint(Vec2(.5, .5));
    notice->setContentSize(Size(475, 40));
    notice->setPosition(Vec2(568, 630));
    m_root->addChild(notice);
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
        
        Personal *personal = Personal::create();
        personal->setTag(PERSONAL);
        personal->setPosition(Vec2(1136, 0));
        this->addChild(personal);
        
        personal->popPersonal();
    }
}


void CommonPlazaLayer::buttonEventWithBank(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(BANK))
            return;
        
        BankShow *bank = BankShow::create();
        bank->setTag(BANK);
        bank->setPosition(Vec2(1136, 0));
        this->addChild(bank);
        bank->popBank();
    }
    
}
void CommonPlazaLayer::buttonEventWithReward(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        Reward *reward = Reward::create();
        reward->setScale(0.5f);
        this->addChild(reward);
        
        Sequence *quence = Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.05), ScaleTo::create(0.2f, 1.0));
        reward->runAction(quence);
    }
}

void CommonPlazaLayer::buttonEventWithShop(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(SHOP))
            return;
        
        Shop *shop = Shop::create();
        shop->setTag(SHOP);
        shop->setPosition(Vec2(1136, 0));
        this->addChild(shop);
        
        shop->popShop();
    }
}

void CommonPlazaLayer::buttonEventWithTask(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(TASK))
            return;
        
        Task *task = Task::create();
        task->setTag(TASK);
        task->setPosition(Vec2(1136, 0));
        this->addChild(task);
        task->popTask();
    }
    
}
void CommonPlazaLayer::buttonEventWithSet(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        Setting *set = Setting::create();
        
        this->addChild(set);
    }
}
void CommonPlazaLayer::buttonEventWithRank(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        if (this->getChildByTag(RANK))
            return;
        
        Rank *rank = Rank::create();
        rank->setTag(RANK);
        rank->setPosition(Vec2(1136, 0));
        this->addChild(rank);
        rank->popRank();
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

//MARK:: 等级信息
void CommonPlazaLayer::LevelUpgrade(void* pData, WORD wSize)
{
    CMD_GP_GrowLevelUpgrade *level = (CMD_GP_GrowLevelUpgrade *)pData;
    HallDataMgr::getInstance()->m_UserScore = level->lCurrScore;
    HallDataMgr::getInstance()->m_Ingot = level->lCurrIngot;
    
    _UserScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","));
    
    std::string str = WHConverUnicodeToUtf8WithArray(level->szNotifyContent);
    HallDataMgr::getInstance()->AddpopLayer("", str, Type_Ensure);
}

//等级信息
void CommonPlazaLayer::LevelInfo(void* pData, WORD wSize)
{
    memcpy(&HallDataMgr::getInstance()->m_levelData, pData, sizeof(HallDataMgr::getInstance()->m_levelData));
    
    _UserLevel->setString(__String::createWithFormat("%d",HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString());
    
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

void CommonPlazaLayer::hideUserInfo(bool bHide)
{
    m_infoLayout->setVisible(!bHide);
    
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
        m_imageListBack->setSpriteFrame(frame);
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

void CommonPlazaLayer::operatesuccessResult(void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    auto btn = static_cast<Button*>(m_infoLayout->getChildByName("bind_machine"));
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
    if (nullptr != btn)
    {
        btn->loadTextureNormal(normalfile);
        btn->loadTexturePressed(prefile);
        btn->loadTextureDisabled(normalfile);
        btn->setTag(nTag);
    }
    
    auto presult = (CMD_GP_OperateSuccess *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
}

void CommonPlazaLayer::operatefailureResult(void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    if (nullptr == pData || 0 == wSize)
    {
        return;
    }
    auto presult = (CMD_GP_OperateFailure *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
}