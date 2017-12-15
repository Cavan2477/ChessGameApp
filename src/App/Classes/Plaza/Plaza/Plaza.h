/************************************************************************************
 * file: 		Plazz.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:28:36
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __Plaza_h__
#define __Plaza_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../Common/MacroDefine.h"
#include "../Notice/Notice.h"

#include "Reward.h"
#include "Rank.h"
#include "Setting.h"
#include "DownManager.h"

USING_NS_CC;

#define TransFadeInPlazz  \
HallDataMgr::getInstance()->m_dwKindID = (DWORD)kind_default; \
Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Plazz::createScene()))

class Progress : public cocos2d::ui::ImageView
{
public:
    enum TAG_PROGRESS
    {
        TAG_Circle = 10
    };
    
public:
    Progress();
    virtual ~Progress();
    
    static Progress* create(const std::string &res);
    
    bool initProgress(const std::string &res);
    void runAction();
    void updatePercent(int percent);
    
private:
    CC_SYNTHESIZE_RETAIN(Label* , _percent, Percent);
    
};
class GameList:public cocos2d::ui::Button
{
public:
    GameList();
    virtual ~GameList();
    static GameList* create();
    virtual bool init();
public:
    void setInfo(DownLoadInfo &info);
    void addProgress();
    void removeProgress();
public:
    Progress  *m_pProgress;
    DownLoadInfo DownInfo;
    
};

class Plazz: public Layer
,public cocos2d::ui::EditBoxDelegate
{
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    Plazz();
    virtual bool init();
    virtual ~Plazz();
    CREATE_FUNC(Plazz);
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    
    //刷新用户信息 头像,分数
    void notifyFreshInfo(cocos2d::EventCustom *event);
    
    void notifyDownRefresh(cocos2d::EventCustom *event);
    
    //查询用户等级
    void sendPacketWithUserLevelInfo();
    
    //等级升级
    void LevelUpgrade(void* pData, WORD wSize);
    
    //等级信息
    void LevelInfo(void* pData, WORD wSize);
    
    //锁定机器
    void initBindMachine(const std::string &title);
    
    //修改操作成功
    void operatesuccessResult(void* pData, WORD wSize);
    
    //修改操作失败
    void operatefailureResult(void* pData, WORD wSize);
public:
    
    cocos2d::ui::Widget* initOneList(int index);
    //根据kindid创建游戏图标
    cocos2d::ui::Widget* initOneListByKind(const int &nKind);
    
    void buttonEventWithTouchUser(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithBank(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithReward(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithShop(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithTask(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithSet(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithRank(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithShare(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithLock(Ref* target,cocos2d::ui::Widget::TouchEventType tType);
    
public:
    void onTaskSuccess(LIST_Kind kind);
    void ontaskError(LIST_Kind kind);
    void onDecompressSuccess(LIST_Kind kind);
    void loadingGame(LIST_Kind game);
public:
    
private:
    cocos2d::ui::ListView*     _list;
    CC_SYNTHESIZE_RETAIN(Sprite*, _headSprite, HeadSprite);
    CC_SYNTHESIZE_RETAIN(Label*,_UserScore,UserScore);
    CC_SYNTHESIZE_RETAIN(Label*,_UserNikcName,UserNickName);
    CC_SYNTHESIZE_RETAIN(Label*,_UserLevel,UserLevel);
    
    cocos2d::ui::Button *m_btnBindMachine;
};

#endif /* Plazz_h */
