//
//  TempGameScene.hpp
//  NiuNiu
//
//  Created by zhong on 12/29/15.
//
//

#ifndef bjl_header_TempGameScene_hpp
#define bjl_header_TempGameScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "BaiJiaLeScene.h"

BJL_NAMESPACE_BEGIN

class TempGameScene:public UIScene
{
public:
    TempGameScene();
    ~TempGameScene();
    SCENE_BIND(BJL_SCENE(TASK_SCENE));
    
    CREATE_FUNC(TempGameScene);
    virtual bool init() override;
private:
    void registerCallBack(const int &subCode, void *data);
};

BJL_NAMESPACE_END
#endif /* TempGameScene_hpp */
