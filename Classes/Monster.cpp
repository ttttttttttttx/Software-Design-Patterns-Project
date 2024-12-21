#include "Monster.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

// ��ʼ������·��
vector<Vec2> CMonster::m_vecPath;

// ������Ĺ��캯��
CMonster::CMonster()
{
}

// ���������������
CMonster::~CMonster()
{
}

// ʹ�ù������ݳ�ʼ������ʵ���ķ���
bool CMonster::initWithData(SMonsterDt* pMonster)
{
	if (!CRoleBase::init()) return false;

	// ��ʼ����������
	initMonsterProperties(pMonster);

	// ���������ù����ģ��
	createAndSetMonsterModel(pMonster);

	// ���ù���Ѫ��λ��
	setHpNodePosition();

	// ���ù����ʼλ�úͷ���
	setInitialPositionAndDirection();

	// ����ÿ֡����
	scheduleUpdate();

	return true;
}

void CMonster::initMonsterProperties(SMonsterDt* pMonster)
{
	// ��ʼ����������
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
	// ���������ģ��
	m_pModel = Sprite::createWithSpriteFrameName(pMonster->strImg);

	// �л�����ģ�͵Ķ����������������顢����ID���Ƿ�ѭ��������֡���ʱ��
	CGameScene::getInstance()->getMyAnimate()->changeAction(m_pModel, pMonster->AnimateID, true, 0.2f);

	// ������ģ����ӵ����������
	this->addChild(m_pModel);
}

void CMonster::setHpNodePosition()
{
	// ���ù���Ѫ��λ��
	m_pHpNode->setPosition(0, m_pModel->getContentSize().height / 2);
}

void CMonster::setInitialPositionAndDirection()
{
	// ���ù����ʼλ��
	this->setPosition(m_vecPath[m_nIndex]);

	// ��������ʼ����
	calculateDir();
}

// ���ݹ������ݴ�������ʵ���ľ�̬����
CMonster* CMonster::createWithData(SMonsterDt* pMonster)
{
	CMonster* pRef = new (std::nothrow) CMonster();

	// �ж�ʵ���Ƿ񴴽��ɹ��������г�ʼ��
	if (pRef && pRef->initWithData(pMonster))
	{
		return pRef;  // ���ش����Ĺ���ʵ��
	}

	delete pRef;  // ʹ�� delete �ͷ��ڴ�

	return nullptr;  // ���ؿ�ָ���ʾ����ʧ��
}

// �������ÿ֡���·���
void CMonster::update(float delta)
{
	// �������ı��˷��򣬼���Ƿ���Ҫ�Ƴ�����
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
		
		// ��ӹ�����������
		CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), 3024);

		// �Ƴ�����
		this->removeMonster();

		return;
	}

	// �ƶ�����
	Vec2 pos = this->getPosition();
	pos.x += delta * m_vDir.x * m_fSpeed;
	pos.y += delta * m_vDir.y * m_fSpeed;
	this->setPosition(pos);
}

// �������������ָ��λ���Ƿ����·������
bool CMonster::hasPathProperty(const Vec2& pos)
{
	// ��ȡָ��λ�õ�·������
	ValueMap vmap = CGameScene::getInstance()->getGameMap()->getProperty("path", pos);
	return vmap.size() != 0 && vmap["point"].asBool();
}

// �������������¹���ķ��������
bool CMonster::updateDirectionAndIndex()
{
	// �����Ҫ�ı䷽��
	m_bChangeDir = true;
	// ����·��������
	m_nIndex++;
	// ���¼��㷽��
	this->calculateDir();

	// �����Ƿ���Ҫ�Ƴ�����
	return (m_nIndex >= m_vecPath.size() - 1);
}

// �ı������·���ķ���
bool CMonster::changeUpdateDir()
{
	// ��ȡ���ﵱǰ����
	Vec2 monsterPos = this->getPosition();
	// ���������һ����������
	Vec2 nextPos = Vec2(monsterPos.x + m_vDir.x * (m_TiledSize.width / 2 + 3), monsterPos.y + m_vDir.y * (m_TiledSize.height / 2 + 3));
	// ��ȡ�������һ����������
	Vec2 nextTiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(nextPos);
	// ��ȡ����ĵ�ǰ��������
	Vec2 tiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(monsterPos);

	// �����һ�����Ӻ͵�ǰ������ͬ����ʾ����δ�ı䷽��
	if (nextTiled == tiled)
	{
		m_bChangeDir = false;
	}

	// �����һ�����ӳ���
	if (CGameScene::getInstance()->getGameMap()->isOutOfMap(nextTiled))
	{
		return updateDirectionAndIndex();
	}
	// �����һ��������·��
	else if (CGameScene::getInstance()->getGameMap()->isInLayer("path", nextTiled))
	{
		// ���δ�ı䷽���ҵ�ǰ������·������
		if (!m_bChangeDir && hasPathProperty(tiled) && nextTiled != tiled)
		{
			return updateDirectionAndIndex();
		}
	}
	// �����һ�����Ӳ���·��
	else
	{
		return updateDirectionAndIndex();
	}

	// �����Ƿ���Ҫ�Ƴ�����
	return false;
}

// ������﷽��ķ���
void CMonster::calculateDir()
{
	// �����ǰ·������С��·������
	if (m_nIndex + 1 < m_vecPath.size())
	{
		// ���㷽������
		Vec2 pos = m_vecPath[m_nIndex + 1] - m_vecPath[m_nIndex];
		m_vDir = pos.getNormalized();
	}
	else
	{
		// ����Խ���������������������־�������������
	}
}

// ���ù���·���ľ�̬����
void CMonster::setPath(vector<Vec2> vecPath)
{
	m_vecPath = vecPath;
}

// �Ƴ�����ķ���
void CMonster::removeMonster()
{
	// �Ƴ��������ϵ�Buff
	CGameScene::getInstance()->getBuffLayer()->removeBuff(this);

	// ��������ߵ���·�����յ㣬�̶���ҽ���Ϊ14
	if (m_nIndex >= m_vecPath.size() - 1)
	{
		m_nMoney = 14;
	}

	// ������ҽ������
	CGameScene::getInstance()->getUILayer()->addMoney(m_nMoney);

	// ������Ҿ���
	Sprite* pSprite = Sprite::createWithSpriteFrameName(StringUtils::format("money%d.png", m_nMoney));
	pSprite->setPosition(this->getPosition());
	MoveBy* pMoveBy = MoveBy::create(0.5, Vec2(0, 50));
	pSprite->runAction(Sequence::createWithTwoActions(pMoveBy, RemoveSelf::create()));
	CGameScene::getInstance()->getUILayer()->addChild(pSprite);

	// ��ӹ�����������
	CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), 3024);

	//����������Ч
	AudioEngine::play2d("sound/monsterDie.mp3", false, 1.1f);

	// �Ӹ��ڵ��Ƴ�����
	this->removeFromParent();
}

