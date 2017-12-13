/************************************************************************************
 * file: 		Personal.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:04:37
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#ifndef __Personal_h__
#define __Personal_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PhotoBridge.h"
#include "../../Common/MacroDefine.h"

USING_NS_CC;

#define MAN 1
#define WOMAN 0

typedef enum
{
    Type_PersonalInfo = 0,
    Type_ModifyLoginPass,
    Type_ModifyBankPass
}Personal_Type;

typedef enum
{
    modify_sex = 0,
    modify_nick,
}PersonalModify_Type;

typedef enum
{
    INFO_NICK     =  7,
    INFO_MODIFYBTN=  8,
    INFO_NICKEDIT =  9,
    INFO_PERSONAL = 10,
    INFO_LOGINPASS= 11,
    INFO_BANKPASS = 12,
    INFO_MAN      = 13,
    INFO_WOMAN    = 14,
    INFO_MAINLAYOUT = 20,
    INFO_MAINROOT   = 21,
    
    INFO_PICBG      = 30
    
    
}INFOTAG;

class Personal : public Layer , public cocos2d::ui::EditBoxDelegate
{
public:
    Personal();
    virtual ~Personal();

    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(Personal);
    
public:
    void popPersonal();
    void initModify();
    
public:
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) ;
    
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) ;
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    void buttonEventWithMenu(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithGender(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithModifyNick(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithUploadRes(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    void buttonEventWithSureModify(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
    void photocomplete(cocos2d::Image *pimage);
    void sendCustomFaceInfo(cocos2d::Image *pimage);
    void sendSystemFaceInfo(WORD wface);

    //用户头像修改结果
    void userFaceinfoResult(void* pData, WORD wSize);

    //修改操作成功
    void operatesuccessResult(void* pData, WORD wSize);

    //修改操作失败
    void operatefailureResult(void* pData, WORD wSize);
    
    //0表示修改性别，1表示修改昵称
    void sendAlterIndividual(const std::string &name, BYTE cbgerder, int type = 0);
    
    //发送修改银行密码
    void sendAlterBankPass(const std::string &oldpass, const std::string &newpass);

    //发送修改登录密码
    void sendAlterloginPass(const std::string &oldpass, const std::string &newpass);

private:
    Personal_Type                   m_eType;
    PersonalModify_Type             m_eModify;
    cocos2d::ui::Layout            *_personalLayout;
    cocos2d::ui::Layout            *_modifyLoginLayout;
    cocos2d::ui::Layout            *_modifyBankLayout;
    
    CC_SYNTHESIZE_RETAIN(Sprite *, _headSprite, HeadSprite);
    
    PhotoBridge *m_photo;
    BYTE         m_cbGender;
    
};

#endif /* Personal_hpp */
