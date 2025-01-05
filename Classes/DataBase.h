#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "cocos2d.h"
using namespace std;

// Base data structure for all data types
struct SDataBase
{
    SDataBase()
    {
        nID = 0;  // Initialize ID to 0
    }
    int nID;  // Data ID
};

// Base data manager class, handles file loading and data retrieval by ID
class CDataBaseMgr
{
public:
    CDataBaseMgr();  // Constructor
    virtual ~CDataBaseMgr();  // Virtual destructor for proper inheritance
    virtual void LoadFile(char* pFileName);  // Load data from file
    SDataBase* getDataByID(int nID);  // Get data by ID
protected:
    vector<SDataBase*> m_vecDatas;  // Data storage vector
};

#endif