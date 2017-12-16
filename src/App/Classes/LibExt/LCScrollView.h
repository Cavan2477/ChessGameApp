//
//  LCScrollView.h
//  CocosDemo
//
//  Created by zhongtao on 14/11/18.
//
//
#ifndef _CocosDemo_LCScrollView_h
#define _CocosDemo_LCScrollView_h

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LCScrollViewDelegate:public ScrollViewDelegate
{

};

class LCScrollView:public cocos2d::extension::ScrollView,public ScrollViewDelegate
{
public:
	LCScrollView();
	virtual ~LCScrollView();

	static LCScrollView* create();
	static LCScrollView* create(Size size, Node* container = NULL);

public:
	//Overrides
	virtual void scrollViewDidScroll(ScrollView* view) override{}
	virtual void scrollViewDidZoom(ScrollView* view)  override {}
	virtual bool onTouchBegan(Touch *touch, Event *event)  override;
	virtual void onTouchMoved(Touch *touch, Event *event)  override;
	virtual void onTouchEnded(Touch *touch, Event *event)  override;
	virtual void onTouchCancelled(Touch *touch, Event *event)  override;
public:
	CC_SYNTHESIZE(bool,_bCanMove,CanMove);
};
#endif