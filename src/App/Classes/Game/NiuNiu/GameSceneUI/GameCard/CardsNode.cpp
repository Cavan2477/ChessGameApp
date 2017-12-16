#include "CardsNode.h"
USING_NS_CC;
using namespace std;
USING_NN_NAMESPACE;

//卡牌弹出动画时间
const float CARD_SHOOT_TIME = 0.2f;
//卡牌回位动画时间
const float CARD_BACK_TIME	= 0.2f;
//卡牌弹出距离
const float CARD_SHOOT_DIS	= 10.0f;
//卡牌Y轴坐标
const float CARD_Y_POS		= 0.0f;

//最低叠放层级
const int MIN_DRAW_ORDER = 0;
//最高叠放层级
const int MAX_DRAW_ORDER = 5;

//卡牌横向间隔
const int CARD_X_DIS = 40.0f;
//卡牌纵向间隔
const int CARD_Y_DIS = 25.0f;

//排序函数：根据卡牌显示层级
bool sortByDrawOrder(const CardSprite *a,const CardSprite *b)
{
	return a->getLocalZOrder() < b->getLocalZOrder();
}

//排序函数：根据卡牌数据
bool sortByCardData(CardSprite *a,CardSprite *b)
{
	return a->getCardData() < b->getCardData();
}

CardsNode::CardsNode():
	m_cardsHolder(nullptr),
	m_actShootCard(nullptr),
	m_actBackCard(nullptr),
	m_bSuggested(false),
	m_leftCardsLabel(nullptr),
	m_bClickable(false),
	m_bAnimation(false),
	m_pSelectedListener(nullptr),
	m_nHandCardCount(5),
    m_beginSelectCard(nullptr),
    m_endSelectCard(nullptr),
    m_bDispating(false)
{
    m_bTouched = false;
	removeAllCards();

	//TODO 卡牌弹出后的回调动作（如确定按钮、逻辑验证）
	auto moveBy = MoveBy::create(CARD_SHOOT_TIME,Vec2(0,CARD_SHOOT_DIS));
	auto call = CallFunc::create([this](){});
	m_actShootCard = Sequence::create(moveBy,call,nullptr);
	CC_SAFE_RETAIN(m_actShootCard);

	moveBy = moveBy->reverse();
	call = CallFunc::create([this](){});
	m_actBackCard = Sequence::create(moveBy,call,nullptr);
	CC_SAFE_RETAIN(m_actBackCard);
}

CardsNode::~CardsNode()
{
	m_pSelectedListener = nullptr;

	removeAllCards();
	this->removeAllChildrenWithCleanup(true);

	//资源释放
	CC_SAFE_RELEASE(m_actShootCard);
	CC_SAFE_RELEASE(m_actBackCard);
}

CardsNode* CardsNode::createCardsNode(const enGameRoleDir &dir, const tagCards &cards, bool isShowCard /*= false*/)
{
	CardsNode* cn = new CardsNode();
	if (nullptr != cn && cn->init())
	{
		cn->autorelease();
		cn->addCardsHolder();
		cn->updateCardsNode(dir,cards,isShowCard);
		cn->m_bClickable = dir == kMY_ROLE;
		return cn;
	}
	CC_SAFE_DELETE(cn);
	return nullptr;
}

CardsNode* CardsNode::createEmptyCardsNode()
{
	CardsNode* cn = new CardsNode();
	if (nullptr != cn && cn->init())
	{
		cn->autorelease();
		cn->addCardsHolder();
		return cn;
	}
	CC_SAFE_DELETE(cn);
	return nullptr;
}

void CardsNode::onEnter()
{
	Node::onEnter();

	m_listener = EventListenerTouchOneByOne::create();
	m_listener->setSwallowTouches(false);
	m_listener->onTouchBegan = CC_CALLBACK_2(CardsNode::onTouchBegan,this);
	m_listener->onTouchMoved = CC_CALLBACK_2(CardsNode::onTouchMoved,this);
	m_listener->onTouchEnded = CC_CALLBACK_2(CardsNode::onTouchEnd,this);
	m_listener->onTouchCancelled = CC_CALLBACK_2(CardsNode::onTouchCancelled,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener,this);
}

void CardsNode::onExit()
{
	_eventDispatcher->removeEventListener(m_listener);
	Node::onExit();
}

bool CardsNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
{
	if (!isVisible())
	{
		return false;
	}
	if (!m_bClickable)
	{
		return false;
	}
    if (m_bDispating)
    {
        return false;
    }
	m_beginSelectCard = nullptr;
	m_endSelectCard = nullptr;
	m_bDragCard = false;
	m_beginTouchPoint = this->convertToNodeSpace(touch->getLocation());
	/*
	Point locationInNode = this->convertToNodeSpace(touch->getLocation());
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	m_bTouched = rect.containsPoint(locationInNode); //判断触摸点是否在目标的范围内
	*/
	m_beginSelectCard = this->filterCard(kHIGHEST,touch->getLocation());
	if (nullptr != m_beginSelectCard)
	{
		m_beginSelectCard->showCardSelectedEffect();
	}
	m_bTouched = m_beginSelectCard != nullptr;

	return m_bTouched;
}

void CardsNode::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event)
{
	if (m_bTouched)
	{
		m_endSelectCard = this->filterCard(kHIGHEST,touch->getLocation());
		m_bDragCard = true;
		Rect touchRect = makeTouchRect(this->convertToNodeSpace(touch->getLocation()));

		//筛选在触摸区域内的卡牌
		vector<CardSprite*> inTouchCards = inTouchAreaCards(touchRect);

		//过滤有效卡牌，选择叠放最高的
		if (inTouchCards.empty())
		{
			return;
		}
		CardSprite* tmp = filterCard(kHIGHEST,touch->getLocation());
		if (nullptr != tmp)
		{	
			if ( !tmp->getCardDragSelect() )
			{
				tmp->showCardSelectedEffect();
				tmp->setCardDragSelect(true);			
				m_mapDragSelectCards.insert(make_pair(tmp->getCardData(),tmp));
			}			
		}

		//剔除不在触摸区域内，但已选择的卡牌
		map<BYTE,CardSprite*> tmpMap;
		for (size_t i = 0; i < inTouchCards.size(); ++i)
		{
			CardSprite* tmp = inTouchCards[i];
			tmpMap.insert(make_pair(tmp->getCardData(),tmp));
		}
		map<BYTE,CardSprite*>::iterator ite = m_mapDragSelectCards.begin();
		for (; ite != m_mapDragSelectCards.end();)
		{
			map<BYTE,CardSprite*>::iterator tmpIte = tmpMap.find(ite->first);
			if (tmpIte != tmpMap.end())
			{
				++ite;
			}
			else
			{
				ite->second->setCardDragSelect(false);
				ite->second->hideCardSelectedEffect();
				ite = m_mapDragSelectCards.erase(ite);
			}
		}
		/*
		Vec2 tmpPoint = (tmp->getPosition());
		Size tmpSize = tmp->getBoundingBox().size;
		Rect tmpRect = Rect(tmpPoint.x - tmpSize.width * 0.5,tmpPoint.y - tmpSize.height * 0.5,tmp->getBoundingBox().size.width,tmp->getBoundingBox().size.height);
		if (touchRect.intersectsRect(tmpRect))
		{
			CCLOG("in");
		}
		else
		{
			CCLOG("out");
		}*/
	}
}

void CardsNode::onTouchEnd(cocos2d::Touch *touch, cocos2d::Event * event)
{	
	m_endSelectCard = this->filterCard(kHIGHEST,touch->getLocation());
	if (!m_bDragCard)
	{
		if (nullptr != m_endSelectCard /*&& !m_endSelectCard->getCardDragSelect()*/)
		{
			m_endSelectCard->setCardDragSelect(true);

			m_mapSelectedCards.insert(make_pair(m_endSelectCard->getCardData(),m_endSelectCard));
		}
        //选牌音效
        INSTANCE(AudioMgr)->playSelectCardAudio();
	}
	if (nullptr != m_beginSelectCard)
	{
		m_beginSelectCard->hideCardSelectedEffect();
	}
	
	m_vecSelectCard = filterDragSelectCards();
	dragCards();

	/*
	if (m_bDragCard)
	{		

	}
	else
	{
		switchCardState(m_endSelectCard);
	}
	*/
	m_beginSelectCard = nullptr;
	m_endSelectCard = nullptr;
	m_bDragCard = false;
    m_bTouched = false;
}

void CardsNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event)
{
	CCLOG("cancel");
}

void CardsNode::updateCardsNode(const enGameRoleDir &dir, const tagCards &cards, bool isShowCard /* = false */)
{
    if (0 == cards.m_cardCount)
    {
        return;
    }
	m_bClickable = dir == kMY_ROLE;
	this->removeAllCards();	
	this->readyForCards(cards.m_cardCount);
    m_bDispating = true;

	m_cardsData = cards;
	m_layerDir = dir;
	m_bShowCard = isShowCard;
	for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
	{
		CardSprite *tmpSp = CardSprite::createCard(m_cardsData.m_cardData[i],"card_back.png");
		tmpSp->setPosition(0,200);
		tmpSp->setCardSpriteDelegate(this);
		tmpSp->showCardBack(true);
		m_cardsHolder->addChild(tmpSp);
		m_mapCard.insert(make_pair(m_cardsData.m_cardData[i],tmpSp));
	}
	
	if (!m_mapCard.empty())
	{
		m_cardSize = m_mapCard.begin()->second->boundingBox().size;
		/*
		//计算尺寸
		int trueCount = m_mapCard.size();
		int cardCount = trueCount == 1 ? 1 : trueCount - 1;
		float tmpFWidth = cardSize.width * cardCount + CARD_X_DIS * (trueCount - 1);
		float tmpFHeight = cardSize.height;

		this->setContentSize(Size(tmpFWidth,500));
		*/
	}	
	
	this->arrangeAllCards(true);
}

tagCards CardsNode::getSelectCards()
{
	tagCards tmp = tagCards();

	tmp.m_cardCount = m_mapSelectedCards.size();
	int i = 0;
	map<BYTE,CardSprite*>::iterator ite = m_mapSelectedCards.begin();
	for (; ite != m_mapSelectedCards.end(); ++ite)
	{
		tmp.m_cardData[i] = ite->second->getCardData();
		++i;
	}

	//清除已选择的卡牌
	m_mapSelectedCards.clear();
	return tmp;
}

void CardsNode::addCards(const tagCards &cards, bool isShowCard /*= false*/)
{
	if(cards.m_cardCount + m_cardsData.m_cardCount > 5) return;

	BYTE total = cards.m_cardCount + m_cardsData.m_cardCount;
	if (total > 5) return;

	BYTE begin = m_cardsData.m_cardCount != 0 ? m_cardsData.m_cardCount : 0;
	BYTE index = 0;
	for (BYTE i = begin; i < total; ++i)
	{
		m_cardsData.m_cardData[i] = cards.m_cardData[index];
		++index;
	}
	m_cardsData.m_cardCount = total;
	m_bShowCard = isShowCard;
	m_nHandCardCount = total;

	//排序
	INSTANCE(GameLogic)->SortCardList(m_cardsData.m_cardData,m_cardsData.m_cardCount);
	this->reSortCards(m_cardsData,isShowCard);
}

void CardsNode::addCardsCount(const BYTE &count)
{
	BYTE total = count + m_cardsData.m_cardCount;
	if (total > 5) return;
	m_cardsData.m_cardCount = total;
}

std::vector<CardSprite*> CardsNode::outCards(const tagCards &cards, bool isShowCard /*= false*/)
{
	m_bShowCard = isShowCard;

	std::vector<CardSprite*> vecOut = std::vector<CardSprite*>();
	if (0 != m_cardsData.m_cardCount)
	{		
		for (BYTE i = 0; i < cards.m_cardCount; ++i)
		{
			BYTE tag = cards.m_cardData[i];
			CardSprite* tmp = static_cast<CardSprite*>(m_cardsHolder->getChildByTag(tag));
			if (nullptr != tmp)
			{
				vecOut.push_back(tmp);
			}

			map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(tag);
			if (ite != m_mapCard.end())
			{
				m_mapCard.erase(ite);
			}
		}

		//清理扑克数据
		tagCards tmpData = m_cardsData;
		BYTE counts = 0;
		for (BYTE i = 0; i < cards.m_cardCount; ++i)
		{
			for (BYTE j = 0; j < m_cardsData.m_cardCount; ++j)
			{
				if (cards.m_cardData[i] == m_cardsData.m_cardData[j])
				{
					tmpData.m_cardData[j] = 0;
				}
			}
		}
		m_cardsData = tagCards();
		for (BYTE i = 0; i < tmpData.m_cardCount; ++i)
		{
			if (tmpData.m_cardData[i] != 0)
			{
				m_cardsData.m_cardData[counts] = tmpData.m_cardData[i];
				++counts;
			}
		}
		m_cardsData.m_cardCount = counts;

		this->reSortCards(m_cardsData,isShowCard);
	}
	else
	{
		for (BYTE i = 0; i < cards.m_cardCount; ++i)
		{
			CardSprite *tmpSp = CardSprite::createCard(cards.m_cardData[i],"card_back.png");
			tmpSp->setPosition(0,0);
			tmpSp->showCardBack(true);
			m_cardsHolder->addChild(tmpSp);
			vecOut.push_back(tmpSp);
		}
	}
	
	//计算手上剩余卡牌
	m_nHandCardCount -= cards.m_cardCount;
	this->refreshLeftCards(m_layerDir,m_nHandCardCount);

	return vecOut;
}

void CardsNode::refreshLeftCards(const enGameRoleDir &dir,const BYTE &left)
{
	//更新剩余扑克数量
	if (nullptr == m_leftCardsLabel)
	{		
		m_leftCardsLabel = Label::createWithSystemFont("","Arial",40);
		m_leftCardsLabel->setVisible(false);
		this->addChild(m_leftCardsLabel);

		if (dir == kLEFT_ROLE)
		{			
			m_leftCardsLabel->setAnchorPoint(Vec2(1,0.5f));
			m_leftCardsLabel->setPosition(-10 - m_cardSize.width * 0.5 , m_cardSize.height * 0.5);
		}
		else if (dir == kRIGHT_ROLE)
		{
			m_leftCardsLabel->setAnchorPoint(Vec2(0,0.5f));
			m_leftCardsLabel->setPosition(10 + m_cardSize.width * 0.5 , m_cardSize.height * 0.5);
		}
	}

	char buf[64];
	sprintf(buf,"%d",left);
	m_leftCardsLabel->setString(buf);
	m_leftCardsLabel->setVisible(true);
}

void CardsNode::reSortCards(const tagCards &cards, bool isShowCard /* = false */)
{
	for (BYTE i = 0; i < cards.m_cardCount; ++i)
	{
		BYTE cardData = cards.m_cardData[i];
		map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
		if (ite == m_mapCard.end())
		{
			CardSprite *tmpSp = CardSprite::createCard(cardData,"card_back.png");
			tmpSp->setCardSpriteDelegate(this);
			tmpSp->showCardBack(true);
			tmpSp->setLocalZOrder(i);
			m_cardsHolder->addChild(tmpSp);
			m_mapCard.insert(make_pair(m_cardsData.m_cardData[i],tmpSp));		
		}
	}	

	//扑克布局
	switch (m_layerDir)
	{
	case kMY_ROLE:      //自己牌
		{
            //niuniu
			float center = 2.0f;
			for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
			{
				BYTE cardData = m_cardsData.m_cardData[i];
				map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
				if (ite == m_mapCard.end())
				{
					continue;
				}
				CardSprite* tmp = ite->second;
				tmp->setLocalZOrder(i);
                tmp->stopAllActions();

				tmp->showCardBack(false);
				Vec2 pos = Vec2(( i - center) * 150, CARD_Y_POS);
				tmp->setPosition(pos);
			}
		}
		break;
	case kLEFT_ROLE:	//左边牌
		{
            //niuniu
            float center = 2.0f;
			//明牌
			if (isShowCard)
			{
				for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
				{
					BYTE cardData = m_cardsData.m_cardData[i];
					map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
					if (ite == m_mapCard.end())
					{
						continue;
					}
					CardSprite* tmp = ite->second;
					tmp->setLocalZOrder(i);
                    tmp->stopAllActions();
					tmp->showCardBack(false);
					Vec2 pos = Vec2(0, -i * CARD_Y_DIS);
					tmp->setPosition(pos);
				}
			}
			else //卡牌叠放，显示剩余卡牌数值
			{
				for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
				{
					BYTE cardData = m_cardsData.m_cardData[i];
					map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
					if (ite == m_mapCard.end())
					{
						continue;
					}
					CardSprite* tmp = ite->second;
					tmp->setLocalZOrder(i);
                    tmp->stopAllActions();
					tmp->showCardBack(true);
					//tmp->setPosition(0,0);
                    Vec2 pos = Vec2(( i - center) * CARD_X_DIS, CARD_Y_POS);
                    tmp->setPosition(pos);
				}
			}			
		}
		break;
	case kRIGHT_ROLE:       //右边牌
		{
            //float center = (m_cardsData.m_cardCount * 0.5);
            //niuniu
            float center = 2.0f;
			if (isShowCard)
			{
				for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
				{
					BYTE cardData = m_cardsData.m_cardData[i];
					map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
					if (ite == m_mapCard.end())
					{
						continue;
					}
					CardSprite* tmp = ite->second;
					tmp->setLocalZOrder(i);
                    tmp->stopAllActions();
					tmp->showCardBack(false);
					Vec2 pos = Vec2(0, -i * CARD_Y_DIS);
					tmp->setPosition(pos);
				}
			}
			else //卡牌叠放，显示剩余卡牌数值
			{
				for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
				{
					BYTE cardData = m_cardsData.m_cardData[i];
					map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
					if (ite == m_mapCard.end())
					{
						continue;
					}
					CardSprite* tmp = ite->second;
					tmp->setLocalZOrder(i);
                    tmp->stopAllActions();
					tmp->showCardBack(true);
					//tmp->setPosition(0,0);
                    Vec2 pos = Vec2(( i - center) * CARD_X_DIS, CARD_Y_POS);
                    tmp->setPosition(pos);
				}
			}			
		}
		break;
        case kTOP_ROLE:     //顶部牌
        {
            //float center = (m_cardsData.m_cardCount * 0.5);
            //niuniu
            float center = 2.0f;
            if (isShowCard)
            {
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE cardData = m_cardsData.m_cardData[i];
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->stopAllActions();
                    tmp->showCardBack(false);
                    Vec2 pos = Vec2(0, -i * CARD_Y_DIS);
                    tmp->setPosition(pos);
                }
            }
            else //卡牌叠放，显示剩余卡牌数值
            {
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE cardData = m_cardsData.m_cardData[i];
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    tmp->stopAllActions();
                    tmp->showCardBack(true);
                    //tmp->setPosition(0,0);
                    Vec2 pos = Vec2(( i - center) * CARD_X_DIS, CARD_Y_POS);
                    tmp->setPosition(pos);
                }
            }
        }
            break;
	default:
		break;
	}
}

void CardsNode::removeAllCards(bool showAnimation /* = true */)
{
    m_cardsData = tagCards();
    m_mapCard.clear();
    vector<CardSprite*>().swap(m_vecSelectCard);
    m_mapSelectedCards.clear();
    m_mapDragSelectCards.clear();
    m_bSuggested = false;
    
    if (nullptr != m_cardsHolder)
    {
        m_cardsHolder->removeAllChildrenWithCleanup(true);
    }
}

void CardsNode::showCards()
{
    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
    {
        BYTE cardData = m_cardsData.m_cardData[i];
        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
        if (ite == m_mapCard.end())
        {
            continue;
        }
        CardSprite* tmp = ite->second;
        tmp->showCardBack(false);
    }
}

void CardsNode::showCardsNiuNiu()
{
    BYTE cards[5] = {0};
    memcpy(cards, m_cardsData.m_cardData, 5);
    tagNiuNiuFilter filter = {0};
    INSTANCE(GameLogic)->FilterNiuNiu(cards, m_cardsData.m_cardCount, filter);
    
    BYTE cardData = 0;
    
    //float center = m_cardsData.m_cardCount * 0.5;
    //niuniu
    float center = 2.0f;
    BYTE idx = 0;
    
    float dis = m_layerDir == kMY_ROLE ? 100 : CARD_X_DIS;
    //牛牛牌
    for (BYTE i = 0; i < filter.cbNiuNiuCardCount; ++i)
    {
        cardData = filter.cbCardData[0][i];
        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
        if (ite == m_mapCard.end())
        {
            continue;
        }
        CardSprite* tmp = ite->second;
        tmp->showCardBack(false);
        tmp->stopAllActions();
        tmp->setPosition(( idx - center) * dis,0);
        tmp->setLocalZOrder(idx);
        ++idx;
    }
    
    //非牛牛牌
    for (BYTE i = 0; i < filter.cbNormalCount; ++i)
    {
        cardData = filter.cbCardData[1][i];
        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
        if (ite == m_mapCard.end())
        {
            continue;
        }
        CardSprite* tmp = ite->second;
        tmp->showCardBack(false);
        tmp->stopAllActions();
        tmp->setPosition(( idx - center) * dis + 20,0);
        tmp->setLocalZOrder(idx);
        ++idx;
    }
}

void CardsNode::suggestShootCards(const tagCards &cards)
{
    if (!m_bTouched)
    {
        m_beginSelectCard = nullptr;
        m_endSelectCard = nullptr;
    }
	//将已经选择的扑克状态更新
	m_vecSelectCard = filterDragSelectCards(false);
	dragCards();
	m_mapSelectedCards.clear();

	if (!m_bSuggested)
	{
		for (BYTE i = 0; i < cards.m_cardCount; ++i)
		{
			CardSprite *tmp = static_cast<CardSprite*>(m_cardsHolder->getChildByTag(cards.m_cardData[i]));
			if (nullptr != tmp)
			{
				tmp->setCardDragSelect(true);
				m_mapSelectedCards.insert(make_pair(tmp->getCardData(),tmp));
			}		
		}
		m_vecSelectCard = filterDragSelectCards(false);
		dragCards();
	}
	m_bSuggested = !m_bSuggested;
}

void CardsNode::addCardsHolder()
{
	if (nullptr == m_cardsHolder)
	{
		m_cardsHolder = Node::create();
		this->addChild(m_cardsHolder);
	}
}

void CardsNode::readyForCards(const BYTE &counts)
{
	m_vecSelectCard.reserve((size_t)counts);
}

void CardsNode::arrangeAllCards(bool showAnimation /* = true */)
{
	//动画效果
	if (showAnimation)
	{
		switch (m_layerDir)
		{
		case enGameRoleDir::kMY_ROLE:
			{
                /*
				Director::getInstance()->setProjection(cocos2d::DisplayLinkDirector::Projection::_2D);

				float center = (m_mapCard.size() * 0.5);
				for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
				{
					BYTE cardData = m_cardsData.m_cardData[i];
					std::map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
					if (ite == m_mapCard.end())
					{
						continue;
					}
					CardSprite *tmpCard = ite->second;
					tmpCard->setLocalZOrder(i);

					auto moveTo = MoveTo::create(0.5f + i / 16.0, Vec2( (i - center) * 150 , CARD_Y_POS));
					auto delay = DelayTime::create(1);
					auto orb = OrbitCamera::create(0.05 + i / 20.0, 1, 0, 90, -90, 0, 0);
					auto hideBack = CallFunc::create([=]{ tmpCard->showCardBack(false);});
					auto orbBack = OrbitCamera::create(0.02f, 1, 0, 0, 90, 0, 0);
					auto seq = Sequence::create(delay,orbBack,hideBack,orb,nullptr);
					auto spawn = Spawn::create(moveTo,seq,nullptr);

					//seq = Sequence::create(moveTo,CallFunc::create([=]{ tmp->showCardBack(false); }),nullptr);
					tmpCard->runAction(spawn);
				}	
				Director::getInstance()->setProjection(cocos2d::DisplayLinkDirector::Projection::DEFAULT);
                 */
                //niuniu
                float center = 2.0f;
                for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                {
                    BYTE cardData = m_cardsData.m_cardData[i];
                    map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                    if (ite == m_mapCard.end())
                    {
                        continue;
                    }
                    CardSprite* tmp = ite->second;
                    tmp->setLocalZOrder(i);
                    
                    Vec2 pos = Vec2(( i - center) * 150, CARD_Y_POS);
                    auto moveTo = MoveTo::create(0.5f + i / 16.0f, pos);
                    auto delay = DelayTime::create(0.5f + i / 16.0f);
                    auto hideBack = CallFunc::create([=]
                                                     {
                                                         INSTANCE(AudioMgr)->playSendCardAudio();
                                                         tmp->showCardBack(false);
                                                         
                                                         if ( i == m_cardsData.m_cardCount - 1)
                                                         {
                                                             m_bDispating = false;
                                                         }
                                                     });
                    auto seq = Sequence::create(delay,hideBack,nullptr);
                    auto spawn = Spawn::create(moveTo,seq,nullptr);
                    
                    tmp->stopAllActions();
                    tmp->runAction(spawn);
                }
			}
			break;
		case enGameRoleDir::kLEFT_ROLE:
			{
                //niuniu
                float center = 2.0f;
				if (m_bShowCard)
				{
				}
				else
				{
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE cardData = m_cardsData.m_cardData[i];
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(( i - center) * CARD_X_DIS, CARD_Y_POS);
                        
                        auto moveTo = MoveTo::create(0.5f + i / 16.0f, pos);
                        auto delay = DelayTime::create(0.5f + i / 16.0f);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             INSTANCE(AudioMgr)->playSendCardAudio();
                                                             tmp->showCardBack(true);
                                                             
                                                             if ( i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispating = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,seq,nullptr);
                        
                        tmp->stopAllActions();
                        tmp->runAction(spawn);
                    }
				}
			}
			break;
		case enGameRoleDir::kRIGHT_ROLE:
			{
                //niuniu
                float center = 2.0f;
				if (m_bShowCard)
				{
				}
				else
				{
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE cardData = m_cardsData.m_cardData[i];
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        Vec2 pos = Vec2(( i - center) * CARD_X_DIS, CARD_Y_POS);
                        
                        auto moveTo = MoveTo::create(0.5f + i / 16.0f, pos);
                        auto delay = DelayTime::create(0.5f + i / 16.0f);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             INSTANCE(AudioMgr)->playSendCardAudio();
                                                             tmp->showCardBack(true);
                                                             
                                                             if ( i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispating = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,seq,nullptr);
                        
                        tmp->stopAllActions();
                        tmp->runAction(spawn);
                    }
				}
			}
			break;
        case enGameRoleDir::kTOP_ROLE:
            {
                //niuniu
                float center = 2.0f;
                if (m_bShowCard)
                {
                }
                else
                {
                    for (BYTE i = 0; i < m_cardsData.m_cardCount; ++i)
                    {
                        BYTE cardData = m_cardsData.m_cardData[i];
                        map<BYTE,CardSprite*>::iterator ite = m_mapCard.find(cardData);
                        if (ite == m_mapCard.end())
                        {
                            continue;
                        }
                        CardSprite* tmp = ite->second;
                        tmp->setLocalZOrder(i);
                        //tmp->setPosition(0,0);
                        Vec2 pos = Vec2(( i - center) * CARD_X_DIS, CARD_Y_POS);
                        
                        auto moveTo = MoveTo::create(0.5f + i / 16.0f, pos);
                        auto delay = DelayTime::create(0.5f + i / 16.0f);
                        auto showBack = CallFunc::create([=]
                                                         {
                                                             INSTANCE(AudioMgr)->playSendCardAudio();
                                                             tmp->showCardBack(true);
                                                             
                                                             if ( i == m_cardsData.m_cardCount - 1)
                                                             {
                                                                 m_bDispating = false;
                                                             }
                                                         });
                        auto seq = Sequence::create(delay,showBack,nullptr);
                        auto spawn = Spawn::create(moveTo,seq,nullptr);
                        
                        tmp->stopAllActions();
                        tmp->runAction(spawn);
                    }
                }
            }
                break;
		default:
			break;
		}
		
	}
	else
	{
		//整理卡牌位置
		this->reSortCards(m_cardsData,m_bShowCard);
	}
}

void CardsNode::switchCardState(CardSprite *card)
{
	if (nullptr == card)
	{
		return;
	}
	card->stopAllActions();
	bool tmpBShoot = card->getCardShoot();
	if (tmpBShoot)
	{		
		card->runAction(m_actBackCard);
		card->setCardShoot(false);
	}
	else
	{
		card->runAction(m_actShootCard);
		card->setCardShoot(true);
	}
	card->hideCardSelectedEffect();
}

void CardsNode::dragCards()
{
	for (size_t i = 0; i < m_vecSelectCard.size(); ++i)
	{
		CardSprite *card = m_vecSelectCard[i];
		Vec2 pos = card->getPosition();
		card->stopAllActions();
		if (!card->getCardShoot())
		{
			auto shoot = MoveTo::create(CARD_SHOOT_TIME,Vec2(pos.x,CARD_Y_POS + CARD_SHOOT_DIS));
			card->runAction(shoot);
			card->setCardShoot(true);
            if (nullptr != m_pSelectedListener)
            {
                m_pSelectedListener->onCardsStateChange(card->getCardData(), true);
            }

			m_mapSelectedCards.insert(make_pair(card->getCardData(),card));
		}
		else
		{
			auto shoot = MoveTo::create(CARD_SHOOT_TIME,Vec2(pos.x,CARD_Y_POS));
			card->runAction(shoot);
			card->setCardShoot(false);
            if (nullptr != m_pSelectedListener)
            {
                m_pSelectedListener->onCardsStateChange(card->getCardData(), false);
            }

			map<BYTE,CardSprite*>::iterator ite = m_mapSelectedCards.find(card->getCardData());
			if (ite != m_mapSelectedCards.end())
			{
				m_mapSelectedCards.erase(ite);
			}
		}
	}
    
    bool tmpShow = m_layerDir == kMY_ROLE ? false : !m_bShowCard;
    auto children = m_cardsHolder->getChildren();
    for (ssize_t i = 0; i < children.size(); ++i)
    {
        CardSprite *sp = (CardSprite*)children.at(i);
        if (nullptr != sp)
        {
            sp->hideCardSelectedEffect();
            sp->setCardDragSelect(false);
            sp->showCardBack(tmpShow);
        }
    }

	m_tSelectedCards = tagCards();
	m_tSelectedCards.m_cardCount = m_mapSelectedCards.size();
	int i = 0;
	map<BYTE,CardSprite*>::iterator ite = m_mapSelectedCards.begin();
	for (; ite != m_mapSelectedCards.end(); ++ite)
	{
		m_tSelectedCards.m_cardData[i] = ite->second->getCardData();
		++i;
	}
	if (nullptr != m_pSelectedListener)
	{
		m_pSelectedListener->onSelectedCards(m_tSelectedCards);
	}
	m_mapDragSelectCards.clear();
	vector<CardSprite*>().swap(m_vecSelectCard);
}

CardSprite* CardsNode::filterCard(const enFilterFlag &flag, const Point& touchPoint)
{
	vector<CardSprite*> tmpSel = vector<CardSprite*>();
	map<BYTE,CardSprite*>::iterator ite = m_mapCard.begin();
	for (; ite != m_mapCard.end(); ++ite)
	{
		CardSprite* tmp = ite->second;
		Point locationInNode = tmp->convertToNodeSpace(touchPoint);;
		Rect rect = Rect(0,0,tmp->boundingBox().size.width,tmp->boundingBox().size.height);
		if (rect.containsPoint(locationInNode))
		{
			tmpSel.push_back(tmp);
		}
	}

	if (tmpSel.empty())
	{
		return nullptr;
	}
	std::sort(tmpSel.begin(),tmpSel.end(),sortByDrawOrder);
	if (CardsNode::enFilterFlag::kHIGHEST == flag)
	{
		return tmpSel[tmpSel.size() - 1];
	}
	else
	{
		return tmpSel[0];
	}
}

std::vector<CardSprite*> CardsNode::inTouchAreaCards(const Rect &touchRect)
{
	map<BYTE,CardSprite*> tmpMap = map<BYTE,CardSprite*>();
	map<BYTE,CardSprite*>::iterator ite = m_mapCard.begin();
	for (; ite != m_mapCard.end(); ++ite)
	{
		CardSprite* tmp = ite->second;
		Point locationInNode = tmp->getPosition();
		Size tmpSize =  tmp->getBoundingBox().size;
		Vec2 anchor = tmp->getAnchorPoint();

		Vec2 ori = Vec2(locationInNode.x - tmpSize.width * anchor.x,locationInNode.y - tmpSize.height * anchor.y);
		Rect rect = Rect(ori.x,ori.y,tmpSize.width,tmpSize.height);

		if (rect.intersectsRect(touchRect))
		{
			tmpMap.insert(make_pair(tmp->getCardData(),tmp));
		}		
	}
	return filterDragSelectCards(true,tmpMap);
}

Rect CardsNode::makeTouchRect(const Vec2 &endTouch)
{
	Vec2 movePoint = endTouch;	
	//判断拖动方向（左右）
	m_dragMoveDir = (m_beginTouchPoint.x) < (movePoint.x) ? enMoveDir::kMoveToRight : enMoveDir::kMoveToLeft;
	bool toRight = (m_beginTouchPoint.x) < (movePoint.x) ? true : false;
	//判断拖动方向（上下）
	bool toTop = (m_beginTouchPoint.y) < (movePoint.y) ? true : false;
	
	Rect touchRect = Rect::ZERO;

	if (toRight && toTop) //右上
	{
		touchRect = Rect(m_beginTouchPoint.x,m_beginTouchPoint.y,movePoint.x - m_beginTouchPoint.x,movePoint.y - m_beginTouchPoint.y);
	}
	else if (toRight && !toTop) //右下
	{
		touchRect = Rect(m_beginTouchPoint.x,movePoint.y,movePoint.x - m_beginTouchPoint.x,m_beginTouchPoint.y - movePoint.y);
	}
	else if (!toRight && toTop) //左上
	{
		touchRect = Rect(movePoint.x,m_beginTouchPoint.y,m_beginTouchPoint.x - movePoint.x, movePoint.y - m_beginTouchPoint.y);
	}
	else if (!toRight && !toTop) //左下
	{
		touchRect = Rect(movePoint.x,movePoint.y,m_beginTouchPoint.x - movePoint.x, m_beginTouchPoint.y - movePoint.y);
	}
	
	return touchRect;
}

vector<CardSprite*> CardsNode::filterDragSelectCards(bool bFilter /*= true*/,const map<BYTE,CardSprite*> &cards /*= map<BYTE,CardSprite*>()*/)
{
	int lowOrder = getLowOrder();
	int hightOrder = getHightOrder();

	//过滤对象
	map<BYTE,CardSprite*>tmpMap = map<BYTE,CardSprite*>();	
	if (cards.empty())
	{
		//合并
		tmpMap.insert(m_mapSelectedCards.begin(),m_mapSelectedCards.end());
		tmpMap.insert(m_mapDragSelectCards.begin(),m_mapDragSelectCards.end());
	}
	else
	{
		tmpMap = cards;
	}

	vector<CardSprite*> tmp = vector<CardSprite*>();
	map<BYTE,CardSprite*>::iterator ite = tmpMap.begin();
	for (; ite != tmpMap.end(); ++ite)
	{		
		CardSprite* tmpCard = ite->second;
		if (bFilter)
		{
			if (tmpCard->getLocalZOrder() >= lowOrder && tmpCard->getLocalZOrder() <= hightOrder)
			{
				tmp.push_back(tmpCard);
			}
		}
		else
		{
			tmp.push_back(tmpCard);
		}		
	}

	return tmp;
}

int CardsNode::getLowOrder()
{
	int beginOrder = m_beginSelectCard != nullptr ? m_beginSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
	int endOrder = MAX_DRAW_ORDER;
	if (kMoveToLeft == m_dragMoveDir)
	{
		endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
	}
	else
	{
		endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MAX_DRAW_ORDER;
	}

	return MIN(beginOrder, endOrder);
}

int CardsNode::getHightOrder()
{
	int beginOrder = m_beginSelectCard != nullptr ? m_beginSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
	int endOrder = MAX_DRAW_ORDER;

	if (kMoveToLeft == m_dragMoveDir)
	{
		endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MIN_DRAW_ORDER;
	}
	else
	{
		endOrder = m_endSelectCard != nullptr ? m_endSelectCard->getLocalZOrder() : MAX_DRAW_ORDER;
	}
	return MAX(beginOrder, endOrder);
}