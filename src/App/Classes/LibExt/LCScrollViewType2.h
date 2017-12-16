//
//  LCScrollViewType2.h
//  CocosDemo
//
//  Created by zhongtao on 14/12/16.
//
//
#ifndef _CocosDemo_LCScrollViewType2_h
#define _CocosDemo_LCScrollViewType2_h
#include <iostream>
#include "cocos2d.h"

#include "LCScrollView.h"
USING_NS_CC;
/*
 * scrollview cell add by user
 */
class LCScrollViewType2:public LCScrollView
{
public:
	LCScrollViewType2();
	~LCScrollViewType2();

	static LCScrollViewType2* create();
	/*
	 * @brief	创建
	 * @param[size]	:scrollview的尺寸
	 * @param[autoRemove] :scrollview上面的cell自动移除的上限，为0表示不自动移除
	 * @param[container] ：用户自定义的container
	 */
	static LCScrollViewType2* create(Size size,int autoRemove = 0,Node* container = nullptr);

	/**
	 * @brief add view cell 
	 * @param1 cell;param2 action
	 **/
	void addScrollViewCell(Node* cell,float offset = 5.0f,Action* action = nullptr);

	/**
	 * @brief move speed
	 **/
	CC_SYNTHESIZE(float,_moveDuration,MoveDuration);

	/*
	 * @brief 是否更新透明度
	 */
	CC_SYNTHESIZE(bool,_updateOpacity,UpdateOpacity);

	/*
	 * @brief 设置cell在超出可见区域后自动隐藏，此属性会造成拖动后元素丢失，在界面不可拖动的时候可设置
	 */
	CC_SYNTHESIZE(bool,_autoHide,AutoHide);

	//清理
	void reSet();
private:	
	void test();
	void recursionMove(size_t idx, Vec2 moveDis);
	void updateCellPos(size_t idx, Vec2 moveDis);
private:
	Vector<Node*> _vecViewCells;
	std::vector<float> _vecOffSet;
	Size _accumulateSize;
	int _autoRemove;
	Size _createSize;
};
#endif