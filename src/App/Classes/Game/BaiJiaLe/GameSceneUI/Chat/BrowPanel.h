//
//  BrowPanel.hpp
//  NiuNiu
//
//  Created by zhong on 12/17/15.
//
//

#ifndef bjl_header_BrowPanel_hpp
#define bjl_header_BrowPanel_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "TouchSprite.h"

BJL_NAMESPACE_BEGIN
class BrowPanel:public cocos2d::Node,
    public TouchSpriteDelegate
{
public:
    BrowPanel();
    ~BrowPanel();
    
    static BrowPanel* createPanel(const std::vector<int> &vecidx);
    bool init(const std::vector<int> &vecidx);
    virtual bool isTouchable(TouchSprite *sp);
    virtual void onTouchSpriteEnd(TouchSprite *sp);
    
    CC_SYNTHESIZE(UILayer*, m_notify, NotifyNode);
    CC_SYNTHESIZE(bool , m_bTouchable, BrowTouchable);
private:
    static const cocos2d::Vec2 m_pos[40];
};
BJL_NAMESPACE_END
#endif /* BrowPanel_hpp */
