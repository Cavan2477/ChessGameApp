//
//  GameMessage.h
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#pragma once

#include <stdio.h>
#include "define.h"
#include "cocos2d.h"
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN
class GameLayer;
class GameMessage : public cocos2d::Ref
{
public:
    GameMessage();
    
    ~GameMessage();
    
    void GameMessageResult(WORD  wSubCmdID, void* pData, WORD wSize);
    void GameSceneMessageResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    CC_SYNTHESIZE(GameLayer * , m_layer, Layer)
};
LKPY_NAMESPACE_END

