#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "cocos2d.h"
using namespace std;

// ���ݽṹ�壬���ڱ�ʾ�������ݵ�ͨ������
struct SDataBase
{
    SDataBase()
    {
        nID = 0;  // ��ʼ������IDΪ0
    }
    int nID;  // ����ID
};

// ���ݹ��������࣬������������ļ����ṩ����ID��ȡ���ݵķ���
class CDataBaseMgr
{
public:
    CDataBaseMgr();  // ���캯��
    virtual ~CDataBaseMgr();  // ������������ȷ���������������ᱻ����
    virtual void LoadFile(char* pFileName);  // ���������ļ����麯��
    SDataBase* getDataByID(int nID);  // ����ID��ȡ���ݵķ���
protected:
    vector<SDataBase*> m_vecDatas;  // ���ݴ洢������
};

#endif