//
//  CannonLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/1.
//
//炮台层

#include "CannonLayer.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "lkpy_GameDataMgr.h"
#include "HallDataMgr.h"
#include "GameLayer.h"
#include "Help.h"
USING_NS_CC;
using namespace ui;
USING_LKPY_NAMESPACE;
enum ButtonTag
{
    Tag_BT_Chat_OnOff,
    Tag_Bt_Send_Chat
};

#define CHAT_MOVE_X 40

const Vec2 cannonpos[6] = {Vec2(-370,35), Vec2(0,35), Vec2(370,35), Vec2(-370,81), Vec2(0,81), Vec2(370,81)};


CannonLayer::CannonLayer():m_bIsChatBoxMoving(false)
,m_bIsChatOnOff(false)
{
    
}

CannonLayer::~CannonLayer()
{
    
}

bool CannonLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto viewsize = WinSize;
//    Sprite* edgeSpace = Sprite::create();
//    PhysicsBody* boundBody = PhysicsBody::createEdgeBox(viewsize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
//    boundBody->getShape(0)->setFriction(0.0f);
//    boundBody->getShape(0)->setRestitution(1.0f);
//    edgeSpace->setPhysicsBody(boundBody);
//    edgeSpace->setPosition(Point(viewsize.width / 2, viewsize.height / 2));
//    this->addChild(edgeSpace);
    
//    auto pbullet = Sprite::create("game_res/im_bullet.png");
//    pbullet->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
//    this->addChild(pbullet);
//
//    auto pbullet1 = Sprite::create("game_res/im_bullet.png");
//    pbullet1->setPosition(Vec2(viewsize.width/2, 0));
//    auto pbody1 = GameDataMgr::getInstance()->getBodyByName("im_bullet");
//    pbody1->getShape(0)->setRestitution(1.0f);
//    pbody1->getShape(0)->setFriction(0.0f);
//    pbody1->getShape(0)->setDensity(1.0f);
//    pbody1->setGravityEnable(false);
//    pbody1->setVelocity(Vec2(0,100));
//    pbullet1->setRotation(90-CC_RADIANS_TO_DEGREES(Vec2(0.5,1).getAngle()));
//    pbullet1->setPhysicsBody(pbody1);
//    pbody1->setCategoryBitmask(3);
//    pbody1->setContactTestBitmask(1|2);
//    //pbody1->setCollisionBitmask(0);
//    this->addChild(pbullet1);
    
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    auto pcannon = Cannon::create();
    pcannon->initwithUser(puser);
    auto pos = cannonpos[pcannon->getPos()];
    pcannon->setPosition(Vec2(viewsize.width/2+pos.x*JudgeScale, pos.y));
    this->addChild(pcannon,2);
    m_cannonlist.insert(puser->m_date.dwUserID, pcannon);
    
    m_trumpet = Button::create("game_res/trumpet_0.png", "game_res/trumpet_1.png");
    m_trumpet->setPosition(Vec2(70, 225));
    m_trumpet->setTag(Tag_BT_Chat_OnOff);
    m_trumpet->addTouchEventListener(CC_CALLBACK_2(CannonLayer::touchEvent, this));
    this->addChild(m_trumpet,10);
    
    m_bt_send = Button::create("game_res/bt_send_0.png", "game_res/bt_send_1.png");
    
    m_bt_send->setPosition(Vec2(450, 225));
    m_bt_send->setTag(Tag_Bt_Send_Chat);
    m_bt_send->addTouchEventListener(CC_CALLBACK_2(CannonLayer::touchEvent, this));
    //this->addChild(m_bt_send,11);
    

    Sprite * clipImg = Sprite::create("game_res/imputfield.png");
    
    inputField = EditBox::create(clipImg->getContentSize(), "game_res/imputfield.png");
    inputField->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    //inputField->setPosition(cocos2d::Point(130,225));
    //m_LoginEdit[0]->setMaxLength(LEN_ACCOUNTS);
    inputField->setMaxLength(LEN_ACCOUNTS);
    inputField->setFontSize(16);
    inputField->setPlaceHolder("");
    inputField->setDelegate(this);
    inputField->setEnabled(false);
    //this->addChild(inputField,10);
    
    Size btSendSize = m_bt_send->getContentSize();
    Size imgSize = clipImg->getContentSize();
    
    //剪切域
    DrawNode* shap = CCDrawNode::create();
    Point point[4] = {ccp(0,0), ccp(imgSize.width+btSendSize.width+50, 0), ccp(imgSize.width+btSendSize.width+50, btSendSize.height), ccp(0, btSendSize.height)};
    shap->drawPolygon(point, 4, ccc4f(0, 0, 0, 255), 2, ccc4f(255, 255, 255,255));
    
    ClippingNode *m_pClipNode=CCClippingNode::create();
    m_pClipNode->setPosition(cocos2d::Point(115,208));
    m_pClipNode->setInverted(false);
    //m_pClipNode->setContentSize(Size(imgSize.width+m_bt_send->getContentSize().width,m_bt_send->getContentSize().height));
    m_pClipNode->addChild(inputField);
    m_pClipNode->addChild(m_bt_send);
    m_pClipNode->setStencil(shap);
    this->addChild(m_pClipNode,10);
    
    inputField->setPosition(Vec2(m_pClipNode->getContentSize().width+120,m_pClipNode->getContentSize().height/2+18));
    m_bt_send->setPosition(Vec2(m_pClipNode->getContentSize().width+255,m_pClipNode->getContentSize().height/2+18));
    m_bt_send->setEnabled(false);
    float moveDir = inputField->getContentSize().width+m_bt_send->getContentSize().width+CHAT_MOVE_X;
    inputField->setPositionX(inputField->getPositionX()-moveDir);
    m_bt_send->setPositionX(m_bt_send->getPositionX()-moveDir);
    return true;
}

void CannonLayer::onEnter()
{
    Layer::onEnter();
    
    NetworkMgr::getInstance()->m_Userdelegate = this;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = CC_CALLBACK_2(CannonLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CannonLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CannonLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CannonLayer::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void CannonLayer::onExit()
{
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    m_cannonlist.clear();
    Layer::onExit();
}

bool CannonLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //log("%f__%f",touch->getPreviousLocation().x, touch->getPreviousLocation().y);
    //log("%f__%f",touch->getLocation().x, touch->getLocation().y);
    GameLayer* pLayer = (GameLayer*)this->getParent();
    pLayer->setSecondCount(60);
    auto pcannon = m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        auto vec2 = touch->getLocation();
        pcannon->shoot(vec2, true);
    }
    auto pgamelayer = static_cast<GameLayer *>(this->getParent());
    if (pgamelayer) {
        pgamelayer->cannonlayerCallback();
    }
    return true;
}

void CannonLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    GameLayer* pLayer = (GameLayer*)this->getParent();
    pLayer->setSecondCount(60);
    auto pcannon = m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        auto vec2 = touch->getLocation();
        pcannon->shoot(vec2, true);
    }
}

void CannonLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    GameLayer* pLayer = (GameLayer*)this->getParent();
    pLayer->setSecondCount(60);
    auto pcannon = m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        auto vec2 = touch->getLocation();
        pcannon->shoot(vec2, false);
    }
}

void CannonLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_Bt_Send_Chat:
                {
                    
                    CMD_GF_C_UserChat cmd_gf_cuc;
                    std::string inputString(inputField->getText());
                    if(inputString.empty())
                    {
                        
                        return;
                    }
                    cmd_gf_cuc.wChatLength = inputString.length();
                    cmd_gf_cuc.dwChatColor = 0;
                    cmd_gf_cuc.dwTargerUserID = INVALID_USERID;
                    memset(cmd_gf_cuc.szChatString, 0, sizeof(cmd_gf_cuc.szChatString));
                    UTF8Str_To_UTF16Str_BYTE((BYTE *)inputString.c_str(), cmd_gf_cuc.szChatString);
                    //WHConverUft8ToUnicodeWithArray(cmd_gf_cuc.szChatString,inputString.c_str());
                    TCHAR ChatStr[LEN_USER_CHAT];
                    memset(ChatStr, 0, sizeof(ChatStr));
                    memcpy(ChatStr,(char *)cmd_gf_cuc.szChatString,cmd_gf_cuc.wChatLength);
                    string chatString = WHConverUnicodeToUtf8WithArray(ChatStr);
                    //NetworkMgr::getInstance()->sendChat(cmd_gf_cuc);
                    int nSize = sizeof(cmd_gf_cuc)-sizeof(cmd_gf_cuc.szChatString)+cmd_gf_cuc.wChatLength*2;
                    NetworkMgr::getInstance()->sendData(MDM_GF_FRAME, SUB_GF_USER_CHAT, &cmd_gf_cuc, nSize);
                    inputField->setText("");
                }
                    break;
                case Tag_BT_Chat_OnOff:
                {
                    if(!m_bIsChatBoxMoving)
                    {
                        //Size btSendSize = m_bt_send->getContentSize();
                        //Size imgSize = clipImg->getContentSize();
                        
                        float moveDir = inputField->getContentSize().width+m_bt_send->getContentSize().width+CHAT_MOVE_X;
                        float moveTime = 0.6f;
                        
                        m_bIsChatBoxMoving = true;
                        if(m_bIsChatOnOff)
                        {
                            inputField->setFocused(false);
                            inputField->setFocusEnabled(false);
                            inputField->setEnabled(false);
                            m_bt_send->setEnabled(false);
                            inputField->detachWithIME();
                            inputField->setText("");
                            inputField->runAction(MoveBy::create(moveTime,Vec2(-moveDir,0)));
                            m_bt_send->runAction(MoveBy::create(moveTime,Vec2(-moveDir,0)));
                            m_bIsChatOnOff = false;
                            this->runAction(Sequence::createWithTwoActions(DelayTime::create(moveTime)
                                                                           , CallFunc::create([=]{m_bIsChatBoxMoving = false;})));
                        }
                        else
                        {
                            m_bt_send->setEnabled(true);
                            inputField->setEnabled(true);
                            inputField->setFocused(true);
                            inputField->setFocusEnabled(true);
                            inputField->runAction(MoveBy::create(moveTime,Vec2(moveDir,0)));
                            m_bt_send->runAction(MoveBy::create(moveTime,Vec2(moveDir,0)));
                            m_bIsChatOnOff = true;
                            this->runAction(Sequence::createWithTwoActions(DelayTime::create(moveTime)
                                                                           , CallFunc::create([=]{m_bIsChatBoxMoving = false;})));
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}


void CannonLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void CannonLayer::OnUserEnter(void *pUser)
{
    auto user = (UserData *)pUser;
    auto viewsize = WinSize;
    auto pcannon = m_cannonlist.at(user->m_date.dwUserID);
    if (lkpy_game::GameDataMgr::getInstance()->m_sceneData.lPlayCurScore[user->m_date.wChairID] != 0) {
        user->m_date.lScore = GameDataMgr::getInstance()->m_sceneData.lPlayCurScore[user->m_date.wChairID];
    }
    if (pcannon!=nullptr ) {
        pcannon->setScore(user->m_date.lScore);
    }
    else
    {
        WORD myTableId = HallDataMgr::getInstance()->m_wTableID;
        if(user->m_date.wChairID!=INVALID_CHAIR&&user->m_date.wTableID!=INVALID_TABLE&&
           user->m_date.wTableID == myTableId)
        {
            auto pcannonold = this->getCannonbypos(user->m_date.wChairID);
            if (pcannonold) {
                m_cannonlist.erase(pcannonold->getUserID());
                pcannonold->removeFromParent();
            }
            
            
            auto pcannon = Cannon::create();
            pcannon->initwithUser(user);
            auto pos = cannonpos[pcannon->getPos()];
            pcannon->setPosition(Vec2(viewsize.width/2+pos.x*JudgeScale, pos.y));
            if (pcannon->getPos() < 3) {
                pcannon->setPositionY(viewsize.height - pos.y);
            }
            this->addChild(pcannon,2);
            m_cannonlist.insert(user->m_date.dwUserID, pcannon);
        }
  
        }
    
    log("%s","用户进入");
}

void CannonLayer::OnUserStatus(void *pUser)
{
    auto user = (UserData *)pUser;
    auto viewsize = WinSize;
    log("%s _%d","用户状态", user->m_date.cbUserStatus);
    if (user->m_date.cbUserStatus == US_FREE) {
        auto pcannon = m_cannonlist.at(user->m_date.dwUserID);
        if (pcannon) {
            auto nchairID = pcannon->getPos();
            if (GameDataMgr::getInstance()->m_reversal) {
                nchairID = 5-nchairID;
            }
            GameDataMgr::getInstance()->m_sceneData.lPlayCurScore[nchairID] = 0;
            
            pcannon->removeFromParent();
            m_cannonlist.erase(user->m_date.dwUserID);
        }
    }
    
    if (user->m_date.cbUserStatus > US_FREE) {
        auto pcannon = m_cannonlist.at(user->m_date.dwUserID);
        if (pcannon!=nullptr ) {
            pcannon->setScore(user->m_date.lScore);
        }
        else
        {
            auto pcannonold = this->getCannonbypos(user->m_date.wChairID);
//            if (pcannonold) {
//                m_cannonlist.erase(pcannonold->getUserID());
//                pcannonold->removeFromParent();
//            }
            if(!pcannonold)
            {
                auto pcannon = Cannon::create();
                pcannon->initwithUser(user);
                auto pos = cannonpos[pcannon->getPos()];
                pcannon->setPosition(Vec2(viewsize.width/2+pos.x*JudgeScale, pos.y));
                if (pcannon->getPos() < 3) {
                    pcannon->setPositionY(viewsize.height - pos.y);
                }
                this->addChild(pcannon,2);
                m_cannonlist.insert(user->m_date.dwUserID, pcannon);
            }
           
        }
    }
}

Cannon *CannonLayer::getCannonbypos(DWORD chair)
{
    int pos = GameDataMgr::getInstance()->m_reversal?5-chair:chair;
    std::vector<DWORD> list = m_cannonlist.keys();
    for (auto iter : list) {
        auto pcannon = m_cannonlist.at(iter);
        if (pcannon->getPos() == pos) {
            return pcannon;
        }
    }
    return nullptr;
}

void CannonLayer::OnUserChat(WORD wSubCode, void *pData, WORD wSize)
{
    switch (wSubCode)
    {
        case SUB_GF_USER_CHAT:
        {
            this->messageChat(pData,wSize);
        }
            break;
        case SUB_GF_USER_EXPRESSION:
        {
            
        }
            break;
        default:
            break;
    }
}

void CannonLayer::messageChat(void *pData, WORD wSize)
{
    CMD_GF_S_UserChat* cmd_gf_cuc = (CMD_GF_S_UserChat*) pData;
    int chatLength = cmd_gf_cuc->wChatLength;
    Cannon* pCannon = this->m_cannonlist.at(cmd_gf_cuc->dwSendUserID);
    if(pCannon)
    {
        TCHAR ChatStr[LEN_USER_CHAT];
        memset(ChatStr, 0, sizeof(ChatStr));
        memcpy(ChatStr,(char *)cmd_gf_cuc->szChatString,chatLength);
        string chatString = WHConverUnicodeToUtf8WithArray(ChatStr);
        pCannon->setChatString(chatString);
    }
}
