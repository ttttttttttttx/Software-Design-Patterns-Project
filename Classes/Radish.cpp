#include "Radish.h"
#include "GameScene.h"

// �ܲ���Ĺ��캯��
CRadish::CRadish()
{
}

// �ܲ������������
CRadish::~CRadish()
{
}

class CRadishState
{
public:
	virtual ~CRadishState() {}
	virtual void damage(CRadish* radish, int damage) = 0;
	virtual void clickEvent(CRadish* radish, Vec2 clickPos) = 0;
	virtual void update(CRadish* radish) = 0;
};

class HealthyState : public CRadishState
{
public:
	void damage(CRadish* radish, int damage) override {
		radish->setHp(radish->getHp() - damage);
		if (radish->getHp() <= 0) {
			radish->changeState(new DeadState());
		}
		else {
			radish->changeState(new InjuredState());
		}
	}

	void clickEvent(CRadish* radish, Vec2 clickPos) override {
		// ����״̬�µĵ���¼�����
	}

	void update(CRadish* radish) override {
		// ����״̬�µĸ����߼�
	}
};

class InjuredState : public CRadishState
{
public:
	void damage(CRadish* radish, int damage) override {
		radish->setHp(radish->getHp() - damage);
		if (radish->getHp() <= 0) {
			radish->changeState(new DeadState());
		}
	}

	void clickEvent(CRadish* radish, Vec2 clickPos) override {
		// ����״̬�µĵ���¼�����
	}

	void update(CRadish* radish) override {
		// ����״̬�µĸ����߼�
	}
};

class DeadState : public CRadishState
{
public:
	void damage(CRadish* radish, int damage) override {
		// ����״̬�£��������ܵ��˺�
	}

	void clickEvent(CRadish* radish, Vec2 clickPos) override {
		// ����״̬�£�����¼���Ч
	}

	void update(CRadish* radish) override {
		// ����״̬�µĸ����߼�
	}
};

class CRadish : public Node
{
public:
	CRadish();
	~CRadish();
	bool init();
	void changeState(CRadishState* newState);
	void addTouch();
	void clickEvent(Vec2 clickPos);
	bool Damage(int damage);
	CREATE_FUNC(CRadish);
	CC_SYNTHESIZE(int, m_nHP, Hp);

private:
	CRadishState* m_state;  // ��ǰ״̬
	Sprite* m_pHp;
	Sprite* m_pModel;
	Sprite* myup;
};

CRadish::CRadish()
{
	m_state = new HealthyState();  // ��ʼ��Ϊ����״̬
}

void CRadish::changeState(CRadishState* newState)
{
	if (m_state != nullptr) {
		delete m_state;
	}
	m_state = newState;
}

bool CRadish::Damage(int damage)
{
	m_state->damage(this, damage);
	return false;
}

void CRadish::clickEvent(Vec2 clickPos)
{
	m_state->clickEvent(this, clickPos);
}


// �ܲ��ܵ��˺��ķ���������ֵ��ʾ�Ƿ��ܲ����
bool CRadish::Damage(int damage)
{
	m_nHP -= damage;

	// �������ֵС�ڵ���0����Ϸ����
	if (m_nHP <= 0)
	{
		CGameScene::getInstance()->getMonsterLayer()->GameOver();
		return true;
	}

	// ֹͣģ�;�������ж���
	m_pModel->stopAllActions();

	// ��������ֵ��ʾ�ľ���
	std::string strName = StringUtils::format("BossHP%02d.png", m_nHP);
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pHp->setSpriteFrame(pFrame);

	// �����ܲ�ģ�͵ľ���
	strName = StringUtils::format("hlb%d.png", m_nHP == 7 || m_nHP == 5 ? m_nHP + 1 : m_nHP);
	pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pModel->setSpriteFrame(pFrame);

	return false;
}

void CRadish::addTouch(){
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = [](Touch* pTouch, Event*) {
		//�����ܲ�����ĵ���¼�
		CGameScene::getInstance()->getRadish()->clickEvent(pTouch->getLocation());

		return true;
	};
	//���¼���������ӵ���ǰ�����ڵ㡣
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

// �������¼��ķ�����clickPos��ʾ�����λ��
void CRadish::clickEvent(Vec2 clickPos)
{
	// �������ֵ���ڵ���10�����ҵ��λ����ģ�;���ı߽����
	if (m_nHP >= 10 && m_pModel->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		// ��ȡ�ܲ������Ķ���
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(3022);
		pAnimate->getAnimation()->setRestoreOriginalFrame(true);

		// ֹͣ���ж���������ģ�;���Ϊ�������״̬����ִ�ж���
		m_pModel->stopAllActions();
		m_pModel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hlb10.png"));
		m_pModel->runAction(pAnimate);
	}
	if (m_nHP < 10 && myup->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		// �������⼼���߼�
		CGameScene::getInstance()->getMonsterLayer()->applySpecialSkill();
		
	}
}

