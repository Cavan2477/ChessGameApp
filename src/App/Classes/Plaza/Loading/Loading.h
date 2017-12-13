/************************************************************************************
 * file: 		Loading.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:21:36
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __Loading_h__
#define __Loading_h__

#include "cocos2d.h"
#include <iostream>
#include "../../Common/MacroDefine.h"

using namespace cocos2d;
using namespace std;

typedef std::function<void(Ref*)> FinishCallFunc;

enum LOADING_Tag
{
    Tag_Bottom_Layout = 10,
    Tag_Loading_Pro,//登录进度条
};

class Loading : public Layer
{
  public:
    virtual bool init();
    CREATE_FUNC(Loading);
    
    Loading();
    virtual ~Loading();
    virtual void onEnterTransitionDidFinish();
    
//    virtual void onExit();
public:
    bool onTouchBegan(Touch *touch, Event *event);
    void runFinish();
    
public:
    void  loadingFinish();
    void  setFinishCallFunc(const FinishCallFunc &func);
    
public:
    FinishCallFunc   finishCallFunc;
    
    
    
};

#endif /* Loading_hpp */
