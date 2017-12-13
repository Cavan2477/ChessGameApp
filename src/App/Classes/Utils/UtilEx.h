#ifndef _WH_UTIL_EX_H_
#define _WH_UTIL_EX_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "define.h"
#include <locale.h>
USING_NS_CC;
using namespace std;
class CUtilEx {
public:
    static Image* getSDCardImage(const std::string& listname);
    // 获取utf－8字符
    static std::string getString(const std::string& key);
    // utf－8字符计数 ˝
    static int getUTF8StringCount(const char* src);
    
    static std::vector<std::string> parseUTF8Helper(const std::string& str,const int& nLen,int& nTotalLen);
    
    /*
     * @brief utf-8字符串解析（筛选中文字符数量）
     * @param[str] 字符串
     * @param[nCNCount] 中文字符数量
     */
    static std::vector<std::string> parseUTF8Helper(const std::string& str, int &nCNCount);
    
    /*
     * @brief utf-8字符串截取(用于系统文本显示过长的时候，进行截取)
     * @param[nLen] 设定的单个字符显示宽度
     * @param[nMaxLen] 设定总宽度
     * @param[bSplite] 是否截取过
     */
    static std::string spliteUTF8Str(const std::string& str,const int &nWidth,const int &nMaxWidth, bool &bSplite);
    // 特殊字符检测
    static bool checkSpecialCharacter(const char* src);
    
    //是否包含emoj表情
    static bool containsEmoji(const std::string &str);
    
    static long getCurrentTime();
    
    static char* getTimeDescribe(long time);
    
    static bool onKeyBackDispatch(Node * node);
    
    static void enableBtn(cocos2d::ui::Button *btn,bool bEnable,bool bHide = false);
    
    //转换成灰度图
    static cocos2d::Sprite* convertToGraySprite(Sprite* sp);
    //转换成普通图
    static cocos2d::Sprite* convertToNormalSprite(Sprite* sp);
};

#endif
