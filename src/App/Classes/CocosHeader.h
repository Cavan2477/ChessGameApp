#ifndef __CocosHeader_H__
#define __CocosHeader_H__

#include <vector>
#include <map>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"

#include "LibExt/UIInterface.h"
#include "Common/MacroDefine.h"
#include "Common/PublicDefine.h"
#include "DataMgr/HallDataMgr.h"
#include "DataMgr/NetworkMgr.h"
#include "DataMgr/AudioMgr.h"
#include "Public/Help.h"
#include "Utils/SceneMgr/SceneMgr.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "MultiPlatformSrc/ios/MultiPlatformMgr-ios.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "MultiPlatformSrc/android/MultiPlatformMgr-android.h"
#endif

#define LOAD_CSB( CSB ) \
	m_root = CSLoader::createNode(CSB); \
	CC_BREAK_IF(m_root == nullptr); \
	this->addChild(m_root); \

#define LOAD_CSB_ITEM( CSB ) \
    m_root = CSLoader::createNode(CSB); \
    CC_BREAK_IF(m_root == nullptr); \
    this->addChild(m_root); \

#define LAYER_LOAD( READERNAME, READER, CSB) \
    CSLoader* instance = CSLoader::getInstance(); \
    instance->registReaderObject(READERNAME,(ObjectFactory::Instance)READER::getInstance); \
    m_root = instance->createNode(CSB); \
    CC_BREAK_IF(m_root == nullptr); \
    this->addChild(m_root); \

#define CONCATE(A,B) A##B

#define PLAZA_RES_SUFIX ("")

//场景切换时间定义
#ifndef TIME_SCENE_CHANGE
#define TIME_SCENE_CHANGE (0.25f)
#endif

//按钮触摸移动限制
#define DIS_BTN_ENABLE (20.f)

#define UILAYER_WITHNOTIFY(VAR) \
public: \
    VAR(); \
    ~VAR(); \
    static VAR* createLayer(UILayer *notify) \
    { \
        VAR* p = new(std::nothrow) VAR(); \
        if(nullptr != p) \
        { \
            p->m_notify = notify; \
            if (p->init()) \
            { \
                p->autorelease(); \
                return p; \
            } \
        } \
        CC_SAFE_DELETE(p); \
        return nullptr; \
    }\
    virtual bool init() override; \
private: \
    UILayer *m_notify;

#define UILAYER_DECLAR(VAR) \
public: \
    VAR(); \
    ~VAR(); \
    static VAR* createLayer(UIScene *notify) \
    { \
        VAR* p = new(std::nothrow) VAR(); \
        if(nullptr != p) \
        { \
            p->m_notify = notify; \
            if (p->init()) \
            { \
                p->autorelease(); \
            return p; \
            } \
        } \
        CC_SAFE_DELETE(p); \
        return nullptr; \
    }\
virtual bool init() override; \
private: \
UIScene *m_notify;

#define UISCENE_DECLAR(VAR,SCENE_NAME) \
public: \
    VAR(); \
    ~VAR(); \
    SCENE_BIND(SCENE_NAME); \
    \
    CREATE_FUNC(VAR); \
    virtual bool init() override; \
    virtual void onEnterTransitionDidFinish() override; \
    virtual void onExit() override; \
    virtual bool uiMsg(const int &main, const int &sub,void *pData, const int &nLen) override; \

#define TOUCH_EVENT \
Widget *pWidget = (Widget*)pSender; \
if (nullptr == pWidget) \
{ \
    return; \
} \
std::string nodename = pWidget->getName(); \

#ifdef __cplusplus
//牛牛命名空间
#define NN_NAMESPACE_BEGIN namespace nn_game {
#define NN_NAMESPACE_END }
#define USING_NN_NAMESPACE using namespace nn_game
#define NN_NAMESPACE nn_game::
#define NN_NAMESPACE_FUN(PARAM) nn_game::PARAM

//百家乐命名空间
#define BJL_NAMESPACE_BEGIN namespace bjl_game {
#define BJL_NAMESPACE_END }
#define USING_BJL_NAMESPACE using namespace bjl_game
#define BJL_NAMESPACE bjl_game::
#define BJL_NAMESPACE_FUN(PARAM) bjl_game::PARAM

//李逵劈鱼命名空间
#define LKPY_NAMESPACE_BEGIN namespace lkpy_game {
#define LKPY_NAMESPACE_END }
#define USING_LKPY_NAMESPACE using namespace lkpy_game
#define LKPY_NAMESPACE lkpy_game
#define LKPY_NAMESPACE_FUN(PARAM) lkpy_game::PARAM

//扎金花命名空间
#define ZJH_NAMESPACE_BEGIN namespace zjh_game {
#define ZJH_NAMESPACE_END }
#define USING_ZJH_NAMESPACE using namespace zjh_game
#define ZJH_NAMESPACE zjh_game::
#define ZJH_NAMESPACE_FUN(PARAM) zjh_game::PARAM

//通比牛牛命名空间
#define TBNN_NAMESPACE_BEGIN namespace tbnn_game {
#define TBNN_NAMESPACE_END }
#define USING_TBNN_NAMESPACE using namespace tbnn_game
#define TBNN_NAMESPACE tbnn_game::
#define TBNN_NAMESPACE_FUN(PARAM) tbnn_game::PARAM

#else

#define NN_NAMESPACE_BEGIN
#define NN_NAMESPACE_END
#define USING_NN_NAMESPACE
#define NN_NAMESPACE
#define NN_NAMESPACE_FUN(PARAM) PARAM

#define BJL_NAMESPACE_BEGIN
#define BJL_NAMESPACE_END
#define USING_BJL_NAMESPACE
#define BJL_NAMESPACE
#define BJL_NAMESPACE_FUN(PARAM) PARAM

#define LKPY_NAMESPACE_BEGIN
#define LKPY_NAMESPACE_END
#define USING_LKPY_NAMESPACE
#define LKPY_NAMESPACE
#define LKPY_NAMESPACE_FUN(PARAM) PARAM

#define ZJH_NAMESPACE_BEGIN
#define ZJH_NAMESPACE_END
#define USING_ZJH_NAMESPACE
#define ZJH_NAMESPACE
#define ZJH_NAMESPACE_FUN(PARAM) PARAM

#define TBNN_NAMESPACE_BEGIN
#define TBNN_NAMESPACE_END
#define USING_TBNN_NAMESPACE
#define TBNN_NAMESPACE
#define TBNN_NAMESPACE_FUN(PARAM) PARAM

#endif

#define DDZ_PREFIX "ddz_"
//牛牛前缀
#define NN_SCENE(SCENE_NAME) nn_##SCENE_NAME
//百家乐前缀
#define BJL_SCENE(SCENE_NAME) bjl_##SCENE_NAME
//通比牛牛
#define TBNN_SCENE(SCENE_NAME) tbnn_##SCENE_NAME

#define SCENE_TRANSITION(SCENE) cocos2d::TransitionProgressOutIn::create(TIME_SCENE_CHANGE, SCENE);
//房间列表 节点坐标
static const cocos2d::Vec2 s_vec2TableListPos = cocos2d::Vec2(0,30);
#endif