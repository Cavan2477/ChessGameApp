/************************************************************************************
 * file: 		BankScene.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:19:36
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "BankScene.h"
#include "../../Public/Help.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../../DataMgr/HallDataMgr.h"

bool BankScene::init()
{
    if (!Layer::init())
        return false;
    
    return true;
}

void BankScene::onEnter()
{
    Layer::onEnter();
}

void BankScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_ENABLE_RESULT, CC_CALLBACK_2(BankScene::InsureEnableResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_INFO, CC_CALLBACK_2(BankScene::InsureInfoResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_SUCCESS, CC_CALLBACK_2(BankScene::BankSuccedResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_FAILURE, CC_CALLBACK_2(BankScene::BankFailureResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_QUERY_USER_INFO_RESULT, CC_CALLBACK_2(BankScene::BankUserInfoResult, this));
}

void BankScene::onExit()
{
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_USER_INSURE_ENABLE_RESULT);
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_USER_INSURE_INFO);
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_USER_INSURE_SUCCESS);
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_USER_INSURE_FAILURE);
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_QUERY_USER_INFO_RESULT);
    
    Layer::onExit();
}

//发送开通银行
void BankScene::sendInsureEnable(const std::string &pass)
{
    std::string bankpass = MD5Encrypt(pass);
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserEnableInsure insuer;
        memset(&insuer, 0, sizeof(insuer));
        
        
        insuer.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), insuer.szLogonPass);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), insuer.szMachineID);
        UTF8Str_To_UTF16Str(bankpass.c_str(), insuer.szInsurePass);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_ENABLE_INSURE, &insuer, sizeof(insuer),NetworkMgr::getInstance()->getSocketOnce());
    }
}

//查询银行
void BankScene::sendInsureInfo()
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_QueryInsureInfo info;
        memset(&info, 0, sizeof(info));
        
        info.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), info.szPassword);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INSURE_INFO, &info, sizeof(info),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_C_QueryInsureInfoRequest info;
        memset(&info,0,sizeof(info));
        
        info.cbActivityGame = SUB_GR_QUERY_INSURE_INFO;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), info.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_INSURE_INFO, &info, sizeof(info));
    }
}

//存款
void BankScene::sendSaveScore(SCORE score)
{
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserSaveScore request;
        memset(&request, 0, sizeof(request));
        
        request.lSaveScore = score;
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), request.szMachineID);
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

//取款
void BankScene::sendTakeScore(SCORE score, const std::string &pass)
{
    if(HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        CMD_GP_UserTakeScore request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTakeScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str(md5pass.c_str(), request.szPassword);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), request.szMachineID);
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
        UTF8Str_To_UTF16Str(md5pass.c_str(), request.szInsurePass);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TAKE_SCORE_REQUEST, &request, sizeof(request));
    }
}

//转帐
void BankScene::sendTransferScore(SCORE score, const std::string &pass, int type, const std::string &nickname)
{
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    
	if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        /*
        if (type == 0)
        {
            CMD_GP_QueryUserInfoRequest request;
            memset(&request, 0, sizeof(request));
            
            request.cbByNickName = type;
            UTF8Str_To_UTF16Str(nickname.c_str(), request.szAccounts);
            NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_USER_INFO_REQUEST, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
            m_TransferScore = score;
            m_BankPass = pass;
            return;
        }
         */
        CMD_GP_UserTransferScore request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTransferScore = score;
        auto md5pass = MD5Encrypt(pass);
    
        UTF8Str_To_UTF16Str(md5pass.c_str(), request.szPassword);
        UTF8Str_To_UTF16Str(nickname.c_str(), request.szAccounts);
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TRANSFER_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        /*
        if (type == 0)
        {
            CMD_GR_C_QueryUserInfoRequest request;
            memset(&request, 0, sizeof(request));
            
            request.cbByNickName = type;
            UTF8Str_To_UTF16Str(nickname.c_str(), request.szAccounts);
            NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_USER_INFO_REQUEST, &request, sizeof(request));
            m_TransferScore = score;
            m_BankPass = pass;
            return;
        }
         */
        
        CMD_GR_C_TransferScoreRequest request;
        memset(&request, 0, sizeof(request));
        
        request.cbActivityGame = SUB_GR_TRANSFER_SCORE_REQUEST;
        request.lTransferScore = score;
        auto md5pass = MD5Encrypt(pass);
        UTF8Str_To_UTF16Str(md5pass.c_str(), request.szInsurePass);
        UTF8Str_To_UTF16Str(nickname.c_str(), request.szAccounts);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TRANSFER_SCORE_REQUEST, &request, sizeof(request));
    } 
}