/************************************************************************************
 * file: 		PhotoBridge.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:03:12
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __PhotoBridge_h__
#define __PhotoBridge_h__

#include <stdio.h>
#include "cocos2d.h"

enum ChoiceType
{
    Type_Head = 0,//头像选择
    
    Type_Image,// 图片选择
};

typedef std::function<void(cocos2d::Image *pimage)> PhotoCallback; // 获取图片回调

class PhotoDelegate
{
public:
    virtual void choiceComplete(cocos2d::Image *pimage)=0;
};

class PhotoBridge : public cocos2d::Node, public PhotoDelegate
{
public:
    PhotoBridge();
    
    ~PhotoBridge();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    //android相册图片选择完成
    void notifyAndroidPhotoImage(cocos2d::EventCustom *event);
    
    void setChoiceType(int type);
    
    void openPhoto();
    
    void choiceComplete(cocos2d::Image *pimage);
    
    void *pickerImage;
    
    ChoiceType m_type;
    
    PhotoCallback m_completecallback;
};

#endif
