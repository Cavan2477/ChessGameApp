//
//  NiuNiuNode.hpp
//  NiuNiu
//
//  Created by zhong on 12/15/15.
//
//

#ifndef NiuNiuNode_h
#define NiuNiuNode_h

#include <stdio.h>
// 结算 牛牛类型节点
#include "CocosHeader.h"
#include "NiuNiuLogicDefine.h"

NN_NAMESPACE_BEGIN

class NiuNiuNode:public cocos2d::Node
{
public:
    NiuNiuNode();
    ~NiuNiuNode();
    
    CREATE_FUNC(NiuNiuNode);
    virtual bool init();
    
    /*
     * @brief 重置
     */
    void reSet();
    
    /*
     * @brief 刷新节点
     * @param[niuniuType] 牛牛类型
     */
    void refreshNiuNiu(const emNiuNiuType &cType);
    
private:
    /*
     * @brief 排列字符
     * @param[count] 有效字符数
     */
    void arrange(const int &count);
private:
    //字符
    cocos2d::Sprite *m_spCharset[5];
};

NN_NAMESPACE_END
#endif /* NiuNiuNode_hpp */
