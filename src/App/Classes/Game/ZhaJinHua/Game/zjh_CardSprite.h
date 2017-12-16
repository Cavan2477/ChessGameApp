#pragma  once 
#include "cocos2d.h"
#include "define.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "CocosHeader.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;
ZJH_NAMESPACE_BEGIN
enum CardSize
{
	CardSize_SMALL,
	CardSize_NORMAL,
};
enum Card_Value
{
    Value_Back = 0x51,//正常背面
    Value_Break,//背面裂开
};
class CardSprite : public Sprite
{
public:
	//function

	CardSprite();
	static CardSprite* createWithCardSize(int _value,CardSize _size=CardSize_NORMAL);
	virtual bool init();
	void selectSelf();//选中当前牌
	void moveCallBack(Ref* obj);//设置当前为非移动状态
	void changeDisplay(int _value, CardSize _size = CardSize_NORMAL);
	//CREATE_FUNC(CardSprite);
    SpriteFrame* getSpriteFrameByTexture(Texture2D* pT2d, cocos2d::Rect _rc);
	SpriteFrame* getSpriteFrameByTexture(Texture2D* pT2d);
	//member
	CardSize m_cardSize;
	BYTE m_nValue;
	bool m_bIsOutState;// 是否为出牌状态
	bool m_bCanMove;//是否正在移动 正在移动 重复点击无效
	
private:
	bool m_bIsFirstInit;//是否为第一次初始化
	Sprite* m_pCardColorRB;//右下角花色
	Sprite* m_pCardColorLT;//左上角花色
	Sprite* m_pCardValue;//大小
	
};
ZJH_NAMESPACE_END
