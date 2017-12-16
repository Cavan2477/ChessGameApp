#ifndef __nn_GameClockNode_H__
#define __nn_GameClockNode_H__

//游戏倒计时
#include "CocosHeader.h"
#include "GameDefine.h"

NN_NAMESPACE_BEGIN

//倒计时回调
typedef std::function<void(cocos2d::Node *sender, const enCountDownTag &tag)> CLOCKNODE_CALLBACK;
class GameClockNode:public cocos2d::Node
{
public:
	GameClockNode();
	~GameClockNode();

	CREATE_FUNC(GameClockNode);
	virtual bool init() override;

	/*
	 * @brief 开启定时器
	 * @param[nLeft] 剩余时间
	 * @param[fun] 倒计时结束回调函数
	 * @param[countTag] 倒计时标签
	 */
	void startCountDown(const int &nLeft,const CLOCKNODE_CALLBACK &fun,const enCountDownTag &countTag);

	//停止定时器
	void stopCountDown();
private:
	void countDown(float dt);
private:
	cocos2d::Sprite *m_spClock;
    //倒计时文本
    cocos2d::ui::TextAtlas *m_atlasLeft;
	int m_nLeft;
	CLOCKNODE_CALLBACK m_funCallBack;
	enCountDownTag m_enCountDownTag;
};

NN_NAMESPACE_END

#endif