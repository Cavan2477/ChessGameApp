//
//  ButtonGroup.hpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#ifndef ButtonGroup_hpp
#define ButtonGroup_hpp

#include <stdio.h>
#include "CocosHeader.h"

class ButtonGroupListen
{
public:
    virtual bool preButtonChecked(cocos2d::Node *pSender,bool bSelected) = 0;
    virtual void onButtonChecked(cocos2d::Node *pSender) = 0;
    virtual void onCheckStatuChange(cocos2d::Node *pSender,bool bSelected) = 0;
};

class ButtonGroup:public cocos2d::Node
{
public:
    ButtonGroup();
    ~ButtonGroup();
    
    CC_SYNTHESIZE(std::string, m_strGroup, GroupName);
    
    void addToGroup(cocos2d::ui::CheckBox* btn,const std::string &btnname);
    
    void removeFromGroup(const std::string &btnname);
    
    bool isBtnInGroup(const std::string &btnname);
    
    void setSelecte(const std::string &btnname);
    
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    //check 回调
    void checkEvent(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type);
    
    CC_SYNTHESIZE(ButtonGroupListen*, m_listen, GroupListen);
    
    //
    CC_SYNTHESIZE(bool, m_bEnable, GroubEnable);
private:
    std::map<std::string,cocos2d::ui::CheckBox*> m_mapGroupButton;
};
#endif /* ButtonGroup_hpp */
