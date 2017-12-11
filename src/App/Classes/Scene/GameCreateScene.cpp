/************************************************************************************
 * file: 		GameCreateScene.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/23 23:08:31
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/23			
 *
 ************************************************************************************/

#include "GameCreateScene.h"

USING_NS_CC;

//************************************************************************
// 	Function:	createScene
//	Privilege:	public
//	Create:		2017/07/23
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		Scene*	
//************************************************************************
Scene* GameCreateScene::createScene()
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
bool GameCreateScene::init()
{
	if (!Layer::init())
		return false;

	// 1.set scene information
	if (!ESScene::initLayerBg(this, PNG_GAME_CREATE_BG))
		return false;

	// 2.set menu item

	return true;
}