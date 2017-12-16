//
//  DeskCell.h
//  MyGame
//
//  Created by wh on 15/5/18.
//
//

#ifndef __MyGame__DeskCell__
#define __MyGame__DeskCell__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Project.h"

class UserData;
class DeskCell : public cocos2d::ui::ImageView
{
public:
    static DeskCell *create(const std::string &imagefile, int index);
    
    void initwithIndex(int index);
    
    int getIndex(){return m_index;}
    
    void updateStatus(BYTE status);
    
    void userEnter(UserData *pdata, WORD chairID);
    
    void userStatus(UserData *pdata, WORD chairID);
    
    void userLeave(WORD chairID);
    
    void clearUser();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    int m_index;
    
};

#endif /* defined(__MyGame__DeskCell__) */
