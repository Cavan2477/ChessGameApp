#pragma once
#include "cocos2d.h"
#include "define.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;

ZJH_NAMESPACE_BEGIN
class ChatLayer:public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    virtual bool init();
    CREATE_FUNC(ChatLayer);
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    void initRecordUi();
private:
    
    Layout *m_layout;
    EditBox* m_inputEditBox;
    ScrollView* m_textScrollView;
    ScrollView* m_browScrollView;
    ScrollView* m_recordScrollView;
    Button* m_textsBtn;
    Button* m_browBtn;
    Button* m_recordBtn;
};
ZJH_NAMESPACE_END