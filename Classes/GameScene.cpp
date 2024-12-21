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
	CLevelDtMgr* pLevelDtMgr=static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));//�õ��ؿ����ݹ�����
	
	//��ȡ��ǰ�ؿ�������
	SLevelDt* pLevelDt = pLevelDtMgr->getCurData();

	m_nMoney = 10000;//�趨��ʼ��Ϸ��������

	//ʹ�ùؿ������еĵ�ͼ·����������Ϸ��ͼ���� 
	m_pGameMap = CGameMap::createWithImgPath(pLevelDt->strMapImg);
	this->addChild(m_pGameMap);//��������ӵ�������

	CMonster::setPath(m_pGameMap->getInitPos());//���ù����·��������ʹ���˵�ͼ����ĳ�ʼ·������

	/***********************��������Ӹ���ͼ������************************/
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

	/***********************��������Ӹ���ͼ������************************/


	Sprite* pSprite = Sprite::create("Map/Radish01_01.png");//�����������ɵ�
	pSprite->setPosition(m_pGameMap->getFirstTiledPos());//���������������λ��Ϊ��ͼ�ĳ�ʼλ��
	this->addChild(pSprite);

	this->scheduleUpdate();//���˳����� update() ����������ÿһ֡�ĸ���
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
	//�õ������ӵ�
	Vector<Node*> VecBullet = m_pBulletLayer->getChildren();
	for (Node* pBulletNode : VecBullet)
	{
		//�õ����е���
		Vector<Node*> VecMonster = m_pMonsterLayer->getChildren();
		for (Node* pMonsterNode : VecMonster)//�������й���ڵ�
		{
			CMonster* pMonster = static_cast<CMonster*>(pMonsterNode);  //pMonsterNode �ǹ���ڵ��ָ�룬��ת��Ϊ CMonster ����
			CBulletBase* pBullet = static_cast<CBulletBase*>(pBulletNode);  //pBulletNode Ҳ��ת��Ϊ CBulletBase ����
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

