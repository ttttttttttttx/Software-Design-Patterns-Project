#include "MonsterLayer.h"
#include "GameScene.h"
#include "DataMgr.h"

// �������Ĺ��캯��
CMonsterLayer::CMonsterLayer()
{

}

// ����������������
CMonsterLayer::~CMonsterLayer()
{

}

// ��ʼ���������������ù����ĳ�ʼ״̬
bool CMonsterLayer::init()
{
	// ���ø���ĳ�ʼ������
	if (!Node::init())
	{
		return false;
	}

	// ��ȡ�ؿ����ݹ�����
	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));

	// ��ȡ��ǰ�ؿ��Ĳ��κ͹���ID
	m_vecWave = pLevelDtMgr->getCurData()->vecWave;  // ��ȡ��ǰ�ؿ��Ĳ�������
	m_nCurMonsterCount = 0;  // ��ʼ����ǰ���ɵĹ�������
	m_nCurWave = 0;  // ��ʼ����ǰ����
	m_vecMonsterID = pLevelDtMgr->getCurData()->MonsterID;  // ��ȡ��ǰ�ؿ��Ĺ���ID����
	if (m_vecMonsterID.size() == 2)
	{
		m_nCurWaveMonsterID = 2002;
	}
	else
	{
		m_nCurWaveMonsterID = rand() % m_vecMonsterID.size() + 2002;  // ���ѡ��ǰ���εĹ���ID
	}
	

	// ����ÿ֡����
	this->scheduleUpdate();

	return true;
}

// ��������ķ���
void CMonsterLayer::createMonster()
{
	// ��ȡ�������ݹ�����
	CMonsterDtMgr* pMonsterDtMgr = static_cast<CMonsterDtMgr*>(CDataMgr::getInstance()->getData("MonsterMgr"));

	// ��ȡ��ǰ���εĹ�������
	SMonsterDt* pMonsterDt = static_cast<SMonsterDt*>(pMonsterDtMgr->getDataByID(m_nCurWaveMonsterID));

	// ��������ʵ��
	CMonster* pMonster = CMonster::createWithData(pMonsterDt);
	this->addChild(pMonster);
	m_nCurMonsterCount++;

	// ������ɵĹ��������ﵽ��ǰ���ε�������ֹͣ���ɲ��л�����һ����
	if (m_nCurMonsterCount >= m_vecWave[m_nCurWave])
	{
		this->unschedule("Monster");
		m_nCurMonsterCount = 0;
		m_nCurWave++;

		if (m_vecMonsterID.size() == 2)
		{
			m_nCurWaveMonsterID = 2001;
		}
		else
		{
			m_nCurWaveMonsterID = rand() % m_vecMonsterID.size() + 2002;  // ���ѡ��ǰ���εĹ���ID
		}
	}
}

// ���ݹ�����Χ��λ�û�ȡһ������ʵ���ķ���
CMonster* CMonsterLayer::getMonster(float fAckRandge, Vec2 Pos)
{
	// ��ȡ������е������ӽڵ㣨�����й��
	Vector<Node*> vecNode = this->getChildren();

	// ����ÿ������ڵ�
	for (Node* pNode : vecNode)
	{
		// ������ڵ�ת��Ϊ����ʵ��
		CMonster* pMonster = static_cast<CMonster*>(pNode);

		// �������ڵ���Ŀ��λ�õľ���
		Vec2 deltaPos = pNode->getPosition() - Pos;

		// �������С�ڹ�����Χ���򷵻ظù���ʵ��
		if (deltaPos.getLength() < fAckRandge)
		{
			return static_cast<CMonster*>(pNode);
		}
	}
	// ���û�з��������Ĺ���򷵻ؿ�ָ��
	return nullptr;
}


// ÿ֡���·��������ڴ���������߼���delta��ʾ��֮֡���ʱ����
void CMonsterLayer::update(float delta)
{
	// �����ǰû�й�����һ���δ���ɵĲ��Σ����ҵ�ǰδ�ڵ������ɹ����ʼ���ɹ���
	if (this->getChildrenCount() <= 0 && m_nCurWave < m_vecWave.size() && !this->isScheduled("Monster") && m_nCurWave != 0)
	{
		// ����UI��ʾ��ǰ����
		CGameScene::getInstance()->getUILayer()->setCurWave(m_nCurWave + 1);

		// �������ɹ���
		this->schedule([=](float) {
			this->createMonster();
			}, 1.0f, "Monster");
	}

	// �����ǰû�й�������Ѿ����������в���
	if (this->getChildrenCount() <= 0 && m_nCurWave >= m_vecWave.size())
	{
		// ��Ϸ����
		CGameScene::getInstance()->getUILayer()->createOver(m_nCurWave, m_vecWave.size(), true);
	}
}

// ��ʼ���ɹ���ķ���
void CMonsterLayer::startCreatMonster()
{
	// ������һ�����ﲢ�������ɹ���
	this->createMonster();
	m_nCurMonsterCount++;
	this->schedule([=](float) {
		this->createMonster();
		}, 1.0f, "Monster");
}

// ��Ϸ�����ķ���
void CMonsterLayer::GameOver()
{
	// ��Ϸ����ʱ������Ϸ�������
	CGameScene::getInstance()->getUILayer()->createOver(m_nCurWave, m_vecWave.size(), false);
}

// ���ݹ�����Χ��λ�û�ȡһ�����ʵ���ķ���
vector<CMonster*> CMonsterLayer::getMonsterVec(float fAckRandge, Vec2 Pos)
{
	// ��ȡ�����������ӽڵ�
	Vector<Node*> vecNode = this->getChildren();

	// �洢���������Ĺ���ʵ��������
	vector<CMonster*> vecMonster;

	// ���������ӽڵ�
	for (Node* pNode : vecNode)
	{
		// ���������Ŀ��λ�õ�ƫ������
		Vec2 deltaPos = pNode->getPosition() - Pos;

		// ���ƫ�������ĳ���С�ڵ��ڹ�����Χ
		if (deltaPos.getLength() <= fAckRandge)
		{
			// �����������Ĺ���ʵ����������
			vecMonster.push_back(static_cast<CMonster*>(pNode));
		}
	}
	// �������������Ĺ���ʵ������
	return vecMonster;
}



void CMonsterLayer::applySpecialSkill()
{
	// ��ȡ������е������ӽڵ㣨�����й��
	Vector<Node*> vecNode = this->getChildren();

	// ����ÿ������ڵ�
	for (Node* pNode : vecNode)
	{
		// ������ڵ�ת��Ϊ����ʵ��
		CMonster* pMonster = static_cast<CMonster*>(pNode);

		// ���ٹ���Ѫ��
		if (pMonster->Damage(5000))
		{
			pMonster->removeMonster();
		}
	}
}

