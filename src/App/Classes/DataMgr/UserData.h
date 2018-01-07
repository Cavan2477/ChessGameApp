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

#ifndef __UserData_h__
#define __UserData_h__

#include <stdio.h>
#include "../Common/Project.h"
#include "../Common/Struct.h"

class UserData : public cocos2d::Ref
{
public:
    UserData(void *pdata)
    {
        memset(&m_date, 0, sizeof(m_date));
        UpdateData(pdata);
    };

    ~UserData(){}
    
    void UpdateData(void *pdata)
    {
        memcpy(&m_date, pdata, sizeof(m_date));
        auto pdescribe = (_stUserDataExt *)((char *)pdata+sizeof(_stMobileUserHeadInfo));
        if (pdescribe->wDataDesc == DTP_GR_NICK_NAME) {
            TCHAR nickname[LEN_NICKNAME];
            memset(nickname, 0, sizeof(nickname));
            memcpy(nickname,(char *)pdata+sizeof(_stMobileUserHeadInfo)+sizeof(_stUserDataExt), pdescribe->wDataSize);

			// todo 2017/12/17 to be continue
            //m_nickname = WHConverUnicodeToUtf8WithArray((WORD)nickname);
        }
    }
    
    void UpdateData(ST_CMD_GR_USER_STATUS *pdata)
    {
        m_date.wTableID = pdata->stUserStatus.wTableID;
        m_date.wChairID = pdata->stUserStatus.wChairID;
        m_date.cbUserStatus = pdata->stUserStatus.cbUserStatus;
    }
    
    void UpdateData(ST_CMD_GR_MOBILE_USER_GAME_COIN *pdata)
    {
        m_date.llGameCoin = pdata->stMobileUserGameCoin.llGameCoin;
        m_date.dBean = pdata->stMobileUserGameCoin.dBean;
        m_date.dwWinCount = pdata->stMobileUserGameCoin.dwWinCount;
        m_date.dwLostCount = pdata->stMobileUserGameCoin.dwLostCount;
        m_date.dwDrawCount = pdata->stMobileUserGameCoin.dwDrawCount;
        m_date.dwLostCount = pdata->stMobileUserGameCoin.dwLostCount;
        m_date.dwEscapeCount = pdata->stMobileUserGameCoin.dwEscapeCount;
        m_date.dwExp = pdata->stMobileUserGameCoin.dwExp;
    }
    
    _stMobileUserHeadInfo  m_date;
    
    std::string m_nickname;
    
};

#endif /* defined(__MyGame__UserData__) */
