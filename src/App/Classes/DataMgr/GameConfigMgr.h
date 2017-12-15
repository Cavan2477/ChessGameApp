//
//  GameConfigMgr.hpp
//  GameProject
//
//  Created by zhong on 5/9/16.
//
//

#ifndef __GameConfigMgr_h__
#define __GameConfigMgr_h__

#include <stdio.h>
#include "cocos2d.h"

#include "../Pattern/Singleton.h"

typedef struct _stGameConfig
{
    //游戏名
    std::string strGamename;

    //kind_id
    int nGameKindId;
    
	//是否激活游戏
    bool bGameActive;
    
	//游戏玩家人数
    int nGamePlayerCount;
    
	//游戏资源下载子路径
    std::string strGameResPath;
    
    _stGameConfig()
    {
        strGamename = "";
        nGameKindId = -1;
        bGameActive = false;
        nGamePlayerCount = 0;
        strGameResPath = "";
    }
}ST_GAME_CONFIG, *PST_GAME_CONFIG;

class GameConfigMgr
{
    CREATE_SINGLETON_MUTEX_CLEAR(GameConfigMgr,s_configInstance,nullptr);

    void init();
    void clear();
    
public:
    //加载游戏列表配置文件
    void loadConfig();
    
    //获取游戏列表游戏数目
    int getGameCount();
    
    //获取游戏配置信息
    _stGameConfig getGameConfigByIdx(const int &nIdx, bool &bHave);
    
    //获取游戏配置信息
    _stGameConfig getGameConfigByKind(const int &nKind, bool &bHave);
    
    //根据kindid判断游戏是否被激活
    bool isGameActive(const int &nKind);
    
    //根据kindid判断游戏是否在列表中
    bool isGameInList(const int &nKind);

private:
    static GameConfigMgr *s_configInstance;
    
    std::vector<_stGameConfig> m_vecGameConfig;
};

#endif /* GameConfigMgr_hpp */
