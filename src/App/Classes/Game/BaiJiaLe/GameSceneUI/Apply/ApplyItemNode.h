//
//  ApplyItemNode.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/2/16.
//
//

#ifndef bjl_header_ApplyItemNode_hpp
#define bjl_header_ApplyItemNode_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "ClipText.h"

BJL_NAMESPACE_BEGIN

struct tagApplyInfo
{
    //用户座椅id
    WORD m_wChairId;
    //用户id
    WORD m_wUserId;
    //用户昵称
    std::string m_strUserName;
    //排号
    BYTE m_cbApplyNum;
    //是否当前庄家
    bool m_bCurrent;
    //编号
    LONGLONG m_llIdx;
};

class ApplyItemNode:public UINode
{
public:
    ApplyItemNode();
    ~ApplyItemNode();
    
    CREATE_FUNC(ApplyItemNode);
    virtual bool init() override;
    
    void refresh(const tagApplyInfo &tagInfo);
private:
    cocos2d::Sprite *m_spCurrentBg, *m_spApplyNumBg;
    cocos2d::ui::Text *m_textApplyNum;
    ClipText *m_clipUserName;
};

BJL_NAMESPACE_END

#endif /* ApplyItemNode_hpp */
