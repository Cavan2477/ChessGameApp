//
//  NameLabel.h
//  
//
//  Created by wh on 16/3/21.
//
//

#ifndef ____NameLabel__
#define ____NameLabel__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class NameLabel : public cocos2d::ui::Layout
{
public:
    static NameLabel *create(const std::string& text, const std::string& font, float fontSize);
    
    virtual bool init();
    
    void initwithstr(const std::string& text, const std::string& font, float fontSize);
    
    void setstring(const std::string &str);
    
    void setFontColor(const cocos2d::Color3B &color);
    
    void setFont(const std::string& font, float fontSize);
    
    void setSize(const cocos2d::Size &contentSize);
private:
    cocos2d::Label *m_label;
    
    cocos2d::Label *m_dotlabel;
};

#endif /* defined(____NameLabel__) */
