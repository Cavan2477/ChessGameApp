#ifndef __bjl_header_PlazaScene_H__
#define __bjl_header_PlazaScene_H__

#include "CocosHeader.h"
#include "BaiJiaLeScene.h"

#include "PlazaSceneUI/PlazaLayer.h"

BJL_NAMESPACE_BEGIN
class PlazaScene:public UIScene
{
public:
	PlazaScene();
    
	~PlazaScene();
    
    //异步预加载资源
    static std::vector<tagUIResourceParam> uiAsynLoadResource();
    
    //资源加载结束
    static std::vector<tagUIResourceParam> uiAsynLoadFinished();
    
    //释放资源
    static std::vector<tagUIResourceParam> uiUnloadResource();

	CREATE_FUNC(PlazaScene);
	SCENE_BIND(BJL_SCENE(PLAZA_SCENE));

	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
	// UI消息处理
	virtual void HanderMessage(int main, int sub, BYTE *pBuffer, int size);
    // ui线程消息
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    
    CC_SYNTHESIZE(bool, m_bIsReEnter, ReEnter);
    
    void removeNetMessage();
    
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
private:
	PlazaLayer *m_plazaLayer;
};
BJL_NAMESPACE_END
#endif