#include "GameScene.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "Monster.h"

// Initialize singleton instance
CGameScene* CGameScene::m_spInstance = nullptr;

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

// Initialize game scene
bool CGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	
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

	// Added for Observer Pattern - Create and register observers
	auto uiObserver = new UIObserver();
	auto buffObserver = new BuffObserver();
	auto animateObserver = new AnimateObserver();
	auto audioObserver = new AudioObserver();
	
	// Observer Pattern - Register observers with monster layer
	m_pMonsterLayer->setObservers({uiObserver, buffObserver, 
								  animateObserver, audioObserver});

	return true;
}

// Get singleton instance
CGameScene* CGameScene::getInstance()
{
	if (!m_spInstance)
	{
		m_spInstance = CGameScene::create();
	}
	return m_spInstance;
}

// Update game logic
void CGameScene::update(float delta)
{
	// Get all bullets
	Vector<Node*> VecBullet = m_pBulletLayer->getChildren();
	for (Node* pBulletNode : VecBullet)
	{
		// Get all monsters
		Vector<Node*> VecMonster = m_pMonsterLayer->getChildren();
		for (Node* pMonsterNode : VecMonster)
		{
			CMonster* pMonster = static_cast<CMonster*>(pMonsterNode);
			CBulletBase* pBullet = static_cast<CBulletBase*>(pBulletNode);
			
			// Check collision
			Vec2 Pos = Vec2(pMonster->getPosition().x - pMonster->getModel()->getContentSize().width / 2,
						   pMonster->getPosition().y - pMonster->getModel()->getContentSize().height / 2);
			Rect newRect = Rect(this->convertToNodeSpace(Pos), pMonster->getModel()->getContentSize());
			
			if (newRect.intersectsCircle(pBullet->getPosition(), 10))
			{
				pBullet->collisions(pMonsterNode);
				break;
			}
		}
	}
}

// Delete singleton instance
void CGameScene::deletInstance()
{
	CGameScene::getInstance()->unscheduleUpdate();
	CGameScene::getInstance()->removeAllChildren();
	m_spInstance = nullptr;
}

