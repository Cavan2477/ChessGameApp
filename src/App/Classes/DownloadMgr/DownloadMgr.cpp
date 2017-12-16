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
#include "HallDataMgr.h"
#include "CMD_Stdafx.h"

USING_NS_CC_EXT;

static DownloadMgr *s_shareDownManager = nullptr;

static pthread_mutex_t g_downloadMutex;

//线程接收函数
void *threadTask(void* param)
{
    DownloadMgr *down = (DownloadMgr*)param;
    
    while (true)
    {
        if (down->downQueue.size() == 0)
        {
            down->_sleepCondition.wait(down->_downQueueMutex);
        }
        
        pthread_mutex_lock(&g_downloadMutex);
        auto iter = down->downQueue.begin();
        CurlDown *curl = *iter;
        down->downQueue.erase(iter);
        pthread_mutex_unlock(&g_downloadMutex);
        
        curl->setErrorCallFunc(CC_CALLBACK_2(DownloadMgr::onError, down));
        curl->setSuccessCallFunc(CC_CALLBACK_2(DownloadMgr::onSuccess, down));
        curl->setProgressCallFunc(CC_CALLBACK_4(DownloadMgr::onProgress, down));
        curl->downloadControler();
        curl->release();
        
        sleep(5.0f);
    }

    
    return 0;
}


DownloadMgr::DownloadMgr():m_fPercent(0.0f),m_bDecompress(false)
{
    pthread_mutex_init(&g_downloadMutex, NULL);
    memset(&m_hDownThread, 0, sizeof(pthread_t));
    
    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(::DownloadMgr::update), this, 0, false);
    
}
DownloadMgr::~DownloadMgr()
{
    
    pthread_mutex_destroy(&g_downloadMutex);
    if (m_hDownThread) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        pthread_cancel(m_hDownThread);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        pthread_kill(m_hDownThread,0);
#endif
        m_hDownThread = 0;
    }

    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(::DownloadMgr::update), this);
    
}

DownloadMgr *DownloadMgr::getInstance()
{
    
    if (!s_shareDownManager)
    {
        s_shareDownManager = new (std::nothrow) DownloadMgr();
        CCASSERT(s_shareDownManager, "FATAL: Not enough memory");
        s_shareDownManager->init();
 
    }
    
    return s_shareDownManager;
}


bool DownloadMgr::init()
{
    
    m_fPercent = 0.0f;
    m_eCurrenKind = kind_default;
    
    //创建线程
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    int code = pthread_create(&m_hDownThread, 0, threadTask, this);
    
    if(code!=0)
    {
        CCLOG("线程创建失败");
        
        return false;
    }
    
    //4/4 join
    pthread_detach(m_hDownThread);
    
    
    return true;
}

bool DownloadMgr::createTaskforDown(const string targetUrl, const string filePath,LIST_Kind kind /*= Game_None*/)
{
    
    for (auto  curl :downQueue)
    {
        if (targetUrl.compare(curl->mDownloadUrl) == 0)
        {
            DebugLog("资源已存在于队列中");
            return false;
        }
    }
    
    CurlDown *curldown = CurlDown::create(targetUrl, filePath);
    curldown->m_ekind = kind;
    curldown->retain();
    
    pthread_mutex_lock(&g_downloadMutex);
    downQueue.push_back(curldown);
    pthread_mutex_unlock(&g_downloadMutex);

     //通知下载线程，处于下载状态
    _sleepCondition.notify_one();
    
    return true;
}

bool DownloadMgr::getDownInfo(LIST_Kind eKind)
{
    UserDefault *downloadDefault = UserDefault::getInstance();
     __String *str = __String::createWithFormat("GAME_%d",eKind);
    Data data = downloadDefault->getDataForKey(str->getCString());
    if(nullptr != data.getBytes())
    {
        DownLoadInfo info;
        memset(&info, 0, sizeof(DownLoadInfo));
        memcpy(&info, data.getBytes(), sizeof(DownLoadInfo));
        return true;
    }
    
    return false;

}

bool DownloadMgr::setDownInfo(LIST_Kind eKind)
{
    
    //写入下载记录
    DownLoadInfo info;
    info.kindID  = eKind;
    info.status  = DownLoadDone;
    
    UserDefault *downloadDefault = UserDefault::getInstance();
    __String *str = __String::createWithFormat("GAME_%d",info.kindID);
    Data *data = new Data;
    BYTE dataBuffer[256];
    memset(&dataBuffer, 0, sizeof(BYTE)*256);
    memcpy(dataBuffer, &info, sizeof(DownLoadInfo));
    
    data->copy(dataBuffer, sizeof(DownLoadInfo));
    downloadDefault->setDataForKey(str->getCString(), *data);
    downloadDefault->flush();
    
    delete data;
    return true;
}

void DownloadMgr::onError(int errorCode,Ref *down)
{

    DebugLog("当前资源下载失败");
}

void DownloadMgr::onProgress(double percent, void *delegate, string filefullPath,Ref *down)
{
    CurlDown *curl = dynamic_cast<CurlDown *>(down);
    
    m_fPercent = percent;
    m_eCurrenKind = curl->m_ekind;
}

void DownloadMgr::onSuccess(string filefullPath,Ref *down)
{
    DebugLog("下载完成");

    CurlDown *curl = dynamic_cast<CurlDown *>(down);
    
    string downUrl =curl->mDownloadUrl;
    LIST_Kind kind = curl->m_ekind;
    this->setDownInfo(kind);

    
    string fileName = downUrl.substr(downUrl.rfind('/') + 1);
    string zipFile = FileUtils::getInstance()->getWritablePath() + fileName;

    AssetsManagerEx *assets = AssetsManagerEx::create("", "");
    assets->retain();
    if (!assets->customDecompress(zipFile))
    {
        DebugLog("解压失败");
    }
    DebugLog("解压成功");
    assets->release();
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
        
        onDecompressSuccess(m_eCurrenKind);
        
        m_bDecompress = false;
        return;
    }
    
    if (m_fPercent == 0.0f)
        return;
    
    //进度更新
    EventCustom event(whNd_Download_Update);
    DownRefresh refresh;
    memset(&refresh, 0, sizeof(DownRefresh));
    refresh.eKind = this->m_eCurrenKind;
    refresh.dPercent = this->m_fPercent;
    event.setUserData(&refresh);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    int nPercent = m_fPercent;
    if (nPercent == 100)
    {
        onTaskSuccess(m_eCurrenKind);
        m_fPercent = 0.0f;
    }
        
    
}

