//
//  TempGameScene.hpp
//  NiuNiu
//
//  Created by zhong on 12/29/15.
//
//

#ifndef tbnn_header_TempGameScene_hpp
#define tbnn_header_TempGameScene_hpp

#include <stdio.h>
#include "CocosHeader.h"
#include "TBNNScene.h"

TBNN_NAMESPACE_BEGIN
class TempGameScene:public UIScene
{
public:
    TempGameScene();
    ~TempGameScene();
    SCENE_BIND(TBNN_SCENE(TEMP_SCENE));
    
    CREATE_FUNC(TempGameScene);
    virtual bool init() override;
private:
    void registerCallBack(const int &subCode, void *data);
};
TBNN_NAMESPACE_END
#endif /* TempGameScene_hpp */
