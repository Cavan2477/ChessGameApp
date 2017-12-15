//
//  GameConfigMgr.cpp
//  GameProject
//
//  Created by zhong on 5/9/16.
//
//

#include "GameConfigMgr.h"
#include "json/document.h"

GameConfigMgr* GameConfigMgr::s_configInstance = nullptr;

void GameConfigMgr::init()
{
    std::vector<_stGameConfig>().swap(m_vecGameConfig);
    m_vecGameConfig.reserve(10);
}

void GameConfigMgr::clear()
{
    std::vector<_stGameConfig>().swap(m_vecGameConfig);
}

void GameConfigMgr::loadConfig()
{
    std::vector<_stGameConfig>().swap(m_vecGameConfig);
    
    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile("public_res/game_config.json");
    rapidjson::Document doc;
    doc.Parse<0>(str.c_str());
    
    if (doc.HasParseError() || !doc.IsArray())
    {
        return;
    }
    
    //读取游戏配置信息
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
    {
        const rapidjson::Value &gameObject = doc[i];
        _stGameConfig config = _stGameConfig();
        
        //游戏名
        if (gameObject.HasMember("name") && gameObject["name"].IsString())
        {
            config.strGamename = gameObject["name"].GetString();
        }
        
        //kind id
        if (gameObject.HasMember("kind") && gameObject["kind"].IsInt())
        {
            config.nGameKindId = gameObject["kind"].GetInt();
        }
        
        //是否激活
        if (gameObject.HasMember("active") && gameObject["active"].IsBool())
        {
            config.bGameActive = gameObject["active"].GetBool();
        }
        
        //是否添加到游戏列表
        if (gameObject.HasMember("addtolist") && gameObject["addtolist"].IsBool())
        {
            if (false == gameObject["addtolist"].GetBool())
            {
                continue;
            }
        }
        else
        {
            continue;
        }
        
        //玩家数量
        if (gameObject.HasMember("player") && gameObject["player"].IsInt())
        {
            config.nGamePlayerCount = gameObject["player"].GetInt();
        }
        
        //下载子路径
        if (gameObject.HasMember("subpath") && gameObject["subpath"].IsString())
        {
            config.strGameResPath = gameObject["subpath"].GetString();
        }
        
        //注释
        if (gameObject.HasMember("comment") && gameObject["comment"].IsString())
        {
            std::string str = gameObject["comment"].GetString();
            cocos2d::log("game name:%s, kind:%d, comment:%s", config.strGamename.c_str(),config.nGameKindId, str.c_str());
        }
        
        m_vecGameConfig.push_back(config);
    }

    cocos2d::log("游戏总数: %d", (int)m_vecGameConfig.size());
}

int GameConfigMgr::getGameCount()
{
    return (int)m_vecGameConfig.size();
}

_stGameConfig GameConfigMgr::getGameConfigByIdx(const int &nIdx, bool &bHave)
{
    bHave = false;
    if (nIdx >=0 && nIdx < m_vecGameConfig.size())
    {
        bHave = true;
        return m_vecGameConfig[nIdx];
    }
    return _stGameConfig();
}

_stGameConfig GameConfigMgr::getGameConfigByKind(const int &nKind, bool &bHave)
{
    bHave = false;
    for (int i = 0; i < m_vecGameConfig.size(); ++i)
    {
        if (m_vecGameConfig[i].nGameKindId == nKind)
        {
            bHave = true;
            return m_vecGameConfig[i];
        }
    }
    return _stGameConfig();
}

bool GameConfigMgr::isGameActive(const int &nKind)
{
    for (int i = 0; i < m_vecGameConfig.size(); ++i)
    {
        if (m_vecGameConfig[i].nGameKindId == nKind)
        {
            return m_vecGameConfig[i].bGameActive;
        }
    }
    return false;
}

bool GameConfigMgr::isGameInList(const int &nKind)
{
    for (int i = 0; i < m_vecGameConfig.size(); ++i)
    {
        if (m_vecGameConfig[i].nGameKindId == nKind)
            return true;
    }

    return false;
}