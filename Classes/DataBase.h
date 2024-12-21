#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "cocos2d.h"
using namespace std;

// 数据结构体，用于表示基础数据的通用属性
struct SDataBase
{
    SDataBase()
    {
        nID = 0;  // 初始化数据ID为0
    }
    int nID;  // 数据ID
};

// 数据管理器基类，负责加载数据文件和提供根据ID获取数据的方法
class CDataBaseMgr
{
public:
    CDataBaseMgr();  // 构造函数
    virtual ~CDataBaseMgr();  // 虚析构函数，确保子类析构函数会被调用
    virtual void LoadFile(char* pFileName);  // 加载数据文件的虚函数
    SDataBase* getDataByID(int nID);  // 根据ID获取数据的方法
protected:
    vector<SDataBase*> m_vecDatas;  // 数据存储的容器
};

#endif