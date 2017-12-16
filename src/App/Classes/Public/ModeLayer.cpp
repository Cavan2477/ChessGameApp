//
//  ModeLayer.cpp
//  Cheyou
//
//  Created by wh on 14-2-27.
//
//

#include "ModeLayer.h"
#include "Project.h"
#include "HallDataMgr.h"
enum ButtonTag
{
    Tag_Ensure = 0,
    Tag_Cancel,
};

ModeLayer* ModeLayer::createBytype(const std::string &titletext, const std::string &contenttext, ModeType type,bool close /*=false*/)
{
    ModeLayer *player = ModeLayer::create();
    
    if (type == Type_Wait)
    {
        player->WaitInit();
    }
    else if (type == Type_Wait_Text)
    {
        player->WaitTextInit(contenttext,close);
    }
    else if(type == Type_Ensure)
    {
        player->TextInit(titletext, contenttext);
        player->EnsureInit();
    }
    else if(type == Type_Ensure_Cancel)
    {
        player->TextInit(titletext, contenttext);
        player->EnsuercancelInit();
    }
    else if(type == Type_Info_Reminder)
    {
        player->RemindInit(contenttext);
    }
    return  player;
    
}

bool ModeLayer::init()
{
    if ( !Layer::init() )
    {
        return  false;
    }
    m_ensure = NULL;
    m_cancel = NULL;
    m_toubutton = NULL;
    m_bg = NULL;
    EnsureCallback = nullptr;
    CancelCallback = nullptr;
    m_Listener = NULL;
    
    auto pbg = ImageView::create();
    pbg->setTouchEnabled(true);
    pbg->setScale9Enabled(true);
    pbg->setContentSize(WinSize);
    pbg->setPosition(WinSize/2);
    this->addChild(pbg);
    return true;
}

void ModeLayer::TextInit(const std::string &title, const std::string &content)
{
    Size visibleSize = WinSize;
    LayerColor *player = LayerColor::create(Color4B(0, 0, 0, 100), visibleSize.width, visibleSize.height);
    this->addChild(player);
    
    m_bg = ImageView::create();
    m_bg->setScale9Enabled(true);
    m_bg->loadTexture("public_res/pop_frame.png");
    m_bg->setContentSize(Size(541, 287));
    m_bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    m_bg->setOpacity(230);
    this->addChild(m_bg);
    
    Text *ptitle = Text::create(title,FontNormal,32);
    ptitle->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height - 55));
    m_bg->addChild(ptitle);
    
    Label *pcontent = Label::createWithSystemFont(content, FontNormal, 28);
    pcontent->setAlignment(TextHAlignment::CENTER);
    pcontent->setDimensions(465, 0);
    pcontent->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height - 86));
    pcontent->setAnchorPoint(Point(0.5, 1.f));
    m_bg->addChild(pcontent);
    
    m_bg->setScale(0.9f);
    FiniteTimeAction *paction1 = ScaleTo::create(0.15f, 1.05f);
    FiniteTimeAction *paction2 = ScaleTo::create(0.07f, 1.f);
    Action *paction = Sequence::createWithTwoActions(paction1, paction2);
    m_bg->runAction(paction);
    
}

void ModeLayer::WaitInit()
{
    Size visibleSize = WinSize;
    m_type = Type_Wait;
    
    LayerColor *player = LayerColor::create(Color4B(0, 0, 0, 100), visibleSize.width, visibleSize.height);
    this->addChild(player);
    
    ImageView *pimage = ImageView::create();
    pimage->loadTexture("public_res/waiting.png");
    pimage->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(pimage);
    
    Action *paction = RepeatForever::create(RotateBy::create(1.2f, 360));
    pimage->runAction(paction);
}

void ModeLayer::WaitTextInit(const std::string &content,bool close /* = false*/)
{
    Size visibleSize = WinSize;
    m_bg = ImageView::create();
    m_bg->setScale9Enabled(true);
    m_bg->loadTexture("public_res/pop_frame.png");
    m_bg->setContentSize(Size(450, 160));
    m_bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2+50));
    m_bg->setOpacity(230);
    this->addChild(m_bg);
    
    Label *pcontent = Label::createWithSystemFont(content, FontNormal, 28);
    pcontent->setAlignment(TextHAlignment::CENTER);
    pcontent->setDimensions(410, 0);
    //pcontent->setColor(DefaultColor);
    pcontent->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height - 36));
    pcontent->setAnchorPoint(Point(0.5, 1.f));
    m_bg->addChild(pcontent);
    
    if (close)
    {
        m_cancel = Button::create("public_res/bt_close_0.png");
        m_cancel->setPosition(Vec2(m_bg->getContentSize().width-10, m_bg->getContentSize().height-10));
        m_cancel->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                        {
                                            if (type == Widget::TouchEventType::ENDED) {
                                                this->removeFromParent();
                                            }
                                        });
        m_bg->addChild(m_cancel);
    }
    
    ImageView *pimage = ImageView::create();
    pimage->loadTexture("public_res/waiting.png");
    pimage->setPosition(Point(m_bg->getContentSize().width/2, 40));
    m_bg->addChild(pimage);
    
    Action *paction = RepeatForever::create(RotateBy::create(1.2f, 360));
    pimage->runAction(paction);
    
    m_bg->setScale(0.9f);
    FiniteTimeAction *paction1 = ScaleTo::create(0.15f, 1.05f);
    FiniteTimeAction *paction2 = ScaleTo::create(0.07f, 1.f);
    Action *sequeaction = Sequence::createWithTwoActions(paction1, paction2);
    m_bg->runAction(sequeaction);
}

void ModeLayer::EnsureInit()
{
    //Size visibleSize = WinSize;
    m_type = Type_Ensure;
    
    m_ensure = Button::create("public_res/bt_ensure.png");
    m_ensure->setTag(Tag_Ensure);
    m_ensure->addTouchEventListener(CC_CALLBACK_2(ModeLayer::touchEvent, this));
    m_ensure->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height - 230));
    m_bg->addChild(m_ensure);
}

void ModeLayer::EnsuercancelInit()
{
    //Size visibleSize = WinSize;
    m_type = Type_Ensure_Cancel;
    
    m_ensure = Button::create("public_res/bt_ensure.png");
    m_ensure->setTag(Tag_Ensure);
    m_ensure->addTouchEventListener(CC_CALLBACK_2(ModeLayer::touchEvent, this));
    m_ensure->setPosition(Point(-140 + m_bg->getContentSize().width/2, m_bg->getContentSize().height - 230));
    m_bg->addChild(m_ensure);
    
    m_cancel = Button::create("public_res/bt_cancle.png");
    m_cancel->setTag(Tag_Cancel);
    m_cancel->addTouchEventListener(CC_CALLBACK_2(ModeLayer::touchEvent, this));
    m_cancel->setPosition(Point(140+ m_bg->getContentSize().width/2, m_bg->getContentSize().height - 230));
    m_bg->addChild(m_cancel);
}

void ModeLayer::RemindInit(const std::string &content)
{
    m_type = Type_Info_Reminder;
    Size visibleSize = WinSize;
    m_bg = ImageView::create();
    m_bg->setScale9Enabled(true);
    m_bg->loadTexture("public_res/pop_frame.png");
    m_bg->setContentSize(Size(540, 200));
    m_bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    m_bg->setOpacity(230);
    this->addChild(m_bg);
    
    Label *pcontent = Label::createWithSystemFont(content, FontBold, 28);
    pcontent->setAlignment(TextHAlignment::CENTER);
    pcontent->setDimensions(420, 0);
    pcontent->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height/2));
    m_bg->addChild(pcontent);
    if ((pcontent->getContentSize().height+140) > 200) {
        m_bg->setContentSize(cocos2d::Size(540,pcontent->getContentSize().height+140));
    }
    
    Action *paction = Sequence::createWithTwoActions(DelayTime::create(1.2f), CallFunc::create(CC_CALLBACK_0(ModeLayer::RemindCallback, this)));
    this->runAction(paction);
}

void ModeLayer::RemindCallback()
{
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
}

void ModeLayer::onEnter()
{
    Layer::onEnter();
    if (m_type == Type_Info_Reminder) {
        return;
    }

}

void ModeLayer::onExit()
{
//    if (m_Listener) {
//        _eventDispatcher->removeEventListener(m_Listener);
//    }
    Layer::onExit();
}

void ModeLayer::touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::ENDED:
        {
            Node *pbutton = (Node *)pSender;
            int tag = pbutton->getTag();
            switch (tag) {
                case Tag_Cancel:
                {
                    this->removeFromParent();
                    if (CancelCallback) {
                        CancelCallback();
                    }
                }
                    break;
                case Tag_Ensure:
                {
                    this->removeFromParent();
                    if (EnsureCallback) {
                        EnsureCallback();
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



void ModeLayer::setEnsureCallback(const std::function<void()> &function)
{
    EnsureCallback = function;
}

void ModeLayer::setCancelCallback(const std::function<void()> &function)
{
    CancelCallback = function;
}