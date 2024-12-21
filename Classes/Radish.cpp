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

// �ܲ���ĳ�ʼ������
bool CRadish::init()
{
	// �жϻ���Node�ĳ�ʼ���Ƿ�ɹ�
	if (!Node::init())
	{
		return false;
	}
	this->addTouch();
	// ��ʼ����ֵ����Ϊ10
	m_nHP = 10;

	// ��������ֵ��ʾ�ľ��飬ʹ�ø�ʽ���ַ�����ȡ��Ӧ����ֵ��ͼƬ֡����
	m_pHp = Sprite::createWithSpriteFrameName(StringUtils::format("BossHP%02d.png", m_nHP));

	// �����ܲ���ģ�;��飬ʹ�ø�ʽ���ַ�����ȡ��Ӧ����ֵ��ͼƬ֡����
	m_pModel = Sprite::createWithSpriteFrameName(StringUtils::format("hlb%d.png", m_nHP));
	myup= Sprite::createWithSpriteFrameName(StringUtils::format("arrow.png"));
	myup->setPosition(25,-50);

	// ��������ֵ��ʾ��λ��
	m_pHp->setPosition(80, 0);

	// ������ֵ��ʾ���ܲ�ģ����ӵ��ܲ��ڵ���
	this->addChild(m_pHp);
	this->addChild(m_pModel);
	this->addChild(myup);

	// ��ʱ����ÿ3��ִ��һ�λص�����
	this->schedule([=](float) {
		// �������ֵС��10��ֹͣ��ʱ��
		if (m_nHP < 10)
		{
			this->unschedule("Move");
			return;
		}

		// ��ȡ�ܲ������Ķ���
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(3023);
		pAnimate->getAnimation()->setRestoreOriginalFrame(true);

		// ֹͣ���ж���������ģ�;���Ϊ�������״̬����ִ�ж���
		m_pModel->stopAllActions();
		m_pModel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hlb10.png"));
		m_pModel->runAction(pAnimate);
		}, 3.0f, "Move");

	// ��ʼ���ɹ�
	return true;
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

