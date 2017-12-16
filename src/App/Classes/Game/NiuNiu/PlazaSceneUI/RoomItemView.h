//
//  RoomItemView.hpp
//  DouDiZhu
//
//  Created by zhong on 1/13/16.
//
//

#ifndef nn_RoomItemView_hpp
#define nn_RoomItemView_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "define.h"

NN_NAMESPACE_BEGIN

class CRoomItemView:public UINode
{
public:
    CRoomItemView();
    ~CRoomItemView();
    CREATE_FUNC(CRoomItemView);
    virtual bool init();
    
    CC_SYNTHESIZE(UILayer*, m_notify, Notify);
public:
    //nType = 10 标示是防作弊
    void refresh(const int &nRoom, const int &nType, const SCORE &lScore, const SCORE &lRuChang, const int &nZaiXian);
    
    //设置房间背景
    void setRoom(const int &nRoom);
    
    //设置房间类型
    void setType(const int &nType);
    
    //设置底分
    void setDifen(const SCORE &difen);
    
    //设置入场分
    void setRuChang(const SCORE &ruchang);
    
    //设置在线人数
    void setZaiXian(const int &zaixian);
    
private:
    //button 点击回调
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType tType);
private:
    //背景
    cocos2d::Sprite *m_spRoomBg;
    cocos2d::ui::Button *m_btnRoomBg;
    //类型
    cocos2d::Sprite *m_spRoomType;
    //防作弊标志
    cocos2d::Sprite *m_spAntiCheating;
    //底分
    //cocos2d::ui::TextAtlas *m_labDiFen;
    cocos2d::ui::Text *m_textDiFen;
    //入场
    //cocos2d::ui::TextAtlas *m_labRuChang;
    cocos2d::ui::Text *m_textRuChang;
    //在线人数
    //cocos2d::ui::TextAtlas *m_labZaiXian;
    cocos2d::ui::Text *m_textZaiXian;
    char m_buf[64];
    
    cocos2d::Action *m_actSelect;
};

NN_NAMESPACE_END

#endif /* RoomItemView_hpp */
