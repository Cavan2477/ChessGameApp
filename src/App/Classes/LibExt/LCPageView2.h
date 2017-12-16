//
//  LCPageView2.hpp
//  NiuNiu
//
//  Created by zhong on 12/21/15.
//
//

#ifndef LCPageView2_hpp
#define LCPageView2_hpp

#include <stdio.h>

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LCPageView2;
class LCPageView2Delegate
{
public:
    virtual void didScrollToPage(cocos2d::Node *sender,const size_t &page){};
};

class LCPageView2DataSource
{
public:
    virtual cocos2d::Node* getPageAt(const size_t &page) = 0;
    virtual void refreshPageAt(cocos2d::Node *node,const size_t &page) = 0;
    virtual cocos2d::Size pageSizeAtIndex(LCPageView2 *view, const size_t &idx) = 0;
    virtual size_t numberOfPageInPageView(LCPageView2 *view) = 0;
};

class LCPageView2:public ui::Layout
{
public:
    enum class eTouchMoveDir
    {
        kMoveDefault,
        kMoveToRight,
        kMoveToLeft
    };
    LCPageView2();
    ~LCPageView2();
    static LCPageView2* createPageView(LCPageView2DataSource *data,const Rect &viewSize, const Rect &touchArea,float pageWidht,Node* container = nullptr);
    
    bool init(Node* container = nullptr);
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
    virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
    virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;
    virtual void update(float dt) override;
public: /*裁剪*/
    /*
     virtual void visit(cocos2d::Renderer* render,const cocos2d::Mat4& parentTransform,uint32_t parentFlags) override;
     void onBeforVisitScissor();
     void onAfterVisitScissor();
     */
public:
    void updateVecInfo(std::vector<Node*> newVec);
    void insertDateToVec(Node* newInfo);
    Node* getContainer() { return _container; };
    
    CC_SYNTHESIZE(Rect,m_rectViewSize,PageViewSize);
    CC_SYNTHESIZE(Rect, _touchArea, TouchArea);
    CC_SYNTHESIZE(eTouchMoveDir, _moveDir, MoveDir);
    CC_SYNTHESIZE(size_t, _curPage, CurPage);
    CC_SYNTHESIZE(float, _pageWidth, PageWidth);
    CC_SYNTHESIZE(LCPageView2Delegate*, _pageDelegate, PageDelegate);
    CC_SYNTHESIZE(bool, _outRange, OutRange);
    CC_SYNTHESIZE(bool, _isAutoScrolling, IsAutoScrolling);
    CC_SYNTHESIZE(bool, m_bInvalid, Invalide); //是否有效(可拖动等)
    
    bool scrollPages(float touchOffset);
    void scrollToPage(const size_t &page);
    void movePage(float distance);
    void setCurentPagePos(const size_t &page);
private:
    void handleReleaseLogic(const Point &touchPoint);
    cocos2d::Node* pageAtIndex(const size_t &idx);
    size_t indexFromOffset(const Vec2 &offset);
    void updatePageAtIndex(const size_t &idx);
private:
    EventListenerTouchOneByOne* _listener;
    Point _beginTouch;
    Point _endTouch;
    Point _moveDis;
    
    int _autoScrollDir;
    float _autoScrollSpeed;
    float _autoScrollDistance;
    
    Node* _leftPage;
    Node* _rightPage;
    Node* _container;
    std::map<size_t,Node*> m_mapPages;
private:
    cocos2d::CustomCommand _beforeVisitCmdScissor;
    cocos2d::CustomCommand _afterVisitCmdScissor;
    LCPageView2DataSource *m_pDataSource;
};

#endif /* LCPageView2_hpp */
