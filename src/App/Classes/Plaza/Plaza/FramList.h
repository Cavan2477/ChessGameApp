/************************************************************************************
 * file: 		FramList.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 23:10:20
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef __FramList__
#define __FramList__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

#define MAX_LISTCOUNT			3   //最大纪录数

typedef unsigned char           UInt8;
typedef UInt8                   Byte;

#pragma pack(1)

struct Account
{
    Account()
    {
        memset(accounts, 0, sizeof(accounts));
        memset(password, 0, sizeof(password));
    }
    
    void clear()
    {
        memset(accounts, 0, sizeof(accounts));
        memset(password, 0, sizeof(password));
    }
    
	char     accounts[33];
	char     password[33];
};

static inline Account AccountZero()
{
    Account aAccount;
    memset(&aAccount, 0, sizeof(aAccount));
    return aAccount;
}

struct AccList
{
    Account acc[5];
};


class CFramList
{
public:
    
    //创建实例
    static CFramList* getInstance();
    
    //构造对象
    CFramList();
    virtual ~CFramList();
    
    //wb+读写打开或创建 二进制文件
    void saveAccountList();
    
    //rw+读写打开 二进制文件
    void readAccountList();
    
    std::string getFilePaht();
    
    void addAccountListWithString(const std::string& strAcc,const std::string& strPW);
    
	//相同纪录 
    void searchSameAccountList(const std::string& accounts);
    
	//插入纪录
    void  insertAccountList(Account newList);
    
    //删除纪录
    void  removeAccountListWithIndex(int index);
    
    //指定获取
    Account getAccountListWithIndex(int index);
    
    int AccountListCount();
    
    void AccountEncrypt(void *data ,int nDataSize);
    void AccountDecrypt(void *data ,int nDataSize);

public:
    AccList     m_pAccList;
};

#endif /* defined(__cocosProject__FramList__) */
