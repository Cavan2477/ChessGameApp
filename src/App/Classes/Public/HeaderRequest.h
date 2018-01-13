/************************************************************************************
 * file: 		HeaderRequest.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:32:31
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

//头像请求框
#ifndef __HEADER_REQUEST_H__
#define __HEADER_REQUEST_H__

#include <stdio.h>
#include "../Common/MacroDefine.h"
#include "cocos2d.h"
#include "network/HttpClient.h"

class HeaderRequest : public cocos2d::Sprite
{
public:
    HeaderRequest();
    ~HeaderRequest();
    
    static HeaderRequest *createwithFaceID(WORD faceID, WORD customID, DWORD userID, BYTE gender);
    
    static HeaderRequest *createwithUrl(const std::string &url, DWORD userID);
    
    bool initwithFaceID(WORD faceID, WORD customID, DWORD userID, BYTE gender);
    
    bool initwithUrl(const std::string &url, DWORD userID);
    
    void setHeadSize(float width);
    
    void FaceRequest(WORD customID, DWORD userID);
    
    void FaceRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    //第三方头像请求
    void ThirdRequest(const std::string &url);
    
    void ThirdRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void enableTouch(bool enable,
                     std::function<bool(cocos2d::Touch *,cocos2d::Event *)> begin,
                     std::function<void(cocos2d::Touch *,cocos2d::Event *)> end);
    
    void addCusEvent();
    
private:
    void faceReuqcestEvent(cocos2d::EventCustom *event);
public:
    cocos2d::EventListenerTouchOneByOne *m_listener;
    std::function<bool(cocos2d::Touch *,cocos2d::Event *)> touchBegin;
    std::function<void(cocos2d::Touch *,cocos2d::Event *)> touchEnd;
    
private:
    float m_width;//头像大小
    
    DWORD m_userID;
    
};

#endif /* defined(__MyGame__HeaderRquest__) */
