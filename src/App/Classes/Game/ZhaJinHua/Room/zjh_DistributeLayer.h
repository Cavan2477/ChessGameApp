//
//  DistributeLayer.hpp
//  gswz
//
//  Created by zsh on 16/3/14.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "Project.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
ZJH_NAMESPACE_BEGIN
class DistributeLayer:public Layer
{
public:
    static cocos2d::Scene* createScene();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual bool init();
    CREATE_FUNC(DistributeLayer);
private:
    Layout *m_layout;
};
ZJH_NAMESPACE_END

