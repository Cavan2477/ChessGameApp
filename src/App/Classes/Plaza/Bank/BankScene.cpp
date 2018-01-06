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
void BankScene::sendInsureEnable(const std::string &strPass)
{
    std::string bankpass = MD5Encrypt(strPass);

    if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
    {
        CMD_GP_USER_ENABLE_INSURE cmdGPUserEnableInsure;

        memset(&cmdGPUserEnableInsure, 0, sizeof(cmdGPUserEnableInsure));
        
        
        cmdGPUserEnableInsure.dwUserID = HallDataMgr::getInstance()->m_dwUserID;

        Utf8ToUtf16(HallDataMgr::getInstance()->m_pPassword.c_str(), (WORD*)cmdGPUserEnableInsure.szLogonPwd);
		Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine.c_str(), (WORD*)cmdGPUserEnableInsure.szMachineID);
		Utf8ToUtf16(bankpass.c_str(), (WORD*)cmdGPUserEnableInsure.szInsurePwd);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_ENABLE_INSURE, &cmdGPUserEnableInsure, sizeof(cmdGPUserEnableInsure),NetworkMgr::getInstance()->getSocketOnce());
    }
}

//查询银行
void BankScene::sendInsureInfo()
{
    if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
    {
        CMD_GP_QUERY_INSURE_INFO info;
        memset(&info, 0, sizeof(info));
        
        info.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        Utf8ToUtf16(HallDataMgr::getInstance()->m_pPassword.c_str(), (WORD*)info.szPassword);
        
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INSURE_INFO, &info, sizeof(info),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        CMD_GR_CLIENT_QUERY_INSURE_INFO_REQ info;
        memset(&info,0,sizeof(info));
        
        info.cbActivityGame = SUB_GR_QUERY_INSURE_INFO;
		Utf8ToUtf16(HallDataMgr::getInstance()->m_pPassword.c_str(), (WORD*)info.szInsurePwd);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_QUERY_INSURE_INFO, &info, sizeof(info));
    }
}

//存款
void BankScene::sendSaveScore(LONGLONG score)
{
    if(HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
    {
        CMD_GP_USER_SAVE_GOLD request;
        memset(&request, 0, sizeof(request));
        
        request.lSaveScore = score;
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
		Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine.c_str(), (WORD*)request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        CMD_GR_CLIENT_SAVE_GOLD_REQ request;
        memset(&request, 0, sizeof(request));
        
        request.cbActivityGame = SUB_GR_SAVE_SCORE_REQUEST;
        request.lSaveGold = score;
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_SAVE_SCORE_REQUEST, &request, sizeof(request));
    }
}

//取款
void BankScene::sendTakeScore(LONGLONG score, const std::string &pass)
{
    if(HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
    {
        CMD_GP_USER_TAKE_OUT_GOLD request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTakeScore = score;
        auto md5pass = MD5Encrypt(pass);
		Utf8ToUtf16(md5pass.c_str(), (WORD*)request.szPassword);
		Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine.c_str(), (WORD*)request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        CMD_GR_CLIENT_TAKE_OUT_GOLD_REQ request;
        memset(&request, 0, sizeof(request));
        
        request.cbAvtivityGame = SUB_GR_TAKE_SCORE_REQUEST;
        request.lTakeOutGold = score;
        auto md5pass = MD5Encrypt(pass);
		Utf8ToUtf16(md5pass.c_str(), (WORD*)request.szInsurePwd);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TAKE_SCORE_REQUEST, &request, sizeof(request));
    }
}

//转帐
void BankScene::sendTransferScore(LONGLONG score, const std::string &pass, int type, const std::string &nickname)
{
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    
	if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
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
        CMD_GP_USER_TRANSFER_GOLD request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lTransferScore = score;
        auto md5pass = MD5Encrypt(pass);
    
		Utf8ToUtf16(md5pass.c_str(), (WORD*)request.szPassword);
		Utf8ToUtf16(nickname.c_str(), (WORD*)request.szAccounts);
		Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine.c_str(), (WORD*)request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TRANSFER_SCORE, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
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
        
        CMD_GR_CLIENT_TRANSFER_GOLD_REQ request;
        memset(&request, 0, sizeof(request));
        
        request.cbActivityGame = SUB_GR_TRANSFER_SCORE_REQUEST;
        request.lTransferGold = score;

        auto md5pass = MD5Encrypt(pass);

		Utf8ToUtf16(md5pass.c_str(), (WORD*)request.szInsurePwd);
		Utf8ToUtf16(nickname.c_str(), (WORD*)request.szAccounts);
        NetworkMgr::getInstance()->sendData(MDM_GR_INSURE, SUB_GR_TRANSFER_SCORE_REQUEST, &request, sizeof(request));
    } 
}