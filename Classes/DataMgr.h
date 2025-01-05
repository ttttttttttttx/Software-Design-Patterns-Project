#ifndef __DATAMGR_H__
#define __DATAMGR_H__

#include "cocos2d.h"
#include "DataBase.h"

USING_NS_CC;

class CDataMgr
{
public:
    ~CDataMgr();

    // Get data manager instance
    static CDataMgr* getInstance();

    // Add data manager
    void addData(string strName, CDataBaseMgr* pDataMgr);

    // Get data manager
    CDataBaseMgr* getData(string strName);

private:
    CDataMgr();  // Private constructor to ensure singleton pattern
    static CDataMgr* m_spInstance;  // Static pointer to store singleton instance
    map<string, CDataBaseMgr*> m_mapDatas;  // Map to store data managers
};

#endif
