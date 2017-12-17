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

#include "LoadingScene.h"
#include "ui/CocosGUI.h"
#include "../../Common/PublicDefine.h"

using namespace cocos2d::ui;

bool LoadingScene::init()
{
    if(!Layer::init())
    {
        
        return false;
    }
    
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->removeChildByTag(EM_BOTTOM_LAYOUT);
    auto playout = Layout::create();
    playout->setTag(EM_BOTTOM_LAYOUT);
    playout->setContentSize(cocos2d::Size(1136,640));
    playout->setBackGroundImage("common_res/common_backGround.png");
    playout->setScaleX(JUDGE_SCALE);
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
    progress->setTag(EM_LOADING_PRO);
    playout->addChild(progress);
    
    return true;
}

LoadingScene::LoadingScene():finishCallFunc(nullptr)
{
    
    
}
LoadingScene::~LoadingScene()
{
   
    finishCallFunc = nullptr;
    DebugLog("Loading Release");
    
}

void LoadingScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    auto playout = this->getChildByTag(EM_BOTTOM_LAYOUT);
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

bool LoadingScene::onTouchBegan(Touch *touch, Event *event)
{
    
    return true;
}

void LoadingScene::runFinish()
{
    
    auto action = CallFunc::create(CC_CALLBACK_0(LoadingScene::loadingFinish, this));
    auto playout = static_cast<Layout *>(this->getChildByTag(EM_BOTTOM_LAYOUT));
    auto progress = static_cast<ProgressTimer *>(playout->getChildByTag(EM_LOADING_PRO));
    auto paction = Sequence::createWithTwoActions(ProgressTo::create(0.7f, 100), action);
    progress->runAction(paction);
    
}

void  LoadingScene::loadingFinish()
{

    if(nullptr != finishCallFunc)
    {
        
        finishCallFunc(this);
        
    }
}
void  LoadingScene::setFinishCallFunc(const FinishCallFunc &func)
{
    
    finishCallFunc = func;
    
}