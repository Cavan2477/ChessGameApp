/************************************************************************************
* file: 		LoginLayer.cpp
* copyright:	Cavan.Liu 2017
* Author: 		Cavan.Liu
* Create: 		2017/12/06 23:27:36
* Description:
* Version	Author		Time			Description
* V1.0    	Cavan.Liu	2017/12/06
*
************************************************************************************/

#include "LoginLayer.h"

using namespace cocos2d::ui;

USING_NS_CC;
USING_NS_CC_EXT;

LoginLayer::~LoginLayer()
{
	//DebugLog("Login Release");
}

Scene* LoginLayer::createScene()
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
	auto layer = LoginLayer::create();

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
bool LoginLayer::init()
{
	if (!Layer::init())
		return false;

	m_bSelectSave = false;

	return true;
}

void LoginLayer::onEnter()
{

}

void LoginLayer::onEnterTransitionDidFinish()
{

}

void LoginLayer::onExit()
{

}

void LoginLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent)
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

void LoginLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{

}

void LoginLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{

}

void LoginLayer::initLogin()
{

}

void LoginLayer::loadLoading()
{

}

void LoginLayer::loadFinish()
{

}

void LoginLayer::initServerList()
{

}

void LoginLayer::loadingCoinAnimateResFinish(Texture2D* pTexture)
{

}

void LoginLayer::loginResult(WORD wSubCmdID, void* pData, WORD wSize)
{

}

void LoginLayer::roomList(WORD wSubCmdID, void* pData, WORD wSize)
{

}

void LoginLayer::loginFailureEvent()
{

}

void LoginLayer::notifyConnectFailure(cocos2d::EventCustom* pEvent)
{

}

void LoginLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox, const string& strText)
{

}

void LoginLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{

}

void LoginLayer::buttonEventWithRemember(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginLayer::buttonEventWithRegist(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginLayer::buttonEventWithLogin(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginLayer::buttonEventWithVisitor(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginLayer::buttonEventWithThirdPlatformLogin(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}

void LoginLayer::buttonEventWithServerSelect(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType)
{

}
