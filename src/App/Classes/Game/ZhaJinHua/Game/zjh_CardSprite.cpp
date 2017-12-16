#include "zjh_CardSprite.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
USING_ZJH_NAMESPACE;

CardSprite::CardSprite() :m_bIsOutState(false)
, m_bCanMove(true)
, m_pCardValue(NULL)
, m_pCardColorRB(NULL)
, m_pCardColorLT(NULL)
, m_bIsFirstInit(false)
{

}

CardSprite* CardSprite::createWithCardSize(int _value,CardSize _size)
{
	CardSprite* ret = new CardSprite();
	if (ret)
	{
		ret->m_cardSize = _size;
		ret->m_nValue = _value;
		ret->init();
	}
	

	return ret;
}

void CardSprite::selectSelf()
{
	if (!m_bCanMove)
		return;
	m_bCanMove = false;
	if (m_bIsOutState)//如果已经出牌点击则退回
	{
//		this->runAction(Sequence::create(MoveBy::create(0.08f, Point(0, -37.0f))
//			, __CCCallFuncO::create(this, callfuncO_selector(CardSprite::moveCallBack), NULL)
//			, NULL));
		m_bIsOutState = false;
	}
	else
	{
//		runAction(Sequence::create(MoveBy::create(0.08f, Point(0, 37.0f))
//			, __CCCallFuncO::create(this, callfuncO_selector(CardSprite::moveCallBack), NULL)
//			, NULL));
		m_bIsOutState = true;
	}
}

void CardSprite::moveCallBack(Ref* obj)
{
	m_bCanMove = true;
}

bool CardSprite::init()
{
	bool ret = false;
	if (!Sprite::init())
		return ret;
	changeDisplay(m_nValue, m_cardSize);
	m_bIsFirstInit = true;
	return ret;

	ret = true;
	return ret;
}

void CardSprite::changeDisplay(int _value, CardSize _size)
{
	TextureCache* tCache = TextureCache::getInstance();
    Texture2D* pCardTex = tCache->getTextureForKey("card2.png");//大小纹理
    float TexWidth = pCardTex->getContentSize().width;
    float TexHeight = pCardTex->getContentSize().height;
	//纹理key名
	if (_value < 0x4E)
	{
		//花色 大小 位置位移
        
//		if (!m_bIsFirstInit)
//		{
//			this->initWithTexture(tCache->getTextureForKey(cardBoardStr));//牌白色面板
//		}
//		else
//		{
//			this->setDisplayFrame(getSpriteFrameByTexture(tCache->getTextureForKey(cardBoardStr)));
//		}
		Size boardSize = this->getContentSize();//牌面板大小
		Rect rc = this->getTextureRect();

		
        float originX = (_value % 16 - 1) * (TexWidth / 13);
        float originY = (/*4 -*/ _value / 16)*(TexHeight / 5);
        Rect cardRc(originX,originY,TexWidth / 13, TexHeight / 5);//取得一块花色纹理
		initWithTexture(pCardTex, cardRc);
		
	}
	else if(_value <= 0x50)
	{
        Rect cardRc((_value - 0x4E) * (TexWidth / 13),TexHeight / 5 *4, TexWidth / 13, TexHeight / 5);//取得一块花色纹理
        initWithTexture(pCardTex, cardRc);
		
	}
    else
    {
        if(_value ==  Value_Back)
            setDisplayFrame(SpriteFrameCache::getInstance()->spriteFrameByName("card_back.png"));
        else if(_value ==  Value_Break)
            setDisplayFrame(SpriteFrameCache::getInstance()->spriteFrameByName("card_break.png"));
    }
	
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile()
}

SpriteFrame* CardSprite::getSpriteFrameByTexture(Texture2D* pT2d,Rect _rc)
{
	if (!pT2d)
		return NULL;
	SpriteFrame* sf = NULL;
	sf = SpriteFrame::createWithTexture(pT2d, _rc);
	return sf;
}

SpriteFrame* CardSprite::getSpriteFrameByTexture(Texture2D* pT2d)
{
	if (!pT2d)
		return NULL;
	Rect _rc(0, 0, pT2d->getPixelsWide(), pT2d->getPixelsHigh());
	SpriteFrame* sf = NULL;
	sf = SpriteFrame::createWithTexture(pT2d, _rc);
	return sf;
}