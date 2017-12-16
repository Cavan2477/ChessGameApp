//
//  GameJettonNode.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/2/16.
//
//

#ifndef bjl_header_GameJettonNode_hpp
#define bjl_header_GameJettonNode_hpp

#include <stdio.h>
#include "CocosHeader.h"

BJL_NAMESPACE_BEGIN

struct tagJettonInfo
{
    BYTE m_cbIdx;           //资源索引(对应图片资源的数字索引)
    BYTE m_cbCount;         //下注次数
    LONGLONG m_llScore;     //下注分数
};
class GameJettonNode:public UINode
{
public:
    GameJettonNode();
    ~GameJettonNode();
    
    CREATE_FUNC(GameJettonNode);
    virtual bool init() override;
    
    /*
     * @brief 刷新下注数额
     * @param[my] 自己下注数额
     * @param[total] 总下注数额
     * @param[bMyJetton] 是否自己下注
     */
    void refreshJetton( LONGLONG my,  LONGLONG total, bool bMyJetton);
    
    void reSet();
private:
    cocos2d::ui::Text *m_textMyJetton, *m_textTotalJetton;
    cocos2d::ui::ImageView *m_imageBg;
    
    LONGLONG m_llMyTotal;
    LONGLONG m_llAreaTotal;
};

BJL_NAMESPACE_END
#endif /* GameJettonNode_hpp */
