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

#ifndef __HallScene_h__
#define __HallScene_h__

#pragma once

#include "cocos2d.h"
#include "../Common/GlobalConst.h"
#include "../Scene/SettingScene.h"
#include "../Scene/ExtBaseScene.h"

class HallScene : public cocos2d::Layer
{
public:
	// create scene
	static cocos2d::Scene* createScene();

	// init
	virtual bool init();

	// initialize menu item 
	bool initMenuItem();
	void initMyMenuItem();

	// menu item callback function
	void menuItemSettingCallback(cocos2d::Ref * pSender);

	CREATE_FUNC(HallScene);

protected:

private:

// field
public:

protected:

private:
	
};

#endif // __HallScene_h__

