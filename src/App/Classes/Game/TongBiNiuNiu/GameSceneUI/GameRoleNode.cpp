#include "GameRoleNode.h"
#include "../../CustomUtilsSrc/AnimationMgr/AnimationMgr.h"

USING_NS_CC;
using namespace ui;
USING_TBNN_NAMESPACE;

GameRoleNode::GameRoleNode():
m_pHead(nullptr),
m_notify(nullptr),
m_clipNickName(nullptr)
{
    auto scaleTo = ScaleTo::create(0.1f,1.0f);
    auto show = Show::create();
    m_actTip = Spawn::create(scaleTo,show,nullptr);
    m_actTip->retain();
}

GameRoleNode::~GameRoleNode()
{
    CC_SAFE_RELEASE(m_actTip);
    
    CC_SAFE_RELEASE(m_actScore);
}

bool GameRoleNode::init()
{
	bool bRes = false;
	do 
	{
		CC_BREAK_IF(!Node::init());
		LOAD_CSB("game/GameRoleItem.csb");

		m_spHeadBg = static_cast<Sprite*>(m_root->getChildByName("head_bg"));
		CC_ASSERT(m_spHeadBg != nullptr);
        /*
		m_textName = static_cast<Text*>(m_root->getChildByName("name_text"));
		CC_ASSERT(m_textName != nullptr);*/
        m_clipNickName = ClipText::createClipText(Size(100, 20), "", "Arial",14);
        CC_ASSERT(m_clipNickName != nullptr);
        m_clipNickName->setTextColor(Color4B::YELLOW);
        m_clipNickName->setTextAnchorPoint(Vec2(0, 0));
        m_clipNickName->setPosition(Vec2(21, 40));
        m_spHeadBg->addChild(m_clipNickName);

		m_textCoin = static_cast<Text*>(m_spHeadBg->getChildByName("coin_text"));
		CC_ASSERT(m_textCoin != nullptr);

		m_spBanker = static_cast<Sprite*>(m_root->getChildByName("banker"));
        CC_ASSERT( m_spBanker != nullptr);
        
        m_spOpenCard = static_cast<Sprite*>(m_root->getChildByName("opencard"));
        CC_ASSERT( m_spOpenCard != nullptr);
        
        m_spChatBg = static_cast<ImageView*>(m_root->getChildByName("chat_bg"));
        CC_ASSERT(m_spChatBg != nullptr);
        
        m_spBrow = static_cast<Sprite*>(m_spChatBg->getChildByName("chat_face"));
        CC_ASSERT(m_spBrow != nullptr);
        
        m_labChat = Label::createWithSystemFont("", "Arial", 20,Size(150,0));
        m_labChat->setPosition(m_spChatBg->getContentSize().width * 0.5, m_spChatBg->getContentSize().height * 0.6);
        m_labChat->setVerticalAlignment(TextVAlignment::CENTER);
        m_spChatBg->addChild(m_labChat);
        
        m_atlasScore = static_cast<TextAtlas*>(m_root->getChildByName("score_atlas"));
        CC_ASSERT(nullptr != m_atlasScore);
        m_atlasScore->setString("");        
        //m_atlasScore->setScale(2.0f);
        m_atlasScore->setLocalZOrder(1);
        
        auto fadein = FadeIn::create(0.2f);
        auto moveUp = MoveBy::create(0.5f, Vec2(0, 20.0f));
        auto fadeOut = FadeOut::create(0.2f);
        m_actScore = Sequence::create(Spawn::create(fadein,moveUp, NULL), DelayTime::create(2.0f),fadeOut, NULL);
        m_actScore->retain();
        
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
        pos = m_spHeadBg->convertToNodeSpace(pos);
        Rect rect = Rect(0,0,m_spHeadBg->getContentSize().width,m_spHeadBg->getContentSize().height);
        m_beginInTouch = rect.containsPoint(pos);
        return this->isVisible();
    };
    m_listener->onTouchEnded = [this](Touch *pTouch, Event *event)
    {
        Vec2 pos = pTouch->getLocation();
        pos = m_spHeadBg->convertToNodeSpace(pos);
        Rect rect = Rect(0,0,m_spHeadBg->getContentSize().width,m_spHeadBg->getContentSize().height);
        if (m_beginInTouch && rect.containsPoint(pos))
        {
            if (m_notify)
            {
                m_notify->uiMsg(UI_MSG_BASE::UI_MSG_GAME, GAME_UI::ROLE_INFO, &m_dwUserId, sizeof(DWORD));
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
    this->setVisible(bShowSelf);
    this->switchToBanker(false);
    this->switchToOpenCard(false);
    m_spChatBg->setVisible(false);
    
    m_atlasScore->setPositionY(-35);
    m_atlasScore->setOpacity(0);
    m_atlasScore->setString("");
}

void GameRoleNode::updateRoleInfo(const DWORD &nRoleId,const char *szName, const SCORE &lCoin,bool bLeave /*= false*/)
{
    m_dwUserId = nRoleId;
	//更新头像文件
    UserData *pUser = HallDataMgr::getInstance()->m_UserList.at(nRoleId);
    if (nullptr == m_pHead)
    {
        m_pHead = HeaderRequest::createwithFaceID(
                                                  pUser->m_date.wFaceID,
                                                  pUser->m_date.dwCustomID,
                                                  pUser->m_date.dwUserID,
                                                  pUser->m_date.cbGender);
        m_pHead->setPosition(0.0f,30.0f);
        m_root->addChild(m_pHead);
    }
    else
    {
        /*
        auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(nRoleId);
        if (ptexture)
        {
            m_pHead->setTexture(ptexture);
            m_pHead->setTextureRect(cocos2d::Rect(0, 0, FACE_CX, FACE_CY));
        }
         */
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
    m_pHead->setHeadSize(89);
    /*
    if (pUser->m_date.cbUserStatus == US_FREE || pUser->m_date.cbUserStatus == US_SIT)
    {
        //更新名字
        m_clipNickName->setString(szName);
    }
     */
    //更新名字
    m_clipNickName->setString(szName);

    char buf[64] = "";
	//更新元宝
	sprintf(buf,"%lld",lCoin);
	m_textCoin->setString(buf);

	this->setVisible(!bLeave);
}

void GameRoleNode::switchToBanker(bool bBanker)
{
	m_spBanker->setVisible(bBanker);
    m_spBanker->setLocalZOrder(1);
}

void GameRoleNode::switchToOpenCard(bool bOpenCard,const enGameRoleDir &dir /*= kMY_ROLE*/)
{
    /*if (kMY_ROLE == dir)
    {
        m_spOpenCard->setPosition(Vec2(105, -50));
    }
    else
    {
        m_spOpenCard->setPosition(Vec2(0, -110));
    }*/
    
    m_spOpenCard->setVisible(bOpenCard);
}

void GameRoleNode::textChat(const enGameChairIndex &dir, const char *str)
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
        m_spChatBg->setContentSize(Size(168, labSize.height + 15));
    }
    else
    {
        m_spChatBg->setContentSize(Size(168, 45));
    }
    m_labChat->setPosition(m_spChatBg->getContentSize().width * 0.5, m_spChatBg->getContentSize().height * 0.55);
}

void GameRoleNode::browChat(const enGameChairIndex &dir, const int &idx)
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

void GameRoleNode::changeChatPos(const enGameChairIndex &dir)
{
    if (tbnn_kGameChair1 == dir
        || tbnn_kGameChair2 == dir
        || tbnn_kGameChair3 == dir)
    {
        m_spChatBg->setAnchorPoint(Vec2(1.0f,0.5f));
        m_spChatBg->loadTexture("game_chat_1.9.png",Widget::TextureResType::PLIST);
        m_spChatBg->setPosition(Vec2(-80,67));
    }
    else
    {
        m_spChatBg->setAnchorPoint(Vec2(0.0f,0.5f));
        m_spChatBg->loadTexture("game_chat_0.9.png",Widget::TextureResType::PLIST);
        m_spChatBg->setPosition(Vec2(80,67));
    }
    m_spChatBg->stopAction(m_actTip);
    m_spChatBg->runAction(m_actTip);
    
    this->schedule(SEL_SCHEDULE(&GameRoleNode::chatTipUpdater), 2.0f);
}

void GameRoleNode::refreshScore(LONGLONG llscore)
{
    std::string buf = formatScore(llscore);
    char szscore[64] = "";
    
    if (llscore > 0)
    {
        sprintf(szscore, "/%lld",llscore);
        buf = "+" + buf;
        m_atlasScore->setProperty(szscore, "game/addnum.png", 19, 23, "/");
    }
    else if (llscore < 0)
    {
        sprintf(szscore, "/%lld",-llscore);
        buf = "-" + buf;
        m_atlasScore->setProperty(szscore, "game/subnum.png", 19, 23, "/");
    }
    m_atlasScore->stopAction(m_actScore);
    m_atlasScore->runAction(m_actScore);
}

void GameRoleNode::showCaiJinAnimation()
{
    auto sp = Sprite::createWithSpriteFrameName("blank.png");
    m_root->addChild(sp);
    sp->setPosition(0, 0);
    tagAnimationParam param = tagAnimationParam();
    param.m_fDelay = 0.1f;
    param.m_strName = tbnn_CAIJIN_ANIMATION;
    auto animate = AnimationMgr::getInstance()->getAnimate(param);
    sp->runAction(animate);
}

void GameRoleNode::chatTipUpdater(float dt)
{
    this->unschedule(SEL_SCHEDULE(&GameRoleNode::chatTipUpdater));
    m_spChatBg->setVisible(false);
}

std::string GameRoleNode::formatScore( LONGLONG llscore)
{
    //处理下注数量
    char buf[64];
    LONGLONG score = llscore > 0 ? llscore : -llscore;
    float fScore = 0.0f;
    if (score / 100000000 > 99)
    {
        fScore = score / 100000000.0f;
        sprintf(buf,"%lld 亿",(LONGLONG)fScore);
    }
    else if (score > 100000000)
    {
        fScore = score / 100000000.0f;
        sprintf(buf,"%.2f 亿",fScore);
    }
    else if (score > 1000000)
    {
        fScore = score / 10000;
        sprintf(buf,"%d 万",(int)fScore);
        
    }
    else if (score > 10000)
    {
        fScore = score / 10000.0f;
        sprintf(buf,"%.2f 万",fScore);
    }
    else
    {
        sprintf(buf,"%lld",score);
    }
    
    return std::string(buf);
}