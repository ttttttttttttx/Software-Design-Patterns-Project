// Refactored with Observer Pattern - Interface definitions
#ifndef __MONSTER_OBSERVER_H__
#define __MONSTER_OBSERVER_H__

#include "cocos2d.h"
#include "Monster.h"

USING_NS_CC;

// Observer interface for monster death notifications
class IMonsterObserver {
public:
    virtual void onMonsterDeath(CMonster* monster) = 0;
    virtual ~IMonsterObserver() {}
};

// Subject interface for monster observable behavior
class IMonsterSubject {
public:
    virtual void addObserver(IMonsterObserver* observer) = 0;
    virtual void removeObserver(IMonsterObserver* observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~IMonsterSubject() {}
};

#endif 