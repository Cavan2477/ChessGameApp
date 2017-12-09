/************************************************************************************
 * file: 		HallLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/22 10:31:54
 * Description: game hall layer
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/22			
 *
 ************************************************************************************/

#ifndef __HallLayer_h__
#define __HallLayer_h__

#pragma once

#include "cocos2d.h"
#include "../Common/GlobalConst.h"
#include "../BaseUI/Layer/ESLayer.h"
#include "../Layer/SettingLayer.h"

class HallLayer : public cocos2d::Layer
{
public:
	// create scene
	static cocos2d::Scene* createScene();

	// init
	virtual bool init();

	// initialize menu item 
	bool initMenuItem();

	// menu item callback function
	void menuItemSettingCallback(cocos2d::Ref * pSender);

	CREATE_FUNC(HallLayer);

protected:

private:

// field
public:

protected:

private:
	
};

#endif // __HallLayer_h__

