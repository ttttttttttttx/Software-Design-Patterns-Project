#include "MonsterLayer.h"
#include "GameScene.h"
#include "DataMgr.h"

// 怪物层类的构造函数
CMonsterLayer::CMonsterLayer()
{

}

// 怪物层类的析构函数
CMonsterLayer::~CMonsterLayer()
{

}

// 初始化方法，用于设置怪物层的初始状态
bool CMonsterLayer::init()
{
	// 调用父类的初始化方法
	if (!Node::init())
	{
		return false;
	}

	// 获取关卡数据管理者
	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));

	// 获取当前关卡的波次和怪物ID
	m_vecWave = pLevelDtMgr->getCurData()->vecWave;  // 获取当前关卡的波次配置
	m_nCurMonsterCount = 0;  // 初始化当前生成的怪物数量
	m_nCurWave = 0;  // 初始化当前波次
	m_vecMonsterID = pLevelDtMgr->getCurData()->MonsterID;  // 获取当前关卡的怪物ID配置
	if (m_vecMonsterID.size() == 2)
	{
		m_nCurWaveMonsterID = 2002;
	}
	else
	{
		m_nCurWaveMonsterID = rand() % m_vecMonsterID.size() + 2002;  // 随机选择当前波次的怪物ID
	}
	

	// 开启每帧更新
	this->scheduleUpdate();

	return true;
}

// 创建怪物的方法
void CMonsterLayer::createMonster()
{
	// 获取怪物数据管理者
	CMonsterDtMgr* pMonsterDtMgr = static_cast<CMonsterDtMgr*>(CDataMgr::getInstance()->getData("MonsterMgr"));

	// 获取当前波次的怪物数据
	SMonsterDt* pMonsterDt = static_cast<SMonsterDt*>(pMonsterDtMgr->getDataByID(m_nCurWaveMonsterID));

	// 创建怪物实例
	CMonster* pMonster = CMonster::createWithData(pMonsterDt);
	this->addChild(pMonster);
	m_nCurMonsterCount++;

	// 如果生成的怪物数量达到当前波次的总数，停止生成并切换到下一波次
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
			m_nCurWaveMonsterID = rand() % m_vecMonsterID.size() + 2002;  // 随机选择当前波次的怪物ID
		}
	}
}

// 根据攻击范围和位置获取一个怪物实例的方法
CMonster* CMonsterLayer::getMonster(float fAckRandge, Vec2 Pos)
{
	// 获取怪物层中的所有子节点（即所有怪物）
	Vector<Node*> vecNode = this->getChildren();

	// 遍历每个怪物节点
	for (Node* pNode : vecNode)
	{
		// 将怪物节点转换为怪物实例
		CMonster* pMonster = static_cast<CMonster*>(pNode);

		// 计算怪物节点与目标位置的距离
		Vec2 deltaPos = pNode->getPosition() - Pos;

		// 如果距离小于攻击范围，则返回该怪物实例
		if (deltaPos.getLength() < fAckRandge)
		{
			return static_cast<CMonster*>(pNode);
		}
	}
	// 如果没有符合条件的怪物，则返回空指针
	return nullptr;
}


// 每帧更新方法，用于处理怪物层的逻辑，delta表示两帧之间的时间间隔
void CMonsterLayer::update(float delta)
{
	// 如果当前没有怪物，并且还有未生成的波次，并且当前未在调度生成怪物，则开始生成怪物
	if (this->getChildrenCount() <= 0 && m_nCurWave < m_vecWave.size() && !this->isScheduled("Monster") && m_nCurWave != 0)
	{
		// 更新UI显示当前波次
		CGameScene::getInstance()->getUILayer()->setCurWave(m_nCurWave + 1);

		// 调度生成怪物
		this->schedule([=](float) {
			this->createMonster();
			}, 1.0f, "Monster");
	}

	// 如果当前没有怪物，并且已经生成了所有波次
	if (this->getChildrenCount() <= 0 && m_nCurWave >= m_vecWave.size())
	{
		// 游戏结束
		CGameScene::getInstance()->getUILayer()->createOver(m_nCurWave, m_vecWave.size(), true);
	}
}

// 开始生成怪物的方法
void CMonsterLayer::startCreatMonster()
{
	// 创建第一个怪物并调度生成怪物
	this->createMonster();
	m_nCurMonsterCount++;
	this->schedule([=](float) {
		this->createMonster();
		}, 1.0f, "Monster");
}

// 游戏结束的方法
void CMonsterLayer::GameOver()
{
	// 游戏结束时创建游戏结算界面
	CGameScene::getInstance()->getUILayer()->createOver(m_nCurWave, m_vecWave.size(), false);
}

// 根据攻击范围和位置获取一组怪物实例的方法
vector<CMonster*> CMonsterLayer::getMonsterVec(float fAckRandge, Vec2 Pos)
{
	// 获取怪物层的所有子节点
	Vector<Node*> vecNode = this->getChildren();

	// 存储满足条件的怪物实例的向量
	vector<CMonster*> vecMonster;

	// 遍历所有子节点
	for (Node* pNode : vecNode)
	{
		// 计算怪物与目标位置的偏移向量
		Vec2 deltaPos = pNode->getPosition() - Pos;

		// 如果偏移向量的长度小于等于攻击范围
		if (deltaPos.getLength() <= fAckRandge)
		{
			// 将满足条件的怪物实例加入向量
			vecMonster.push_back(static_cast<CMonster*>(pNode));
		}
	}
	// 返回满足条件的怪物实例向量
	return vecMonster;
}



void CMonsterLayer::applySpecialSkill()
{
	// 获取怪物层中的所有子节点（即所有怪物）
	Vector<Node*> vecNode = this->getChildren();

	// 遍历每个怪物节点
	for (Node* pNode : vecNode)
	{
		// 将怪物节点转换为怪物实例
		CMonster* pMonster = static_cast<CMonster*>(pNode);

		// 减少怪物血量
		if (pMonster->Damage(5000))
		{
			pMonster->removeMonster();
		}
	}
}

