#include "DataClass.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace rapidjson;

// 关卡数据管理器构造函数
CLevelDtMgr::CLevelDtMgr()
{
	m_nCurIndex = 0;
	m_nLockLevel = 1;
}

// 关卡数据管理器析构函数
CLevelDtMgr::~CLevelDtMgr()
{
}

// 从文件加载关卡数据
void CLevelDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SLevelDt* pData = new SLevelDt();
		rapidjson::Value& vData = Doc[i];

		// 填充关卡数据
		pData->nID = vData["id"].GetInt();
		pData->strViewImg = vData["viewimg"].GetString();
		pData->strMapImg = vData["mapimg"].GetString();
		pData->strCardView = vData["CardView"].GetString();
		pData->nStartMoney = vData["startMoney"].GetInt();

		// 解析 MonsterID 列表
		rapidjson::Value& vMonsterID = vData["MonsterID"];
		for (int j = 0; j < vMonsterID.Size(); j++)
		{
			pData->MonsterID.push_back(vMonsterID[j].GetInt());
		}

		// 解析 CardID 列表
		rapidjson::Value& vCardID = vData["CardID"];
		for (int m = 0; m < vCardID.Size(); m++)
		{
			pData->CardID.push_back(vCardID[m].GetInt());
		}

		// 解析 vecWave 列表
		rapidjson::Value& vWave = vData["wave"];
		for (int m = 0; m < vWave.Size(); m++)
		{
			pData->vecWave.push_back(vWave[m].GetInt());
		}

		m_vecDatas.push_back(pData);
	}
}

// 动画数据管理器构造函数
CAnimateDtMgr::CAnimateDtMgr()
{
}

// 动画数据管理器析构函数
CAnimateDtMgr::~CAnimateDtMgr()
{
}

// 从文件加载动画数据
void CAnimateDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SAniMateDt* pData = new SAniMateDt();
		rapidjson::Value& vData = Doc[i];

		// 填充动画数据
		pData->nID = vData["id"].GetInt();
		pData->nCount = vData["count"].GetInt();
		pData->strName = vData["name"].GetString();

		m_vecDatas.push_back(pData);
	}
}

// 怪物数据管理器构造函数
CMonsterDtMgr::CMonsterDtMgr()
{

}

// 怪物数据管理器析构函数
CMonsterDtMgr::~CMonsterDtMgr()
{

}

// 从文件加载怪物数据
void CMonsterDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SMonsterDt* pData = new SMonsterDt();
		rapidjson::Value& vData = Doc[i];

		// 填充怪物数据
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["img"].GetString();
		pData->fSpeed = vData["speed"].GetDouble();
		pData->AnimateID = vData["animateID"].GetInt();
		pData->nMoney = vData["Money"].GetInt();

		m_vecDatas.push_back(pData);
	}
}

// 卡牌数据管理器构造函数
CCardDtMgr::CCardDtMgr()
{

}

// 卡牌数据管理器析构函数
CCardDtMgr::~CCardDtMgr()
{

}

// 从文件加载卡牌数据
void CCardDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SCardDt* pData = new SCardDt();
		rapidjson::Value& vData = Doc[i];

		// 填充卡牌数据
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["img"].GetString();
		pData->nArmsID = vData["ArmsID"].GetInt();

		m_vecDatas.push_back(pData);
	}
}

// 武器数据管理器构造函数
CArmsDtMgr::CArmsDtMgr()
{

}

// 武器数据管理器析构函数
CArmsDtMgr::~CArmsDtMgr()
{

}

// 从文件加载武器数据
void CArmsDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SArmsDt* pData = new SArmsDt();
		rapidjson::Value& vData = Doc[i];

		// 填充武器数据
		pData->nID = vData["id"].GetInt();
		pData->strImg = vData["Img"].GetString();
		pData->nAttackID = vData["AttackID"].GetInt();
		pData->strBaseImg = vData["BaseImg"].GetString();
		pData->nBulletID = vData["BulletID"].GetInt();

		// 解析 Money、Range、Interval 列表
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

// 子弹数据管理器构造函数
CBulletDtMgr::CBulletDtMgr()
{

}

// 子弹数据管理器析构函数
CBulletDtMgr::~CBulletDtMgr()
{

}

// 从文件加载子弹数据
void CBulletDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SBulletDt* pData = new SBulletDt();
		rapidjson::Value& vData = Doc[i];

		// 填充子弹数据
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

// Buff数据管理器构造函数
CBuffDtMgr::CBuffDtMgr()
{

}

// Buff数据管理器析构函数
CBuffDtMgr::~CBuffDtMgr()
{

}

// 从文件加载Buff数据
void CBuffDtMgr::LoadFile(char* pFileName)
{
	// 读取文件内容
	string strData = FileUtils::getInstance()->getStringFromFile(pFileName);
	Document Doc;
	Doc.Parse(strData.c_str());

	// 解析 JSON 数据
	for (int i = 0; i < Doc.Size(); i++)
	{
		SBuffDt* pData = new SBuffDt();
		rapidjson::Value& vData = Doc[i];

		// 填充Buff数据
		pData->nID = vData["id"].GetInt();
		pData->nValue = vData["value"].GetInt();
		pData->fTime = vData["time"].GetDouble();
		pData->nAnimateID = vData["animateID"].GetInt();

		m_vecDatas.push_back(pData);
	}
}
