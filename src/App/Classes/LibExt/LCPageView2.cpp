//
//  LCPageView22.cpp
//  NiuNiu
//
//  Created by zhong on 12/21/15.
//
//

#include "LCPageView2.h"
USING_NS_CC;

LCPageView2::LCPageView2():
_touchArea(Rect::ZERO),
_beginTouch(Point::ZERO),
_endTouch(Point::ZERO),
_moveDis(Point::ZERO),
_moveDir(eTouchMoveDir::kMoveDefault),
_curPage(0),
_isAutoScrolling(false),
_autoScrollDir(-1),
_autoScrollSpeed(0.0),
_autoScrollDistance(0.0),
_leftPage(nullptr),
_rightPage(nullptr),
_container(nullptr),
_outRange(false)
{
    m_mapPages.clear();
}

LCPageView2::~LCPageView2()
{
    _pageDelegate = nullptr;
    m_mapPages.clear();
    this->removeAllChildrenWithCleanup(true);
}

LCPageView2* LCPageView2::createPageView(LCPageView2DataSource *data,
                                         const cocos2d::Rect &viewSize,
                                         const cocos2d::Rect &touchArea,
                                         float pageWidht,
                                         Node* container /*= nullptr*/)
{
    LCPageView2* _new = new LCPageView2();
    if (_new)
    {
        _new->autorelease();
        _new->m_pDataSource = data;
        _new->m_rectViewSize = viewSize;
        _new->_touchArea = touchArea;
        _new->_pageWidth = pageWidht;
        _new->setContentSize(viewSize.size);
        _new->setClippingEnabled(true);
        
        _new->init(container);
        return _new;
    }
    CC_SAFE_DELETE(_new);
    return nullptr;
}

bool LCPageView2::init(Node* container /*= nullptr*/)
{
    _container = container;
    if (nullptr == _container)
    {
        _container = Layer::create();
        _container->ignoreAnchorPointForPosition(false);
        _container->setAnchorPoint(Vec2(0.0f, 0.0f));
    }
    _container->setPosition(Vec2(0,0));
    this->addChild(_container);
    
    size_t count = m_pDataSource->numberOfPageInPageView(this);
    if (count > 0)
    {
        _leftPage = pageAtIndex(0);
        _rightPage = pageAtIndex(count - 1);
        
        this->scheduleUpdate();
    }
    
    return true;
}

void LCPageView2::onEnter()
{
    Node::onEnter();
    _listener = _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(false);
    //触摸事件
    _listener->onTouchBegan = CC_CALLBACK_2(LCPageView2::onTouchBegan, this);
    
    _listener->onTouchMoved = CC_CALLBACK_2(LCPageView2::onTouchMoved, this);
    
    _listener->onTouchEnded = CC_CALLBACK_2(LCPageView2::onTouchEnded, this);
    
    _listener->onTouchCancelled = CC_CALLBACK_2(LCPageView2::onTouchCancelled, this);
    //_eventDispatcher->addEventListenerWithFixedPriority(_listener, -1);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void LCPageView2::onExit()
{
    _eventDispatcher->removeEventListener(_listener);
    Node::onExit();
}

bool LCPageView2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (!isVisible() || !m_bInvalid)
    {
        return false;
    }
    _beginTouch = touch->getLocation();
    if (_touchArea.containsPoint(_beginTouch))
    {
        return true;
    }
    return false;
}

void LCPageView2::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    _moveDis = touch->getDelta();
    if (_moveDis.x >0)
    {
        _moveDir = eTouchMoveDir::kMoveToRight;
    }
    else
        _moveDir = eTouchMoveDir::kMoveToLeft;
    
    _outRange = false;
    
    auto winSize = Director::getInstance()->getWinSize();
    if (_leftPage->getPositionX() > winSize.width * 0.8 || _rightPage->getPositionX() < -winSize.width * 0.8)
    {
        _moveDis.x = 0;
        _outRange = true;
    }
    
    scrollPages(_moveDis.x);
}

void LCPageView2::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    _beginTouch = Point::ZERO;
    _endTouch = touch->getLocation();
    //触摸结束后的处理
    handleReleaseLogic(_endTouch);
    
    _moveDis = Point::ZERO;
}

void LCPageView2::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    handleReleaseLogic(touch->getLocation());
}

void LCPageView2::update(float dt)
{
    /**
     在更新的时候屏蔽触摸操作
     **/
    //    m_layColor->setVisible(_isAutoScrolling);
    
    if (_isAutoScrolling)
    {
        switch (_autoScrollDir)
        {
            case 0:
            {
                float step = _autoScrollSpeed * dt * 2;
                if (_autoScrollDistance + step >= 0.0f)
                {
                    step = -_autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance += step;
                }
                scrollPages(-step);
                break;
            }
                break;
            case 1:
            {
                float step = _autoScrollSpeed * dt * 2;
                if (_autoScrollDistance - step <= 0.0f)
                {
                    step = _autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance -= step;
                }
                scrollPages(step);
                break;
            }
            default:
                break;
        }
    }
}

/*
 void LCPageView2::visit(cocos2d::Renderer* render,const cocos2d::Mat4& parentTransform,uint32_t parentFlags)
 {
 _beforeVisitCmdScissor.init(_globalZOrder);
 _beforeVisitCmdScissor.func = CC_CALLBACK_0(LCPageView2::onBeforVisitScissor,this);
 render->addCommand(&_beforeVisitCmdScissor);
 
 Node::visit(render,parentTransform,parentFlags);
 
 if (!_children.empty())
 {
 for (int i = 0; i < _children.size(); ++i)
 {
 Node *child = _children.at(i);
 child->visit(render, parentTransform, parentFlags);
 }
 }
 _afterVisitCmdScissor.init(_globalZOrder);
 _afterVisitCmdScissor.func = CC_CALLBACK_0(LCPageView2::onAfterVisitScissor,this);
 render->addCommand(&_afterVisitCmdScissor);
 }
 
 void LCPageView2::onBeforVisitScissor()
 {
 Rect clippingRect = Rect(m_rectViewSize.origin.x,m_rectViewSize.origin.y,m_rectViewSize.size.width,m_rectViewSize.size.height);
 glEnable(GL_SCISSOR_TEST);
 auto glView = Director::getInstance()->getOpenGLView();
 glView->setScissorInPoints(clippingRect.origin.x,clippingRect.origin.y,clippingRect.size.width,clippingRect.size.height);
 }
 
 void LCPageView2::onAfterVisitScissor()
 {
 glDisable(GL_SCISSOR_TEST);
 }
 */

#pragma mark -

void LCPageView2::updateVecInfo(std::vector<cocos2d::Node *> newVec)
{
    
}

void LCPageView2::insertDateToVec(cocos2d::Node *newInfo)
{
    
}

#pragma mark -
void LCPageView2::handleReleaseLogic(const cocos2d::Point &touchPoint)
{
    auto winSize = Director::getInstance()->getWinSize();
    if (m_pDataSource->numberOfPageInPageView(this) <= 0)
    {
        return;
    }
    
    Node* curPage = pageAtIndex(_curPage);
    if (curPage)
    {
        Point curPagePos = curPage->getPosition();
        ssize_t pageCount = m_pDataSource->numberOfPageInPageView(this)/*pageVec.size()*/;
        float curPageLocation = curPagePos.x;
        float boundary = _pageWidth * 0.5f;
        if ((_outRange && _moveDis.x == 0) || ((!_outRange && _moveDis.x != 0) && fabs(_moveDis.x) < 5))//拖动慢的时候
        {
            if (curPageLocation <= -boundary)
            {
                if (_curPage >= pageCount-1)
                {
                    scrollToPage(pageCount-1);
                }
                else
                {
                    scrollToPage(_curPage+1);
                }
            }
            else if (curPageLocation >= boundary)
            {
                if (_curPage <= 0)
                {
                    scrollToPage(0);
                }
                else
                {
                    scrollToPage(_curPage-1);
                }
            }
            else
            {
                scrollToPage(_curPage);
            }
        }
        else//拖动快的时候
        {
            if (_moveDis.x > 0)
            {
                if (_curPage <= 0)
                {
                    scrollToPage(0);
                }
                else
                {
                    scrollToPage(_curPage-1);
                }
            }
            else if (_moveDis.x < 0)
            {
                if (_curPage >= pageCount-1)
                {
                    scrollToPage(pageCount-1);
                }
                else
                {
                    scrollToPage(_curPage+1);
                }
            }
            else
            {
                scrollToPage(_curPage);
            }
        }
    }
}

cocos2d::Node* LCPageView2::pageAtIndex(const size_t &idx)
{
    Node *page = nullptr;
    std::map<size_t,Node*>::iterator ite = m_mapPages.find(idx);
    if (ite == m_mapPages.end())
    {
        page = m_pDataSource->getPageAt(idx);
        _container->addChild(page);
        
        auto size = m_pDataSource->pageSizeAtIndex(this, idx);
        if (eTouchMoveDir::kMoveToLeft == _moveDir)
        {
            Node *prePage = nullptr;
            if ((idx - 1) == 0)
            {
                prePage = _leftPage;
            }
            else
            {
                prePage = pageAtIndex(idx - 1);
            }
            page->setPositionX(prePage->getPositionX() + size.width);
        }
        else if (eTouchMoveDir::kMoveToRight == _moveDir)
        {
            /*
             Node *prePage = nullptr;
             if ((idx + 1) == (m_pDataSource->numberOfPageInPageView(this) - 1))
             {
             prePage = _rightPage;
             }
             else
             {
             prePage = pageAtIndex(idx + 1);
             }
             */
            page->setPositionX(-size.width);
        }
        else
        {
            page->setPositionX(size.width * idx);
        }
        m_mapPages.insert(std::make_pair(idx, page));
    }
    else
    {
        page = ite->second;
    }
    
    if (nullptr != page)
    {
        m_pDataSource->refreshPageAt(page, idx);
    }
    
    return page;
}

size_t LCPageView2::indexFromOffset(const cocos2d::Vec2 &offset)
{
    size_t idx = 0;
    
    //log("offset.x : %.2f, offset.y : %.2f",offset.x,offset.y);
    if (eTouchMoveDir::kMoveToLeft == _moveDir)
    {
        idx = abs((int)offset.x) / (int)m_rectViewSize.size.width + 1;
        if (idx >= m_pDataSource->numberOfPageInPageView(this))
        {
            idx = m_pDataSource->numberOfPageInPageView(this) - 1;
        }
    }
    else if (eTouchMoveDir::kMoveToRight == _moveDir)
    {
        int tmp = abs((int)offset.x) / (int)m_rectViewSize.size.width - 1;
        if (tmp < 0)
        {
            idx = 0;
        }
        else
        {
            idx = tmp;
        }
    }
    //log("idx %d",(int)idx);
    return idx;
}

void LCPageView2::updatePageAtIndex(const size_t &idx)
{
    pageAtIndex(idx);
}

bool LCPageView2::scrollPages(float touchOffset)
{
    if (m_pDataSource->numberOfPageInPageView(this) <= 0)
    {
        return false;
    }
    if (!_leftPage || !_rightPage)
    {
        return false;
    }
    
    float realOffset = touchOffset;
    switch (_moveDir)
    {
        case eTouchMoveDir::kMoveToLeft:
        {
            
        }
            break;
        case eTouchMoveDir::kMoveToRight:
        {
            
        }
            break;
        default:
            break;
    }
    
    movePage(realOffset);
    return true;
}

void LCPageView2::scrollToPage(const size_t &page)
{
    if (page > m_pDataSource->numberOfPageInPageView(this))
    {
        return;
    }
    _curPage = page;
    auto curPage = pageAtIndex(_curPage)/*pageVec.at(_curPage)*/;
    
    _autoScrollDistance = -(curPage->getPosition().x);
    _autoScrollSpeed = fabs(_autoScrollDistance)/0.5f;
    _autoScrollDir = _autoScrollDistance > 0 ? 1 : 0;
    _isAutoScrolling = true;
    
    if (_pageDelegate)
    {
        _pageDelegate->didScrollToPage(curPage,_curPage);
    }
}

void LCPageView2::movePage(float distance)
{/*
  for (auto &pageLayer:_container->getChildren())
  {
  auto posx = pageLayer->getPositionX();
  posx += distance;
  pageLayer->setPositionX(posx);
  }*/
    //计算偏移
    /*
     float left = _leftPage->getPositionX();
     if (left < m_rectViewSize.size.width)
     {
     
     }*/
    
    size_t idx = indexFromOffset(_leftPage->getPosition());
    updatePageAtIndex(idx);
    
    std::map<size_t,Node*>::iterator ite = m_mapPages.begin();
    for (; ite != m_mapPages.end(); ++ite)
    {
        auto pageLayer = ite->second;
        auto posx = pageLayer->getPositionX();
        posx += distance;
        pageLayer->setPositionX(posx);
    }
    //log("move:%.2f",distance);
    /*
     size_t count = m_pDataSource->numberOfPageInPageView(this);
     for (size_t i = 0; i < count; ++i)
     {
     auto pageLayer = pageAtIndex(i);
     auto posx = pageLayer->getPositionX();
     posx += distance;
     pageLayer->setPositionX(posx);
     }
     */
}

void LCPageView2::setCurentPagePos(const size_t &page)
{
    if (page > m_pDataSource->numberOfPageInPageView(this))
    {
        return;
    }
    _curPage = page;
    
    auto winSize = Director::getInstance()->getWinSize();
    size_t count = m_pDataSource->numberOfPageInPageView(this);
    for (size_t i = 0; i < count; ++i)
    {
        auto pageLayer = pageAtIndex(i);
        pageLayer->setPositionX(winSize.width * (i - page));
    }
    
    //此时不需要更新
    _isAutoScrolling = false;
}