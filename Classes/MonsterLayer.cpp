#include "MonsterLayer.h"
#include "GameScene.h"
#include "DataMgr.h"

// Monster layer constructor
CMonsterLayer::CMonsterLayer()
{

}

// Monster layer destructor
CMonsterLayer::~CMonsterLayer()
{

}

// Initialize monster layer and set initial state
bool CMonsterLayer::init()
{
	// Call parent's init function
	if (!Node::init())
	{
		return false;
	}

	// Get level data manager
	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));

	// Get current level's wave and monster IDs
	m_vecWave = pLevelDtMgr->getCurData()->vecWave;  // Get current level's wave data
	m_nCurMonsterCount = 0;  // Initialize current monster count
	m_nCurWave = 0;  // Initialize current wave
	m_vecMonsterID = pLevelDtMgr->getCurData()->MonsterID;  // Get current level's monster ID list
	if (m_vecMonsterID.size() == 2)
	{
		m_nCurWaveMonsterID = 2002;
	}
	else
	{
		m_nCurWaveMonsterID = rand() % m_vecMonsterID.size() + 2002;  // Randomly select current wave's monster ID
	}

	// Schedule update function
	this->scheduleUpdate();

	return true;
}

// Create monster function
void CMonsterLayer::createMonster()
{
	// Get monster data manager
	CMonsterDtMgr* pMonsterDtMgr = static_cast<CMonsterDtMgr*>(CDataMgr::getInstance()->getData("MonsterMgr"));

	// Get current wave's monster data
	SMonsterDt* pMonsterDt = static_cast<SMonsterDt*>(pMonsterDtMgr->getDataByID(m_nCurWaveMonsterID));

	// Create monster instance
	CMonster* pMonster = CMonster::createWithData(pMonsterDt);
	this->addChild(pMonster);
	m_nCurMonsterCount++;

	// If current wave's monster count reaches limit, stop creating and switch to next wave
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
			m_nCurWaveMonsterID = rand() % m_vecMonsterID.size() + 2002;  // Randomly select next wave's monster ID
		}
	}
}

// Get monster instance within attack range at position
CMonster* CMonsterLayer::getMonster(float fAckRandge, Vec2 Pos)
{
	// Get all child nodes (all monsters)
	Vector<Node*> vecNode = this->getChildren();

	// Check each monster node
	for (Node* pNode : vecNode)
	{
		// Convert node to monster instance
		CMonster* pMonster = static_cast<CMonster*>(pNode);

		// Calculate distance to target position
		Vec2 deltaPos = pNode->getPosition() - Pos;

		// If distance is less than attack range, return this monster
		if (deltaPos.getLength() < fAckRandge)
		{
			return static_cast<CMonster*>(pNode);
		}
	}
	// If no monster found in range, return nullptr
	return nullptr;
}

// Update function
void CMonsterLayer::update(float delta)
{
	// If no monster is alive and current wave is not finished, create new monsters
	if (this->getChildrenCount() <= 0 && m_nCurWave < m_vecWave.size() && !this->isScheduled("Monster") && m_nCurWave != 0)
	{
		// Update UI to show current wave
		CGameScene::getInstance()->getUILayer()->setCurWave(m_nCurWave + 1);

		// Schedule monster creation
		this->schedule([=](float) {
			this->createMonster();
			}, 1.0f, "Monster");
	}

	// If no monster is alive and current wave is finished, show game over
	if (this->getChildrenCount() <= 0 && m_nCurWave >= m_vecWave.size())
	{
		// Show game over
		CGameScene::getInstance()->getUILayer()->createOver(m_nCurWave, m_vecWave.size(), true);
	}
}

// Start creating monsters
void CMonsterLayer::startCreatMonster()
{
	// Create a new monster and start creating monsters
	this->createMonster();
	m_nCurMonsterCount++;
	this->schedule([=](float) {
		this->createMonster();
		}, 1.0f, "Monster");
}

// Game over function
void CMonsterLayer::GameOver()
{
	// Show game over screen
	CGameScene::getInstance()->getUILayer()->createOver(m_nCurWave, m_vecWave.size(), false);
}

// Get monster vector within attack range at position
vector<CMonster*> CMonsterLayer::getMonsterVec(float fAckRandge, Vec2 Pos)
{
	// Get all child nodes (all monsters)
	Vector<Node*> vecNode = this->getChildren();

	// Store monster instances
	vector<CMonster*> vecMonster;

	// Check each monster node
	for (Node* pNode : vecNode)
	{
		// Calculate distance to target position
		Vec2 deltaPos = pNode->getPosition() - Pos;

		// If distance is less than attack range, add this monster to the vector
		if (deltaPos.getLength() <= fAckRandge)
		{
			// Add this monster to the vector
			vecMonster.push_back(static_cast<CMonster*>(pNode));
		}
	}
	// Return the vector of monster instances
	return vecMonster;
}

void CMonsterLayer::applySpecialSkill()
{
	// Get all child nodes (all monsters)
	Vector<Node*> vecNode = this->getChildren();

	// Check each monster node
	for (Node* pNode : vecNode)
	{
		// Convert node to monster instance
		CMonster* pMonster = static_cast<CMonster*>(pNode);

		// Reduce monster health
		if (pMonster->Damage(5000))
		{
			pMonster->removeMonster();
		}
	}
}

