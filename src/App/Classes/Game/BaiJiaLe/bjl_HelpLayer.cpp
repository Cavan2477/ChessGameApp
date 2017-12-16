//
//  HelpLayer.cpp
//  NiuNiu
//
//  Created by zhong on 12/24/15.
//
//

#include "BaiJiaLeScene.h"
USING_NS_CC;
using namespace ui;
USING_BJL_NAMESPACE;

HelpLayer::HelpLayer()
{
    
}

HelpLayer::~HelpLayer()
{
    
}

Scene* HelpLayer::createScene()
{
    Scene* pScene = Scene::create();
    HelpLayer* pLayer = HelpLayer::create();
    pScene->addChild(pLayer);
    return pScene;
}

bool HelpLayer::init()
{
    bool bRes = false;
    do
    {
        CC_BREAK_IF( !Layer::init() );
        LOAD_CSB("help/HelpLayer.csb");
        
        auto pscrollview = static_cast<ui::ScrollView*>(m_root->getChildByName("bg_dikuang")->getChildByName("content_view"));
        CC_ASSERT(pscrollview != nullptr);
        pscrollview->setBounceEnabled(true);
        
        auto content = FileUtils::getInstance()->getStringFromFile("public_res/help.txt");
        auto ptext = Label::createWithSystemFont(content, FontBold, 28);
        ptext->setDimensions(pscrollview->getContentSize().width * 0.99,0);
        ptext->setAnchorPoint(Vec2(0.5f, 1.f));
        ptext->setPosition(Vec2(pscrollview->getContentSize().width * 0.5, ptext->getContentSize().height));
        pscrollview->setInnerContainerSize(cocos2d::Size(pscrollview->getContentSize().width,ptext->getContentSize().height));
        pscrollview->addChild(ptext);
        
        //关闭
        auto csbBtn = static_cast<Button*>(m_root->getChildByName("back_btn"));
        CC_ASSERT(csbBtn != nullptr);
        csbBtn->addTouchEventListener(CC_CALLBACK_2(HelpLayer::touchEvent, this));
        bRes = true;
    } while (false);
    return bRes;
}

void HelpLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    TOUCH_EVENT
    if (Widget::TouchEventType::ENDED == type)
    {
        if (0 == strcmp("back_btn", nodename.c_str()))
        {
            INSTANCE(AudioMgr)->playClickAudio();
            /*
            //场景切换
            Scene *pScene = static_cast<Scene*>(PlazaScene::create());
            TransitionScene *reScene = TransitionSlideInR::create( TIME_SCENE_CHANGE, pScene);
            Director::getInstance()->replaceScene(reScene);
             */
            INSTANCE(SceneMgr)->transitionScene(BJL_SCENE(PLAZA_SCENE),false);
        }
    }
}