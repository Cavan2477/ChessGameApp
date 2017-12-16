//
//  ChatItem.hpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#ifndef bjl_header_ChatItem_hpp
#define bjl_header_ChatItem_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "define.h"
#include "ClipText.h"

class ChatItem:public cocos2d::Node
{
public:
    ChatItem();
    ~ChatItem();
    
    static ChatItem* createTextItem(const std::string &str, const DWORD &senduser = INVALID_USERID);
    
    static ChatItem* createBrowItem(const int &nIdx, const DWORD &senduser = INVALID_USERID);
    
    //bool init(const DWORD &senduser = INVALID_USERID);
    
    void initTextItem(const std::string &str);
    
    void initBrowItem(const int &nIdx);
    
    CREATE_FUNC(ChatItem);
    virtual bool init();
    
    void refreshTextItem(const std::string &str, const DWORD &senduser = INVALID_USERID);
    
    void refreshBrowItem(const int &nIdx, const DWORD &senduser = INVALID_USERID);
private:
    //cocos2d::Label *m_labelUserName;
    ClipText *m_clipUserName;
    cocos2d::Label *m_labelChat;
    cocos2d::Sprite *m_spBrow;
};
#endif /* ChatItem_hpp */
