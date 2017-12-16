//
//  GameDefine.h
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#pragma once
#include "CocosHeader.h"
LKPY_NAMESPACE_BEGIN
#define RenYu_YD_Anim       "fish_17"       //美人鱼正面游动动画
#define RenYu_BYD_Anim      "fish_18"       //美人鱼背面游动动画

#define RenYu_B_To_Q        "fish_btoq"     //美人鱼前转身
#define RenYu_Q_To_B        "fish_qtob"     //美人鱼后转身

#define WaterAnim           "water"         //水波动画

#define FortAnim            "fort"          //炮台动画
#define FortLightAnim       "fort_0"        //激光炮动画
#define BulletAnim          "bullet"        //子弹尾动画
#define BombAnim            "bomb"          //鱼死亡爆炸动画
#define BombDartsAnim       "bombDartsAnim"          //飞镖爆炸动画
#define BlueIceAnim         "blueIce"       //爆炸动画
#define CopperAnim          "copper"        //铜币动画
#define SilverAnim          "silver"        //银币动画
#define GoldAnim            "gold"          //金币动画
#define LightAnim           "light"         //激光准备发射动画
#define watchAnim           "watch"         //无操作倒计时动画
#define FishBall            "fishball"      //炸弹鱼死亡动画
#define FishLight           "fishlight"     //炸弹鱼激光

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
LKPY_NAMESPACE_END
