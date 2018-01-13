/************************************************************************************
 * file: 		GameRoleInfoLayer.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 21:42:36
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __NN_GAME_ROLE_INFO_LAYER_HPP__
#define __NN_GAME_ROLE_INFO_LAYER_HPP__

#include <stdio.h>
#include "CocosHeader.h"
#include "../../../Public/HeaderRequest.h"

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
