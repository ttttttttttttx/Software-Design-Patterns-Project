#include "BuffLayer.h"
#include "Buff.h"

// Constructor
CBuffLayer::CBuffLayer()
{
}

// Destructor
CBuffLayer::~CBuffLayer()
{
}

// Initialize the buff layer
bool CBuffLayer::init()
{
    return Node::init() && (inintCallBack(), scheduleUpdate(), true);
}

// Initialize callback for specific buff type
void CBuffLayer::initCallbackForBuff(int buffId, CBuffBase* buffInstance)
{
    // Store in m_mapCallBack for later use
    m_mapCallBack[buffId] = buffInstance;
    // Retain buff instance to prevent release
    m_mapCallBack[buffId]->retain();
}

// Initialize callback functions
void CBuffLayer::inintCallBack()
{
    // Create speed buff instance (7001) and store in m_mapCallBack
    initCallbackForBuff(7001, CBuffSpeed::create());

    // Create damage buff instance (7002) and store in m_mapCallBack
    initCallbackForBuff(7002, CBuffHurt::create());
}

// Add buff to node
void CBuffLayer::addBuff(Node* pNode, int nBuffID)
{
    // Only proceed if buff ID is not 0
    if (nBuffID != 0)
    {
        // Get buff data manager instance
        CBuffDtMgr* pBuffMgr = static_cast<CBuffDtMgr*>(CDataMgr::getInstance()->getData("BuffMgr"));

        // Get buff data by ID
        SBuffDt* pBuffDt = static_cast<SBuffDt*>(pBuffMgr->getDataByID(nBuffID));

        // Use corresponding buff instance to add buff to specified node
        m_mapCallBack[nBuffID]->addBuffNode(pNode, pBuffDt);
    }
}

// Update function called each frame
void CBuffLayer::update(float delta)
{
    // Update both speed and damage buffs
    m_mapCallBack[7001]->update(delta);
    m_mapCallBack[7002]->update(delta);
}

// Remove buff from node
void CBuffLayer::removeBuff(Node* pNode)
{
    // Remove both speed and damage buffs from specified node
    m_mapCallBack[7001]->removeBuffNode(pNode);
    m_mapCallBack[7002]->removeBuffNode(pNode);
}

