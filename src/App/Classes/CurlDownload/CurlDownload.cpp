

#include "CurlDownload.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #include <curl/include/ios/curl/curl.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <curl/include/android/curl/curl.h>
#endif


static pthread_mutex_t g_downloadMutex_1;

CurlDownload::~CurlDownload(){
    m_dFileLenth = 0;
    
    errorCodeFunc = nullptr;
    progressCallFunc = nullptr;
    successCallFunc = nullptr;
    
}
CurlDownload::CurlDownload():
isStop(false),
m_strDownloadUrl(""),
m_lDownloadTimeout(20),
errorCodeFunc(nullptr),
progressCallFunc(nullptr),
successCallFunc(nullptr)
{ 
    m_dFileLenth = 0;
    m_strFilePath = "";
    pthread_mutex_init(&g_downloadMutex_1, NULL);
}

CurlDownload::CurlDownload(string downUrl,string filePath):
m_dFileLenth(0),
isStop(false),
m_strDownloadUrl(downUrl),
m_lDownloadTimeout(20),
m_strFilePath(filePath),
errorCodeFunc(nullptr),
progressCallFunc(nullptr),
successCallFunc(nullptr)
{
    pthread_mutex_init(&g_downloadMutex_1, NULL);
}

CurlDownload* CurlDownload::create(const std::string downUrl, const std::string filePath)
{
    CurlDownload* ret = new (std::nothrow) CurlDownload(downUrl, filePath);
    if (ret)
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


#pragma mark- 控制方法
void CurlDownload::downloadControler()
{

    // 获取远程文件大小
    m_dFileLenth = getDownloadFileLenth();
    if (m_dFileLenth <= 0)
    {
        cout << "-------------------------------download file fail..." << endl;
        if(errorCodeFunc == nullptr)
            return;
        
        errorCodeFunc(kNetwork,this);
        return;
    }
    vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, m_strFilePath);
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    log("--------------------------mFileLenth:%f",m_dFileLenth);
	mFileName = m_strDownloadUrl.substr(m_strDownloadUrl.rfind('/') + 1);

    
    log("mFileName:%s;",mFileName.c_str());

    m_strFilePath = m_strFilePath + mFileName;
    log("mFilePath:%s",m_strFilePath.c_str());
    
    bool ret = false;
    while (true)
    {
        // 循环下载 每30秒进行下载 避免断网情况
        ret = download(); //直接下载 进行堵塞线程
        log("----stop---%s------",isStop?"true":"false");
        if (isStop)
        { // 如果进行停止 break
            log("----stop---------");
            break;
        }
        if (ret )
        { //下载完成
            break;
        }
        sleep(0.5); //每次下载中间间隔0.5秒
    }
    
    if (ret)
    {
        log("download ok");
        if(successCallFunc == nullptr)
        {
            return;
        }
        
        successCallFunc(m_strFilePath,this);
       
    } else
    {
        log("download fail");
        if(errorCodeFunc == nullptr)
            return;
        
        errorCodeFunc(kUncompress,this);
    }
}

void CurlDownload::setStopDown()
{
    pthread_mutex_lock(&g_downloadMutex_1);
    isStop = true;
    pthread_mutex_unlock(&g_downloadMutex_1);
    log("----stop------%s------",isStop?"true":"false");
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
    if(!curlDown || curlDown->m_dFileLenth == 0 || nowDownloaded == 0) return 0;
    double nowDown = (curlDown->m_dFileLenth - totalToDownload + nowDownloaded);
    double curpercent = nowDown / curlDown->m_dFileLenth * 100;
    
    
    if(nullptr == curlDown->progressCallFunc)
        return 0;
    
    curlDown->progressCallFunc(curpercent,ptr, curlDown->m_strFilePath,curlDown);
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
    FILE *fp = fopen(m_strFilePath.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fclose(fp);
    return length;
}

#pragma mark 进行下载
bool CurlDownload::download() {
    FILE *fp = NULL;
    if(access(m_strFilePath.c_str(), 0)==0)
    {
        // 以二进制形式追加
        fp = fopen(m_strFilePath.c_str(), "ab+");
    } else
    {
        // 二进制写
        fp = fopen(m_strFilePath.c_str(), "wb");
    }
    
    if (fp == NULL)
    {
        // 如果文件初始化失败进行返回
        return false;
    }
    
    // 读取本地文件下载大小
    long localFileLenth = getLocalFileLength(); //已经下载的大小
    log("filePath:%s；leng:%ld",m_strFilePath.c_str() , localFileLenth );
    
    CURL *handle = curl_easy_init();
    std::string packageUrl = m_strDownloadUrl; //下载地址+下载文件名
    curl_easy_setopt(handle, CURLOPT_URL, packageUrl.c_str());
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, m_lDownloadTimeout);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_write_func);   //写文件回调方法
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp); // 写入文件对象
    curl_easy_setopt(handle, CURLOPT_RESUME_FROM, localFileLenth);  // 从本地大小位置进行请求数据
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, my_progress_func ); //下载进度回调方法
    curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, this); // 传入本类对象
    
    CURLcode res = curl_easy_perform(handle);
    fclose(fp);
    return res == CURLE_OK;
}

/* 得到远程文件的大小, 要下载的文件大小 */ // 参考的那个不对 获取不到大小
long CurlDownload::getDownloadFileLenth()
{
    //	double filesize = 0.0;
	CURL *handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, m_strDownloadUrl.c_str());
    log("%s",m_strDownloadUrl.c_str());
    /* No download if the file */
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);
    /* Ask for filetime */
    curl_easy_setopt(handle, CURLOPT_HEADER, 0L); // 0 不打印日志 1打印日志
    
	if (curl_easy_perform(handle) == CURLE_OK)
    {
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &m_dFileLenth);
        printf("filesize : %0.0f bytes\n", m_dFileLenth);
    } else
    {
		m_dFileLenth = -1;
	}
	return m_dFileLenth;
}

//设置回调
void CurlDownload::setErrorCallFunc(const ErrorCallFunc &error)
{
    
    errorCodeFunc = error;
}

void CurlDownload::setProgressCallFunc(const ProgressCallFunc &progress)
{
    
    progressCallFunc = progress;
}

void CurlDownload::setSuccessCallFunc(const SuccessCallFunc &success)
{
    
    successCallFunc = success;
    
}



