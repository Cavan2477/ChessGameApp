//
//  Regist.hpp
//  MyGame
//
//  Created by Tang Miao on 1/12/16.
//
//

#ifndef __RegistScene_h__
#define __RegistScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../Common/MacroDefine.h"

USING_NS_CC;

using namespace std;

enum ENUM_REGIST
{
    EM_LOADING = 1,
};

class RegistScene : public Layer, public cocos2d::ui::EditBoxDelegate
{
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    CREATE_FUNC(RegistScene);
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    void notifyConnectFailure(cocos2d::EventCustom *event);
    
    /////网络消息模块
    void loginResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void roomlistResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
public:
    bool checkSpaceStr(const std::string str) const;
    bool checkLength(int len) const;
    void buttonEventWithBack(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithRegist(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
    
    void loadFinish(Ref *ref);
protected:
    cocos2d::ui::EditBox   *m_pAccountEdit;
    cocos2d::ui::EditBox   *m_pPasswordEdit;
    cocos2d::ui::EditBox   *m_pPWDConfirmEdit;
    
    
};
#endif /* Regist_hpp */
