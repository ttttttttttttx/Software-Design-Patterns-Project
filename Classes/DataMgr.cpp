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
    // ������ݹ���������Ϊ�ջ��ߴ�������ݹ�����Ϊ�գ���ֱ�ӷ���
    if (strName.empty() || !pDataMgr)
    {
        return;
    }

    // �������ݹ�������������
    m_mapDatas.insert(pair<string, CDataBaseMgr*>(strName, pDataMgr));
}

CDataBaseMgr* CDataMgr::getData(string strName)
{
    // ������ݹ���������Ϊ�ջ����Ҳ�����Ӧ���Ƶ����ݹ��������򷵻ؿ�ָ��
    if (strName.empty() || !m_mapDatas[strName])
    {
        return nullptr;
    }

    // ���ض�Ӧ���Ƶ����ݹ�����
    return m_mapDatas[strName];
}


