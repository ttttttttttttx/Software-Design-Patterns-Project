#include "Radish.h"
#include "GameScene.h"

// Constructor
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
		// Handle radish click event
		CGameScene::getInstance()->getRadish()->clickEvent(pTouch->getLocation());

		return true;
	};
	// Add event listener to the current node's event dispatcher
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

void CRadish::updateRadishDisplay() {
	// Stop all actions of the model
	m_pModel->stopAllActions();

	// Update the sprite for health display
	std::string strName = StringUtils::format("BossHP%02d.png", m_nHP);
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pHp->setSpriteFrame(pFrame);

	// Update the sprite for radish model
	strName = StringUtils::format("hlb%d.png", m_nHP == 7 || m_nHP == 5 ? m_nHP + 1 : m_nHP);
	pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pModel->setSpriteFrame(pFrame);
}

// Refactored with State Pattern - Change state method
void CRadish::changeState(CRadishState* newState) {
	if (m_state) {
		delete m_state;
	}
	m_state = newState;
}

