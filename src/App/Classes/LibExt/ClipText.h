/************************************************************************************
 * file: 		ClipText.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:10:43
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __ClipText_h__
#define __ClipText_h__

#include <stdio.h>
#include "CocosHeader.h"

class ClipText:public cocos2d::ui::Layout
{
public:
    ClipText(const cocos2d::Size &visibleSize);
    ~ClipText();
    
    enum emTextType
    {
        kCLIP_TEXT,
        kCLIP_TEXTATLAS,
    };
    
    enum emClipType
    {
        //以文本移动的方式进行裁剪
        kClipRunAct,
        kClipReplace,
    };
    
    static ClipText* createClipText(const cocos2d::Size &visibleSize,
                                    const std::string &str,
                                    const std::string &fontName = "Arial",
                                    const int &fontSize = 20);
    
    virtual void onEnterTransitionDidFinish() override;
    
    virtual void onExit() override;
    
    virtual void setAnchorPoint(const cocos2d::Vec2& anchorPoint) override;
    
    void setClipMode(const emClipType &mode, const std::string &replacestr = ".");
    
    cocos2d::Node* getClipText();
    
    void setTextAnchorPoint(const cocos2d::Vec2 &anchor);
    
    void setTextPosition(const cocos2d::Vec2 &pos);
    
    void setString(const std::string &str);
    
    std::string getString() { return m_strText; }
    
    void setTextColor(const cocos2d::Color4B &color);
    
    void setTextFontSize(const float &fSize);
private:
    bool initText(const std::string &str,
                  const std::string &fontName = "Arial",
                  const int &fontSize = 20);
    
    void updateMoveAction(cocos2d::Node *text);
    
    //
    void threadParseStr(const std::string &str);
private:
    std::string m_strText;
    int m_nFontSize;
    
    emTextType m_emTextType;
    cocos2d::Size m_szVisibleSize;
    cocos2d::EventListenerTouchOneByOne *m_listener;
    cocos2d::Action *m_actMoveAct;
    
    emClipType m_emClipType;
    std::string m_strReplaceStr;
};
#endif /* ClipText_hpp */
