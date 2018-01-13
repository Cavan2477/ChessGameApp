/************************************************************************************
 * file: 		DownloadMgr.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/17 21:46:04
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/17			
 *
 ************************************************************************************/

#ifndef __DOWNLOAD_MGR_H__
#define __DOWNLOAD_MGR_H__

#include <stdio.h>
#include "cocos2d.h"
#include "assets-manager/AssetsManagerEx.h"
#include "CurlDownload.h"

USING_NS_CC;
using namespace std;

#define GAMECOUNT   7

typedef pair<string, string> Target;                    //下载源
typedef pair<Target, EM_GAME> DownInfo;               //下载信息

class DownloadMgr : public Ref
{
public:
    DownloadMgr(void);
    virtual ~DownloadMgr();

	static DownloadMgr* getInstance();

    virtual bool init();
    
    //下载信息
public:
	bool createTaskforDown(const string targetUrl, const string filePath, EM_GAME emGame = EM_GAME_DEFALUT);
    bool getDownInfo(EM_GAME eKind);
    bool setDownInfo(EM_GAME eKind);
    
    //下载回调
    void onError(int errorCode, Ref *down);
    void onProgress(double percent, void *delegate, string filefullPath, Ref *down);
    void onSuccess(string filefullPath, Ref *down);
    
    void  update(float time);         //主线程更新ui
   
public:
    pthread_t               m_hThreadDownload;              //下载线程句柄  
    vector<CurlDownload *>  m_vecCurlDownloadQueue;			//下载队列
    
    float                   m_fPercent;                     //下载进度
    
    EM_GAME					m_emGameCurr;				//当前游戏
public:
    std::function<void(EM_GAME)>    onTaskSuccess;
    std::function<void(EM_GAME)>    ontaskError;
    std::function<void(EM_GAME)>    onDecompressSuccess;
    
    bool    m_bDecompress;
    
    std::condition_variable_any _sleepCondition;			//信号量
    std::mutex _downQueueMutex;
};

#endif /* DownManager_h */
