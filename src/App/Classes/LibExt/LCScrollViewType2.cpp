//
//  LCScrollViewType2.cpp
//  CocosDemo
//
//  Created by zhongtao on 14/12/16.
//
//
#include "LCScrollViewType2.h"
LCScrollViewType2::LCScrollViewType2():
	_moveDuration(0.2f),
	_autoRemove(0),
	_updateOpacity(true),
	_createSize(Size::ZERO),
	_autoHide(false)
{
    _accumulateSize = Size::ZERO;
	_vecViewCells.clear();
	std::vector<float>().swap(_vecOffSet);
}

LCScrollViewType2::~LCScrollViewType2()
{
	_vecViewCells.clear();
	std::vector<float>().swap(_vecOffSet);
	this->removeAllChildrenWithCleanup(true);
}

LCScrollViewType2* LCScrollViewType2::create()
{
	LCScrollViewType2* pRet = new LCScrollViewType2();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

LCScrollViewType2* LCScrollViewType2::create(Size size,int autoRemove /*= 0*/,Node* container /*= nullptr*/)
{
	LCScrollViewType2* p = new LCScrollViewType2();
	if (nullptr != p && p->initWithViewSize(size,container))
	{
		p->autorelease();
		p->_createSize = size;
		p->setContentSize(size);
		p->_autoRemove = autoRemove;
		//p->test();
		return p;
	}
	else
	{
		CC_SAFE_DELETE(p);
		return nullptr;
	}
}

void LCScrollViewType2::test()
{
	auto sp = Sprite::create("Res/menu2.png"); 
	sp->setPosition(Vec2(this->getViewSize().width * 0.5,0));
	this->getContainer()->addChild(sp);
}

void LCScrollViewType2::addScrollViewCell(Node* cell,float offset /*= 5.0f*/,Action* action /* = nullptr */)
{
	auto dir = this->getDirection();
	//自清除
	if (_autoRemove > 0 && _vecViewCells.size() > _autoRemove)
	{
		//删除资源		
		Node* frontNode = _vecViewCells.front();
		std::string useResPath = frontNode->getName();
		if ("" != useResPath)
		{
			FileUtils::getInstance()->removeFile(useResPath);
		}
		this->getContainer()->removeChild(frontNode);	

		_vecViewCells.erase(_vecViewCells.begin());
		_vecOffSet.erase(_vecOffSet.begin());
	}
	_vecViewCells.pushBack(cell);
	_vecOffSet.push_back(offset);
	this->getContainer()->addChild(cell);

	auto viewSize = this->getViewSize();
	auto curContentSize = this->getContentSize();

	auto cContentSize = cell->getContentSize();
	auto cellSize = Size(cContentSize.width * cell->getScaleX(),cContentSize.height * cell->getScaleY());

	float nWidth = curContentSize.width;
	float nHeight = curContentSize.height;
	float nViewWidth = nWidth;
	float nViewHeight = nHeight;

	_accumulateSize.width += (cellSize.width + offset);
	_accumulateSize.height += (cellSize.height + offset);

	float tempWidth = nWidth + curContentSize.width;
	float tempHeight = nHeight + curContentSize.height;

	if (ScrollView::Direction::HORIZONTAL == dir)
	{
		if (_accumulateSize.width > viewSize.width)
		{
			nWidth += cellSize.width + offset;
			nViewWidth = nWidth > _createSize.width ? _createSize.width : nWidth;
		}
	}
	else if (ScrollView::Direction::VERTICAL == dir)
	{
		if (_accumulateSize.height > viewSize.height)
		{
			nHeight = _accumulateSize.height + offset;
			nViewHeight = nHeight > _createSize.height ? _createSize.height : nHeight;
		}		
	}
	/*
	if (_accumulateSize.width > viewSize.width || _accumulateSize.height > viewSize.height)
	{
		if (ScrollView::Direction::HORIZONTAL == dir)
		{
			nWidth += cellSize.width;
			nViewWidth = nWidth > _createSize.width ? _createSize.width : nWidth;
		}
		else if (ScrollView::Direction::VERTICAL == dir)
		{
			nHeight = _accumulateSize.height;
			nViewHeight = nHeight > _createSize.height ? _createSize.height : nHeight;
		}
	}
	*/
	this->setContentSize(Size(nWidth,nHeight));
	this->setViewSize(Size(nViewWidth,nViewHeight));

	this->recursionMove(_vecViewCells.size() - 1,Vec2(cellSize.width,cellSize.height));
}

void LCScrollViewType2::reSet()
{
	_vecViewCells.clear();
	std::vector<float>().swap(_vecOffSet);

    _accumulateSize = Size::ZERO;
	this->setContentSize(_createSize);
	this->setViewSize(_createSize);
	this->getContainer()->removeAllChildrenWithCleanup(true);
}

#pragma mark private
void LCScrollViewType2::recursionMove(size_t idx, Vec2 moveDis)
{
	if (0 == idx)
	{		
		this->updateCellPos(idx,moveDis);
		//G_LOG("move end");
	}
	else
	{
		this->updateCellPos(idx,moveDis);
		this->recursionMove(idx - 1,moveDis);
	}
}

void LCScrollViewType2::updateCellPos(size_t idx, Vec2 moveDis)
{
	int opacity = (-25 * (_vecViewCells.size() - idx)) + 255;
	opacity = opacity < 0 ? 0 : opacity;
	auto dir = this->getDirection();
	if (idx < _vecViewCells.size())
	{
		auto cell = _vecViewCells.at(idx);
		if (nullptr != cell)
		{
			float xMove = moveDis.x + _vecOffSet[idx];
			float yMove = moveDis.y + _vecOffSet[idx];
			if (idx == _vecViewCells.size()-1)
			{
				xMove = cell->getContentSize().width * cell->getScaleX() * cell->getAnchorPoint().x + _vecOffSet[idx];
				yMove = cell->getContentSize().height * cell->getScaleY() * cell->getAnchorPoint().y + _vecOffSet[idx];
			}

			bool bHide = false;
			if (ScrollView::Direction::HORIZONTAL == dir)
			{
				yMove = cell->getPosition().y;
				xMove = xMove + cell->getPosition().x;

				auto hideDis = xMove + cell->getContentSize().width * cell->getScaleX();
				auto viewSize = this->getViewSize().width;
				if (hideDis > viewSize)
				{
					bHide = true;
				}
			}
			else if (ScrollView::Direction::VERTICAL == dir)
			{				
				xMove = cell->getPosition().x;
				yMove = yMove + cell->getPosition().y;

				if (yMove > this->getViewSize().height)
				{
					bHide = true;
				}
			}
			//G_LOG("xmove = %f,ymove = %f",xMove,yMove);
			auto act = MoveTo::create(_moveDuration,Vec2(xMove,yMove));
			
			cell->runAction(act);			
			if (_updateOpacity)
			{
				cell->setCascadeOpacityEnabled(true);
				cell->setOpacity(opacity);
			}
			if (_autoHide && bHide)
			{
				cell->setVisible(false);
			}
		}
	}
}