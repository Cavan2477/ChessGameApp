//
//  GameRoleInfoLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/31/15.
//
//

#include "GameRoleInfoLayer.h"
#include "GameDataMgr.h"
#include "../DataMgr/GameLogic.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

GameRoleInfoLayer::GameRoleInfoLayer():
m_pHead(nullptr)
{
    m_actHideAct = nullptr;
    m_actShowAct = nullptr;
}

GameRoleInfoLayer::~GameRoleInfoLayer()
{
    CC_SAFE_RELEASE(m_actShowAct);
    CC_SAFE_RELEASE(m_actHideAct);
}

bool GameRoleInfoLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        LOAD_CSB("game/GameRoleInfo.csb");
        
        m_spBg = static_cast<Sprite*>(m_root->getChildByName("bg_kuang"));
        CC_ASSERT(m_spBg != nullptr);
        
        m_spHeadBg = static_cast<Sprite*>(m_spBg->getChildByName("head_bg"));
        CC_ASSERT(m_spHeadBg != nullptr);
        
        auto tmp = m_spBg->getChildByName("nick_text");
        CC_ASSERT(tmp != nullptr);
        m_clipNickName = ClipText::createClipText(tmp->getContentSize(), "");
        CC_ASSERT(m_clipNickName != nullptr);
        m_clipNickName->setTextColor(Color4B::BLACK);
        m_clipNickName->setTextAnchorPoint(Vec2(0, 0));
        m_clipNickName->setPosition(tmp->getPosition());
        m_spBg->addChild(m_clipNickName);
        tmp->removeFromParentAndCleanup(true);
        
        m_textId = static_cast<Text*>(m_spBg->getChildByName("id_text"));
        CC_ASSERT(m_textId != nullptr);
        
        m_textMoney = static_cast<Text*>(m_spBg->getChildByName("money_text"));
        CC_ASSERT(m_textMoney != nullptr);
        
        //m_textWin = static_cast<Text*>(m_root->getChildByName("win_text"));
        //CC_ASSERT(m_textWin != nullptr);
        m_textBean = static_cast<Text*>(m_spBg->getChildByName("bean_text"));
        CC_ASSERT(m_textBean != nullptr);
        /*
        m_textChengJi = static_cast<Text*>(m_spBg->getChildByName("chengji_text"));
        CC_ASSERT(m_textChengJi != nullptr);
        
        m_textMingZhongLv = static_cast<Text*>(m_spBg->getChildByName("mingzhonglv_text"));
        CC_ASSERT(m_textMingZhongLv != nullptr);
        */
        m_actShowAct = ScaleTo::create(0.5f, 1.0f);
        m_actShowAct->retain();
        
        auto scal = ScaleTo::create(0.5f, 0.01f);
        auto call = CallFunc::create([this](){ this->setVisible(false); });
        m_actHideAct = Sequence::create(scal, call, NULL);
        m_actHideAct->retain();
        
        bRes = true;
    } while (false);
    return bRes;
}

void GameRoleInfoLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    
    m_listener->onTouchBegan = [this](Touch *pTouch,Event *event)
    {
        return this->isVisible();
    };
    m_listener->onTouchEnded = [this](Touch *pTouch,Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        pos = m_spBg->convertToNodeSpace(pos);
        Rect rec = Rect(0, 0, m_spBg->getContentSize().width, m_spBg->getContentSize().height);
        if (!rec.containsPoint(pos))
        {
            this->setVisible(false);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void GameRoleInfoLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Layer::onExit();
}

void GameRoleInfoLayer::refresh(UserData *pUser, const enGameRoleDir &dir)
{
    this->setVisible(true);
    m_spBg->setScale(0.01f);
    switch (dir)
    {
        case enGameRoleDir::kMY_ROLE:
        {
            m_spBg->setAnchorPoint(Vec2(0, 0));
            m_spBg->setPosition(Vec2(185, 150));
        }
            break;
        case enGameRoleDir::kLEFT_ROLE:
        {
            m_spBg->setAnchorPoint(Vec2(0, 0));
            m_spBg->setPosition(Vec2(134, 265));
        }
            break;
        case enGameRoleDir::kRIGHT_ROLE:
        {
            m_spBg->setAnchorPoint(Vec2(1.0f, 0));
            m_spBg->setPosition(Vec2(1000, 265));
        }
            break;
        default:
            break;
    }
    m_spBg->stopAllActions();
    m_spBg->runAction(m_actShowAct);
    //头像
    if (nullptr == m_pHead)
    {
        m_pHead = HeaderRequest::createwithFaceID(pUser->m_date.wFaceID,
                                                  pUser->m_date.dwCustomID,
                                                  pUser->m_date.dwUserID,
                                                  pUser->m_date.cbGender);
        m_pHead->setPosition(m_spHeadBg->getPosition());
        m_spBg->addChild(m_pHead);
        m_pHead->setHeadSize(120.0f);
    }
    else
    {
        if (0 != pUser->m_date.dwCustomID)
        {
            auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(pUser->m_date.dwUserID);
            if (ptexture)
            {
                m_pHead->setTexture(ptexture);
                m_pHead->setTextureRect(cocos2d::Rect(0, 0, FACE_CX, FACE_CY));
            }
        }
        else
        {
            auto faceID = pUser->m_date.wFaceID%10;
            std::string headstr = 1 == pUser->m_date.cbGender? "man" : "woman";
            headstr = __String::createWithFormat("public_res/im_head_%s_%d.png", headstr.c_str(), faceID)->getCString();
            m_pHead->initWithFile(headstr);
        }
    }
    
    char buf[64] = "";
    
    //姓名
    if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
    {
        m_clipNickName->setString("玩家");
    }
    else
    {
        
        m_clipNickName->setString(pUser->m_nickname);
    }
    
    //id
    sprintf(buf, "%d",pUser->m_date.dwGameID);
    m_textId->setString(buf);
    
    //金币
    sprintf(buf, "%lld",pUser->m_date.lScore);
    m_textMoney->setString(buf);
    
    //游戏豆
    sprintf(buf, "%.2f",pUser->m_date.dBean);
    m_textBean->setString(buf);
    /*
    //成绩
    sprintf(buf, "%lld",INSTANCE(CGameLogic)->m_llTotalScore);
    m_textChengJi->setString(buf);
    
    float tmpRate = INSTANCE(CGameLogic)->m_fGameRate;
    if (tmpRate > 0)
    {
        //命中率
        sprintf(buf, "%.2f%%",tmpRate * 100);
        m_textMingZhongLv->setString(buf);
    }
    else
    {
        m_textMingZhongLv->setString("无战果");
    }
     */
}