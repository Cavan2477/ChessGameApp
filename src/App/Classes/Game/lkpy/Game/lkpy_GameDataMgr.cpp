//
//  GameDataMgr.cpp
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#include "lkpy_GameDataMgr.h"
#include "Project.h"
#include "GameMessage.h"
using namespace tinyxml2;
USING_NS_CC;
USING_LKPY_NAMESPACE;
static GameDataMgr *s_SharedGameData = NULL;

GameDataMgr::GameDataMgr()
:m_reversal(false)
,m_autoshoot(false)
,m_autolock(false)
,m_fishIndex(INT_MAX)
,m_netcolorindex(0)
,m_FishArray(NULL)
{
    m_gameMessage = nullptr;
    //this->readBodyPlist("game_res/fish_0.plist");
//    this->readBodyPlist("game_res/fish_1.plist");
//    this->readBodyPlist("game_res/fish_2.plist");
    this->readBodyPlist("game_res/fish_bodies.plist");
    this->readBodyPlist("game_res/bullet_bodies.plist");
    m_FishArray = Array::create();
    m_FishArray->retain();
}

GameDataMgr::~GameDataMgr()
{
    this->clearGameData();
    m_FishArray->release();
    m_FishArray = NULL;
}

GameDataMgr * GameDataMgr::getInstance()
{
    if (s_SharedGameData == NULL) {
        s_SharedGameData = new GameDataMgr();
    }
    return s_SharedGameData;
}

void GameDataMgr::clearGameData()
{
    m_fishList.clear();
    m_waitList.clear();
    m_fishKingList.clear();
    m_autolock = false;
    m_autoshoot = false;
    m_reversal = false;
    CC_SAFE_RELEASE(m_gameMessage);
    m_gameMessage = nullptr;
}

void GameDataMgr::initgameMessage()
{
    m_gameMessage = new GameMessage;
}

void GameDataMgr::onGameScene(void *pData, WORD wSize)
{
    NetworkMgr::getInstance()->m_GameScenecallback = nullptr;
    if(lkpy_game::GameDataMgr::getInstance()->m_gameMessage)
        return;
    HallDataMgr::getInstance()->m_bStartGame = true;
    int nsize = sizeof(GameScene);
    //if(wSize!=sizeof(GameScene)) return;
    auto presult = (GameScene *)pData;
    lkpy_game::GameDataMgr::getInstance()->m_sceneData = *presult;
    int multipeArray[6] = {1,10,100,500,1000,5000};
    for (int i = 0; i < 6; i++) {
        lkpy_game::GameDataMgr::getInstance()->m_sceneData.nMultipleValue[i] = multipeArray[i];
    }
    GameScene gs = lkpy_game::GameDataMgr::getInstance()->m_sceneData;
    
    lkpy_game::GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
    lkpy_game::GameDataMgr::getInstance()->initgameMessage();
    lkpy_game::GameDataMgr* gdm = lkpy_game::GameDataMgr::getInstance();
}

cocos2d::Vec2 GameDataMgr::convertCoordinateSystem(const cocos2d::Vec2 &point,int type,bool isconvert)
{
    Vec2 point1 = point;
    if (type == 0) {
        point1.x = point1.x/ScaleX;
        point1.y = Win32Size.y - point1.y/ScaleY;
        if (isconvert == true) {
            point1.x = Win32Size.x - point1.x;
            point1.y = Win32Size.y - point1.y;
        }
        return  point1;
    }
    else if (type == 1)
    {
        point1.x = point.x *ScaleX;
        point1.y = WinSize.height - point.y*ScaleY;
        if (isconvert == true) {
            point1.x = WinSize.width - point1.x;
            point1.y = WinSize.height- point1.y;
        }
        return point1;
    }
    if (type == 2) {
        point1.x = point1.x/ScaleX;
        
        if (isconvert == true) {
             point1.y = Win32Size.y - point1.y/ScaleY;
            point1.x = Win32Size.x - point1.x;
            //point1.y = Win32Size.y - point1.y;
        }
        else{
            //point1.y = Win32Size.y - point1.y/ScaleY;
        }
        
        return  point1;
    }
    return Vec2::ZERO;
}

float GameDataMgr::getAngleByTwoPoint(const cocos2d::Vec2 &point1,const cocos2d::Vec2 &point2)
{
    Vec2 vec2 = point1 - point2;
    float angle = 90 - CC_RADIANS_TO_DEGREES(vec2.getAngle());
    return angle;
    
    
    double len_y = point1.y - point2.y;
    double len_x = point1.x - point2.x;
    //float angle = 0;
    
    if (len_x == 0.f) {
        if (len_y < 0) {
            angle = 180;
        }
    }
    if (len_y == 0) {
        if (len_x > 0) {
            angle = 90;
        }
        else if(len_x < 0)
        {
            angle = -90;
        }
    }
    double tan_yx =  fabs(len_y)/fabs(len_x);
    if(len_y > 0 && len_x < 0) {
        angle = atan(tan_yx)*180/M_PI - 90;
    } else if (len_y > 0 && len_x > 0) {
        angle = 90 - atan(tan_yx)*180/M_PI;
    } else if(len_y < 0 && len_x < 0) {
        angle = -atan(tan_yx)*180/M_PI - 90;
    } else if(len_y < 0 && len_x > 0) {
        angle = atan(tan_yx)*180/M_PI + 90;
    }
    return  angle;
}

void GameDataMgr::readBodyPlist(const std::string &filename)
{
    auto pdoc = new XMLDocument;
//    ssize_t bufferSize = 0;
//    std::string inFullpath = FileUtils::getInstance()->fullPathForFilename(filename).c_str();
//    
//    if (!FileUtils::getInstance()->isFileExist(inFullpath))
//    {
//        log("%s","刚体数据不存在");
//    }
//    std::string content =(char*)FileUtils::getInstance()->getFileData(inFullpath, "r", &bufferSize);
   // auto file = FileUtils::getInstance()->fullPathForFilename(filename);
    tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument;
    unsigned char* pBuffer=NULL;
    ssize_t bufferSize=0;
    int i =0;
    pBuffer=CCFileUtils::sharedFileUtils()->getFileData(filename, "r", &bufferSize);
    if(pBuffer){
        pdoc->Parse((const char*)pBuffer);
    }else{

        CC_SAFE_DELETE(pDoc);
        CC_SAFE_DELETE(pBuffer);
        return;
    }
    // auto errorID = pdoc->LoadFile(file.c_str());
    // if (errorID != 0) {
    //     log("fileName %s errorID %d",file.c_str(),errorID);
    //     return;
    // }
    auto rootele = pdoc->RootElement();
    
    auto dicele = rootele->FirstChildElement("dict");
    auto keyele = dicele->FirstChildElement("key");
    
    auto bodyeld = keyele->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
    log("%s", keyele->GetText());
    
    auto pbody = bodyeld->FirstChildElement("key");
    auto pbodydic = pbody->NextSiblingElement();
    while (pbody) {
        this->readBodydata(pbody, pbodydic);
        pbody = pbodydic->NextSiblingElement();
        if (pbody) {
            pbodydic = pbody->NextSiblingElement();
        }
    }
}

void GameDataMgr::readBodydata(tinyxml2::XMLElement *pname, tinyxml2::XMLElement *pdata)
{
    log("%s", pname->GetText());
    auto namestr = pname->GetText();

//    auto pDict = pdata->FirstChildElement("array")->FirstChildElement("dict");
//    std::vector<std::vector<cocos2d::Vec2>> vec2data;
//    while (pDict)
//    {
//        
//        auto parrayT = pDict->FirstChildElement("array");
//        
//        while (parrayT)
//        {
//            auto parray = parrayT->FirstChildElement("array");
//            while (parray)
//            {
//                std::vector<Vec2> vec2data1;
//                auto pstring = parray->FirstChildElement("string");
//                while (pstring) {
//                    Vec2 vec2;
//                    //log("%s",pstring->GetText());
//                    sscanf(pstring->GetText(), "{ %f,%f }", &vec2.x, &vec2.y);
//                    vec2data1.push_back(vec2);
//                    pstring = pstring->NextSiblingElement();
//                }
//                vec2data.push_back(vec2data1);
//                parray = parray->NextSiblingElement();
//            }
//
//            parrayT = parrayT->NextSiblingElement();
//        }
//        
//        pDict = pDict->NextSiblingElement();
//    }
//    m_bodylist.insert(std::make_pair(namestr, vec2data));
    
    auto parray = pdata->FirstChildElement("array")->FirstChildElement("dict")->FirstChildElement("array")->FirstChildElement("array");
    std::vector<std::vector<cocos2d::Vec2>> vec2data;
    while (parray) {
        std::vector<Vec2> vec2data1;
        auto pstring = parray->FirstChildElement("string");
        while (pstring) {
            Vec2 vec2;
            //log("%s { %f,%f }",pstring->GetText(), &vec2.x, &vec2.y);
            sscanf(pstring->GetText(), "{ %f,%f }", &vec2.x, &vec2.y);
            log("{ %f,%f }",vec2.x,vec2.y);
            vec2data1.push_back(vec2);
            pstring = pstring->NextSiblingElement();
        }
        vec2data.push_back(vec2data1);
        parray = parray->NextSiblingElement();
    }
    m_bodylist.insert(std::make_pair(namestr, vec2data));

}

PhysicsBody * GameDataMgr::getBodyByType(int type)
{
    //auto pname = __String::createWithFormat("fish_%d_yd_0", type)->getCString();
    auto* pname = __String::createWithFormat("fishMove_%03d_1", type+1)->getCString();
    CCLOG("fish body %s",pname);
    return this->getBodyByName(pname);
}

PhysicsBody *GameDataMgr::getBodyByName(const std::string &name)
{
    if (m_bodylist.find(name) == m_bodylist.end()) {
        log("%s", "没有该物体数据");
        return NULL;
    }
    auto pbody = PhysicsBody::create();
    auto pdata = m_bodylist.at(name);
    for (auto iter : pdata)
    {
        int length = (int)iter.size();
        auto pbodyshap = PhysicsShapePolygon::create(&iter[0], length);
        pbody->addShape(pbodyshap);
    }
    pbody->setGravityEnable(false);
    return pbody;
}

DWORD GameDataMgr::selectMaxFish()
{
    cocos2d::Vector<Fish *>fishlist;
    int fishtype = 0;
    auto rect = cocos2d::Rect(0, 0, WinSize.width, WinSize.height);
    for (auto iter : m_fishList) {
        if (iter.second->getFishData()->nFishType > fishtype) {
            
            if (rect.containsPoint(iter.second->getPosition())) {
                fishtype = iter.second->getFishData()->nFishType;
                fishlist.clear();
                fishlist.pushBack(iter.second);
            }
        }
        
        if (iter.second->getFishData()->nFishType == fishtype) {
            fishlist.pushBack(iter.second);
        }
    }
    
    auto mid = Vec2(WinSize.width/2, WinSize.height/2);
    float distant = 1000;
    DWORD fishindex = INT_MAX;
    for (auto iter : fishlist) {
        if (iter) {
            auto distant1 = iter->getPosition().distance(mid);
            if (distant1 < distant) {
                distant = distant1;
                fishindex = iter->getFishData()->nFishKey;
            }
        }
    }
    fishlist.clear();
    
    return fishindex;
}

Color3B GameDataMgr::getNetColor()
{
    auto color = Color3B::WHITE;
    switch (m_netcolorindex) {
        case 0:
            color = Color3B::BLUE;
            break;
        case 1:
            color = Color3B::GREEN;
            break;
        case 2:
            color = Color3B::MAGENTA;
            break;
        case 3:
            color = Color3B::RED;
            break;
        case 4:
            color = Color3B::YELLOW;
            break;
            
        default:
            break;
    }
    return color;
}

//void GameDataMgr::pointDeal(tagBezierPoint *pdata)
//{
//    pdata->BeginPoint.y = Win32Size.y - pdata->BeginPoint.y;
//    pdata->EndPoint.y = Win32Size.y - pdata->EndPoint.y;
//    pdata->KeyOne.y = Win32Size.y - pdata->KeyOne.y;
//    pdata->KeyTwo.y = Win32Size.y - pdata->KeyTwo.y;
//    
//    pdata->BeginPoint.x *= ScaleX;
//    pdata->BeginPoint.y *= ScaleY;
//    
//    pdata->EndPoint.x *= ScaleX;
//    pdata->EndPoint.y *= ScaleY;
//    
//    pdata->KeyOne.x *= ScaleX;
//    pdata->KeyOne.y *= ScaleY;
//    
//    pdata->KeyTwo.x *= ScaleX;
//    pdata->KeyTwo.y *= ScaleY;
//}
