//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#ifndef TASK_H
#define TASK_H

#include "cocos2d.h"
#include "Monster.h"
#include "Bullet.h"

USING_NS_CC;

// Base Task class - Abstract interface for all tasks
class Task {
public:
    virtual ~Task() = default;
    // Pure virtual function that must be implemented by derived classes
    virtual void execute() = 0;
};

// Collision Detection Task - Concrete implementation for collision checking
class CollisionTask : public Task {
public:
    // Constructor takes the necessary objects for collision detection
    CollisionTask(Node* bulletNode, Node* monsterNode, Scene* scene);
    // Implementation of the execute method for collision detection
    virtual void execute() override;

private:
    // Pointers to the objects involved in collision detection
    Node* m_pBulletNode;    // Bullet object to check
    Node* m_pMonsterNode;   // Monster object to check
    Scene* m_pScene;        // Scene context for coordinate conversion
};

#endif // TASK_H 