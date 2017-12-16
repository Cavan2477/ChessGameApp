//
//  UserDataRef.hpp
//  HLDouDiZhu
//
//  Created by zhong on 3/10/16.
//
//

#ifndef UserDataRef_hpp
#define UserDataRef_hpp

#include <stdio.h>
#include "CocosHeader.h"

class UserDataRef : public cocos2d::Ref
{
public:
    enum enPointerType
    {
        kIntPoint,
        kStringPoint,
        kRefPoint,
    };
    UserDataRef();
    
    ~UserDataRef();
    
    CREATE_FUNC(UserDataRef);
    virtual bool init() { return true; }
    
    CC_SYNTHESIZE(bool, m_bBooleanValue, BooleanValue);
    
    CC_SYNTHESIZE(int, m_nIntValue, IntValue);
    
    CC_SYNTHESIZE(std::string, m_strStringValue, StringValue);
    
    void setPointerValue(const UserDataRef::enPointerType &enType, void *p);
    
    void *getPointerValue() { return m_pPointer; };
private:
    enPointerType m_enPointerType;
    void *m_pPointer;
};
#endif /* UserDataRef_hpp */
