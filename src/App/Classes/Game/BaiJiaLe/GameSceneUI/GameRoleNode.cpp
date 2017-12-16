//
//  GameRoleNode.cpp
//  DouDiZhu
//
//  Created by zhong on 1/21/16.
//
//

#include "GameRoleNode.h"
#include "UtilEx.h"

USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

GameRoleNode::GameRoleNode():
m_pHead(nullptr),
m_notify(nullptr),
m_clipNickName(nullptr),
m_spBanker(nullptr),
m_bConvertBanker(false),
m_bConvertHead(false)
{
    auto scaleTo = ScaleTo::create(0.1f,1.0f);
    auto show = Show::create();
    m_actTip = Spawn::create(scaleTo,show,nullptr);
    m_actTip->retain();
}

GameRoleNode::~GameRoleNode()
{
    CC_SAFE_RELEASE(m_actTip);
}

bool GameRoleNode::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Node::init());
        LOAD_CSB_ITEM("game/GameRoleItem.csb");
        
        m_spHeadFrame = static_cast<Sprite*>(m_root->getChildByName("head_frame"));
        CC_ASSERT(m_spHeadFrame != nullptr);
        m_spHeadFrame->setLocalZOrder(-1);
        
        m_spHeadBg = static_cast<Sprite*>(m_root->getChildByName("head_bg"));
        CC_ASSERT(m_spHeadBg != nullptr);
        m_spHeadBg->setLocalZOrder(1);
        
        auto tmp = m_spHeadBg->getChildByName("name_text");
        CC_ASSERT(tmp != nullptr);
        m_clipNickName = ClipText::createClipText(Size(80, 20), "", "Arial",14);
        CC_ASSERT(m_clipNickName != nullptr);
        m_clipNickName->setTextColor(Color4B::YELLOW);
        m_clipNickName->setTextAnchorPoint(Vec2(0, 0));
        m_clipNickName->setPosition(tmp->getPosition());
        m_spHeadBg->addChild(m_clipNickName);
        
        m_textCoin = static_cast<Text*>(m_spHeadBg->getChildByName("coin_text"));
        CC_ASSERT(m_textCoin != nullptr);
        
        /*
         m_spBanker = static_cast<Sprite*>(m_root->getChildByName("banker"));
         CC_ASSERT( m_spBanker != nullptr);
         
         m_spOpenCard = static_cast<Sprite*>(m_root->getChildByName("opencard"));
         CC_ASSERT( m_spOpenCard != nullptr);
         */
        
        m_spChatBg = static_cast<ImageView*>(m_root->getChildByName("chat_bg"));
        CC_ASSERT(m_spChatBg != nullptr);
        
        m_spBrow = static_cast<Sprite*>(m_spChatBg->getChildByName("chat_face"));
        CC_ASSERT(m_spBrow != nullptr);
        
        m_labChat = Label::createWithSystemFont("", "Arial", 20,Size(180,0));
        m_labChat->setPosition(m_spChatBg->getContentSize().width * 0.5, m_spChatBg->getContentSize().height * 0.6);
        m_labChat->setVerticalAlignment(TextVAlignment::CENTER);
        m_spChatBg->addChild(m_labChat);
        
        this->reSet();
        bRes = true;
    } while (false);
    return bRes;
}

void GameRoleNode::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(false);
    m_listener->onTouchBegan = [this](Touch *pTouch,Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        if (nullptr != m_pHead)
        {
            pos = m_pHead->convertToNodeSpace(pos);
            Rect rect = Rect(0,0,m_pHead->getContentSize().width,m_pHead->getContentSize().height);
            m_beginInTouch = rect.containsPoint(pos);
            return this->isVisible();
        }
        return false;
    };
    m_listener->onTouchEnded = [this](Touch *pTouch, Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        if (nullptr != m_pHead)
        {
            pos = m_pHead->convertToNodeSpace(pos);
            Rect rect = Rect(0,0,m_pHead->getContentSize().width,m_pHead->getContentSize().height);
            if (m_beginInTouch && rect.containsPoint(pos))
            {
                if (m_notify)
                {
                    m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::ROLE_INFO, &m_dwUserId, sizeof(DWORD));
                }
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void GameRoleNode::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    Node::onExit();
}

void GameRoleNode::reSet(bool bShowSelf /*= true*/)
{
    this->setVisible(!bShowSelf);
    this->switchToOpenCard(false);
    m_spChatBg->setVisible(false);
    m_spHeadBg->setVisible(false);
    m_spHeadFrame->setVisible(true);
    if (nullptr != m_pHead)
    {
        m_pHead->setVisible(false);
    }
    if (nullptr != m_spBanker)
    {
        m_spBanker->setVisible(false);
    }
    
    if (m_bConvertHead && nullptr != m_pHead)
    {
        CUtilEx::convertToNormalSprite(m_pHead);
    }
    
    if (m_bConvertBanker && nullptr != m_spBanker)
    {
        CUtilEx::convertToNormalSprite(m_spBanker);
    }
}

void GameRoleNode::updateRoleInfo(const DWORD &nRoleId,const char *szName,  SCORE lCoin, bool bReEnter,bool bLeave /*= false*/)
{
    m_dwUserId = nRoleId;
    //更新头像文件
    UserData *pUser = HallDataMgr::getInstance()->getUserData(nRoleId);
    if (nullptr == pUser)
        return;
    
    if (nullptr == m_pHead)
    {
        if (bReEnter)
        {
            m_pHead = HeaderRequest::createwithFaceID(0, 0, 0, 0);
        }
        else
        {
            m_pHead = HeaderRequest::createwithFaceID(pUser->m_date.wFaceID,
                                                      pUser->m_date.dwCustomID,
                                                      pUser->m_date.dwUserID,
                                                      pUser->m_date.cbGender);
        }
        m_root->addChild(m_pHead);
    }
    else
    {
        if (!bReEnter)
        {
            if (0 != pUser->m_date.dwCustomID)
            {
                auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(nRoleId);
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
    }
    m_pHead->setHeadSize(70.0f);
    m_pHead->setVisible(!bReEnter);
    m_pHead->setPosition(0.0f,0.0f);
    SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("game_head_bg.png");
    if (nullptr != frame)
    {
        m_spHeadFrame->setSpriteFrame(frame);
    }
    m_spHeadFrame->setVisible(!bReEnter);
    
    //更新名字
    m_clipNickName->setString(szName);
    
    char buf[64] = "";
    //更新元宝
    sprintf(buf,"%lld",lCoin);
    std::string tmpBuf = buf;
    if (tmpBuf.length() > 12)
    {
        tmpBuf = tmpBuf.substr(0, 10);
    }
    m_textCoin->setString(buf);
    
    this->setVisible(!bLeave);
}

void GameRoleNode::updateRoleState(const BYTE &cbState)
{
    Sprite *tmpSp = nullptr;
    if (nullptr != m_spBanker && m_spBanker->isVisible())
    {
        m_bConvertBanker = cbState == US_OFFLINE;
        tmpSp = m_spBanker;
    }
    else if (nullptr != m_pHead && m_pHead->isVisible())
    {
        m_bConvertHead = cbState == US_OFFLINE;
        tmpSp = m_pHead;
    }
    
    if (nullptr == tmpSp)
    {
        return;
    }
    
    if (US_OFFLINE == cbState)
    {
        CUtilEx::convertToGraySprite(tmpSp);
    }
    else
    {
        CUtilEx::convertToNormalSprite(tmpSp);
    }
}

void GameRoleNode::switchToBanker(bool bBanker,const enGameRoleDir &dir)
{
    if (kMY_ROLE != dir)
    {
        m_spHeadBg->setVisible(true);
    }
    else
    {
        m_spHeadBg->setVisible(false);
    }
    
    SpriteFrame *frame = INSTANCE(SpriteFrameCache)->getSpriteFrameByName("blank.png");
    if (nullptr != frame)
    {
        m_spHeadFrame->setSpriteFrame(frame);
    }
    if (nullptr == m_spBanker)
    {
        m_spBanker = Sprite::create();
        m_root->addChild(m_spBanker);
    }
    if (nullptr != m_pHead)
    {
        m_pHead->setVisible(false);
    }
    m_spBanker->setVisible(true);
    
    int role = 1;
    if (bBanker)
    {
        role = 1;
    }
    else
    {
        role = 2;
    }
    char buf[64] = "";
    sprintf(buf, "game_role%d%d.png",(int)dir,role);
    frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
    if (nullptr != frame)
    {
        m_spBanker->setSpriteFrame(frame);
    }
    
    //调整z轴
}

void GameRoleNode::switchToOpenCard(bool bOpenCard,const enGameRoleDir &dir /*= kMY_ROLE*/)
{
    /*
     if (kMY_ROLE == dir)
     {
     m_spOpenCard->setPosition(Vec2(105, -50));
     }
     else
     {
     m_spOpenCard->setPosition(Vec2(0, -110));
     }
     m_spOpenCard->setVisible(bOpenCard);
     */
}

void GameRoleNode::textChat(const enGameRoleDir &dir, const char *str)
{
    m_spBrow->setVisible(false);
    
    //调整位置
    changeChatPos(dir);
    
    m_labChat->setString(str);
    m_labChat->setVisible(true);
    
    //调整九宫格尺寸
    Size labSize = m_labChat->getContentSize();
    if (labSize.height >= 45)
    {
        m_spChatBg->setContentSize(Size(198, labSize.height + 15));
    }
    else
    {
        m_spChatBg->setContentSize(Size(labSize.width + 35, 45));
    }
    m_labChat->setPosition(m_spChatBg->getContentSize().width * 0.5, m_spChatBg->getContentSize().height * 0.55);
}

void GameRoleNode::browChat(const enGameRoleDir &dir, const int &idx)
{
    m_labChat->setVisible(false);
    m_spChatBg->setContentSize(Size(168, 45));
    
    //调整位置
    changeChatPos(dir);
    char buf[12] = "";
    
    //索引加一
    int tmp = idx;
    if (10 > tmp)
    {
        sprintf(buf, "e0%d.png",tmp);
    }
    else
    {
        sprintf(buf, "e%d.png",tmp);
    }
    
    SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
    if (nullptr == pFrame)
    {
        return;
    }
    m_spBrow->setSpriteFrame(pFrame);
    
    m_spBrow->setVisible(true);
}

void GameRoleNode::changeChatPos(const enGameRoleDir &dir)
{
    if (kRIGHT_ROLE == dir)
    {
        m_spChatBg->setAnchorPoint(Vec2(1.0f,0.5f));
        m_spChatBg->loadTexture("game_chat_1.9.png",Widget::TextureResType::PLIST);
        m_spChatBg->setPosition(Vec2(-93,50));
    }
    else
    {
        m_spChatBg->setAnchorPoint(Vec2(0.0f,0.5f));
        m_spChatBg->loadTexture("game_chat_0.9.png",Widget::TextureResType::PLIST);
        m_spChatBg->setPosition(Vec2(93,50));
    }
    m_spChatBg->stopAction(m_actTip);
    m_spChatBg->runAction(m_actTip);
    
    this->schedule(SEL_SCHEDULE(&GameRoleNode::chatTipUpdater), 2.0f);
}

std::string GameRoleNode::getRoleName()
{
    return m_clipNickName->getString();
}

void GameRoleNode::chatTipUpdater(float dt)
{
    this->unschedule(SEL_SCHEDULE(&GameRoleNode::chatTipUpdater));
    m_spChatBg->setVisible(false);
}