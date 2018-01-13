/************************************************************************************
 * file: 		GameDataMgr.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 21:08:06
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#include "GameDataMgr.h"
#include <string>
#include "HallDataMgr.h"
#include "NetworkMgr.h"
//#include "GameLogic.h"

using namespace std;

GameDataMgr* GameDataMgr::m_gameDataInstance = nullptr;

void GameDataMgr::init()
{
	/*
	m_pRoomMission = nullptr;
	m_pRoomSocekt = nullptr;
	*/
	m_bIsGameStart = false;
	m_bExit = false;

	m_bIsMyRound = false;
	m_bIsLander = false;

	m_bChangeDesk = false;
	m_bRoundOver = false;
	m_bEnterNormalRoom = false;
	m_bEnterAntiCheat = false;
	m_nEnterRoom = -1;

	//聊天
	std::vector<tagChatRecord>().swap(m_vecChatRecord);
	m_vecChatRecord.reserve(100);

	auto str = cocos2d::FileUtils::getInstance()->getStringFromFile("public_res/chat_text.json");
	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	if (!doc.HasParseError() && doc.IsArray())
	{
		tagChatRecord record = { 0 };
		for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
		{
			const rapidjson::Value& v = doc[i];
			record.bBrow = false;
			record.strChat = v.GetString();
			m_vecStaticChat.push_back(record);
		}
	}
}

void GameDataMgr::sendData(const tagNetParam &param)
{
	NetworkMgr::getInstance()->sendData(param.m_nMainCode, param.m_nSubCode, (char*)param.m_pData, param.m_nDataLen);
}

void GameDataMgr::sendCurrentRoomData(tagNetParam &param)
{
	param.m_socketParam.m_sUrl = getRoomURL();
	param.m_socketParam.m_nPort = getRoomPort();
	sendData(param);
}

void GameDataMgr::clear()
{/*
 if (m_pRoomSocekt)
 {
 delete m_pRoomSocekt;
 m_pRoomSocekt = nullptr;
 }

 if (m_pRoomMission)
 {
 delete m_pRoomMission;
 m_pRoomMission = nullptr;
 }*/
}

void GameDataMgr::addRecord(const tagChatRecord &record)
{
	m_vecChatRecord.push_back(record);
}

bool GameDataMgr::getRecordAt(const size_t &idx, tagChatRecord &record)
{
	if (idx < m_vecChatRecord.size())
	{
		record = m_vecChatRecord[idx];
		return true;
	}

	return false;
}

void GameDataMgr::clearRecord()
{
	std::vector<tagChatRecord>().swap(m_vecChatRecord);
	m_vecChatRecord.reserve(100);
}

bool GameDataMgr::getStaticTextAt(const size_t &idx, tagChatRecord &record)
{
	if (idx < m_vecStaticChat.size())
	{
		record = m_vecStaticChat[idx];
		return true;
	}

	return false;
}

bool GameDataMgr::compareWithStaticText(const std::string &src, size_t &idx)
{
	bool bRes = false;

	for (size_t i = 0; i < m_vecStaticChat.size(); ++i)
	{
		if (0 == strcmp(m_vecStaticChat[i].strChat.c_str(), src.c_str()))
		{
			idx = i;
			bRes = true;

			break;
		}
	}

	return bRes;
}

enGameRoleDir GameDataMgr::getRoleDir(const WORD &otherchair)
{
	//自己
	WORD myChair = HallDataMgr::getInstance()->m_wChairID;
	bool isMine = otherchair == myChair;

	if (isMine)
		return kMY_ROLE;

	int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;

	int right = myChair + 1;					//逆时针
	right = right >= max ? right - max : right;

	int left = myChair - 1;
	left = left < 0 ? left + max : left;

	int top = myChair + 2;
	top = top >= max ? top - max : top;

	if (otherchair == left)
		return kLEFT_ROLE;
	else if (otherchair == right)
		return kRIGHT_ROLE;
	else if (otherchair == top)
		return kTOP_ROLE;
	else
		return kDEFAULT_ROLE;
}

enGameResult GameDataMgr::getWinDir(const LONGLONG &llGameCoin)
{
	enGameResult dir = kDefault;

	if (m_bIsLander)
	{
		if (llGameCoin > 0)
			dir = kLanderWin;
		else if (llGameCoin < 0)
			dir = kLanderLose;
	}
	else
	{
		if (llGameCoin > 0)
		{
			dir = kFarmerWin;
		}
		else if (llGameCoin < 0)
		{
			dir = kFarmerLose;
		}
	}
	return dir;
}

bool GameDataMgr::isValidDir(const enGameRoleDir &dir)
{
	int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;

	for (int i = 0; i < max; ++i)
	{
		if (i == (int)dir)
			return true;
	}

	return false;
}

void GameDataMgr::connectRoom(const ST_GAME_ROOM_SERVER *pstGameRoomServer)
{
	setRoomPort(pstGameRoomServer->wServerPort);

	TCHAR szTemp[32] = {};
	memcpy(szTemp, pstGameRoomServer->szServerAddr, 32);

	std::string ipaddress = WHConverUnicodeToUtf8WithArray((WORD*)szTemp);

	setRoomURL(ipaddress);

	//socket 连接
	NetworkMgr::getInstance()->doConnect(ipaddress.c_str(), pstGameRoomServer->wServerPort, EM_DATA_TYPE_ROOM);
}

void GameDataMgr::enterRoom(const std::string &strGameRoomPwd)
{
	ST_CMD_GR_LOGON_MOBILE stCmdGrLogonMobile = { 0 };

	stCmdGrLogonMobile.cbDeviceType = 0X19;
	stCmdGrLogonMobile.dwProcessVersion = VERSION_MOBILE;
	stCmdGrLogonMobile.wPageTableCount = PAGE_TABLE_COUNT;
	stCmdGrLogonMobile.wGameID = HallDataMgr::getInstance()->m_dwKindID;
	stCmdGrLogonMobile.wBehaviorFlags = (BEHAVIOR_LOGON_NORMAL | VIEW_MODE_ALL);
	stCmdGrLogonMobile.dwUserID = HallDataMgr::getInstance()->m_dwUserID;

	Utf8ToUtf16(HallDataMgr::getInstance()->m_strMachineID, (WORD*)stCmdGrLogonMobile.szMachineID);
	Utf8ToUtf16(HallDataMgr::getInstance()->m_strDynamicPwd, (WORD*)stCmdGrLogonMobile.szDynamicPwd);
	Utf8ToUtf16(strGameRoomPwd, (WORD*)stCmdGrLogonMobile.szGameRoomPwd);

	tagNetParam param = tagNetParam();

	param.m_nMainCode = MDM_GR_LOGON;
	param.m_nSubCode = SUB_GR_LOGON_MOBILE;
	param.m_pData = &stCmdGrLogonMobile;
	param.m_nDataLen = sizeof(ST_CMD_GR_LOGON_MOBILE);

	sendCurrentRoomData(param);

	HallDataMgr::getInstance()->AddpopLayer("", "正在进入房间...", EM_MODE_TYPE_WAIT);
}

void GameDataMgr::leaveRoom(const WORD &wTabelID, const WORD &wChairID, const BYTE &bForceLeave)
{
	ST_CMD_GR_USER_STAND_UP stCmdGrUserStandUp = { 0 };

	stCmdGrUserStandUp.wTableID = wTabelID;
	stCmdGrUserStandUp.wChairID = wChairID;
	stCmdGrUserStandUp.cbForceLeave = bForceLeave;

	tagNetParam param = tagNetParam();

	param.m_nMainCode = MDM_GR_USER;
	param.m_nSubCode = SUB_GR_USER_STAND_UP;
	param.m_pData = &stCmdGrUserStandUp;
	param.m_nDataLen = sizeof(ST_CMD_GR_USER_STAND_UP);

	sendCurrentRoomData(param);
}

void GameDataMgr::queryChairInfo(const WORD &wTabelID, const WORD &myChair)
{
	int max = HallDataMgr::getInstance()->m_tagSubParam.m_nPlayerCount;

	for (int i = 0; i < max; ++i)
	{
		if (i != myChair)
		{
			//配桌查询
			ST_CMD_GR_USER_INFO_CHAIR_REQ cmd = { 0 };

			cmd.wTableID = wTabelID;
			cmd.wChairID = i;

			tagNetParam param = tagNetParam();

			param.m_nMainCode = MDM_GR_USER;
			param.m_nSubCode = SUB_GR_USER_CHAIR_INFO_REQ;
			param.m_pData = &cmd;
			param.m_nDataLen = sizeof(ST_CMD_GR_USER_INFO_CHAIR_REQ);

			sendCurrentRoomData(param);
		}
	}
}

void GameDataMgr::requestSitdown(const WORD &wTabelID, const WORD &wChairID, const std::string &password /*= ""*/)
{
	NetworkMgr::getInstance()->sendSitDown(wTabelID, wChairID, password);
}

void GameDataMgr::randomSit()
{
	tagNetParam param = tagNetParam();

	param.m_nMainCode = MDM_GR_USER;
	param.m_nSubCode = SUB_GR_USER_CHAIR_REQ;

	sendCurrentRoomData(param);
}

void GameDataMgr::requestReady(bool bReady)
{
	tagNetParam param = tagNetParam();

	param.m_nMainCode = MDM_GF_FRAME;
	param.m_nSubCode = SUB_GF_USER_READY;

	sendCurrentRoomData(param);
}

void GameDataMgr::requestChangeDesk()
{
	m_bChangeDesk = true;
	//INSTANCE(GameDataMgr)->requestSitdown(INVALID_TABLE, INVALID_CHAIR);
	randomSit();
}

void GameDataMgr::userTrusteeship(const BYTE &flag)
{
	/*
	m_param = tagNetParam();
	m_param.m_nMainCode = MDM_GF_GAME;
	m_param.m_nSubCode = SUB_C_TRUSTEE;

	CMD_C_TRUSTEE cmd = {0};
	cmd.bTrustee = flag;

	m_param.m_pData = &cmd;
	m_param.m_nDataLen = sizeof(CMD_C_TRUSTEE);

	sendCurrentRoomData(m_param);
	*/
}

void GameDataMgr::requestFrame()
{
	NetworkMgr::getInstance()->sendGameOption();
}

void GameDataMgr::sureJoin()
{
	m_param = tagNetParam();

	m_param.m_nMainCode = MDM_GF_FRAME;
	m_param.m_nSubCode = SUB_GF_DISTRIBUTE_AFFIRM;

	sendCurrentRoomData(m_param);
}

/*
void GameDataMgr::userCallBanker(const BYTE &btBanker)
{
m_param = tagNetParam();

CMD_C_CallBanker cmd = {0};
cmd.bBanker = btBanker;

m_param.m_nMainCode = MDM_GF_GAME;
m_param.m_nSubCode = SUB_C_CALL_BANKER;
m_param.m_pData = &cmd;
m_param.m_nDataLen = sizeof(CMD_C_CallBanker);
sendCurrentRoomData(m_param);
}

void GameDataMgr::userAddScore(const LONGLONG &lAddScore)
{
m_param = tagNetParam();

CMD_C_AddScore cmd = {0};
cmd.lScore = lAddScore;

m_param.m_nMainCode = MDM_GF_GAME;
m_param.m_nSubCode = SUB_C_ADD_SCORE;
m_param.m_pData = &cmd;
m_param.m_nDataLen = sizeof(CMD_C_AddScore);
sendCurrentRoomData(m_param);
}

void GameDataMgr::userOpenCard(const BYTE &bOpen)
{
m_param = tagNetParam();

CMD_C_OxCard cmd = {0};
cmd.bOX = bOpen;

m_param.m_nMainCode = MDM_GF_GAME;
m_param.m_nSubCode = SUB_C_OPEN_CARD;
m_param.m_pData = &cmd;
m_param.m_nDataLen = sizeof(CMD_C_OxCard);
sendCurrentRoomData(m_param);
}
*/

void GameDataMgr::textChat(const int &len, const std::string &msg, const DWORD &tagetUser /*= INVALID_USERID*/)
{
	m_param = tagNetParam();

	ST_CMD_GR_USER_CLIENT_CHAT stInfo = { 0 };

	stInfo.wChatLength = len;
	stInfo.dwChatColor = GLCOLOR_RGB(255, 255, 255);
	stInfo.dwTargetUserID = tagetUser;

	Utf8ToUtf16(msg, (WORD*)stInfo.szChatMsg);

	m_param.m_nMainCode = MDM_GF_FRAME;
	m_param.m_nSubCode = SUB_GF_USER_CHAT;
	m_param.m_pData = &stInfo;
	m_param.m_nDataLen = sizeof(ST_CMD_GR_USER_CLIENT_CHAT)-sizeof(stInfo.szChatMsg) + stInfo.wChatLength * sizeof(TCHAR);//实际长度

	sendCurrentRoomData(m_param);
}

void GameDataMgr::browChat(const int &idx, const DWORD &dwTargetUserID /*= INVALID_USERID*/)
{
	m_param = tagNetParam();

	ST_CMD_GR_USER_CLIENT_EXPRESSION cmd = { 0 };

	cmd.wItemIndex = idx;
	cmd.dwTargetUserID = dwTargetUserID;

	m_param.m_nMainCode = MDM_GF_FRAME;
	m_param.m_nSubCode	= SUB_GF_USER_EXPRESSION;
	m_param.m_pData		= &cmd;
	m_param.m_nDataLen	= sizeof(ST_CMD_GR_USER_CLIENT_EXPRESSION);

	sendCurrentRoomData(m_param);
}