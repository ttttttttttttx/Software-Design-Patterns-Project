#include "Buff.h"
#include "GameScene.h"
#include "Monster.h"
// CBuffBase ��Ĺ��캯��
CBuffBase::CBuffBase()
{

}

// CBuffBase �����������
CBuffBase::~CBuffBase()
{

}

// CBuffBase ��ĳ�ʼ������
bool CBuffBase::init()
{
	return Node::init();
}

// CBuffBase ���ÿ֡���·���
void CBuffBase::update(float delta)
{
	// ������ʱ�����κβ��������������ʵ��
}

// CBuffBase ������Buff�ڵ㷽��
void CBuffBase::addBuffNode(Node* pNode, SBuffDt* pBuff)
{
	// ��m_mapBuff�в���������ڵ�pNode������Buff
	auto iter = m_mapBuff.find(pNode);

	// ����ҵ��˶�Ӧ��Buff
	if (iter != m_mapBuff.end())
	{
		// ����Buff��ʣ��ʱ��
		m_mapBuff[pNode]->fTime = pBuff->fTime;
	}
	else
	{
		// ���û���ҵ���Ӧ��Buff���򴴽�һ���µ�SData�ṹ�����ڴ洢Buff�������
		SData* pData = new SData();
		pData->fTime = pBuff->fTime;
		pData->nValue = pBuff->nValue;

		// ����Buff��ͼ��
		pData->pSprite = Sprite::create();

		// ����Ϸ�����Ķ��������л�Buff�Ķ���
		auto animateLayer = CGameScene::getInstance()->getMyAnimate();
		animateLayer->changeAction(pData->pSprite, pBuff->nAnimateID, true, 0.1f);

		// ����Buffͼ���λ��
		pData->pSprite->setPositionY(static_cast<CMonster*>(pNode)->getContentSize().height / 2);

		// ��Buffͼ����ӵ��ڵ���
		pNode->addChild(pData->pSprite);

		// ���´�����SData�ṹ����ӵ�m_mapBuff�У�������ڵ�pNode����
		m_mapBuff[pNode] = pData;
	}
	
}

// CBuffBase ����Ƴ�Buff�ڵ㷽��
void CBuffBase::removeBuffNode(Node* pNode)
{
	auto iter = m_mapBuff.find(pNode);

	if (iter != m_mapBuff.end())
	{
		// ֹͣBuffͼ���ϵ����ж���
		iter->second->pSprite->stopAllActions();

		// �Ӹ��ڵ��Ƴ�Buffͼ��
		iter->second->pSprite->removeFromParent();

		// ��m_mapBuff���Ƴ�������ڵ�pNode������Buff
		m_mapBuff.erase(iter);
	}
}

// CBuffSpeed ��ĳ�ʼ������
bool CBuffSpeed::init()
{
    return CBuffBase::init();
}

// CBuffSpeed ���ÿ֡���·���
void CBuffSpeed::update(float delta)
{
    // ���Buff�Ƿ����
    if (m_mapBuff.empty())
        return;

    // ʹ�÷�Χ���ڷ�Χ�� for ѭ��
    for (auto iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
    {
        // ����Buff��ʣ��ʱ��
        iter->second->fTime -= delta;

        // ��ȡ��ǰBuff�����Ĺ���������ٶ�����ΪBuff����ֵ
        auto monster = static_cast<CMonster*>(iter->first);
        if (monster->getSpeed() >= iter->second->nValue)
        {
            monster->setSpeed(iter->second->nValue);
        }
        else
        {
            monster->setSpeed(8);
        }

        // ���Buff�Ƿ��Ѿ���ʱ
        if (iter->second->fTime <= 0)
        {
            // �ָ�����ԭʼ�ٶ�
            monster->setSpeed(monster->getSaveSpeed());

            // ֹͣ���ж������Ӹ��ڵ��Ƴ�Buff��ͼ��
            auto buffSprite = iter->second->pSprite;
            buffSprite->stopAllActions();
            buffSprite->removeFromParent();

            // ��Buff Map ���Ƴ���ǰBuff������ȡ��һ��������
            iter = m_mapBuff.erase(iter);
        }
        else
        {
            // �ƶ�����һ��Buff
            ++iter;
        }
    }
}

// CBuffHurt ��ĳ�ʼ������
bool CBuffHurt::init()
{
    return CBuffBase::init();
}

// CBuffHurt ���ÿ֡���·���
void CBuffHurt::update(float delta)
{
    // ���Buff�Ƿ����
    if (m_mapBuff.empty())
        return;

    // ʹ�÷�Χ���ڷ�Χ�� for ѭ��
    for (auto iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
    {
        // ����Buff��ʣ��ʱ��
        iter->second->fTime -= delta;

        // ��ȡ��ǰBuff�����Ĺ������������˺�
        CMonster* monster = static_cast<CMonster*>(iter->first);
        if (monster->Damage(iter->second->nValue))
        {
            // �Ƴ����ﲢ��ֹѭ��
            monster->removeMonster();
            return;
        }

        // ���Buff�Ƿ��Ѿ���ʱ
        if (iter->second->fTime <= 0)
        {
            // ֹͣ���ж������Ӹ��ڵ��Ƴ�Buff��ͼ��
            auto buffSprite = iter->second->pSprite;
            buffSprite->stopAllActions();
            buffSprite->removeFromParent();

            // ��Buff Map ���Ƴ���ǰBuff������ȡ��һ��������
            iter = m_mapBuff.erase(iter);
        }
        else
        {
            // �ƶ�����һ��Buff
            ++iter;
        }
    }
}

