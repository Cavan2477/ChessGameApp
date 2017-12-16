//
//  GameMessage.cpp
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#include "GameMessage.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "lkpy_GameDataMgr.h"
#include "Fish.h"
#include "GameLayer.h"
USING_LKPY_NAMESPACE;
GameMessage::GameMessage()
{
    NetworkMgr::getInstance()->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(GameMessage::GameMessageResult, this));
    m_layer = nullptr;
}

GameMessage::~GameMessage()
{
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GF_GAME);
}

void GameMessage::GameSceneMessageResult(WORD  wSubCmdID, void* pData, WORD wSize)
{
    
}

void GameMessage::GameMessageResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_S_SYNCHRONOUS:
        {
            //GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
            
            auto presult = (CMD_S_FishFinish *)pData;
            if (presult->nOffSetTime) {
                LONGLONG time = GameDataMgr::getInstance()->getEnterTime();
                GameDataMgr::getInstance()->setEnterTime(time - presult->nOffSetTime);
            }
//            for (auto iter : GameDataMgr::getInstance()->m_waitList) {
//                iter.second->dealproductime();
//            }
        }
            break;
        case SUB_S_FISH_CREATE:
        {
            
            int structSize=sizeof(CMD_S_FishCreate);
            if (wSize%structSize != 0) {
                CCLOG("%s", "创建鱼游戏消息数据不匹配");
                return;
            }
            int count=wSize/structSize;
            
            for(int index=0; index<count; ++index)
            {
                auto pdata1 = (char *)pData+index*structSize;
                auto fishdata = (CMD_S_FishCreate *)pdata1;
                //if(fishdata->nFishType!=8)
                  //  return;
                //if(GameDataMgr::getInstance()->m_fishList.size()>50 && fishdata->nFishType !=FishType_BaoXiang)
                    //continue;
                auto pfish = Fish::create(fishdata);
                pfish->dealproductime();
                
                if(fishdata->nFishState != FishState_King || (fishdata->nFishState == FishState_King
                                                              &&GameDataMgr::getInstance()->m_fishKingList.size() <= 0))
                    
                {
                    if(fishdata->nFishState == FishState_King && GameDataMgr::getInstance()->m_fishKingList.size() >= 1)
                        log("forbid fish king");
                    if(fishdata->nFishState == FishState_King && GameDataMgr::getInstance()->m_fishKingList.size() <= 0)
                        GameDataMgr::getInstance()->m_fishKingList.insert(fishdata->nFishKey, pfish);
                    GameDataMgr::getInstance()->m_waitList.insert(fishdata->nFishKey, pfish);
                }
                
            }
        }
            break;
//        case SUB_S_FISH_FINISH:
//        {
//            auto presult = (CMD_S_FishFinish *)pData;
//            if (presult->nOffSetTime) {
//                LONGLONG time = GameDataMgr::getInstance()->getEnterTime();
//                GameDataMgr::getInstance()->setEnterTime(time - presult->nOffSetTime);
//            }
//            for (auto iter : GameDataMgr::getInstance()->m_waitList) {
//                iter.second->dealproductime();
//            }
//        }
            break;
        case SUB_S_FIRE:
        {
            auto presult = (CMD_S_Fire *)pData;
            if (m_layer) {
                m_layer->messageFire(presult);
            }
        }
            break;
        case SUB_S_FISH_CATCH:
        {
            if (m_layer) {
                m_layer->messageCatchFish(pData,wSize);
            }
        }
            break;
        case SUB_S_MULTIPLE:
        {
            if (m_layer) {
                m_layer->messageMULTIPLE(pData,wSize);
            }
        }
            break;
        case SUB_S_EXCHANGE_SCENE:
        {
            if (m_layer) {
                auto presult = (CMD_S_ExchangeScene *)pData;
                m_layer->messageExchangeScene(presult->cbBackIndex);
            }
        }
            break;
        case SUB_S_SUPPLY_TIP:
        {
            if (m_layer) {
                auto presult = (CMD_S_SupplyTip *)pData;
                m_layer->messageSupplyTip(presult->wChairID);
            }
        }
            break;
        case SUB_S_SUPPLY:
        {
            if (m_layer) {
                m_layer->messageSupply(pData,wSize);
            }
        }
            break;
        case SUB_S_SPEECH:
            if(m_layer)
            {
                m_layer->messageSpeech(pData,wSize);
            }
            break;
        case SUB_S_AWARD_TIP:
        {
            if (m_layer) {
                m_layer->messageAwardTip(pData,wSize);
            }
        }
            break;
        case SUB_S_BEGIN_LASER:
        {
//            if (m_layer) {
//                m_layer->messageBeginLaster(int charID)
//            }
        }
            break;
        case SUB_S_LASER:
        {
            auto presult = (CMD_S_Laser *)pData;
            if (m_layer) {
                m_layer->messageLaster(presult);
            }
        }
            break;
        case SUB_S_UPDATE_GAME:
        {
            auto presult = (CMD_S_UpdateGame *)pData;
            GameDataMgr::getInstance()->m_sceneData.bUnlimitedRebound = presult->bUnlimitedRebound;
            memcpy(GameDataMgr::getInstance()->m_sceneData.nFishMultiple, presult->nCatchFishMultiple, sizeof(int)*FISH_ALL_COUNT*RAND_INDEX);
            memcpy(GameDataMgr::getInstance()->m_sceneData.nMultipleValue, presult->nMultipleValue, sizeof(int)*MULTIPLE_MAX_INDEX);
        }
            break;
        case SUB_S_STAY_FISH:
        {
            auto presult = (CMD_S_StayFish*)pData;
            if (m_layer) {
                m_layer->messageStayFish(presult);
            }
            
        }
            break;
        default:
            break;
    }
}