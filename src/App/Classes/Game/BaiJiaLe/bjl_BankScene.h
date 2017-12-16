//
//  BankScene.hpp
//  NiuNiu
//
//  Created by zhong on 12/23/15.
//
//

#ifndef bjl_header_BankScene_hpp
#define bjl_header_BankScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "BaiJiaLeScene.h"
#include "BankSceneUI/BankLayer.h"

BJL_NAMESPACE_BEGIN

class BankScene:public UIScene
{
    UISCENE_DECLAR(BankScene, BJL_SCENE(BANK_SCENE));
    //异步预加载资源
    static std::vector<tagUIResourceParam> uiAsynLoadResource();
    
    //资源加载结束
    static std::vector<tagUIResourceParam> uiAsynLoadFinished();
    
    //释放资源
    static std::vector<tagUIResourceParam> uiUnloadResource();
private:
    //大厅消息回包处理
    //开通银行结果
    void InsureEnableResult(void* pData, WORD wSize);
    //银行资料
    void InsureInfoResult(void* pData, WORD wSize);
    //操作成功
    void BankSuccedResult(void* pData, WORD wSize);
    //操作失败
    void BankFailureResult(void* pData, WORD wSize);
    //用户信息
    void BankUserInfoResult(void* pData, WORD wSize);
    //游戏消息回包
    void BankInsureResult(WORD  wSubCmdID, void* pData, WORD wSize);
private:
    BankLayer *m_bankLayer;
};

BJL_NAMESPACE_END
#endif /* BankScene_hpp */
