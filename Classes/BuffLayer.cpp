#include "BuffLayer.h"
#include "Buff.h"

// CBuffLayer 类的构造函数
CBuffLayer::CBuffLayer()
{

}

// CBuffLayer 类的析构函数
CBuffLayer::~CBuffLayer()
{

}

// CBuffLayer 类的初始化方法
bool CBuffLayer::init()
{
    return Node::init() && (inintCallBack(), scheduleUpdate(), true);
}

// 初始化回调函数，用于创建不同类型的Buff实例
void CBuffLayer::initCallbackForBuff(int buffId, CBuffBase* buffInstance)
{
	// 存入m_mapCallBack中，以便后续使用
	m_mapCallBack[buffId] = buffInstance;
	// 对Buff实例进行retain，防止被释放
	m_mapCallBack[buffId]->retain();
}

// CBuffLayer类的初始化回调函数方法
void CBuffLayer::inintCallBack()
{
	// 创建速度Buff实例(7001)，并将其存入m_mapCallBack中
	initCallbackForBuff(7001, CBuffSpeed::create());

	// 创建伤害Buff实例(7002)，并将其存入m_mapCallBack中
	initCallbackForBuff(7002, CBuffHurt::create());
}

// CBuffLayer 类的添加Buff方法
void CBuffLayer::addBuff(Node* pNode, int nBuffID)
{
	// 如果Buff的ID为0，则不执行添加操作
	if (nBuffID != 0)
	{
		// 获取Buff数据管理器的实例
		CBuffDtMgr* pBuffMgr = static_cast<CBuffDtMgr*>(CDataMgr::getInstance()->getData("BuffMgr"));

		// 根据Buff的ID获取对应的Buff数据
		SBuffDt* pBuffDt = static_cast<SBuffDt*>(pBuffMgr->getDataByID(nBuffID));

		// 调用对应Buff实例的addBuffNode方法，将Buff添加到指定的节点上
		m_mapCallBack[nBuffID]->addBuffNode(pNode, pBuffDt);
	}
}

// CBuffLayer 类的每帧更新方法
void CBuffLayer::update(float delta)
{
	// 分别调用速度Buff和伤害Buff的更新方法
	m_mapCallBack[7001]->update(delta);
	m_mapCallBack[7002]->update(delta);
}

// CBuffLayer 类的移除Buff方法
void CBuffLayer::removeBuff(Node* pNode)
{
	// 分别调用速度Buff和伤害Buff的removeBuffNode方法，移除指定节点上的Buff
	m_mapCallBack[7001]->removeBuffNode(pNode);
	m_mapCallBack[7002]->removeBuffNode(pNode);
}

