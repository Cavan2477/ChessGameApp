/************************************************************************************
 * file: 		Notice.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:27:16
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "Notice.h"
#include "cocostudio/DictionaryHelper.h"

using namespace cocostudio;

const std::string NOTICE_REQUEST_EVENT = "NOTICE_REQUEST_EVENT";

Notice::Notice():_noticeText(nullptr)
{
}

Notice::~Notice()
{
    CC_SAFE_RELEASE(_noticeText);
    
    if (!m_bGetNotice)
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(NOTICE_REQUEST_EVENT);
}

Notice *Notice::CreateNotice()
{
    Notice *ret = new Notice();

    if (ret)
    {
        ret->initNotice();
        ret->autorelease();
    }
    
    return ret;
}

bool Notice::initNotice()
{
    this->setClippingRegion(Rect(0, 0, 475, 40));

    RequestNoticeContent();

    return true;
}

void Notice::NoticeReuqcestEvent(cocos2d::EventCustom *event)
{
    m_bGetNotice = true;

    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(NOTICE_REQUEST_EVENT);

    __String *data = static_cast<__String *>(event->getUserData());
    
    std::string noticecontent = data->getCString();
    
    Label *label = Label::createWithSystemFont(noticecontent, FONT_TREBUCHET_MS_BOLD, 20);

    setNoticeText(label);

    _noticeText->setAnchorPoint(Vec2(.0, .0));
    _noticeText->setPosition(Vec2(200, 0));

    this->addChild(_noticeText);
    this->runAction();
}

void Notice::RequestNoticeContent()
{
    m_bGetNotice = false;

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(NOTICE_REQUEST_EVENT, CC_CALLBACK_1(Notice::NoticeReuqcestEvent, this)), 1);
    
    auto prequest = new HttpRequest;
    auto purl = __String::createWithFormat("%s/WS/MobileInterface.ashx?action=getmobilenotice&kindid=%d", ADDRESS_WHhttp,LOGON_KIND_ID);

    prequest->setUrl(purl->getCString());
    prequest->setRequestType(HttpRequest::Type::GET);
    prequest->setResponseCallback(CC_CALLBACK_2(Notice::NoticeRequestCallback, this));

    HttpClient::getInstance()->send(prequest);

    prequest->release();
}

void Notice::NoticeRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (response == NULL)
        return;

    if (response->isSucceed() == false)
        return;

    std::vector<char> *buffer = response->getResponseData();

    int nsize = (int)buffer->size();

    if (nsize == 0)
        return;

    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    
    rapidjson::Document doc;
    doc.Parse<0>(backdata.c_str());

    const rapidjson::Value &val = DICTOOL->getSubDictionary_json(doc,"data");

    int noticeNum = DICTOOL->getArrayCount_json(val, "notice");

    if (noticeNum == 0)
        return;
    
    std::string noticecontent = "";

    for (int index=0; index<noticeNum; ++index)
    {
        const rapidjson::Value &noticedata = DICTOOL->getSubDictionary_json(val, "notice", index);
        std::string onecontent = DICTOOL->getStringValue_json(noticedata, "date");
        noticecontent = noticecontent + onecontent + " " + DICTOOL->getStringValue_json(noticedata, "content") + "        ";
    }
    
    __String *notice = __String::create(noticecontent);

    EventCustom event(NOTICE_REQUEST_EVENT);
    event.setUserData(notice);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    log("notice event");
}

void Notice::runAction()
{
    auto action = CallFunc::create([=]{_noticeText->setPosition(Vec2(200, 0));});
    auto quene =  Sequence::createWithTwoActions(MoveTo::create(30.0, Vec2(-2200, 0)), action);
    
   _noticeText->runAction(RepeatForever::create(quene));
}
