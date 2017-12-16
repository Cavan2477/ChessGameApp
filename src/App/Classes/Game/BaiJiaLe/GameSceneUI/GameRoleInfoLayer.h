//
//  GameRoleInfoLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/31/15.
//
//

#ifndef bjl_header_GameRoleInfoLayer_hpp
#define bjl_header_GameRoleInfoLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "HeaderRequest.h"
#include "ClipText.h"
#include "GameDefine.h"

BJL_NAMESPACE_BEGIN

class GameRoleInfoLayer:public UILayer
{
public:
    GameRoleInfoLayer();
    ~GameRoleInfoLayer();
    
    CREATE_FUNC(GameRoleInfoLayer);
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    
    void refresh(UserData *pUser, const enGameRoleDir &dir);
private:
    cocos2d::Sprite *m_spBg;
    cocos2d::Sprite *m_spHeadBg;
    ClipText *m_clipNickName;
    cocos2d::ui::Text *m_textId;
    cocos2d::ui::Text *m_textMoney;
    cocos2d::ui::Text *m_textBean;
    //cocos2d::ui::Text *m_textWin;
    HeaderRequest *m_pHead;
    /*
    cocos2d::ui::Text *m_textChengJi;
    cocos2d::ui::Text *m_textMingZhongLv;
    */
    cocos2d::EventListenerTouchOneByOne *m_listener;
    cocos2d::Action *m_actShowAct;
    cocos2d::Action *m_actHideAct;
};

BJL_NAMESPACE_END

#endif /* GameRoleInfoLayer_hpp */
