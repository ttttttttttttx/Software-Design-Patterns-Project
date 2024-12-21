#include "BuffLayer.h"
#include "Buff.h"

// CBuffLayer ��Ĺ��캯��
CBuffLayer::CBuffLayer()
{

}

// CBuffLayer �����������
CBuffLayer::~CBuffLayer()
{

}

// CBuffLayer ��ĳ�ʼ������
bool CBuffLayer::init()
{
    return Node::init() && (inintCallBack(), scheduleUpdate(), true);
}

// ��ʼ���ص����������ڴ�����ͬ���͵�Buffʵ��
void CBuffLayer::initCallbackForBuff(int buffId, CBuffBase* buffInstance)
{
	// ����m_mapCallBack�У��Ա����ʹ��
	m_mapCallBack[buffId] = buffInstance;
	// ��Buffʵ������retain����ֹ���ͷ�
	m_mapCallBack[buffId]->retain();
}

// CBuffLayer��ĳ�ʼ���ص���������
void CBuffLayer::inintCallBack()
{
	// �����ٶ�Buffʵ��(7001)�����������m_mapCallBack��
	initCallbackForBuff(7001, CBuffSpeed::create());

	// �����˺�Buffʵ��(7002)�����������m_mapCallBack��
	initCallbackForBuff(7002, CBuffHurt::create());
}

// CBuffLayer ������Buff����
void CBuffLayer::addBuff(Node* pNode, int nBuffID)
{
	// ���Buff��IDΪ0����ִ����Ӳ���
	if (nBuffID != 0)
	{
		// ��ȡBuff���ݹ�������ʵ��
		CBuffDtMgr* pBuffMgr = static_cast<CBuffDtMgr*>(CDataMgr::getInstance()->getData("BuffMgr"));

		// ����Buff��ID��ȡ��Ӧ��Buff����
		SBuffDt* pBuffDt = static_cast<SBuffDt*>(pBuffMgr->getDataByID(nBuffID));

		// ���ö�ӦBuffʵ����addBuffNode��������Buff��ӵ�ָ���Ľڵ���
		m_mapCallBack[nBuffID]->addBuffNode(pNode, pBuffDt);
	}
}

// CBuffLayer ���ÿ֡���·���
void CBuffLayer::update(float delta)
{
	// �ֱ�����ٶ�Buff���˺�Buff�ĸ��·���
	m_mapCallBack[7001]->update(delta);
	m_mapCallBack[7002]->update(delta);
}

// CBuffLayer ����Ƴ�Buff����
void CBuffLayer::removeBuff(Node* pNode)
{
	// �ֱ�����ٶ�Buff���˺�Buff��removeBuffNode�������Ƴ�ָ���ڵ��ϵ�Buff
	m_mapCallBack[7001]->removeBuffNode(pNode);
	m_mapCallBack[7002]->removeBuffNode(pNode);
}

