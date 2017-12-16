//
//  GameRecordMgr.hpp
//  BaiJiaLe
//
//  Created by zhong on 3/8/16.
//
//

#ifndef GameRecordMgr_hpp
#define GameRecordMgr_hpp

#include <stdio.h>
#include "Singleton_Z.h"
#include "GameDefine.h"
#include "BaiJiaLeLogicDefine.h"
#include "CocosHeader.h"


BJL_NAMESPACE_BEGIN

class GameRecordMgr
{
    CREATE_SINGLETON_MUTEX(GameRecordMgr,m_gameRecordInstance,nullptr);
    void init();
public:
    //路单
    void addGameRecord(const tagGameRecord &rec);
    
    std::vector<tagGameRecord> getRecords();
    
    std::vector<tagGameWallBill> getWallBills();
    
    void clearRecord();
    
private:
    /*
     * @brief 在连赢的情况下，从指定索引开始，根据给定输赢结果寻找最近的上一局符合连赢条件的记录
     */
    bool seekWinList(const int &endIdx, const BYTE &cbResult);
    
    /*
     * @brief 找到最后一个连赢记录下标
     */
    int lastWinListIdx(const int &nBegin, bool &bFound);
private:
    static GameRecordMgr* m_gameRecordInstance;
    
    //简易路单记录管理
    std::vector<tagGameRecord> m_vecRecord;
    //路单系统
    std::vector<tagGameWallBill> m_vecWallBill;
};

BJL_NAMESPACE_END
#endif /* GameRecordMgr_hpp */
