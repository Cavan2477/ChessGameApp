#ifndef __nn_GameScene_H__
#define __nn_GameScene_H__

#include "CocosHeader.h"
#include "NiuNiuScene.h"
#include "GameSceneUI/GameLayer.h"

NN_NAMESPACE_BEGIN
class GameScene:public UIScene,
    public UserItemDelegate
{
public:
	GameScene();
	~GameScene();

	CREATE_FUNC(GameScene);
	/*
	 * @brief 创建游戏场景
	 * @param[wTable] 进入的桌子
	 * @param[wChair] 进入的椅子
	 * @param[bSitDown] 是否主动请求入座（防作弊房间自动入席）
	 */
	static GameScene* createGameScene(const WORD &wTable,const WORD &wChair,bool bSitDown);
    
    static GameScene* createGameScene(bool bReEnter);

	SCENE_BIND(NN_SCENE(GAME_SCENE));
	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
	// ui线程消息
	virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    //

	CC_SYNTHESIZE(WORD,m_wGameTable,GameTable);
	CC_SYNTHESIZE(WORD,m_wGameChair,GameChair);
	CC_SYNTHESIZE(bool,m_bSitDown,SitDown);
    //点击确认离开游戏
    CC_SYNTHESIZE(bool, m_bClickExit, ClickExit);
private:
	//接收到ui线程消息
	bool onGetGameUIMsg(const int &sub,void *pData,const int &nLen);
private:
	////////////////////////////////////////////////////////////////////////// 网络消息
    //游戏场景消息
    void onGameScene(void *pData, WORD wSize);
    //用户进入
    void OnUserEnter(void* pUser) override;
    //用户状态
    void OnUserStatus(void* pUser) override;
    //用户分数
    void OnUserScore(void* pUser) override;
    //用户聊天
    virtual void OnUserChat(WORD wSubCode, void* pData, WORD wSize) override;
    
    //游戏消息
    void onGameMessage(WORD wSubCmdID, void *pData, WORD wSize);
    //房间登陆结果
    void roomloginResult(WORD wSubCmdID, void *pData, WORD wSize);
    
	//处理登陆消息
	void onHandleGameLoginMsg(const int &sub, BYTE *pBuffer, const int &size);
	//处理配置消息
	void onHandleGameConfigMsg(const int &sub, BYTE *pBuffer, const int &size);

	//处理user消息
	void onHandleUserMsg(const int &sub,BYTE *pBuffer, const int &size);
	void onGetUserStatus(BYTE *pBuffer, const int &size);
    void onGetUserTextChat(void *pBuffer);
    void onGetUserBrowChat(void *pBuffer);

	//处理场景消息
	void onHandleFrameMsg(const int &sub, BYTE *pBuffer, const int &size);

	/*
	 * 处理游戏消息
	 */
	void onHandleGameMsg(const int &sub, BYTE *pBuffer,const int &size);
	//接收到游戏开始
	void onGetStartGame(BYTE *pBuffer,const int &size);
	//接收到用户叫庄
	void onGetCallBanker(BYTE *pBuffer,const int &size);
	//接收到玩家加注
	void onGetAddScore(BYTE *pBuffer,const int &size);
	//接收到发牌
	void onGetSendCard(BYTE *pBuffer,const int &size);
    //接收到摊牌
    void onGetOpenCard(BYTE *pBuffer,const int &size);
    //接收到游戏结束
    void onGetGameOver(BYTE *pBuffer, const int &size);
	//////////////////////////////////////////////////////////////////////////
private:
    //离开游戏
    void notifyLeaveGame(cocos2d::EventCustom *event);
    
	void replaceScene();
    
    void removeNetMessage();
    
    //加载资源
    void loadRes();
    //释放资源
    void unloadRes();
private:
	GameLayer *m_gameLayer;
};

NN_NAMESPACE_END

#endif