/************************************************************************************
 * file: 		Shop.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:09:20
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "ShopScene.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/DictionaryHelper.h"

#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../../LibExt/ClipText.h"

const std::string REAL_REQUEST_EVENT = "REAL_REQUEST_EVENT";
const std::string TEXTURE_REQUEST_EVENT = "TEXTURE_REQUEST_EVENT";

using namespace cocos2d::network;
using namespace cocos2d::ui;
using namespace cocostudio;

const int purnum = 9;
const float purper[9] = {1.0,5.00,10.00,20.00,50.00,100.00,500.00,1000.00,5000.00};

#pragma pack(1)
//商品纹理缓存
typedef struct _listInfo
{
public:
    uint64_t  nID;
    uint64_t  bufferLen;
    char *buffer;
    
    _listInfo(){
        
        nID = 0;
        bufferLen = 0;
        buffer = nullptr;
    }
}LIST_Info;
#pragma pack()

//MARK::ShopList
ShopList::ShopList():
_price(nullptr)
,_cellImage(nullptr)
,_awardName(nullptr)
{
    

}
ShopList::~ShopList()
{
    _listRes.clear();
    
}
ShopList *ShopList::create()
{
    
    ShopList* widget = new (std::nothrow) ShopList();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool ShopList::init()
{
    if (Widget::init())
    {
        return true;
    }
    return false;
}

void ShopList::onExit()
{
    std::string _event = TEXTURE_REQUEST_EVENT + __String::createWithFormat("%d",_nAwardID)->getCString();
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_event);
    
    Button::onExit();
}

void ShopList::setData(ListData *data)
{
    //存入id
    auto userDefault  = UserDefault::getInstance();
    userDefault->setIntegerForKey(__String::createWithFormat("list_id%d",data->_awardID)->getCString(), data->_awardID);
    userDefault->flush();
    
    //商品名称
    ClipText *clipName = ClipText::createClipText(cocos2d::Size(200,30), data->_awardName, FONT_TREBUCHET_MS_BOLD, 24);
    clipName->setTextColor(cocos2d::Color4B::YELLOW);
    clipName->setAnchorPoint(Vec2(0.5, 0.5));
    clipName->setPosition(Vec2(this->getContentSize().width/2,175));
    this->addChild(clipName);
    //商品价格
    
    //金币数目
    auto ingotIcon = ImageView::create("shop_res/shop_ingot.png");
    ingotIcon->setPosition(Vec2(60, 30));
    this->addChild(ingotIcon);
    
    auto ingotNum = Label::createWithSystemFont(__String::createWithFormat("%d",(int)data->_price)->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
    ingotNum->setAnchorPoint(Vec2(.0, 0.5));
    ingotNum->setPosition(Vec2(90, 30));
    this->addChild(ingotNum);
    
    auto cell = Sprite::create();
    cell->setContentSize(Size(181,110));
    cell->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2 + 3));
    setCellImage(cell);
    this->addChild(getCellImage());
    
    //本地获取内容
    if (getTextureByID(data->_awardID) != nullptr)
    {
        
        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
        
        auto ptexture = getTextureByID(data->_awardID);
        
        getCellImage()->initWithTexture(ptexture);
        log("%0.2f,%0.2f",ptexture->getContentSize().width, ptexture->getContentSize().height);
        getCellImage()->setScale(181/ptexture->getContentSize().width ,110/ptexture->getContentSize().height);
        
        _listRes.insert((int)data->_awardID, ptexture);
        
        return;
    }
    
    //请求商品图片
    std::string event = TEXTURE_REQUEST_EVENT + __String::createWithFormat("%d",data->_awardID)->getCString();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(event, CC_CALLBACK_1(ShopList::TexureReuqcestEvent, this)), 1);
    
    _nAwardID = data->_awardID;
    auto prequest = new cocos2d::network::HttpRequest;
    auto purl = __String::createWithFormat("%s",data->_listFile.c_str());
    prequest->setUrl(purl->getCString());
    prequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    prequest->setResponseCallback([=](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
       
        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
        
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
        
        //将纹理写入本地文件
        std::string writePath = FileUtils::getInstance()->getWritablePath() + __String::createWithFormat("texture_%d.txt",data->_awardID)->getCString();
        FILE *fp = fopen(writePath.c_str(), "wb");
        if (fp)
        {
            
            fwrite(backdata.c_str(), 1, nsize, fp);
            fclose(fp);
            
            log("%s Write success",__String::createWithFormat("texture_%d.txt",data->_awardID)->getCString());
        }else
        {
            
            log("%s Write failure",__String::createWithFormat("texture_%d.txt",data->_awardID)->getCString());
            
        }
        
        std::string _event = TEXTURE_REQUEST_EVENT + __String::createWithFormat("%d",data->_awardID)->getCString();
        EventCustom event(_event);
        LIST_Info info;
        memset(&info, 0, sizeof(LIST_Info));
        info.nID = data->_awardID;
        info.bufferLen = nsize;
        info.buffer = (char *)backdata.c_str();
        event.setUserData(&info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    });
    cocos2d::network::HttpClient::getInstance()->send(prequest);
    prequest->release();
}

void ShopList::TexureReuqcestEvent(cocos2d::EventCustom *event)
{
    LIST_Info *_data = ( LIST_Info*)event->getUserData();
    
    auto ptexture = CreatewithImageData((unsigned char *)_data->buffer,(DWORD)_data->bufferLen);
    if (!ptexture)
    {
        return;
    }
    
    getCellImage()->initWithTexture(ptexture);
    log("%0.2f,%0.2f",ptexture->getContentSize().width, ptexture->getContentSize().height);
    getCellImage()->setScale(181/ptexture->getContentSize().width ,110/ptexture->getContentSize().height);
    
    _listRes.insert((int)_data->nID, ptexture);
}

Texture2D * ShopList::getTextureByID(uint64_t ID)
{
    
    std::string writePath = FileUtils::getInstance()->getWritablePath() + __String::createWithFormat("texture_%llu.txt",ID)->getCString();
    FILE *fp = fopen(writePath.c_str(), "rb");
    if (fp)
    {
        LIST_Info info;
        memset(&info,0,sizeof(LIST_Info));
        fread(&info, 1, sizeof(LIST_Info), fp);
        fclose(fp);
        
        auto ptexture = CreatewithImageData((unsigned char *)info.buffer,(DWORD)info.bufferLen);
        if (ptexture)
        {
            return ptexture;
        }
        else
        {
            return nullptr;
        }
        
    }
    
    return nullptr;
}

//MARK::SHOP
ShopScene::ShopScene():
_userBean(nullptr)
,_userIngot(nullptr)
,_userScore(nullptr)
,_rootNode(nullptr)
,_picker(nullptr)
,_coinExchangeLayout(nullptr)
,_beanPurchaseLayout(nullptr)
,_realExchangeLayout(nullptr)
,_nameEidt(nullptr)
,telEdit(nullptr)
,_adressEidt(nullptr)
,m_BeanRate(0)
,m_IngotRate(0)
,_nProvinceCode(-1)
,_nCityCode(-1)
,_nAreaCode(-1)
,m_nExchangeCount(1)
,m_nExchangeID(0)
,m_bShopData(false)
,_httpType(HTTP_LIST)
{
          Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(REAL_REQUEST_EVENT, CC_CALLBACK_1(ShopScene::RealReuqcestEvent, this)), 1);
}
ShopScene::~ShopScene()
{
    
    CC_SAFE_RELEASE(_userScore);
    CC_SAFE_RELEASE(_userBean);
    CC_SAFE_RELEASE(_userIngot);
    
    for (auto iter : m_Datalist)
    {
        
        CC_SAFE_RELEASE(iter);
    }
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(REAL_REQUEST_EVENT);
    m_Datalist.clear();
}

bool ShopScene::init()
{
    
    if (!Layer::init())
    {
        
        return false;
    }
    
    auto layout = Layout::create();
    layout->setContentSize(Size(1136, 640));
    
     _rootNode = CSLoader::createNode("shop_res/ShopScene.csb");
    layout->addChild(_rootNode);
    
    //用户携带金币
    Label *coin = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_lUserGold, ","), FONT_TREBUCHET_MS_BOLD, 24);
    setUserScore(coin);
    _userScore->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_userScore, 145);
    _userScore->setAnchorPoint(Vec2(.0, .5));
    _userScore->setPosition(Vec2(200, 595));
    layout->addChild(_userScore);
    
    //用户金币
    Label *ingot = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_lGold, ","), FONT_TREBUCHET_MS_BOLD, 24);
    setUserIngot(ingot);
    _userIngot->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_userIngot, 145);
    _userIngot->setAnchorPoint(Vec2(.0, .5));
    _userIngot->setPosition(Vec2(740, 595));
    layout->addChild(_userIngot);
    
    //用户游戏豆
    Label *bean = Label::createWithSystemFont(__String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_dBean)->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
    setUserBean(bean);
    _userBean->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_userBean, 145);
    _userBean->setAnchorPoint(Vec2(.0, .5));
    _userBean->setPosition(Vec2(465, 595));
    layout->addChild(_userBean);
  
    //子layout
    
    //游戏币兑换
    _coinExchangeLayout = Layout::create();
    _coinExchangeLayout->setContentSize(Size(800, 501));
    _coinExchangeLayout->setAnchorPoint(Vec2(.0, .0));
    _coinExchangeLayout->setPosition(Vec2(248, 20));
    layout->addChild(_coinExchangeLayout);

    auto coinList = ListView::create();
    coinList->setContentSize(Size(800, 460));
    coinList->setAnchorPoint(Vec2(.0, .0));
    coinList->setPosition(Vec2(.0, .0));
    coinList->setBounceEnabled(true);
    coinList->setDirection(ui::ScrollView::Direction::VERTICAL);
    coinList->setItemsMargin(40.0f);
    coinList->setTag(10);
    _coinExchangeLayout->addChild(coinList);
    
    //游戏豆购买
    _beanPurchaseLayout = Layout::create();
    _beanPurchaseLayout->setContentSize(Size(800, 501));
    _beanPurchaseLayout->setAnchorPoint(Vec2(.0, .0));
    _beanPurchaseLayout->setPosition(Vec2(248, 20));
    layout->addChild(_beanPurchaseLayout);
    _beanPurchaseLayout->setVisible(false);
    
    initPurchase();
    
    //实物兑换
    _realExchangeLayout = Layout::create();
    _realExchangeLayout->setContentSize(Size(800, 500));
    _realExchangeLayout->setAnchorPoint(Vec2(.0, .0));
    _realExchangeLayout->setPosition(Vec2(248, 20));
    layout->addChild(_realExchangeLayout);
    _realExchangeLayout->setVisible(false);
    

    //菜单按钮
    auto coinExchange = static_cast<Button *>(_rootNode->getChildByName("Button_coin"));
    //游戏豆购买
    auto beanPurchase = static_cast<Button *>(_rootNode->getChildByName("Button_bean"));
    //实物兑换
    auto realExchange = static_cast<Button *>(_rootNode->getChildByName("Button_realExchange"));
    
    if (nullptr != coinExchange)
    {
        
        coinExchange->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
           
            if (type == Widget::TouchEventType::ENDED)
            {
                
                coinExchange->loadTextures("shop_res/shop_exchange_coin1.png", "shop_res/shop_exchange_coin0.png");
                beanPurchase->loadTextures("shop_res/shop_exchange_bean0.png", "shop_res/shop_exchange_bean1.png");
                realExchange->loadTextures("shop_res/shop_real_exchange0.png", "shop_res/shop_real_exchange1.png");
                
                _coinExchangeLayout->setVisible(true);
                _beanPurchaseLayout->setVisible(false);
                _realExchangeLayout->setVisible(false);
            }
        
        });
                                            
    }
    
 
    if (nullptr != beanPurchase)
    {
        
        beanPurchase->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                coinExchange->loadTextures("shop_res/shop_exchange_coin0.png", "shop_res/shop_exchange_coin1.png");
                beanPurchase->loadTextures("shop_res/shop_exchange_bean1.png", "shop_res/shop_exchange_bean0.png");
                realExchange->loadTextures("shop_res/shop_real_exchange0.png", "shop_res/shop_real_exchange1.png");
                
                _coinExchangeLayout->setVisible(false);
                _beanPurchaseLayout->setVisible(true);
                _realExchangeLayout->setVisible(false);
            }
            
        });
    }
    

    if (nullptr != realExchange)
    {
        
        realExchange->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                coinExchange->loadTextures("shop_res/shop_exchange_coin0.png", "shop_res/shop_exchange_coin0.png");
                beanPurchase->loadTextures("shop_res/shop_exchange_bean0.png", "shop_res/shop_exchange_bean1.png");
                realExchange->loadTextures("shop_res/shop_real_exchange1.png", "shop_res/shop_real_exchange0.png");
                
                _coinExchangeLayout->setVisible(false);
                _beanPurchaseLayout->setVisible(false);
                _realExchangeLayout->setVisible(true);
                
       
                if (!m_bShopData)
                {
                    
                    //请求商品列表
                    //参数设定
                    int page = 1;
                    int PageSize = 20;
                    
                    auto prequest = new cocos2d::network::HttpRequest;
                    auto purl = __String::createWithFormat("%s&page=%d&PageSize=%d",URL_LIST,page,PageSize);
                    prequest->setUrl(purl->getCString());
                    prequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
                    prequest->setResponseCallback(CC_CALLBACK_2(ShopScene::ListRequestCallback, this));
                    cocos2d::network::HttpClient::getInstance()->send(prequest);
                    prequest->release();
                    
                    
                    HallDataMgr::getInstance()->AddpopLayer("", "正在加载,请稍后...", Type_Wait_Text);
                    
                }

            }
            
        });
        
    }
    
    
    
    //关闭按钮
    Button *closeBtn = static_cast<Button *>(_rootNode->getChildByName("Button_return"));
    if (closeBtn != nullptr)
    {
        closeBtn->addTouchEventListener([=](Ref * ref, cocos2d::ui::Widget::TouchEventType type){
            
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                
                if (_realExchangeLayout->isVisible())
                {
                    
                    auto node = _realExchangeLayout->getChildByTag(1001);
                    auto listView = _realExchangeLayout->getChildByTag(1000);
                    auto listDetail = _realExchangeLayout->getChildByTag(1002);
                    if (node != nullptr && listView != nullptr && listDetail != nullptr)
                    {
                        m_nExchangeCount = 1;
                        m_nExchangeID = -1;
                        node->removeFromParent();
                        listDetail->removeFromParent();
                        listView->setVisible(true);
                        coinExchange->setVisible(true);
                        beanPurchase->setVisible(true);
                        realExchange->setVisible(true);
                        
                        return;
                        
                    }
                }
                CallFunc *func = CallFunc::create([this]{this->removeFromParent();});
                Sequence *quence = Sequence::createWithTwoActions(MoveTo::create(0.4f, Vec2(1136, 0)), func);
                this->runAction(quence);
            }
        });
    }
    
    
    layout->setScaleX(JUDGE_SCALE);
    this->addChild(layout);
    
    
    
    return true;
}

void ShopScene::setIndex(int nindex)
{
    
    //菜单按钮
    auto coinExchange = static_cast<Button *>(_rootNode->getChildByName("Button_coin"));
    //游戏豆购买
    auto beanPurchase = static_cast<Button *>(_rootNode->getChildByName("Button_bean"));
    //实物兑换
    auto realExchange = static_cast<Button *>(_rootNode->getChildByName("Button_realExchange"));
    
    switch (nindex) {
        case 0:
            
            break;
        case 1:
            coinExchange->loadTextures("shop_res/shop_exchange_coin0.png", "shop_res/shop_exchange_coin1.png");
            beanPurchase->loadTextures("shop_res/shop_exchange_bean1.png", "shop_res/shop_exchange_bean0.png");
            realExchange->loadTextures("shop_res/shop_real_exchange0.png", "shop_res/shop_real_exchange1.png");
            
            _coinExchangeLayout->setVisible(false);
            _beanPurchaseLayout->setVisible(true);
            _realExchangeLayout->setVisible(false);
            break;
        default:
            break;
    }
    
    
}

void ShopScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_EXCHANGE_PARAMETER, CC_CALLBACK_2(ShopScene::ExchangeParameterResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_EXCHANGE_RESULT, CC_CALLBACK_2(ShopScene::ExchangeResult, this));
    
}

void ShopScene::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_EXCHANGE_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_EXCHANGE_RESULT);
    Layer::onExit();
    
}

void ShopScene::initShop()
{
 
    int per[9] = {1,5,10,20,50,100,500,1000,5000};
    
    
    auto listView = static_cast<ListView *>(_coinExchangeLayout->getChildByTag(10));
    
    Layout *sublayout = nullptr;
    //游戏豆兑换
    for (int i = 0; i < 9; i++)
    {
        
        if (i%3==0)
        {
            
            sublayout = Layout::create();
            sublayout->setContentSize(Size(800, 197));
            
            listView->pushBackCustomItem(sublayout);
            
        }
        
        auto exchange_byBean = Button::create("shop_res/shop_one_list.png");
        exchange_byBean->setAnchorPoint(Vec2(.0, .5));
        exchange_byBean->setPosition(Vec2(100 + 230*(i%3), sublayout->getContentSize().height/2));
        sublayout->addChild(exchange_byBean);
        
        exchange_byBean->addTouchEventListener([=](Ref *ref , cocos2d::ui::Widget::TouchEventType type){
           
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "兑换中...", Type_Wait_Text);
                this->sendExchangeBean(per[i]);
            }
            
        });
        
        //游戏币
        auto coin = Label::createWithSystemFont(__String::createWithFormat("%u",per[i] * this->m_BeanRate)->getCString(),
                                                FONT_TREBUCHET_MS_BOLD,
                                                24);
        
        coin->setAlignment(cocos2d::TextHAlignment::RIGHT);
        coin->setTextColor(cocos2d::Color4B::YELLOW);
        coin->setAnchorPoint(Vec2(1.0, 0.5));
        coin->setPosition(exchange_byBean->getContentSize().width/2,175);
        exchange_byBean->addChild(coin);
        
        auto tips = Label::createWithSystemFont("游戏币",
                                               FONT_TREBUCHET_MS_BOLD,
                                               24);
        
        tips->setAlignment(cocos2d::TextHAlignment::LEFT);
        tips->setTextColor(cocos2d::Color4B::WHITE);
        tips->setAnchorPoint(Vec2(0.0, 0.5));
        tips->setPosition(exchange_byBean->getContentSize().width/2 + 2 ,175);
        exchange_byBean->addChild(tips);
        
        //豆子数目
        auto beanIcon = ImageView::create("shop_res/shop_bean.png");
        beanIcon->setPosition(Vec2(60, 30));
        exchange_byBean->addChild(beanIcon);
        
        auto beanNum = Label::createWithSystemFont(__String::createWithFormat("%d",per[i])->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
        beanNum->setAnchorPoint(Vec2(.0, 0.5));
        beanNum->setPosition(Vec2(beanIcon->getPosition().x + 30, beanIcon->getPosition().y));
        exchange_byBean->addChild(beanNum);
        
    }
    
 
    //金币兑换
    for (int i = 0; i < 9; i++)
    {
        if (i%3==0)
        {
            
            sublayout = Layout::create();
            sublayout->setContentSize(Size(800, 197));
            
            listView->pushBackCustomItem(sublayout);
            
        }
        
        auto exchange_byIngot = Button::create("shop_res/shop_one_list.png");
        exchange_byIngot->setAnchorPoint(Vec2(.0, .5));
        exchange_byIngot->setPosition(Vec2(100 + 230*(i%3), sublayout->getContentSize().height/2));
        sublayout->addChild(exchange_byIngot);
        
        exchange_byIngot->addTouchEventListener([=](Ref *ref , cocos2d::ui::Widget::TouchEventType type){
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "兑换中...", Type_Wait_Text);
                this->sendExchangeIngot(per[i]);
            }
            
        });
        
        //游戏币
        auto coin = Label::createWithSystemFont(__String::createWithFormat("%u",per[i] * this->m_IngotRate)->getCString(),
                                                FONT_TREBUCHET_MS_BOLD,
                                                24);
        
        coin->setAlignment(cocos2d::TextHAlignment::RIGHT);
        coin->setTextColor(cocos2d::Color4B::YELLOW);
        coin->setAnchorPoint(Vec2(1.0, 0.5));
        coin->setPosition(exchange_byIngot->getContentSize().width/2,175);
        exchange_byIngot->addChild(coin);
        
        auto tips = Label::createWithSystemFont("游戏币",
                                                FONT_TREBUCHET_MS_BOLD,
                                                24);
        
        tips->setAlignment(cocos2d::TextHAlignment::LEFT);
        tips->setTextColor(cocos2d::Color4B::WHITE);
        tips->setAnchorPoint(Vec2(0.0, 0.5));
        tips->setPosition(exchange_byIngot->getContentSize().width/2 + 2 ,175);
        exchange_byIngot->addChild(tips);
        
        //豆子数目
        auto ingotIcon = ImageView::create("shop_res/shop_ingot.png");
        ingotIcon->setPosition(Vec2(60, 30));
        exchange_byIngot->addChild(ingotIcon);
        
        auto ingotNum = Label::createWithSystemFont(__String::createWithFormat("%d",per[i])->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
        ingotNum->setAnchorPoint(Vec2(.0, 0.5));
        ingotNum->setPosition(Vec2(ingotIcon->getPosition().x + 30, ingotIcon->getPosition().y));
        exchange_byIngot->addChild(ingotNum);
    }
  
}

void ShopScene::initPurchase()
{
    
    auto beanPurchaseList = ListView::create();
    beanPurchaseList->setContentSize(Size(800, 460));
    beanPurchaseList->setAnchorPoint(Vec2(.0, .0));
    beanPurchaseList->setPosition(Vec2(.0, .0));
    beanPurchaseList->setBounceEnabled(true);
    beanPurchaseList->setDirection(ui::ScrollView::Direction::VERTICAL);
    beanPurchaseList->setItemsMargin(40.0f);
    beanPurchaseList->setTag(10);
    _beanPurchaseLayout->addChild(beanPurchaseList);
    

    
    int per[9] = {1,5,10,20,50,100,500,1000,5000};
    
    
    Layout *sublayout = nullptr;
    //游戏豆兑换
    for (int i = 0; i < 9; i++)
    {
        
        if (i%3==0)
        {
            
            sublayout = Layout::create();
            sublayout->setContentSize(Size(800, 197));
            
            beanPurchaseList->pushBackCustomItem(sublayout);
            
        }
        
        auto shopList = Button::create("shop_res/shop_purchase_0.png");
        shopList->setAnchorPoint(Vec2(.0, .5));
        shopList->setPosition(Vec2(100 + 230*(i%3), sublayout->getContentSize().height/2));
        sublayout->addChild(shopList);
        shopList->addTouchEventListener([=](Ref *ref , cocos2d::ui::Widget::TouchEventType type){
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                
                HallDataMgr::getInstance()->AddpopLayer("提示", "暂无数据...", Type_Ensure);
            }
            
        });
        
        //游戏币
        auto coin = Label::createWithSystemFont(__String::createWithFormat("¥%d",per[i])->getCString(),
                                                FONT_TREBUCHET_MS_BOLD,
                                                24);
        
        coin->setAlignment(cocos2d::TextHAlignment::RIGHT);
        coin->setTextColor(cocos2d::Color4B::YELLOW);
        coin->setAnchorPoint(Vec2(.5, 0.5));
        coin->setPosition(shopList->getContentSize().width/2,175);
        shopList->addChild(coin);
        
     
        //豆子数目
        auto beanIcon = ImageView::create("shop_res/shop_bean.png");
        beanIcon->setPosition(Vec2(60, 30));
        shopList->addChild(beanIcon);
        
        auto beanNum = Label::createWithSystemFont(__String::createWithFormat("%d",per[i])->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
        beanNum->setAnchorPoint(Vec2(.0, 0.5));
        beanNum->setPosition(Vec2(beanIcon->getPosition().x + 30, beanIcon->getPosition().y));
        shopList->addChild(beanNum);
        
    }
}

void ShopScene::initReal()
{
    auto realList = ListView::create();
    realList->setTag(1000);
    realList->setContentSize(Size(800, 460));
    realList->setAnchorPoint(Vec2(.0, .0));
    realList->setPosition(Vec2(.0, .0));
    realList->setBounceEnabled(true);
    realList->setDirection(ui::ScrollView::Direction::VERTICAL);
    realList->setItemsMargin(40.0f);
    _realExchangeLayout->addChild(realList);
    
    
    //地址选择器
     _picker = AreaPicker::create();
    _picker->setPosition(Vec2(.0, -400));
    _picker->_nIndex = 0;
    _picker->loadConfig();
    this->addChild(_picker);
    _picker->setVisible(false);
    
    int size = (int)m_Datalist.size();

    Layout *sublayout = nullptr;
    
    //游戏豆兑换
    for (int i = 0; i < size; i++)
    {
        
        if (i%3==0)
        {
            
            sublayout = Layout::create();
            sublayout->setContentSize(Size(800, 197));
            
            realList->pushBackCustomItem(sublayout);
            
        }
        
        auto shopList = ShopList::create();
        shopList->setContentSize(Size(219, 197));
        shopList->loadTextures("shop_res/shop_real_0.png", "shop_res/shop_real_0.png");
        shopList->setAnchorPoint(Vec2(.0, .5));
        shopList->setPosition(Vec2(100 + 230*(i%3), sublayout->getContentSize().height/2));
        sublayout->addChild(shopList);
        shopList->setData(m_Datalist.at(i));
        shopList->addTouchEventListener([=](Ref *ref , cocos2d::ui::Widget::TouchEventType type){
            
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                this->initDetail(m_Datalist.at(i),shopList);
                
            }
            
        });
    }
}
void ShopScene::initDetail(ListData *data,ShopList *list)
{
    
    auto realList = _realExchangeLayout->getChildByTag(1000);
    assert(realList);
    
    realList->setVisible(false);
    
    m_nExchangeID = data->_awardID;
    
    //隐藏菜单按钮
    //菜单按钮
    auto coinExchange = static_cast<Button *>(_rootNode->getChildByName("Button_coin"));
    //游戏豆购买
    auto beanPurchase = static_cast<Button *>(_rootNode->getChildByName("Button_bean"));
    //实物兑换
    auto realExchange = static_cast<Button *>(_rootNode->getChildByName("Button_realExchange"));
    
    if (coinExchange != nullptr && beanPurchase != nullptr && realExchange != nullptr)
    {
        coinExchange->setVisible(false);
        beanPurchase->setVisible(false);
        realExchange->setVisible(false);
    }
    //加载商品详细信息
    auto node = CSLoader::createNode("shop_res/realExchange.csb");
    node->setTag(1001);
    node->setAnchorPoint(Vec2(.0, .0));
    node->setPosition(Vec2(-200, -10));
    _realExchangeLayout->addChild(node);
    
    
    //所需金币
    auto needIngot =  Label::createWithCharMap("shop_res/shop_num_yellow.png", 16, 22, '0');
    needIngot->setString(__String::createWithFormat("%d",(int)data->_price)->getCString());
    needIngot->setAnchorPoint(Vec2(.0, .5));
    needIngot->setPosition(Vec2(460, 385));
    node->addChild(needIngot);
    
    //剩余金币
    auto restIngot =  Label::createWithCharMap("shop_res/shop_num_yellow.png", 16, 22, '0');
    restIngot->setString(__String::createWithFormat("%lld",HallDataMgr::getInstance()->m_lGold)->getCString());
    restIngot->setAnchorPoint(Vec2(.0, .5));
    restIngot->setPosition(Vec2(910, 385));
    node->addChild(restIngot);
    
    //数量
    auto orginal =  Label::createWithCharMap("shop_res/shop_num_yellow.png", 16, 22, '0');
    orginal->setString("1");
    orginal->setAnchorPoint(Vec2(.0, .5));
    orginal->setPosition(Vec2(650, 440));
    node->addChild(orginal);
    
    
    auto plus = static_cast<Button *>(node->getChildByName("Button_plus"));
    if (nullptr != plus)
    {
        
        plus->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            
            if (type == Widget::TouchEventType::ENDED)
            {
               
                if ((m_nExchangeCount+1) * data->_price > HallDataMgr::getInstance()->m_lGold)
                {
                    
                    HallDataMgr::getInstance()->AddpopLayer("提示", "您的金币不足", Type_Ensure);
                    return ;
                }
                
               
                m_nExchangeCount++;
                orginal->setString(__String::createWithFormat("%d",m_nExchangeCount)->getCString());
                needIngot->setString(__String::createWithFormat("%d",(int)data->_price * m_nExchangeCount)->getCString());
                
            }
            
        });
        
    }
    auto min = static_cast<Button *>(node->getChildByName("Button_min"));
    if (nullptr != min)
    {
        
        min->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            
            if (type == Widget::TouchEventType::ENDED)
            {
                
                if (m_nExchangeCount > 1)
                {
                    m_nExchangeCount--;
                    orginal->setString(__String::createWithFormat("%d",m_nExchangeCount)->getCString());
                    needIngot->setString(__String::createWithFormat("%d",(int)data->_price * m_nExchangeCount)->getCString());
                    
                }
                
            }
            
        });
        
    }
    
    
    //收货人姓名
    _nameEidt = EditBox::create(Size(416, 45), "shop_res/shop_text_long.png");
    _nameEidt->setAnchorPoint(Vec2(.0, .5));
    _nameEidt->setPosition(Vec2(459, 330));
    _nameEidt->setDelegate(this);
    _nameEidt->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    _nameEidt->setFontSize(28);
    _nameEidt->setReturnType(EditBox::KeyboardReturnType::DONE);
    _nameEidt->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
    _nameEidt->setPlaceHolder("请输入收货人姓名");
    _nameEidt->setFontColor(Color3B::YELLOW);
    node->addChild(_nameEidt);
    
    //手机号码
    telEdit = EditBox::create(Size(416, 45), "shop_res/shop_text_long.png");
    telEdit->setAnchorPoint(Vec2(.0, .5));
    telEdit->setPosition(Vec2(459, 276));
    telEdit->setDelegate(this);
    telEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    telEdit->setFontSize(28);
    telEdit->setReturnType(EditBox::KeyboardReturnType::DONE);
    telEdit->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
    telEdit->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
    telEdit->setMaxLength(11);
    telEdit->setPlaceHolder("请输入收货人手机号码");
    telEdit->setFontColor(Color3B::YELLOW);
    node->addChild(telEdit);
    
    //详细地址
    _adressEidt  =  EditBox::create(Size(416, 45), "shop_res/shop_text_long.png");
    _adressEidt->setAnchorPoint(Vec2(.0, .5));
    _adressEidt->setPosition(Vec2(459, 172));
    _adressEidt->setDelegate(this);
    _adressEidt->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    _adressEidt->setFontSize(28);
    _adressEidt->setReturnType(EditBox::KeyboardReturnType::DONE);
    _adressEidt->setPlaceholderFont(FONT_TREBUCHET_MS_BOLD, 24);
    _adressEidt->setPlaceHolder("请输入收货人详细地址");
    _adressEidt->setFontColor(Color3B::YELLOW);
    node->addChild(_adressEidt);
    
    //确认兑换
    auto sureExchange = static_cast<Button *>(node->getChildByName("Button_exchange_sure"));
    if (sureExchange != nullptr)
    {
        
        sureExchange->addTouchEventListener(CC_CALLBACK_2(ShopScene::buttoneEventWithRealExchange, this));
        
    }
    
    
    auto listDetail = ImageView::create("shop_res/shop_real_0.png");
    listDetail->setTag(1002);
    listDetail->setAnchorPoint(Vec2(1.0, 0.0));
    listDetail->setPosition(Vec2(35, 250));
    _realExchangeLayout->addChild(listDetail);
    
    //商品名称
    ClipText *clipName = ClipText::createClipText(cocos2d::Size(200,30), data->_awardName, FONT_TREBUCHET_MS_BOLD, 24);
    clipName->setTextColor(cocos2d::Color4B::YELLOW);
    clipName->setAnchorPoint(Vec2(0.5, 0.5));
    clipName->setPosition(Vec2(listDetail->getContentSize().width/2,175));
    listDetail->addChild(clipName);
    //商品价格
    
    //金币数目
    auto ingotIcon = ImageView::create("shop_res/shop_ingot.png");
    ingotIcon->setPosition(Vec2(60, 30));
    listDetail->addChild(ingotIcon);
    
    auto ingotNum = Label::createWithSystemFont(__String::createWithFormat("%d",(int)data->_price)->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
    ingotNum->setAnchorPoint(Vec2(.0, 0.5));
    ingotNum->setPosition(Vec2(90, 30));
    listDetail->addChild(ingotNum);
    
    
    if (list->_listRes.at(data->_awardID) == nullptr)
        return;
    
    //详细图片
    auto texure = list->_listRes.at(data->_awardID);
    Sprite *res = Sprite::createWithTexture(texure);
    res->setAnchorPoint(Vec2(.5, .5));
    res->setPosition(Vec2(listDetail->getContentSize().width/2, listDetail->getContentSize().height/2 + 3));
    listDetail->addChild(res);
    res->setScale(181/texure->getContentSize().width , 110/texure->getContentSize().height);
    
    float yOffset = 0.0f;
    auto detailName = static_cast<Text *>(node->getChildByName("Text_detailName"));
    if (detailName != nullptr)
    {
        auto nameLabel = Label::createWithSystemFont(data->_awardName, FONT_ARIAL, 24, cocos2d::Size(200,0));
        nameLabel->setColor(Color3B::YELLOW);
        nameLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
        nameLabel->setPosition(Vec2(125, 240));
        node->addChild(nameLabel);
        
        if (nameLabel->getContentSize().height > 30)
        {
            yOffset = nameLabel->getContentSize().height - 30;
        }
        detailName->removeFromParentAndCleanup(true);
    }
    
    auto detailNum = static_cast<Text*>(node->getChildByName("Text_detailNum"));
    if (detailNum != nullptr)
    {
        detailNum->setString(__String::createWithFormat("%d 枚",(int)data->_price)->getCString());
        
        if (yOffset > 0.0f)
        {
            detailNum->setPositionY(detailNum->getPositionY() - yOffset);
        }
    }
    
    auto detailPre = node->getChildByName("Text_3");
    if (nullptr != detailPre)
    {
        if (yOffset > 0.0f)
        {
            detailPre->setPositionY(detailPre->getPositionY() - yOffset);
        }
    }
    
    //省份
    auto Province = static_cast<Button *>(node->getChildByName("Button_province"));
    auto textProvince = static_cast<Text *>(node->getChildByName("Text_select_p"));
    auto textCity = static_cast<Text *>(node->getChildByName("Text_select_c"));
    auto textArea = static_cast<Text *>(node->getChildByName("Text_select_a"));
    if (nullptr != Province)
    {
        
        Province->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                
                _picker->setVisible(true);
                _picker->openPicker(0);
                _picker->popPicker();
              
                _picker->_selectCallback = [=](std::string text,int nselect,int nindex){
                    
                    if (nullptr != textProvince)
                    {
                        _nProvinceCode = nselect;
                        _nCityCode = -1;
                        _nAreaCode = -1;
                        if ((textCity != nullptr) && (textCity->getString().compare("请选择")!=0))
                        {
                            textCity->setString("请选择");
                            
                        }
                        if ((textArea != nullptr) && (textArea->getString().compare("请选择")!=0))
                        {
                            textArea->setString("请选择");
                            
                        }
                        textProvince->setString(text);
                    }
                    
                };
                
            }
            
        });
    }
    
    auto City = static_cast<Button *>(node->getChildByName("Button_city"));
  
    if (nullptr != City)
    {
        
        City->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                
                if (textProvince->getString().compare("请选择") == 0)
                {
                    
                    HallDataMgr::getInstance()->AddpopLayer("提示", "请先选择省份", Type_Ensure);
                    return ;
                }
                
                _picker->setVisible(true);
                _picker->openPicker(1);
                _picker->popPicker();
                
                _picker->_selectCallback = [=](std::string text,int nselect,int nindex){
                    
                    if (nullptr != textCity)
                    {
                        _nCityCode = nselect;
                        _nAreaCode = -1;
                        
                        if ((textArea != nullptr) && (textArea->getString().compare("请选择")!=0))
                        {
                            textArea->setString("请选择");
                            
                        }
                        textCity->setString(text);
                    }
                    
                };
            }
            
        });
    }
    
    auto Area = static_cast<Button *>(node->getChildByName("Button_area"));
    if (nullptr != City)
    {
        
        Area->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                
                if (textProvince->getString().compare("请选择") == 0)
                {
                    
                    HallDataMgr::getInstance()->AddpopLayer("提示", "请先选择省份", Type_Ensure);
                    return ;
                }
                
                if (textCity->getString().compare("请选择") == 0)
                {
                    
                    HallDataMgr::getInstance()->AddpopLayer("提示", "请先选择市", Type_Ensure);
                    return ;
                }
                
                _picker->setVisible(true);
                _picker->openPicker(2);
                _picker->popPicker();
                
                _picker->_selectCallback = [=](std::string text,int nselect,int nindex){
                    
                    if (nullptr != textArea)
                    {
                        _nAreaCode = nselect;
                        textArea->setString(text);
                    }
                    
                };
                
                
            }
            
        });
    }
}

void ShopScene::updateInfo()
{
    _userScore->setString(getScorewithComma(HallDataMgr::getInstance()->m_lUserGold, ","));
    _userBean->setString(getScorewithComma(HallDataMgr::getInstance()->m_dBean, ","));
    _userIngot->setString(getScorewithComma(HallDataMgr::getInstance()->m_lGold, ","));
    
}
void ShopScene::sendExchangeParameter()
{
    if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD)
    {
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_QUERY, NULL, 0, NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        NetworkMgr::getInstance()->sendData(MDM_GR_EXCHANGE, SUB_GR_EXCHANGE_LOAD_INFO, NULL, 0);
    }
}

void ShopScene::sendExchangeBean(DOUBLE beannum)
{
    if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD) {
        ST_CMD_GP_EXCHANGE_GAME_COIN_BY_BEAN request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.dExchangedBean = beannum;
        Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_GAME_COIN_BY_BEAN, &request, sizeof(request), NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        CMD_GR_EXCHANGE_GAME_COIN_BY_BEAN request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.dExchangedBean = beannum;
        Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GR_EXCHANGE, SUB_GR_EXCHANGE_GAME_COIN_BY_BEAN, &request, sizeof(request));
    }
}

void ShopScene::sendExchangeIngot(LONGLONG ingotnum)
{
    if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_LOAD) {
        ST_CMD_GP_EXCHANGE_GAME_COIN_BY_GOLD request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lExchangedGold = ingotnum;
        Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_GAME_COIN_BY_GOLD, &request, sizeof(request), NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == EM_DATA_TYPE_ROOM)
    {
        CMD_GR_EXCHANGE_GAME_GOIN request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lExchangedGold = ingotnum;
        Utf8ToUtf16(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GR_EXCHANGE, SUB_GR_EXCHANGE_GAME_COIN_BY_GOLD, &request, sizeof(request));
        
    }
}

void ShopScene::ExchangeParameterResult(void *pData, WORD wSize)
{
    auto result = (ST_CMD_GP_EXCHANGE_PARAM *)pData;
    m_IngotRate = result->wGoldExchangeGameCoinRate;
    m_BeanRate = result->wRateBeanExchangeGameCoin;
    
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    this->initShop();
   
}

void ShopScene::ExchangeResult(void *pData, WORD wSize)
{
     HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    auto result = (ST_CMD_GP_EXCHANGE_GAME_COIN_RESULT *)pData;
    if (result->bSucc)
    {
        HallDataMgr::getInstance()->m_lUserGold = result->lCurrGameCoin;
        HallDataMgr::getInstance()->m_dBean = result->dCurrBean;
        HallDataMgr::getInstance()->m_lGold = result->lCurrGold;
        
        //金币更新
        EventCustom event(whEvent_User_Data_Change);
        auto value = __Integer::create(User_Change_Score);
        event.setUserData(value);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        
        auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
        HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
        
        this->updateInfo();
        
    }
    else
    {
        auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
        HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
    }
}


void ShopScene::popShop()
{
    
    CallFunc *func = CallFunc::create([this]{
        HallDataMgr::getInstance()->AddpopLayer("", "正在加载,请稍后...", Type_Wait_Text);
        this->sendExchangeParameter();
        
    });
    
    auto run = MoveTo::create(0.2f, Vec2(.0,.0));
    
    this->runAction(Sequence::createWithTwoActions(run, func));
}
//MARK::EditDelegate
void ShopScene::editBoxReturn(EditBox* editBox)
{
    
    
    
}
void ShopScene::RealReuqcestEvent(cocos2d::EventCustom *event)
{
    __Integer *value = static_cast<__Integer *>(event->getUserData());
    
    if (value->getValue() == 0)
    {
        initReal();
        
    }else
    {
        _nameEidt->setText("");
        telEdit->setText("");
        _adressEidt->setText("");
        
        _nProvinceCode = -1;
        _nCityCode = -1;
        _nAreaCode = -1;
        
        auto node = _realExchangeLayout->getChildByTag(1001);
        if (nullptr != node)
        {
            auto textProvince = static_cast<Text *>(node->getChildByName("Text_select_p"));
            auto textCity = static_cast<Text *>(node->getChildByName("Text_select_c"));
            auto textArea = static_cast<Text *>(node->getChildByName("Text_select_a"));
            if (textProvince != nullptr && textCity != nullptr && textArea != nullptr)
            {
                
                
                textProvince->setString("请选择");
                textCity->setString("请选择");
                textArea->setString("请选择");
            }
        }
        

        
    }
    
}

//MARK::HTTP
void ShopScene::ListRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
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
    log("%s\n",backdata.c_str());
    
    
    if (_httpType == HTTP_LIST)
    {
        
          m_bShopData = true;
     
        rapidjson::Document doc;
        doc.Parse<0>(backdata.c_str());
        const rapidjson::Value &val = DICTOOL->getSubDictionary_json(doc,"data");
        int num = DICTOOL->getArrayCount_json(val, "List");
        m_Datalist.clear();
        for (int index=0; index<num; ++index)
        {
            
            const rapidjson::Value &checkdata = DICTOOL->getDictionaryFromArray_json(val, "List", index);
            

            auto list = new ListData();
            
            list->_awardName = DICTOOL->getStringValue_json(checkdata, "AwardName");
            list->_listFile = DICTOOL->getStringValue_json(checkdata, "SmallImage");
            list->_price = DICTOOL->getFloatValue_json(checkdata, "Price");
            list->_awardID = DICTOOL->getIntValue_json(checkdata, "AwardID");
            
            m_Datalist.push_back(list);
            
        }
        EventCustom event(REAL_REQUEST_EVENT);
        __Integer *value = __Integer::create(0);
        event.setUserData(value);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        
    
    }else
    {
        
    
        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
        rapidjson::Document doc;
        doc.Parse<0>(backdata.c_str());
        const rapidjson::Value &val = DICTOOL->getSubDictionary_json(doc,"data");
        bool vailid = DICTOOL->getBooleanValue_json(val, "valid");
        if (!vailid)
        {
            
            std::string tips = DICTOOL->getStringValue_json(doc, "msg");
            HallDataMgr::getInstance()->AddpopLayer("提示", tips, Type_Ensure);
        }else
        {
             HallDataMgr::getInstance()->AddpopLayer("提示", "您的兑换申请已经提交,客户会尽快受理您的申请", Type_Ensure);
            
        }
        
        EventCustom event(REAL_REQUEST_EVENT);
        __Integer *value = __Integer::create(1);
        event.setUserData(value);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        
        
    }
}

void ShopScene::buttoneEventWithRealExchange(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        
        
        if(strlen(_nameEidt->getText())==0 )
        {
            HallDataMgr::getInstance()->AddpopLayer("提示", "请输入收货人姓名", Type_Ensure);
            return;
        }
        
        if (strlen(telEdit->getText())==0)
        {
             HallDataMgr::getInstance()->AddpopLayer("提示", "请输入收货人电话号码", Type_Ensure);
            return;
        }
        
        if ((_nAreaCode==-1)||(_nCityCode==-1)||(_nAreaCode==-1))
        {
            HallDataMgr::getInstance()->AddpopLayer("提示", "请输入收货地址", Type_Ensure);
            return;
        }
        
        if (strlen(_adressEidt->getText())==0)
        {
             HallDataMgr::getInstance()->AddpopLayer("提示", "请输入收货人详细地址", Type_Ensure);
            return;
        }
        
    
       
        _httpType = HTTP_PURCHASE;
        auto prequest = new cocos2d::network::HttpRequest;
        auto purl = __String::createWithFormat("%s",URL_PURCHASE);
        prequest->setUrl(purl->getCString());
        prequest->setRequestType(cocos2d::network::HttpRequest::Type::POST);
        
        std::string postData = "";
        
         LONGLONG _time = getsystemtomillisecond() - HallDataMgr::getInstance()->m_Logintime;
        postData += __String::createWithFormat("userID=%u%s",
                                               HallDataMgr::getInstance()->m_dwUserID,
                                               HallDataMgr::getInstance()->getSignature(_time).c_str())->getCString();
        postData += "&";
        postData += __String::createWithFormat("awardID=%d&counts=%d&name=%s&phone=%s&province=%d&city=%d&area=%d&address=%s",m_nExchangeID,m_nExchangeCount,_nameEidt->getText(),telEdit->getText(),_nProvinceCode,_nCityCode,_nAreaCode,_adressEidt->getText())->getCString();
        
        
        prequest->setRequestData(postData.c_str(), postData.size());
        prequest->setResponseCallback(CC_CALLBACK_2(ShopScene::ListRequestCallback, this));
        cocos2d::network::HttpClient::getInstance()->send(prequest);
        prequest->release();
        
        
        HallDataMgr::getInstance()->AddpopLayer("", "正在加载,请稍后...", Type_Wait_Text);

    }
    
}
