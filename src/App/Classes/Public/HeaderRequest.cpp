/************************************************************************************
 * file: 		HeaderRequest.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 22:33:39
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "HeaderRequest.h"
#include "../Common/Project.h"
#include "../DataMgr/HallDataMgr.h"
#include "../DataMgr/NetworkMgr.h"

USING_NS_CC;

using namespace ui;
using namespace cocos2d::network;

const std::string FACE_REQUEST_EVENT = "wHNd_FaceRequest";

HeaderRequest::HeaderRequest():
m_listener(nullptr)
{
}

HeaderRequest::~HeaderRequest()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(FACE_REQUEST_EVENT);
    
    if (nullptr != m_listener)
    {
        _eventDispatcher->removeEventListener(m_listener);
        CC_SAFE_RELEASE_NULL(m_listener);
    }
}

HeaderRequest * HeaderRequest::createwithFaceID(WORD faceID, WORD customID, DWORD userID, BYTE gender)
{
    auto prequest = new HeaderRequest;
    if (prequest->initwithFaceID(faceID, customID, userID, gender)) {
        prequest->autorelease();
        return prequest;
    }
    CC_SAFE_DELETE(prequest);
    return nullptr;
}

HeaderRequest *HeaderRequest::createwithUrl(const std::string &url, DWORD userID)
{
    auto prequest = new HeaderRequest;
    if (prequest->initwithUrl(url, userID)) {
        prequest->autorelease();
        return prequest;
    }
    CC_SAFE_DELETE(prequest);
    return nullptr;
}

bool HeaderRequest::initwithFaceID(WORD faceID, WORD customID, DWORD userID, BYTE gender)
{
    if (customID != 0)
    {
        faceID = 0;
        auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(userID);
        if (!ptexture)
        {
            this->FaceRequest(customID, userID);
        }
        else
        {
            this->initWithTexture(ptexture);
            m_userID = userID;
            return true;
        }
    }
    faceID = faceID%10;
    std::string headstr = gender?"man":"woman";
    headstr = __String::createWithFormat("public_res/im_head_%s_%d.png", headstr.c_str(), faceID)->getCString();
    this->initWithFile(headstr);
    m_userID = userID;
    return true;
}

bool HeaderRequest::initwithUrl(const std::string &url, DWORD userID)
{
    auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(userID);
    if (!ptexture) {
        this->ThirdRequest(url);
        this->initWithFile("public_res/im_head_man_0.png");
    }
    else
    {
        this->initWithTexture(ptexture);
        m_userID = userID;
        return true;
    }
    m_userID = userID;
    return true;
}

void HeaderRequest::setHeadSize(float width)
{
    m_width = width;
    this->setScale(width/this->getContentSize().width);
}

void HeaderRequest::FaceRequest(WORD customID, DWORD userID)
{
    
    this->addCusEvent();

    auto prequest = new HttpRequest;
    auto purl = __String::createWithFormat("%s/CustomFace.aspx?UserID=%u&CustomID=%hu", ADDRESS_WHhttp, userID, customID);
    prequest->setUrl(purl->getCString());
    prequest->setRequestType(HttpRequest::Type::GET);
    prequest->setResponseCallback(CC_CALLBACK_2(HeaderRequest::FaceRequestCallback, this));
    HttpClient::getInstance()->send(prequest);
    prequest->release();
}

void HeaderRequest::FaceRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (response == NULL)
        return;

    if (response->isSucceed() == false)
        return;

    std::vector<char> *vecBuffer = response->getResponseData();
    int nSize = (int)vecBuffer->size();

    if (nSize == 0)
        return;

    std::string strResponseData;
    strResponseData.append(vecBuffer->begin(), vecBuffer->end());
    
	//char bytes[nSize + 1];
	char szTextureBuff[2048 * 2048 + 1];
	memset(szTextureBuff, 0, nSize);

    for (int i = 0; i < nSize/4; ++i)
    {
        szTextureBuff[i*4] = strResponseData[i*4+2];
        szTextureBuff[i*4+1] = strResponseData[i*4+1];
        szTextureBuff[i*4+2] = strResponseData[i*4];
        szTextureBuff[i*4+3] = 0xff;
    }
    
    Texture2D *texture = new Texture2D();
    texture->initWithData(szTextureBuff, nSize, Texture2D::PixelFormat::RGBA8888, 48, 48, cocos2d::Size(48,48));
    
    HallDataMgr::getInstance()->m_Headlist.insert(m_userID, texture);
    texture->release();

    EventCustom event(FACE_REQUEST_EVENT);

    __Integer *value = __Integer::create(m_userID);

    event.setUserData(value);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void HeaderRequest::ThirdRequest(const std::string &url)
{
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(FACE_REQUEST_EVENT, CC_CALLBACK_1(HeaderRequest::faceReuqcestEvent, this)), 1);
    
    auto prequest = new HttpRequest;
    prequest->setUrl(url.c_str());
    prequest->setRequestType(HttpRequest::Type::GET);
    prequest->setResponseCallback(CC_CALLBACK_2(HeaderRequest::ThirdRequestCallback, this));
    HttpClient::getInstance()->send(prequest);
    prequest->release();
}

void HeaderRequest::ThirdRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (response == NULL) {
        return;
    }
    if (response->isSucceed() == false) {
        
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    int nsize = (int)buffer->size();
    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    if (!backdata.length()) {
        return;
    }
    auto ptexture = CreatewithImageData((unsigned char *)backdata.c_str(),nsize);
    if (!ptexture)
    {
        return;
    }
    auto psprite = Sprite::createWithTexture(ptexture);
    psprite->setAnchorPoint(Vec2(0.0f, 0.0f));
    psprite->setScale(FACE_CX/psprite->getContentSize().width);
    psprite->retain();
    
    auto render = RenderTexture::create(FACE_CX, FACE_CY);
    render->retain();
    render->beginWithClear(0, 0, 0, 0);
    psprite->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    auto pimage = render->newImage();
    auto newtexture = new Texture2D;
    newtexture->initWithImage(pimage);
    
    HallDataMgr::getInstance()->m_Headlist.insert(m_userID, newtexture);
    NetworkMgr::getInstance()->sendCustomFaceInfo(pimage);
    HallDataMgr::getInstance()->m_wCustom = 1;
    newtexture->release();
    psprite->release();
    render->release();
    
    EventCustom event(FACE_REQUEST_EVENT);
    __Integer *value = __Integer::create(m_userID);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void HeaderRequest::enableTouch(bool enable,
                                std::function<bool (cocos2d::Touch *, cocos2d::Event *)> begin,
                                std::function<void (cocos2d::Touch *, cocos2d::Event *)> end)
{
    if (enable)
    {
        m_listener = EventListenerTouchOneByOne::create();
        CC_SAFE_RETAIN(m_listener);
        m_listener->setSwallowTouches(false);
        m_listener->onTouchBegan = begin;
        m_listener->onTouchEnded = end;
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
    }
    else
    {
        _eventDispatcher->removeEventListener(m_listener);
        CC_SAFE_RELEASE_NULL(m_listener);
    }
}

void HeaderRequest::addCusEvent()
{
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(FACE_REQUEST_EVENT, CC_CALLBACK_1(HeaderRequest::faceReuqcestEvent, this)), 1);
}
void HeaderRequest::faceReuqcestEvent(cocos2d::EventCustom *event)
{
    __Integer *pUserId = (__Integer*)event->getUserData();
    auto tex = HallDataMgr::getInstance()->m_Headlist.at(pUserId->getValue());
    if (nullptr != tex && pUserId->getValue() == m_userID)
    {
        this->initWithTexture(tex);
        this->setScale(m_width/tex->getContentSize().width);
    }
}