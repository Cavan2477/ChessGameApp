/************************************************************************************
 * file: 		ESLayer.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/22 10:31:44
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/22			
 *
 ************************************************************************************/

#include "ESLayer.h"

USING_NS_CC;

//************************************************************************
// 	Function:	initLayer
//	Privilege:	public
//	Create:		2017/07/14
//	Author:		Cavan.Liu
//	Parameter:	Layer*		pLayer
//				LayerInfo*	pSceneInfo
//	Return:		bool
//************************************************************************
bool ESLayer::initLayer(Layer* pLayer, LayerInfo* pLayerInfo)
{
	if (NULL == pLayer || NULL == pLayerInfo)
		return false;

	// 1.init scene
	/*if (!Scene::init())
		return false;*/

	Director* pDirectorInstance = Director::getInstance();

	// 2.get visible size
	Size winSize = pDirectorInstance->getWinSize();

	SIZE_SCREEN = winSize;

	// 3.set background
	Sprite* pSpriteBg = Sprite::create(pLayerInfo->pngBg.getCString());

	if (NULL == pSpriteBg)
	{
		// todo 2017/07/12 to be continue log upload to server
		log("pSpriteBg is invalid.");
		return false;
	}

	pSpriteBg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	float fWinWidth = winSize.width;
	float fWinHeight = winSize.height;
	float fSpX = pSpriteBg->getTextureRect().getMaxX();
	float fSpY = pSpriteBg->getTextureRect().getMaxY();

	// 4.Set the elves width scaling
	pSpriteBg->setScaleX(fWinWidth/fSpX);
	pSpriteBg->setScaleY(fWinHeight/fSpY);

	pLayer->addChild(pSpriteBg);

	return true;
}

//************************************************************************
// 	Function:	initMenuItem
//	Privilege:	
//	Create:		2017/07/24
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		
//************************************************************************
bool ESLayer::initMenuItem(cocos2d::Layer* pLayer, MenuItemInfo* pMenuItemInfo)
{
	if (NULL == pLayer || NULL == pMenuItemInfo)
		return false;



	return true;
}
