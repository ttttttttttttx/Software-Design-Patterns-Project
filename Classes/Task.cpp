//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#include "Task.h"

// Constructor initializes the task with required objects
CollisionTask::CollisionTask(Node* bulletNode, Node* monsterNode, Scene* scene)
    : m_pBulletNode(bulletNode)
    , m_pMonsterNode(monsterNode)
    , m_pScene(scene)
{
}

// Execute method implements the collision detection logic
void CollisionTask::execute() {
    // Validate pointers to prevent null pointer access
    if (!m_pBulletNode || !m_pMonsterNode || !m_pScene) {
        return;
    }

    // Cast nodes to their specific types
    CMonster* pMonster = static_cast<CMonster*>(m_pMonsterNode);
    CBulletBase* pBullet = static_cast<CBulletBase*>(m_pBulletNode);

    // Calculate collision area for the monster
    Vec2 Pos = Vec2(pMonster->getPosition().x - pMonster->getModel()->getContentSize().width / 2,
                    pMonster->getPosition().y - pMonster->getModel()->getContentSize().height / 2);
    Rect newRect = Rect(m_pScene->convertToNodeSpace(Pos), 
                       pMonster->getModel()->getContentSize());

    // Check for collision between bullet and monster
    if (newRect.intersectsCircle(pBullet->getPosition(), 10)) {
        // Schedule collision handling on the main thread
        Director::getInstance()->getScheduler()->performFunctionInMainThread([=]() {
            pBullet->collisions(m_pMonsterNode);
        });
    }
} 