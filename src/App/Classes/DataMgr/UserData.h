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
        auto pdescribe = (tagDataDescribe *)((char *)pdata+sizeof(tagMobileUserInfoHead));
        if (pdescribe->wDataDecribe == DTP_GR_NICK_NAME) {
            TCHAR nickname[LEN_NICKNAME];
            memset(nickname, 0, sizeof(nickname));
            memcpy(nickname,(char *)pdata+sizeof(tagMobileUserInfoHead)+sizeof(tagDataDescribe), pdescribe->wDataSize);
            m_nickname = WHConverUnicodeToUtf8WithArray(nickname);
        }
    }
    
    void UpdateData(CMD_GR_UserStatus *pdata)
    {
        m_date.wTableID = pdata->UserStatus.wTableID;
        m_date.wChairID = pdata->UserStatus.wChairID;
        m_date.cbUserStatus = pdata->UserStatus.cbUserStatus;
    }
    
    void UpdateData(CMD_GR_MobileUserScore *pdata)
    {
        m_date.lScore = pdata->UserScore.lScore;
        m_date.dBean = pdata->UserScore.dBean;
        m_date.dwWinCount = pdata->UserScore.dwWinCount;
        m_date.dwLostCount = pdata->UserScore.dwLostCount;
        m_date.dwDrawCount = pdata->UserScore.dwDrawCount;
        m_date.dwLostCount = pdata->UserScore.dwLostCount;
        m_date.dwFleeCount = pdata->UserScore.dwFleeCount;
        m_date.dwExperience = pdata->UserScore.dwExperience;
    }
    
    tagMobileUserInfoHead  m_date;
    
    std::string m_nickname;
    
};

#endif /* defined(__MyGame__UserData__) */
