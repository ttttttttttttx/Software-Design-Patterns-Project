#ifndef __DATAMGR_H__
#define __DATAMGR_H__

#include "cocos2d.h"
#include "DataBase.h"

USING_NS_CC;

class CDataMgr
{
public:
    ~CDataMgr();

    // ��ȡ���ݹ���������
    static CDataMgr* getInstance();

    // ������ݹ�����
    void addData(string strName, CDataBaseMgr* pDataMgr);

    // ��ȡ���ݹ�����
    CDataBaseMgr* getData(string strName);

private:
    CDataMgr();  // ���캯����Ϊ��ȷ���ⲿ�޷�ֱ��ʵ��������
    static CDataMgr* m_spInstance;  // ��ָ̬�룬���ڱ��浥��ʵ��
    map<string, CDataBaseMgr*> m_mapDatas;  // �������ݹ�����������
};

#endif
