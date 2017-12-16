//
//  OpenBankLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#ifndef bjl_header_OpenBankLayer_hpp
#define bjl_header_OpenBankLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"

BJL_NAMESPACE_BEGIN

class OpenBankLayer: public UILayer,
public cocos2d::ui::EditBoxDelegate
{
public:
    OpenBankLayer();
    ~OpenBankLayer();
    
    static OpenBankLayer* createOpenBank(UILayer *notify);
    virtual bool init() override;
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
private:
    UILayer *m_notify;
    cocos2d::ui::EditBox *m_fieldPasswd;
    cocos2d::ui::EditBox *m_fieldSure;
};

BJL_NAMESPACE_END

#endif /* OpenBankLayer_hpp */
