#ifndef __DATACLASS_H__
#define __DATACLASS_H__

#include "cocos2d.h"
#include "DataBase.h"

// �ؿ����ݽṹ
struct SLevelDt : SDataBase
{
    SLevelDt()
    {
        strViewImg = "";
        strMapImg = "";
        strCardView = "";
        nStartMoney = 0;
    }

    string strViewImg;       // �ؿ���ͼ����ͼ
    string strMapImg;        // �ؿ���ͼͼƬ
    string strCardView;      // ����Ԥ��ͼ
    int nStartMoney;         // ��ʼ���
    vector<int> MonsterID;   // �ؿ�����ID�б�
    vector<int> CardID;       // �ؿ�����ID�б�
    vector<int> vecWave;      // �ؿ�������Ϣ
};

// �ؿ����ݹ�����
class CLevelDtMgr : public CDataBaseMgr
{
public:
    CLevelDtMgr();
    ~CLevelDtMgr();
    int getsize()// ��ȡ�ؿ���������
    {
        return m_vecDatas.size();
    }
    void setCurIndex(int nIndex)// ���õ�ǰ�ؿ�����
    {
        m_nCurIndex = nIndex;

    }
    void addCurIndex()// ���ӵ�ǰ�ؿ�����
    {
        if (m_nCurIndex < m_vecDatas.size() - 1)
        {
            m_nCurIndex++;
        }
    }
    int getCurIndex()// ��ȡ��ǰ�ؿ�����
    {
        return m_nCurIndex;
    }
    void addLockLevel()// ���ӽ����ؿ�
    {
        if (m_nLockLevel < m_vecDatas.size())
        {
            m_nLockLevel = m_nCurIndex + 2;
        }
    }
    int getLockLevel()// ��ȡ�����ؿ�����
    {
        return m_nLockLevel;

    }
    SLevelDt* getCurData()// ��ȡ��ǰ�ؿ�����
    {
        return static_cast<SLevelDt*>(m_vecDatas[m_nCurIndex]);
    }

    void LoadFile(char* pFileName);   // ���ļ����عؿ�����

private:
    int m_nLockLevel;                 // ��������߹ؿ�����
    int m_nCurIndex;                  // ��ǰ�ؿ�����
};

// �������ݽṹ
struct SAniMateDt : public SDataBase
{
    SAniMateDt()
    {
        nCount = 0;
        strName = "";
    }

    int nCount;          // ����֡��
    string strName;      // ����֡����ǰ׺
};

// �������ݹ�����
class CAnimateDtMgr : public CDataBaseMgr
{
public:
    CAnimateDtMgr();
    ~CAnimateDtMgr();

    void LoadFile(char* pFileName);   // ���ļ����ض�������
    int getsize();                    // ��ȡ������������

private:
};

// �������ݽṹ
struct SMonsterDt : public SDataBase
{
    SMonsterDt()
    {
        strImg = "";
        fSpeed = 0;
        AnimateID = 0;
        nMoney = 0;
    }

    string strImg;       // ����ͼƬ
    float fSpeed;        // �ƶ��ٶ�
    int AnimateID;       // ���ﶯ��ID
    int nMoney;          // ���ܹ����õĽ��
};

// �������ݹ�����
class CMonsterDtMgr : public CDataBaseMgr
{
public:
    CMonsterDtMgr();
    ~CMonsterDtMgr();

    void LoadFile(char* pFileName);   // ���ļ����ع�������

private:
};

// �������ݽṹ
struct SCardDt : SDataBase
{
    SCardDt()
    {
        strImg = "";
        nArmsID = 0;
    }

    string strImg;       // ����ͼƬ
    int nArmsID;         // ����������ID
};

// �������ݹ�����
class CCardDtMgr : public CDataBaseMgr
{
public:
    CCardDtMgr();
    ~CCardDtMgr();

    void LoadFile(char* pFileName);   // ���ļ����ؿ�������

private:
};

// �������ݽṹ
struct SArmsDt : SDataBase
{
    SArmsDt()
    {
        strBaseImg = "";
        nAttackID = 0;
        strImg = "";
    }

    string strBaseImg;    // ��������ͼƬ
    string strImg;        // ����ͼƬ
    int nAttackID;        // ����Ч��ID
    int nBulletID;        // �������ӵ�ID
    vector<int> vecMoney;  // �����������б�
    vector<int> vecRange;  // ����б�
    vector<float> vecInterval;  // ��������б�
};

// �������ݹ�����
class CArmsDtMgr : public CDataBaseMgr
{
public:
    CArmsDtMgr();
    ~CArmsDtMgr();

    void LoadFile(char* pFileName);   // ���ļ�������������

private:
};

// �ӵ����ݽṹ
struct SBulletDt : SDataBase
{
    SBulletDt()
    {
        strImg = "";
        nDieID = 0;
        strType = "";
        nBuffId = 0;
        nMoveAnimateID = 0;
        fSpeed = 0;
        nAck = 0;
    }

    string strImg;       // �ӵ�ͼƬ
    string strType;      // �ӵ�����
    int nDieID;          // ��ʧЧ��ID
    int nBuffId;         // ������Buff ID
    int nMoveAnimateID;  // �ƶ�����ID
    float fSpeed;        // �ƶ��ٶ�
    int nAck;            // ������
};

// �ӵ����ݹ�����
class CBulletDtMgr : public CDataBaseMgr
{
public:
    CBulletDtMgr();
    ~CBulletDtMgr();

    void LoadFile(char* pFileName);   // ���ļ������ӵ�����

private:
};

// Buff���ݽṹ
struct SBuffDt : SDataBase
{
    SBuffDt()
    {
        nValue = 0;
        fTime = 0;
        nAnimateID = 0;
    }

    int nValue;          // Buff��ֵ
    float fTime;         // Buff����ʱ��
    int nAnimateID;      // Buff����ID
};

// Buff���ݹ�����
class CBuffDtMgr : public CDataBaseMgr
{
public:
    CBuffDtMgr();
    ~CBuffDtMgr();

    void LoadFile(char* pFileName);   // ���ļ�����Buff����

private:
};

#endif
