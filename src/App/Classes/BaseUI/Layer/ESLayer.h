/************************************************************************************
 * file: 		ESLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/24 7:18:44
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/24			
 *
 ************************************************************************************/

#ifndef __ESScene_h__
#define __ESScene_h__

#pragma once

#include "cocos2d.h"
#include "../../Common/Global.h"

// Layer Information
class LayerInfo
{
public:
	cocos2d::String		pngBg;
};

// MenuItem Information
class MenuItemInfo
{
public:
	cocos2d::String		pngBg;
};

class ESLayer
{
public:
	// Initialize layer
	static bool initLayer(cocos2d::Layer* pLayer, LayerInfo* pLayerInfo);

	// Initialize menuitem
	static bool initMenuItem(cocos2d::Layer* pLayer, MenuItemInfo* pMenuItemInfo);
};
#endif // __ESScene_h__

