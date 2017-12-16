//
//  BankerNode.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/1/16.
//
//

#ifndef bjl_header_BankerNode_hpp
#define bjl_header_BankerNode_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "ClipText.h"
#include "HeaderRequest.h"

BJL_NAMESPACE_BEGIN

class BankerNode:public UINode
{
public:
    BankerNode();
    ~BankerNode();
    
    CREATE_FUNC(BankerNode);
    virtual bool init() override;
    
    /*
     * @brief 刷新庄家信息
     * @param[wChair] 庄家位置
     * @param[lScore] 庄家分数
     * @param[bSystemBanker] 系统庄
     */
    void refreshBanker(const WORD &wChair,  LONGLONG lScore, const bool &bSystemBanker);
    
    void refreshBankerScore( LONGLONG lScore);
    
    CC_SYNTHESIZE(WORD, m_wChair, BankerChair);
private:
    cocos2d::Sprite *m_spBg;
    cocos2d::ui::Text *m_textScore;
    HeaderRequest *m_pUserHead;
    ClipText *m_clipBankerName;
    cocos2d::Sprite *m_spHeadBg;
    cocos2d::Action *m_actTransition;
};

BJL_NAMESPACE_END

#endif /* BankerNode_hpp */
