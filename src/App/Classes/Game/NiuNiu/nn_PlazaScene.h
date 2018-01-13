/************************************************************************************
 * file: 		nn_PlazaScene.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:49:56
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __NN_PLAZA_SCENE_H__
#define __NN_PLAZA_SCENE_H__

#include "CocosHeader.h"
#include "Header/NiuNiuScene.h"
#include "../../DataMgr/TableMgr.h"

NN_NAMESPACE_BEGIN

class PlazaScene:public UIScene
{
public:
	PlazaScene();
	~PlazaScene();

	CREATE_FUNC(PlazaScene);
	SCENE_BIND(NN_SCENE(PLAZA_SCENE));

	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
	// UI消息处理
	virtual void HanderMessage(int main, int sub, BYTE *pBuffer, int size);
    //桌子消息
    void onTableMsgCallBack(const enTableCode& nCode, void *pData, const int& nSize);
    // ui线程消息
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    CC_SYNTHESIZE(bool, m_bIsReEnter, ReEnter);
    
    CC_SYNTHESIZE(bool, m_bBackToPlazz, BackToPlazz);
private:
    ///////////////////////// 网络    
    //等级升级
    void levelUpgrade(void* pData, WORD wSize);
    //等级信息
    void levelInfo(void* pData, WORD wSize);
    //刷新用户金币显示，头像，昵称,等级刷新
    void notifyRefreshScore(cocos2d::EventCustom *event);
    //房间登陆消息
    void roomloginResult(WORD wSubCmdID, void *pData, WORD wSize);
    //房间列表消息
    void roomlistResult(WORD wSubCmdID, void *pData, WORD wSize);
    /////////////////////////
	//接收到状态消息
	void onGetPlazaStatusMsg(const int &sub,BYTE *pData,const int &nLen);
	//接收到ui线程消息
	void onGetPlazaUIMsg(const int &sub,void *pData,const int &nLen);
private:
    void sendPacketWithUserLevelInfo();
};

NN_NAMESPACE_END
#endif