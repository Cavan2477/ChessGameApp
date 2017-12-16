#ifndef __nn_TableItemView_H__
#define __nn_TableItemView_H__

#include "CocosHeader.h"
#include "CMD_Game.h"
#include "TableMgr.h"
#include "HeaderRequest.h"
#include "ClipText.h"

NN_NAMESPACE_BEGIN

struct tagTableParam
{
	WORD wTableId;
	int nChair;
};
class TableItemView:public UINode
{
public:
	TableItemView();
	~TableItemView();

	CREATE_FUNC(TableItemView);
	virtual bool init();

	//桌子编号
	CC_SYNTHESIZE(int,m_nTableId,TableId);
	//ui消息通知
	CC_SYNTHESIZE(UILayer*,m_notify,NotifyNode);
    //有效点击区域
    CC_SYNTHESIZE(cocos2d::Rect, m_rectValidArea, ValidTouchArea);

	void refreshTableItem(const tagTableItem* tableItem);
    
    void refreshTableUser(UserData *pUser);
    
    void removeUser(const WORD &wChair);
private:
	//button 点击回调
	void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	//桌子玩家
	cocos2d::ui::Button *m_btnChairs[4];
    HeaderRequest *m_pChairs[4];
	//玩家准备标志
	cocos2d::Sprite *m_spReady[4];
    //玩家姓名
    cocos2d::ui::Text *m_textUserName[4];
    ClipText *m_clipUserName[4];
	//桌子编号
	cocos2d::ui::Text *m_textTableId;
	//桌子锁定
    cocos2d::Sprite *m_spLock;
    //游戏进行中
    cocos2d::Sprite *m_spPlaying;
};

NN_NAMESPACE_END

#endif