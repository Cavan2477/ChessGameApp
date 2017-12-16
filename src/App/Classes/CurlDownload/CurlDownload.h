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

#ifndef __download__CurlDown__
#define __download__CurlDown__

#include <iostream>
#include "cocos2d.h"
#include <stdio.h>
#include <unistd.h>
#include "../Common/PublicDefine.h"

using namespace cocos2d;
using namespace std;

USING_NS_CC;

typedef std::function<void(int,Ref*)> ErrorCallFunc;
typedef std::function<void(double , void *, string ,Ref*)> ProgressCallFunc;
typedef std::function<void(string,Ref*)> SuccessCallFunc;

class CurlDownload : public Ref
{
private:
    string mFileName; // 下载文件名称
    bool isStop;

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
    
    enum ErrorCode
    {
        // Error caused by creating a file to store downloaded data
        kCreateFile,
        /** Error caused by network
         -- network unavaivable
         -- timeout
         -- ...
         */
        kNetwork,
        /** There is not a new version
         */
        kNoNewVersion,
        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */
        kUncompress,
    };
    
 public:
    ErrorCallFunc       errorCodeFunc;
    ProgressCallFunc    progressCallFunc;
    SuccessCallFunc     successCallFunc;

	string		m_strFilePath;					// 本地存储地址
	string		m_strDownloadUrl;				// 下载URL
	double		m_dFileLenth;					// 下载文件大小
	long		m_lDownloadTimeout;				// 请求超时时间 为了测试用 设置超时时间为2秒 如果是发正式版本 改为20秒超时时间
	ENUM_GAME   m_listKind;						// 游戏类型
};

#endif /* defined(__muli_download__CurlDown__) */
