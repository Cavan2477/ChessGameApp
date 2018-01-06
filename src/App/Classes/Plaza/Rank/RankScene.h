//
//  Rank.hpp
//  GameProject
//
//  Created by Tang Miao on 2/2/16.
//
//

#ifndef __RankScene_h__
#define __RankScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "../../Common/MacroDefine.h"
#include "../../Common/CMD_Stdafx.h"

using namespace cocos2d;

class RankScene : public Layer
{
public:
    RankScene();
    virtual ~RankScene();
    virtual bool init();
    CREATE_FUNC(RankScene);
public:
    void initRankShow();
    void requestRankData();
    void popRank();
    void rankRequestCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void buttonEventWithClose(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void RankReuqcestEvent(cocos2d::EventCustom *event);
public:
    struct RankData :public Ref
    {
        
        public:
        
        //昵称
        CC_SYNTHESIZE(std::string, _nickName, NickName);
        
        //金币
        CC_SYNTHESIZE(LONGLONG, _userScore,UserScore);
        
        //排行
        CC_SYNTHESIZE(int, _rankIndex, RankIndex);
        
    };
    
private:
    std::vector<RankData*>  m_RankData;
    cocos2d::ui::Layout *_layout;
    
};
#endif /* Rank_hpp */
