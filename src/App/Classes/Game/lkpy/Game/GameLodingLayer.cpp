//
//  GameLodingLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/22.
//
//

#include "GameLodingLayer.h"
#include "Project.h"
#include "lkpy_GameDataMgr.h"
#include "GameLayer.h"
#include "HallDataMgr.h"
#include "lkpy_CMD_NotifyDefine.h"
using namespace std;
USING_NS_CC;
using namespace ui;
USING_LKPY_NAMESPACE;
//鱼游动动画的帧数
#define  FishAnimNum    12
//鱼的种类数
#define FishTypeNum     20
#define FISH_TYPE_NUM     23
//导入图片数量
#define  LoadingImageNum    7

#define Tag_Light   10

#define FISH_MOVE_TYPE_NUM 26

#define FISH_DEAD_TYPE_NUM 22


//每一种鱼游动的图片个数
static int fishFrameMoveNum[FISH_MOVE_TYPE_NUM] = {
    6,8,12,
    12,12,13,
    12,10,12,
    8,12,6,
    12,10,12,
    12,12,20,
    9,16,15,
    8,12,1,
    12,7
};

static int fishFrameDeadNum[FISH_DEAD_TYPE_NUM] = {
    2,2,2,
    3,3,3,
    6,3,2,
    6,4,3,
    3,3,3,
    3,3,20,
    3,8,9,
    12
    
};

const char *LoadingPngName[LoadingImageNum] = {
    "whater.png","bullet.png"
    ,"fish_dead.png","watch.png","images.png"
    //,"bullets.png","fish_dead.png","fish_move.png"
    ,"lock_fish.png","boom_darts.png"
};

const char *LoadintPlistName[LoadingImageNum] = {
    "whater.plist","bullet.plist"
    ,"fish_dead.plist","watch.plist","images.plist"
    //,"bullets.plist","fish_dead.plist","fish_move.plist"
    ,"lock_fish.plist","boom_darts.plist"
};


Scene* GameLodingLayer::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameLodingLayer::create();

    scene->addChild(layer);
    return scene;
}

bool GameLodingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    //GameDataMgr::getInstance();
    NetworkMgr::getInstance()->m_GameScenecallback = CC_CALLBACK_2(GameLodingLayer::onGameScene, this);
    m_index = 0;
    cocos2d::Size viewsize = WinSize;
    auto pbg = ImageView::create("login_res/login_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(pbg);
    
    
    auto progressbg = ImageView::create("login_res/load_bg.png");
    progressbg->setPosition(Vec2(viewsize.width/2, 200));
    this->addChild(progressbg);
    
    m_progress = ProgressTimer::create(Sprite::create("login_res/load_bar.png"));
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setMidpoint(Vec2(0.f, 0.5f));
    m_progress->setBarChangeRate(Vec2(1, 0));
    m_progress->setPosition(Vec2(viewsize.width/2, 200));
    this->addChild(m_progress);
    
    auto plight = ImageView::create("login_res/load_light.png");
    plight->setTag(Tag_Light);
    this->addChild(plight);
    plight->setPosition(Vec2(m_progress->getPositionX() - m_progress->getContentSize().width/2, m_progress->getPositionY()));
    
    for (int index=0; index<LoadingImageNum; ++index) {
        auto str = __String::createWithFormat("game_res/%s", LoadingPngName[index])->getCString();
        Director::getInstance()->getTextureCache()->addImageAsync(str,CC_CALLBACK_1(GameLodingLayer::loadingCallback, this));
    }
    
    this->scheduleUpdate();
    return true;
}

void GameLodingLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
}

void GameLodingLayer::onGameScene(void *pData, WORD wSize)
{
    NetworkMgr::getInstance()->m_GameScenecallback = nullptr;
    if(lkpy_game::GameDataMgr::getInstance()->m_gameMessage)
        return;
    HallDataMgr::getInstance()->m_bStartGame = true;
    int nsize = sizeof(GameScene);
    //if(wSize!=sizeof(GameScene)) return;
    auto presult = (GameScene *)pData;
    lkpy_game::GameDataMgr::getInstance()->m_sceneData = *presult;
    //int multipeArray[6] = {1,10,100,500,1000,5000};
//    for (int i = 0; i < 6; i++) {
//        lkpy_game::GameDataMgr::getInstance()->m_sceneData.nMultipleValue[i] = multipeArray[i];
//    }
    GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
    GameDataMgr::getInstance()->initgameMessage();
}

void GameLodingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_wChairID < 3) {
        GameDataMgr::getInstance()->m_reversal = true;
    }
}

void GameLodingLayer::onExit()
{
    Layer::onExit();
}

void GameLodingLayer::update(float delta)
{
    auto plight = this->getChildByTag(Tag_Light);
    plight->setPositionX(m_progress->getPositionX() - m_progress->getContentSize().width/2 + m_progress->getContentSize().width*(m_progress->getPercentage()/100.f));
}

void GameLodingLayer::loadingCallback(cocos2d::Texture2D *ptexture)
{
    LONGLONG ltime = getsystemtomillisecond();
    log("%d __%lld",ptexture->getName() ,ltime);
    m_index++;
    ProgressTo *action = CCProgressTo::create(0.5f, m_index*(90.f/LoadingImageNum));
    m_progress->runAction(action);
    
    if (m_index == (LoadingImageNum)) {
        for (int index=0; index<LoadingImageNum; ++index) {
            log("game_res/%s",LoadintPlistName[index]);
            auto str = __String::createWithFormat("game_res/%s", LoadintPlistName[index])->getCString();
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(str);
        }
        LONGLONG ltime = getsystemtomillisecond();
        this->readFishAniam();
        log("%d __%lld",ptexture->getName() ,ltime);
        //NetworkMgr::getInstance()->sendGameOption();
        auto action1 = ProgressTo::create(0.5f, 100.f);
        auto paction = Sequence::createWithTwoActions(action1, CallFunc::create([=]{
            
            Director::getInstance()->replaceScene(CCTransitionFade::create(0.3f, GameLayer::createScene()));
        }));
        m_progress->runAction(paction);
    }
    
}

void GameLodingLayer::readFishAniam()
{
    
    //鱼游动动画
    {
        
        char str[255];
        for (size_t i = 1; i <= FISH_MOVE_TYPE_NUM; i++)
        {
            float actionTime = 0.09f;
            if(i==21)
                actionTime = 0.15f;
            Animation* pAnimation = Animation::create();
            Vector<SpriteFrame*> pAnimationArray;
            int nCount = fishFrameMoveNum[i-1];
            for (size_t j = 1; j <= nCount; j++)
            {
                SpriteFrame* sf = NULL;
                sprintf(str, "fishMove_%03d_%d.png",i,j);
                sf = SpriteFrameCache::sharedSpriteFrameCache()->getInstance()->spriteFrameByName(str);
                if (sf)
                    pAnimationArray.pushBack(sf);
                
            }
            pAnimation = Animation::createWithSpriteFrames(pAnimationArray, actionTime);
            sprintf(str,"animation_fish_move%d",i);
            AnimationCache::getInstance()->addAnimation(pAnimation, string(str));
        }
    }
    
    
    //鱼死亡动画
    {
        {
            
            char str[255];
            for (size_t i = 1; i <= FISH_DEAD_TYPE_NUM; i++)
            {
                Animation* pAnimation = Animation::create();
                Vector<SpriteFrame*> pAnimationArray;
                int nCount = fishFrameDeadNum[i-1];
                if(i == FISH_DEAD_TYPE_NUM)
                    i = 25;
                for (size_t j = 1; j <= nCount; j++)
                {
                    SpriteFrame* sf = NULL;
                    sprintf(str, "fishDead_%03d_%d.png", i, j);
                    sf = SpriteFrameCache::sharedSpriteFrameCache()->getInstance()->spriteFrameByName(str);
                    if (sf)
                        pAnimationArray.pushBack(sf);
                }
                pAnimation = Animation::createWithSpriteFrames(pAnimationArray, 0.05f);
                sprintf(str, "animation_fish_dead%d", i);
                AnimationCache::getInstance()->addAnimation(pAnimation, string(str));
            }
        }
    }
    
    
    this->readAnimation("water_%d.png", WaterAnim, 12, 0.12f);
    this->readAnimation("fort_%d.png", FortAnim, 6, 0.02f);
    this->readAnimation("fort_light_%d.png", FortLightAnim, 6, 0.02f);
    this->readAnimation("silver_coin_%d.png", SilverAnim, 12, 0.05f);
    this->readAnimation("gold_coin_%d.png", GoldAnim, 12, 0.08f);
    this->readAnimation("copper_coin_%d.png", CopperAnim, 10, 0.05f);
    this->readAnimation("boom%02d.png", BombAnim, 32,0.03f);
    this->readAnimation("boom_darts%d.png", BombDartsAnim, 33,0.03f);
    this->readAnimation("blue%02d.png", BlueIceAnim, 22,0.03f);
    this->readAnimation("bullet_%d.png", BulletAnim, 10);
    this->readAnimation("light_%d.png", LightAnim, 16, 0.05f);
    this->readAnimation("watch_%d.png", watchAnim, 24, 0.08f);

    this->readAnimationbyfilename("game_res/fish_bomb_ball.png", 70, 70, 2, 5, FishBall);
    this->readAnimationbyfilename("game_res/fish_bomb_light.png", 40, 256, 1, 6, FishLight);
}

void GameLodingLayer::readFishAnimation(const char *FramForm, const char *SaveName ,int FishType, int animnum, float time/* = 0.15f*/)
{
    cocos2d::Vector<SpriteFrame *> fisharray;
    for (int index = 0 ; index < animnum ; ++index)
    {
        auto str = __String::createWithFormat(FramForm,FishType,index)->getCString();
        auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        fisharray.pushBack(pframe);
    }
    auto savename = __String::createWithFormat(SaveName,FishType)->getCString();
    auto fishanim = Animation::createWithSpriteFrames(fisharray, time);
    AnimationCache::getInstance()->addAnimation(fishanim, savename);
}

void GameLodingLayer::readAnimation(const char *FramForm, const char *SaveName, int framnum ,float time)
{
    cocos2d::Vector<SpriteFrame *> animarray;
    for (int index = 0 ; index < framnum ; ++index)    {
        auto str = __String::createWithFormat(FramForm,index)->getCString();
        auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animarray.pushBack(pframe);
    }
    auto fishanim = Animation::createWithSpriteFrames(animarray, time);
    AnimationCache::getInstance()->addAnimation(fishanim, SaveName);
}

void GameLodingLayer::readAnimationbyfilename(const char *filename, int width, int height, int rownum, int linenum, const char *SaveName, float time)
{
    cocos2d::Vector<SpriteFrame *> animarray;
    for (int i =0; i<rownum; ++i) {
        for (int j = 0; j<linenum; ++j) {
            SpriteFrame *frame = SpriteFrame::create(filename, Rect(width*j, height*i, width, height));
            animarray.pushBack(frame);
        }
    }
    auto fishanim = Animation::createWithSpriteFrames(animarray, time);
    AnimationCache::getInstance()->addAnimation(fishanim, SaveName);
}


