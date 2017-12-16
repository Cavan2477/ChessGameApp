
#include "zjh_DeskCell.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "Project.h"
#include "zjh_DeskLayer.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
USING_ZJH_NAMESPACE;
enum IMTag
{
    Tag_IM_Lock = 0,// 锁
    Tag_IM_Playing = 1,// 锁
    Tag_BT_Seat,
    tag_Bt_Ready,//准备手掌
    Tag_User = 10,//
};

DeskCell * DeskCell::create(const std::string &imagefile, int index)
{
    auto  widget = new (std::nothrow) DeskCell;
    if (widget && widget->init(imagefile, TextureResType::LOCAL)) {
        widget->initwithIndex(index);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void DeskCell::initwithIndex(int index)
{
    auto number = Text::create(__String::createWithFormat("%d",index+1)->getCString(),FontBold,20);
    number->setColor(Color3B(251,252,2));
    number->setPosition(Vec2(50, this->getContentSize().height/2));
    this->addChild(number);
    m_index = index;
    
    auto status = HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[index];
    //if (status.cbTableLock)
    {
        this->updateStatus(status.cbTableLock,status.cbPlayStatus);
    }
    
    for (int seatid=0; seatid<HallDataMgr::getInstance()->m_ChairCount; ++seatid) {
        auto seat = Button::create("plazz/desk_noBody.png");
        seat->setPressedActionEnabled(false);
        seat->addTouchEventListener(CC_CALLBACK_2(DeskCell::touchEvent, this));
        seat->setPosition(Vec2(116+110*seatid, this->getContentSize().height/2+10));
        seat->setTag(Tag_BT_Seat+seatid);
        this->addChild(seat);
    }
}

void DeskCell::updateStatus(BYTE bLockStatus,BYTE bPlayStatus)
{
    this->removeChildByTag(Tag_IM_Lock);
    this->removeChildByTag(Tag_IM_Playing);
    
    if (bLockStatus) {
        auto lock = ImageView::create("room_res/im_lock.png");
        lock->setPosition(Vec2(60, this->getContentSize().height/2-40));
        lock->setTag(Tag_IM_Lock);
        this->addChild(lock);
    }
    
    if (bPlayStatus) {
        auto playImg = ImageView::create("room_res/desk_playing.png");
        playImg->setPosition(Vec2(this->getContentSize()/2));
        playImg->setTag(Tag_IM_Playing);
        this->addChild(playImg,3);
    }
}

void DeskCell::userEnter(UserData *pdata, WORD chairID)
{
    auto pbutton = (Button *)this->getChildByTag(Tag_BT_Seat+chairID);
    if (pbutton) {
        pbutton->setTouchEnabled(false);
    }
    
    int tag = Tag_User+chairID;
    this->removeChildByTag(Tag_User+chairID);
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(90, 112));
    playout->setAnchorPoint(Vec2(0.5f, 0.5f));
    playout->setPosition(Vec2(116+110*chairID, this->getContentSize().height/2));
    playout->setTag(tag);
    this->addChild(playout);
    
    Sprite* pSenti = Sprite::create("studioUi/desk/desk_seat.png");
    if (nullptr != pSenti)
    {
        ClippingNode *pClip = ClippingNode::create(pSenti);
        pClip->setAlphaThreshold(0);
        pClip->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-44));
        playout->addChild(pClip);
        
        auto head = HeaderRequest::createwithFaceID(pdata->m_date.wFaceID,
                                                    pdata->m_date.dwCustomID,
                                                    pdata->m_date.dwUserID,
                                                    pdata->m_date.cbGender);
        head->setHeadSize(90);
        pClip->addChild(head);
    }
    
    auto pname = Label::createWithSystemFont(pdata->m_nickname, FontNormal, 18);
    pname->setColor(Color3B::GREEN);
    pname->setHorizontalAlignment(TextHAlignment::CENTER);
    pname->setDimensions(82, 23);
    pname->setPosition(Vec2(playout->getContentSize().width/2, 15));
    playout->addChild(pname);
    
    playout->removeChildByTag(tag_Bt_Ready);
    
    if(pdata->m_date.cbUserStatus == US_READY)
    {
        auto desk_ready = ImageView::create("room_res/desk_ready.png");
        desk_ready->setPosition(Vec2(desk_ready->getContentSize().width/2+1,playout->getContentSize().height/2));
        desk_ready->setTag(tag_Bt_Ready);
        playout->addChild(desk_ready,1);
    }
}

void DeskCell::userStatus(UserData *pdata, WORD chairID)
{
    int tag = Tag_User+chairID;
    auto playout = (Layout*)this->getChildByTag(tag);
    if(!playout)
        return;
    playout->removeChildByTag(tag_Bt_Ready);
    
    if(pdata->m_date.cbUserStatus == US_READY)
    {
        auto desk_ready = ImageView::create("room_res/desk_ready.png");
        desk_ready->setPosition(Vec2(desk_ready->getContentSize().width/2+1,playout->getContentSize().height/2));
        desk_ready->setTag(tag_Bt_Ready);
        playout->addChild(desk_ready,1);
    }
}

void DeskCell::userLeave(WORD chairID)
{
    this->removeChildByTag(Tag_User+chairID);
    
    auto pbutton = (Button *)this->getChildByTag(Tag_BT_Seat+chairID);
    if (pbutton) {
        pbutton->setTouchEnabled(true);
    }
}

void DeskCell::clearUser()
{
    for (int index=0; index<HallDataMgr::getInstance()->m_ChairCount; ++index) {
        this->removeChildByTag(Tag_User+index);
        auto pbutton = (Button *)this->getChildByTag(Tag_BT_Seat+index);
        pbutton->setTouchEnabled(true);
    }
}

void DeskCell::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            if (!HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[m_index].cbTableLock) {
                HallDataMgr::getInstance()->AddpopLayer("", "正在进入游戏。。。", Type_Wait_Text);
                NetworkMgr::getInstance()->sendSitDown(m_index, tag-Tag_BT_Seat, "");
            }
            else
            {
                auto pdesk = Director::getInstance()->getRunningScene();
                auto viewsize = WinSize;
                auto pbg = Button::create();
                pbg->setScale9Enabled(true);
                pbg->setContentSize(viewsize);
                pbg->setPosition(viewsize/2);
                pdesk->addChild(pbg,1);
                
                auto ppop = ImageView::create("public_res/pop_frame.png");
                ppop->setScale9Enabled(true);
                ppop->setContentSize(cocos2d::Size(500,300));
                ppop->setPosition(viewsize/2);
                pbg->addChild(ppop);
                
                auto pclose = Button::create("public_res/bt_close_0.png");
                pclose->setScale(0.95f);
                pclose->setPosition(Vec2(ppop->getContentSize().width-31, ppop->getContentSize().height-31));
                pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                              {
                                                  if (type == Widget::TouchEventType::ENDED) {
                                                      pbg->removeFromParent();
                                                  }
                                              });
                ppop->addChild(pclose);
                
                auto ptext = Text::create("该桌子已设置进入密码，请输入密码", FontBold, 22);
                ptext->setPosition(Vec2(70, ppop->getContentSize().height-80));
                ptext->setAnchorPoint(Vec2(0.f, 0.5f));
                ppop->addChild(ptext);
                
                auto ptitle = Text::create("桌子密码：", FontBold, 22);
                ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
                ptitle->setPosition(Vec2(70, ppop->getContentSize().height-140));
                ppop->addChild(ptitle);
                
                auto pedit = EditBox::create(cocos2d::Size(250,59), "public_res/desk_pwd_text.png");
                pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
                pedit->setPosition(Vec2(ptitle->getPositionX()+100, ptitle->getPositionY()));
                pedit->setInputFlag(EditBox::InputFlag::PASSWORD);
                pedit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                pedit->setFontSize(28);
                ppop->addChild(pedit);
                
                auto pensure = Button::create("public_res/desk_sure.png");
                pensure->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                               {
                                                   if (type == Widget::TouchEventType::ENDED) {
                                                       std::string passstr = pedit->getText();
                                                       if (!passstr.size()) {
                                                           HallDataMgr::getInstance()->AddpopLayer("提示", "密码为空，请输入密码", Type_Ensure);
                                                           return;
                                                       }
                                                       if (strLength(passstr) > LEN_PASSWORD - 1) {
                                                           HallDataMgr::getInstance()->AddpopLayer("提示", "密码过长，请重新输入", Type_Ensure);
                                                           return;
                                                       }
                                                       if (passstr.find(" ") != std::string::npos) {
                                                           HallDataMgr::getInstance()->AddpopLayer("提示", "密码中不能有空格，请重新输入", Type_Ensure);
                                                           return;
                                                       }
                                                       pbg->removeFromParent();
                                                       HallDataMgr::getInstance()->AddpopLayer("", "正在进入游戏。。。", Type_Wait_Text);
                                                       NetworkMgr::getInstance()->sendSitDown(m_index, tag-Tag_BT_Seat, passstr);
                                                   }
                                               });
                pensure->setPosition(Point(ppop->getContentSize().width/2, ppop->getContentSize().height - 225));
                ppop->addChild(pensure);
                
            }
        }
            break;
            
        default:
            break;
    }
}