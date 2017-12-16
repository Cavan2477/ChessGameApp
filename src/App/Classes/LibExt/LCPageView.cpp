//
//  LCPageView.cpp
//  CocosDemo
//
//  Created by zhongtao on 14/12/22.
//
//
#include "LCPageView.h"
LCPageView::LCPageView()
{
    pageVec.clear();
    _touchArea = Rect::ZERO;
    _beginTouch = Point::ZERO;
    _endTouch = Point::ZERO;
    _moveDis = Point::ZERO;
    _moveDir = eTouchMoveDir::kMoveDefault;
    
    _curPage = 0;
    
    _pageWidth = 0.0;
    
    _isAutoScrolling = false;
    _autoScrollDir = -1;
    _autoScrollSpeed = 0.0;
    _autoScrollDistance = 0.0;
    
    _leftPage = nullptr;
    _rightPage = nullptr;
    
    _pageDelegate = nullptr;
    
    _outRange = false;

	_container = nullptr;
}

LCPageView::~LCPageView()
{
    _pageDelegate = nullptr;
    std::vector<Node*>().swap(pageVec);
    this->removeAllChildrenWithCleanup(true);
}

LCPageView* LCPageView::createPageView(std::vector<cocos2d::Node *> vec,Rect touchArea,float pageWidth,Node* container /*= nullptr*/)
{
    LCPageView* _new = new LCPageView();
    if (_new)
    {
        _new->autorelease();
        _new->_touchArea = touchArea;
        _new->_pageWidth = pageWidth;

		_new->init(vec,container);
    }
    else
    {
        CC_SAFE_DELETE(_new);
    }
    return _new;
}

bool LCPageView::init(std::vector<cocos2d::Node *> vec,Node* container /*= nullptr*/)
{
    pageVec = vec;

	_container = container;
	if (nullptr == _container)
	{
		_container = Layer::create();
		_container->ignoreAnchorPointForPosition(false);
		_container->setAnchorPoint(Vec2(0.0f, 0.0f));
	}
	_container->setPosition(Vec2(0,0));
	this->addChild(_container);
    
    if (!pageVec.empty())
    {		
        _leftPage = pageVec.front();
        _rightPage = pageVec.back();
        
		for (int i = 0; i < pageVec.size(); i++)
		{
			auto page = pageVec.at(i);
			page->setPositionX(_pageWidth * i);
			_container->addChild(page);
		}

        this->scheduleUpdate();
    }
    
    return true;
}

void LCPageView::onEnter()
{
    Node::onEnter();
    _listener = _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(false);
    //触摸事件
    _listener->onTouchBegan = CC_CALLBACK_2(LCPageView::onTouchBegan, this);
    
    _listener->onTouchMoved = CC_CALLBACK_2(LCPageView::onTouchMoved, this);
    
    _listener->onTouchEnded = CC_CALLBACK_2(LCPageView::onTouchEnded, this);
    
    _listener->onTouchCancelled = CC_CALLBACK_2(LCPageView::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, -1);
}

void LCPageView::onExit()
{
    _eventDispatcher->removeEventListener(_listener);
    Node::onExit();
}

bool LCPageView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    _beginTouch = touch->getLocation();
    if (_touchArea.containsPoint(_beginTouch))
    {
        return true;
    }
    return false;
}

void LCPageView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
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
    if (_leftPage->getPositionX() > winSize.width*0.8 || _rightPage->getPositionX() < -winSize.width*0.8)
    {
        _moveDis.x = 0;
        _outRange = true;
    }
    
    scrollPages(_moveDis.x);
}

void LCPageView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    _beginTouch = Point::ZERO;
    _endTouch = touch->getLocation();
    //触摸结束后的处理
    handleReleaseLogic(_endTouch);
    
    _moveDis = Point::ZERO;
}

void LCPageView::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    handleReleaseLogic(touch->getLocation());
}

void LCPageView::update(float dt)
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
                float step = _autoScrollSpeed*dt*2;
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
                float step = _autoScrollSpeed*dt*2;
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

#pragma mark -

void LCPageView::updateVecInfo(std::vector<cocos2d::Node *> newVec)
{
    
}

void LCPageView::insertDateToVec(cocos2d::Node *newInfo)
{
    
}

#pragma mark -
void LCPageView::handleReleaseLogic(const cocos2d::Point &touchPoint)
{
    auto winSize = Director::getInstance()->getWinSize();
    if (pageVec.size()<=0 || _curPage < 0)
    {
        return;
    }
    auto curPage = pageVec.at(_curPage);
    if (curPage)
    {
        Point curPagePos = curPage->getPosition();
        ssize_t pageCount = pageVec.size();
        float curPageLocation = curPagePos.x;
        float boundary = _pageWidth/2.0f;
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

bool LCPageView::scrollPages(float touchOffset)
{
    if (pageVec.size()<=0)
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

void LCPageView::scrollToPage(int page)
{
    if (page<0 || page>pageVec.size())
    {
        return;
    }
    _curPage = page;
    auto curPage = pageVec.at(_curPage);
    
    _autoScrollDistance = -(curPage->getPosition().x);
    _autoScrollSpeed = fabs(_autoScrollDistance)/0.5f;
    _autoScrollDir = _autoScrollDistance > 0 ? 1 : 0;
    _isAutoScrolling = true;

	if (_pageDelegate)
	{
		_pageDelegate->didScrollToPage(curPage,_curPage);
	}
}

void LCPageView::movePage(float distance)
{
    for (auto &pageLayer:pageVec)
    {
        auto posx = pageLayer->getPositionX();
        posx += distance;
        pageLayer->setPositionX(posx);
    }
}

void LCPageView::setCurentPagePos(int page)
{
    if (page < 0 || page>pageVec.size())
    {
        return;
    }
    _curPage = page;
    
    auto* curPage = pageVec.at(page);
    curPage->setPositionX(0);
    auto winSize = Director::getInstance()->getWinSize();
    for (int i = 0; i<pageVec.size(); ++i)
    {
        pageVec.at(i)->setPositionX(winSize.width * (i - page));
    }    
    
    //此时不需要更新
    _isAutoScrolling = false;
}