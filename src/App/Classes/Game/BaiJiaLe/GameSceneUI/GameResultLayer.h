//
//  GameResultLayer.hpp
//  DouDiZhu
//
//  Created by zhong on 1/19/16.
//
//

#ifndef bjl_header_GameResultLayer_hpp
#define bjl_header_GameResultLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "GameDefine.h"
#include "ClipText.h"
#include "BaiJiaLeLogicDefine.h"

BJL_NAMESPACE_BEGIN

class GameResultLayer:public UILayer
{
public:
    UILAYER_WITHNOTIFY(GameResultLayer);
public:
    void hideGameResult();
    void showGameResult(const tagGameResult &rs);
    virtual void onEnter() override;
    virtual void onExit() override;
private:
    void reSet();
private:
    //触摸监听
    cocos2d::EventListenerTouchOneByOne *m_listener;
    cocos2d::Node *m_root;
    
    cocos2d::ui::TextAtlas *m_atlasScore[AREA_MAX];
    cocos2d::ui::TextAtlas *m_atlasTotal;
};

BJL_NAMESPACE_END
#endif /* GameResultLayer_hpp */
