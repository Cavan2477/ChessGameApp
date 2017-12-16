//
//  GameChatLayer.hpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#ifndef bjl_header_GameChatLayer_hpp
#define bjl_header_GameChatLayer_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "ChatTableView.h"
#include "ButtonGroup.h"
#include "LCPageView.h"
#include "BrowPanel.h"
#include "ChatItem.h"

BJL_NAMESPACE_BEGIN

class GameChatLayer:public UILayer,
    public ButtonGroupListen,
    public ChatTableViewDataSource,
    public cocos2d::ui::EditBoxDelegate
{
public:
    GameChatLayer();
    ~GameChatLayer();
    
    CREATE_FUNC(GameChatLayer);
    virtual bool init() override;
    virtual bool preButtonChecked(cocos2d::Node *pSender,bool bSelected) override;
    virtual void onButtonChecked(cocos2d::Node *pSender) override;
    virtual void onCheckStatuChange(cocos2d::Node *pSender,bool bSelected) override;
    
    virtual cocos2d::Node *getDataItemAt(ChatTableView *chatTable, const size_t &idx) override;
    virtual void refreshDataItemAt(ChatTableView *chatTable, cocos2d::Node *item,const size_t &idx) override;
    virtual size_t getDataItemSize(ChatTableView *chatTable) override;
    virtual cocos2d::Size getItemSize(ChatTableView *chatTable, const size_t &idx) override;
    virtual void onTouchItem(ChatTableView *chatTable, cocos2d::extension::TableViewCell* cell) override;
    
    //ui 线程消息(不能在非ui线程内调用)
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override;
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    
    CC_SYNTHESIZE(UILayer*, m_notify, Notify);
    void showGameChat(bool bShow);
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void showTextChat(bool bShow);
    void showBrowChat(bool bShow);
    void showRecord(bool bShow);
    
    void uiGameMsg(const int &sub,void *pData, const int &nLen);
    
    void enableBrowItem(bool bEnable);
private:
    cocos2d::Sprite *m_spChatArea;
    cocos2d::ui::CheckBox *m_checkText; //表情
    cocos2d::ui::CheckBox *m_checkBrow; //短语
    cocos2d::ui::CheckBox *m_checkRecord; //聊天记录
    ButtonGroup *m_group;
    
    cocos2d::ui::EditBox *m_fieldChat; //聊天输入框
    cocos2d::ui::Button *m_btnSend; //发送
    cocos2d::ui::Button *m_btnClose; //关闭
    
    ChatTableView *m_textChat;
    //LCPageView *m_browChat;
    ChatTableView *m_browChat;
    ChatTableView *m_recordChat;
    
    BrowPanel *m_browPanels[2];
    
    std::map<size_t,cocos2d::Node*> m_mapTextItem;
    std::map<size_t,cocos2d::Node*> m_mapRecordItem;
    std::map<size_t,BrowPanel*> m_mapBrowItem;
    
    bool m_bEditChat;
};

BJL_NAMESPACE_END

#endif /* GameChatLayer_hpp */
