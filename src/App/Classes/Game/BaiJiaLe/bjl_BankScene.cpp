//
//  BankScene.cpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#include "BaiJiaLeScene.h"
USING_NS_CC;
USING_BJL_NAMESPACE;
BankScene::BankScene()
{
    
}

BankScene::~BankScene()
{
    //资源释放
    INSTANCE(SceneMgr)->doUISceneResourceRelease(BankScene::uiUnloadResource());
}

std::vector<tagUIResourceParam> BankScene::uiAsynLoadResource()
{
    std::vector<tagUIResourceParam> vec = std::vector<tagUIResourceParam>();
    tagUIResourceParam param = {};
    param.resType = tagUIResourceParam::UIResourceType::kTEXTURE_RES;
    param.resPath = "bank/bank.png";
    param.plistFile = "bank/bank.plist";
    param.texType = ui::Widget::TextureResType::PLIST;
    vec.push_back(param);
    
    return vec;
}

std::vector<tagUIResourceParam> BankScene::uiAsynLoadFinished()
{
    return BankScene::uiAsynLoadResource();
}

std::vector<tagUIResourceParam> BankScene::uiUnloadResource()
{
    return BankScene::uiAsynLoadResource();
}

bool BankScene::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Scene::init());
        
        m_bankLayer = BankLayer::create();
        this->addChild(m_bankLayer);
        
        bRes = true;
    } while (false);
    return bRes;
}

void BankScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load)
    {
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_ENABLE_RESULT, CC_CALLBACK_2(BankScene::InsureEnableResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_INFO, CC_CALLBACK_2(BankScene::InsureInfoResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_SUCCESS, CC_CALLBACK_2(BankScene::BankSuccedResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_INSURE_FAILURE, CC_CALLBACK_2(BankScene::BankFailureResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_QUERY_USER_INFO_RESULT, CC_CALLBACK_2(BankScene::BankUserInfoResult, this));
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        NetworkMgr::getInstance()->registerroomfunction(MDM_GR_INSURE, CC_CALLBACK_3(BankScene::BankInsureResult, this));
    }
}

void BankScene::onExit()
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
    Scene::onExit();
}

bool BankScene::uiMsg(const int &main, const int &sub, void *pData, const int &nLen)
{
    return true;
}

void BankScene::InsureEnableResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureEnableResult *)pData;
    if (result->cbInsureEnabled == 1)
    {
        HallDataMgr::getInstance()->m_cbInsureEnable = 1;
        
        m_bankLayer->openBankSuccess();
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

void BankScene::InsureInfoResult(void *pData, WORD wSize)
{
    if (wSize != sizeof(CMD_GP_UserInsureInfo))
    {
        return;
    }
    
    m_bankLayer->onGetBankInsureInfo(pData, wSize);
    
    auto action = CallFunc::create([]
                                   {
                                       NetworkMgr::getInstance()->Disconnect(Data_Load);
                                   });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
    
    m_bankLayer->updateScore();
}

void BankScene::BankSuccedResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserInsureSuccess *)pData;
    HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
    HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
    m_bankLayer->updateScore();
    
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

void BankScene::BankFailureResult(void *pData, WORD wSize)
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

void BankScene::BankUserInfoResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserTransferUserInfo *)pData;
    auto nickname = WHConverUnicodeToUtf8WithArray(result->szAccounts);
    m_bankLayer->onGetBankUserInfo(nickname);
}

void BankScene::BankInsureResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID)
    {
        case SUB_GR_USER_INSURE_ENABLE_RESULT:
        {
            auto result = (CMD_GR_S_UserInsureEnableResult *)pData;
            if (result->cbInsureEnabled == 1)
            {
                HallDataMgr::getInstance()->m_cbInsureEnable = 1;
                
                m_bankLayer->openBankSuccess();
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
            /*
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
             */
            m_bankLayer->onGetBankInsureInfo(pData, wSize);
            
            m_bankLayer->updateScore();
        }
            break;
        case SUB_GR_USER_INSURE_SUCCESS:
        {
            auto result = (CMD_GR_S_UserInsureSuccess *)pData;
            HallDataMgr::getInstance()->m_UserInsure = result->lUserInsure;
            HallDataMgr::getInstance()->m_UserScore = result->lUserScore;
            m_bankLayer->updateScore();
            
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
            m_bankLayer->onGetBankUserInfo(nickname);
        }
            break;
            
        default:
            break;
    }
}