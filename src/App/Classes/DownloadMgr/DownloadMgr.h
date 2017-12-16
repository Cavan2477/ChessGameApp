//
//  DownManager.hpp
//  MyGame
//
//  Created by Tang Miao on 12/18/15.
//
//

#ifndef DownManager_h
#define DownManager_h

#include <stdio.h>
#include "CurlDown.h"
#include "cocos2d.h"
#include "assets-manager/AssetsManagerEx.h"
USING_NS_CC;
using namespace std;

#define GAMECOUNT   7

typedef pair<string, string> Target;                    //下载源
typedef pair<Target, LIST_Kind> DownInfo;               //下载信息

class DownloadMgr : public Ref
{
public:
    DownloadMgr(void);
    virtual ~DownloadMgr();
    virtual bool init();
    
    static DownloadMgr* getInstance();
    
    //下载信息
public:
    bool createTaskforDown(const string targetUrl ,const string filePath,LIST_Kind kind = kind_default);
    bool getDownInfo(LIST_Kind eKind);
    bool setDownInfo(LIST_Kind eKind);
    
    //下载回调
     void onError(int errorCode, Ref *down);
     void onProgress(double percent, void *delegate, string filefullPath, Ref *down);
     void onSuccess(string filefullPath, Ref *down);
    
    
    void  update(float time);         //主线程更新ui
    

public:
    pthread_t               m_hDownThread;                  //线程句丙   
    vector<CurlDown *>         downQueue;                     //下载队列
    
    float                   m_fPercent;                     //下载进度
    
    LIST_Kind               m_eCurrenKind;                  //游戏类型
public:
      std::function<void(LIST_Kind)>    onTaskSuccess;
      std::function<void(LIST_Kind)>    ontaskError;
      std::function<void(LIST_Kind)>    onDecompressSuccess;
    
      bool    m_bDecompress;
    
     std::condition_variable_any _sleepCondition;   //信号量
     std::mutex _downQueueMutex;
    
};



#endif /* DownManager_h */
