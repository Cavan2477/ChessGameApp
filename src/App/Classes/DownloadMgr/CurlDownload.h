/************************************************************************************
 * file: 		CurlDown.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/16 21:38:21
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/16			
 *
 ************************************************************************************/

#ifndef __CURL_DOWNLOAD_H__
#define __CURL_DOWNLOAD_H__

#include "cocos2d.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../Common/PublicDefine.h"

using namespace cocos2d;
using namespace std;

USING_NS_CC;

typedef std::function<void(int,Ref*)> ErrorCallFunc;
typedef std::function<void(double , void *, string ,Ref*)> ProgressCallFunc;
typedef std::function<void(string,Ref*)> SuccessCallFunc;

class CurlDownload : public Ref
{
public:
    ~CurlDownload();
    CurlDownload();
    CurlDownload(string downUrl,string filePath);
    
    static CurlDownload *create(const std::string downUrl, const std::string filePath);

	void setErrorCallFunc(const ErrorCallFunc &error);
	void setProgressCallFunc(const ProgressCallFunc &progress);
	void setSuccessCallFunc(const SuccessCallFunc &success);
    
    bool download(); // 下载方法
    long getDownloadFileLenth(); // 下载文件大小方法
    void downloadControler(); // 下载控制方法
    long getLocalFileLength(); // 当前下载文件大小
    
    void setStopDown();// 停止下载
    
    typedef enum EM_ERR_CODE
    {
        // Error caused by creating a file to store downloaded data
        EM_ERR_CODE_CREATE_FILE,

        /** Error caused by network
         -- network unavaivable
         -- timeout
         -- ...
         */
        EM_ERR_CODE_NETWORK,

        /** There is not a new version
         */
        EM_ERR_CODE_NO_NEW_VERSION,

        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */
        EM_ERR_CODE_UNCOMPRESS,
    };
    
public:
    ErrorCallFunc       m_errorCodeFunc;
    ProgressCallFunc    m_progressCallFunc;
    SuccessCallFunc     m_successCallFunc;

	string		m_strFileName;					// 下载文件名称
	string		m_strFilePath;					// 本地存储地址
	string		m_strDownloadUrl;				// 下载URL

	bool		m_bIsStop;

	double		m_dFileLenth;					// 下载文件大小

	long		m_lDownloadTimeout;				// 请求超时时间 为了测试用 设置超时时间为2秒 如果是发正式版本 改为20秒超时时间

	EM_GAME		m_emGame;						// 游戏类型
};

#endif /* defined(__muli_download__CurlDown__) */
