/************************************************************************************
 * file: 		GameCreateLayer.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/23 23:08:31
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/23			
 *
 ************************************************************************************/

#include "GameCreateLayer.h"

USING_NS_CC;

//************************************************************************
// 	Function:	createScene
//	Privilege:	public
//	Create:		2017/07/23
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		Scene*	
//************************************************************************
Scene* GameCreateLayer::createScene()
{
	return Scene::create();
}

//************************************************************************
// 	Function:	init
//	Privilege:	public
//	Create:		2017/07/12
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		bool
//************************************************************************
bool GameCreateLayer::init()
{
	LayerInfo layerInfo;

	layerInfo.pngBg = PNG_GAME_CREATE_BG;

	if (!Layer::init())
		return false;

	// 1.set scene information
	if (!ESLayer::initLayer(this, &layerInfo))
		return false;

	// 2.set menu item

	return true;
}