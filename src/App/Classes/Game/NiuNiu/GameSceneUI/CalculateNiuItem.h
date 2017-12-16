//
//  CalculateNiuItem.hpp
//  NiuNiu
//
//  Created by zhong on 12/16/15.
//
//

#ifndef nn_CalculateNiuItem_hpp
#define nn_CalculateNiuItem_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "NiuNiuNode.h"

NN_NAMESPACE_BEGIN

class CalculateNiuItem: public UINode
{
public:
    CalculateNiuItem();
    ~CalculateNiuItem();
    
    CREATE_FUNC(CalculateNiuItem);
    virtual bool init();
    
    void reSet();
    
    void refreshNumber(const BYTE &num1, const BYTE &num2, const BYTE &num3);
    void addNumber(const BYTE &num);
    void cutNumber(const BYTE &num);
    
    void showResult(const emNiuNiuType &cardType);
    void hideResult();
private:
    //计算
    cocos2d::ui::Text *m_text[3];
    //牛牛
    NiuNiuNode *m_niuniu;
};

NN_NAMESPACE_END

#endif /* CalculateNiuItem_hpp */
