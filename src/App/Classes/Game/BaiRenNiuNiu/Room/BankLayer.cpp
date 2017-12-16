//
//  BankLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/11.
//
//

#include "BankLayer.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "Project.h"
#include "ModeLayer.h"
USING_NS_CC;
using namespace ui;
using namespace brnn_game;

enum ButtonTag
{
    Tag_Open_Layout = 0,
    Tag_Access_Layout,
    Tag_Presented_Layout,
    
    Tag_Reminder,
    
    Tag_Open_Pass,          //设置银行密码
    Tag_Open_EnsurePass,    //确认银行密码
    Tag_BT_Open,            //开通我的银行
    
    Tag_BT_Access,          //存取
    Tag_BT_Presented,       //赠送
    
    Tag_BT_Save,            //存入
    Tag_BT_Getout,          //取出
    Tag_Access_Num,         //金额输入框
    Tag_Access_Pass,        //密码输入框
    
    Tag_BT_ID,              //ID
    Tag_BT_Nickname,        //依据昵称
    Tag_BT_Ensure,          //确认赠送
    Tag_Presented_Nickname, //昵称输入框
    Tag_Presented_Num,      //金额输入框
    Tag_Presented_Pass,     //密码输入框
    
    Tag_User_Score,
    Tag_Insure_Socre,
    Tag_Access_Reminder,//存取界面提示内容
    
    Tag_Presented_inNum,    //赠送存款数
    Tag_Presented_CTypeStr, //赠送方式文字显示
};

bool BankLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_nSendType = 0;
    memset(&m_info,0,sizeof(m_info));
    cocos2d::Size viewsize = WinSize;
    
    auto pbg = Button::create("public_res/im_bg.png", "public_res/im_bg.png");
    pbg->setPosition(viewsize/2);
    this->addChild(pbg);
    
    m_frame = ImageView::create("public_res/im_frame.png");
    m_frame->setAnchorPoint(Vec2(0.5f, 1.f));
    m_frame->setPosition(Vec2(viewsize.width/2,viewsize.height+m_frame->getContentSize().height));
    m_frame->runAction(EaseSineOut::create(MoveBy::create(0.17f, Vec2(0.f, -m_frame->getContentSize().height))));
    this->addChild(m_frame);
    
    auto pframe0 = ImageView::create("public_res/im_frame_0.png");
    pframe0->setAnchorPoint(Vec2(0.5f, 1.f));
    pframe0->setPosition(Vec2(m_frame->getContentSize().width/2, m_frame->getContentSize().height-174));
    m_frame->addChild(pframe0);
    
    auto titlebg = ImageView::create("public_res/im_title_bg.png");
    titlebg->setPosition(Vec2(m_frame->getContentSize().width/2, m_frame->getContentSize().height-110));
    m_frame->addChild(titlebg);
    
    auto title = ImageView::create("bank_res/txt_title.png");
    title->setPosition(Vec2(titlebg->getContentSize().width/2, 32));
    titlebg->addChild(title);
    
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(60, viewsize.height-50));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            this->removeFromParent();
        }
    });
    this->addChild(preturn);
    
    if (HallDataMgr::getInstance()->m_cbInsureEnable) {
        this->typeinit();
    }
    else
    {
        this->openinit();
    }
    
    return true;
}

void BankLayer::typeinit()
{
    //存取
    m_save = Button::create("bank_res/bt_access_1.png", "bank_res/bt_access_0.png");
    m_save->setPosition(Vec2(182, m_frame->getContentSize().height-155));
    m_save->setTag(Tag_BT_Access);
    m_save->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    m_save->setTouchEnabled(false);
    m_frame->addChild(m_save);
    
    //赠送
    m_send = Button::create("bank_res/bt_presented_0.png", "bank_res/bt_presented_1.png");
    m_send->setPosition(Vec2(798, m_frame->getContentSize().height-155));
    m_send->setTag(Tag_BT_Presented);
    m_send->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    m_frame->addChild(m_send);
    
    this->accessinit();
}

void BankLayer::openinit()
{
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(890,367));
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setPosition(Vec2(m_frame->getContentSize().width/2, m_frame->getContentSize().height-174));
    playout->setTag(Tag_Open_Layout);
    m_frame->addChild(playout);
    
    //温馨提示
    auto reminder = ImageView::create("bank_res/im_reminder.png");
    reminder->setAnchorPoint(Vec2(0.f, 0.f));
    reminder->setTag(Tag_Reminder);
    reminder->setPosition(Vec2(25, playout->getContentSize().height-2));
    playout->addChild(reminder);
    
    auto reminder_txt = Text::create("请先设置银行密码，以便保护你的个人财产安全。", FontNormal, 28);
    reminder_txt->setColor(Color3B(237, 226, 118));
    reminder_txt->setPosition(Vec2(156, 302));
    reminder_txt->setAnchorPoint(Vec2(0.f, 0.5f));
    playout->addChild(reminder_txt);
    
    const char *titlestr[2] = {"设置银行密码：", "确认银行密码："};
    for (int index=0; index<2; ++index) {
        auto title = Text::create(titlestr[index], FontNormal, 32);
        title->setAnchorPoint(Vec2(0.f, 0.5f));
        title->setPosition(Vec2(156, 234-80*index));
        playout->addChild(title);
        
        auto edit = EditBox::create(cocos2d::Size(325, 50), "bank_res/edit_frame.png");
        edit->setAnchorPoint(Vec2(0.f, 0.5f));
        edit->setPosition(Vec2(385, title->getPositionY()));
        edit->setTag(Tag_Open_Pass+index);
        edit->setInputFlag(EditBox::InputFlag::PASSWORD);
        edit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        edit->setPlaceHolder("6-32位英文或数字");
        edit->setFontSize(28);
        playout->addChild(edit);
    }
    
    auto bt_open = Button::create("bank_res/bt_open_0.png", "bank_res/bt_open_1.png");
    bt_open->setTag(Tag_BT_Open);
    bt_open->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    bt_open->setPosition(Vec2(playout->getContentSize().width/2, 62));
    playout->addChild(bt_open);
}

void BankLayer::accessinit()
{
    auto presentedlayout = m_frame->getChildByTag(Tag_Presented_Layout);
    if (presentedlayout) {
        presentedlayout->setVisible(false);
    }
    
    auto accesslayout = m_frame->getChildByTag(Tag_Access_Layout);
    if (accesslayout) {
        accesslayout->setVisible(true);
        return;
    }
    
    this->sendInsureInfo();
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(890,367));
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setPosition(Vec2(m_frame->getContentSize().width/2, m_frame->getContentSize().height-174));
    playout->setTag(Tag_Access_Layout);
    m_frame->addChild(playout);
    
    auto pgold = ImageView::create("bank_res/im_gold.png");
    pgold->setPosition(Vec2(115, 300));
    playout->addChild(pgold);
    
    auto ptaketxt = Text::create("携带：", FontBold, 28);
    ptaketxt->setAnchorPoint(Vec2(0.f, 0.5f));
    ptaketxt->setColor(Color3B(237, 226, 118));
    ptaketxt->setPosition(Vec2(180, 288));
    playout->addChild(ptaketxt);
    
    std::string scorestring = getScorewithComma(HallDataMgr::getInstance()->m_UserScore);
    auto userscore = Label::createWithSystemFont(scorestring.c_str(), FontBold, 26);
    Labellengthdeal(userscore, 160.f);
    userscore->setHorizontalAlignment(TextHAlignment::LEFT);
    userscore->setAnchorPoint(Vec2(0.f, 0.5f));
    userscore->setPosition(Vec2(ptaketxt->getPositionX()+76, ptaketxt->getPositionY()));
    userscore->setColor(Color3B(237, 226, 118));
    userscore->setTag(Tag_User_Score);
    playout->addChild(userscore);
    
    auto parrow = ImageView::create("bank_res/im_arrow.png");
    parrow->setPosition(Vec2(466, 300));
    playout->addChild(parrow);
    
    auto pmoney = ImageView::create("bank_res/im_money.png");
    pmoney->setPosition(Vec2(582, 300));
    playout->addChild(pmoney);
    
    auto psavetxt = Text::create("存款：", FontBold, 28);
    psavetxt->setAnchorPoint(Vec2(0.f, 0.5f));
    psavetxt->setColor(Color3B(237, 226, 118));
    psavetxt->setPosition(Vec2(630, ptaketxt->getPositionY()));
    playout->addChild(psavetxt);
    
    std::string insuerstring = getScorewithComma(HallDataMgr::getInstance()->m_UserInsure);
    auto insuerscore = Label::createWithSystemFont(insuerstring.c_str(), FontBold, 26);
    insuerscore->setAnchorPoint(Vec2(0.f, 0.5f));
    insuerscore->setPosition(Vec2(psavetxt->getPositionX()+76, ptaketxt->getPositionY()));
    insuerscore->setColor(Color3B(237, 226, 118));
    insuerscore->setTag(Tag_Insure_Socre);
    Labellengthdeal(insuerscore, 160.f);
    playout->addChild(insuerscore);
    
    auto reminderstr = __String::createWithFormat("温馨提示：存入游戏币免手续费，取出将扣除%d‰手续费",m_info.wRevenueTake)->getCString();
    auto reminder = Text::create(reminderstr, FontNormal, 20);
    reminder->setColor(Color3B::RED);
    reminder->setPosition(Vec2(playout->getContentSize().width/2, 242));
    reminder->setTag(Tag_Access_Reminder);
    playout->addChild(reminder);
    
    const char *titlestr[2] = {"输入存取金额：", "输入银行密码："};
    for (int index=0; index<2; ++index) {
        auto title = Text::create(titlestr[index], FontBold, 32);
        title->setColor(Color3B::WHITE);
        title->setAnchorPoint(Vec2(0.f, 0.5f));
        title->setPosition(Vec2(ptaketxt->getPositionX(), 190-56*index));
        playout->addChild(title);
        
        auto edit = EditBox::create(cocos2d::Size(325, 50), "bank_res/edit_frame.png");
        edit->setAnchorPoint(Vec2(0.f, 0.5f));
        edit->setPosition(Vec2(title->getPositionX()+225, title->getPositionY()));
        edit->setTag(Tag_Access_Num+index);
        edit->setDelegate(this);
        edit->setMaxLength(LEN_ACCOUNTS-1);
        edit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        if (index == 0) {
            edit->setInputMode(EditBox::InputMode::NUMERIC);
            edit->setPlaceHolder("输入存取金额");
        }
        if (index == 1) {
            edit->setInputFlag(EditBox::InputFlag::PASSWORD);
            edit->setPlaceHolder("6-32位英文或数字");
        }
        edit->setFontSize(28);
        playout->addChild(edit);
    }
    
    auto save = Button::create("bank_res/bt_save_0.png", "bank_res/bt_save_1.png", "bank_res/bt_save_1.png");
    save->setTag(Tag_BT_Save);
    save->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    save->setPosition(Vec2(playout->getContentSize().width/2-180, 54));
    playout->addChild(save);
    
    auto getout = Button::create("bank_res/bt_getout_0.png", "bank_res/bt_getout_1.png");
    getout->setTag(Tag_BT_Getout);
    getout->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    getout->setPosition(Vec2(playout->getContentSize().width/2+180, 54));
    playout->addChild(getout);
}

void BankLayer::presentedinit()
{
    auto accesslayout = m_frame->getChildByTag(Tag_Access_Layout);
    if (accesslayout) {
        accesslayout->setVisible(false);
    }
    
    auto presentedlayout = m_frame->getChildByTag(Tag_Presented_Layout);
    if (presentedlayout) {
        presentedlayout->setVisible(true);
        return;
    }
    
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(890,367));
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setPosition(Vec2(m_frame->getContentSize().width/2, m_frame->getContentSize().height-174));
    playout->setTag(Tag_Presented_Layout);
    m_frame->addChild(playout);
    
    auto insurestr = Text::create("银行存款：", FontBold, 28);
    insurestr->setColor(Color3B(237, 226, 118));
    insurestr->setAnchorPoint(Vec2(0.f, 0.5f));
    insurestr->setPosition(Vec2(185, 332));
    playout->addChild(insurestr);
    
    auto insurescore = Text::create(getScorewithComma(HallDataMgr::getInstance()->m_UserInsure), FontBold, 28);
    insurescore->setColor(Color3B(237, 226, 118));
    insurescore->setAnchorPoint(Vec2(0.f, 0.5f));
    insurescore->setPosition(Vec2(insurestr->getPositionX()+140, insurestr->getPositionY()));
    insurescore->setTag(Tag_Presented_inNum);
    playout->addChild(insurescore);
    
    auto ID = Button::create();
    auto Nickname = Button::create();
    if (m_nSendType == 0) {
        ID->loadTextures("bank_res/bt_ID_yes.png",  "bank_res/bt_ID_no.png");
        Nickname->loadTextures("bank_res/bt_nickname_no.png", "bank_res/bt_nickname_yes.png");
    }
    else{
        ID->loadTextures("bank_res/bt_ID_no.png",  "bank_res/bt_ID_yes.png");
        Nickname->loadTextures("bank_res/bt_nickname_yes.png", "bank_res/bt_nickname_no.png");
    }
    ID->setTag(Tag_BT_ID);
    ID->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    ID->setPosition(Vec2(insurestr->getPositionX()+140, 288));
    ID->setAnchorPoint(Vec2(0.f, 0.5f));
    playout->addChild(ID);
    
    Nickname->setTag(Tag_BT_Nickname);
    Nickname->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    Nickname->setPosition(Vec2(insurestr->getPositionX()+328, ID->getPositionY()));
    Nickname->setAnchorPoint(Vec2(0.f, 0.5f));
    playout->addChild(Nickname);
    
    const char *titlestr[3] = {"接收ID：", "赠送金额：", "银行密码："};
    for (int index=0; index<3; ++index) {
        auto title = Text::create(titlestr[index], FontBold, 28);
        title->setColor(Color3B::WHITE);
        title->setAnchorPoint(Vec2(0.f, 0.5f));
        title->setPosition(Vec2(insurestr->getPositionX(), 230-50*index));
        if (index == 0) {
            title->setTag(Tag_Presented_CTypeStr);
        }
        playout->addChild(title);
        
        auto edit = EditBox::create(cocos2d::Size(340, 50), "bank_res/edit_frame.png");
        edit->setAnchorPoint(Vec2(0.f, 0.5f));
        edit->setPosition(Vec2(insurestr->getPositionX()+140, title->getPositionY()));
        edit->setTag(Tag_Presented_Nickname+index);
        edit->setDelegate(this);
        edit->setMaxLength(LEN_ACCOUNTS-1);
        if (index == 1 || index == 0) {
            edit->setInputMode(EditBox::InputMode::NUMERIC);
            if (index == 1) {
                edit->setPlaceHolder("请输入赠送金额");
            }
            else
            {
                edit->setPlaceHolder("请输入赠送ID或昵称");
            }
        }
        if (index == 2) {
            edit->setInputFlag(EditBox::InputFlag::PASSWORD);
            edit->setPlaceHolder("6-32位英文或数字");
        }
        edit->setFontSize(28);
        playout->addChild(edit);
    }
    
    auto ensurebutton = Button::create("bank_res/bt_ensure_0.png", "bank_res/bt_ensure_1.png");
    ensurebutton->setTag(Tag_BT_Ensure);
    ensurebutton->setPosition(Vec2(playout->getContentSize().width/2, 48));
    ensurebutton->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    playout->addChild(ensurebutton);
}

 BankLayer * BankLayer::createWithGame()
{
    auto player = new BankLayer();
    if (player->bankgameinit()) {
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool BankLayer::bankgameinit()
{
    if (!Layer::init()) {
        return false;
    }
    m_nSendType = 0;
    memset(&m_info,0,sizeof(m_info));
    cocos2d::Size viewsize = WinSize;
    auto pbuttonbg = Button::create();
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(viewsize);
    pbuttonbg->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParent();
        }
    });
    pbuttonbg->setPosition(viewsize/2);
    this->addChild(pbuttonbg);
    pbuttonbg->addChild(LayerColor::create(Color4B(0, 0, 0, 110)));
    
    m_frame= Button::create("bank_res/im_game_frame.png", "bank_res/im_game_frame.png");
    m_frame->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    this->addChild(m_frame);
    
//    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
//    pclose->setPosition(Vec2(m_frame->getContentSize().width-50, m_frame->getContentSize().height-50));
//    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
//                                    {
//                                        if (type == Widget::TouchEventType::ENDED) {
//                                            this->removeFromParent();
//                                        }
//                                    });
//    m_frame->addChild(pclose);
    
    if (HallDataMgr::getInstance()->m_cbInsureEnable) {
        this->gameaccessinit();
    }
    else
    {
        this->gameopeninit();
    }
    return true;
}

void BankLayer::gameopeninit()
{
    this->openinit();
    auto playout = static_cast<Layout *>(m_frame->getChildByTag(Tag_Open_Layout));
    if (playout) {
        playout->setPositionY(m_frame->getContentSize().height-5);
        playout->removeChildByTag(Tag_Reminder);
    }
}

void BankLayer::gameaccessinit()
{
    this->sendInsureInfo();
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(657,374));
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setPosition(Vec2(m_frame->getContentSize().width/2, m_frame->getContentSize().height));
    playout->setTag(Tag_Access_Layout);
    m_frame->addChild(playout);
    
    auto ptaketxt = Text::create("携带：", FontBold, 28);
    ptaketxt->setAnchorPoint(Vec2(0.f, 0.5f));
    ptaketxt->setColor(Color3B(237, 226, 118));
    ptaketxt->setPosition(Vec2(46, 306));
    playout->addChild(ptaketxt);
    
    std::string scorestring = getScorewithComma(HallDataMgr::getInstance()->m_UserScore);
    auto userscore = Label::createWithSystemFont(scorestring.c_str(), FontBold, 26);
    Labellengthdeal(userscore, 160.f);
    userscore->setHorizontalAlignment(TextHAlignment::LEFT);
    userscore->setAnchorPoint(Vec2(0.f, 0.5f));
    userscore->setPosition(Vec2(ptaketxt->getPositionX()+76, ptaketxt->getPositionY()));
    userscore->setColor(Color3B(237, 226, 118));
    userscore->setTag(Tag_User_Score);
    playout->addChild(userscore);
    
    auto psavetxt = Text::create("存款：", FontBold, 28);
    psavetxt->setAnchorPoint(Vec2(0.f, 0.5f));
    psavetxt->setColor(Color3B(237, 226, 118));
    psavetxt->setPosition(Vec2(370, ptaketxt->getPositionY()));
    playout->addChild(psavetxt);
    
    std::string insuerstring = getScorewithComma(HallDataMgr::getInstance()->m_UserInsure);
    auto insuerscore = Label::createWithSystemFont(insuerstring.c_str(), FontBold, 26);
    insuerscore->setAnchorPoint(Vec2(0.f, 0.5f));
    insuerscore->setPosition(Vec2(psavetxt->getPositionX()+76, ptaketxt->getPositionY()));
    insuerscore->setColor(Color3B(237, 226, 118));
    insuerscore->setTag(Tag_Insure_Socre);
    Labellengthdeal(insuerscore, 160.f);
    playout->addChild(insuerscore);
    
    auto reminderstr = __String::createWithFormat("温馨提示：存入游戏币免手续费，取出将扣除%d‰手续费",m_info.wRevenueTake)->getCString();
    auto reminder = Text::create(reminderstr, FontNormal, 20);
    reminder->setColor(Color3B::GREEN);
    reminder->setAnchorPoint(Vec2(0.f, 0.5f));
    reminder->setPosition(Vec2(ptaketxt->getPositionX(), 258));
    reminder->setTag(Tag_Access_Reminder);
    playout->addChild(reminder);
    
    const char *titlestr[2] = {"输入存取金额：", "输入银行密码："};
    for (int index=0; index<2; ++index) {
        auto title = Text::create(titlestr[index], FontBold, 32);
        title->setColor(Color3B::WHITE);
        title->setAnchorPoint(Vec2(0.f, 0.5f));
        title->setPosition(Vec2(ptaketxt->getPositionX(), 210-62*index));
        playout->addChild(title);
        
        auto edit = EditBox::create(cocos2d::Size(325, 50), "bank_res/edit_frame.png");
        edit->setAnchorPoint(Vec2(0.f, 0.5f));
        edit->setPosition(Vec2(title->getPositionX()+225, title->getPositionY()));
        edit->setTag(Tag_Access_Num+index);
        edit->setDelegate(this);
        edit->setMaxLength(LEN_ACCOUNTS-1);
        edit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        if (index == 0) {
            edit->setInputMode(EditBox::InputMode::NUMERIC);
            edit->setPlaceHolder("输入存取金额");
        }
        if (index == 1) {
            edit->setInputFlag(EditBox::InputFlag::PASSWORD);
            edit->setPlaceHolder("6-32位英文或数字");
        }
        edit->setFontSize(28);
        playout->addChild(edit);
    }
    
    auto save = Button::create("bank_res/bt_game_save_0.png", "bank_res/bt_game_save_1.png", "bank_res/bt_game_save_1.png");
    save->setTag(Tag_BT_Save);
    save->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    save->setPosition(Vec2(playout->getContentSize().width/2-175, 68));
    playout->addChild(save);
    
    auto getout = Button::create("bank_res/bt_game_getout_0.png", "bank_res/bt_game_getout_1.png");
    getout->setTag(Tag_BT_Getout);
    getout->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
    getout->setPosition(Vec2(playout->getContentSize().width/2+175, 68));
    playout->addChild(getout);
}

void BankLayer::updategamesave(bool enable)
{
    auto playout = static_cast<Layout *>(m_frame->getChildByTag(Tag_Access_Layout));
    if (playout) {
        auto psave = static_cast<Button *>(playout->getChildByTag(Tag_BT_Save));
        if (psave) {
            psave->setTouchEnabled(enable);
            psave->setBright(enable);
        }
    }
}

void BankLayer::updateScore()
{
    auto accesslayout = m_frame->getChildByTag(Tag_Access_Layout);
    if (accesslayout) {
        auto userscore = (Label *)accesslayout->getChildByTag(Tag_User_Score);
        auto insurescore = (Label *)accesslayout->getChildByTag(Tag_Insure_Socre);
        
        std::string scorestring = getScorewithComma(HallDataMgr::getInstance()->m_UserScore);
        std::string insuerstring = getScorewithComma(HallDataMgr::getInstance()->m_UserInsure);

        userscore->setString(scorestring);
        insurescore->setString(insuerstring);
        Labellengthdeal(userscore, 160.f);
        Labellengthdeal(userscore, 160.f);
    }
    
    auto presentedlayout = m_frame->getChildByTag(Tag_Presented_Layout);
    if (presentedlayout) {
        auto insurescore = (Text *)presentedlayout->getChildByTag(Tag_Presented_inNum);
        insurescore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserInsure));
    }
}

void BankLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_ENABLE_RESULT, CC_CALLBACK_2(BankLayer::InsureEnableResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_INFO, CC_CALLBACK_2(BankLayer::InsureInfoResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_SUCCESS, CC_CALLBACK_2(BankLayer::BankSuccedResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_FAILURE, CC_CALLBACK_2(BankLayer::BankFailureResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_QUERY_USER_INFO_RESULT, CC_CALLBACK_2(BankLayer::BankUserInfoResult, this));
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        NetworkMgr::getInstance()->registerroomfunction(MDM_GR_INSURE, CC_CALLBACK_3(BankLayer::BankInsureResult, this));
    }
}

void BankLayer::onExit()
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_USER_INSURE_ENABLE_RESULT);
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_USER_INSURE_INFO);
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_USER_INSURE_SUCCESS);
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_USER_INSURE_FAILURE);
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_QUERY_USER_INFO_RESULT);
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_INSURE);
    }
    Layer::onExit();
}

void BankLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            switch (tag) {
                case Tag_BT_Open:
                {
                    auto playout = m_frame->getChildByTag(Tag_Open_Layout);
                    if (!playout) {
                        return;
                    }
                    auto passedit = (EditBox *)playout->getChildByTag(Tag_Open_Pass);
                    auto ensurepassedit = (EditBox *)playout->getChildByTag(Tag_Open_EnsurePass);
                    std::string passstr = passedit->getText();
                    std::string ensurepassstr = ensurepassedit->getText();
                    if (this->checkStrlength(passstr, "密码") && this->checkStrlength(ensurepassstr, "确认密码"))
                    {
                        if (passstr != ensurepassstr) {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "两次输入的密码不一致", Type_Ensure);
                            return;
                        }
                        this->sendInsureEnable(passstr);
                    }
                }
                    break;
                case Tag_BT_Access:
                {
                    m_save->setTouchEnabled(false);
                    m_save->loadTextures("bank_res/bt_access_1.png", "bank_res/bt_access_0.png");
                    
                    m_send->loadTextures("bank_res/bt_presented_0.png", "bank_res/bt_presented_1.png");
                    m_send->setTouchEnabled(true);
                    this->accessinit();
                }
                    break;
                case Tag_BT_Presented:
                {
                    m_send->setTouchEnabled(false);
                    m_send->loadTextures("bank_res/bt_presented_1.png", "bank_res/bt_presented_0.png");
                    
                    m_save->loadTextures("bank_res/bt_access_0.png", "bank_res/bt_access_1.png");
                    m_save->setTouchEnabled(true);
                    this->presentedinit();
                }
                    break;
                case Tag_BT_Save:
                {
                    auto playout = m_frame->getChildByTag(Tag_Access_Layout);
                    auto numedit = (EditBox *)playout->getChildByTag(Tag_Access_Num);
                    std::string numstr = numedit->getText();
                    if (!numstr.size()) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "请输入存入金额", Type_Ensure);
                        return;
                    }
                    SCORE score;
                    sscanf(numedit->getText(), "%lld", &score);
                    if (score == 0) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "存款不能为0，请重新输入", Type_Ensure);
                        return;
                    }
                    if (score > HallDataMgr::getInstance()->m_UserScore) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "存款不能大于你的金币值", Type_Ensure);
                        return;
                    }
                    this->sendSaveScore(score);
                }
                    break;
                case Tag_BT_Getout:
                {
                    auto playout = m_frame->getChildByTag(Tag_Access_Layout);
                    auto numedit = (EditBox *)playout->getChildByTag(Tag_Access_Num);
                    auto passedit = (EditBox *)playout->getChildByTag(Tag_Access_Pass);
                    std::string numstr = numedit->getText();
                    if (!numstr.size()) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "请输入取出金额", Type_Ensure);
                        return;
                    }
                    SCORE score;
                    sscanf(numedit->getText(), "%lld", &score);
                    if (score == 0) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "取款不能为0，请重新输入", Type_Ensure);
                        return;
                    }
                    if (score > HallDataMgr::getInstance()->m_UserInsure) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "取款不能大于你的银行金币值", Type_Ensure);
                        return;
                    }
                    if (this->checkStrlength(passedit->getText(), "银行密码")) {
                        this->sendTakeScore(score, passedit->getText());
                    }
                }
                    break;
                case Tag_BT_ID:
                {
                    if (m_nSendType == 0) {
                        return;
                    }
                    m_nSendType = 0;
                    auto playout = m_frame->getChildByTag(Tag_Presented_Layout);
                    auto nickname = (Button *)playout->getChildByTag(Tag_BT_Nickname);
                    pbutton->loadTextures("bank_res/bt_ID_yes.png",  "bank_res/bt_ID_no.png");
                    nickname->loadTextures("bank_res/bt_nickname_no.png", "bank_res/bt_nickname_yes.png");
                    
                    auto text = (Text *)playout->getChildByTag(Tag_Presented_CTypeStr);
                    text->setString("接收ID：");
                    auto edit = (EditBox *)playout->getChildByTag(Tag_Presented_Nickname);
                    edit->setInputMode(EditBox::InputMode::NUMERIC);
                    edit->setText("");
                }
                    break;
                case Tag_BT_Nickname:
                {
                    if (m_nSendType == 1) {
                        return;
                    }
                    m_nSendType = 1;
                    auto playout = m_frame->getChildByTag(Tag_Presented_Layout);
                    auto ID = (Button *)playout->getChildByTag(Tag_BT_ID);
                    ID->loadTextures("bank_res/bt_ID_no.png",  "bank_res/bt_ID_yes.png");
                    pbutton->loadTextures("bank_res/bt_nickname_yes.png", "bank_res/bt_nickname_no.png");
                    
                    auto text = (Text *)playout->getChildByTag(Tag_Presented_CTypeStr);
                    text->setString("接收昵称：");
                    auto edit = (EditBox *)playout->getChildByTag(Tag_Presented_Nickname);
                    edit->setInputMode(EditBox::InputMode::ANY);
                    edit->setText("");
                }
                    break;
                case Tag_BT_Ensure:
                {
                    auto playout = m_frame->getChildByTag(Tag_Presented_Layout);
                    auto nickedit = (EditBox *)playout->getChildByTag(Tag_Presented_Nickname);
                    auto numedit = (EditBox *)playout->getChildByTag(Tag_Presented_Num);
                    auto passedit = (EditBox *)playout->getChildByTag(Tag_Presented_Pass);
                    std::string nickname = nickedit->getText();
                    if (!nickname.size()) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "赠送玩家不能为空", Type_Ensure);
                        return;
                    }
                    if (strLength(nickname) > LEN_ACCOUNTS-1) {
                        auto title = m_nSendType?"输入昵称过长，请重新输入":"输入ID过长，请重新输入";
                        HallDataMgr::getInstance()->AddpopLayer("提示", title, Type_Ensure);
                        return;
                    }
                    
                    std::string scorestr = numedit->getText();
                    if (!scorestr.size()) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "请输入赠送金额", Type_Ensure);
                        return;
                    }
                    
                    if (this->checkStrlength(passedit->getText(), "银行密码")) {
                        SCORE score;
                        sscanf(scorestr.c_str(), "%lld", &score);
                        this->sendTransferScore(score, passedit->getText(), m_nSendType, nickname);
                    }
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

bool BankLayer::checkStrlength(const std::string &str, const std::string &title)
{
    int length = strLength(str);
    if (length == 0) {
        auto reminderstr = __String::createWithFormat("%s不能为空，请重新输入",title.c_str())->getCString();
        HallDataMgr::getInstance()->AddpopLayer("提示", reminderstr, Type_Ensure);
        return false;
    }
    else if (length<6)
    {
        auto reminderstr = __String::createWithFormat("%s长度小于6位，请重新输入",title.c_str())->getCString();
        HallDataMgr::getInstance()->AddpopLayer("提示", reminderstr, Type_Ensure);
        return  false;
    }
    else if (length > LEN_PASSWORD)
    {
        auto reminderstr = __String::createWithFormat("%s长度超过32位，请重新输入",title.c_str())->getCString();
        HallDataMgr::getInstance()->AddpopLayer("提示", reminderstr, Type_Ensure);
        return false;
    }
    
    if (str.find(" ") != std::string::npos) {
        auto reminderstr = __String::createWithFormat("%s中不能有空格，请重新输入",title.c_str())->getCString();
        HallDataMgr::getInstance()->AddpopLayer("提示", reminderstr, Type_Ensure);
        return false;
    }
    return true;
}

void BankLayer::sendInsureEnable(const std::string &pass)
{
    std::string bankpass = MD5Encrypt(pass);
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        CMD_GP_UserEnableInsure insuer;
        memset(&insuer, 0, sizeof(insuer));
        
        
        insuer.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, insuer.szLogonPass);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, insuer.szMachineID);
        UTF8Str_To_UTF16Str(bankpass, insuer.szInsurePass);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_ENABLE_INSURE, &insuer, sizeof(insuer),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_C_EnableInsureRequest insuer;
        memset(&insuer, 0, sizeof(insuer));
        
        insuer.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, insuer.szLogonPass);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, insuer.szMachineID);
        UTF8Str_To_UTF16Str(bankpass, insuer.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_ENABLE_INSURE_REQUEST, &insuer, sizeof(insuer));
    }
}

void BankLayer::sendInsureInfo()
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        CMD_GP_QueryInsureInfo info;
        memset(&info, 0, sizeof(info));
        
        info.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, info.szPassword);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INSURE_INFO, &info, sizeof(info),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_C_QueryInsureInfoRequest info;
        memset(&info,0,sizeof(info));
        
        info.cbActivityGame = SUB_GR_QUERY_INSURE_INFO;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, info.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_INSURE_INFO, &info, sizeof(info));
    }
}

void BankLayer::sendSaveScore(SCORE score)
{
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserSaveScore request;
        memset(&request, 0, sizeof(request));
        
        request.lSaveScore = score;
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_C_SaveScoreRequest request;
        memset(&request, 0, sizeof(request));
        
        request.cbActivityGame = SUB_GR_SAVE_SCORE_REQUEST;
        request.lSaveScore = score;
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_SAVE_SCORE_REQUEST, &request, sizeof(request));
    }
}

void BankLayer::sendTakeScore(SCORE score, const std::string &pass)
{
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserTakeScore request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTakeScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str(md5pass, request.szPassword);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_C_TakeScoreRequest request;
        memset(&request, 0, sizeof(request));
        
        request.cbAvtivityGame = SUB_GR_TAKE_SCORE_REQUEST;
        request.lTakeScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str(md5pass, request.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TAKE_SCORE_REQUEST, &request, sizeof(request));
    }
}

void BankLayer::sendTransferScore(SCORE score, const std::string &pass, int type, const std::string &nickname)
{
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        if (type == 0) {
            CMD_GP_QueryUserInfoRequest request;
            memset(&request, 0, sizeof(request));
            
            request.cbByNickName = type;
            UTF8Str_To_UTF16Str(nickname, request.szAccounts);
            NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_USER_INFO_REQUEST, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
            m_TransferScore = score;
            m_BankPass = pass;
            return;
        }
        CMD_GP_UserTransferScore request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTransferScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str(md5pass, request.szPassword);
        UTF8Str_To_UTF16Str(nickname, request.szAccounts);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TRANSFER_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        if (type == 0) {
            CMD_GR_C_QueryUserInfoRequest request;
            memset(&request, 0, sizeof(request));
            
            request.cbByNickName = type;
            UTF8Str_To_UTF16Str(nickname, request.szAccounts);
            NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_USER_INFO_REQUEST, &request, sizeof(request));
            m_TransferScore = score;
            m_BankPass = pass;
            return;
        }
        
        CMD_GR_C_TransferScoreRequest request;
        memset(&request, 0, sizeof(request));
        
        request.cbActivityGame = SUB_GR_TRANSFER_SCORE_REQUEST;
        request.lTransferScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str(md5pass, request.szInsurePass);
        UTF8Str_To_UTF16Str(nickname, request.szAccounts);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TRANSFER_SCORE_REQUEST, &request, sizeof(request));
    }
}

void BankLayer::InsureEnableResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureEnableResult *)pData;
    if (result->cbInsureEnabled == 1) {
        HallDataMgr::getInstance()->m_cbInsureEnable = 1;
        m_frame->removeChildByTag(Tag_Open_Layout);
        
        this->typeinit();
        this->accessinit();
    }
    else
    {
        auto title = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
        HallDataMgr::getInstance()->AddpopLayer("系统提示", title, Type_Ensure);
        
        auto action = CallFunc::create([]{
            NetworkMgr::getInstance()->Disconnect(Data_Load);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    }
}

void BankLayer::InsureInfoResult(void *pData, WORD wSize)
{
    if (wSize != sizeof(CMD_GP_UserInsureInfo)) {
        return;
    }
    
    memset(&m_info, 0, sizeof(m_info));
    memcpy(&m_info, pData, wSize);
    HallDataMgr::getInstance()->m_UserScore = m_info.lUserScore;
    HallDataMgr::getInstance()->m_UserInsure = m_info.lUserInsure;
    
    auto action = CallFunc::create([]{
        NetworkMgr::getInstance()->Disconnect(Data_Load);});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    
    auto accesslayout = m_frame->getChildByTag(Tag_Access_Layout);
    if (!accesslayout) {
        return;
    }
    this->updateScore();
    auto reminder = (Text *)accesslayout->getChildByTag(Tag_Access_Reminder);
    auto reminderstr = __String::createWithFormat("温馨提示：存入游戏币免手续费，取出将扣除%d‰手续费", m_info.wRevenueTake);
    reminder->setString(reminderstr->getCString());
}

void BankLayer::BankSuccedResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureSuccess *)pData;
    HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
    HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
    this->updateScore();
    
    auto str = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("操作成功", str , Type_Ensure);
    
    //金币更新
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    auto action = CallFunc::create([]{
        NetworkMgr::getInstance()->Disconnect(Data_Load);});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

void BankLayer::BankFailureResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureFailure *)pData;
    auto str = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("操作失败", str , Type_Ensure);
    
    auto action = CallFunc::create([]{
        NetworkMgr::getInstance()->Disconnect(Data_Load);});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

void BankLayer::BankUserInfoResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserTransferUserInfo *)pData;
    std::string nickname = WHConverUnicodeToUtf8WithArray(result->szAccounts);
    std::string showname = "“" + nickname;
    showname += "”";
    std::string tipstr = __String::createWithFormat("你确定要向%s赠送游戏币%lld", showname.c_str(), m_TransferScore)->getCString();
    auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("", tipstr, Type_Ensure_Cancel));
    player->setEnsureCallback([=]{
        this->sendTransferScore(m_TransferScore, m_BankPass, 1, nickname);
    });
}

void BankLayer::BankInsureResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_USER_INSURE_ENABLE_RESULT:
        {
            auto result = (CMD_GR_S_UserInsureEnableResult *)pData;
            if (result->cbInsureEnabled == 1) {
                HallDataMgr::getInstance()->m_cbInsureEnable = 1;
                m_frame->removeChildByTag(Tag_Open_Layout);
                
                this->gameaccessinit();
            }
            else
            {
                auto title = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
                HallDataMgr::getInstance()->AddpopLayer("系统提示", title, Type_Ensure);
            }
        }
            break;
        case SUB_GR_USER_INSURE_INFO:
        {
            if (wSize != sizeof(CMD_GR_S_UserInsureInfo)) {
                return;
            }
            
            auto result = (CMD_GR_S_UserInsureInfo *)pData;
            memset(&m_info, 0, sizeof(m_info));
            m_info.lUserInsure = result->lUserInsure;
            m_info.lUserScore = result->lUserScore;
            m_info.wRevenueTake = result->wRevenueTake;
            m_info.wRevenueTransfer = result->wRevenueTransfer;
            m_info.wRevenueTransferMember = result->wRevenueTransferMember;
            m_info.wServerID = result->wServerID;
            m_info.lTransferPrerequisite = result->lTransferPrerequisite;
            HallDataMgr::getInstance()->m_UserScore = m_info.lUserScore;
            HallDataMgr::getInstance()->m_UserInsure = m_info.lUserInsure;
            
            this->updateScore();
            auto accesslayout = m_frame->getChildByTag(Tag_Access_Layout);
            if (!accesslayout) {
                return;
            }
            auto reminder = (Text *)accesslayout->getChildByTag(Tag_Access_Reminder);
            auto reminderstr = __String::createWithFormat("温馨提示：存入游戏币免手续费，取出将扣除%d‰手续费", m_info.wRevenueTake);
            reminder->setString(reminderstr->getCString());
        }
            break;
        case SUB_GR_USER_INSURE_SUCCESS:
        {
            auto result = (CMD_GR_S_UserInsureSuccess *)pData;
            HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
            HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
            this->updateScore();
            
            //金币更新
            EventCustom event(whEvent_User_Data_Change);
            auto value = __Integer::create(User_Change_Score);
            event.setUserData(value);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            
            auto str = WHConverUnicodeToUtf8WithArray(result->szDescribrString);
            HallDataMgr::getInstance()->AddpopLayer("操作成功", str , Type_Ensure);
        }
            break;
        case SUB_GR_USER_INSURE_FAILURE:
        {
            auto result = (CMD_GR_S_UserInsureFailure *)pData;
            auto str = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("操作失败", str , Type_Ensure);
        }
            break;
        case SUB_GR_USER_TRANSFER_USER_INFO:
        {
            auto result = (CMD_GR_S_UserTransferUserInfo *)pData;
            std::string nickname = WHConverUnicodeToUtf8WithArray(result->szAccounts);
            std::string showname = "“" + nickname;
            showname += "”";
            std::string tipstr = __String::createWithFormat("你确定要向%s赠送游戏币%lld", showname.c_str(), m_TransferScore)->getCString();
            auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("", tipstr, Type_Ensure_Cancel));
            player->setEnsureCallback([=]{
                this->sendTransferScore(m_TransferScore, m_BankPass, 1, nickname);
            });
        }
            break;
            
        default:
            break;
    }
}

void BankLayer::editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text)
{
    if (editBox->getTag() == Tag_Access_Num) {
        auto score = MAX(HallDataMgr::getInstance()->m_UserInsure, HallDataMgr::getInstance()->m_UserScore);
        SCORE editscore;
        if (!text.size()) {
            editscore = 0;
        }
        else
        {
            if (text == ".") {
                editscore = 0;
            }
            else
            {
                sscanf(text.c_str(), "%lld", &editscore);

            }
            std::string addstr = &text[text.length()-1];
            if (addstr == ".") {
                editBox->setText(text.substr(0,text.length()-1).c_str());
            }
        }
        if (editscore > score) {
            editBox->setText(__String::createWithFormat("%lld", score)->getCString());
        }
    }
    
    if (editBox->getTag() == Tag_Presented_Num) {
        auto score = HallDataMgr::getInstance()->m_UserInsure;
        SCORE editscore;
        if (!text.size()) {
            editscore = 0;
        }
        else
        {
            if (text == ".") {
                editscore = 0;
            }
            else
            {
                sscanf(text.c_str(), "%lld", &editscore);
                
            }
            std::string addstr = &text[text.length()-1];
            if (addstr == ".") {
                editBox->setText(text.substr(0,text.length()-1).c_str());
            }
        }
        if (editscore > score) {
            editBox->setText(__String::createWithFormat("%lld", score)->getCString());
        }
    }
}

