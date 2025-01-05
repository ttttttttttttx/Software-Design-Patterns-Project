#include "Buff.h"
#include "GameScene.h"
#include "Monster.h"

// Constructor
CBuffBase::CBuffBase()
{
}

// Destructor
CBuffBase::~CBuffBase()
{
}

// Base initialization
bool CBuffBase::init()
{
	return Node::init();
}

// Base update function
void CBuffBase::update(float delta)
{
	// Empty implementation, to be overridden by derived classes
}

// Add buff node method
void CBuffBase::addBuffNode(Node* pNode, SBuffDt* pBuff)
{
	// Search for existing buff on the node
	auto iter = m_mapBuff.find(pNode);

	if (iter != m_mapBuff.end())
	{
		// Update existing buff duration
		m_mapBuff[pNode]->fTime = pBuff->fTime;
	}
	else
	{
		// Create new buff data structure
		SData* pData = new SData();
		pData->fTime = pBuff->fTime;
		pData->nValue = pBuff->nValue;

		// Create buff sprite
		pData->pSprite = Sprite::create();

		// Get animation from game scene
		auto animateLayer = CGameScene::getInstance()->getMyAnimate();
		animateLayer->changeAction(pData->pSprite, pBuff->nAnimateID, true, 0.1f);

		// Set buff sprite position
		pData->pSprite->setPositionY(static_cast<CMonster*>(pNode)->getContentSize().height / 2);

		// Add buff sprite to node
		pNode->addChild(pData->pSprite);

		// Store buff data in map
		m_mapBuff[pNode] = pData;
	}
}

// Remove buff node method
void CBuffBase::removeBuffNode(Node* pNode)
{
	auto iter = m_mapBuff.find(pNode);

	if (iter != m_mapBuff.end())
	{
		// Stop all animations on buff sprite
		iter->second->pSprite->stopAllActions();

		// Remove buff sprite from parent node
		iter->second->pSprite->removeFromParent();

		// Remove buff from map
		m_mapBuff.erase(iter);
	}
}

// Speed buff initialization
bool CBuffSpeed::init()
{
	return CBuffBase::init();
}

// Speed buff update function
void CBuffSpeed::update(float delta)
{
	// Check if buff map is empty
	if (m_mapBuff.empty())
		return;

	// Iterate through all buffs
	for (auto iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
	{
		// Update remaining buff time
		iter->second->fTime -= delta;

		// Apply speed effect to monster
		auto monster = static_cast<CMonster*>(iter->first);
		if (monster->getSpeed() >= iter->second->nValue)
		{
			monster->setSpeed(iter->second->nValue);
		}
		else
		{
			monster->setSpeed(8);
		}

		// Check if buff has expired
		if (iter->second->fTime <= 0)
		{
			// Restore original speed
			monster->setSpeed(monster->getSaveSpeed());

			// Remove buff sprite
			auto buffSprite = iter->second->pSprite;
			buffSprite->stopAllActions();
			buffSprite->removeFromParent();

			// Remove buff from map
			iter = m_mapBuff.erase(iter);
		}
		else
		{
			// Move to next buff
			++iter;
		}
	}
}

// Damage buff initialization
bool CBuffHurt::init()
{
	return CBuffBase::init();
}

// Damage buff update function
void CBuffHurt::update(float delta)
{
	// Check if buff map is empty
	if (m_mapBuff.empty())
		return;

	// Iterate through all buffs
	for (auto iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
	{
		// Update remaining buff time
		iter->second->fTime -= delta;

		// Apply damage effect to monster
		CMonster* monster = static_cast<CMonster*>(iter->first);
		if (monster->Damage(iter->second->nValue))
		{
			// Remove monster and exit if killed
			monster->removeMonster();
			return;
		}

		// Check if buff has expired
		if (iter->second->fTime <= 0)
		{
			// Remove buff sprite
			auto buffSprite = iter->second->pSprite;
			buffSprite->stopAllActions();
			buffSprite->removeFromParent();

			// Remove buff from map
			iter = m_mapBuff.erase(iter);
		}
		else
		{
			// Move to next buff
			++iter;
		}
	}
}

