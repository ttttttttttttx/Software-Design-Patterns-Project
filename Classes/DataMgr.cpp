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
    // 如果数据管理器名称为空或者传入的数据管理器为空，则直接返回
    if (strName.empty() || !pDataMgr)
    {
        return;
    }

    // 插入数据管理器到容器中
    m_mapDatas.insert(pair<string, CDataBaseMgr*>(strName, pDataMgr));
}

CDataBaseMgr* CDataMgr::getData(string strName)
{
    // 如果数据管理器名称为空或者找不到对应名称的数据管理器，则返回空指针
    if (strName.empty() || !m_mapDatas[strName])
    {
        return nullptr;
    }

    // 返回对应名称的数据管理器
    return m_mapDatas[strName];
}


