//
//  CommonPlazaLayer.hpp
//  GameProject
//
//  Created by zhong on 3/25/16.
//
//

#ifndef CommonPlazaLayer_hpp
#define CommonPlazaLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "define.h"
#include "HeaderRequest.h"

class CommonPlazaLayer:public cocos2d::Layer
,public cocos2d::ui::EditBoxDelegate
{
public:
    CommonPlazaLayer();
    ~CommonPlazaLayer();
    
    CREATE_FUNC(CommonPlazaLayer);
    virtual bool init() override;
    
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    
    //等级升级
    void LevelUpgrade(void* pData, WORD wSize);
    
    //等级信息
    void LevelInfo(void* pData, WORD wSize);
    
    //刷新用户信息 头像,分数
    void notifyFreshInfo(cocos2d::EventCustom *event);
    
    //隐藏用户信息
    void hideUserInfo(bool bHide);
    
    //锁定机器
    void initBindMachine(const std::string &title);
    
    //修改操作成功
    void operatesuccessResult(void* pData, WORD wSize);
    
    //修改操作失败
    void operatefailureResult(void* pData, WORD wSize);
    
private:
    //button 点击回调
    void buttonEventWithTouchUser(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithBank(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithReward(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithShop(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithTask(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithSet(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithRank(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithShare(cocos2d::Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithLock(Ref* target,cocos2d::ui::Widget::TouchEventType tType);
    
private:
    cocos2d::ui::Layout *m_root;
    //大厅标题
    cocos2d::ui::ImageView *m_imageTitle;
    //列表背景
    cocos2d::Sprite *m_imageListBack;
    //用户头像
    HeaderRequest *_headSprite;
    //用户昵称
    cocos2d::Label *_UserNikcName;
    //用户分数
    cocos2d::Label *_UserScore;
    //用户等级
    cocos2d::Label *_UserLevel;
    //用户信息
    cocos2d::ui::Layout *m_infoLayout;
};
#endif /* CommonPlazaLayer_hpp */
