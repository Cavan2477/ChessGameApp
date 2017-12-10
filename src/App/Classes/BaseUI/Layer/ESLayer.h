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

USING_NS_CC;

using namespace std;

// They will be adjust to the plist/config file
#define RESOLUTION_WIDTH	1024
#define RESOLUTION_HEIGHT	768

#define SCREEN_DISTANCE_X	10
#define SCREEN_DISTANCE_Y	10

// menu item information structure
typedef struct stMenuItemInfo
{
	// scale png to adapter to screen position type enum
	enum EnumPosType
	{
		
	};

	char	szPNGMenuItem[MAX_PATH];		// menu item image 
	char	szPNGNormal[MAX_PATH];			// normal status
	char	szPNGSelect[MAX_PATH];			// selected status
	int		nX;								// x
	int		nY;								// y

	//ccMenuCallback ccMenuCallbackFunc;		// menu item callback function
}ST_MENU_ITEM_INFO, *PST_MENU_ITEM_INFO;

class ESLayer
{
public:
	// Initialize layer background
	static bool initLayerBg(cocos2d::Layer* pLayer, IN const string& strPngBg);

	// Initialize menu item
	static bool initMenuItem(cocos2d::Layer* pLayer, IN stMenuItemInfo* pstMenuItemInfo, IN const cocos2d::ccMenuCallback& menuCallback);
};
#endif // __ESScene_h__

