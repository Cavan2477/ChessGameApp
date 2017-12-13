/************************************************************************************
 * file: 		SettingLayer.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/22 9:34:16
 * Description: Setting layer
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/22			
 *
 ************************************************************************************/

#include "SettingScene.h"

USING_NS_CC;

Scene* SettingScene::createScene()
{
	return Scene::create();
}

//************************************************************************
// 	Function:	init
//	Privilege:	public static
//	Create:		2017/07/22
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		bool
//************************************************************************
bool SettingScene::init()
{
	if (!Layer::init())
		return false;

	// 1.set scene information
	if (!ESScene::initLayerBg(this, PNG_SETTING_BG))
		return false;

	// 2.

	return true;
}

