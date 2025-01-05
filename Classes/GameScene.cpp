//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#include "GameScene.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "Monster.h"
#include "ThreadPool.h"  // Include thread pool header file

CGameScene* CGameScene::m_spInstance = nullptr;
CGameScene::CGameScene()
{

}

CGameScene::~CGameScene()
{

}

bool CGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// Initialize thread pool, assuming 4 threads to process tasks in parallel
	m_pThreadPool = new ThreadPool(4);


	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr")); // Get the level data manager

	SLevelDt* pLevelDt = pLevelDtMgr->getCurData();

	m_nMoney = 10000;

	m_pGameMap = CGameMap::createWithImgPath(pLevelDt->strMapImg);
	this->addChild(m_pGameMap);

	CMonster::setPath(m_pGameMap->getInitPos());

	m_pMonsterLayer = CMonsterLayer::create();
	this->addChild(m_pMonsterLayer);

	myAnimate = MyAnimate::create();
	this->addChild(myAnimate);


	m_pBulletLayer = CBulletLayer::create();
	this->addChild(m_pBulletLayer);

	myArms = BuildArms::create();
	this->addChild(myArms);


	m_pBuffLayer = CBuffLayer::create();
	this->addChild(m_pBuffLayer);


	m_pRadish = CRadish::create();
	m_pRadish->setPosition(m_pGameMap->getLastTiledPos());
	this->addChild(m_pRadish);

	m_pUILayer = CUILayer::create();
	this->addChild(m_pUILayer);

	myCard = ArmsCard::create();
	this->addChild(myCard);

	myUpCard = UpCard::create();
	this->addChild(myUpCard);

	Sprite* pSprite = Sprite::create("Map/Radish01_01.png");
	pSprite->setPosition(m_pGameMap->getFirstTiledPos());
	this->addChild(pSprite);

	this->scheduleUpdate();
	return true;
}

CGameScene* CGameScene::getInstance()
{
	if (!m_spInstance)
	{
		m_spInstance = CGameScene::create();
	}
	return m_spInstance;
}

// Submit collision detection tasks to the thread pool
void CGameScene::update(float delta) {
	Vector<Node*> VecBullet = m_pBulletLayer->getChildren();  // Get all bullets
	Vector<Node*> VecMonster = m_pMonsterLayer->getChildren();  // Get all monsters

	// Iterate over all bullets and monsters, create collision detection tasks
	for (Node* pBulletNode : VecBullet) {
		for (Node* pMonsterNode : VecMonster) {
			// Create a task for each bullet-monster collision detection and submit it to the thread pool
			m_pThreadPool->enqueueTask([=]() {
				// Task body: Handle bullet and monster collision detection
				CMonster* pMonster = static_cast<CMonster*>(pMonsterNode);  // Get monster object
				CBulletBase* pBullet = static_cast<CBulletBase*>(pBulletNode);  // Get bullet object

				// Calculate the monster's rectangle area
				Vec2 Pos = Vec2(pMonster->getPosition().x - pMonster->getModel()->getContentSize().width / 2,
					pMonster->getPosition().y - pMonster->getModel()->getContentSize().height / 2);
				Rect newRect = Rect(this->convertToNodeSpace(Pos), pMonster->getModel()->getContentSize());

				// Check if the bullet intersects with the monster
				if (newRect.intersectsCircle(pBullet->getPosition(), 10)) {
					// If collision occurs, handle collision
					Director::getInstance()->getScheduler()->performFunctionInMainThread([=]() {
						pBullet->collisions(pMonsterNode);  // Collision handler must run on the main thread
						});
				}
				});
		}
	}
}

void CGameScene::deletInstance()
{
	CGameScene::getInstance()->unscheduleUpdate();
	CGameScene::getInstance()->removeAllChildren();
	m_spInstance = nullptr;
}

// Clean up thread pool
CGameScene::~CGameScene() {
	delete m_pThreadPool;  // Delete thread pool, destroying all threads
	// Other cleanup operations...
}
