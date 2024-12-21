#include "DataBase.h"

CDataBaseMgr::CDataBaseMgr()
{
    // 数据管理器构造函数，通常用于初始化
}

CDataBaseMgr::~CDataBaseMgr()
{
    // 数据管理器析构函数，用于释放资源或做清理工作
}

void CDataBaseMgr::LoadFile(char* pFileName)
{
    // 数据管理器加载文件的方法，子类需要实现具体的文件加载逻辑
    // 这里是一个虚函数，所以可以在子类中进行重写
}

SDataBase* CDataBaseMgr::getDataByID(int nID)
{
    // 根据数据ID获取数据的方法
    for (SDataBase* pData : m_vecDatas)
    {
        if (nID == pData->nID)
        {
            return pData;
        }
    }

    return nullptr;  // 未找到匹配的数据，返回空指针
}

