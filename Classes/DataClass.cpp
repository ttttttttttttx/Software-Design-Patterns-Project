#include "DataClass.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace rapidjson;

// Level data manager constructor
CLevelDtMgr::CLevelDtMgr()
{
	m_nCurIndex = 0;
	m_nLockLevel = 1;
}

// Level data manager destructor
CLevelDtMgr::~CLevelDtMgr()
{
}

// Load level data from file
void CLevelDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SLevelDt* pData = new SLevelDt();
		rapidjson::Value& vData = Doc[i];

		// Load level data
		pData->nID = vData["id"].GetInt();
		pData->strViewImg = vData["viewimg"].GetString();
		pData->strMapImg = vData["mapimg"].GetString();
		pData->strCardView = vData["CardView"].GetString();
		pData->nStartMoney = vData["startMoney"].GetInt();

		// Load MonsterID list
		rapidjson::Value& vMonsterID = vData["MonsterID"];
		for (int j = 0; j < vMonsterID.Size(); j++)
		{
			pData->MonsterID.push_back(vMonsterID[j].GetInt());
		}

		// Load CardID list
		rapidjson::Value& vCardID = vData["CardID"];
		for (int m = 0; m < vCardID.Size(); m++)
		{
			pData->CardID.push_back(vCardID[m].GetInt());
		}

		// Load Wave list
		rapidjson::Value& vWave = vData["wave"];
		for (int m = 0; m < vWave.Size(); m++)
		{
			pData->vecWave.push_back(vWave[m].GetInt());
		}

		m_vecDatas.push_back(pData);
	}
}

// Animate data manager constructor
CAnimateDtMgr::CAnimateDtMgr()
{
}

// Animate data manager destructor
CAnimateDtMgr::~CAnimateDtMgr()
{
}

// Load animate data from file
void CAnimateDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SAniMateDt* pData = new SAniMateDt();
		rapidjson::Value& vData = Doc[i];

		// Load animate data
		pData->nID = vData["id"].GetInt();
		pData->nCount = vData["count"].GetInt();
		pData->strName = vData["name"].GetString();

		m_vecDatas.push_back(pData);
	}
}

// Monster data manager constructor
CMonsterDtMgr::CMonsterDtMgr()
{

}

// Monster data manager destructor
CMonsterDtMgr::~CMonsterDtMgr()
{

}

// Load monster data from file
void CMonsterDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SMonsterDt* pData = new SMonsterDt();
		rapidjson::Value& vData = Doc[i];

		// Load monster data
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["img"].GetString();
		pData->fSpeed = vData["speed"].GetDouble();
		pData->AnimateID = vData["animateID"].GetInt();
		pData->nMoney = vData["Money"].GetInt();

		m_vecDatas.push_back(pData);
	}
}

// Card data manager constructor
CCardDtMgr::CCardDtMgr()
{

}

// Card data manager destructor
CCardDtMgr::~CCardDtMgr()
{

}

// Load card data from file
void CCardDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SCardDt* pData = new SCardDt();
		rapidjson::Value& vData = Doc[i];

		// Load card data
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["img"].GetString();
		pData->nArmsID = vData["ArmsID"].GetInt();

		m_vecDatas.push_back(pData);
	}
}

// Arms data manager constructor
CArmsDtMgr::CArmsDtMgr()
{

}

// Arms data manager destructor
CArmsDtMgr::~CArmsDtMgr()
{

}

// Load arms data from file
void CArmsDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SArmsDt* pData = new SArmsDt();
		rapidjson::Value& vData = Doc[i];

		// Load arms data
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["Img"].GetString();
		pData->nAttackID = vData["AttackID"].GetInt();
		pData->strBaseImg = vData["BaseImg"].GetString();
		pData->nBulletID = vData["BulletID"].GetInt();

		// Load Money, Range, Interval lists
		rapidjson::Value& vMoney = vData["upgrade"];
		for (int i = 0; i < vMoney.Size(); i++)
		{
			pData->vecMoney.push_back(vMoney[i].GetInt());
		}

		rapidjson::Value& vRange = vData["range"];
		for (int i = 0; i < vRange.Size(); i++)
		{
			pData->vecRange.push_back(vRange[i].GetInt());
		}

		rapidjson::Value& vInterval = vData["Interval"];
		for (int i = 0; i < vInterval.Size(); i++)
		{
			pData->vecInterval.push_back(vInterval[i].GetDouble());
		}

		m_vecDatas.push_back(pData);
	}
}

// Bullet data manager constructor
CBulletDtMgr::CBulletDtMgr()
{

}

// Bullet data manager destructor
CBulletDtMgr::~CBulletDtMgr()
{

}

// Load bullet data from file
void CBulletDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SBulletDt* pData = new SBulletDt();
		rapidjson::Value& vData = Doc[i];

		// Load bullet data
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["Img"].GetString();
		pData->strType = vData["Type"].GetString();
		pData->nDieID = vData["die"].GetInt();
		pData->nBuffId = vData["buffID"].GetInt();
		pData->nMoveAnimateID = vData["moveAnimateID"].GetInt();
		pData->fSpeed = vData["speed"].GetDouble();
		pData->nAck = vData["ack"].GetInt();

		m_vecDatas.push_back(pData);
	}
}

// Buff data manager constructor
CBuffDtMgr::CBuffDtMgr()
{

}

// Buff data manager destructor
CBuffDtMgr::~CBuffDtMgr()
{

}

// Load buff data from file
void CBuffDtMgr::LoadFile(char* pFileName)
{
	// Read file content
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// Parse JSON data
	for (int i = 0; i < Doc.Size(); i++)
	{
		SBuffDt* pData = new SBuffDt();
		rapidjson::Value& vData = Doc[i];

		// Load buff data
		pData->nID = vData["id"].GetInt();
		pData->nValue = vData["value"].GetInt();
		pData->fTime = vData["time"].GetDouble();
		pData->nAnimateID = vData["animateID"].GetInt();

		m_vecDatas.push_back(pData);
	}
}
