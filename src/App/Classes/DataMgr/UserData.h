/************************************************************************************
 * file: 		UserData.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:44:47
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

//用户数据

#ifndef __USER_DATA_H__
#define __USER_DATA_H__

#include <stdio.h>
#include "../Common/Project.h"
#include "../Common/Struct.h"

class UserData : public cocos2d::Ref
{
public:
    UserData(void *pdata)
    {
        memset(&m_stMobileUserHeadInfo, 0, sizeof(m_stMobileUserHeadInfo));
        UpdateData(pdata);
    };

    ~UserData(){}
    
    void UpdateData(void *pData)
    {
        memcpy(&m_stMobileUserHeadInfo, pData, sizeof(m_stMobileUserHeadInfo));
        auto pdescribe = (_stUserDataExt *)((char *)pData+sizeof(_stMobileUserHeadInfo));

        if (pdescribe->wDataDesc == DTP_GR_NICK_NAME) 
		{
            TCHAR nickname[LEN_MAX_NICKNAME];
            memset(nickname, 0, sizeof(nickname));
            memcpy(nickname,(char *)pData+sizeof(_stMobileUserHeadInfo)+sizeof(_stUserDataExt), pdescribe->wDataSize);

			// todo 2017/12/17 to be continue
            //m_nickname = WHConverUnicodeToUtf8WithArray((WORD)nickname);
        }
    }
    
    void UpdateData(ST_CMD_GR_USER_STATUS *pInfo)
    {
        m_stMobileUserHeadInfo.wTableID		= pInfo->stUserStatus.wTableID;
        m_stMobileUserHeadInfo.wChairID		= pInfo->stUserStatus.wChairID;
        m_stMobileUserHeadInfo.cbUserStatus = pInfo->stUserStatus.cbUserStatus;
    }
    
    void UpdateData(ST_CMD_GR_MOBILE_USER_GAME_COIN *pInfo)
    {
        m_stMobileUserHeadInfo.llGameCoin	= pInfo->stMobileUserGameCoin.llGameCoin;
        m_stMobileUserHeadInfo.dBean		= pInfo->stMobileUserGameCoin.dBean;
        m_stMobileUserHeadInfo.dwWinCount	= pInfo->stMobileUserGameCoin.dwWinCount;
        m_stMobileUserHeadInfo.dwLostCount	= pInfo->stMobileUserGameCoin.dwLostCount;
        m_stMobileUserHeadInfo.dwDrawCount	= pInfo->stMobileUserGameCoin.dwDrawCount;
        m_stMobileUserHeadInfo.dwLostCount	= pInfo->stMobileUserGameCoin.dwLostCount;
        m_stMobileUserHeadInfo.dwEscapeCount= pInfo->stMobileUserGameCoin.dwEscapeCount;
        m_stMobileUserHeadInfo.dwExp		= pInfo->stMobileUserGameCoin.dwExp;
    }
    
    _stMobileUserHeadInfo  m_stMobileUserHeadInfo;
    
    std::string m_strNickname;
};

#endif /* defined(__MyGame__UserData__) */
