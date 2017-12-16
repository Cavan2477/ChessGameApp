//
//  UserDataRef.cpp
//  HLDouDiZhu
//
//  Created by zhong on 3/10/16.
//
//

#include "UserDataRef.h"
UserDataRef::UserDataRef():
m_bBooleanValue(false),
m_nIntValue(0),
m_strStringValue(""),
m_pPointer(nullptr)
{
    
}

UserDataRef::~UserDataRef()
{
    if (nullptr != m_pPointer)
    {
        switch (m_enPointerType)
        {
            case UserDataRef::kIntPoint:
            {
                int *tmp = (int*)(m_pPointer);
                delete tmp;
                tmp = nullptr;
            }
                break;
            case UserDataRef::kStringPoint:
            {
                std::string *tmp = (std::string*)m_pPointer;
                delete tmp;
                tmp = nullptr;
            }
                break;
            case UserDataRef::kRefPoint:
            {
                cocos2d::Ref *tmp = (cocos2d::Ref*)m_pPointer;
                delete tmp;
                tmp = nullptr;
            }
                break;
            default:
                CC_ASSERT(false);
                break;
        }
    }
}

void UserDataRef::setPointerValue(const UserDataRef::enPointerType &enType, void *p)
{
    m_enPointerType = enType;
    m_pPointer = p;
}