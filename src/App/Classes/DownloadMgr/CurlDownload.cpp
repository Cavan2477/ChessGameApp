/************************************************************************************
 * file: 		CurlDownload.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/17 19:32:03
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/17			
 *
 ************************************************************************************/

#include "CurlDownload.h"

// ios
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include <curl/include/ios/curl/curl.h>
// android
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <curl/include/android/curl/curl.h>
// win32
#else
	#include <curl/include/win32/curl/curl.h>
#endif

#include <bullet/BulletMultiThreaded/TrbStateVec.h>

static pthread_mutex_t g_downloadMutex_1;

CurlDownload::CurlDownload():
m_bStop(false),
m_strDownloadUrl(""),
m_lDownloadTimeout(20),
m_errorCodeFunc(nullptr),
m_progressCallFunc(nullptr),
m_successCallFunc(nullptr)
{ 
    m_dFileLenth = 0;
    m_strFilePath = "";

    pthread_mutex_init(&g_downloadMutex_1, NULL);
}

CurlDownload::CurlDownload(string downUrl,string filePath):
m_dFileLenth(0),
m_bStop(false),
m_strDownloadUrl(downUrl),
m_lDownloadTimeout(20),
m_strFilePath(filePath),
m_errorCodeFunc(nullptr),
m_progressCallFunc(nullptr),
m_successCallFunc(nullptr)
{
    pthread_mutex_init(&g_downloadMutex_1, NULL);
}

CurlDownload::~CurlDownload()
{
	m_dFileLenth = 0;

	m_errorCodeFunc = nullptr;
	m_progressCallFunc = nullptr;
	m_successCallFunc = nullptr;
}

CurlDownload* CurlDownload::create(const std::string downUrl, const std::string filePath)
{
    CurlDownload* ret = new (std::nothrow) CurlDownload(downUrl, filePath);

    if (ret)
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);

    return ret;
}

#pragma mark- 控制方法
void CurlDownload::downloadController()
{
    // 获取远程文件大小
    m_dFileLenth = getDownloadFileLenth();

    if (m_dFileLenth <= 0)
    {
        cout << "-------------------------------download file fail..." << endl;

        if(m_errorCodeFunc == nullptr)
            return;
        
        m_errorCodeFunc(EM_ERR_CODE_NETWORK,this);

        return;
    }

    vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();

    searchPaths.insert(iter, m_strFilePath);

    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    log("--------------------------mFileLenth:%f",m_dFileLenth);

	m_strFileName = m_strDownloadUrl.substr(m_strDownloadUrl.rfind('/') + 1);

    log("mFileName:%s;",m_strFileName.c_str());

    m_strFilePath = m_strFilePath + m_strFileName;

    log("mFilePath:%s",m_strFilePath.c_str());
    
    bool ret = false;

    while (true)
    {
        // 循环下载 每30秒进行下载 避免断网情况
        ret = download(); //直接下载 进行堵塞线程

        log("----stop---%s------",m_bStop?"true":"false");

		// 如果进行停止 break
        if (m_bStop)
        { 
            log("----stop---------");
            break;
        }

		//下载完成
        if (ret)
            break;

		// todo 2017/12/17 to be continue
		// sleep(0.5);
        Sleep(500);			//每次下载中间间隔0.5秒
    }
    
    if (ret)
    {
        log("download ok");

        if(m_successCallFunc == nullptr)
            return;
        
        m_successCallFunc(m_strFilePath,this);
       
    } 
	else
    {
        log("download fail");

        if(m_errorCodeFunc == nullptr)
            return;
        
        m_errorCodeFunc(EM_ERR_CODE_UNCOMPRESS,this);
    }
}

void CurlDownload::stopDownload()
{
    pthread_mutex_lock(&g_downloadMutex_1);
    m_bStop = true;
    pthread_mutex_unlock(&g_downloadMutex_1);

    log("----stop------%s------",m_bStop?"true":"false");
}

#pragma mark 进行文件写入本地回调函数
static size_t my_write_func(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

#pragma mark 下载进度函数 - 每次下载大小 不是总的大小
static int my_progress_func(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    CurlDownload* curlDown = (CurlDownload*)ptr;

    if(!curlDown || curlDown->m_dFileLenth == 0 || nowDownloaded == 0) 
		return 0;

    double dNowDownload = (curlDown->m_dFileLenth - totalToDownload + nowDownloaded);
    double dCurPercent = dNowDownload / curlDown->m_dFileLenth * 100;
    
    if(nullptr == curlDown->m_progressCallFunc)
        return 0;
    
    curlDown->m_progressCallFunc(dCurPercent,ptr, curlDown->m_strFilePath,curlDown);

    return 0;
}

//fopen函数调用如下：
//fopen（“文件名”，“使用文件方式”）；
//r：只读；
//w：只写；
//r+：允许读写；
//w+：允许读写；
//a：向文本文件末添加数据；
//a+：向文本文件末添加数据，允许读；
//rb：只读二进制文件；
//wb：只写二进制文件；
//rb+：只读二进制文件，允许写；
//wb+：只写二进制文件，允许读；
//ab：向二进制文件末添加数据；
//ab+：向二进制文件末添加数据，允许读；
long CurlDownload::getLocalFileLength()
{
    FILE *pFile = fopen(m_strFilePath.c_str(), "r");

	if (NULL == pFile)
		return -1;

    fseek(pFile, 0, SEEK_END);
    long length = ftell(pFile);

    fclose(pFile);

    return length;
}

#pragma mark 进行下载
bool CurlDownload::download() 
{
    FILE *pFile = NULL;

	// 以二进制形式追加
    if (access(m_strFilePath.c_str(), 0)==0)
        pFile = fopen(m_strFilePath.c_str(), "ab+");
	// 二进制写
	else
        pFile = fopen(m_strFilePath.c_str(), "wb");
    
	// 如果文件初始化失败进行返回
    if (pFile == NULL)
        return false;
    
    // 读取本地文件下载大小
    long lLocalFileLenth = getLocalFileLength(); //已经下载的大小

    log("filePath:%s；leng:%ld", m_strFilePath.c_str(), lLocalFileLenth);
    
    CURL *handle = curl_easy_init();

    std::string strPackageUrl = m_strDownloadUrl; //下载地址+下载文件名

    curl_easy_setopt(handle, CURLOPT_URL, strPackageUrl.c_str());

    curl_easy_setopt(handle, CURLOPT_TIMEOUT, m_lDownloadTimeout);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_write_func);			//写文件回调方法
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, pFile);						// 写入文件对象
    curl_easy_setopt(handle, CURLOPT_RESUME_FROM, lLocalFileLenth);			// 从本地大小位置进行请求数据
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, my_progress_func );	//下载进度回调方法
    curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, this);					// 传入本类对象
    
    CURLcode res = curl_easy_perform(handle);

    fclose(pFile);

    return res == CURLE_OK;
}

/* 得到远程文件的大小, 要下载的文件大小 */ // 参考的那个不对 获取不到大小
long CurlDownload::getDownloadFileLenth()
{
    //	double filesize = 0.0;
	CURL *handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, m_strDownloadUrl.c_str());
    log("%s",m_strDownloadUrl.c_str());
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);	/* No download if the file */
    curl_easy_setopt(handle, CURLOPT_HEADER, 0L);	// 0 不打印日志 1打印日志 Ask for filetime
    
	if (curl_easy_perform(handle) == CURLE_OK)
    {
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &m_dFileLenth);
        printf("filesize : %0.0f bytes\n", m_dFileLenth);
    } 
	else
		m_dFileLenth = -1;

	return m_dFileLenth;
}

//设置回调
void CurlDownload::setErrorCallFunc(const ErrorCallFunc &error)
{
    m_errorCodeFunc = error;
}

void CurlDownload::setProgressCallFunc(const ProgressCallFunc &progress)
{
    m_progressCallFunc = progress;
}

void CurlDownload::setSuccessCallFunc(const SuccessCallFunc &success)
{
    m_successCallFunc = success;
}