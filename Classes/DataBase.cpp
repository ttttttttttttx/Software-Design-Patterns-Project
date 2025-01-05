#include "DataBase.h"

CDataBaseMgr::CDataBaseMgr()
{
    // Constructor for data manager, used for initialization
}

CDataBaseMgr::~CDataBaseMgr()
{
    // Destructor for data manager, handles resource cleanup
}

void CDataBaseMgr::LoadFile(char* pFileName)
{
    // Virtual function for loading data from file
    // Derived classes should implement their own loading logic
}

SDataBase* CDataBaseMgr::getDataByID(int nID)
{
    // Get data by ID from storage
    for (SDataBase* pData : m_vecDatas)
    {
        if (nID == pData->nID)
        {
            return pData;
        }
    }

    return nullptr;  // Return null if no matching data found
}

