//
//  BankLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#include "BankLayer.h"
#include "BaiJiaLeScene.h"
#include "HallDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

BankLayer::BankLayer():
m_openBankLayer(nullptr),
m_bankSaveLayer(nullptr),
m_bankSendLayer(nullptr),
m_bSceneMode(true)
{
    m_group = new ButtonGroup();
    m_group->setGroupListen(this);
    m_group->setGroupName("bank_group");
}

BankLayer::~BankLayer()
{
    if (!m_bSceneMode)
    {
        showLayer(false);
    }
    m_group->removeFromGroup("save_check");
    m_group->removeFromGroup("send_check");
    delete m_group;
    m_group = nullptr;
}

bool BankLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("bank/BankLayer.csb");
        
        m_btnLayout = static_cast<Layout*>(m_root->getChildByName("btn_layout"));
        CC_ASSERT(m_btnLayout != nullptr);
        
        //关闭
        auto csbBtn = static_cast<Button*>(m_root->getChildByName("back_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(BankLayer::touchEvent, this));
        
        //存取款
        m_checkSave = static_cast<CheckBox*>(m_btnLayout->getChildByName("save_check"));
        CC_ASSERT(m_checkSave != nullptr);
        m_group->addToGroup(m_checkSave, "save_check");
        m_checkSave->setEnabled(false);
        m_checkSave->setVisible(false);
        
        //赠送
        m_checkSend = static_cast<CheckBox*>(m_btnLayout->getChildByName("send_check"));
        CC_ASSERT(m_checkSend != nullptr);
        //m_group->addToGroup(m_checkSend, "send_check");
        m_checkSend->setVisible(false);
        m_checkSend->setEnabled(false);
        
        //判断银行是否开通
        if (HallDataMgr::getInstance()->m_cbInsureEnable)
        {
            m_btnLayout->setVisible(true);
            m_group->setSelecte("save_check");
        }
        else
        {
            m_btnLayout->setVisible(false);
            showOpenBank(true);
        }
        
        bRes = true;
    }while(false);
    return bRes;
}

bool BankLayer::uiMsg(const int &main, const int &sub, void *pData, const int &nLen)
{
    switch (main) {
        case UI_MSG_BASE::UI_MSG_BANK:
            return this->handleBankUIMsg(sub, pData, nLen);
            break;
        case UI_MSG_BASE::UI_MSG_COMMON:
        {
            bool bEnable = *((bool *)pData);
            enableWidget(m_root,bEnable);
        }
            break;
        default:
            break;
    }
    return false;
}

bool BankLayer::preButtonChecked(cocos2d::Node *pSender, bool bSelected)
{
    return true;
}

void BankLayer::onButtonChecked(cocos2d::Node *pSender)
{
    std::string btnname = pSender->getName();
    log("on: check :%s,select",btnname.c_str());
}

void BankLayer::onCheckStatuChange(cocos2d::Node *pSender, bool bSelected)
{
    std::string btnname = pSender->getName();
    log("change: check :%s,select",btnname.c_str());
    
    if (0 == strcmp(btnname.c_str(), "save_check"))
    {
        showBankSave(bSelected);
    }
    else if (0 == strcmp(btnname.c_str(), "send_check"))
    {
        showBankSend(bSelected);
    }
}

void BankLayer::showLayer(bool bShow)
{
    this->setVisible(bShow);
    
    if (m_bSceneMode)
        return;
    if (m_root->getChildByName("register_bg"))
    {
        m_root->removeChildByName("register_bg");
    }
    
    if (bShow)
    {
        if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
        {
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
    else
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
    }
}

void BankLayer::sendInsureEnable(const std::string &pass)
{
    std::string bankpass = MD5Encrypt(pass);
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
    
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load)    //此时未进入桌子列表
    {
        CMD_GP_UserEnableInsure insuer;
        memset(&insuer, 0, sizeof(insuer));
        
        
        insuer.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), insuer.szLogonPass);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), insuer.szMachineID);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)bankpass.c_str(), insuer.szInsurePass);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_ENABLE_INSURE, &insuer, sizeof(insuer),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room) //桌子列表界面
    {
        CMD_GR_C_EnableInsureRequest insuer;
        memset(&insuer, 0, sizeof(insuer));
        
        insuer.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), insuer.szLogonPass);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), insuer.szMachineID);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)bankpass.c_str(), insuer.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_ENABLE_INSURE_REQUEST, &insuer, sizeof(insuer));
    }
}

void BankLayer::sendInsureInfo()
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_QueryInsureInfo info;
        memset(&info, 0, sizeof(info));
        
        info.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), info.szPassword);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INSURE_INFO, &info, sizeof(info),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_C_QueryInsureInfoRequest info;
        memset(&info,0,sizeof(info));
        
        info.cbActivityGame = SUB_GR_QUERY_INSURE_INFO;
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_pPassword.c_str(), info.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_INSURE_INFO, &info, sizeof(info));
    }
}

void BankLayer::sendSaveScore(SCORE score)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserSaveScore request;
        memset(&request, 0, sizeof(request));
        
        request.lSaveScore = score;
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), request.szMachineID);
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
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserTakeScore request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTakeScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)md5pass.c_str(), request.szPassword);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), request.szMachineID);
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
        UTF8Str_To_UTF16Str_BYTE((BYTE *)md5pass.c_str(), request.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TAKE_SCORE_REQUEST, &request, sizeof(request));
    }
}

void BankLayer::sendTransferScore(SCORE score, const std::string &pass, int type, const std::string &nickname)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        if (type == 0)
        {
            CMD_GP_QueryUserInfoRequest request;
            memset(&request, 0, sizeof(request));
            
            request.cbByNickName = type;
            UTF8Str_To_UTF16Str_BYTE((BYTE *)nickname.c_str(), request.szAccounts);
            NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
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
        UTF8Str_To_UTF16Str_BYTE((BYTE *)md5pass.c_str(), request.szPassword);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)nickname.c_str(), request.szAccounts);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TRANSFER_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        if (type == 0)
        {
            CMD_GR_C_QueryUserInfoRequest request;
            memset(&request, 0, sizeof(request));
            
            request.cbByNickName = type;
            UTF8Str_To_UTF16Str_BYTE((BYTE *)nickname.c_str(), request.szAccounts);
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
        UTF8Str_To_UTF16Str_BYTE((BYTE *)md5pass.c_str(), request.szInsurePass);
        UTF8Str_To_UTF16Str_BYTE((BYTE *)nickname.c_str(), request.szAccounts);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TRANSFER_SCORE_REQUEST, &request, sizeof(request));
    }
}

void BankLayer::updateScore()
{
    if (nullptr != m_bankSaveLayer)
    {
        m_bankSaveLayer->refreshScore();
    }
    if (nullptr != m_bankSendLayer)
    {
        m_bankSendLayer->refreshScore();
    }
}

void BankLayer::openBankSuccess()
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    m_btnLayout->setVisible(true);
    //隐藏开通界面，显示银行操作界面
    showOpenBank(false);
    m_group->setSelecte("save_check");
}

void BankLayer::onGetBankInsureInfo(void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    memset(&m_info, 0, sizeof(m_info));
    memcpy(&m_info, pData, wSize);
    HallDataMgr::getInstance()->m_UserScore = m_info.lUserScore;
    HallDataMgr::getInstance()->m_UserInsure = m_info.lUserInsure;
}

void BankLayer::onGetBankUserInfo(const std::string &nickname)
{
    this->sendTransferScore(m_TransferScore, m_BankPass, 1, nickname);
}

void BankLayer::InsureEnableResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureEnableResult *)pData;
    if (result->cbInsureEnabled == 1)
    {
        HallDataMgr::getInstance()->m_cbInsureEnable = 1;
        
        openBankSuccess();
    }
    else
    {
        auto title = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
        HallDataMgr::getInstance()->AddpopLayer("系统提示", title, Type_Ensure);
    }
    
    auto action = CallFunc::create([]
                                   {
                                       NetworkMgr::getInstance()->Disconnect(Data_Load);
                                   });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

void BankLayer::InsureInfoResult(void *pData, WORD wSize)
{
    if (wSize != sizeof(CMD_GP_UserInsureInfo))
    {
        return;
    }
    
    onGetBankInsureInfo(pData, wSize);
    
    auto action = CallFunc::create([]
                                   {
                                       NetworkMgr::getInstance()->Disconnect(Data_Load);
                                   });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    
    updateScore();
}

void BankLayer::BankSuccedResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureSuccess *)pData;
    HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
    HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
    updateScore();
    
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
    
    auto action = CallFunc::create([]
                                   {
                                       NetworkMgr::getInstance()->Disconnect(Data_Load);
                                   });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

void BankLayer::BankUserInfoResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserTransferUserInfo *)pData;
    auto nickname = WHConverUnicodeToUtf8WithArray(result->szAccounts);
    onGetBankUserInfo(nickname);
}

void BankLayer::BankInsureResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID)
    {
        case SUB_GR_USER_INSURE_ENABLE_RESULT:
        {
            auto result = (CMD_GR_S_UserInsureEnableResult *)pData;
            if (result->cbInsureEnabled == 1)
            {
                HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
                
                HallDataMgr::getInstance()->m_cbInsureEnable = 1;
                
                openBankSuccess();
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
            if (wSize != sizeof(CMD_GR_S_UserInsureInfo))
            {
                return;
            }
            onGetBankInsureInfo(pData, wSize);
            
            updateScore();
        }
            break;
        case SUB_GR_USER_INSURE_SUCCESS:
        {
            auto result = (CMD_GR_S_UserInsureSuccess *)pData;
            HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
            HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
            updateScore();
            
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
            auto nickname = WHConverUnicodeToUtf8WithArray(result->szAccounts);
            onGetBankUserInfo(nickname);
        }
            break;
            
        default:
            break;
    }
}

void BankLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == type)
    {
        INSTANCE(AudioMgr)->playClickAudio();
        if (0 == strcmp("back_btn", nodename.c_str()))
        {            
            if (m_bSceneMode)
            {
                pWidget->setEnabled(false);
                INSTANCE(SceneMgr)->transitionScene(BJL_SCENE(PLAZA_SCENE),false);
            }
            else
            {
                showLayer(false);
            }
        }
    }
}

bool BankLayer::handleBankUIMsg(const int &sub, void *pData, const int &nLen)
{
    bool bRes = false;
    do
    {
        switch (sub)
        {
            case BANK_UI::OPEN_BANK:
            {
                std::string passstr = *((std::string*)pData);
                this->sendInsureEnable(passstr);
                bRes = true;
            }
                break;
            case BANK_UI::SAVE_GOLD:
            {
                tagSaveParam* param = (tagSaveParam*)pData;
                this->sendSaveScore(param->score);
                bRes = true;
            }
                break;
            case BANK_UI::OUT_GOLD:
            {
                tagSaveParam* param = (tagSaveParam*)pData;
                this->sendTakeScore(param->score, param->passwd);
                bRes = true;
            }
                break;
            case BANK_UI::SEND_GOLD_ID:
            {
                tagSendParam* param = (tagSendParam*)pData;
                this->sendTransferScore(param->sendScore, param->passwd, 0, param->targetUser);
                bRes = true;
            }
                break;
            case BANK_UI::SEND_GOLD_NICK:
            {
                tagSendParam* param = (tagSendParam*)pData;
                this->sendTransferScore(param->sendScore, param->passwd, 1, param->targetUser);
                bRes = true;
            }
                break;
            case BANK_UI::PASSWD_CHECK:
            {
                std::string ps = *((std::string*)pData);
                bRes = this->checkStrlength(ps, "密码");
            }
                break;
            case BANK_UI::SUREPASSWD_CHECK:
            {
                std::string ps = *((std::string*)pData);
                bRes = this->checkStrlength(ps, "确认密码");
            }
                break;
            default:
                break;
        }
    }while(false);
    return bRes;
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

void BankLayer::showOpenBank(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_openBankLayer)
        {
            m_openBankLayer = OpenBankLayer::createOpenBank(this);
            m_root->addChild(m_openBankLayer);
        }
        m_openBankLayer->setVisible(true);
    }
    else
    {
        if (nullptr != m_openBankLayer)
        {
            m_root->removeChild(m_openBankLayer);
        }
    }
}

void BankLayer::showBankSave(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_bankSaveLayer)
        {
            m_bankSaveLayer = BankSaveLayer::createLayer(this);
            m_root->addChild(m_bankSaveLayer);
        }
        m_bankSaveLayer->refreshScore();
    }
    if (nullptr != m_bankSaveLayer)
    {
        m_bankSaveLayer->setVisible(bShow);
    }
}

void BankLayer::showBankSend(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_bankSendLayer)
        {
            m_bankSendLayer = BankSendLayer::createLayer(this);
            m_root->addChild(m_bankSendLayer);
        }
        m_bankSendLayer->refreshScore();
    }
    if (nullptr != m_bankSendLayer)
    {
        m_bankSendLayer->setVisible(bShow);
    }
}