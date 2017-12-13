/************************************************************************************
 * file: 		LoginScene.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/06 23:27:36
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/06			
 *
 ************************************************************************************/

#ifndef __LoginScene_h__
#define __LoginScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "../../BaseUI/Scene/ESScene.h"
#include "../../Pattern/Singleton.h"
#include <string>

using namespace std;

USING_NS_CC;

//#define TransFadeInLogin Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Login::createScene()))

// 登录服务器结构
typedef struct stLoginServerInfo
{
	bool bSelect;				// 所选服务器
	char szName[256];			// 服务器名称
	char szAddress[256];		// 服务器地址
}ST_LOGIN_SERVER_INFO, *PST_LOGIN_SERVER_INFO;

class LoginScene :
	public cocos2d::Layer,
	public cocos2d::ui::EditBoxDelegate//,
	//public cocos2d::extension::TableViewDataSource,
	//public cocos2d::extension::TableViewDelegate
{
public:
	//Login();
	~LoginScene();

	static cocos2d::Scene* createScene();

	//CREATE_FUNC(Login);

	// 初始化
	virtual bool init();

	virtual void onEnter();

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();

	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);

	//virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView* pTableView, ssize_t idx) override;

	//virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView* pTableView) override;

	//virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView* pTableView) override;

	//virtual void tableCellTouched(cocos2d::extension::TableView* pTableView, cocos2d::extension::TableViewCell* pTableViewCell) override;

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);

	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);

	void initLogin();
	void loadLoading();
	void loadFinish();
	void initServerList();
	void loadingCoinAnimateResFinish(Texture2D* pTexture);

	// 登录结果
	void loginResult(WORD wSubCmdID, void* pData, WORD wSize);

	// 房间列表
	void roomList(WORD wSubCmdID, void* pData, WORD wSize);

	// 登录失败处理
	void loginFailureEvent();

	void notifyConnectFailure(cocos2d::EventCustom* pEvent);

	//void refreshItemCell(cocos2d::extension::TableViewCell* pTableViewCell, const int& nIndex);

	void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox, const string& strText);
	void editBoxReturn(cocos2d::ui::EditBox* pEditBox);

	void buttonEventWithRemember(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType);
	void buttonEventWithRegist(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType);
	void buttonEventWithLogin(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType);
	void buttonEventWithVisitor(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType);
	void buttonEventWithThirdPlatformLogin(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType);
	void buttonEventWithServerSelect(Ref* target, cocos2d::ui::Widget::TouchEventType touchEventType);

public:

protected:
	cocos2d::ui::EditBox*	m_pAccountEditBox;
	cocos2d::ui::EditBox*	m_pPwdEditBox;

	//cocos2d::extension::TableView*	m_pTableView;

	cocos2d::Sprite*				m_pSpriteServerListBg;

	vector<stLoginServerInfo>		m_vecLoginServerList;

	float	m_fCount;

private:
	bool	m_bRemember;
	bool	m_bSelectSave;
};

#endif // __LoginScene_h__

