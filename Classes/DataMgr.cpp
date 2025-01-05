#include "DataMgr.h"

CDataMgr* CDataMgr::m_spInstance = nullptr;

CDataMgr::CDataMgr()
{
}

CDataMgr::~CDataMgr()
{
}

CDataMgr* CDataMgr::getInstance()
{
    if (!m_spInstance)
    {
        m_spInstance = new CDataMgr();
    }
    return m_spInstance;
}

void CDataMgr::addData(string strName, CDataBaseMgr* pDataMgr)
{
    // Return if name is empty or data manager is null
    if (strName.empty() || !pDataMgr)
    {
        return;
    }

    // Add data manager to map
    m_mapDatas.insert(pair<string, CDataBaseMgr*>(strName, pDataMgr));
}

CDataBaseMgr* CDataMgr::getData(string strName)
{
    // Return null if name is empty or data manager not found
    if (strName.empty() || !m_mapDatas[strName])
    {
        return nullptr;
    }

    // Return data manager
    return m_mapDatas[strName];
}


