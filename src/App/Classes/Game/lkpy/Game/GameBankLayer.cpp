//
//  GameBankLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/23.
//
//
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "GameBankLayer.h"
#include "HallDataMgr.h"
#include "lkpy_GameDataMgr.h"
#include "NetworkMgr.h"
#include "GameLayer.h"
#include "lkpy_Help.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
USING_LKPY_NAMESPACE;
enum ButtonTag
{
    Tag_BG = 0,
    Tag_Open_Layout,
    Tag_Open_Pass,
    Tag_Open_EnsurePass,
    Tag_BT_Open,
    
    Tag_Access_Layout,
    Tag_Access_Reminder,
    Tag_User_Score,
    Tag_Insure_Socre,
    Tag_Access_Num,
    Tag_Access_Pass,
    Tag_BT_Getout,
};

bool GameBankLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto viewsize = WinSize;
    auto pbgbutton = Button::create();
    pbgbutton->setScale9Enabled(true);
    pbgbutton->setContentSize(viewsize);
    pbgbutton->setPosition(viewsize/2);
    this->addChild(pbgbutton);
    
    auto pbg = Scale9Sprite::create(Rect(57,66,847,399),"login_res/register_bg.png");
    pbg->setPreferredSize(cocos2d::Size(740, 480));
    pbg->setPosition(viewsize/2);
    pbg->setTag(Tag_BG);
    this->addChild(pbg);
    
    auto pclose = Button::create("setting_res/bt_close_0.png", "setting_res/bt_close_1.png");
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParent();
        }});
    pclose->setPosition(Vec2(pbg->getContentSize().width-28, pbg->getContentSize().height-28));
    pclose->setScale(0.85f);
    pbg->addChild(pclose);
    
    auto ptitle = ImageView::create("bank_res/bank_title.png");
    ptitle->setPosition(Vec2(pbg->getContentSize().width/2, pbg->getContentSize().height-20));
    pbg->addChild(ptitle);
    
    if (HallDataMgr::getInstance()->m_cbInsureEnable) {
        this->accessinit();
    }
    else
    {
        this->openinit();
    }
    return true;
    
}

void GameBankLayer::openinit()
{
    auto playout = Layout::create();
    playout->setTag(Tag_Open_Layout);
    playout->setContentSize(cocos2d::Size(590,310));
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setPosition(Vec2(WinSize.width/2, WinSize.height-200));
    this->addChild(playout);
    
    auto reminder = ImageView::create("bank_res/im_reminder.png");
    reminder->setAnchorPoint(Vec2(1.f, 0.5f));
    reminder->setScaleX(JudgeScale);
    reminder->setPosition(Vec2(playout->getContentSize().width/2-85, playout->getContentSize().height+30));
    playout->addChild(reminder);

    
    auto reminder_txt = Text::create("请先设置银行密码，以便保护你的个人财产安全。", FontNormal, 28);
    reminder_txt->setColor(Color3B(253,233,101));
    reminder_txt->setPosition(Vec2(10, playout->getContentSize().height-20));
    reminder_txt->setAnchorPoint(Vec2(0.f, 0.5f));
    playout->addChild(reminder_txt);
    
    const char *titlestr[2] = {"设置银行密码：", "确认银行密码："};
    for (int index=0; index<2; ++index) {
        auto title = Text::create(titlestr[index], FontNormal, 32);
        title->setAnchorPoint(Vec2(0.f, 0.5f));
        title->setPosition(Vec2(10, playout->getContentSize().height-90-68*index));
        playout->addChild(title);
        
        auto edit = EditBox::create(cocos2d::Size(325, 47), "bank_res/edit_frame.png");
        edit->setAnchorPoint(Vec2(0.f, 0.5f));
        edit->setPosition(Vec2(240, playout->getContentSize().height-90-68*index));
        edit->setTag(Tag_Open_Pass+index);
        edit->setInputFlag(EditBox::InputFlag::PASSWORD);
        edit->setPlaceHolder("6-32位英文或数字");
        edit->setFontSize(28);
        playout->addChild(edit);
    }
    
    auto bt_open = Button::create("bank_res/bt_open_0.png", "bank_res/bt_open_1.png");
    bt_open->setTag(Tag_BT_Open);
    bt_open->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::touchEvent, this));
    bt_open->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-250));
    playout->addChild(bt_open);
}

void GameBankLayer::accessinit()
{
    this->sendInsureInfo();
    cocos2d::Size viewsize = WinSize;
    auto playout = Layout::create();
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setContentSize(cocos2d::Size(740,350));
    playout->setPosition(Vec2(viewsize.width/2+10, viewsize.height-160));
    playout->setTag(Tag_Access_Layout);
    this->addChild(playout);
    
    auto im_gold = ImageView::create("bank_res/im_gold.png");
    im_gold->setPosition(Vec2(100, playout->getContentSize().height-50));
    playout->addChild(im_gold);
    
    auto im_arrow = ImageView::create("bank_res/im_arrow.png");
    im_arrow->setPosition(Vec2(playout->getContentSize().width/2, im_gold->getPositionY()));
    playout->addChild(im_arrow);
    
    auto im_money = ImageView::create("bank_res/im_money.png");
    im_money->setPosition(Vec2(im_arrow->getPositionX()+108, im_gold->getPositionY()+10));
    playout->addChild(im_money);
    
    std::string scorestring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestring = getGoldStr(scorestring,',');
    if (scorestring.length()>16) {
        scorestring = scorestring.substr(0,14);
        scorestring.append("..");
    }
    
    auto userscore = Label::createWithSystemFont(scorestring, FontBold, 25);
    
    //userscore->setDimensions(150, 25);
    userscore->setHorizontalAlignment(TextHAlignment::CENTER);
    userscore->setPosition(Vec2(im_gold->getPositionX()+140, im_gold->getPositionY()));
    userscore->setColor(Color3B(253,233,101));
    userscore->setTag(Tag_User_Score);
    playout->addChild(userscore);
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser) {
        string scoreStr = __String::createWithFormat("%lld", puser->m_date.lScore)->getCString();
        scoreStr = getGoldStr(scoreStr,',');
        userscore->setString(scoreStr);
    }
    
    std::string insuerstring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserInsure)->getCString();
    insuerstring = getGoldStr(insuerstring,',');
    if (insuerstring.length()>16) {
        insuerstring = insuerstring.substr(0,14);
        insuerstring.append("..");
    }
    auto insuerscore = Label::createWithSystemFont(insuerstring, FontBold, 25);
    //insuerscore->setDimensions(150, 25);
    insuerscore->setHorizontalAlignment(TextHAlignment::CENTER);
    insuerscore->setPosition(Vec2(im_money->getPositionX()+130, im_gold->getPositionY()));
    insuerscore->setColor(Color3B(253,233,101));
    insuerscore->setTag(Tag_Insure_Socre);
    playout->addChild(insuerscore);
    
    auto reminderstr = __String::createWithFormat("温馨提示：存入游戏币免手续费，取出将扣除%d‰手续费",10)->getCString();
    auto reminder = Text::create(reminderstr, FontNormal, 20);
    reminder->setColor(WATHETBLUE);
    reminder->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-112));
    reminder->setTag(Tag_Access_Reminder);
    playout->addChild(reminder);
    
    const char *titlestr[2] = {"输入取出金额：", "输入银行密码："};
    for (int index=0; index<2; ++index) {
        auto title = Text::create(titlestr[index], FontNormal, 32);
        title->setColor(Color3B::WHITE);
        title->setAnchorPoint(Vec2(0.f, 0.5f));
        title->setPosition(Vec2(im_gold->getPositionX()+25, playout->getContentSize().height-162-58*index));
        playout->addChild(title);
        
        auto edit = EditBox::create(cocos2d::Size(325, 47), "bank_res/edit_frame.png");
        edit->setAnchorPoint(Vec2(0.f, 0.5f));
        edit->setPosition(Vec2(im_gold->getPositionX()+250, playout->getContentSize().height-162-58*index));
        edit->setTag(Tag_Access_Num+index);
        edit->setDelegate(this);
        edit->setMaxLength(LEN_ACCOUNTS-1);
        if (index == 0) {
            edit->setInputMode(EditBox::InputMode::NUMERIC);
            edit->setPlaceHolder("不低于5000");
        }
        if (index == 1) {
            edit->setInputFlag(EditBox::InputFlag::PASSWORD);
            edit->setPlaceHolder("6-32位英文或数字");
        }
        edit->setFontSize(28);
        playout->addChild(edit);
    }
    
    auto getout = Button::create("bank_res/bt_getout_0.png", "bank_res/bt_getout_1.png");
    getout->setTag(Tag_BT_Getout);
    getout->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::touchEvent, this));
    getout->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-300));
    playout->addChild(getout);
}

void GameBankLayer::updateScore()
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whEvent_User_Data_Change);
    auto accesslayout = this->getChildByTag(Tag_Access_Layout);
    if (accesslayout) {
        auto userscore = (Label *)accesslayout->getChildByTag(Tag_User_Score);
        auto insurescore = (Label *)accesslayout->getChildByTag(Tag_Insure_Socre);
        auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
        if (puser) {
            std::string scorestring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString();
            scorestring = getGoldStr(scorestring,',');
            if (scorestring.length()>16) {
                scorestring = scorestring.substr(0,14);
                scorestring.append("..");
            }
            userscore->setString(scorestring);
        }
        
        std::string insuerstring = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserInsure)->getCString();
        insuerstring = getGoldStr(insuerstring,',');
        if (insuerstring.length()>16) {
            insuerstring = insuerstring.substr(0,14);
            insuerstring.append("..");
        }
        insurescore->setString(insuerstring);
    }
}

void GameBankLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            switch (tag)
            {
                case Tag_BT_Open:
                {
                    auto playout = this->getChildByTag(Tag_Open_Layout);
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
                case Tag_BT_Getout:
                {
                    auto playout = this->getChildByTag(Tag_Access_Layout);
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
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
}

bool GameBankLayer::checkStrlength(const std::string &str, const std::string &title)
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

void GameBankLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_INSURE, CC_CALLBACK_3(GameBankLayer::BankInsureResult, this));
}

void GameBankLayer::onExit()
{
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_INSURE);
    Layer::onExit();
}

void GameBankLayer::sendInsureEnable(const std::string &pass)
{
    CMD_GR_C_EnableInsureRequest insuer;
    memset(&insuer, 0, sizeof(insuer));
    
    insuer.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), insuer.szLogonPass);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), insuer.szMachineID);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)pass.c_str(), insuer.szInsurePass);
    NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_ENABLE_INSURE_REQUEST, &insuer, sizeof(insuer));
}

void GameBankLayer::sendInsureInfo()
{
    CMD_GR_C_QueryInsureInfoRequest info;
    memset(&info,0,sizeof(info));
    
    info.cbActivityGame = SUB_GR_QUERY_INSURE_INFO;
    if(HallDataMgr::getInstance()->m_loadtype == Load_Normal)
    {
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), info.szInsurePass);
    }
    else
    {
        std::string pass = MD5Encrypt("888888");
        UTF8Str_To_UTF16Str_BYTE((BYTE *)pass.c_str(), info.szInsurePass);
    }
    NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_INSURE_INFO, &info, sizeof(info));
}

void GameBankLayer::sendTakeScore(SCORE score, const std::string &pass)
{
    CMD_GR_C_TakeScoreRequest request;
    memset(&request, 0, sizeof(request));
    
    request.cbAvtivityGame = SUB_GR_TAKE_SCORE_REQUEST;
    request.lTakeScore = score;
    auto md5pass = MD5Encrypt(pass);
    UTF8Str_To_UTF16Str_BYTE((BYTE *)md5pass.c_str(), request.szInsurePass);
    NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TAKE_SCORE_REQUEST, &request, sizeof(request));
}

void GameBankLayer::BankInsureResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_USER_INSURE_ENABLE_RESULT:
        {
            auto result = (CMD_GR_S_UserInsureEnableResult *)pData;
            if (result->cbInsureEnabled == 1) {
                HallDataMgr::getInstance()->m_cbInsureEnable = 1;
                this->removeChildByTag(Tag_Open_Layout);
                this->accessinit();
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
            
            HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
            HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
            
            auto accesslayout = this->getChildByTag(Tag_Access_Layout);
            if (!accesslayout) {
                return;
            }
            auto reminder = (Text *)accesslayout->getChildByTag(Tag_Access_Reminder);
            auto reminderstr = __String::createWithFormat("温馨提示：存入游戏币免手续费，取出将扣除%d‰手续费", result->wRevenueTake);
            reminder->setString(reminderstr->getCString());
        }
            break;
        case SUB_GR_USER_INSURE_SUCCESS:
        {
            auto result = (CMD_GR_S_UserInsureSuccess *)pData;
            auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
            auto addscore = result->lUserScore - HallDataMgr::getInstance()->m_UserScore;
            if (puser) {
                puser->m_date.lScore += addscore;
            }
            
            HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
            HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
            this->updateScore();
            
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
        default:
            break;
    }
}

void GameBankLayer::editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text)
{
    if (editBox->getTag() == Tag_Access_Num) {
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

