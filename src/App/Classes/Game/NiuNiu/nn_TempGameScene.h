//
//  TempGameScene.hpp
/************************************************************************************
 * file: 		nn_TempGameScene.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 20:36:39
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __NN_TEMP_GAME_SCENE_HPP__
#define __NN_TEMP_GAME_SCENE_HPP__

#include <stdio.h>
#include "CocosHeader.h"
#include "Header/NiuNiuScene.h"

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
