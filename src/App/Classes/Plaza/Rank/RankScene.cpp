//
//  Rank.cpp
//  GameProject
//
//  Created by Tang Miao on 2/2/16.
//
//

#include "RankScene.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/DictionaryHelper.h"
#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"

const std::string RANK_REQUEST_EVENT = "rank_request_event";

using namespace cocos2d::network;
using namespace cocostudio;
using namespace cocos2d::ui;

RankScene::RankScene():
_layout(nullptr)
{
    
    
}

RankScene::~RankScene()
{
    
     Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(RANK_REQUEST_EVENT);
    
 
    for (auto iter : m_RankData)
    {
        if (iter != nullptr)
        {
            
            delete iter;
        }
    }
    m_RankData.clear();
    _layout = nullptr;
}

bool RankScene::init()
{
    
    if (!Layer::init())
    {
        
        return false;
    }
    
    _layout = Layout::create();
    _layout->setTag(100);
    _layout->setContentSize(Size(1136, 960));
    auto rootNode = CSLoader::createNode("rank_res/RankNode.csb");
    _layout->addChild(rootNode);
    
    _layout->setScaleX(JUDGE_SCALE);
    this->addChild(_layout);
    
    
    //关闭按钮
    auto close = static_cast<Button *>(rootNode->getChildByName("Button_return"));
    if (nullptr != close)
    {
        close->addTouchEventListener(CC_CALLBACK_2(RankScene::buttonEventWithClose, this));
        
    }
    

    return true;
}

void RankScene::RankReuqcestEvent(cocos2d::EventCustom *event)
{
    
    initRankShow();
    
}

void RankScene::requestRankData()
{
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(RANK_REQUEST_EVENT, CC_CALLBACK_1(RankScene::RankReuqcestEvent, this)), 1);

    auto prequest = new HttpRequest;
    auto purl = __String::createWithFormat("%s/WS/PhoneRank.ashx?action=getscorerank&count=%d&userid=%u",
                                           ADDRESS_WHhttp6603,
                                           20,
                                           HallDataMgr::getInstance()->m_dwUserID);
    

    prequest->setUrl(purl->getCString());
    prequest->setRequestType(HttpRequest::Type::GET);
    prequest->setResponseCallback(CC_CALLBACK_2(RankScene::rankRequestCallBack, this));
    HttpClient::getInstance()->send(prequest);
    prequest->release();
    
    
}

void RankScene::rankRequestCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    if(this->getReferenceCount() == 0)
    {
        
        return;
    }
    
    if (response == NULL)
    {
        return;
    }
    if (response->isSucceed() == false)
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    int nsize = (int)buffer->size();
    if (nsize == 0)
    {
        return;
    }
    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    
    log("%s \n",backdata.c_str());
    
    rapidjson::Document doc;
    doc.Parse<0>(backdata.c_str());
 
    int num = 0;
    if (doc.IsArray())
    {
        num = doc.Size();
    }
    if (num == 0)
        return;
    
    m_RankData.clear();
    for (int index=0; index<num; ++index)
    {
        const rapidjson::Value &val = doc[index];
        
        RankData *data = new RankData();
        data->setNickName(DICTOOL->getStringValue_json(val, "NickName")) ;
        if (index == 0)
        {
            data->setRankIndex(DICTOOL->getIntValue_json(val, "Rank"));
        }else
        {
            
            data->setRankIndex(index);
        }
        
        LONG_LONG score = 0;
        if (val.HasMember("Score") && val["Score"].IsInt64())
        {
            score = val["Score"].GetInt64();
        }
        data->setUserScore(score);
        
        m_RankData.push_back(data);
    }
    
    EventCustom event(RANK_REQUEST_EVENT);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
}
void RankScene::initRankShow()
{
    

    auto listView = ListView::create();
    listView->setBounceEnabled(true);
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setContentSize(Size(1048, 390));
    listView->setBackGroundColor(Color3B::RED);
    listView->setAnchorPoint(Vec2(.5, 0.0));
    listView->setPosition(Vec2(568, 41));
    listView->setItemsMargin(1.0f);
    _layout->addChild(listView);
    
    int index = 0;
    for (auto rankData : m_RankData)
    {
        if (index == 0)
        {
            //自己当前排名
            auto selfRank = Label::createWithSystemFont(__String::createWithFormat("您当前排名第 %d 名",rankData->getRankIndex())->getCString(),
                                                        FONT_TREBUCHET_MS_BOLD,
                                                        28);
            selfRank->setPosition(Vec2(568, 470));
            selfRank->setTextColor(cocos2d::Color4B(44,220,115,255));
            _layout->addChild(selfRank);
            
            index ++;
            continue;
        }
        
        Layout *_layout = Layout::create();
        _layout->setContentSize(Size(1048, 110));
        
        //排名
        if (index >= 1 && index <4)
        {
            if (1 == index)
            {
                
                auto firstCell = ImageView::create("rank_res/rank_self.png");
                firstCell->setPosition(_layout->getContentSize()/2);
                _layout->addChild(firstCell);
            }
            
            ImageView *rank = ImageView::create(__String::createWithFormat("rank_res/rank_%d.png",index)->getCString());
            rank->setPosition(Vec2(80, _layout->getContentSize().height/2));
            _layout->addChild(rank);
        }else
            
        {
            
            auto rank = Label::createWithSystemFont(__String::createWithFormat("%d",index)->getCString(), FONT_TREBUCHET_MS_BOLD, 28);
            rank->setPosition(Vec2(80, _layout->getContentSize().height/2));
            _layout->addChild(rank);
        }
        
        
        //头像框
        ImageView *headRect = ImageView::create("rank_res/rank_headRect.png");
        headRect->setPosition(Vec2(200, _layout->getContentSize().height/2));
        _layout->addChild(headRect);
        
        //头像
        HeaderRequest *pHead = HeaderRequest::createwithFaceID(0,0,0,1);
        pHead->setHeadSize(70.0);
        
        pHead->setPosition(headRect->getContentSize()/2);
        headRect->addChild(pHead);
            
        
        //昵称
        Label *nick = Label::createWithSystemFont(rankData->getNickName().c_str(), FONT_TREBUCHET_MS_BOLD, 24);
        nick->setTextColor(cocos2d::Color4B(36,236,250,255));
        nick->setAnchorPoint(Vec2(0.0, 0.5));
        nick->setDimensions(250, nick->getContentSize().height + 2);
        nick->setPosition(Vec2(260, _layout->getContentSize().height/2  ));
        _layout->addChild(nick);
        
        //金币
        Label *coin = Label::createWithSystemFont(__String::createWithFormat("总金币 : %lld",rankData->getUserScore())->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
        coin->setTextColor(cocos2d::Color4B(243, 239, 147, 255));
        coin->setAnchorPoint(Vec2(0.0,0.5));
        coin->setPosition(Vec2(530,  _layout->getContentSize().height/2));
        _layout->addChild(coin);
        
        //分割线
        auto line = ImageView::create("rank_res/rank_line.png");
        line->setPosition(Vec2(_layout->getContentSize().width/2, 2));
        _layout->addChild(line);
        
        listView->pushBackCustomItem(_layout);
        
        index ++;
    }
    
}
void RankScene::buttonEventWithClose(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        CallFunc *func = CallFunc::create([this]{this->removeFromParent();});
        Sequence *quence = Sequence::createWithTwoActions(MoveTo::create(0.4f, Vec2(1136, 0)), func);
        
        this->runAction(quence);
    }
}

void RankScene::popRank()
{
    CallFunc *func = CallFunc::create([this]{
        HallDataMgr::getInstance()->AddpopLayer("", "正在加数据,请稍后...", Type_Wait_Text);
        requestRankData();
        
    });
    
    auto run = MoveTo::create(0.4f, Vec2(.0,.0));
    
    this->runAction(Sequence::createWithTwoActions(run, func));
    
}