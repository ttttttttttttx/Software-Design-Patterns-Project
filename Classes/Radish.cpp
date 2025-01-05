#include "Radish.h"
#include "GameScene.h"

// 构造函数
CRadish::CRadish()
{
	m_state = new HealthyState();
}

CRadish::~CRadish()
{
	if (m_state) {
		delete m_state;
		m_state = nullptr;
	}
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

void CRadish::addTouch(){
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = [](Touch* pTouch, Event*) {
		//处理萝卜的点击事件
		CGameScene::getInstance()->getRadish()->clickEvent(pTouch->getLocation());

		return true;
	};
	//将事件监听器添加到当前节点的事件分发器中。
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

void CRadish::updateRadishDisplay() {
	// 停止模型进行的动画
	m_pModel->stopAllActions();

	// 更新生命值显示的精灵
	std::string strName = StringUtils::format("BossHP%02d.png", m_nHP);
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pHp->setSpriteFrame(pFrame);

	// 更新萝卜模型的精灵
	strName = StringUtils::format("hlb%d.png", m_nHP == 7 || m_nHP == 5 ? m_nHP + 1 : m_nHP);
	pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pModel->setSpriteFrame(pFrame);
}

void CRadish::changeState(CRadishState* newState) {
	if (m_state) {
		delete m_state;
	}
	m_state = newState;
}

