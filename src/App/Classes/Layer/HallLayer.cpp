/************************************************************************************
 * file: 		HallLayer.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/23 23:08:24
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/23			
 *
 ************************************************************************************/

#include "HallLayer.h"

USING_NS_CC;

//************************************************************************
// 	Function:	createScene
//	Privilege:	public
//	Create:		2017/07/14
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		bool
//************************************************************************
Scene* HallLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HallLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

//************************************************************************
// 	Function:	init
//	Privilege:	public
//	Create:		2017/07/12
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		bool
//************************************************************************
bool HallLayer::init()
{
	if (!Layer::init())
		return false;

	// 1.set scene information
	if (!ESLayer::initLayerBg(this, PNG_HALL_BG))
		return false;

	// 2.set menu item
	if (!initMenuItem())
		return false;

	return true;
}

//************************************************************************
// 	Function:	initialize menu item
//	Privilege:	
//	Create:		2017/07/24
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		void
//************************************************************************
bool HallLayer::initMenuItem()
{
	ST_MENU_ITEM_INFO stMenuItemInfo = {0};

	sprintf_s(stMenuItemInfo.szPNGMenuItem, sizeof(stMenuItemInfo.szPNGMenuItem), PNG_SETTING_UP.c_str());
	sprintf_s(stMenuItemInfo.szPNGNormal, sizeof(stMenuItemInfo.szPNGNormal), PNG_SETTING_UP.c_str());
	sprintf_s(stMenuItemInfo.szPNGSelect, sizeof(stMenuItemInfo.szPNGSelect), PNG_SETTING_DOWN.c_str());

	stMenuItemInfo.nX = 100;
	stMenuItemInfo.nY = 100;

	if (!ESLayer::initMenuItem(this, &stMenuItemInfo, CC_CALLBACK_1(HallLayer::menuItemSettingCallback, this)))
		return false;

	ST_MENU_ITEM_INFO stMenuItemInfo2 = {0};

	sprintf_s(stMenuItemInfo2.szPNGMenuItem, sizeof(stMenuItemInfo2.szPNGMenuItem), PNG_HELP_DOWN.c_str());
	sprintf_s(stMenuItemInfo2.szPNGNormal, sizeof(stMenuItemInfo2.szPNGNormal), PNG_HELP_UP.c_str());
	sprintf_s(stMenuItemInfo2.szPNGSelect, sizeof(stMenuItemInfo2.szPNGSelect), PNG_HELP_DOWN.c_str());

	stMenuItemInfo2.nX = 100;
	stMenuItemInfo2.nY = 200;

	if (!ESLayer::initMenuItem(this, &stMenuItemInfo2, CC_CALLBACK_1(HallLayer::menuItemSettingCallback, this)))
		return false;

	return true;
}

//************************************************************************
// 	Function:	menuItemSettingCallback
//	Privilege:	
//	Create:		2017/07/24
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		void
//************************************************************************
void HallLayer::menuItemSettingCallback(cocos2d::Ref * pSender)
{
	log("进入设置页面 HallLayer::menuItemSettingCallback(cocos2d::Ref * pSender)");

	//auto createSceneSettingLayer = SettingLayer::createScene();

	//Director::getInstance()->pushScene(createSceneSettingLayer);
}
