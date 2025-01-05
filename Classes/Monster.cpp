#include "Monster.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

// Initialize the monster path
vector<Vec2> CMonster::m_vecPath;

// Constructor of the monster class
CMonster::CMonster()
{
}

// Destructor of the monster class
CMonster::~CMonster()
{
}

//------------------------------------Refactored with Factory Method Pattern--------------------------------------//

// 1. Define concrete product class CNormalMonster
// CNormalMonster is a normal monster class that inherits from CMonster and implements initWithData method
class CNormalMonster : public CMonster {
public:
    bool initWithData(SMonsterDt* pMonster) override {
        if (!CRoleBase::init()) return false;

        initMonsterProperties(pMonster); // Initialize monster properties
        // No further modifications
        return true;
    }
};

// 2. Define concrete product class CEliteMonster
// CEliteMonster is an elite monster class that inherits from CMonster and implements initWithData method
class CEliteMonster : public CMonster {
public:
    bool initWithData(SMonsterDt* pMonster) override {
        if (!CRoleBase::init()) return false;

        initMonsterProperties(pMonster); // Initialize monster properties
        m_nHp *= 2; // Double the HP
        return true;
    }
};

// 3: Define concrete product class CBossMonster
// CBossMonster is a boss monster class that inherits from CMonster and implements initWithData method
class CBossMonster : public CMonster {
public:
    bool initWithData(SMonsterDt* pMonster) override {
        if (!CRoleBase::init()) return false;

        initMonsterProperties(pMonster); // Initialize monster properties
        m_nHp *= 5;    // Significantly increase HP
        m_fSpeed *= 0.5; // Decrease speed
        return true;
    }
};

// 4: Define abstract factory class CMonsterFactory
// This factory class is responsible for creating different monster instances based on the monster type
class CMonsterFactory {
public:
    virtual CMonster* createMonster(SMonsterDt* pMonster) = 0; // Create monster instance
    virtual ~CMonsterFactory() = default; // Virtual destructor
};

// 5: Define concrete factory class CNormalMonsterFactory
// This factory class is responsible for creating normal monster instances
class CNormalMonsterFactory : public CMonsterFactory {
public:
    CMonster* createMonster(SMonsterDt* pMonster) override {
        CNormalMonster* pMonsterObj = new CNormalMonster();
        pMonsterObj->initWithData(pMonster); // Initialize monster
        return pMonsterObj; // Return created monster
    }
};

// 6: Define concrete factory class CEliteMonsterFactory
// This factory class is responsible for creating elite monster instances
class CEliteMonsterFactory : public CMonsterFactory {
public:
    CMonster* createMonster(SMonsterDt* pMonster) override {
        CEliteMonster* pMonsterObj = new CEliteMonster();
        pMonsterObj->initWithData(pMonster); // Initialize monster
        return pMonsterObj; // Return created monster
    }
};

// 7: Define concrete factory class CBossMonsterFactory
// This factory class is responsible for creating boss monster instances
class CBossMonsterFactory : public CMonsterFactory {
public:
    CMonster* createMonster(SMonsterDt* pMonster) override {
        CBossMonster* pMonsterObj = new CBossMonster();
        pMonsterObj->initWithData(pMonster); // Initialize monster
        return pMonsterObj; // Return created monster
    }
};

// 8: Using the factory method for object creation
// Use the factory classes to create monster instances instead of directly creating CMonster objects
class CMonster {
public:
    bool initWithData(SMonsterDt* pMonsterData) {
        CMonsterFactory* pFactory;
        CMonster* pMonster = nullptr;

        // Choose different factories based on monster type
        switch (pMonsterData->type) {
            case MonsterType::Normal:
                pFactory = new CNormalMonsterFactory();
                break;
            case MonsterType::Elite:
                pFactory = new CEliteMonsterFactory();
                break;
            case MonsterType::Boss:
                pFactory = new CBossMonsterFactory();
                break;
            default:
                CCLOG("Unknown monster type!");
                return false;
        }

        // Use the factory to create the monster
        pMonster = pFactory->createMonster(pMonsterData);
        delete pFactory;  // Delete the factory object

        // Initialize other monster properties
        initMonsterProperties(pMonsterData);
        // You can also call other methods to initialize the monster, such as model, health bar, etc.

        // Return success
        return true;
    }
};

void CMonster::initMonsterProperties(SMonsterDt* pMonster)
{
    // Initialize monster properties
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
    // Create the monster model
    m_pModel = Sprite::createWithSpriteFrameName(pMonster->strImg);

    // Switch the monster model's animation, parameters include sprite, animation ID, loop flag, and frame interval
    CGameScene::getInstance()->getMyAnimate()->changeAction(m_pModel, pMonster->AnimateID, true, 0.2f);

    // Add the monster model to the monster object
    this->addChild(m_pModel);
}

void CMonster::setHpNodePosition()
{
    // Set the monster health bar position
    m_pHpNode->setPosition(0, m_pModel->getContentSize().height / 2);
}

void CMonster::setInitialPositionAndDirection()
{
    // Set the initial position of the monster
    this->setPosition(m_vecPath[m_nIndex]);

    // Calculate the initial direction of the monster
    calculateDir();
}

// Static method to create a monster instance based on monster data
CMonster* CMonster::createWithData(SMonsterDt* pMonster)
{
    CMonster* pRef = new (std::nothrow) CMonster();

    // Check if the instance is successfully created and initialize it
    if (pRef && pRef->initWithData(pMonster))
    {
        return pRef;  // Return the created monster instance
    }

    delete pRef;  // Use delete to release memory

    return nullptr;  // Return nullptr to indicate creation failure
}

// The monster class's update method for each frame
void CMonster::update(float delta)
{
    // If the monster has changed direction, check if it should be removed
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

        // Add monster death animation
        CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), 3024);

        // Remove the monster
        this->removeMonster();

        return;
    }

    // Move the monster
    Vec2 pos = this->getPosition();
    pos.x += delta * m_vDir.x * m_fSpeed;
    pos.y += delta * m_vDir.y * m_fSpeed;
    this->setPosition(pos);
}

// Helper method: check if a specified position has path properties
bool CMonster::hasPathProperty(const Vec2& pos)
{
    // Get the path properties of the specified position
    ValueMap vmap = CGameScene::getInstance()->getGameMap()->getProperty("path", pos);
    return vmap.size() != 0 && vmap["point"].asBool();
}

// Helper method: update the monster's direction and index
bool CMonster::updateDirectionAndIndex()
{
    // Mark that the direction needs to change
    m_bChangeDir = true;
    // Increment the path point index
    m_nIndex++;
    // Recalculate the direction
    this->calculateDir();

    // Return whether the monster needs to be removed
    return (m_nIndex >= m_vecPath.size() - 1);
}

// Method to change the monster's update direction
bool CMonster::changeUpdateDir()
{
    // Get the current position of the monster
    Vec2 monsterPos = this->getPosition();
    // Calculate the next grid position of the monster
    Vec2 nextPos = Vec2(monsterPos.x + m_vDir.x * (m_TiledSize.width / 2 + 3), monsterPos.y + m_vDir.y * (m_TiledSize.height / 2 + 3));
    // Get the next grid position of the monster
    Vec2 nextTiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(nextPos);
    // Get the current grid position of the monster
    Vec2 tiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(monsterPos);

    // If the next grid and the current grid are the same, it means the monster has not changed direction
    if (nextTiled == tiled)
    {
        m_bChangeDir = false;
    }

    // If the next grid is out of bounds
    if (CGameScene::getInstance()->getGameMap()->isOutOfMap(nextTiled))
    {
        return updateDirectionAndIndex();
    }
    // If the next grid is a path
    else if (CGameScene::getInstance()->getGameMap()->isInLayer("path", nextTiled))
    {
        // If the direction hasn't changed and the current grid has a path property
        if (!m_bChangeDir && hasPathProperty(tiled) && nextTiled != tiled)
        {
            return updateDirectionAndIndex();
        }
    }
    // If the next grid is not a path
    else
    {
        return updateDirectionAndIndex();
    }

    // Return whether the monster needs to be removed
    return false;
}

// Method to calculate the monster's direction
void CMonster::calculateDir()
{
    // If the current path index is less than the total number of paths
    if (m_nIndex + 1 < m_vecPath.size())
    {
        // Calculate the direction vector
        Vec2 pos = m_vecPath[m_nIndex + 1] - m_vecPath[m_nIndex];
        m_vDir = pos.getNormalized();
    }
    else
    {
        // Handle out-of-bounds case, such as logging an error or other processing
    }
}

// Static method to set the monster's path
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

// Refactored with Observer Pattern - monster removal
void CMonster::removeMonster() {
	// Notify all observers about monster death
	notifyObservers();
	
	// Remove monster from parent node
	this->removeFromParent();
}