//
//  Bullet.cpp
//  MyGame
//
//  Created by wh on 15/6/2.
//
//

#include "Bullet.h"
#include "lkpy_GameDataMgr.h"
#include "CMD_Stdafx.h"
#include "NetworkMgr.h"
#include "Cannon.h"
USING_NS_CC;
using namespace ui;
using namespace std;
USING_LKPY_NAMESPACE;
Bullet * Bullet::create(float angle,WORD _wChiarId,int _score,CannonType _cannonType)
{
    Bullet* pbullet = new Bullet();
    pbullet->m_nScore = _score;
    if (pbullet->initwithangle(angle,_wChiarId,_score,_cannonType)) {
        pbullet->autorelease();
        return pbullet;
    }
    CC_SAFE_DELETE(pbullet);
    return nullptr;
}

bool Bullet::initwithangle(float angle,WORD _wChiarId,int _score,CannonType _cannonType)
{
    int nBulletNum = getBulletNum(_score);
    __String *str = NULL;
    if(_cannonType != Special_Cannon)
        str = __String::createWithFormat("Bullet%d_Normal_%d_b.png",nBulletNum,_wChiarId+1);
    else
        str = __String::createWithFormat("Bullet%d_Specialt_b.png",nBulletNum);
    //CCLOG(str->getCString());
    //this->initWithSpriteFrameName("bullet_0.png");
    this->initWithSpriteFrameName(str->getCString());
    this->setRotation(angle);
    //m_nScore = _score;
    m_fishindex = INT_MAX;
    m_Type = Normal_Bullet;
    m_isturn = false;
    m_netcolor = Color3B::RED;
    this->setTag(Tag_Bullet);
    m_speed = GameDataMgr::getInstance()->m_sceneData.nBulletVelocity/1000.f*25;
    m_movedir = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-this->getRotation()));
    m_targetpoint = Vec2::ZERO;
    
    //BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    //this->setBlendFunc(blendFunc);
    //this->scheduleUpdate();
    
//    auto panim = AnimationCache::getInstance()->getAnimation(BulletAnim);
//    if (panim) {
//        auto paction = RepeatForever::create(Animate::create(panim));
//        this->runAction(paction);
//    }
    return true;
}

void Bullet::onEnter()
{
    Sprite::onEnter();
    this->scheduleUpdate();
}

int Bullet::getBulletNum(int _score)
{
    int nNum = _score / 100;
    int nBulletNum = 1;
    if(nNum < 1)
    {
        nBulletNum = 1;
    }
    else if(nNum >= 1&&nNum < 10)
    {
        nBulletNum = 2;
    }
    else if(nNum >= 10)
    {
        nBulletNum = 3;
    }
    return nBulletNum;
}



void Bullet::changeDisplayFrame(WORD _wChiarId,int _score)
{
    int nBulletNum = getBulletNum(_score);
    auto str = __String::createWithFormat("Bullet%d_Normal_%d_b.png",nBulletNum,_wChiarId+1);
    this->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(string(str->getCString())));
}

void Bullet::update(float t)
{
    if (m_fishindex == INT_MAX) {
        if (m_targetpoint.isZero()) {
            this->nomalupdate(t);
        }
        else
        {
            auto movedis = t*m_speed;
            auto movedir = m_movedir*movedis;
            this->setPosition(this->getPosition()+movedir);
            auto distant = m_targetpoint.distance(this->getPosition());
            if ( distant <= movedis*2) {
                this->setPosition(m_targetpoint);
                //m_bbullet->setPosition(m_targetpoint+Vec2(5, 2));
                this->fallingnet();
                //m_bbullet->fallingnet();
                
               // m_bbullet->removeFromParent();
                this->removeFromParent();
            }
        }
    }
    else
    {
        this->followFish(t);
    }
}

void Bullet::nomalupdate(float t)
{
    auto viewsize = WinSize;
    auto movedis = t*m_speed;
    auto movedir = m_movedir*movedis;
    this->setPosition(this->getPosition()+movedir);
    auto rect = cocos2d::Rect(0, 0, viewsize.width, viewsize.height);
    auto m_point = this->getPosition();
    if (!rect.containsPoint(m_point)) {
        if (m_isturn && GameDataMgr::getInstance()->m_sceneData.bUnlimitedRebound==false) {
            this->fallingnet();
            m_bbullet->fallingnet();
            this->removeFromParent();
            m_bbullet->removeFromParent();
        }
        else
        {
            //m_isturn = true;
            if (m_point.x<0 || m_point.x>viewsize.width) {
                auto fangle = this->getRotation();
                this->setRotation(-fangle);
                if (m_point.x < 0.f) {
                    m_point.x = 0.f;
                }
                else
                {
                    m_point.x = WinSize.width;
                }
            }
            else
            {
                auto fangle = this->getRotation();
                this->setRotation(-fangle + 180);
                if (m_point.y < 0.f) {
                    m_point.y = 0.f;
                }
                else
                {
                    m_point.y = WinSize.height;
                }
            }
            m_movedir = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-this->getRotation()));
            auto movedis = t*m_speed;
            auto movedir = m_movedir*movedis;
            this->setPosition(this->getPosition()+movedir);
        }
    }
}

void Bullet::followFish(float t)
{
    auto pfish = GameDataMgr::getInstance()->m_fishList.at(m_fishindex);
    auto rect = cocos2d::Rect(0, 0, WinSize.width, WinSize.height);
    if (!pfish ||!rect.containsPoint(pfish->getPosition())) {
        m_fishindex = INT_MAX;
        auto pbody = GameDataMgr::getInstance()->getBodyByName("Bullet1_Normal_1_b");
        if (pbody) {
            this->setPhysicsBody(pbody);
            //pbody->setPosition(ccp(20,20));
            pbody->setCategoryBitmask(2);
            pbody->setContactTestBitmask(1);
            pbody->setCollisionBitmask(0);
        }
        return;
    }
    
    auto fishpoint = pfish->getPosition();
    if (GameDataMgr::getInstance()->m_reversal) {
        fishpoint = Vec2(WinSize.width, WinSize.height) - fishpoint;
    }
    
    auto fangle = GameDataMgr::getInstance()->getAngleByTwoPoint(fishpoint, this->getPosition());
    this->setRotation(fangle);
    
    m_movedir = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-fangle));
    auto movedis = t*m_speed;
    auto movedir = m_movedir*movedis;
    this->setPosition(this->getPosition()+movedir);
    if (fishpoint.distance(this->getPosition()) <= movedis) {
        this->setPosition(fishpoint);
        //m_bbullet->setPosition(fishpoint+Vec2(5, 2));
        this->fallingnet();
        //m_bbullet->fallingnet();
        
        //m_bbullet->removeFromParent();
        this->removeFromParent();
    }
}

Rect Bullet::getNetRect(Point centerPos,Point* offsetArray,int len,Size contentSize)
{
    float minX = 0;
    float minY = 0;
    float maxX = 0;
    float maxY = 0;
    for (int i =0; i < len; i++)
    {
        Point pos = offsetArray[i];
        float _minX = pos.x - contentSize.width/2;
        float _minY = pos.y - contentSize.height/2;
        float _maxX = pos.x + contentSize.width/2;
        float _maxY = pos.y + contentSize.height/2;
        if(_minX<minX)
            minX = _minX;
        if(_maxX>maxX)
            maxX = _maxX;
        if(_minY<minY)
            minY = _minY;
        if(_maxY>maxY)
            maxY = _maxY;
    }
    //Rect rect(centerPos.x+(maxX+minX)/2, centerPos.y+(maxY+minY)/2, fabs(maxX-minX), fabs(maxY-minY));
    Rect rect(centerPos.x, centerPos.y, contentSize.width+10,contentSize.height+10);
    return rect;
}

void Bullet::sendCatchFish(Rect rect)
{
    
    Object* obj;
    Array* tempArray = Array::create();
    for(auto iter : GameDataMgr::getInstance()->m_fishList)
    {
        Fish* pFish = iter.second;
        Point pos = pFish->getPosition();
        Rect rect2 = pFish->boundingBox();
        if(rect.intersectsRect(pFish->boundingBox()))
            //if(rect.containsPoint(pFish->getPosition()))
        {
            tempArray->addObject(pFish);
        }
    }
    int nCount = 0;//网中符合条件的鱼的个数
    Array* catchArray = Array::create();
    //如果符合条件总数大于等于5而当前捕获的鱼小于5则一直循环 先加大鱼再加小鱼
    bool isBigFish = true;//
    
    do
    {
    //优先抓大鱼
        Object* obj;
        CCARRAY_FOREACH(tempArray, obj)
        {
            Fish* pFish = (Fish*)obj;
            CMD_S_FishCreate *cmd_fc = pFish->getFishData();       //或者大鱼选完后选小鱼
            if( (isBigFish&&cmd_fc->nFishState != FishState_Normal) || ( !isBigFish&&cmd_fc->nFishState == FishState_Normal))
            {
                catchArray->addObject(pFish);
                //tempArray->removeObject(pFish);
                nCount++;
                if(nCount>=5)
                    break;
            }
        }
        if(!isBigFish)
            break;
        if(isBigFish)
            isBigFish = false;
        if(nCount>=5)
            break;
    }while (!isBigFish&&nCount<5);
    

    CMD_C_CatchFish request;
    memset(&request, 0, sizeof(request));
    
            request.nBulletKey = m_index;
    int nFishCount = catchArray->count();
    for (int i = 0; i < 5; i++) {
        //超过捕获的鱼填0
        if(i>=nFishCount)
            request.nFishKey[i] = 0;
        else
        {
            Fish* pFish = (Fish*) catchArray->objectAtIndex(i);
            request.nFishKey[i] = pFish->getFishData()->nFishKey;
        }
        
    }
            NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_CATCH_FISH, &request, sizeof(request));

    
}

void Bullet::fallingnet()
{
    Point pointArray2[2] = {Point(-25,0),Point(25,0)};
    Point pointArray3[3] = {Point(0,20),Point(-cos(PI/6)*20,-sin(PI/6)*20),Point(cos(PI/6)*20,-sin(PI/6)*20)};
    Point pointArray4[4] = {Point(-25,25),Point(25,25),Point(25,-25),Point(-25,-25)};
    
    this->unscheduleUpdate();
    auto pparent = this->getParent();
    if (!pparent) {
        return;
    }
    Point* posPtr = NULL;
    int nBulletNum = getBulletNum(m_nScore);
    if(nBulletNum==1)
        posPtr = pointArray2;
    else if(nBulletNum==2)
        posPtr = pointArray3;
    else if(nBulletNum==3)
        posPtr = pointArray4;
    
    auto offset = m_movedir*20;
    Size netSize;
    CCRect rect;
    for (int i = 0;i<= nBulletNum; i++)
    {
        Sprite* pnet;
        if (m_Type == Normal_Bullet || m_Type == Special_Bullet) {
            pnet = Sprite::create("game_res/im_net.png");
        }
        else if (m_Type == Bignet_Bullet)
        {
            pnet = Sprite::create("game_res/im_net_big.png");
        }
        if(i == nBulletNum)
        {
            netSize = pnet->getContentSize();
        }
        pnet->setScale(205.f/pnet->getContentSize().width);
        if (m_Type == Bignet_Bullet) {
            pnet->setScale(pnet->getScale()*1.5f);
        }
       
//        auto pcolor = Sprite::create("game_res/im_net_dot.png");
//        pcolor->setColor(m_netcolor);
//        pcolor->setPosition(Vec2(pnet->getContentSize().width/2, pnet->getContentSize().height/2));
//        pnet->addChild(pcolor);
        //pcolor->setPosition(this->getPosition()+Point(pnet->getContentSize().width/3*2,0));
        
        //this->addChild(pcolor);
        
        pnet->setPosition(this->getPosition()+offset);
        rect = pnet->boundingBox();
        pnet->setPosition(this->getPosition()+offset+posPtr[i]);
        
        auto *paction = ScaleTo::create(0.08f, pnet->getScale()*1.16f);
        auto *rev_action = ScaleTo::create(0.08f, pnet->getScale());
        auto *sq = Sequence::create(paction, rev_action, paction, CallFunc::create([=]{pnet->removeFromParent();}), NULL);
        pnet->runAction(sq);
        pnet->runAction(Sequence::createWithTwoActions(DelayTime::create(0.16f), FadeTo::create(0.05f, 0)));
        
        pparent->addChild(pnet,1);
    }
    
    
    if (m_isself) {
        Sprite* pnet;
        if (m_Type == Normal_Bullet || m_Type == Special_Bullet) {
            pnet = Sprite::create("game_res/im_net.png");
        }
        else if (m_Type == Bignet_Bullet)
        {
            pnet = Sprite::create("game_res/im_net_big.png");
        }

        
        Point catchPos = GameDataMgr::getInstance()->convertCoordinateSystem(this->getPosition()+offset, 2, GameDataMgr::getInstance()->m_reversal);
        pnet->setPosition(catchPos);
        //sendCatchFish(getNetRect(this->getPosition()+offset, posPtr, nBulletNum+1, netSize));
        //rect.origin = GameDataMgr::getInstance()->convertCoordinateSystem(catchPos, 1, GameDataMgr::getInstance()->m_reversal);
        Rect rect = pnet->boundingBox();
        rect.size.width = rect.size.width-20+nBulletNum*10;
        rect.size.height = rect.size.height-20+nBulletNum*10;
       // Rect rect(catchPos.x,catchPos.y,netSize.width-10,netSize.height-10);
        sendCatchFish(rect);
       
    }
}

void Bullet::setType(int type)
{
    int nBulletNum = getBulletNum(m_nScore);
    m_Type = (BulletType)type;
//    Sprite *pwarhead;
//    if (m_Type==Normal_Bullet || m_Type==Bignet_Bullet) {
//        //子弹头
//        pwarhead = Sprite::create("game_res/im_bullet.png");
//    }
//    else if (m_Type == Special_Bullet)
//    {
//        //子弹头
//        pwarhead = Sprite::create("game_res/im_bullet_red.png");
//    }
//   
//    pwarhead->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
//    this->addChild(pwarhead);
//    pwarhead->setVisible(false);
    //auto str = __String::createWithFormat("Bullet%d_Normal_%d_b.png",nBulletNum,_wChiarId+1);
    __String* str = NULL;
    if (m_Type == Special_Bullet) {
        m_speed = m_speed*2;
        str = __String::createWithFormat("Bullet%d_Specialt_b",nBulletNum);
    }
    else
    {
        str = __String::createWithFormat("Bullet%d_Normal_1_b",nBulletNum);
        
    }
    //auto pbody = GameDataMgr::getInstance()->getBodyByName(str->getCString());
    auto pbody = GameDataMgr::getInstance()->getBodyByName("Bullet1_Normal_1_b");
    if (!pbody) {
        return;
    }
    
    this->setPhysicsBody(pbody);
    pbody->setCategoryBitmask(2);
    pbody->setContactTestBitmask(1);
    pbody->setCollisionBitmask(0);
}

void Bullet::setFishIndex(DWORD fishindex)
{
    m_fishindex = fishindex;
    if (fishindex != INT_MAX) {
        //this->getPhysicsBody()->removeFromWorld();
        //this->removeComponent(_physicsBody);
        this->removeAllComponents();
        
        //this->getPhysicsBody()->release();
        //this->setPhysicsBody(nullptr);
    }
}

void Bullet::setTargetPoint(cocos2d::Vec2 target)
{
    m_targetpoint = target;
    
    //this->getPhysicsBody()->removeFromWorld();
    //this->removeComponent(_physicsBody);
    this->removeAllComponents();
    //this->getPhysicsBody()->release();
    //this->setPhysicsBody(nullptr);
}