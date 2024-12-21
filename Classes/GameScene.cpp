#include "GameScene.h"
#include "DataMgr.h"
#include "DataClass.h"

#include "Monster.h"
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
	CLevelDtMgr* pLevelDtMgr=static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));//拿到关卡数据管理者
	
	//获取当前关卡的数据
	SLevelDt* pLevelDt = pLevelDtMgr->getCurData();

	m_nMoney = 10000;//设定初始游戏货币数量

	//使用关卡数据中的地图路径创建了游戏地图对象 
	m_pGameMap = CGameMap::createWithImgPath(pLevelDt->strMapImg);
	this->addChild(m_pGameMap);//并将其添加到场景中

	CMonster::setPath(m_pGameMap->getInitPos());//设置怪物的路径，这里使用了地图对象的初始路径数据

	/***********************创建并添加各个图层和组件************************/
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

	/***********************创建并添加各个图层和组件************************/


	Sprite* pSprite = Sprite::create("Map/Radish01_01.png");//创建怪物生成地
	pSprite->setPosition(m_pGameMap->getFirstTiledPos());//设置这个精灵对象的位置为地图的初始位置
	this->addChild(pSprite);

	this->scheduleUpdate();//用了场景的 update() 函数，用于每一帧的更新
	return true;
}

CGameScene* CGameScene::getInstance()
{
	if (!m_spInstance)
	{
		m_spInstance =CGameScene::create();
	}
	return m_spInstance;
}

void CGameScene::update(float delta)
{
	//拿到所有子弹
	Vector<Node*> VecBullet = m_pBulletLayer->getChildren();
	for (Node* pBulletNode : VecBullet)
	{
		//拿到所有敌人
		Vector<Node*> VecMonster = m_pMonsterLayer->getChildren();
		for (Node* pMonsterNode : VecMonster)//遍历所有怪物节点
		{
			CMonster* pMonster = static_cast<CMonster*>(pMonsterNode);  //pMonsterNode 是怪物节点的指针，被转换为 CMonster 类型
			CBulletBase* pBullet = static_cast<CBulletBase*>(pBulletNode);  //pBulletNode 也被转换为 CBulletBase 类型
			Vec2 Pos = Vec2(pMonster->getPosition().x - pMonster->getModel()->getContentSize().width / 2, pMonster->getPosition().y - pMonster->getModel()->getContentSize().height / 2);
			Rect newRect = Rect(this->convertToNodeSpace(Pos), pMonster->getModel()->getContentSize());
			if (newRect.intersectsCircle(pBullet->getPosition(), 10))
			{
				pBullet->collisions(pMonsterNode);
				break;
			}
		}
	}
}

void CGameScene::deletInstance()
{
	CGameScene::getInstance()->unscheduleUpdate();
	CGameScene::getInstance()->removeAllChildren();
	m_spInstance = nullptr;
}

