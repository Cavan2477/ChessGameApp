//
//  AnimationMgr.hpp
//  DouDiZhu
//
//  Created by zhong on 1/21/16.
//
//

#ifndef AnimationMgr_hpp
#define AnimationMgr_hpp

#include <stdio.h>
//动画管理
#include "CocosHeader.h"
#include "../Singleton_Z.h"

struct tagAnimationParam
{
    //动画名
    std::string m_strName;
    //是否重置动画参数
    bool m_bResetParam;
    //每帧动画持续时间
    float m_fDelay;
    //是否恢复到第一帧
    bool m_bRestore;
    
    tagAnimationParam()
    {
        m_strName = "";
        m_bResetParam = true;
        m_fDelay = 0.0f;
        m_bRestore = true;
    }
};

typedef cocos2d::CallFunc ANIMATION_CALLBACK;
class AnimationMgr
{
    CREATE_SINGLETON_INIT(AnimationMgr,_anInstance,nullptr);
    void init();
public:
    /*
     * @brief 加载帧动画
     * @param[plist] 帧动画plist文件
     */
    void loadAnimationFromFile(const std::string &plist);
    
    /*
     * @brief 加载动画
     * @param[format] 动画图片名字format格式
     * @param[start] 起始索引
     * @param[count] 加载帧数量
     * @param[key] 动画key，用于播放动画
     * @param[texType] 资源类型（plist合图、单图）
     */
    void loadAnimationFromFrame(const std::string &format,
                                const int &start,
                                const int &count,
                                const std::string &key,
                                const cocos2d::ui::Widget::TextureResType &texType = cocos2d::ui::Widget::TextureResType::LOCAL);
    
    /*
     * @brief 清除已加载的动画
     * @param[key] 动画key
     */
    void removeCachedAnimation(const std::string &key);
    
    /*
     * @brief 播放帧动画
     * @param[node] 节点
     * @param[param] 动画参数
     * @param[fun] 回调
     */
    void playAnimation(cocos2d::Node *node, const tagAnimationParam &param, ANIMATION_CALLBACK* fun = nullptr);
    
    cocos2d::Animate* getAnimate(const tagAnimationParam &param);
private:
    static AnimationMgr* _anInstance;
};
#endif /* AnimationMgr_hpp */
