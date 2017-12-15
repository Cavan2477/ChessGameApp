/************************************************************************************
 * file: 		UIInterface.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/11 22:58:37
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/11			
 *
 ************************************************************************************/

#ifndef UIInterface_h
#define UIInterface_h

#include "CocosHeader.h"
#include "../Common/UIMsgDefine.h"

#define SCENE_BIND( SCENE_NAME ) \
    static std::string getSceneName() { return SCENE_NAME; } \

struct tagUIResourceParam
{
    enum UIResourceType
    {
        kTEXTURE_RES,
        kAUDIO_EFFECT,
        kAUDIO_BACKGROUND,
    };
    
    //资源路径
    std::string resPath;
    //plist文件
    std::string plistFile;
    //资源类型
    UIResourceType resType;
    //图片资源类型（PLIST 指plist合图，LOCAL 指单图片）
    cocos2d::ui::Widget::TextureResType texType;
};

static void enableChildren(cocos2d::Node *rootNode, const bool &bEnable)
{
    auto children = rootNode->getChildren();
    for (auto& child : children)
    {
        auto btn = dynamic_cast<cocos2d::ui::Button*>(child);
        if (nullptr != btn)
        {
            cocos2d::log("%s:%d",btn->getName().c_str(), bEnable);
            btn->setEnabled(bEnable);
        }
        else
        {
            auto check = dynamic_cast<cocos2d::ui::CheckBox*>(child);
            if (nullptr != check)
            {
                cocos2d::log("%s:%d",check->getName().c_str(), bEnable);
                check->setEnabled(bEnable);
            }
            else
            {
                enableChildren(child, bEnable);
            }
        }
    }
}

class UIMessageInterface
{
public:
    //ui 线程消息(不能在非ui线程内调用)
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen)
    {
        cocos2d::log("base msg");
        return true;
    }
    
    virtual void enableWidget(cocos2d::Node *rootNode,
                              const bool &bEnable,
                              const std::vector<cocos2d::ui::Widget*> param = std::vector<cocos2d::ui::Widget*>())
    {
        cocos2d::log("base enable");
        if (!param.empty())
        {
            for (size_t i = 0; i < param.size(); ++i)
            {
                param[i]->setEnabled(bEnable);
            }
        }
        else
        {
            enableChildren(rootNode, bEnable);
        }
    }
};

class UIResourceInterface
{
public:
    //异步预加载资源
    static std::vector<tagUIResourceParam> uiAsynLoadResource()
    {
        return std::vector<tagUIResourceParam>();
    }
    
    //资源加载结束
    static std::vector<tagUIResourceParam> uiAsynLoadFinished()
    {
        return std::vector<tagUIResourceParam>();
    }
    
    //释放资源
    static std::vector<tagUIResourceParam> uiUnloadResource()
    {
        return std::vector<tagUIResourceParam>();
    }
public:
    cocos2d::Node *m_root;
};

class UINode:
public cocos2d::Node,
public UIResourceInterface
{
    
};

class UILayer:
	public cocos2d::Layer,
	public UIResourceInterface,
	public UIMessageInterface
{

};

class UIScene: public cocos2d::Scene,
	public UIResourceInterface,
	public UIMessageInterface
{
public:
    static std::string getSceneName(){ return ""; }
    virtual bool initUIScene(){ return true; }
};
#endif /* UIInterface_h */
