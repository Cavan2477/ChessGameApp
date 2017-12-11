/************************************************************************************
 * file: 		GameCreateScene.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/07/23 23:01:40
 * Description: create game scene
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/07/23			
 *
 ************************************************************************************/

#ifndef __GameCreateScene_h__
#define __GameCreateScene_h__

#pragma once

#include "../Common/GlobalConst.h"
#include "../BaseUI/Layer/ESScene.h"

class GameCreateScene : public cocos2d::Layer
{
public:
	// create scene
	static cocos2d::Scene* createScene();

	// init
	virtual bool init();

	// menu item
	void menuItemCreateRoomCallback(cocos2d::Ref * pSender);			
	void menuItemJoinRoomCallback(cocos2d::Ref * pSender);
	void menuItemPlayNoFeeCallback(cocos2d::Ref * pSender);

	CREATE_FUNC(GameCreateScene);

protected:

private:

	// field
public:

protected:

private:

};

#endif // __GameCreateScene_h__

