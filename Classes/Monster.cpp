#include "Monster.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

// Initialize monster path
vector<Vec2> CMonster::m_vecPath;

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
}

// Initialize monster with data
bool CMonster::initWithData(SMonsterDt* pMonster)
{
	if (!CRoleBase::init()) return false;

	initMonsterProperties(pMonster);

	createAndSetMonsterModel(pMonster);

	setHpNodePosition();

	setInitialPositionAndDirection();

	scheduleUpdate();

	return true;
}

void CMonster::initMonsterProperties(SMonsterDt* pMonster)
{
	// Initialize basic properties
	m_nIndex = 0;
	m_TiledSize = CGameScene::getInstance()->getGameMap()->getTiledSize();
	m_fSaveSpeed = m_fSpeed = pMonster->fSpeed;
	m_bChangeDir = false;
	if (pMonster->fSpeed == 10)
	{
		m_nHp = m_nMaxHp = 600000;
	}
	else
	{
		m_nHp = m_nMaxHp = 10000;
	}
	m_nMoney = pMonster->nMoney;
}

void CMonster::createAndSetMonsterModel(SMonsterDt* pMonster)
{
	m_pModel = Sprite::createWithSpriteFrameName(pMonster->strImg);

	CGameScene::getInstance()->getMyAnimate()->changeAction(m_pModel, pMonster->AnimateID, true, 0.2f);

	this->addChild(m_pModel);
}

void CMonster::setHpNodePosition()
{
	m_pHpNode->setPosition(0, m_pModel->getContentSize().height / 2);
}

void CMonster::setInitialPositionAndDirection()
{
	this->setPosition(m_vecPath[m_nIndex]);

	calculateDir();
}

// Create monster instance with data
CMonster* CMonster::createWithData(SMonsterDt* pMonster)
{
	CMonster* pRef = new (std::nothrow) CMonster();

	if (pRef && pRef->initWithData(pMonster))
	{
		return pRef;
	}

	delete pRef;
	return nullptr;
}

// Update per frame
void CMonster::update(float delta)
{
	if (this->changeUpdateDir())
	{
		if (m_nMoney == 999)
		{
			CGameScene::getInstance()->getRadish()->Damage(10);
		}
		else
		{
			CGameScene::getInstance()->getRadish()->Damage(1);
		}
		
		CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), 3024);

		this->removeMonster();

		return;
	}

	Vec2 pos = this->getPosition();
	pos.x += delta * m_vDir.x * m_fSpeed;
	pos.y += delta * m_vDir.y * m_fSpeed;
	this->setPosition(pos);
}

// Check if position has path property
bool CMonster::hasPathProperty(const Vec2& pos)
{
	ValueMap vmap = CGameScene::getInstance()->getGameMap()->getProperty("path", pos);
	return vmap.size() != 0 && vmap["point"].asBool();
}

// Update direction and index
bool CMonster::updateDirectionAndIndex()
{
	m_bChangeDir = true;
	m_nIndex++;
	this->calculateDir();

	return (m_nIndex >= m_vecPath.size() - 1);
}

// Change update direction
bool CMonster::changeUpdateDir()
{
	Vec2 monsterPos = this->getPosition();
	Vec2 nextPos = Vec2(monsterPos.x + m_vDir.x * (m_TiledSize.width / 2 + 3), 
					   monsterPos.y + m_vDir.y * (m_TiledSize.height / 2 + 3));
	Vec2 nextTiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(nextPos);
	Vec2 tiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(monsterPos);

	if (nextTiled == tiled)
	{
		m_bChangeDir = false;
	}

	if (CGameScene::getInstance()->getGameMap()->isOutOfMap(nextTiled))
	{
		return updateDirectionAndIndex();
	}
	else if (CGameScene::getInstance()->getGameMap()->isInLayer("path", nextTiled))
	{
		if (!m_bChangeDir && hasPathProperty(tiled) && nextTiled != tiled)
		{
			return updateDirectionAndIndex();
		}
	}
	else
	{
		return updateDirectionAndIndex();
	}

	return false;
}

// Calculate direction
void CMonster::calculateDir()
{
	if (m_nIndex + 1 < m_vecPath.size())
	{
		Vec2 pos = m_vecPath[m_nIndex + 1] - m_vecPath[m_nIndex];
		m_vDir = pos.getNormalized();
	}
}

// Set monster path
void CMonster::setPath(vector<Vec2> vecPath)
{
	m_vecPath = vecPath;
}

// Observer Pattern implementation
void CMonster::addObserver(IMonsterObserver* observer) {
	m_observers.push_back(observer);
}

void CMonster::removeObserver(IMonsterObserver* observer) {
	auto it = std::find(m_observers.begin(), m_observers.end(), observer);
	if (it != m_observers.end()) {
		m_observers.erase(it);
	}
}

void CMonster::notifyObservers() {
	for (auto observer : m_observers) {
		observer->onMonsterDeath(this);
	}
}

// Refactored removeMonster method
void CMonster::removeMonster() {
	// Notify all observers about monster death
	notifyObservers();
	
	// Remove monster from parent node
	this->removeFromParent();
}

