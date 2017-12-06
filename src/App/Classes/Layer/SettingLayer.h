/************************************************************************************
 * file: 		SettingLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/22 10:34:18
 * Description: Setting layer
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/22			
 *
 ************************************************************************************/

#ifndef __SettingLayer_h__
#define __SettingLayer_h__

#pragma once

#include "cocos2d.h"
#include "../Common/GlobalConst.h"
#include "../BaseUI/Layer/ESLayer.h"

class SettingLayer : public cocos2d::Layer
{
public:
	// create scene
	static cocos2d::Scene* createScene();

	// initialize 
	virtual bool init();

	CREATE_FUNC(SettingLayer);

protected:

private:

};

#endif // __SettingLayer_h__

