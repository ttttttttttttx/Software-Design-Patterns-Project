#ifndef __DATAMGR_H__
#define __DATAMGR_H__

#include "cocos2d.h"
#include "DataBase.h"

USING_NS_CC;

class CDataMgr
{
public:
    ~CDataMgr();

    // 获取数据管理器单例
    static CDataMgr* getInstance();

    // 添加数据管理器
    void addData(string strName, CDataBaseMgr* pDataMgr);

    // 获取数据管理器
    CDataBaseMgr* getData(string strName);

private:
    CDataMgr();  // 构造函数，为了确保外部无法直接实例化该类
    static CDataMgr* m_spInstance;  // 静态指针，用于保存单例实例
    map<string, CDataBaseMgr*> m_mapDatas;  // 保存数据管理器的容器
};

#endif
