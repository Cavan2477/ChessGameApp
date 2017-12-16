//
//  NiuNiuNode.cpp
//  NiuNiu
//
//  Created by zhong on 12/15/15.
//
//

#include "NiuNiuNode.h"

USING_NS_CC;
USING_NN_NAMESPACE;

static const char * CARD_TYPE_FILE = "game/game_cardtype.png";
static const char * CARD_TYPE_WUNIU = "wuniu";
const int TYPE_WIDTH = 47;
const int TYPE_HEIGHT = 42;
const int DISTANCE = 42;

NiuNiuNode::NiuNiuNode()
{
    
}

NiuNiuNode::~NiuNiuNode()
{
    
}

bool NiuNiuNode::init()
{
    bool bRes = false;
    do {
        CC_BREAK_IF(!Node::init());
        
        for (int i = 0; i < 5; ++i)
        {
            m_spCharset[i] = nullptr;
            m_spCharset[i] = Sprite::createWithSpriteFrameName("blank.png");
            this->addChild(m_spCharset[i]);
        }
        bRes = true;
    } while (false);
    return bRes;
}

void NiuNiuNode::reSet()
{
    //空
    for (int i = 0; i < 5; ++i)
    {
        m_spCharset[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("blank.png"));
    }
}

void NiuNiuNode::refreshNiuNiu(const emNiuNiuType &cType)
{
    int count = 0;
    Texture2D *typeTex = Director::getInstance()->getTextureCache()->getTextureForKey(CARD_TYPE_FILE);
    if (nullptr == typeTex)
    {
        return;
    }
    
    //空
    for (int i = 0; i < 5; ++i)
    {
        m_spCharset[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("blank.png"));
    }
    
    switch (cType)
    {
        case OX_VALUE0:  //无牛
        {
            count = 1;
            m_spCharset[0]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CARD_TYPE_WUNIU));
        }
            break;
        case OX_NIU1: //牛1
        case OX_NIU2: //牛2
        case OX_NIU3: //牛3
        case OX_NIU4: //牛4
        case OX_NIU5: //牛5
        case OX_NIU6: //牛6
        case OX_NIU7: //牛7
        case OX_NIU8: //牛8
        case OX_NIU9: //牛9
        case OX_NIUNIU: //牛牛
        {
            count = 2;
            auto tmp = Sprite::createWithTexture(typeTex, Rect(10 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            //牛
            m_spCharset[0]->setSpriteFrame(tmp->getSpriteFrame());
            //几
            tmp = Sprite::createWithTexture(typeTex,Rect(cType * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            m_spCharset[1]->setSpriteFrame(tmp->getSpriteFrame());
        }
            break;
        case OX_FOURKING: //四金花
        {
            count = 3;
            auto tmp = Sprite::createWithTexture(typeTex, Rect(4 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            //四
            m_spCharset[0]->setSpriteFrame(tmp->getSpriteFrame());
            //金
            tmp = Sprite::createWithTexture(typeTex,Rect(11 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            m_spCharset[1]->setSpriteFrame(tmp->getSpriteFrame());
            //花
            tmp = Sprite::createWithTexture(typeTex,Rect(12 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            m_spCharset[2]->setSpriteFrame(tmp->getSpriteFrame());
        }
            break;
        case OX_FIVEKING: //五金花
        {
            count = 3;
            auto tmp = Sprite::createWithTexture(typeTex, Rect(5 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            //五
            m_spCharset[0]->setSpriteFrame(tmp->getSpriteFrame());
            //金
            tmp = Sprite::createWithTexture(typeTex,Rect(11 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            m_spCharset[1]->setSpriteFrame(tmp->getSpriteFrame());
            //花
            tmp = Sprite::createWithTexture(typeTex,Rect(12 * TYPE_WIDTH, 0, TYPE_WIDTH, TYPE_HEIGHT));
            m_spCharset[2]->setSpriteFrame(tmp->getSpriteFrame());
        }
            break;
        default:
            break;
    }
    arrange(count);
}

void NiuNiuNode::arrange(const int &count)
{
    if (count > 5)
    {
        return;
    }
    
    float center = ((count & 1) == 1) ? count * 0.5 : (count  - 1) * 0.5;
    for (int i = 0; i < count; ++i)
    {
        if (1 == count)
        {
            m_spCharset[i]->setPosition(Vec2(0,0));
        }
        else
        {
            m_spCharset[i]->setPosition(Vec2((i - center) * DISTANCE,0));
        }
    }
}