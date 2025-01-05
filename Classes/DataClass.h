#ifndef __DATACLASS_H__
#define __DATACLASS_H__

#include "cocos2d.h"
#include "DataBase.h"

// Level data structure
struct SLevelDt : SDataBase
{
    SLevelDt()
    {
        strViewImg = "";
        strMapImg = "";
        strCardView = "";
        nStartMoney = 0;
    }

    string strViewImg;       // Level preview image
    string strMapImg;        // Level map image
    string strCardView;      // Card preview image
    int nStartMoney;         // Starting money
    vector<int> MonsterID;   // List of monster IDs for level
    vector<int> CardID;      // List of card IDs for level
    vector<int> vecWave;     // Wave information for level
};

// Level data manager
class CLevelDtMgr : public CDataBaseMgr
{
public:
    CLevelDtMgr();
    ~CLevelDtMgr();
    int getsize()           // Get number of levels
    {
        return m_vecDatas.size();
    }
    void setCurIndex(int nIndex)  // Set current level index
    {
        m_nCurIndex = nIndex;
    }
    void addCurIndex()      // Increment current level index
    {
        if (m_nCurIndex < m_vecDatas.size() - 1)
        {
            m_nCurIndex++;
        }
    }
    int getCurIndex()       // Get current level index
    {
        return m_nCurIndex;
    }
    void addLockLevel()     // Add unlocked level
    {
        if (m_nLockLevel < m_vecDatas.size())
        {
            m_nLockLevel = m_nCurIndex + 2;
        }
    }
    int getLockLevel()      // Get highest unlocked level
    {
        return m_nLockLevel;
    }
    SLevelDt* getCurData()  // Get current level data
    {
        return static_cast<SLevelDt*>(m_vecDatas[m_nCurIndex]);
    }

    void LoadFile(char* pFileName);   // Load level data from file

private:
    int m_nLockLevel;                 // Highest unlocked level
    int m_nCurIndex;                  // Current level index
};

// Animation data structure
struct SAniMateDt : public SDataBase
{
    SAniMateDt()
    {
        nCount = 0;
        strName = "";
    }

    int nCount;          // Number of animation frames
    string strName;      // Animation frame prefix
};

// Animation data manager
class CAnimateDtMgr : public CDataBaseMgr
{
public:
    CAnimateDtMgr();
    ~CAnimateDtMgr();

    void LoadFile(char* pFileName);   // Load animation data from file
    int getsize();                    // Get number of animation data

private:
};

// Monster data structure
struct SMonsterDt : public SDataBase
{
    SMonsterDt()
    {
        strImg = "";
        fSpeed = 0;
        AnimateID = 0;
        nMoney = 0;
    }

    string strImg;       // Monster image
    float fSpeed;        // Movement speed
    int AnimateID;       // Animation ID
    int nMoney;          // Money earned from the monster
};

// Monster data manager
class CMonsterDtMgr : public CDataBaseMgr
{
public:
    CMonsterDtMgr();
    ~CMonsterDtMgr();

    void LoadFile(char* pFileName);   // Load monster data from file

private:
};

// Card data structure
struct SCardDt : SDataBase
{
    SCardDt()
    {
        strImg = "";
        nArmsID = 0;
    }

    string strImg;       // Card image
    int nArmsID;         // Card ID
};

// Card data manager
class CCardDtMgr : public CDataBaseMgr
{
public:
    CCardDtMgr();
    ~CCardDtMgr();

    void LoadFile(char* pFileName);   // Load card data from file

private:
};

// Arms data structure
struct SArmsDt : SDataBase
{
    SArmsDt()
    {
        strBaseImg = "";
        nAttackID = 0;
        strImg = "";
    }

    string strBaseImg;    // Base image of the arms
    string strImg;        // Arms image
    int nAttackID;        // Attack ID
    int nBulletID;        // Bullet ID
    vector<int> vecMoney;  // List of money
    vector<int> vecRange;  // List of range
    vector<float> vecInterval;  // List of interval
};

// Arms data manager
class CArmsDtMgr : public CDataBaseMgr
{
public:
    CArmsDtMgr();
    ~CArmsDtMgr();

    void LoadFile(char* pFileName);   // Load arms data from file

private:
};

// Bullet data structure
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

    string strImg;       // Bullet image
    string strType;      // Bullet type
    int nDieID;          // Die ID
    int nBuffId;         // Buff ID
    int nMoveAnimateID;  // Move animation ID
    float fSpeed;        // Movement speed
    int nAck;            // Acknowledgment
};

// Bullet data manager
class CBulletDtMgr : public CDataBaseMgr
{
public:
    CBulletDtMgr();
    ~CBulletDtMgr();

    void LoadFile(char* pFileName);   // Load bullet data from file

private:
};

// Buff data structure
struct SBuffDt : SDataBase
{
    SBuffDt()
    {
        nValue = 0;
        fTime = 0;
        nAnimateID = 0;
    }

    int nValue;          // Buff value
    float fTime;         // Buff time
    int nAnimateID;      // Buff ID
};

// Buff data manager
class CBuffDtMgr : public CDataBaseMgr
{
public:
    CBuffDtMgr();
    ~CBuffDtMgr();

    void LoadFile(char* pFileName);   // Load buff data from file

private:
};

#endif
