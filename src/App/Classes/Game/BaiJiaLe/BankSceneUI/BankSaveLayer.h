//
//  BankSaveLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#ifndef bjl_header_BankSaveLayer_hpp
#define bjl_header_BankSaveLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

BJL_NAMESPACE_BEGIN

struct tagSaveParam
{
    SCORE score;
    std::string passwd;
};

class BankSaveLayer:public UILayer,
public cocos2d::ui::EditBoxDelegate
{
    UILAYER_WITHNOTIFY(BankSaveLayer);
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
public:
    void refreshScore();
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    //当前金币
    cocos2d::ui::Text *m_textGold;
    //存款
    cocos2d::ui::Text *m_textSave;
    //金额
    cocos2d::ui::EditBox *m_fieldNumber;
    //银行密码
    cocos2d::ui::EditBox *m_fieldPasswd;
};

BJL_NAMESPACE_END
#endif /* BankSaveLayer_hpp */
