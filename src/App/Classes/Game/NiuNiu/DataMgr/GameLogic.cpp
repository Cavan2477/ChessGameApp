#include "GameLogic.h"

USING_NN_NAMESPACE;
//索引变量
const BYTE cbIndexCount=5;

GameLogic* GameLogic::m_gameInstance = nullptr;

void GameLogic::init()
{
    
}

//获取类型
BYTE GameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
    assert(cbCardCount==NN_MAXCOUNT);
    
    BYTE bKingCount=0,bTenCount=0;
    for(BYTE i=0;i<cbCardCount;i++)
    {
        if(GetCardValue(cbCardData[i])>10)
        {
            bKingCount++;
        }
        else if(GetCardValue(cbCardData[i])==10)
        {
            bTenCount++;
        }
    }
    if(bKingCount==NN_MAXCOUNT)
        return OX_FIVEKING;
    else if(bKingCount==NN_MAXCOUNT-1 && bTenCount==1)
        return OX_FOURKING;
    
    BYTE bTemp[NN_MAXCOUNT];
    BYTE bSum=0;
    for (BYTE i=0;i<cbCardCount;i++)
    {
        bTemp[i]=GetCardLogicValue(cbCardData[i]);
        bSum+=bTemp[i];
    }
    
    for (BYTE i=0;i<cbCardCount-1;i++)
    {
        for (BYTE j=i+1;j<cbCardCount;j++)
        {
            if((bSum-bTemp[i]-bTemp[j])%10==0)
            {
                return ((bTemp[i]+bTemp[j])>10)?(bTemp[i]+bTemp[j]-10):(bTemp[i]+bTemp[j]);
            }
        }
    }
    
    return OX_VALUE0;
}

BYTE GameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
    if(cbCardCount!=NN_MAXCOUNT)return 0;
    
    BYTE bTimes=GetCardType(cbCardData,NN_MAXCOUNT);
    if(bTimes<7)return 1;
    else if(bTimes==7)return 2;
    else if(bTimes==8)return 3;
    else if(bTimes==9)return 4;
    else if(bTimes==10)return 5;
    else if(bTimes==OX_THREE_SAME)return 5;
    else if(bTimes==OX_FOUR_SAME)return 5;
    else if(bTimes==OX_FOURKING)return 5;
    else if(bTimes==OX_FIVEKING)return 5;
    
    return 0;
}

//ªÒ»°≈£≈£
bool GameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
    assert(cbCardCount==NN_MAXCOUNT);
    
    //…Ë÷√±‰¡ø
    BYTE bTemp[NN_MAXCOUNT],bTempData[NN_MAXCOUNT];
    memcpy(bTempData,cbCardData,sizeof(bTempData));
    BYTE bSum=0;
    for (BYTE i=0;i<cbCardCount;i++)
    {
        bTemp[i]=GetCardLogicValue(cbCardData[i]);
        bSum+=bTemp[i];
    }
    
    //≤È’“≈£≈£
    for (BYTE i=0;i<cbCardCount-1;i++)
    {
        for (BYTE j=i+1;j<cbCardCount;j++)
        {
            if((bSum-bTemp[i]-bTemp[j])%10==0)
            {
                BYTE bCount=0;
                for (BYTE k=0;k<cbCardCount;k++)
                {
                    if(k!=i && k!=j)
                    {
                        cbCardData[bCount++] = bTempData[k];
                    }
                }assert(bCount==3);
                
                cbCardData[bCount++] = bTempData[i];
                cbCardData[bCount++] = bTempData[j];
                
                return true;
            }
        }
    }
    
    return false;
}

//ªÒ»°’˚ ˝
bool GameLogic::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
    BYTE sum=0;
    for(BYTE i=0;i<cbCardCount;i++)
    {
        sum+=GetCardLogicValue(cbCardData[i]);
    }
    assert(sum>0);
    return (sum%10==0);
}

//≈≈¡–∆ÀøÀ
void GameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
    //◊™ªª ˝÷µ
    BYTE cbLogicValue[NN_MAXCOUNT];
    for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardValue(cbCardData[i]);
    
    //≈≈–Ú≤Ÿ◊˜
    bool bSorted=true;
    BYTE cbTempData,bLast=cbCardCount-1;
    do
    {
        bSorted=true;
        for (BYTE i=0;i<bLast;i++)
        {
            if ((cbLogicValue[i]<cbLogicValue[i+1])||
                ((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
            {
                //ΩªªªŒª÷√
                cbTempData=cbCardData[i];
                cbCardData[i]=cbCardData[i+1];
                cbCardData[i+1]=cbTempData;
                cbTempData=cbLogicValue[i];
                cbLogicValue[i]=cbLogicValue[i+1];
                cbLogicValue[i+1]=cbTempData;
                bSorted=false;
            }
        }
        bLast--;
    } while(bSorted==false);
    
    return;
}

//ªÏ¬“∆ÀøÀ
void GameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
    //CopyMemory(cbCardBuffer,m_cbCardListData,cbBufferCount);
    //ªÏ¬“◊º±∏
    BYTE cbCardData[CountArray(g_cbCardData)];
    memcpy(cbCardData,g_cbCardData,sizeof(g_cbCardData));
    
    //ªÏ¬“∆ÀøÀ
    BYTE bRandCount=0,bPosition=0;
    do
    {
        bPosition=rand()%(CountArray(g_cbCardData)-bRandCount);
        cbCardBuffer[bRandCount++]=cbCardData[bPosition];
        cbCardData[bPosition]=cbCardData[CountArray(g_cbCardData)-bRandCount];
    } while (bRandCount<cbBufferCount);
    
    return;
}

void GameLogic::SearchOutCard(BYTE *cbCardData, BYTE cbCardCount, tagSearchCardResult &tagResult)
{
    if (nullptr == cbCardData || 0 == cbCardCount)
    {
        return;
    }
    
    tagResult = {0};
    tagNiuNiuFilter filter = {0};
    
    FilterNiuNiu(cbCardData, cbCardCount, filter);
    tagResult.cbSearchCount = filter.cbNiuNiuCardCount > 0 ? 1 : 0;
    tagResult.cbCardCount[1] = filter.cbNiuNiuCardCount;
    memcpy(tagResult.cbResultCard[1], filter.cbCardData[0], NN_MAXCOUNT);
    return;
}

void  GameLogic::SearchOutCard( tagCards cards, tagCards &tagResult)
{    
    tagNiuNiuFilter filter = {0};
    FilterNiuNiu(cards.m_cardData, cards.m_cardCount, filter);
    
    if (filter.cbNiuNiuCardCount > 0)
    {
        tagResult.m_cardCount = filter.cbNiuNiuCardCount;
        memcpy(tagResult.m_cardData, filter.cbCardData[0], NN_MAXCOUNT);
    }
    return;
}

void GameLogic::FilterNiuNiu(BYTE *cbCardData, BYTE cbCardCount, tagNiuNiuFilter &tagResult)
{
    memset(&tagResult, 0, sizeof(tagNiuNiuFilter));    
    if (nullptr == cbCardData || 0 == cbCardCount)
    {
        return;
    }
    
    //判断牛牛
    if (!GetOxCard(cbCardData,cbCardCount))
    {
        tagResult.cbNiuNiuCardCount = 0;
        tagResult.cbNormalCount = 5;
        //无牛
        tagResult.cbCardData[1][0] = cbCardData[0];
        tagResult.cbCardData[1][1] = cbCardData[1];
        tagResult.cbCardData[1][2] = cbCardData[2];
        tagResult.cbCardData[1][3] = cbCardData[3];
        tagResult.cbCardData[1][4] = cbCardData[4];
        return;
    }
    tagResult.cbNiuNiuCardCount = 3;
    tagResult.cbNormalCount = 2;
    //三张
    tagResult.cbCardData[0][0] = cbCardData[0];
    tagResult.cbCardData[0][1] = cbCardData[1];
    tagResult.cbCardData[0][2] = cbCardData[2];
    //两张
    tagResult.cbCardData[1][0] = cbCardData[3];
    tagResult.cbCardData[1][1] = cbCardData[4];
    
    return;
    
    /*
    BYTE tmpCard[MAX_COUNT] = {0};
    memcpy(tmpCard, cbCardData, MAX_COUNT);
    
    //排序
    SortCardList(tmpCard, cbCardCount);
    
    //判断有三张大于10的牌
    for (BYTE i = 0; i < cbCardCount; ++i)
    {
        for (BYTE j = i + 1; j < cbCardCount; ++j)
        {
            for (BYTE k = j + 1; k < cbCardCount; ++k)
            {
                BYTE logicI = GetCardLogicValue(tmpCard[i]);
                BYTE logicJ = GetCardLogicValue(tmpCard[j]);
                BYTE logicK = GetCardLogicValue(tmpCard[k]);
                
                if (logicI >= 10 && logicJ >= 10 && logicK >= 10 )
                {
                    tagResult.cbNiuNiuCardCount = 3;
                    tagResult.cbNormalCount = 2;
                    //牛牛牌
                    tagResult.cbCardData[0][0] = tmpCard[i];
                    tagResult.cbCardData[0][1] = tmpCard[j];
                    tagResult.cbCardData[0][2] = tmpCard[k];
                    
                    //非牛牛牌
                    tmpCard[i] = 0;
                    tmpCard[j] = 0;
                    tmpCard[k] = 0;
                    BYTE idx = 0;
                    for (BYTE l = 0; l < cbCardCount; ++l)
                    {
                        if (tmpCard[l] != 0)
                        {
                            tagResult.cbCardData[1][idx] = tmpCard[l];
                            ++idx;
                        }
                    }
                    return;
                }
            }
        }
    }
    
    //判断三张相加为10的倍数的牌
    for (BYTE i = 0; i < cbCardCount; ++i)
    {
        for (BYTE j = i + 1; j < cbCardCount; ++j)
        {
            for (BYTE k = j + 1; k < cbCardCount; ++k)
            {
                BYTE logicI = GetCardLogicValue(tmpCard[i]);
                BYTE logicJ = GetCardLogicValue(tmpCard[j]);
                BYTE logicK = GetCardLogicValue(tmpCard[k]);
                
                logicI = logicI > 10 ? 10 : logicI;
                logicJ = logicJ > 10 ? 10 : logicJ;
                logicK = logicK > 10 ? 10 : logicK;
                
                BYTE add = logicI + logicJ + logicK;
                if (0 == add % 10)
                {
                    tagResult.cbNiuNiuCardCount = 3;
                    tagResult.cbNormalCount = 2;
                    //牛牛牌
                    tagResult.cbCardData[0][0] = tmpCard[i];
                    tagResult.cbCardData[0][1] = tmpCard[j];
                    tagResult.cbCardData[0][2] = tmpCard[k];
                    
                    //非牛牛牌
                    tmpCard[i] = 0;
                    tmpCard[j] = 0;
                    tmpCard[k] = 0;
                    BYTE idx = 0;
                    for (BYTE l = 0; l < cbCardCount; ++l)
                    {
                        if (tmpCard[l] != 0)
                        {
                            tagResult.cbCardData[1][idx] = tmpCard[l];
                            ++idx;
                        }
                    }
                    return;
                }
            }
        }
    }
    
    
    return;
     */
}

//牛牛的牌的逻辑值，大于10返回10
BYTE GameLogic::GetCardLogicValue(BYTE cbCardData)
{
    //∆ÀøÀ Ù–‘
    //BYTE bCardColor=GetCardColor(cbCardData);
    BYTE bCardValue=GetCardValue(cbCardData);
    
    //◊™ªª ˝÷µ
    return (bCardValue>10)?(10):bCardValue;
}

//∂‘±»∆ÀøÀ
bool GameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,bool FirstOX,bool NextOX)
{
    if(FirstOX!=NextOX)return (FirstOX>NextOX);
    
    //±»Ωœ≈£¥Û–°
    if(FirstOX==true)
    {
        //ªÒ»°µ„ ˝
        BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
        BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);
        
        //µ„ ˝≈–∂œ
        if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);
        
        switch(cbNextType)
        {
            case OX_FOUR_SAME:		//’®µØ≈∆–Õ
            case OX_THREE_SAME:		//∫˘¬´≈∆–Õ
            {
                //≈≈–Ú¥Û–°
                BYTE bFirstTemp[NN_MAXCOUNT],bNextTemp[NN_MAXCOUNT];
                memcpy(bFirstTemp,cbFirstData,cbCardCount);
                memcpy(bNextTemp,cbNextData,cbCardCount);
                SortCardList(bFirstTemp,cbCardCount);
                SortCardList(bNextTemp,cbCardCount);
                
                return GetCardValue(bFirstTemp[NN_MAXCOUNT/2])>GetCardValue(bNextTemp[NN_MAXCOUNT/2]);
                
                break;
            }
        }
    }
    
    //≈≈–Ú¥Û–°
    BYTE bFirstTemp[NN_MAXCOUNT],bNextTemp[NN_MAXCOUNT];
    memcpy(bFirstTemp,cbFirstData,cbCardCount);
    memcpy(bNextTemp,cbNextData,cbCardCount);
    SortCardList(bFirstTemp,cbCardCount);
    SortCardList(bNextTemp,cbCardCount);
    
    //±»Ωœ ˝÷µ
    BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
    BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
    if(cbNextMaxValue!=cbFirstMaxValue)return cbFirstMaxValue>cbNextMaxValue;
    
    //±»Ωœ—’…´
    return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);
    
    return false;
}