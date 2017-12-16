//
//  TempGameScene.hpp
//  NiuNiu
//
//  Created by zhong on 12/29/15.
//
//

#ifndef nn_TempGameScene_hpp
#define nn_TempGameScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "NiuNiuScene.h"

NN_NAMESPACE_BEGIN

class TempGameScene:public UIScene
{
public:
    TempGameScene();
    ~TempGameScene();
    SCENE_BIND(NN_SCENE(TASK_SCENE));
    
    CREATE_FUNC(TempGameScene);
    virtual bool init() override;
};

NN_NAMESPACE_END
#endif /* TempGameScene_hpp */
