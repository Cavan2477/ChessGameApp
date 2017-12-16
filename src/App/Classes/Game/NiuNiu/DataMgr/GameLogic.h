#ifndef __GameLogic_H__
#define __GameLogic_H__

#include "CocosHeader.h"
#include "NiuNiuLogicDefine.h"
#include "GameDefine.h"
#include "Singleton_Z.h"
#include <mutex>
#include "define.h"

NN_NAMESPACE_BEGIN
#ifdef WIN32
#else
#ifndef CopyMemory
#define CopyMemory memcpy
#endif
#endif // WIN32
class GameLogic
{
	CREATE_SINGLETON_MUTEX(GameLogic,m_gameInstance,nullptr);
	void init();
public:
    //ªÒ»°¿‡–Õ
    BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
    //ªÒ»° ˝÷µ
    BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
    //ªÒ»°ª®…´
    BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
    //ªÒ»°±∂ ˝
    BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
    //ªÒ»°≈£≈£
    bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
    //ªÒ»°’˚ ˝
    bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);
    //øÿ÷∆∫Ø ˝
public:
    //≈≈¡–∆ÀøÀ
    void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
    //ªÏ¬“∆ÀøÀ
    void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
    
    //牛牛，搜索提示牌
    void SearchOutCard(BYTE cbCardData[], BYTE cbCardCount, tagSearchCardResult &tagResult);
    
    //牛牛，搜索提示牌
    void SearchOutCard( tagCards cards,tagCards &tagResult);
    
    //牛牛牌过滤(TODO)
    void FilterNiuNiu(BYTE cbCardData[], BYTE cbCardCount, tagNiuNiuFilter &tagResult);
    //π¶ƒ‹∫Ø ˝
public:
    //¬ﬂº≠ ˝÷µ
    BYTE GetCardLogicValue(BYTE cbCardData);
    //∂‘±»∆ÀøÀ
    bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,bool FirstOX,bool NextOX);
private:
	static GameLogic* m_gameInstance;
};
NN_NAMESPACE_END
#endif