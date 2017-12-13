/************************************************************************************
 * file: 		AreaPicker.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:13:02
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __AreaPicker_h__
#define __AreaPicker_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class Locate
{
    Locate():
    _province("")
    ,_city("")
    ,_area("")
    ,_code("")
    {
    }
    
    ~Locate(){};
    
private:
    std::string    _province;    //省(直辖市)
    std::string    _city;        //市
    std::string    _area;        //区
    std::string    _code;        //编号
};

class AreaList : public Ref
{
};

class AreaPicker:public Layer
{
public:
    AreaPicker();
    ~AreaPicker();
    CREATE_FUNC(AreaPicker);
    virtual bool init();

public:
    void loadConfig();
    void openPicker(int nindex);
    void PickedEnd();
    void popPicker();
    int  getProvinceCode() const {return _selectProvince;};
    int  getCityCode() const {return _selectCity;};
    int  getAreaCode() const {return _selectArea;};

public :
    int     _nIndex;    //默认0省 1市 2区
    
    std::function<void(std::string,int,int)> _selectCallback;

private:
    ListView * _list;
    
    int     _selectProvince ;
    int     _selectCity;
    int    _selectArea;
};

#endif /* AreaPicker_h */
