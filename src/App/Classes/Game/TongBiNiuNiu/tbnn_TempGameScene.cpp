//
//  TempGameScene.cpp
//  NiuNiu
//
//  Created by zhong on 12/29/15.
//
//

#include "TBNNScene.h"
#include "GameDataMgr.h"
USING_NS_CC;
using namespace ui;
USING_TBNN_NAMESPACE;

TempGameScene::TempGameScene()
{
    
}

TempGameScene::~TempGameScene()
{
    
}

bool TempGameScene::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF(!Scene::init());
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        auto layer = Layer::create();
        this->addChild(layer);
        
        auto sp = Sprite::createWithSpriteFrameName("game_bg_0.png");
        sp->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
        layer->addChild(sp);
        
        sp = Sprite::createWithSpriteFrameName("game_bg.png");
        sp->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
        layer->addChild(sp);
        
        if (INSTANCE(GameDataMgr)->getEnterAntiCheat())
        {            
            if (nullptr == HallDataMgr::getInstance()->AddpopLayer("系统提示", "正在进行分组,请稍后...", Type_Wait)
                ||(nullptr != HallDataMgr::getInstance()->AddpopLayer("系统提示", "正在进行分组,请稍后...", Type_Wait)
                   && nullptr != HallDataMgr::getInstance()->AddpopLayer("系统提示", "正在进行分组,请稍后...", Type_Wait)->getParent()))
            {
                Size visibleSize = WinSize;
                auto m_bg = ImageView::create();
                m_bg->setScale9Enabled(true);
                m_bg->loadTexture("pop_frame.png",Widget::TextureResType::PLIST);
                m_bg->setContentSize(Size(450, 160));
                m_bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2+50));
                m_bg->setOpacity(230);
                m_bg->setTag(1001);
                this->addChild(m_bg);
                
                Label *pcontent = Label::createWithSystemFont("正在进行分组,请稍后...", FontNormal, 28);
                pcontent->setAlignment(TextHAlignment::CENTER);
                pcontent->setDimensions(410, 0);
                pcontent->setColor(Color3B::BLACK);
                pcontent->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height - 36));
                pcontent->setAnchorPoint(Point(0.5, 1.f));
                m_bg->addChild(pcontent);
                
                auto m_cancel = Button::create("bt_close_0.png", "bt_close_1.png", "", Widget::TextureResType::PLIST);
                m_cancel->setPosition(Vec2(m_bg->getContentSize().width-10, m_bg->getContentSize().height-20));
                m_cancel->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                                {
                                                    if (type == Widget::TouchEventType::ENDED) {
                                                        this->removeFromParent();
                                                    }
                                                });
                m_bg->addChild(m_cancel);
                
                ImageView *pimage = ImageView::create();
                pimage->loadTexture("waiting.png", Widget::TextureResType::PLIST);
                pimage->setPosition(Point(m_bg->getContentSize().width/2, 60));
                m_bg->addChild(pimage);
                
                Action *paction = RepeatForever::create(RotateBy::create(1.2f, 360));
                pimage->runAction(paction);
                
                m_bg->setScale(0.9f);
                FiniteTimeAction *paction1 = ScaleTo::create(0.15f, 1.05f);
                FiniteTimeAction *paction2 = ScaleTo::create(0.07f, 1.f);
                Action *sequeaction = Sequence::createWithTwoActions(paction1, paction2);
                m_bg->runAction(sequeaction);
            }
        }
        
        bRes = true;
    } while (false);
    return bRes;
}

void TempGameScene::registerCallBack(const int &subCode, void *data)
{
    if (nullptr != this->getChildByTag(1001))
    {
        this->removeChildByTag(1001);
    }
}