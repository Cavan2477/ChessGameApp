#ifndef __nn_PlazaLayer_H__
#define __nn_PlazaLayer_H__

#include "CocosHeader.h"
#include "RoomListLayer.h"
#include "TableListLayer.h"
#include "CommonPlazaLayer.h"
#include "HeaderRequest.h"

#include "PasswdLayer.h"
#include "AntiCheatLayer.h"
#include "ClipText.h"

#include "NiuNiuScene.h"
#include "nn_SettingLayer.h"

NN_NAMESPACE_BEGIN

class PlazaLayer:public UILayer
{
public:
    PlazaLayer();
    ~PlazaLayer();
    
    CREATE_FUNC(PlazaLayer);
    virtual bool init();
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen);
    CC_SYNTHESIZE(UIScene*,m_notify,NotifyScene);
public:
    void refreshTableList();
    void refreshCurrentTableInfo();
    void refreshTableInfo(tagTableItem *pItem);
    void refreshTableUser(const tagTableUserInfo &tagUser);
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void levelchange(void *pData, WORD wSize);
    void scorechange(void *pData, WORD wSize);
    //刷新用户金币显示，头像，昵称,等级刷新
    void notifyRefreshScore(cocos2d::EventCustom *event);
    
    void showTableList(bool bShow);
private:
    void initPlazeLayout();
    //void initBtn();
    //void initUserInfo();
    void initTableListInfo();
    void initPasswdLayout();
    void createTableList();
    void enterGameScene();
private:
    bool handlePlazaMsg(const int &sub,void *pData,const int &nLen);
    
    void showLevelInfo(bool bShow);
private:
    cocos2d::ui::Layout *m_btnLayout;
    cocos2d::ui::Button *m_btnTask;
    cocos2d::ui::Button *m_btnBack;
    cocos2d::Vec2 m_vcTaskPos;
    cocos2d::ui::Button *m_btnReward;
    cocos2d::Vec2 m_vcRewardPos;
    
    cocos2d::ui::Layout *m_tableMenuLayout;
    
    //用户信息
    cocos2d::ui::Layout *m_userInfoLayout;
    cocos2d::ui::ImageView *m_imgUserHead;
    HeaderRequest *m_spHead;
    cocos2d::ui::Text *m_textNickName;
    ClipText *m_clipNickName;
    cocos2d::ui::LoadingBar *m_barLevel;
    cocos2d::ui::TextAtlas *m_atlasLevel;
    cocos2d::ui::TextAtlas *m_atlasJinBi;
    cocos2d::ui::TextAtlas *m_atlasJinDou;
    
    //桌子加密
    PasswdLayer *m_passwdLayer;
    
    //设置界面
    SettingLayer *m_settingLayer;
    
    //
    RoomListLayer *m_roomList;
    TableListLayer *m_tableList;
    CommonPlazaLayer *m_commonPlaza;
    
    bool m_bEnterTableList;
};

NN_NAMESPACE_END
#endif