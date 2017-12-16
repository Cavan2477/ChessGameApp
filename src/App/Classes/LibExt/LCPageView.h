//
//  LCPageView.h
//  CocosDemo
//
//  Created by zhongtao on 14/12/22.
//
//
#ifndef __CocosDemo__LCPageView__
#define __CocosDemo__LCPageView__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LCPageViewDelegate
{
public:
	virtual void pageDidScroll(){};
	virtual void didScrollToPage(cocos2d::Node *sender,const int &page){};
};

class LCPageView:public Node
{
public:
	enum class eTouchMoveDir
	{
		kMoveDefault,
		kMoveToRight,
		kMoveToLeft
	};
	LCPageView();
	~LCPageView();
	static LCPageView* createPageView(std::vector<Node*> vec,Rect touchArea,float pageWidht,Node* container = nullptr);
	bool init(std::vector<Node*> vec,Node* container = nullptr);
public:
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);
	virtual void update(float dt) override;
public:
	void updateVecInfo(std::vector<Node*> newVec);
	void insertDateToVec(Node* newInfo);
	Node* getContainer() { return _container; };

	CC_SYNTHESIZE(std::vector<Node*>, pageVec, PageVec);
	CC_SYNTHESIZE(Rect, _touchArea, TouchArea);
	CC_SYNTHESIZE(eTouchMoveDir, _moveDir, MoveDir);
	CC_SYNTHESIZE(int, _curPage, CurPage);
	CC_SYNTHESIZE(float, _pageWidth, PageWidth);
	CC_SYNTHESIZE(LCPageViewDelegate*, _pageDelegate, PageDelegate);
	CC_SYNTHESIZE(bool, _outRange, OutRange);

	bool scrollPages(float touchOffset);
	void scrollToPage(int page);
	void movePage(float distance);
	void setCurentPagePos(int page);
private:
	void handleReleaseLogic(const Point &touchPoint);	
public:
	EventListenerTouchOneByOne* _listener;
	Point _beginTouch;
	Point _endTouch;
	Point _moveDis;

	CC_SYNTHESIZE(bool, _isAutoScrolling, IsAutoScrolling);
	int _autoScrollDir;
	float _autoScrollSpeed;
	float _autoScrollDistance;

	Node* _leftPage;
	Node* _rightPage;

	Node* _container;
};
#endif