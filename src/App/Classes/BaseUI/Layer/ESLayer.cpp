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
//	Parameter:	Layer*				pLayer
//				IN const string&	strPngBg
//	Return:		bool
//************************************************************************
bool ESLayer::initLayerBg(Layer* pLayer, IN const string& strPngBg)
{
	if (NULL == pLayer || strPngBg.empty())
		return false;

	// 1.init scene
	/*if (!Scene::init())
		return false;*/

	Director* pDirectorInstance = Director::getInstance();

	// 2.get visible size
	Size winSize = pDirectorInstance->getWinSize();

	SIZE_SCREEN = winSize;

	// 3.set background
	Sprite* pSpriteBg = Sprite::create(strPngBg);

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
// 	Function:	初始化菜单项
//	Privilege:	
//	Create:		2017/07/24
//	Author:		Cavan.Liu
//	Parameter:	cocos2d::Layer* pLayer
//				IN stMenuItemInfo* pstMenuItemInfo
//	Return:		bool
//************************************************************************
bool ESLayer::initMenuItem(cocos2d::Layer* pLayer, IN stMenuItemInfo* pstMenuItemInfo)
{
	if (NULL == pLayer || NULL == pstMenuItemInfo)
		return false;

	// 1.set menu item
	CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pstMenuItemInfo->szPNGMenuItem);

	CCSprite* pSpriteNormal = Sprite::create(pstMenuItemInfo->szPNGNormal);
	CCSprite* pSpriteSelect = Sprite::create(pstMenuItemInfo->szPNGSelect);

	auto sizePngNormal = pSpriteNormal->getTextureRect().size;

	pSpriteNormal->setScaleX(RESOLUTION_WIDTH/sizePngNormal.width);
	pSpriteNormal->setScaleY(RESOLUTION_HEIGHT/sizePngNormal.height);

	// todo 2017/12/8 to be continue
	//int nPngNormalX = SIZE_SCREEN.width - sizePngNormal.width - SCREEN_DISTANCE_X;
	//int nPngNormalY = SCREEN_DISTANCE_Y;

	MenuItemSprite* pMenuItemSprite = MenuItemSprite::create(pSpriteNormal, pSpriteSelect, CC_CALLBACK_1(pstMenuItemInfo->ccMenuCallbackFunc, pLayer));

	// 2.scale png to adapter to screen size and position
	// 2017/12/9 to be continue
	pMenuItemSprite->setPosition(Director::getInstance()->convertToGL(Vec2(pstMenuItemInfo->nX, pstMenuItemInfo->nY)));

	// 3.create menu item
	Menu* pMenu = Menu::create(pMenuItemSprite, NULL);

	// 4.set menu position
	pMenu->setPosition(Vec2::ZERO);

	// 5.add menu
	pLayer->addChild(pMenu);

	return true;
}
