//
//  Help.cpp
//  MyGame
//
//  Created by wh on 15/4/27.
//
//

#include "lkpy_Help.h"
#include "md5.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <CommonCrypto/CommonDigest.h>
#include <sys/types.h>

#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>

#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#include <codecvt>
#endif


#include "gif_lib.h"
#include "CocosHeader.h"
USING_NS_CC;
LKPY_NAMESPACE_BEGIN
//int UTF8_To_UCS4( const BYTE* pbUTF8, DWORD& dwUCS4 )
//{
//    INT i, iLen;
//    BYTE b;
//    if( pbUTF8 == NULL )
//    { // 参数错误
//        return 0;
//    }
//    b = *pbUTF8++;
//    if( b < 0x80 )
//    {
//        dwUCS4 = b;
//        return 1;
//    }
//    if( b < 0xC0 || b > 0xFD )
//    { // 非法UTF8
//        return 0;
//    }
//    if( b < 0xE0 )
//    {
//        dwUCS4 = b & 0x1F;
//        iLen = 2;
//    }
//    else if( b < 0xF0 )
//    {
//        dwUCS4 = b & 0x0F;
//        iLen = 3;
//    }
//    else if( b < 0xF8 )
//    {
//        dwUCS4 = b & 7;
//        iLen = 4;
//    }
//    else if( b < 0xFC )
//    {
//        dwUCS4 = b & 3;
//        iLen = 5;
//    }
//    else
//    {
//        dwUCS4 = b & 1;
//        iLen = 6;
//    }
//    for( i = 1; i < iLen; i++ )
//    {
//        b = *pbUTF8++;
//        if( b < 0x80 || b > 0xBF )
//        { // 非法UTF8
//            break;
//        }
//        dwUCS4 = (dwUCS4 << 6) + (b & 0x3F);
//    }
//    if( i < iLen )
//    { // 非法UTF8
//        return 0;
//    }
//    else
//    {
//        return iLen;
//    }
//}

// 转换UCS4编码到UCS2编码
//INT UCS4_To_UTF16( DWORD dwUCS4, WORD* pwUTF16 )
//{
//    if( dwUCS4 <= 0xFFFF )
//    {
//        if( pwUTF16 != NULL )
//        {
//            *pwUTF16 = static_cast<WORD>(dwUCS4);
//        }
//        return 1;
//    }
//    else if( dwUCS4 <= 0xEFFFF )
//    {
//        if( pwUTF16 != NULL )
//        {
//            pwUTF16[0] = static_cast<WORD>( 0xD800 + (dwUCS4 >> 10) - 0x40 ); // 高10位
//            pwUTF16[1] = static_cast<WORD>( 0xDC00 + (dwUCS4 & 0x03FF) ); // 低10位
//        }
//        return 2;
//    }
//    else
//    {
//        return 0;
//    }
//}

// 转换UTF8字符串到UTF16字符串
//INT UTF8Str_To_UTF16Str( const BYTE* pbszUTF8Str, WORD* pwszUTF16Str )
//{
//    INT iNum, iLen;
//    DWORD dwUCS4;
//    if( pbszUTF8Str == NULL )
//    { // 参数错误
//        return 0;
//    }
//    iNum = 0; // 统计有效字符个数
//    while( *pbszUTF8Str )
//    { // UTF8编码转换为UIImagePickerControllerSourceTypeSavedPhotosAlbum
//        iLen = UTF8_To_UCS4( pbszUTF8Str, dwUCS4 );
//        if( iLen == 0 )
//        { // 非法的UTF8编码
//            return 0;
//        }
//        pbszUTF8Str += iLen;
//        // UCS4编码转换为UTF16编码
//        iLen = UCS4_To_UTF16( dwUCS4, pwszUTF16Str );
//        if( iLen == 0 )
//        {
//            return 0;
//        }
//        if( pwszUTF16Str != NULL )
//        {
//            pwszUTF16Str += iLen;
//        }
//        iNum += iLen;
//    }
//    if( pwszUTF16Str != NULL )
//    {
//        *pwszUTF16Str = 0; // 写入字符串结束标记
//    }
//
//    return iNum;
//}

//单字符:unicode -> utf8 返回转换长度
int WHConverUnicodeToUtf8(char* destUtf8, WORD srcUnicode)
{
    int i=0;
    if( 0x80 > srcUnicode )
    {
        destUtf8[i++]=(char)srcUnicode;
    }
    else if(0x80 <= srcUnicode && srcUnicode <= 0x07FF)
    {
        destUtf8[i++] = (char)((srcUnicode>>6)|0xC0);
        destUtf8[i++] = (char)((srcUnicode&0x3F)|0x80);
    }
    else
    {
        destUtf8[i++] = (char)(0xE0|(srcUnicode>>12));
        destUtf8[i++] = (char)(0x80|((srcUnicode>>6)&0x3F));
        destUtf8[i++] = (char)(0x80|(srcUnicode&0x3F));
    }
    return i;
    
}

//数组:unicode -> utf8
//const std::string WHConverUnicodeToUtf8WithArray( WORD srcUnicode[])
//{
//    char dest[256];
//    memset(dest, 0, 256);
//
//    WORD wIndex=0;
//    for (int i=0; ; i++){
//        if(srcUnicode[i])
//            wIndex += WHConverUnicodeToUtf8(dest+wIndex, srcUnicode[i]);
//        else
//            break;
//    }
//    std::string str;
//    str.append(dest);
//    return str;
////    std::wstring ptest;
////    int length = 0;
////    while (srcUnicode[length++]) {
////
////    }
////    ptest.append(srcUnicode[0], length*2);
////    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
////    return conv.to_bytes( ptest );
//}

//数组:utf8 -> md5 unicode
//void WHConverUtf8ToMD5Unicode(WORD destUnicode[], c_char srcUtf8[])
//{
//    CMD5 cmd5;
//    char* md5Str=cmd5.MD5_Algorithm(srcUtf8);
//    UTF8Str_To_UTF16Str((const BYTE *)md5Str,destUnicode);
//}

LONGLONG getsystemtomillisecond()
{
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    
    LONGLONG time = tv.tv_sec*1000 + tv.tv_usec/1000;
    return time;
}

//const std::string MD5Encrypt(const std::string &str)
//{
//    char md5p[33];
//    md5_passwd((char *)str.c_str(), md5p);
//    std::string test;
//    test.append(md5p);
//    for (int index=0; index<test.size(); ++index) {
//        if (test[index] >='a' && test[index] <= 'z') {
//            test[index] -= 32;
//        }
//    }
//    return test;
//}

//mac address
//const std::string WHGetMacAddress()
//{
//    //安卓设备 mac
//#ifdef WIN32
//    {
//        return NULL;
//    }
//    //ios设备 mac
//#endif
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    int                 mib[6];
//    size_t              len;
//    char                *buf;
//    unsigned char       *ptr;
//    struct if_msghdr    *ifm;
//    struct sockaddr_dl  *sdl;
//
//    mib[0] = CTL_NET;
//    mib[1] = AF_ROUTE;
//    mib[2] = 0;
//    mib[3] = AF_LINK;
//    mib[4] = NET_RT_IFLIST;
//
//    if ((mib[5] = if_nametoindex("en0")) == 0) {
//        printf("Error: if_nametoindex error/n");
//        return NULL;
//    }
//
//    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
//         printf("Error: sysctl, take 1/n");
//        return NULL;
//    }
//
//    if ((buf=(char*)malloc(len)) == NULL) {
//        printf("Could not allocate memory. error!/n");
//        return NULL;
//    }
//
//    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
//        printf("Error: sysctl, take 2");
//        free(buf);
//        return NULL;
//    }
//
//    ifm = (struct if_msghdr *)buf;
//    sdl = (struct sockaddr_dl *)(ifm + 1);
//    ptr = (unsigned char *)LLADDR(sdl);
//
//    NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
//    free(buf);
//
//    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
//    NSString *stringToHash = [NSString stringWithFormat:@"%@%@",outstring,bundleIdentifier];
//    std::string test = [stringToHash UTF8String];
//    test = MD5Encrypt(test);
//    return test;
//#endif
//
//}

int strLength(const std::string &str)
{
    if (typeid(str) == typeid(std::string) && str.length() > 0) {
        int len = (int)str.length();
        std::vector<std::string> dump;
        int i = 0;
        while(i < len) {
            if (~(str.at(i) >> 8) == 0) {
                dump.push_back(str.substr(i, 3));
                i = i + 3;
            } else {
                dump.push_back(str.substr(i, 1));
                i = i + 1;
            }
        }
        return (int)dump.size();
    } else {
        printf("str is not string\n");
        return 0;
    }
}

int getTheoryStrLength(const std::string &str)
{
    int length = 0;
    if (typeid(str) == typeid(std::string) && str.length() > 0) {
        int len = (int)str.length();
        std::vector<std::string> dump;
        int i = 0;
        while(i < len) {
            if (~(str.at(i) >> 8) == 0) {
                dump.push_back(str.substr(i, 3));
                length += 3;
                i = i + 3;
            } else {
                dump.push_back(str.substr(i, 1));
                i = i + 1;
                length += 1;
            }
        }
        return length;
    } else {
        printf("str is not string\n");
        return length;
    }
    
}
//单字符:utf8 -> unicode 返回转换长度
//int WHConverUtf8ToUnicode(WORD *destUnicode, c_char *srcUtf8)
//{
//    int i=0;
//    if( (0x80&srcUtf8[0]) == 0 )         //一个字节的UTF-8
//    {
//        destUnicode[0] = srcUtf8[i++];
//    }
//    if( (0xE0&srcUtf8[0]) == 0xC0 )      //两字节的UTF-8
//    {
//        int a = ( 0x1F & srcUtf8[i++] ) << 6;
//        int b = ( 0x3F & srcUtf8[i++] );
//        destUnicode[0] = a + b;
//    }
//    if( (0xF0&srcUtf8[0]) == 0xE0 )      //三字节的UTF-8
//    {
//        int a = ( 0x1F & srcUtf8[i++] ) << 12;
//        int b = ( 0x3F & srcUtf8[i++] ) << 6;
//        int c = ( 0x3F & srcUtf8[i++] );
//
//        destUnicode[0] = a + b + c;
//    }
//
//    return i;
//}



//void WHConverUft8ToUnicodeWithArray(WORD destUnicode[], c_char srcUtf8[])
//{
//    memset(destUnicode, 0, strlen(srcUtf8)*3);
//
//    WORD wLen=strlen(srcUtf8);
//    WORD wIndex=0;
//    for (WORD i=0; i<wLen; i++){
//        if(*(srcUtf8+wIndex)!='\0')
//            wIndex += WHConverUtf8ToUnicode(destUnicode+i, srcUtf8+wIndex);
//        else
//            break;
//    }
//}

string getGoldStr(string _str,char insertChar)
{
    string tScorestr = "";
    size_t strSize = _str.length();
    size_t index = 0;
    index = _str.find_first_of('.');
    if(index >= strSize-1)
        index = strSize-1;
    //金钱字符加逗号
    int nCount =1;
    for(int i = strSize-1;i >= 0;i--)
    {
        string tStr="";
        if(i <= index)
        {
            
            if(nCount%3 == 0 && i != 0)
            {
                tStr.push_back(insertChar);
            }
            
            nCount++;
        }
        tStr.push_back(_str[i]);
        tScorestr = tStr + tScorestr;
    }
    return tScorestr;
}

int getStringLength(string _str)
{
    int nLen = 0;
    for(int i = 0; i < _str.size();)
    {
        int codeSize = getUnicodeSize(_str[i]);
        nLen++;
        i+=codeSize;
    }
    return nLen;
}

int getTextWidth(string _str,int fontSzie)
{
    int nWidth = 0;
    for(int i = 0; i < _str.size();)
    {
        int codeSize = getUnicodeSize(_str[i]);
        if(codeSize == 1)
            nWidth += fontSzie/2;
        else if(codeSize > 1)
            nWidth += fontSzie;
        i+=codeSize;
    }
    return nWidth;
}

string getStringByWidth(string _str,int fontSzie,int maxWidth,bool bRecursion)
{
    //maxWidth -= fontSzie;//后面补字符".."
    string tempStr;
    int nWidth = 0;
    for(int i = 0; i < _str.size();)
    {
        int codeSize = getUnicodeSize(_str[i]);
        if(codeSize == 1)
            nWidth += fontSzie/5*3;
        else if(codeSize > 1)
            nWidth += fontSzie;
        if(nWidth > maxWidth)
        {
            if(bRecursion)
            {
                int newWidth = 0;
                //if(fontSzie)
                tempStr = getStringByWidth(tempStr,fontSzie,maxWidth-codeSize/2*3,false);
                tempStr.append("..");
            }
            break;
        }
        for(int j = 0;j < codeSize;++j)
        {
            tempStr.push_back(_str[i+j]);
        }
        i+=codeSize;
    }
    return tempStr;
}

bool isEmojiCharacter(char codePoint) {
    return !((codePoint == 0x0) ||
             (codePoint == 0x9) ||
             (codePoint == 0xA) ||
             (codePoint == 0xD) ||
             ((codePoint >= 0x20) && (codePoint <= 0xD7FF)) ||
             ((codePoint >= 0xE000) && (codePoint <= 0xFFFD)) ||
             ((codePoint >= 0x10000) && (codePoint <= 0x10FFFF)));
}

bool isEmojiString(string str)
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
int getUnicodeSize(char c_char)
{
    int i=0;
    if( (0x80&c_char) == 0 )         //一个字节的UTF-8
    {
        i = 1;
    }
    if( (0xE0&c_char) == 0xC0 )      //两字节的UTF-8
    {
        i = 2;
    }
    if( (0xF0&c_char) == 0xE0 )      //三字节的UTF-8
    {
        i = 3;
    }
    return i;
}
//GIF转换回调函数
//int memReadFuncGif(GifFileType* GifFile, GifByteType* buf, int count)
//{
//    char* ptr = (char*)(GifFile->UserData);
//    memcpy(buf, ptr, count);
//    GifFile->UserData = ptr + count;
//    return count;
//}
//
////GIF图片数据转换为RGBA8888
//unsigned char* DecodeMemoryGIF(unsigned char* fData, long fSize, int& width, int& height, int index)
//{
//    GifFileType *GifFile;
//
//    if ((GifFile = DGifOpen(fData, memReadFuncGif)) == NULL)
//    {
//        return 0;
//    }
//
//    if (DGifSlurp(GifFile) == 0)
//    {
//        return 0;
//    }
//
//    unsigned char *image_data = 0, *src = 0, *dst = 0;
//    ColorMapObject *ColorMap;
//    SavedImage *saveImg;
//    GifRowType *ScreenBuffer;
//    GifColorType *ColorMapEntry;
//    int loc = 0;
//
//    if (index >= GifFile->ImageCount)
//    {
//        return 0;
//    }
//    //    index = index % GifFile->ImageCount;
//
//    ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
//    saveImg = &(GifFile->SavedImages[index]);
//    ScreenBuffer = &(saveImg->RasterBits);
//
//    if ((image_data = (unsigned char *) malloc(GifFile->SWidth * 4 * GifFile->SHeight)) == NULL)
//    {
//        DGifCloseFile(GifFile);
//        return 0;
//    }
//    src = saveImg->RasterBits;
//    dst = image_data;
//
//    for (int i = 0; i < GifFile->SHeight; i++)
//    {
//        loc = GifFile->SWidth * i;
//        for (int j = 0; j < GifFile->SWidth; j++)
//        {
//            ColorMapEntry = &(ColorMap->Colors[*(src + loc + j)]);
//            *dst++ = ColorMapEntry->Red;
//            *dst++ = ColorMapEntry->Green;
//            *dst++ = ColorMapEntry->Blue;
//            *dst++ = 255;
//        }
//    }
//
//    width = GifFile->SWidth;
//    height = GifFile->SHeight;
//
//    DGifCloseFile(GifFile);
//    return image_data;
//}

//Texture2D *CreatewithImageData(unsigned char *pdata, DWORD wsize)
//{
//    char buffer[5];
//    memset(buffer, 0, 5);
//    memcpy(buffer, pdata, 3);
//    std::string imageHead(buffer);
//    Texture2D *texture = new Texture2D;
//    Image *image = new Image();
//    if (imageHead == "<!D" || imageHead == "\r\n<") {
//        return NULL;
//    }
//    if (imageHead == "GIF") {
//        int headwidth,headheight;
//        unsigned char * RGBAdata = DecodeMemoryGIF(pdata, wsize,headwidth,headheight,0);
//        texture->initWithData(RGBAdata, wsize, Texture2D::PixelFormat::RGBA8888, headwidth, headheight, cocos2d::Size(headwidth, headheight));
//    }
//    else
//    {
//        image->initWithImageData(pdata, wsize);
//        texture->initWithImage(image);
//    }
//    image->release();
//    texture->autorelease();
//    return texture;
//}
LKPY_NAMESPACE_END
