/************************************************************************************
 * file: 		UtilEx.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/28 22:08:20
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/28			
 *
 ************************************************************************************/

#include "UtilEx.h"
#include "stdlib.h"
#include <ctype.h>

Image* CUtilEx::getSDCardImage(const std::string& listname) 
{
    Image *image = new Image();
    char filepath[128] = { };
    // .......
    // ......
    if (!image->initWithImageFile(filepath))
    {
        CC_SAFE_DELETE(image);
    }
    return image;
}

std::string CUtilEx::getString(const std::string& key) {
    do {
        // 锟斤拷取锟侥憋拷锟街碉拷
        __Dictionary* pDictionary = __Dictionary::createWithContentsOfFile(
                                                                           "string.plist");
        CC_BREAK_IF(pDictionary==NULL);
        __Dictionary* stringDic =
        dynamic_cast<__Dictionary*>(pDictionary->objectForKey(
                                                              "txt_info"));
        CC_BREAK_IF(stringDic==NULL);
        return stringDic->valueForKey(key)->getCString();
    } while (0);
    return "";
}
// UTF8字符计数
int CUtilEx::getUTF8StringCount(const char* src) {
    int count = 0;
    do {
        if (nullptr == src || src[0] == '\0')
            break;
#ifdef WIN32
        count = strlen(src);
#else
        char ch = 0;
        while ((ch = *src)) {
            CC_BREAK_IF(!ch);
            if ((0x80 & ch) == 0x00) {
                count++;
            } else if (0x80 != (0xC0 & ch)) {
                count += 2;
            }
            ++src;
        }
#endif
    } while (0);
    return count;
}

std::vector<std::string> CUtilEx::parseUTF8Helper(const std::string& str,const int& nLen,int& nTotalLen)
{
    auto l = str.length();
    std::vector<std::string> res = std::vector<std::string>();
    
    for (int p = 0; p < l; )
    {
        int size;
        unsigned char c = str[p];
        
        if(c < 0x80) //
        {
            size = 1;
            nTotalLen += nLen / 2;
        }
        else if (c < 0xe0)
        {
            size = 2;
        }
        else if (c < 0xf0) //
        {
            size = 3;
            nTotalLen += nLen;
        }
        else if (c < 0xf8)
        {
            size = 4;
        }
        else if (c < 0xfc)
        {
            size = 5;
        }
        else if (c < 0xfe)
        {
            size = 6;
        }
        std::string temp = "";
        temp = str.substr(p,size);
        res.push_back(temp);
        
        p += size;
    }
    return res;
}

std::vector<std::string> CUtilEx::parseUTF8Helper(const std::string& str, int &nCNCount)
{
    auto l = str.length();
    std::vector<std::string> res = std::vector<std::string>();
    nCNCount = 0;
    
    for (int p = 0; p < l; )
    {
        int size;
        unsigned char c = str[p];
        
        if(c < 0x80) //
        {
            size = 1;
        }
        else if (c < 0xe0)
        {
            size = 2;
        }
        else if (c < 0xf0) //
        {
            size = 3;
            ++nCNCount;
        }
        else if (c < 0xf8)
        {
            size = 4;
        }
        else if (c < 0xfc)
        {
            size = 5;
        }
        else if (c < 0xfe)
        {
            size = 6;
        }
        std::string temp = "";
        temp = str.substr(p,size);
        res.push_back(temp);
        
        p += size;
    }
    return res;
}

std::string CUtilEx::spliteUTF8Str(const std::string &str, const int &nWidth, const int &nMaxWidth, bool &bSplite)
{
    auto l = str.length();
    std::string res = "";
    int tmpLen = 0;
    bSplite = false;
    
    for (int p = 0; p < l; )
    {
        int size;
        unsigned char c = str[p];
        
        if(c < 0x80) //
        {
            size = 1;
            if ('A' <= c && c <= 'Z')
            {
                tmpLen += nWidth * 2 / 3;
            }
            else
            {
                tmpLen += nWidth / 2;
            }
        }
        else if (c < 0xe0)
        {
            size = 2;
        }
        else if (c < 0xf0) //
        {
            size = 3;
            tmpLen += nWidth;
        }
        else if (c < 0xf8)
        {
            size = 4;
        }
        else if (c < 0xfc)
        {
            size = 5;
        }
        else if (c < 0xfe)
        {
            size = 6;
        }
        if (tmpLen >= nMaxWidth)
        {
            bSplite = true;
            break;
        }
        
        std::string temp = "";
        temp = str.substr(p,size);
        res += temp;
        
        p += size;
    }
    return res;
}

bool CUtilEx::checkSpecialCharacter(const char* src) {
    
    do {
        if (nullptr == src || src[0] == '\0')
            break;
        char ch = 0;
        while ((ch = *src)) {
            CC_BREAK_IF(!ch);
            if ((ch >= 0 && ch <= 47) || (ch >= 58 && ch <= 64)
                || (ch >= 91 && ch <= 96) || (ch >= 123 /*&& ch <= 223*/)) {
                return true;
            }
            ++src;
        }
        
    } while (0);
    
    return false;
}

bool CUtilEx::containsEmoji(const std::string &str)
{
    std::u16string ut16;
    if (StringUtils::UTF8ToUTF16(str, ut16))
    {
        if (ut16.empty())
        {
            return false;
        }
        size_t len = ut16.length();
        for (size_t i = 0; i < len; ++i)
        {
            char16_t hs = ut16[i];
            if (0xd800 <= hs && hs <= 0xdbff)
            {
                if (ut16.length() > (i + 1))
                {
                    char16_t ls = ut16[i + 1];
                    int uc = ((hs - 0xd800) * 0x400) + (ls - 0xdc00) + 0x10000;
                    if (0x1d000 <= uc && uc <= 0x1f77f)
                    {
                        return true;
                    }
                }
            }/*
              else if (ut16.length() > (i + 1))
              {
              char16_t ls = ut16[i + 1];
              if (ls == 0x20e3)
              {
              return true;
              }
              }*/
            else
            {
                if (0x2100 <= hs && hs <= 0x27ff)
                {
                    return true;
                }
                else if (0x2B05 <= hs && hs <= 0x2b07)
                {
                    return true;
                }
                else if (0x2934 <= hs && hs <= 0x2935)
                {
                    return true;
                }
                else if (0x3297 <= hs && hs <= 0x3299)
                {
                    return true;
                }
                else if (hs == 0xa9 || hs == 0xae || hs == 0x303d || hs == 0x3030 || hs == 0x2b55 || hs == 0x2b1c || hs == 0x2b1b || hs == 0x2b50)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

long CUtilEx::getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    CCLOG("getCurrentTime:[%ld],[%d]", tv.tv_sec, tv.tv_usec);
    return tv.tv_sec;
}

char* CUtilEx::getTimeDescribe(long num) {
    struct tm *tm;
    time_t lt = (time_t) num;
    tm = localtime(&lt);
    char *date = new char[32];
    sprintf(date, "%02d-%02d-%02d %02d:%02d", ((int) tm->tm_year + 1900),
            (int) tm->tm_mon + 1, (int) tm->tm_mday, (int) tm->tm_hour,
            (int) tm->tm_min);
    return date;
}

bool CUtilEx::onKeyBackDispatch(Node * node) {
    do {
        CC_BREAK_IF(node == NULL);
        if (node->getChildrenCount() > 0) {
            Vector<Node*> child = node->getChildren();
            vector<Node *>::iterator iter;
            for (iter = child.begin(); iter != child.end(); ++iter) {
                Node * tmp = *iter;
                if (onKeyBackDispatch(tmp))
                    return true;
            }
        }
        /*
         if (INTERFACE_OF(IKeyBackListener, node)) {
         IKeyBackListener * tmp_node = dynamic_cast<IKeyBackListener*>(node);
         return tmp_node->OnKeyBackEvent();
         }
         */
    } while (false);
    return false;
}

void CUtilEx::enableBtn(cocos2d::ui::Button *btn,bool bEnable,bool bHide /*= false*/)
{
    btn->setEnabled(bEnable);
    
    if (bEnable)
    {
        btn->setOpacity(255);
        btn->setVisible(!bHide);
    }
    else
    {
        if (bHide)
        {
            btn->setVisible(false);
        }
        else
        {
            btn->setOpacity(125);
        }
    }
}

cocos2d::Sprite* CUtilEx::convertToGraySprite(Sprite* sp)
{
    const GLchar* pszFragSource =
    "#ifdef GL_ES \n \
    precision mediump float; \n \
#endif \n \
    uniform sampler2D u_texture; \n \
    varying vec2 v_texCoord; \n \
    varying vec4 v_fragmentColor; \n \
    void main(void) \n \
    { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 col = texture2D(u_texture, v_texCoord); \n \
        float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(grey, grey, grey, col.a); \n \
    }";
    /*
    const GLchar* pszVertSource =
    "attribute vec4 a_position; \n \
    attribute vec2 a_texCoord; \n \
    attribute vec4 a_color; \n \
    \n#ifdef GL_ES\n \n \
    varying lowp vec4 v_fragmentColor; \n \
    varying mediump vec2 v_texCoord; \n \
    \n#else\n \n \
        varying vec4 v_fragmentColor; \n \
    varying vec2 v_texCoord; \n \
    \n#endif\n \n \
    void main() \n \
    { \n \
        gl_Position = CC_PMatrix * a_position; \n \
        v_fragmentColor = a_color; \n \
        v_texCoord = a_texCoord; \n \
    }";
     */
    if (sp)
    {
        GLProgram* pProgram = new GLProgram();
        //ccPositionTextureColor_vert
        /*
         2015.3.11 bug (engine v3.2): use "ccPositionTextureColor_vert" param will trigger sprite's scale°¢position offset problem
         */
        pProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, pszFragSource);
        sp->setGLProgram(pProgram);
        pProgram->release();
        
        sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
        
        sp->getGLProgram()->link();
        sp->getGLProgram()->updateUniforms();
    }
    return sp;
}

cocos2d::Sprite* CUtilEx::convertToNormalSprite(Sprite* sp)
{
    const GLchar* pszFragSource =
    "#ifdef GL_ES \n \
    precision mediump float; \n \
#endif \n \
    uniform sampler2D u_texture; \n \
    varying vec2 v_texCoord; \n \
    varying vec4 v_fragmentColor; \n \
    void main(void) \n \
    { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 col = texture2D(u_texture, v_texCoord); \n \
        gl_FragColor = vec4(col.r, col.g, col.b, col.a); \n \
    }";
    if (sp)
    {
        GLProgram* pProgram = new GLProgram();
        pProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, pszFragSource);
        sp->setGLProgram(pProgram);
        pProgram->release();
        
        sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
        
        sp->getGLProgram()->link();
        sp->getGLProgram()->updateUniforms();
    }
    return sp;
}