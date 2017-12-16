//
//  GameRoleInfoLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/31/15.
//
//

#ifndef nn_GameRoleInfoLayer_hpp
#define nn_GameRoleInfoLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "HeaderRequest.h"

NN_NAMESPACE_BEGIN

class GameRoleInfoLayer:public UILayer
{
public:
    GameRoleInfoLayer();
    ~GameRoleInfoLayer();
    
    CREATE_FUNC(GameRoleInfoLayer);
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    
    void refresh(UserData *pUser);
private:
    cocos2d::Sprite *m_spBg;
    cocos2d::Sprite *m_spHeadBg;
    cocos2d::Sprite *m_spGender;
    cocos2d::ui::Text *m_textName;
    cocos2d::ui::Text *m_textId;
    cocos2d::ui::Text *m_textMoney;
    cocos2d::ui::Text *m_textWin;
    HeaderRequest *m_pHead;
    
    cocos2d::EventListenerTouchOneByOne *m_listener;
};

NN_NAMESPACE_END

#endif /* GameRoleInfoLayer_hpp */
