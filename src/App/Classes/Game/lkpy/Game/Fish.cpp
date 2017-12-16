//
//  Fish.cpp
//  MyGame
//
//  Created by wh on 15/5/27.
//
//

#include "Fish.h"
#include "lkpy_GameDataMgr.h"
#include "HallDataMgr.h"
#include "Project.h"
#include "lkpy_GameDefine.h"
USING_NS_CC;
USING_LKPY_NAMESPACE;
Fish * Fish::create(CMD_S_FishCreate *fishdata)
{
    auto pfish = new Fish;
    if (pfish->initwithType(fishdata)) {
        pfish->autorelease();
        return pfish;
    }
    CC_SAFE_DELETE(pfish);
    return  nullptr;
}

bool Fish::initwithType(CMD_S_FishCreate *fishdata)
{
    memset(&m_data, 0, sizeof(m_data));
    memcpy(&m_data, fishdata, sizeof(m_data));
    m_producttime = 0;
    this->setPosition(Vec2(-500, -500));
    this->setTag(Tag_Fish);
    m_pathIndex = 0;
    m_light = nullptr;
    m_guan = nullptr;
    m_isfastmove = false;
    return true;
}

void Fish::initAnim()
{
    auto namestr = __String::createWithFormat("fishMove_%03d_1.png", m_data.nFishType+1)->getCString();
    CCLOG("createFish %s",namestr);
    this->initWithSpriteFrameName(namestr);
    
    auto pname = __String::createWithFormat("animation_fish_move%d", m_data.nFishType+1)->getCString();
    auto panim = AnimationCache::getInstance()->getAnimation(pname);
    
    if (panim) {
        if(m_data.nFishType == FishType_BaoZhaFeiBiao)
        {
            this->setOpacity(0);
            auto pactionRepeat =  RepeatForever::create(RotateBy::create(8.5f,360.0f));

            CCSize size = this->getContentSize();
            Sprite *pCopySelf = Sprite::createWithSpriteFrameName(namestr);
            pCopySelf->setPosition(size/2);
            pCopySelf->runAction(pactionRepeat);
            this->addChild(pCopySelf,1);
            
            //this->runAction(paction);
        }
        else
        {
            auto paction = RepeatForever::create(Animate::create(panim));
            this->runAction(paction);
            //渐变出现
            this->setOpacity(0);
            auto pshowaction = FadeTo::create(1.f, 255);
            this->runAction(pshowaction);
        }
    }
    
    LONGLONG time = getsystemtomillisecond();
    m_ydtime = time - m_producttime;
    //this->scheduleUpdate();
}

void Fish::initphysicsBody()
{
    auto pbody = GameDataMgr::getInstance()->getBodyByType(m_data.nFishType);
    if (!pbody) {
        return;
    }
    pbody->setGravityEnable(false);
    pbody->setCategoryBitmask(1);
    pbody->setContactTestBitmask(2);
    pbody->setCollisionBitmask(0);
    this->setPhysicsBody(pbody);
    
    //if (m_data.nFishState == FishState_King || m_data.nFishState == FishState_Killer)
    if (m_data.nFishState != FishState_Normal)
    {
        int fishType = m_data.nFishType;
        if(fishType>6)
            fishType = 6;
        
        
        //if (m_data.bSpecial)
        if (m_data.nFishState == FishState_King)
        {
            m_guan = Sprite::createWithSpriteFrameName("fish_king.png");
            m_guan->setPosition(this->getContentSize()/2);
            this->addChild(m_guan,-1);
            auto paction = RepeatForever::create(RotateBy::create(8.5f,360.0f));
            m_guan->runAction(paction);
        }
        else if (m_data.nFishState == FishState_Killer) {
            m_guan = Sprite::createWithSpriteFrameName("fish_bomb_1.png");
            m_guan->setPosition(this->getContentSize()/2);
            this->addChild(m_guan,-1);
            auto paction = RepeatForever::create(RotateBy::create(8.5f,360.0f));
            m_guan->runAction(paction);
        }
        else if (m_data.nFishState == FishState_Aquatic) {
            m_guan = Sprite::createWithSpriteFrameName("fishMove_026_1.png");
            m_guan->setPosition(this->getContentSize()/2);
            this->addChild(m_guan,3);
            auto panim = AnimationCache::getInstance()->getAnimation(string("animation_fish_move26"));
            auto paction1 = Repeat::create(Animate::create(panim),999999);
            auto paction2 = Repeat::create(RotateBy::create(8.5f,360.0f),999999);
            m_guan->runAction(paction1);
            m_guan->runAction(paction2);
        }
    }
    //this->getPhysicsBody()->removeFromWorld();
    //this->removeComponent(_physicsBody);
    //this->removeAllComponents();
    
    //this->getPhysicsBody()->release();
    //this->setPhysicsBody(nullptr);
}

bool Fish::isCanAddtoScene()
{
    SCORE time = getsystemtomillisecond();
    if (m_producttime <= time && m_producttime!=0) {
        return true;
    }
    return false;
}

void Fish::update(float t)
{
    m_ydtime += t*1000;
    while(m_ydtime > m_data.TBezierPoint[m_pathIndex].Time) {
        m_ydtime -= m_data.TBezierPoint[m_pathIndex].Time;
        m_pathIndex++;
    }
    if (m_data.nBezierCount <= m_pathIndex) {
        //this->removeFromParent();
        
        //this->getPhysicsBody()->removeFromWorld();
        this->removeAllComponents();
        //removeComponent(_physicsBody);
        //this->getPhysicsBody()->release();
        //this->setPhysicsBody(nullptr);
        Fish* pfish = GameDataMgr::getInstance()->m_fishList.at(m_data.nFishKey);
        GameDataMgr::getInstance()->m_FishArray->removeObject(pfish);
        
        if(m_data.nFishState == FishState_King)
        {
            GameDataMgr::getInstance()->m_fishKingList.erase(m_data.nFishKey);
        }
        GameDataMgr::getInstance()->m_fishList.erase(m_data.nFishKey);
        auto paction = Sequence::createWithTwoActions(FadeTo::create(1.f, 0), CallFunc::create([=]{
            this->removeFromParent();
            if (m_light) {
                m_light->removeFromParent();
            }}));
        this->runAction(paction);
        return;
    }
    
    float percent = m_data.TBezierPoint[m_pathIndex].Time;
    percent = m_ydtime/percent;
    auto  point = this->PointOnCubicBezier(&m_data.TBezierPoint[m_pathIndex], percent);
    if (m_data.fRotateAngle) {
        auto beginvec2 = Vec2(m_data.TBezierPoint[0].BeginPoint.x, m_data.TBezierPoint[0].BeginPoint.y);
        point = this->RotatePoint(beginvec2, m_data.fRotateAngle, point);
    }
    point.add(Vec2(m_data.PointOffSet.x, m_data.PointOffSet.y));
    m_oldPoint = this->getPosition();
    //坐标转换
    this->setconvertpoint(point);
    
    auto angle = GameDataMgr::getInstance()->getAngleByTwoPoint(this->getPosition(), m_oldPoint);
    //angle = 180 -angle;
    this->setRotation(angle);
    if (m_light) {
        m_light->setRotation(angle);
    }
    
    if (this->getPosition().x<m_oldPoint.x && !GameDataMgr::getInstance()->m_reversal)
    {
        this->setFlippedX(true);
    }
    else if(this->getPosition().x<m_oldPoint.x && GameDataMgr::getInstance()->m_reversal)
    {
        this->setFlippedX(false);
    }
    else if (this->getPosition().x>m_oldPoint.x && GameDataMgr::getInstance()->m_reversal)
    {
        
        this->setFlippedX(true);
    }
    
    else
    {
        this->setFlippedX(false);
    }
//    if (m_guan) {
//        m_guan->setFlippedX(this->isFlippedX());
//    }
//    if (m_light) {
//        m_light->setFlippedX(this->isFlippedX());
//    }

    
}

void Fish::onEnter()
{
    Sprite::onEnter();
    this->scheduleUpdate();
}

void Fish::dealproductime()
{
    LONGLONG llEnterTime = lkpy_game::GameDataMgr::getInstance()->getEnterTime();
    m_producttime =  llEnterTime + m_data.unCreateTime; //+ m_data.unCreateDelay;
}

void Fish::deadDeal()
{
    this->stopAllActions();
    this->unscheduleUpdate();
    //this->getPhysicsBody()->removeFromWorld();
    //this->removeComponent(_physicsBody);
    //
    this->removeAllComponents();
    //this->getPhysicsBody()->release();
    //this->setPhysicsBody(nullptr);
    Fish* pfish = lkpy_game::GameDataMgr::getInstance()->m_fishList.at(m_data.nFishKey);
    GameDataMgr::getInstance()->m_FishArray->removeObject(pfish);
    GameDataMgr::getInstance()->m_fishList.erase(m_data.nFishKey);
    if(m_data.nFishState == FishState_King)
    {
        lkpy_game::GameDataMgr::getInstance()->m_fishKingList.erase(m_data.nFishKey);
    }
    //auto pname = __String::createWithFormat("fish_%d_die", m_data.nFishType)->getCString();
    auto pname = __String::createWithFormat("animation_fish_dead%d", m_data.nFishType+1)->getCString();
    auto panim = AnimationCache::getInstance()->getAnimation(pname);
    auto pParent = (Layer*)this->getParent();
    
    //爆炸飞镖效果
    if(pfish->getFishData()->nFishType == FishType_BaoZhaFeiBiao)
    {
        int nKnife = 18;
        float angle = 360.0f / nKnife;
        float radius = 1200.0f;
        for(int i =0;i < nKnife;++i)
        {
            auto pKnife = Sprite::create("game_res/knife.png");
            pKnife->setAnchorPoint(Vec2(0, 0.5f));
            pKnife->setPosition(this->getPosition());
            Point pos =this->getPosition();
            Point purPos;
            purPos.x = pos.x + radius * cos(i*M_PI/nKnife*2.0f);
            purPos.y = pos.y + radius * sin(i*M_PI/nKnife*2.0f);
            
            
            purPos = GameDataMgr::getInstance()->convertCoordinateSystem(purPos, 1, GameDataMgr::getInstance()->m_reversal);
            
            pKnife->runAction(Sequence::create(MoveTo::create(1.5f, purPos)
                                               ,CallFunc::create([=]{pKnife->removeFromParent();})
                                               ,NULL));
            pParent->addChild(pKnife);
            
            float angle = (purPos.y -pKnife->getPosition().y)/(purPos.x - pKnife->getPosition().x);
            angle = atanf(angle)/M_PI*180;
            if(angle < 0){
                angle=-(90 + angle);
                if(GameDataMgr::getInstance()->m_reversal)
                    angle+=180;
                pKnife->setRotation(angle);
                
                
            }else if(angle > 0){
                angle=90 - angle;
                if(GameDataMgr::getInstance()->m_reversal)
                    angle+=180;
                pKnife->setRotation(angle);
            }
            
        }
        //全屏鱼的位置出现飞镖爆炸
        int nBoom = 14+rand()%5;
        float fDelayTime;
        for(int i = 0;i <nBoom;++i)
        {
            fDelayTime = (rand() % 4) * 0.13f;
            auto pBoomAnim = AnimationCache::getInstance()->getAnimation(BombDartsAnim);
            auto pbomb = Sprite::createWithSpriteFrameName("boom_darts0.png");
            Point pos;
            pos.x = 100 + rand() % 1036;
            pos.y = 100 + rand() % 540;
            pbomb->setPosition(pos);
            auto paction = Sequence::create(DelayTime::create(fDelayTime),Animate::create(pBoomAnim),CallFunc::create([=]{
                pbomb->removeFromParent();
            }),NULL);
            pbomb->runAction(paction);
            pParent->addChild(pbomb,41);
        }


    }
    
    //水浒传冰冻效果
    if(pfish->getFishData()->nFishType == FishType_ShuiHuZhuan)
    {
        float radius = 580.0f;
        int nBomb = 12;
        float angle = 360.0f / nBomb;
        Point pos =this->getPosition();
        for(int i =0;i < nBomb;++i)
        {
            auto pBoomAnim = AnimationCache::getInstance()->getAnimation(BlueIceAnim);
            auto pbomb = Sprite::createWithSpriteFrameName("blue00.png");
            pbomb->setPosition(this->getPosition());
            pbomb->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(BlueIceAnim)));
            pParent->addChild(pbomb,40);

            
            if (pBoomAnim)
            {
                
                ActionInterval *paction1 = NULL;
                    Point purPos;
                    purPos.x = pos.x + radius * cos(i*M_PI/nBomb*2.0f);
                    purPos.y = pos.y + radius * sin(i*M_PI/nBomb*2.0f);
                    purPos = GameDataMgr::getInstance()->convertCoordinateSystem(purPos, 2, GameDataMgr::getInstance()->m_reversal);
                    paction1 = MoveTo::create(0.8f, purPos);
                auto paction = Sequence::create(paction1
                                                , CallFunc::create([=]{
                    pbomb->removeFromParent();
                }), NULL);
                pbomb->runAction(paction);
            }
            
        }
    }
    
    if( (pfish->getFishData()->nFishType >= FishType_JianYu && pfish->getFishData()->nFishType <= FishType_LiKui) || pfish->getFishData()->nFishType == FishType_BaoZhaFeiBiao)
    //if(false)
    {
        float radius = 360.0f;
        int nBomb = 1;
        if(pfish->getFishData()->nFishType >= FishType_JianYu && pfish->getFishData()->nFishType <= FishType_DaJinSha)
            nBomb = 1;
        else if(pfish->getFishData()->nFishType > FishType_DaJinSha && pfish->getFishData()->nFishType <= FishType_LiKui)
            nBomb = 6;
        else if(pfish->getFishData()->nFishType == FishType_BaoZhaFeiBiao)
        	{
        		nBomb = 8;
        		radius = 580;
        	}
        float angle = 360.0f / nBomb;
        Point pos =this->getPosition();
        for(int i =0;i < nBomb;++i)
        {
            auto pBoomAnim = AnimationCache::getInstance()->getAnimation(BombAnim);
            auto pbomb = Sprite::createWithSpriteFrameName("boom00.png");
            pbomb->setPosition(this->getPosition());
            pbomb->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(BombAnim)));
            pParent->addChild(pbomb,40);

            
            if (pBoomAnim)
            {
                
                ActionInterval *paction1 = NULL;
                if(nBomb == 1)
                    paction1 = DelayTime::create(0.8f);
                else
                {
                    Point purPos;
                    purPos.x = pos.x + radius * cos(i*M_PI/nBomb*2.0f);
                    purPos.y = pos.y + radius * sin(i*M_PI/nBomb*2.0f);
                    purPos = GameDataMgr::getInstance()->convertCoordinateSystem(purPos, 2, GameDataMgr::getInstance()->m_reversal);
                    paction1 = MoveTo::create(0.8f, purPos);
                    
                }
                auto paction = Sequence::create(paction1
                                                , CallFunc::create([=]{
                    pbomb->removeFromParent();
                }), NULL);
                pbomb->runAction(paction);
            }
            
        }
        
        
    }
    
    
    if (panim) {
        auto paction1 = Repeat::create(Animate::create(panim), 4);
        auto paction = Sequence::create(paction1, CallFunc::create([=]{this->removeFromParent();}), NULL);
        this->runAction(paction);
    }
    else
    {
        this->removeFromParent();
    }
    
    if (m_light) {
        m_guan->removeFromParent();
        m_light->removeFromParent();
        m_guan = nullptr;
        m_light = nullptr;
    }
    
    
}

cocos2d::FiniteTimeAction *Fish::createPath(int index)
{
//    ccBezierConfig config;
//    config.controlPoint_1 = Vec2(m_data.TBezierPoint[index].KeyOne.x, m_data.TBezierPoint[index].KeyOne.y);
//    config.controlPoint_2 = Vec2(m_data.TBezierPoint[index].KeyTwo.x, m_data.TBezierPoint[index].KeyTwo.y);
//    config.endPosition = Vec2(m_data.TBezierPoint[index].EndPoint.x, m_data.TBezierPoint[index].EndPoint.y);
//    auto time = m_data.TBezierPoint[index].Time/1000.f;
//    auto paction = RotateWithAction::create(time, config);
//    return paction;
    return nullptr;
}

void Fish::PathCallback()
{
//    if (m_data.nBezierCount <= m_pathIndex)
//    {
//        this->removeFromParent();
//        GameDataMgr::getInstance()->m_fishList.erase(m_data.nFishIndex);
//        return;
//    }
//    auto paction = this->createPath(m_pathIndex);
//    this->setPosition(Vec2(m_data.TBezierPoint[m_pathIndex].BeginPoint.x, m_data.TBezierPoint[m_pathIndex].BeginPoint.y));
//    m_pathIndex++;
//    this->runAction(Sequence::createWithTwoActions(paction, CallFunc::create(CC_CALLBACK_0(Fish::PathCallback, this))));
}

cocos2d::Vec2 Fish::PointOnCubicBezier(tagBezierPoint *path,DOUBLE t)
{
    DOUBLE ax = 0.f,bx = 0.f, cx = 0.f;
    DOUBLE ay = 0.f,by = 0.f, cy = 0.f;
    DOUBLE tSquared = 0.f,tCubed = 0.f;
    Vec2 result = Vec2::ZERO;
    cx = 3.f * (path->KeyOne.x - path->BeginPoint.x );
    bx = 3.f * (path->KeyTwo.x - path->KeyOne.x) - cx;
    ax = path->EndPoint.x - path->BeginPoint.x - cx - bx;
    
    cy = 3.f * (path->KeyOne.y - path->BeginPoint.y );
    by = 3.f * (path->KeyTwo.y - path->KeyOne.y) - cy;\
    ay = path->EndPoint.y - path->BeginPoint.y - cy - by;
    
    tSquared = t*t;
    tCubed = tSquared*t;
    
    result.x = (ax * tCubed) + (bx * tSquared) + (cx * t ) + path->BeginPoint.x;
    result.y = (ay * tCubed) + (by * tSquared) + (cy * t ) + path->BeginPoint.y;
    
    return result;
}

cocos2d::Vec2 Fish::RotatePoint(cocos2d::Vec2 &pcircle, double dradian, cocos2d::Vec2 &ptsome)
{
    Vec2 temp;
    ptsome -= pcircle;
    temp.x = ptsome.x*cos(dradian) - ptsome.y*sin(dradian) + pcircle.x;
    temp.y = ptsome.x*sin(dradian) + ptsome.y*cos(dradian) + pcircle.y;
    return temp;
}

void Fish::setconvertpoint(const cocos2d::Vec2 &vec2)
{
    
    auto pos = Vec2(vec2.x*ScaleX, (Win32Size.y-vec2.y)*ScaleY);
    //pos = Director::getInstance()->convertToGL(pos);
    //pos=vec2;
    this->setPosition(pos);
    if (m_light) {
        m_light->setPosition(pos);
    }
}

void Fish::setFastMove()
{
    this->unscheduleUpdate();
    if (m_light) {
        m_light->removeFromParent();
    }
    auto viewsize = WinSize;
    auto point = this->getPosition();
    auto rect1 = Rect(0, 0, viewsize.width/2, viewsize.height/2);
    auto rect2 = Rect(viewsize.width/2, 0, viewsize.width/2, viewsize.height/2);
    auto rect3 = Rect(0, viewsize.height/2, viewsize.width/2, viewsize.height/2);
    auto rect4 = Rect(viewsize.width/2, viewsize.height/2, viewsize.width/2, viewsize.height/2);
    auto callfunc = [=]{this->removeFromParent();};
    Vec2 targetpoint;
    
    auto rect = Rect(0, 0, viewsize.width, viewsize.height);
    if (rect.containsPoint(point)) {
        if (rect1.containsPoint(point)) {
            targetpoint = Vec2(-200, -200);
        }
        else if(rect2.containsPoint(point))
        {
            targetpoint = Vec2(viewsize.width+200, -200);
        }
        else if(rect3.containsPoint(point))
        {
            targetpoint = Vec2(-200, viewsize.height+200);
        }
        else if(rect4.containsPoint(point))
        {
            targetpoint = Vec2(viewsize.width+200, viewsize.height+200);
        }
        auto paction = Sequence::create(MoveTo::create(2.f, targetpoint), FadeTo::create(0.5f, 0), CallFunc::create(callfunc), NULL);
        auto angle = GameDataMgr::getInstance()->getAngleByTwoPoint(targetpoint, point);
        this->setRotation(angle);
        this->runAction(paction);
    }
    else
    {
        auto paction = Sequence::createWithTwoActions(FadeTo::create(0.5f, 0), CallFunc::create(callfunc));
        this->runAction(paction);
    }
}

void Fish::stay(float _time)
{
    this->unscheduleUpdate();
    this->scheduleOnce(schedule_selector(Fish::startUpdate),_time/1000);
}

void Fish::startUpdate(float dt)
{
    this->scheduleUpdate();
}
