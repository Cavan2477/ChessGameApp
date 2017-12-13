#ifndef __GameDataMgr_h__
#define __GameDataMgr_h__

#pragma once

#include <stdio.h>
#include <string>
#include <concrt.h>
#include "Pattern/Singleton.h"

using namespace std;

//socket通信参数
typedef struct tagSocketParam
{
	string m_sUrl;

	int m_nPort;

	tagSocketParam()
	{
		m_sUrl = "";
		m_nPort = 0;
	}
}ST_TAG_SOCKET_PARAM, *PST_TAG_SOCKET_PARAM;

//发送网络数据参数
typedef struct tagNetParam
{
	int m_nMainCode;
	int m_nSubCode;
	void *m_pData;
	int m_nDataLen;

	tagSocketParam m_socketParam;

	tagNetParam()
	{
		m_nMainCode = 0;
		m_nSubCode = 0;
		m_pData = nullptr;
		m_nDataLen = 0;
		m_socketParam = tagSocketParam();
	}
}ST_TAG_NET_PARAM, *PST_TAG_NET_PARAM;

typedef struct tagChatRecord
{
	DWORD           dwChatUserId;
	std::string     strChat;
	bool            bBrow;
	int             nIdx;
}ST_TAG_CHAT_RECORD, *PST_TAG_CHAT_RECORD;

class GameDataMgr
{
	CREATE_SINGLETON_INIT(GameDataMgr, m_sInstance, nullptr);
	void init();

public:
	void sendData(const tagNetParam &param);
	void sendCurrentRoomData(tagNetParam &param);
	void clear();

	//CC_SYNTHESIZE(int, m_nRoomPort, RoomPort);
	//CC_SYNTHESIZE(std::string, m_sRoomURL, RoomURL);

protected:

private:


public:
	
private:
	static GameDataMgr* m_sInstance;

	tagNetParam m_tagNetParam;
};

#endif // __GameDataMgr_h__
