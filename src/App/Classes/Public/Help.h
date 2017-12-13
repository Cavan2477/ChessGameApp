//
//  Help.h
//  MyGame
//
//  Created by wh on 15/4/27.
//
//

#ifndef __MyGame__Help__
#define __MyGame__Help__

#include <stdio.h>
#include "CMD_Stdafx.h"
#include "cocos2d.h"
#include "PublicDefine.h"
USING_NS_CC;


extern "C"{
    
    const char VersionCheckURL[] = "http://www.foxuc.cn/APPService/JDDownService.ashx?action=getversion";
    
    void getCurrentVersion(WORD kindID);

}


extern "C"
{
    // 转换UTF8字符串到UTF16字符串
    void UTF8Str_To_UTF16Str( const std::string &str, WORD* pwszUTF16Str );
    
    int UTF8Str_To_UTF16Str_BYTE( const BYTE* pbszUTF8Str, WORD* pwszUTF16Str );
    
    //获取当前时间，单位为毫秒
    LONGLONG getsystemtomillisecond();
    
 
    
}

void Labellengthaddpoint(cocos2d::Label *plabel, float lenght);

void readAnimation(const char *FramForm, const char *SaveName, int framnum ,float time , int beginindex = 0);

//获取字符串长度，1个汉字长度为1
int strLength(const std::string &str);

//UTF_16 转换为字符串
const std::string WHConverUnicodeToUtf8WithArray(WORD srcUnicode[]);

//字符串MD5加密
const std::string MD5Encrypt(const std::string &str);

//对分数进行处理
const std::string getScoreString(LONGLONG score);
//分数添加逗号分隔,plusstr默认为逗号
const std::string getScorewithComma(LONGLONG score, const std::string & plusstr = ",");
//分数固定长度
const std::string getScoreWithLength(LONGLONG score, int length);

//分数显示大小缩放
void Labellengthdeal(cocos2d::Label *plabel , float length);

//ios获取手机设备码
const std::string WHGetMacAddress();

//获取当前时间
const std::string getCurrentTime();

//获取时间
const std::string getTimeStr(SCORE timesec);



//由数据创建纹理，对GIF数据做了额外处理
cocos2d::Texture2D *CreatewithImageData(unsigned char *pdata, DWORD wsize);

const std::string File(LIST_Kind kind,const std::string &res);


void getAreaConfig();



#endif /* defined(__MyGame__Help__) */
