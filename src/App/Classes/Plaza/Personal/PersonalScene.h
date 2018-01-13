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
    EM_PERSONAL_TYPE_INFO = 0,
    EM_PERSONAL_TYPE_MODIFY_LOGON_PWD,
    EM_PERSONAL_TYPE_MODIFY_BANK_PWD
}EM_PERSONAL_TYPE;

typedef enum
{
    EM_PERSONAL_MODIFY_TYPE_SEX = 0,
    EM_PERSONAL_MODIFY_TYPE_NICK_NAME,
}EM_PERSONAL_MODIFY_TYPE;

typedef enum
{
    EM_INFO_TEXT_NICKNAME	=  7,			//文本-昵称
    EM_INFO_BTN_MODIFY		=  8,			//按钮-修改
    EM_INFO_EDIT_NICKNAME	=  9,			//输入框-昵称
    EM_INFO_TEXT_PERSONAL	= 10,			//文本-个人
    EM_INFO_EDIT_LOGON_PWD	= 11,			//输入框-登录密码
    EM_INFO_EDIT_BANK_PWD	= 12,			//输入框-银行密码
    EM_INFO_TEXT_FALE		= 13,			//文本-男性
    EM_INFO_TEXT_FEMALE		= 14,			//文本-女性
    EM_INFO_LAYOUT_MAIN		= 20,			//布局
    EM_INFO_ROOT_MAIN		= 21,			//根
    EM_INFO_IMAGE_BG		= 30			//背景
}EM_INFO;

class PersonalScene : public Layer , public cocos2d::ui::EditBoxDelegate
{
public:
    PersonalScene();
    virtual ~PersonalScene();

    virtual bool init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    CREATE_FUNC(PersonalScene);
    
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
    void SendModifyBankPwd(const std::string &oldpass, const std::string &newpass);

    //发送修改登录密码
    void SendModifyLogonPwd(const std::string &oldpass, const std::string &newpass);

private:
    EM_PERSONAL_TYPE				m_emPersonalType;
    EM_PERSONAL_MODIFY_TYPE			m_emPersonalModifyType;

	PhotoBridge						*m_pPhotoBridge;
	
    cocos2d::ui::Layout				*m_pUiLayoutPersonal;
    cocos2d::ui::Layout				*m_pUiLayoutModifyLogon;
    cocos2d::ui::Layout				*m_pUiLayoutModifyBank;
    
	BYTE							m_cbGender;

	CC_SYNTHESIZE_RETAIN(Sprite *, _headSprite, HeadSprite);
};

#endif /* Personal_hpp */
