/************************************************************************************
 * file: 		AreaPicker.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:24:43
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "AreaPicker.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/DictionaryHelper.h"
#include "../../Common/PublicDefine.h"
#include "../../DataMgr/HallDataMgr.h"

using namespace cocostudio;

//MARK::AreaPicker
AreaPicker::AreaPicker():
_nIndex(0)
,_selectProvince(-1)
,_selectCity(-1)
,_selectArea(-1)
,_list(nullptr)
,_selectCallback(nullptr)
{
    
}

AreaPicker::~AreaPicker()
{
    
    _selectCallback = nullptr;
}

bool AreaPicker::init()
{

    if (!Layer::init())
    {
        
        return false;
    }
    
    auto button = Button::create();
    button->setTouchEnabled(true);
    button->setScale9Enabled(true);
    button->setContentSize(WIN_SIZE);
    button->setPosition(WIN_SIZE/2);
    button->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
        
        
        if (type == Widget::TouchEventType::ENDED)
        {
            
            this->setVisible(false);
        }
    });
    this->addChild(button);
    
    
    ImageView *pickerImg = ImageView::create();
    pickerImg->setScale9Enabled(true);
    pickerImg->setTouchEnabled(true);
    pickerImg->setContentSize(Size(Director::getInstance()->getWinSize().width, 400));
    pickerImg->setAnchorPoint(Vec2(.0, .0));
    pickerImg->setPosition(Vec2(.0, .0));
    this->addChild(pickerImg);
    
    LayerColor *pikerBG = LayerColor::create(Color4B::WHITE);
    pikerBG->setContentSize(Size(Director::getInstance()->getWinSize().width, 400));
    this->addChild(pikerBG);
   

    _list = ListView::create();
    _list->setContentSize(Size(this->getContentSize().width - 20, 380));
    _list->setBounceEnabled(true);
    _list->setDirection(ui::ScrollView::Direction::VERTICAL);
    _list->setItemsMargin(40.0f);
    _list->setAnchorPoint(Vec2(.0, .0));
    _list->setPosition(Vec2(10, 0));
    this->addChild(_list);
    

    return true;
}

void AreaPicker::openPicker(int nindex)
{
    assert( nindex >= 0 && nindex <= 2);
    
    _list->removeAllChildren();
    
    if (nindex == 0) //省
    {
        std::map<int,std::string> province = HallDataMgr::getInstance()->m_adderssList.at("0");
        
        int pos = 0;
        for (auto iter : province)
        {
            std:: string str = province.at(pos);
            auto _province = Button::create();
            _province->setTitleText(str);
            _province->setTitleColor(Color3B::BLACK);
            auto _layout = Layout::create();
            _layout->setContentSize(Size(_list->getContentSize().width , _province->getContentSize().height + 40));
            _province->setAnchorPoint(Vec2(.5, .5));
            _province->setTitleFontSize(30);
            _province->setTitleFontName(FONT_ARIAL);
            //_province->setTitleAlignment(TextHAlignment::CENTER);
            _province->setPosition(_layout->getContentSize()/2);
            _province->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED)
                {
                    _selectProvince = pos;
                    if (_selectCallback != nullptr)
                    {
                        
                        _selectCallback(str,_selectProvince,nindex);
                    }
                    this->setPosition(Vec2(.0, -400));
                    this->setVisible(false);
                }
                
            });
            _layout->addChild(_province);
            _list->pushBackCustomItem(_layout);
            pos ++;
        }
    }else  if (nindex == 1) //市
    {
        
        
         std::string citykey = __String::createWithFormat("0-%d",_selectProvince)->getCString();
         std::map<int,std::string> city = HallDataMgr::getInstance()->m_adderssList.at(citykey);
        int pos = 0;
        for (auto iter : city)
        {
            std:: string str = city.at(pos);
            auto _city = Button::create();
            _city->setTitleText(str);
            _city->setTitleColor(Color3B::BLACK);
            auto _layout = Layout::create();
            _layout->setContentSize(Size(_list->getContentSize().width , _city->getContentSize().height + 40));
            _city->setAnchorPoint(Vec2(.5, .5));
            _city->setTitleFontSize(30);
            _city->setTitleFontName(FONT_ARIAL);
            //_city->setTitleAlignment(TextHAlignment::CENTER);
            _city->setPosition(_layout->getContentSize()/2);
            _city->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED)
                {
                    _selectCity = pos;
                    if (_selectCallback != nullptr)
                    {
                        
                        _selectCallback(str,_selectCity,nindex);
                    }
                    this->setPosition(Vec2(.0, -400));
                    this->setVisible(false);
                }
                
            });
            _layout->addChild(_city);
            _list->pushBackCustomItem(_layout);
            pos ++;
        }

    }else                //区
    {
        std::string citykey = __String::createWithFormat("0-%d",_selectProvince)->getCString();
        //区县读取
        std::string areakey = citykey + __String::createWithFormat("-%d",_selectCity)->getCString();
        std::map<int,std::string> area = HallDataMgr::getInstance()->m_adderssList.at(areakey);
        int pos = 0;
        for (auto iter : area)
        {
            std:: string str = area.at(pos);
            auto _area = Button::create();
            _area->setTitleText(str);
            _area->setTitleColor(Color3B::BLACK);
            auto _layout = Layout::create();
            _layout->setContentSize(Size(_list->getContentSize().width , _area->getContentSize().height + 40));
            _area->setAnchorPoint(Vec2(.5, .5));
            _area->setTitleFontSize(30);
            _area->setTitleFontName(FONT_ARIAL);
            //_area->setTitleAlignment(TextHAlignment::CENTER);
            _area->setPosition(_layout->getContentSize()/2);
            _area->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED)
                {
                    _selectArea = pos;
                    if (_selectCallback != nullptr)
                    {
                        _selectCallback(str,_selectArea,nindex);
                    }
                    this->setPosition(Vec2(.0, -400));
                    this->setVisible(false);
                    
                }
                
            });
            _layout->addChild(_area);
            _list->pushBackCustomItem(_layout);
            pos ++;
        }

    }
}

void AreaPicker::loadConfig()
{

    

}

void AreaPicker::PickedEnd()
{
    

}

void AreaPicker::popPicker()
{

    this->runAction(MoveTo::create(0.2f, Vec2(0, 0)));
    
}
