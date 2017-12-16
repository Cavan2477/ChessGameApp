//
//  GameRecordMgr.cpp
//  BaiJiaLe
//
//  Created by zhong on 3/8/16.
//
//

#include "GameRecordMgr.h"
#include "CocosHeader.h"

USING_BJL_NAMESPACE;
GameRecordMgr* GameRecordMgr::m_gameRecordInstance = nullptr;

void GameRecordMgr::init()
{
    
}

void GameRecordMgr::addGameRecord(const tagGameRecord &rec)
{
    m_vecRecord.push_back(rec);
    
    //路单
    if (m_vecWallBill.empty())
    {
        tagGameWallBill bill = tagGameWallBill();
        bill.m_cbIndex = 0;
        bill.m_pRecords[0] = rec.m_cbGameResult;
        m_vecWallBill.push_back(bill);
    }
    else
    {
        bool bFound = false;
        bool bHaveWinList = false;
        int nBeginIdx = 0;
        int nCount = 0;
        
        int nIdx = 0;
        do
        {
            nIdx = this->lastWinListIdx(nBeginIdx, bFound);
            
            if (bFound)
            {
                tagGameWallBill &bill = m_vecWallBill[nIdx];
                /*
                
                */
                if (0 == bill.m_cbIndex)
                {
                    //连赢的情况下，当第六行为平局，判断前面记录是否是连赢情况
                    if (AREA_PING == bill.m_pRecords[5]
                        && seekWinList(m_vecRecord.size() - 2, rec.m_cbGameResult))
                    {
                        //新加一列，继续连赢
                        tagGameWallBill newBill = tagGameWallBill();
                        newBill.m_bWinList = true;
                        newBill.m_cbIndex = 0;
                        newBill.m_pRecords[5] = rec.m_cbGameResult;
                        m_vecWallBill.push_back(newBill);
                        
                        bHaveWinList = true;
                        break;
                    }
                    
                    if ((bill.m_pRecords[5] == rec.m_cbGameResult && seekWinList(m_vecRecord.size() - 2, rec.m_cbGameResult))
                        || AREA_PING == rec.m_cbGameResult)
                    {
                        //新加一列，继续连赢
                        tagGameWallBill newBill = tagGameWallBill();
                        newBill.m_bWinList = true;
                        newBill.m_cbIndex = 0;
                        newBill.m_pRecords[5] = rec.m_cbGameResult;
                        m_vecWallBill.push_back(newBill);
                        
                        bHaveWinList = true;
                        break;
                    }
                    else
                    {
                        bill.m_pRecords[bill.m_cbIndex] = rec.m_cbGameResult;
                        
                        ++bill.m_cbIndex;
                        bill.m_cbIndexWithoutPing = 0;
                        
                        bHaveWinList = true;
                        break;
                    }                    
                }
                else if (5 == bill.m_cbIndex)
                {
                    bill.m_bJumpIdx = true;
                }
                else
                {
                    if (bill.m_pRecords[bill.m_cbIndexWithoutPing] == rec.m_cbGameResult
                        || AREA_PING == rec.m_cbGameResult)
                    {
                        //大于六行，新加一列
                        if (bill.m_cbIndex == 5)
                        {
                            bill.m_cbIndex = 5;
                            bill.m_bWinList = true;
                            bill.m_bJumpIdx = true;
                            if (AREA_PING != rec.m_cbGameResult)
                            {
                                bill.m_cbIndexWithoutPing = bill.m_cbIndex;
                            }
                            
                            tagGameWallBill newBill = tagGameWallBill();
                            newBill.m_bWinList = true;
                            newBill.m_cbIndex = 0;
                            newBill.m_pRecords[5] = rec.m_cbGameResult;
                            m_vecWallBill.push_back(newBill);
                            
                            bHaveWinList = true;
                            break;
                        }
                        else
                        {
                            bill.m_pRecords[bill.m_cbIndex] = rec.m_cbGameResult;
                            
                            if (AREA_PING != rec.m_cbGameResult)
                            {
                                bill.m_cbIndexWithoutPing = bill.m_cbIndex;
                            }
                            ++bill.m_cbIndex;
                            bHaveWinList = true;
                            break;
                        }
                    }
                    else
                    {
                        bill.m_bJumpIdx = true;
                    }
                }
                nBeginIdx = nIdx;
            }
            else
            {
                break;
            }
            
            ++nCount;
        } while (nCount < m_vecWallBill.size());
        
        if (!bHaveWinList) //没有连赢记录
        {
            tagGameWallBill &bill = m_vecWallBill[m_vecWallBill.size() - 1];
            //与上一局结果相同 或者为平局
            if (bill.m_pRecords[bill.m_cbIndexWithoutPing] == rec.m_cbGameResult
                || AREA_PING == rec.m_cbGameResult)
            {
                ++bill.m_cbIndex;
                if (AREA_PING != rec.m_cbGameResult)
                {
                    bill.m_cbIndexWithoutPing = bill.m_cbIndex;
                }
                
                //大于六行，新加一列
                if (bill.m_cbIndex > 5)
                {
                    bill.m_cbIndex = 5;                    
                    if (AREA_PING != rec.m_cbGameResult)
                    {
                        bill.m_cbIndexWithoutPing = bill.m_cbIndex;
                    }
                    
                    tagGameWallBill newBill = tagGameWallBill();
                    newBill.m_bWinList = true;
                    newBill.m_cbIndex = 0;
                    newBill.m_pRecords[5] = rec.m_cbGameResult;
                    m_vecWallBill.push_back(newBill);
                }
                else
                {
                    bill.m_pRecords[bill.m_cbIndex] = rec.m_cbGameResult;
                }
                
                if (5 == bill.m_cbIndex)
                {
                    bill.m_bWinList = true;
                }
            }
            else //与上一局结果不一致
            {
                //新加一列
                tagGameWallBill newBill = tagGameWallBill();
                newBill.m_cbIndex = 0;
                newBill.m_pRecords[0] = rec.m_cbGameResult;
                m_vecWallBill.push_back(newBill);
            }
        }
    }
}

std::vector<tagGameRecord> GameRecordMgr::getRecords()
{
    return m_vecRecord;
}

std::vector<tagGameWallBill> GameRecordMgr::getWallBills()
{
    return m_vecWallBill;
}

void GameRecordMgr::clearRecord()
{
    for (size_t i = 0; i < m_vecRecord.size(); ++i)
    {
        delete m_vecRecord[i].m_pServerRecord;
        m_vecRecord[i].m_pServerRecord = nullptr;
    }
    std::vector<tagGameRecord>().swap(m_vecRecord);
    m_vecRecord.reserve(64);
    
    std::vector<tagGameWallBill>().swap(m_vecWallBill);
    m_vecWallBill.reserve(64);
}

bool GameRecordMgr::seekWinList(const int &endIdx, const BYTE &cbResult)
{
    if (AREA_PING == cbResult)
    {
        return true;
    }
    for (int i = endIdx; i >= 0; --i)
    {
        tagGameRecord rec = m_vecRecord[i];
        //找到相同的即连赢
        if (cbResult == rec.m_cbGameResult)
        {
            return true;
        }
        //找到一个不相同的就是非连赢
        if (cbResult != rec.m_cbGameResult && AREA_PING != rec.m_cbGameResult)
        {
            return false;
        }
    }
    return false;
}

int GameRecordMgr::lastWinListIdx(const int &nBegin, bool &bFound)
{
    int idx = 0;
    bFound = false;
    for (int i = nBegin; i < m_vecWallBill.size(); ++i)
    {
        tagGameWallBill bill = m_vecWallBill[i];
        if (bill.m_bWinList && !bill.m_bJumpIdx)
        {
            idx = i;
            bFound = true;
            break;
        }
    }
    
    return idx;
}