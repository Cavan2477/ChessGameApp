//
//  DotNode.hpp
//  NiuNiu
//
//  Created by zhong on 12/28/15.
//
//

#ifndef DotNode_hpp
#define DotNode_hpp

#include <stdio.h>
#include "CocosHeader.h"

class DotNode: public cocos2d::Node
{
public:
    DotNode();
    ~DotNode();
    
    static DotNode* createDotNode(const std::string &normalfile, const std::string &selectfile);
    bool initDot(const std::string &normalfile, const std::string &selectfile);
    
    void refreshDot(const int &nCount);
    
    void moveToDot(const int &nCount);
    
private:
    void arrangePos();
private:
    std::vector<cocos2d::Sprite*> m_vecDot;
    std::string m_strNormal;
    std::string m_strSelect;
    int m_nCurrentDot;
};
#endif /* DotNode_hpp */
