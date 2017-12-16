//
//  PasswdLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/22/15.
//
//

#ifndef tbnn_header_PasswdLayer_hpp
#define tbnn_header_PasswdLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

TBNN_NAMESPACE_BEGIN

class PasswdLayer:public UILayer,
public cocos2d::ui::EditBoxDelegate
{
public:
    PasswdLayer();
    ~PasswdLayer();
    
    CREATE_FUNC(PasswdLayer);
    virtual bool init() override;
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    
    enum emPasswdType
    {
        kSET_PASSWD,
        kENTER_LIANXIROOM,          //进入练习房间
        kENTER_ANTICHEAT,           //进入加密的防作弊房间
        kINPUT_TABLE_PASS,          //输入加密桌子密码
    };
    
    //显示密码输入
    void showPasswd(bool bShow);
    
    //刷新提示
    void refreshNotice(const std::string &notice,const emPasswdType &passwdType);
    
    //刷新输入框默认文本
    void refreshDefaultInput(const std::string &input);
    
    CC_SYNTHESIZE(UILayer*, m_notify, Notify);
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    cocos2d::ui::Text *m_textNotice;
    cocos2d::ui::EditBox *m_fieldInput;
    emPasswdType m_emPasswdType;
};

TBNN_NAMESPACE_END
#endif /* PasswdLayer_hpp */
