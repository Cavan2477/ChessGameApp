/************************************************************************************
 * file: 		DownloadMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/16 22:20:09
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/16			
 *
 ************************************************************************************/

#include "DownloadMgr.h"
#include "../DataMgr/HallDataMgr.h"
#include "../Common/CMD_Stdafx.h"

USING_NS_CC_EXT;

static DownloadMgr *s_pDownloadMgr = nullptr;

static pthread_mutex_t g_downloadMutex;

//线程接收函数
void *threadTask(void* param)
{
    DownloadMgr* pDownloadMgr = (DownloadMgr*)param;
    
    while (true)
    {
        if (pDownloadMgr->m_vecCurlDownloadQueue.size() == 0)
            pDownloadMgr->_sleepCondition.wait(pDownloadMgr->_downQueueMutex);
        
        pthread_mutex_lock(&g_downloadMutex);
        auto iter = pDownloadMgr->m_vecCurlDownloadQueue.begin();
        CurlDownload *curl = *iter;
        pDownloadMgr->m_vecCurlDownloadQueue.erase(iter);
        pthread_mutex_unlock(&g_downloadMutex);
        
        curl->setErrorCallFunc(CC_CALLBACK_2(DownloadMgr::onError, pDownloadMgr));
        curl->setSuccessCallFunc(CC_CALLBACK_2(DownloadMgr::onSuccess, pDownloadMgr));
        curl->setProgressCallFunc(CC_CALLBACK_4(DownloadMgr::onProgress, pDownloadMgr));
        curl->downloadControler();
        curl->release();
        
        //sleep(5.0f);
		Sleep(500);
    }

    return 0;
}


DownloadMgr::DownloadMgr():m_fPercent(0.0f),m_bDecompress(false)
{
    pthread_mutex_init(&g_downloadMutex, NULL);

    memset(&m_hThreadDownload, 0, sizeof(pthread_t));
    
    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(::DownloadMgr::update), this, 0, false);
}

DownloadMgr::~DownloadMgr()
{
    pthread_mutex_destroy(&g_downloadMutex);

    if (m_hThreadDownload.p) 
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        pthread_cancel(m_hThreadDownload);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        pthread_kill(m_hThreadDownload,0);
#endif
        m_hThreadDownload.p = 0;
		m_hThreadDownload.x = 0;
    }

    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(::DownloadMgr::update), this);
    
}

DownloadMgr *DownloadMgr::getInstance()
{
    
    if (!s_pDownloadMgr)
    {
        s_pDownloadMgr = new (std::nothrow) DownloadMgr();
        CCASSERT(s_pDownloadMgr, "FATAL: Not enough memory");
        s_pDownloadMgr->init();
 
    }
    
    return s_pDownloadMgr;
}

// 初始化
bool DownloadMgr::init()
{
    m_fPercent = 0.0f;
	m_emGameCurrent = EM_GAME_DEFALUT;
    
    //创建线程
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    int code = pthread_create(&m_hThreadDownload, 0, threadTask, this);
    
    if(code!=0)
    {
        CCLOG("线程创建失败");
        return false;
    }
    
    //4/4 join
    pthread_detach(m_hThreadDownload);

    return true;
}

bool DownloadMgr::createTaskforDown(const string strTargetUrl, const string strFilePath, EM_GAME emGame /*= Game_None*/)
{
    for (auto curl : m_vecCurlDownloadQueue)
    {
		if (strTargetUrl.compare(curl->m_strDownloadUrl) == 0)
		{
			//DebugLog("资源已存在于队列中");
			return false;
		}
    }
    
    CurlDownload *curlDownload = CurlDownload::create(strTargetUrl, strFilePath);

    curlDownload->m_emGame = emGame;
    curlDownload->retain();
    
    pthread_mutex_lock(&g_downloadMutex);
    m_vecCurlDownloadQueue.push_back(curlDownload);
    pthread_mutex_unlock(&g_downloadMutex);

    //通知下载线程，处于下载状态
    _sleepCondition.notify_one();
    
    return true;
}

bool DownloadMgr::getDownInfo(EM_GAME eKind)
{
    UserDefault *downloadDefault = UserDefault::getInstance();
     __String *str = __String::createWithFormat("GAME_%d",eKind);
    Data data = downloadDefault->getDataForKey(str->getCString());
    if(nullptr != data.getBytes())
    {
		ST_DOWNLOAD_INFO stDownloadInfo;

		memset(&stDownloadInfo, 0, sizeof(ST_DOWNLOAD_INFO));
		memcpy(&stDownloadInfo, data.getBytes(), sizeof(ST_DOWNLOAD_INFO));

        return true;
    }
    
    return false;

}

bool DownloadMgr::setDownInfo(EM_GAME emGame)
{
    //写入下载记录
	ST_DOWNLOAD_INFO stDownloadInfo;

    stDownloadInfo.emGame			= emGame;
    stDownloadInfo.emDownloadStatus = EM_DOWNLOAD_DONE;
    
    UserDefault *downloadDefault = UserDefault::getInstance();

    __String *str = __String::createWithFormat("GAME_%d", stDownloadInfo.emGame);

    Data *pData = new Data;

    BYTE dataBuffer[256];

    memset(&dataBuffer, 0, sizeof(BYTE)*256);
	memcpy(dataBuffer, &stDownloadInfo, sizeof(ST_DOWNLOAD_INFO));
    
	pData->copy(dataBuffer, sizeof(ST_DOWNLOAD_INFO));

    downloadDefault->setDataForKey(str->getCString(), *pData);
    downloadDefault->flush();
    
	if (nullptr != pData)
	{
		delete pData;
		pData = nullptr;
	}

    return true;
}

void DownloadMgr::onError(int errorCode,Ref *down)
{

    DebugLog("当前资源下载失败");
}

void DownloadMgr::onProgress(double percent, void *delegate, string filefullPath,Ref *down)
{
    CurlDownload *curlDownload = dynamic_cast<CurlDownload *>(down);
    
    m_fPercent = percent;
	m_emGameCurrent = curlDownload->m_emGame;
}

void DownloadMgr::onSuccess(string filefullPath,Ref *down)
{
    DebugLog("下载完成");

    CurlDownload *curl = dynamic_cast<CurlDownload *>(down);
    
    string downUrl =curl->m_strDownloadUrl;
    EM_GAME emGame = curl->m_emGame;

    this->setDownInfo(emGame);

    string fileName = downUrl.substr(downUrl.rfind('/') + 1);
    string zipFile = FileUtils::getInstance()->getWritablePath() + fileName;

	AssetsManagerEx *pAssetsMgrEx = AssetsManagerEx::create("", "");
    pAssetsMgrEx->retain();

	// todo 2017/12/17 to be continue 未解压
	/*if (!pAssetsMgrEx->customDecompress(zipFile))
	{
		DebugLog("解压失败");
	}*/

    DebugLog("解压成功");

	pAssetsMgrEx->release();

    //解压成功
    m_bDecompress = true;

    if (FileUtils::getInstance()->removeFile(zipFile))
    {
        DebugLog("删除zip成功");
    }else
    {
        
        DebugLog("删除zip失败");
    }

}

void DownloadMgr::update(float time)
{
    if (m_bDecompress)
    {
        
        onDecompressSuccess(m_emGameCurrent);
        
        m_bDecompress = false;
        return;
    }
    
    if (m_fPercent == 0.0f)
        return;
    
    //进度更新
    EventCustom event(whNd_Download_Update);
	ST_DOWNLOAD_REFRESH stDownloadRefresh;

	memset(&stDownloadRefresh, 0, sizeof(ST_DOWNLOAD_REFRESH));

    stDownloadRefresh.emGame = this->m_emGameCurrent;
    stDownloadRefresh.dDownloadPercent = this->m_fPercent;

    event.setUserData(&stDownloadRefresh);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    int nPercent = m_fPercent;

    if (nPercent == 100)
    {
        onTaskSuccess(m_emGameCurrent);
        m_fPercent = 0.0f;
    }
}

