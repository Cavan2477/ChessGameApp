//
//  LCScrollView.cpp
//  CocosDemo
//
//  Created by zhongtao on 14/11/18.
//
//
#include "LCScrollView.h"

LCScrollView::LCScrollView():
_bCanMove(true)
{

}

LCScrollView::~LCScrollView()
{

}

LCScrollView* LCScrollView::create()
{
	LCScrollView* _lcS = new LCScrollView();
	if (nullptr != _lcS && _lcS->init())
	{
		_lcS->autorelease();
		return _lcS;
	}
	else
	{
		CC_SAFE_DELETE(_lcS);
		return nullptr;
	}
}

LCScrollView* LCScrollView::create(Size size, Node* container /* = NULL */)
{
	LCScrollView* _lcS = new LCScrollView();
	if (nullptr != _lcS && _lcS->initWithViewSize(size,container))
	{
		_lcS->autorelease();
		return _lcS;
	}
	else
	{
		CC_SAFE_DELETE(_lcS);
		return nullptr;
	}
}

bool LCScrollView::onTouchBegan(Touch *touch, Event *event)
{
	auto bRes = ScrollView::onTouchBegan(touch,event);

	return bRes;
}

void LCScrollView::onTouchMoved(Touch *touch, Event *event)
{
	if (_bCanMove)
	{
		ScrollView::onTouchMoved(touch,event);
	}
}

void LCScrollView::onTouchEnded(Touch *touch, Event *event)
{
	ScrollView::onTouchEnded(touch,event);
}

void LCScrollView::onTouchCancelled(Touch *touch, Event *event)
{
	ScrollView::onTouchCancelled(touch,event);
}