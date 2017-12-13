/************************************************************************************
 * file: 		Shop.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:08:17
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __Shop_h__
#define __Shop_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../Common/MacroDefine.h"
#include "../../Common/CMD_Stdafx.h"
#include "AreaPicker.h"
#include "network/HttpClient.h"

USING_NS_CC;

using namespace std;

#define URL_LIST    "http://jd.foxuc.net/WS/shop.ashx?action=mobilegetawardlist"
#define URL_PURCHASE  "http://jd.foxuc.net/WS/shop.ashx?action=mobilebuyaward"

class ListData : public Ref
{
public:
    ListData():
    _awardID(0)
    ,_price(.0)
    ,_listFile("")
    ,_awardName("")
    {
        
    };
    ~ListData(){};
public:
    
public:
    float           _price;             //价格
    std::string     _listFile;          //商品图片
    std::string     _awardName;         //商品名称
    
    int             _awardID;           //商品id
    
    
};
class ShopList : public Button
{
public:
    ShopList();
    ~ShopList();
    
    static ShopList *create();
    virtual bool init();
    virtual void onExit();
    
    Texture2D *getTextureByID(uint64_t ID);
    void TexureReuqcestEvent(cocos2d::EventCustom *event);
    void setData(ListData *data);    
public:
    CC_SYNTHESIZE(Label*, _price, Price);
    CC_SYNTHESIZE(Sprite*,_cellImage, CellImage);//商品图片
    CC_SYNTHESIZE(Label*, _awardName, Award);  //商品名称
    
    int _nAwardID;
    cocos2d::Map<int ,cocos2d::Texture2D *> _listRes;
};

class Shop : public Layer,public cocos2d::ui::EditBoxDelegate
{
public:
    Shop();
    virtual ~Shop();

    bool init();

    CREATE_FUNC(Shop);
    
public:
    void initShop();
    void initPurchase();
    void initReal();
    void initDetail(ListData *data,ShopList *list);
    void updateInfo();
    void popShop();
    void setIndex(int nindex);

    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    void sendExchangeParameter();
    
    void sendExchangeBean(DOUBLE beannum);
    
    void sendExchangeIngot(SCORE ingotnum);
    
    //大厅消息回包处理
    //兑换数据
    void ExchangeParameterResult(void* pData, WORD wSize);

    //兑换结果
    void ExchangeResult(void* pData, WORD wSize);
    void ListRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void editBoxReturn(EditBox* editBox);
    
    void buttoneEventWithRealExchange(Ref *ref,cocos2d::ui::Widget::TouchEventType type);
    
    void RealReuqcestEvent(cocos2d::EventCustom *event);
    
private:
    CC_SYNTHESIZE_RETAIN(Label *, _userScore, UserScore);
    CC_SYNTHESIZE_RETAIN(Label *, _userIngot, UserIngot);
    CC_SYNTHESIZE_RETAIN(Label *, _userBean, UserBean);
    
    cocos2d::ui::Layout         *_coinExchangeLayout;
    cocos2d::ui::Layout         *_beanPurchaseLayout;
    cocos2d::ui::Layout         *_realExchangeLayout;
    
    
    DWORD m_IngotRate;  //元宝兑换比率
    
    DWORD m_BeanRate;   //游戏豆兑换比率
    
    bool  m_bShopData;
    
    typedef enum
    {
        
        HTTP_LIST = 0,   //商品列表
        HTTP_PURCHASE,   //购买
        
    }HttpType;
    
    HttpType _httpType;
    
    std::vector<ListData *> m_Datalist;
    
    Node    *_rootNode;
    
    AreaPicker *_picker;
    
    
    int      _nProvinceCode;
    int      _nCityCode;
    int      _nAreaCode;
    
    EditBox     *_nameEidt;
    EditBox     *telEdit;
    EditBox     *_adressEidt;
    
    int         m_nExchangeCount;
    int         m_nExchangeID;
};

#endif /* Shop_hpp */
