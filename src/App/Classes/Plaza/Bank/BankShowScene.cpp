/************************************************************************************
 * file: 		BankShow.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:16:30
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "BankShowScene.h"
#include "BankSendReceiptLayer.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/DictionaryHelper.h"
#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../../Scene/ModeLayer.h"

const std::string  RECORD_REQUEST_EVENT = "record_request_event";

using namespace cocos2d::network;
using namespace cocos2d::ui;
using namespace cocostudio;

typedef enum
{
    BANK_TAG_MAINLAYOUT = 0,
    BANK_TAG_ROOTNODE   = 1,
    BANK_TAG_SUBNODE    = 5,
	BANK_TAG_USERSCORE	= 6,
	BANK_TAG_USERINSURE = 7,
	BANK_TAG_CLOSE		= 10,
	BANK_TAG_SAVE		= 11,
	BANK_TAG_TAKE		= 12,
	BANK_TAG_APPLY		= 13,
	BANK_TAG_SAVEBTN	= 100,
	BANK_TAG_PRESENT	= 101,
	BANK_TAG_RECORD		= 102,
	BANK_TAG_TOP		= 103,
    BANK_TAG_RECORD_LISET = 200,
}BANK_TAG;

BankShowScene::BankShowScene():
_layout(nullptr),
_applyLayout(nullptr),
_saveLayout(nullptr),
_presentLayout(nullptr),
_recordLayout(nullptr),
m_nSendType(1),
m_eType(type_savetake),
_userScoreLB(nullptr),
_userInsureScoreLB(nullptr)
{
}

BankShowScene::~BankShowScene()
{
    CC_SAFE_RELEASE(_userScoreLB);
    CC_SAFE_RELEASE(_userInsureScoreLB);
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(RECORD_REQUEST_EVENT);
}

bool BankShowScene::init()
{
    if(!BankScene::init())
    {
        return false;
    }
    
    auto pbg = ImageView::create();
    pbg->setTouchEnabled(true);
    pbg->setScale9Enabled(true);
    pbg->setContentSize(WIN_SIZE);
    pbg->setPosition(WIN_SIZE/2);
    this->addChild(pbg);

    
    _layout = Layout::create();
    _layout->setTag(BANK_TAG_MAINLAYOUT);
    _layout->setContentSize(Size(1136, 640));
    
    
    auto rootNode = CSLoader::createNode("bank_res/Bank.csb");
    rootNode->setTag(BANK_TAG_ROOTNODE);
    _layout->addChild(rootNode);
    
    
    initSaveTake();
    initPresent();
    initRecord();
    
    _saveLayout->setVisible(true);
    _presentLayout->setVisible(false);
    _recordLayout->setVisible(false);
   
    if (HallDataMgr::getInstance()->m_cbInsureEnable == 0)
    {
        initApply();
        m_eType = type_apply;
        _applyLayout->setVisible(true);
        _saveLayout->setVisible(false);
        
    }
     _layout->setScaleX(JUDGE_SCALE);
    

    //存取款
    Button *SaveTake = static_cast<Button *>(rootNode->getChildByName("bank_saveTake_btn"));
    
    if (nullptr != SaveTake)
    {

        SaveTake->setTag(BANK_TAG_SAVEBTN);
        SaveTake->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithTouch, this));
        
        
        SaveTake->loadTextures("bank_res/bank_saveTake_Icon.png", "bank_res/bank_saveTakeBtn_0.png");
        
    }
    //赠送
    Button *Present = static_cast<Button *>(rootNode->getChildByName("bank_present_btn"));
    if (nullptr != Present)
    {
    
        Present->setTag(BANK_TAG_PRESENT);
        Present->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithTouch, this));
    }
    
    //操作记录
    Button *Record = static_cast<Button *>(rootNode->getChildByName("bank_record_btn"));
    if (nullptr != Record)
    {
        Record->setTag(BANK_TAG_RECORD);
        Record->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithRecord, this));
    }
    
    //返回按钮
    Button *back = static_cast<Button *>(rootNode->getChildByName("back_btn"));
    back->setTag(BANK_TAG_CLOSE);
    back->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithTouch, this));
    this->addChild(_layout);
    return true;
    
}

void BankShowScene::onEnter()
{
    
    BankScene::onEnter();
    
}

void BankShowScene::onEnterTransitionDidFinish()
{
    BankScene::onEnterTransitionDidFinish();
}

void BankShowScene::onExit()
{
    
    
    BankScene::onExit();
    
}

void BankShowScene::initApply()
{
    
    m_eType = type_apply;
    
    _applyLayout =  Layout::create();
    _applyLayout->setContentSize(Size(1136, 640));
    _layout->addChild(_applyLayout);
    
    auto apply = CSLoader::createNode("bank_res/Apply.csb");
    apply->setTag(BANK_TAG_SUBNODE);
    _applyLayout->addChild(apply);
    

    Text *passwordText = static_cast<Text *>(apply->getChildByName("label_password"));
    EditBox *password = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    password->setDelegate(this);
    password->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    password->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    password->setPosition(cocos2d::Point(passwordText->getPositionX()+80, passwordText->getPositionY()));
    password->setMaxLength(LEN_ACCOUNT);
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    password->setFontSize(24);
    password->setTag(1);
    password->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 23);
    password->setPlaceHolder("请输入密码");
    password->setFontColor(Color3B::YELLOW);
    _applyLayout->addChild(password);
    
    
    Text *confirmText = static_cast<Text *>(apply->getChildByName("label_confirm"));
    EditBox *confirm = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    confirm->setDelegate(this);
    confirm->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    confirm->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    confirm->setPosition(cocos2d::Point(confirmText->getPositionX()+80, confirmText->getPositionY()));
    confirm->setMaxLength(LEN_ACCOUNT);
    confirm->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    confirm->setFontSize(24);
    confirm->setTag(2);
    confirm->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 23);
    confirm->setPlaceHolder("请重复您刚才输入的密码");
    confirm->setFontColor(Color3B::YELLOW);
    _applyLayout->addChild(confirm);
    
    //开通银行
    Button *applyBtn = static_cast<Button *>(apply->getChildByName("bank_apply_btn"));
    if(applyBtn != nullptr)
    {
        applyBtn->setTag(BANK_TAG_APPLY);
        applyBtn->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithTouch, this));
    }
}

void BankShowScene::initSaveTake()
{

    _saveLayout =  Layout::create();
    _saveLayout->setContentSize(Size(1136, 640));
    _layout->addChild(_saveLayout);

    
    auto saveTake = CSLoader::createNode("bank_res/SaveTake.csb");
    saveTake->setTag(BANK_TAG_SUBNODE);
    _saveLayout->addChild(saveTake);
    
    //取款数目
    Text *saveText = static_cast<Text *>(saveTake->getChildByName("Text_savetake"));
    EditBox *takeNum = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    takeNum->setDelegate(this);
    takeNum->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    takeNum->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    takeNum->setPosition(cocos2d::Point(saveText->getPositionX()+80, saveText->getPositionY()));
    takeNum->setMaxLength(LEN_ACCOUNT);
    takeNum->setFontSize(20);
    takeNum->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    takeNum->setPlaceHolder("请输入取款数目");
    takeNum->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
    takeNum->setTag(1);
    takeNum->setFontColor(Color3B::YELLOW);
    _saveLayout->addChild(takeNum);
    
    //取款密码
    Text *passwordText = static_cast<Text *>(saveTake->getChildByName("Text_password"));
    EditBox *password = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    password->setDelegate(this);
    password->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    password->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    password->setPosition(cocos2d::Point(passwordText->getPositionX()+80, passwordText->getPositionY()));
    password->setMaxLength(LEN_ACCOUNT);
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    password->setFontSize(20);
    password->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    password->setPlaceHolder("存款不需要输入密码");
    password->setTag(2);
    password->setFontColor(Color3B::YELLOW);
    _saveLayout->addChild(password);
    
    //携带
    Text *pText = static_cast<Text *>(saveTake->getChildByName("Text_0"));
    Label *score = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","), FONT_TREBUCHET_MS_BOLD, 24);
    score->setAnchorPoint(Vec2(.0, .5));
    score->setTag(BANK_TAG_USERSCORE);
    score->setPosition(Vec2(pText->getPosition().x + 50, pText->getPosition().y));
    score->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(score, 200);
    _saveLayout->addChild(score);
    
    //银行存款
    Text *_pText = static_cast<Text *>(saveTake->getChildByName("Text_1"));
    Label *insureScore = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserInsure, ","), FONT_TREBUCHET_MS_BOLD, 24);
    insureScore->setAnchorPoint(Vec2(.0, .5));
    insureScore->setTag(BANK_TAG_USERINSURE);
    insureScore->setPosition(Vec2(_pText->getPosition().x + 50, _pText->getPosition().y));
    insureScore->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(insureScore, 200);
    _saveLayout->addChild(insureScore);
    
    //存入
    Button *saveBtn = static_cast<Button *>(saveTake->getChildByName("Button_save"));
    if (nullptr != saveBtn)
    {
        saveBtn->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithTouch, this));
        saveBtn->setTag(BANK_TAG_SAVE);
    }
    
    //取款
    Button *takeBtn = static_cast<Button *>(saveTake->getChildByName("Button_take"));
    if (nullptr != takeBtn)
    {
        takeBtn->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithTouch, this));
        takeBtn->setTag(BANK_TAG_TAKE);
    }
    
    //刷新信息
    CallFunc *func = CallFunc::create([this]{
        HallDataMgr::getInstance()->AddpopLayer("","", Type_Wait);this->sendInsureInfo();});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.4f), func));
}

void BankShowScene::initPresent()
{
    _presentLayout = Layout::create();
    _presentLayout->setContentSize(Size(1136, 640));
    _layout->addChild(_presentLayout);
    
    auto presentNode = CSLoader::createNode("bank_res/Present.csb");
    presentNode->setTag(BANK_TAG_SUBNODE);
    _presentLayout->addChild(presentNode);
    
    
    //银行存款
    Text *_pText = static_cast<Text *>(presentNode->getChildByName("Text_0"));
    Label *insureScore = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserInsure, ","), FONT_TREBUCHET_MS_BOLD, 24);
    insureScore->setAnchorPoint(Vec2(.0, .5));
    insureScore->setTag(BANK_TAG_USERINSURE);
    insureScore->setPosition(Vec2(_pText->getPosition().x + 50, _pText->getPosition().y));
    insureScore->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(insureScore, 200);
    _presentLayout->addChild(insureScore);
    
    //接收目标
    Text *targetText = static_cast<Text *>(presentNode->getChildByName("Text_1"));
    EditBox *targetEdit = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    targetEdit->setDelegate(this);
    targetEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    targetEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    targetEdit->setPosition(cocos2d::Point(targetText->getPositionX()+80, targetText->getPositionY()));
    targetEdit->setMaxLength(LEN_ACCOUNT);
    targetEdit->setFontSize(20);
    targetEdit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    targetEdit->setPlaceHolder("请输入接收目标");
    targetEdit->setTag(0);
    targetEdit->setFontColor(Color3B::YELLOW);
    _presentLayout->addChild(targetEdit);
    
    //赠送金额
    Text *numText = static_cast<Text *>(presentNode->getChildByName("Text_2"));
    EditBox *prentNum = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    prentNum->setDelegate(this);
    prentNum->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    prentNum->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    prentNum->setPosition(cocos2d::Point(numText->getPositionX()+80, numText->getPositionY()));
    prentNum->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
    prentNum->setMaxLength(LEN_ACCOUNT);
    prentNum->setFontSize(20);
    prentNum->setTag(1);
    prentNum->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    prentNum->setPlaceHolder("请输入赠送金额");
    prentNum->setFontColor(Color3B::YELLOW);
    _presentLayout->addChild(prentNum);
    
    //银行密码
    Text *passwordText = static_cast<Text *>(presentNode->getChildByName("Text_3"));
    EditBox *password = EditBox::create(Size(318, 58), "bank_res/bank_edit_textIcon.png");
    password->setDelegate(this);
    password->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    password->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    password->setPosition(cocos2d::Point(passwordText->getPositionX()+80, passwordText->getPositionY()));
    password->setMaxLength(LEN_ACCOUNT);
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    password->setFontSize(20);
    password->setTag(2);
    password->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 20);
    password->setPlaceHolder("请输入银行密码");
    password->setFontColor(Color3B::YELLOW);
    _presentLayout->addChild(password);
    
    //赠送模式
    Button *prentByID = static_cast<Button *>(presentNode->getChildByName("Button_id"));
    if (prentByID != nullptr)
    {
        prentByID->setTag(4);
        prentByID->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithSendType, this));
    }
    
    Button *presentByNick = static_cast<Button *>(presentNode->getChildByName("Button_nick"));
    if (presentByNick != nullptr)
    {
        presentByNick->setTag(5);
        presentByNick->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithSendType, this));
    }
    presentByNick->setEnabled(false);
    presentByNick->setVisible(false);
    
    //赠送
    Button *presentBtn= static_cast<Button *>(presentNode->getChildByName("Button_present"));
    if (presentBtn != nullptr)
    {
        presentBtn->addTouchEventListener(CC_CALLBACK_2(BankShowScene::buttonEventWithPresent, this));
    }
    
    Text *prentText = static_cast<Text *>(presentNode->getChildByName("Text_1"));
    prentText->setTag(6);
    
    //默认id赠送模式
    buttonEventWithSendType(prentByID,ui::Widget::TouchEventType::ENDED);
}

void BankShowScene::initRecord()
{
   
    _recordLayout = Layout::create();
    _recordLayout->setContentSize(Size(1136, 640));
    _layout->addChild(_recordLayout);
    
    //
    
    auto recordNode = CSLoader::createNode("bank_res/Record.csb");
    recordNode->setTag(BANK_TAG_SUBNODE);
    _recordLayout->addChild(recordNode);
    
}

int BankShowScene::getBankType() const
{

    return m_eType;
}
void BankShowScene::updateScore()
{
    if(m_eType == type_savetake)
    {
        Label *userScore = static_cast<Label *>(_saveLayout->getChildByTag(BANK_TAG_USERSCORE));
        if (nullptr != userScore)
        {
            
            userScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","));
        }
        
        
        Label *userInsure = static_cast<Label *>(_saveLayout->getChildByTag(BANK_TAG_USERINSURE));
        if (nullptr != userInsure)
        {
            userInsure->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserInsure, ","));
            
        }
    }
    
    if (m_eType == type_present)
    {
        
        Label *userInsure = static_cast<Label *>(_presentLayout->getChildByTag(BANK_TAG_USERINSURE));
        if (nullptr != userInsure)
        {
            userInsure->setString(getScorewithComma(HallDataMgr::getInstance()->m_UserInsure, ","));
            
        }
    }
}

//MARK::开通银行结果
 void BankShowScene:: InsureEnableResult(void* pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    auto result = (CMD_GP_UserInsureEnableResult *)pData;
    if (result->cbInsureEnabled == 1)
    {
        HallDataMgr::getInstance()->m_cbInsureEnable = 1;
        
        _applyLayout->removeFromParent();
        _applyLayout = nullptr;
        m_eType = type_savetake;
        _saveLayout->setVisible(true);
        
        DebugLog("开通银行成功");
    }
    else
    {
        DebugLog("开通银行失败");
        
        auto title = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
        HallDataMgr::getInstance()->AddpopLayer("系统提示", title, Type_Ensure);
        
        auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    }
}
//MARK::银行资料
 void BankShowScene::InsureInfoResult(void* pData, WORD wSize)
{
     HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    if (wSize != sizeof(CMD_GP_UserInsureInfo))
    {
        return;
    }
    
    memset(&m_sInfo, 0, sizeof(m_sInfo));
    memcpy(&m_sInfo, pData, wSize);
    HallDataMgr::getInstance()->m_UserScore = m_sInfo.lUserScore;
    HallDataMgr::getInstance()->m_UserInsure = m_sInfo.lUserInsure;
    
    updateScore();
    
    auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    
    
    DebugLog("银行资料");
}
//MARK::操作成功
 void BankShowScene::BankSuccedResult(void* pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    
    
    auto result = (CMD_GP_UserInsureSuccess *)pData;
    HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
    HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
    
    //更新显示
    updateScore();
    
    //凭证编号
    DWORD idx = result->dwRecordIndex;
    
    auto str = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
    auto modeLayer = (ModeLayer*)HallDataMgr::getInstance()->AddpopLayer("操作成功", str , Type_Ensure);
    if (3 == result->cbOperateType)
    {
        modeLayer->setEnsureCallback([=]()
                                     {
                                         BankSendReceiptLayer *p = BankSendReceiptLayer::create();
                                         this->addChild(p);
                                         
                                         _stBankGiftInfo info = HallDataMgr::getInstance()->m_tagBankSend;
                                         _stReceipt rec = {};
                                         rec.dwRecordIndex = idx;
                                         rec.dwRecUserId = info.dwReceiveUserId;
                                         rec.llSendCount = info.llSendCount;
                                         rec.dwSendUserId = HallDataMgr::getInstance()->m_dwGameID;
                                         rec.strRecUserName = info.sReceiveUserName;
                                         rec.strSendUserName = HallDataMgr::getInstance()->m_pNickName;
                                         p->refreshReceipt(rec);
                                         
                                         HallDataMgr::getInstance()->m_tagBankSend = {};
                                     });
    }
    
    //大厅金币更新
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    
    DebugLog("银行操作成功");
}
//MARK::操作失败
 void BankShowScene::BankFailureResult(void* pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);});
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    
    auto result = (CMD_GP_UserInsureFailure *)pData;
    auto str = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("操作失败", str , Type_Ensure);
    
   
    DebugLog("银行操作失败");
    
}
//MARK::用户信息
 void BankShowScene::BankUserInfoResult(void* pData, WORD wSize)
{
    DebugLog("银行用户信息");
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    auto result = (CMD_GP_UserTransferUserInfo *)pData;
    auto nickname = WHConverUnicodeToUtf8WithArray(result->szAccounts);
    if (0 == HallDataMgr::getInstance()->m_tagBankSend.nSendType)       //id赠送
    {
        HallDataMgr::getInstance()->m_tagBankSend.sReceiveUserName = nickname;
        
        this->sendTransferScore(m_TransferScore, m_BankPass, 1, nickname);
    }
    else if (1 == HallDataMgr::getInstance()->m_tagBankSend.nSendType)  //昵称赠送
    {
        HallDataMgr::getInstance()->m_tagBankSend.dwReceiveUserId = result->dwTargetGameID;
        
        this->sendTransferScore(m_TransferScore, m_BankPass, 0, HallDataMgr::getInstance()->m_tagBankSend.sReceiveUserName);
    }
}

void BankShowScene::queryUserInfo(const BYTE &cbType, const std::string &sTarget)
{
    if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
    {
        CMD_GP_QueryUserInfoRequest request;
        memset(&request, 0, sizeof(request));
        
        request.cbByNickName = cbType;
        UTF8Str_To_UTF16Str(sTarget.c_str(), request.szAccounts);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_USER_INFO_REQUEST, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        CMD_GR_C_QueryUserInfoRequest request;
        memset(&request, 0, sizeof(request));
        
        request.cbByNickName = cbType;
        UTF8Str_To_UTF16Str(sTarget.c_str(), request.szAccounts);
        
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_USER_INFO_REQUEST, &request, sizeof(request));
    }
}

//MARK::EditBoxDelegate
void BankShowScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{

    if (editBox->getTag() == 1)
    {
        if (m_eType == type_savetake)
        {
            auto score = MAX(HallDataMgr::getInstance()->m_UserInsure, HallDataMgr::getInstance()->m_UserScore);
            SCORE editscore = 0;
            sscanf(text.c_str(), "%lld[0-9]", &editscore);
            
            if (editscore > score)
            {
                CallFunc *func = CallFunc::create([=](){
                    editBox->setText(__String::createWithFormat("%lld", score)->getCString());
                    editBox->touchDownAction(editBox, cocos2d::ui::Widget::TouchEventType::ENDED);
                                                  });
                DelayTime *delay = DelayTime::create(0.3f);
                Sequence *action = Sequence::createWithTwoActions(delay, func);
                this->runAction(action);
      
            } else
            {
         
            }
        }
        if (m_eType == type_present)
        {
            
//            if (m_nSendType == 0)
//            {
//                SCORE editID = 0;
//                sscanf(text.c_str(), "%lld[0-9]", &editID);
//                if (editID == 0)
//                {
//                    
//                }
//            }
//            
            
            auto score = HallDataMgr::getInstance()->m_UserInsure;
            SCORE editscore = 0;
            sscanf(text.c_str(), "%lld[0-9]", &editscore);
            if (editscore > score)
            {
                CallFunc *func = CallFunc::create([=](){
                    editBox->setText(__String::createWithFormat("%lld", score)->getCString());
                    editBox->touchDownAction(editBox, cocos2d::ui::Widget::TouchEventType::ENDED);
                });
                DelayTime *delay = DelayTime::create(0.3f);
                Sequence *action = Sequence::createWithTwoActions(delay, func);
                this->runAction(action);
            } else
            {
                if (editscore == 0)
                {

                }
            }
        }
    }
}

//MARK::赠送模式
void BankShowScene::buttonEventWithSendType(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    Button *btn = static_cast<Button *>(target);
    Node *subNode =  static_cast<Node *>(_presentLayout->getChildByTag(BANK_TAG_SUBNODE));
    Text *presentText = static_cast<Text *>(subNode->getChildByTag(6));
    EditBox *id = static_cast<EditBox *>(_presentLayout->getChildByTag(0));
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        //id赠送
        if (btn->getTag() == 4)
        {
            Button *ID = btn;
            Button *Nickname = static_cast<Button *>(subNode->getChildByTag(5));
            ID->loadTextures("bank_res/bank_typeID_1.png",  "bank_res/bank_typeID_0.png");
            Nickname->loadTextures("bank_res/bank_typeNick_0.png", "bank_res/bank_typeNick_1.png");
            
            presentText->setString("接收ID :");
            
            id->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
            
            m_nSendType = 0;
            
        }else
        {
            //昵称赠送
            Button *Nickname = btn;
            Button *ID = static_cast<Button *>(subNode->getChildByTag(4));
            ID->loadTextures("bank_res/bank_typeID_0.png",  "bank_res/bank_typeID_1.png");
            Nickname->loadTextures("bank_res/bank_typeNick_1.png", "bank_res/bank_typeNick_0.png");
            
            presentText->setString("接收昵称 :");
            
            id->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
            
            m_nSendType = 1;
        }
    }
}

//MARK::
void BankShowScene::buttonEventWithTouch(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    Button *btn = static_cast<Button *>(target);
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        
        if (btn->getTag() == BANK_TAG_CLOSE) //关闭银行
        {
            CallFunc *func = CallFunc::create([this]{this->removeFromParent();});
            Sequence *quence = Sequence::createWithTwoActions(MoveTo::create(0.4f, Vec2(1136, 0)), func);
            
            this->runAction(quence);
            
        }else if (btn->getTag() == BANK_TAG_SAVEBTN) //取款切换
        {
            
            
            if(m_eType == type_savetake)
            {
                
                return;
            }
            
            m_eType = type_savetake;
            _presentLayout->setVisible(false);
            _recordLayout->setVisible(false);
            _saveLayout->setVisible(true);
            
            if (HallDataMgr::getInstance()->m_cbInsureEnable == 0)
            {
                
                m_eType = type_apply;
                _applyLayout->setVisible(true);
                _saveLayout->setVisible(false);
                
            }
            
            auto rootNode = static_cast<Node*>(_layout->getChildByTag(BANK_TAG_ROOTNODE));
            assert(rootNode);
            auto SaveTake = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_SAVEBTN));
            auto  Present = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_PRESENT));
            auto  Record  = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_RECORD));
            if (SaveTake && Present && Record)
            {
                SaveTake->loadTextures("bank_res/bank_saveTake_Icon.png", "bank_res/bank_saveTakeBtn_0.png");
                Present->loadTextures("bank_res/bank_presentBtn_0.png", "bank_res/bank_present_Icon.png");
                Record->loadTextures("bank_res/bank_recordBtn_0.png", "bank_res/bank_recordIcon.png");
            }
            
    
        }else if (btn->getTag() == BANK_TAG_PRESENT)
        {
           
            if(m_eType == type_present)
            {
                
                return;
            }
            
            m_eType = type_present;
            _presentLayout->setVisible(true);
            _recordLayout->setVisible(false);
            _saveLayout->setVisible(false);
            if (_applyLayout != nullptr)
                _applyLayout->setVisible(false);
            
            auto rootNode = static_cast<Node*>(_layout->getChildByTag(BANK_TAG_ROOTNODE));
            assert(rootNode);
            auto SaveTake = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_SAVEBTN));
            auto  Present = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_PRESENT));
            auto  Record  = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_RECORD));
            if (SaveTake && Present && Record)
            {
                SaveTake->loadTextures("bank_res/bank_saveTakeBtn_0.png", "bank_res/bank_saveTake_Icon.png");
                Present->loadTextures("bank_res/bank_present_Icon.png", "bank_res/bank_presentBtn_0.png");
                Record->loadTextures("bank_res/bank_recordBtn_0.png", "bank_res/bank_recordIcon.png");
            }
     
        }
        else if(btn->getTag() == BANK_TAG_APPLY) //开通银行
        {
            
            EditBox *passwordEdit = static_cast<EditBox *>(_applyLayout->getChildByTag(1));
            EditBox *confirmEdit = static_cast<EditBox *>(_applyLayout->getChildByTag(2));
            std::string password = std::string(passwordEdit->getText());
            std::string confirm = std::string(confirmEdit->getText());
            
            if (!checkSpaceStr(password) && !checkSpaceStr(confirm))
            {
                if ((strlen(passwordEdit->getText()) == 0) || (strlen(confirmEdit->getText()) == 0))
                {
                    HallDataMgr::getInstance()->AddpopLayer("提示","密码不能为空", Type_Ensure);
                    return;
                }
                
                if (password.compare(confirm) != 0)
                {
                    HallDataMgr::getInstance()->AddpopLayer("提示","两次输入的密码不一致", Type_Ensure);
                    return;
                }
                
                HallDataMgr::getInstance()->AddpopLayer("","", Type_Wait);
                sendInsureEnable(password);
            }
        }else  if (btn->getTag() == BANK_TAG_SAVE) //存款
        {
            
            EditBox *saveNumEdit = static_cast<EditBox *>(_saveLayout->getChildByTag(1));

            if (strlen(saveNumEdit->getText()) == 0)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示","存款不能为0", Type_Ensure);
                return;
            }
            
            HallDataMgr::getInstance()->AddpopLayer("","", Type_Wait);
            SCORE saveScore = 0;
            sscanf(saveNumEdit->getText(), "%lld", &saveScore);
            sendSaveScore(saveScore);
            
        }else if (btn->getTag() == BANK_TAG_TAKE)
        {
            
            EditBox *takeNumEdit = static_cast<EditBox *>(_saveLayout->getChildByTag(1));
            
            if (strlen(takeNumEdit->getText()) == 0)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示","取款不能为0", Type_Ensure);
                return;
            }
            
            EditBox *passwordEdit = static_cast<EditBox *>(_saveLayout->getChildByTag(2));
            
            if (strlen(passwordEdit->getText()) == 0)
            {
                HallDataMgr::getInstance()->AddpopLayer("提示","请输入密码", Type_Ensure);
                return;
            }
            
            HallDataMgr::getInstance()->AddpopLayer("","", Type_Wait);
            SCORE takeScore = 0;
            sscanf(takeNumEdit->getText(), "%lld", &takeScore);
            sendTakeScore(takeScore, passwordEdit->getText());
        }
    }
}


void BankShowScene::buttonEventWithRecord(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
    
        if(m_eType == type_record)
        {
            
            return;
        }
        
        m_eType = type_record;
        _presentLayout->setVisible(false);
        _recordLayout->setVisible(true);
        _saveLayout->setVisible(false);
        if (_applyLayout != nullptr)
            _applyLayout->setVisible(false);
        
        auto rootNode = static_cast<Node*>(_layout->getChildByTag(BANK_TAG_ROOTNODE));
        assert(rootNode);
        auto SaveTake = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_SAVEBTN));
        auto  Present = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_PRESENT));
        auto  Record  = static_cast<Button*>(rootNode->getChildByTag(BANK_TAG_RECORD));
        if (SaveTake && Present && Record)
        {
            SaveTake->loadTextures("bank_res/bank_saveTakeBtn_0.png", "bank_res/bank_saveTake_Icon.png");
            Present->loadTextures("bank_res/bank_presentBtn_0.png", "bank_res/bank_present_Icon.png");
            Record->loadTextures("bank_res/bank_recordIcon.png", "bank_res/bank_recordBtn_0.png");
        }
        
        
        HallDataMgr::getInstance()->AddpopLayer("", "正在获取数据...", Type_Wait);
        
        
         Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(RECORD_REQUEST_EVENT, CC_CALLBACK_1(BankShowScene::RecordReuqcestEvent, this)), 1);
        
        SCORE _time = getsystemtomillisecond() - HallDataMgr::getInstance()->m_Logintime;
        auto prequest = new HttpRequest;
        auto purl = __String::createWithFormat("%s/WS/MobileInterface.ashx?action=GetBankRecord&userid=%u%s", ADDRESS_WHhttp6603,HallDataMgr::getInstance()->m_dwUserID,HallDataMgr::getInstance()->getSignature(_time).c_str());
        
        log("%s\n",purl->getCString());
        prequest->setUrl(purl->getCString());
        prequest->setRequestType(HttpRequest::Type::GET);
        prequest->setResponseCallback(CC_CALLBACK_2(BankShowScene::RecoreRequestCallBack, this));
        HttpClient::getInstance()->send(prequest);
        prequest->release();

    }
}

void BankShowScene::buttonEventWithPresent(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
      if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
      {
          EditBox *edTarget = static_cast<EditBox *>(_presentLayout->getChildByTag(0));
          EditBox *edNum = static_cast<EditBox *>(_presentLayout->getChildByTag(1));
          EditBox *edPassword = static_cast<EditBox *>(_presentLayout->getChildByTag(2));
          
          std::string target = edTarget->getText();
          std::string num = edNum->getText();
          std::string password = edPassword->getText();          
          
          if (target.length() == 0)
          {
              if (m_nSendType == 0)
              {
                  HallDataMgr::getInstance()->AddpopLayer("提示", "请输入玩家id", Type_Ensure);
              }
              else
              {
                   HallDataMgr::getInstance()->AddpopLayer("提示", "请输入玩家昵称", Type_Ensure);
              }
              
              return;
          }
          
          if (num.length() == 0)
          {
               HallDataMgr::getInstance()->AddpopLayer("提示", "请输入赠送金币数目", Type_Ensure);
              
              return;
          }
          
          if (password.length() == 0)
          {
               HallDataMgr::getInstance()->AddpopLayer("提示", "请输入银行密码", Type_Ensure);
              return;
          }
          
          HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
          SCORE score = 0;
          sscanf(edNum->getText(), "%lld",&score);
          //sendTransferScore(score, password, m_nSendType, target);
          
          _stBankGiftInfo tagInfo = {};
          tagInfo.nSendType = m_nSendType;
          tagInfo.llSendCount = score;
          if (0 == m_nSendType)
          {
              tagInfo.dwReceiveUserId = atoi(target.c_str());
          }
          else
          {
              tagInfo.sReceiveUserName = target;
          }
          HallDataMgr::getInstance()->m_tagBankSend = tagInfo;
          
          m_TransferScore = score;
          m_BankPass = password;
          queryUserInfo(m_nSendType, target);
      }
    
}
//MARK::检查空格
bool BankShowScene::checkSpaceStr(const std::string str) const
{
    
    if (str.find(" ") != std::string::npos)
    {
        //有空格
        HallDataMgr::getInstance()->AddpopLayer("提示","密码中不能有空格", Type_Ensure);
 
        return true;
        
    }
    
    return false;
}

//MARK::弹出动画
void BankShowScene::popBank()
{
  
    Action *action = MoveTo::create(0.4f, Vec2(.0, .0));
    this->runAction(action);
    
}

void BankShowScene::RecordReuqcestEvent(cocos2d::EventCustom *event)
{
    
    showRecord();
}

//MARK::http回调
void BankShowScene::RecoreRequestCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
     HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    if(this->getReferenceCount() == 0)
    {
        
        return;
    }
    
    if (response == NULL)
    {
        return;
    }
    if (response->isSucceed() == false)
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    int nsize = (int)buffer->size();
    if (nsize == 0)
    {
        return;
    }
    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    log("%s\n",backdata.c_str());
    
    rapidjson::Document doc;
    doc.Parse<0>(backdata.c_str());
    const rapidjson::Value &val = DICTOOL->getSubDictionary_json(doc,"data");
    int num = DICTOOL->getArrayCount_json(val, "list");
    m_Datalist.clear();
    for (int index=0; index<num; ++index)
    {
        const rapidjson::Value &checkdata = DICTOOL->getDictionaryFromArray_json(val, "list", index);
        
        auto pcheckdata = new BankRecord;
        pcheckdata->autorelease();
        
        std::string datestr = DICTOOL->getStringValue_json(checkdata, "CollectDate");
        SCORE timevalue = 0;
        sscanf(datestr.c_str(), "\/Date(%lld)\/", &timevalue);
        pcheckdata->setDate(getTimeStr(timevalue/1000));
        pcheckdata->setType(DICTOOL->getStringValue_json(checkdata, "TradeTypeDescription"));
        pcheckdata->setSwapScore(DICTOOL->getIntValue_json(checkdata, "SwapScore"));
        pcheckdata->setRevenue(DICTOOL->getIntValue_json(checkdata, "Revenue"));
        m_Datalist.push_back(pcheckdata);
    }
    
    
    EventCustom event(RECORD_REQUEST_EVENT);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

    
    DebugLog("银行操作记录获取成功");
    
}

//MARK::银行记录处理
void BankShowScene::showRecord()
{
    
    if (m_Datalist.size() == 0)
    {
        
        auto record = Label::createWithSystemFont("无操作记录", FONT_TREBUCHET_MS_BOLD, 32);
        record->setAnchorPoint(Vec2(.5, .5));
        record->setPosition(Vec2(568, 320));
        _recordLayout->addChild(record);
        
        return;
        
    }
    
    
    ListView *list = static_cast<ListView *>(_recordLayout->getChildByTag(BANK_TAG_RECORD_LISET)) ;
    if (list != nullptr)
    {
        list->removeFromParent();
    }
    
    
    auto listView = ListView::create();
    listView->setBounceEnabled(true);
    listView->setTag(BANK_TAG_RECORD_LISET);
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setContentSize(Size(1048, 420));
    listView->setBackGroundColor(Color3B::RED);
    listView->setAnchorPoint(Vec2(.5, 0.0));
    listView->setPosition(Vec2(568, 21.5));
    listView->setItemsMargin(1.0f);
    _recordLayout->addChild(listView);
    
    for (auto record : m_Datalist)
    {
        Layout *layout = Layout::create();
        layout->setContentSize(Size(1048, 50));
        
        //交易日期
        Label *date = Label::createWithSystemFont(record->getDate(), FONT_TREBUCHET_MS_BOLD, 24);
        date->setPosition(Vec2(100, layout->getContentSize().height/2));
        layout->addChild(date);
        
        //交易类型
        Label *type = Label::createWithSystemFont(record->getType(), FONT_TREBUCHET_MS_BOLD, 24);
        type->setPosition(Vec2(380, layout->getContentSize().height/2));
        layout->addChild(type);
        
        //交易金额
        Label *num = Label::createWithSystemFont(__String::createWithFormat("%lld",record->getSwapScore())->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
        num->setPosition(Vec2(660, layout->getContentSize().height/2));
        layout->addChild(num);
        
        //服务费
        Label *fee = Label::createWithSystemFont(__String::createWithFormat("%lld",record->getRevenue())->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
        fee->setPosition(Vec2(950, layout->getContentSize().height/2));
        layout->addChild(fee);
     
        //分割线
        ImageView *line = ImageView::create("bank_res/bank_line.png");
        line->setPosition(Vec2(layout->getContentSize().width/2, 0));
        layout->addChild(line);
        
        listView->pushBackCustomItem(layout);
        
    }
    
}




