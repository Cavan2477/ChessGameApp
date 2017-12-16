//
//  Help.h
//  MyGame
//
//  Created by wh on 15/4/27.
//
//

#pragma once

#include <stdio.h>
#include "CMD_Stdafx.h"
#include "cocos2d.h"
#include "CocosHeader.h"
using namespace std;
LKPY_NAMESPACE_BEGIN
extern "C"
{
    // 转换UTF8字符串到UTF16字符串
    //INT UTF8Str_To_UTF16Str( const BYTE* pbszUTF8Str, WORD* pwszUTF16Str );
    
    //void WHConverUtf8ToMD5Unicode(WORD destUnicode[], c_char srcUtf8[]);
    
    //void WHConverUft8ToUnicodeWithArray(WORD destUnicode[], c_char srcUtf8[]);
    
    //LONGLONG getsystemtomillisecond();
    
    string getGoldStr(string _str,char insertChar);
    
    int getStringLength(string _str);
    
    int getTextWidth(string _str,int fontSzie);
    
    string getStringByWidth(string _str,int fontSzie,int maxWidth,bool bRecursion = true);
    
    int getUnicodeSize(char c_char);
    
    bool isEmojiCharacter(char codePoint);
    
    bool isEmojiString(string str);
    
    int getTheoryStrLength(const std::string &str);
}

int strLength(const std::string &str);


//const std::string WHConverUnicodeToUtf8WithArray(WORD srcUnicode[]);

//const std::string MD5Encrypt(const std::string &str);

//const std::string WHGetMacAddress();

//cocos2d::Texture2D *CreatewithImageData(unsigned char *pdata, DWORD wsize);

LKPY_NAMESPACE_END
