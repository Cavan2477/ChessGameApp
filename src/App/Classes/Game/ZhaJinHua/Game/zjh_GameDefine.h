
#pragma once
#include "CocosHeader.h"
ZJH_NAMESPACE_BEGIN

#define VS_ANIM           "vs_animate"

#define GameMaxLevel   60  //游戏最高等级


#define MSG_DISPLAY_IMG "displayImg"

#define MSG_UPDATE_NICKNAME "update_nickname"
//PC上游戏的分辨率
#define  Win32Size  Vec2(1280,800)

#define ScaleX  (WinSize.width/Win32Size.x)
#define ScaleY  (WinSize.height/Win32Size.y)

#define PTM_RATIO 32

#define Tag_Fish  10
#define Tag_Bullet 11

#define GLCOLOR_RGB(r,g,b)              ((COLORREF)(((255)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
#define GLCOLOR_RGB1(rgb)               GLCOLOR_RGB(rgb.r,rgb.g,rgb.b)
#define GLCOLOR_RGBA(r,g,b,a)           ((COLORREF)(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
#define GLCOLOR_ARGB(a,r,g,b)           ((COLORREF)(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
#define GLCOLOR_GetR(rgb)               ((BYTE)((rgb)>>16))
#define GLCOLOR_GetG(rgb)               ((BYTE)((rgb)>>8))
#define GLCOLOR_GetB(rgb)               ((BYTE)((rgb)>>0))
#define GLCOLOR_GetA(rgb)               ((BYTE)((rgb)>>24))

#define GETCOLOR_LONG(long)              Color3B(GLCOLOR_GetR(long),GLCOLOR_GetG(long),GLCOLOR_GetB(long))

const int multiplenum[20] = {2,3,4,5,6,8,10,15,20,10,25,30,40,50,100,200,300,200,45,1000};
ZJH_NAMESPACE_END