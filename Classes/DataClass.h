#ifndef __DATACLASS_H__
#define __DATACLASS_H__

#include "cocos2d.h"
#include "DataBase.h"

// 关卡数据结构
struct SLevelDt : SDataBase
{
    SLevelDt()
    {
        strViewImg = "";
        strMapImg = "";
        strCardView = "";
        nStartMoney = 0;
    }

    string strViewImg;       // 关卡地图背景图
    string strMapImg;        // 关卡地图图片
    string strCardView;      // 卡牌预览图
    int nStartMoney;         // 初始金币
    vector<int> MonsterID;   // 关卡怪物ID列表
    vector<int> CardID;       // 关卡卡牌ID列表
    vector<int> vecWave;      // 关卡波次信息
};

// 关卡数据管理器
class CLevelDtMgr : public CDataBaseMgr
{
public:
    CLevelDtMgr();
    ~CLevelDtMgr();
    int getsize()// 获取关卡数据数量
    {
        return m_vecDatas.size();
    }
    void setCurIndex(int nIndex)// 设置当前关卡索引
    {
        m_nCurIndex = nIndex;

    }
    void addCurIndex()// 增加当前关卡索引
    {
        if (m_nCurIndex < m_vecDatas.size() - 1)
        {
            m_nCurIndex++;
        }
    }
    int getCurIndex()// 获取当前关卡索引
    {
        return m_nCurIndex;
    }
    void addLockLevel()// 增加解锁关卡
    {
        if (m_nLockLevel < m_vecDatas.size())
        {
            m_nLockLevel = m_nCurIndex + 2;
        }
    }
    int getLockLevel()// 获取解锁关卡索引
    {
        return m_nLockLevel;

    }
    SLevelDt* getCurData()// 获取当前关卡数据
    {
        return static_cast<SLevelDt*>(m_vecDatas[m_nCurIndex]);
    }

    void LoadFile(char* pFileName);   // 从文件加载关卡数据

private:
    int m_nLockLevel;                 // 解锁的最高关卡索引
    int m_nCurIndex;                  // 当前关卡索引
};

// 动画数据结构
struct SAniMateDt : public SDataBase
{
    SAniMateDt()
    {
        nCount = 0;
        strName = "";
    }

    int nCount;          // 动画帧数
    string strName;      // 动画帧名称前缀
};

// 动画数据管理器
class CAnimateDtMgr : public CDataBaseMgr
{
public:
    CAnimateDtMgr();
    ~CAnimateDtMgr();

    void LoadFile(char* pFileName);   // 从文件加载动画数据
    int getsize();                    // 获取动画数据数量

private:
};

// 怪物数据结构
struct SMonsterDt : public SDataBase
{
    SMonsterDt()
    {
        strImg = "";
        fSpeed = 0;
        AnimateID = 0;
        nMoney = 0;
    }

    string strImg;       // 怪物图片
    float fSpeed;        // 移动速度
    int AnimateID;       // 怪物动画ID
    int nMoney;          // 击败怪物获得的金币
};

// 怪物数据管理器
class CMonsterDtMgr : public CDataBaseMgr
{
public:
    CMonsterDtMgr();
    ~CMonsterDtMgr();

    void LoadFile(char* pFileName);   // 从文件加载怪物数据

private:
};

// 卡牌数据结构
struct SCardDt : SDataBase
{
    SCardDt()
    {
        strImg = "";
        nArmsID = 0;
    }

    string strImg;       // 卡牌图片
    int nArmsID;         // 关联的武器ID
};

// 卡牌数据管理器
class CCardDtMgr : public CDataBaseMgr
{
public:
    CCardDtMgr();
    ~CCardDtMgr();

    void LoadFile(char* pFileName);   // 从文件加载卡牌数据

private:
};

// 武器数据结构
struct SArmsDt : SDataBase
{
    SArmsDt()
    {
        strBaseImg = "";
        nAttackID = 0;
        strImg = "";
    }

    string strBaseImg;    // 武器基础图片
    string strImg;        // 武器图片
    int nAttackID;        // 攻击效果ID
    int nBulletID;        // 关联的子弹ID
    vector<int> vecMoney;  // 升级所需金币列表
    vector<int> vecRange;  // 射程列表
    vector<float> vecInterval;  // 攻击间隔列表
};

// 武器数据管理器
class CArmsDtMgr : public CDataBaseMgr
{
public:
    CArmsDtMgr();
    ~CArmsDtMgr();

    void LoadFile(char* pFileName);   // 从文件加载武器数据

private:
};

// 子弹数据结构
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

    string strImg;       // 子弹图片
    string strType;      // 子弹类型
    int nDieID;          // 消失效果ID
    int nBuffId;         // 关联的Buff ID
    int nMoveAnimateID;  // 移动动画ID
    float fSpeed;        // 移动速度
    int nAck;            // 攻击力
};

// 子弹数据管理器
class CBulletDtMgr : public CDataBaseMgr
{
public:
    CBulletDtMgr();
    ~CBulletDtMgr();

    void LoadFile(char* pFileName);   // 从文件加载子弹数据

private:
};

// Buff数据结构
struct SBuffDt : SDataBase
{
    SBuffDt()
    {
        nValue = 0;
        fTime = 0;
        nAnimateID = 0;
    }

    int nValue;          // Buff数值
    float fTime;         // Buff持续时间
    int nAnimateID;      // Buff动画ID
};

// Buff数据管理器
class CBuffDtMgr : public CDataBaseMgr
{
public:
    CBuffDtMgr();
    ~CBuffDtMgr();

    void LoadFile(char* pFileName);   // 从文件加载Buff数据

private:
};

#endif
