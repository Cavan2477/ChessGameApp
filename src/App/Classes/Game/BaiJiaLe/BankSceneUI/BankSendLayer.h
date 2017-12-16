//
//  BankSendLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#ifndef bjl_header_BankSendLayer_hpp
#define bjl_header_BankSendLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "ButtonGroup.h"

BJL_NAMESPACE_BEGIN

struct tagSendParam
{
    SCORE sendScore;
    std::string passwd;
    std::string targetUser;
};

class BankSendLayer:public UILayer,
public ButtonGroupListen,
public cocos2d::ui::EditBoxDelegate
{
    UILAYER_WITHNOTIFY(BankSendLayer);
    virtual bool preButtonChecked(cocos2d::Node *pSender,bool bSelected) override;
    virtual void onButtonChecked(cocos2d::Node *pSender) override;
    virtual void onCheckStatuChange(cocos2d::Node *pSender,bool bSelected) override;
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
public:
    void refreshScore();
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void switchSendByID(bool bSelect);
    void switchSendByNick(bool bSelect);
private:
    cocos2d::ui::Text *m_textSave;
    cocos2d::ui::EditBox *m_fieldGetUser;
    cocos2d::ui::EditBox *m_fieldSendNumber;
    cocos2d::ui::EditBox *m_fieldPasswd;
    cocos2d::ui::CheckBox *m_checkByID;
    cocos2d::ui::CheckBox *m_checkByNick;
    ButtonGroup *m_group;
    
    int m_sendMode; //0 by id; 1 by name
};

BJL_NAMESPACE_END

#endif /* BankSendLayer_hpp */
