/************************************************************************************
 * file: 		Loading.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:22:05
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "Loading.h"
#include "../../Common/PublicDefine.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

bool Loading::init()
{
    if(!Layer::init())
    {
        
        return false;
    }
    
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Loading::onTouchBegan, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->removeChildByTag(Tag_Bottom_Layout);
    auto playout = Layout::create();
    playout->setTag(Tag_Bottom_Layout);
    playout->setContentSize(cocos2d::Size(1136,640));
    playout->setBackGroundImage("common_res/common_backGround.png");
    playout->setScaleX(JudgeScale);
    playout->setPosition(Vec2(0,0));
    this->addChild(playout);
    
    auto progressbg = ImageView::create("load_res/im_loading_bg.png");
    progressbg->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height/2));
    playout->addChild(progressbg);
    
    auto progress = ProgressTimer::create(Sprite::create("load_res/im_loading_pro.png"));
    progress->setType(ProgressTimer::Type::BAR);
    progress->setMidpoint(Vec2(0.f, 0.5f));
    progress->setBarChangeRate(Vec2(1, 0));
    progress->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height/2));
    progress->setPercentage(0.f);
    progress->runAction(ProgressTo::create(0.7f, 70));
    progress->setTag(Tag_Loading_Pro);
    playout->addChild(progress);
    
    return true;
}

Loading::Loading():finishCallFunc(nullptr)
{
    
    
}
Loading::~Loading()
{
   
    finishCallFunc = nullptr;
    DebugLog("Loading Release");
    
}

void Loading::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    auto playout = this->getChildByTag(Tag_Bottom_Layout);
    Vector<SpriteFrame *> animarray;
    for (int index=0; index<5; ++index)
    {
        SpriteFrame *frame = SpriteFrame::create("load_res/anim_loading.png", cocos2d::Rect(67.4*index, 0, 67.4, 105));
        animarray.pushBack(frame);
    }
    auto loadinganim = Animation::createWithSpriteFrames(animarray, 0.07f);
    auto loading = Sprite::create("load_res/anim_loading.png", cocos2d::Rect(0, 0, 67.4, 105));
    loading->setAnchorPoint(Vec2(0.5f, 0.f));
    loading->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height/2+25));
    playout->addChild(loading);
    
    auto paction = RepeatForever::create(Animate::create(loadinganim));
    loading->runAction(paction);

}

bool Loading::onTouchBegan(Touch *touch, Event *event)
{
    
    return true;
}

void Loading::runFinish()
{
    
    auto action = CallFunc::create(CC_CALLBACK_0(Loading::loadingFinish, this));
    auto playout = static_cast<Layout *>(this->getChildByTag(Tag_Bottom_Layout));
    auto progress = static_cast<ProgressTimer *>(playout->getChildByTag(Tag_Loading_Pro));
    auto paction = Sequence::createWithTwoActions(ProgressTo::create(0.7f, 100), action);
    progress->runAction(paction);
    
}

void  Loading::loadingFinish()
{

    if(nullptr != finishCallFunc)
    {
        
        finishCallFunc(this);
        
    }
}
void  Loading::setFinishCallFunc(const FinishCallFunc &func)
{
    
    finishCallFunc = func;
    
}