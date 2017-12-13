/************************************************************************************
* file: 		LoginScene.cpp
* copyright:	Cavan.Liu 2017
* Author: 		Cavan.Liu
* Create: 		2017/12/06 23:27:36
* Description:
* Version	Author		Time			Description
* V1.0    	Cavan.Liu	2017/12/06
*
************************************************************************************/

#include "LoginScene.h"

using namespace cocos2d::ui;

USING_NS_CC;
USING_NS_CC_EXT;

LoginScene::~LoginScene()
{
	//DebugLog("Login Release");
}

Scene* LoginScene::createScene()
{
	string strPublicRes = "public_res/public_res.plist";

	if (nullptr != INSTANCE(Director)->getTextureCache()->getTextureForKey(strPublicRes))
	{
		INSTANCE(SpriteFrameCache)->removeSpriteFramesFromFile(strPublicRes);
		INSTANCE(Director)->getTextureCache()->removeTextureForKey(strPublicRes);
	}

	INSTANCE(Director)->getTextureCache()->removeUnusedTextures();
	INSTANCE(SpriteFrameCache)->removeUnusedSpriteFrames();

	FileUtils::getInstance()->setSearchPaths(std::vector<std::string>());

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoginScene::create();

	// ×÷ÓÃ£¿
	layer->setTag(10);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

//************************************************************************
// 	Function:	init
//	Privilege:	
//	Create:		2017/12/10
//	Author:		Cavan.Liu
//	Parameter:	
//	Return:		bool
//************************************************************************
bool LoginScene::init()
{
	if (!Layer::init())
		return false;

	m_bSelectSave = false;

	return true;
}

void LoginScene::onEnter()
{

}

void LoginScene::onEnterTransitionDidFinish()
{

}

void LoginScene::onExit()
{

}

void LoginScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent)
{

}

/*
cocos2d::extension::TableViewCell* Login::tableCellAtIndex(cocos2d::extension::TableView* pTableView, ssize_t idx)
{

}

ssize_t Login::numberOfCellsInTableView(cocos2d::extension::TableView* pTableView)
{

}

cocos2d::Size Login::cellSizeForTable(cocos2d::extension::TableView* pTableView)
{

}

void Login::tableCellTouched(cocos2d::extension::TableView* pTableView, cocos2d::extension::TableViewCell* pTableViewCell)
{

}
*/

void LoginScene::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{

}

void LoginScene::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{

}

void LoginScene::initLogin()
{

}

void LoginScene::loadLoading()
{

}

void LoginScene::loadFinish()
{

}

void LoginScene::initServerList()
{

}

void LoginScene::loadingCoinAnimateResFinish(Texture2D* pTexture)
{

}

void LoginScene::loginResult(WORD wSubCmdID, void* pData, WORD wSize)
{

}

void LoginScene::roomList(WORD wSubCmdID, void* pData, WORD wSize)
{

}

void LoginScene::loginFailureEvent()
{

}

void LoginScene::notifyConnectFailure(cocos2d::EventCustom* pEvent)
{

}

void LoginScene::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox, const string& strText)
{

}

void LoginScene::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{

}

void LoginScene::buttonEventWithRemember(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginScene::buttonEventWithRegist(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginScene::buttonEventWithLogin(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginScene::buttonEventWithVisitor(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginScene::buttonEventWithThirdPlatformLogin(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginScene::buttonEventWithServerSelect(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}
