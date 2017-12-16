//
//  GameBankLayer.h
//  MyGame
//
//  Created by wh on 15/6/23.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "define.h"
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN
class GameBankLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(GameBankLayer)
    
    virtual bool init();
    
    void openinit();
    
    void accessinit();
    
    void updateScore();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    bool checkStrlength(const std::string &str, const std::string &title);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    //发送开通银行
    void sendInsureEnable(const std::string &pass);
    //查询银行
    void sendInsureInfo();
    //取款
    void sendTakeScore(SCORE score, const std::string &pass);
    
    //游戏消息回包
    void BankInsureResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
};

LKPY_NAMESPACE_END
