//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#include "GameScene.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "Monster.h"
#include "ThreadPool.h"  // Include thread pool header file
#include "Task.h"

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
	// Get all bullets and monsters from their respective layers
	Vector<Node*> VecBullet = m_pBulletLayer->getChildren();
	Vector<Node*> VecMonster = m_pMonsterLayer->getChildren();

	// Create collision detection tasks for each bullet-monster pair
	for (Node* pBulletNode : VecBullet) {
		for (Node* pMonsterNode : VecMonster) {
			// Create and submit a new collision detection task to the thread pool
			auto task = std::make_shared<CollisionTask>(pBulletNode, pMonsterNode, this);
			m_pThreadPool->enqueueTask(task);
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
