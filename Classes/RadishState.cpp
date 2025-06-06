#include "RadishState.h"
#include "Radish.h"

// Refactored with State Pattern - HealthyState implementation
void HealthyState::damage(CRadish* radish, int damage) {
    radish->setHp(radish->getHp() - damage);
    if (radish->getHp() <= 5) {
        radish->changeState(new DamagedState());
    }
    radish->updateRadishDisplay();
}

void HealthyState::clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) {
    // Click response in healthy state
}

// Refactored with State Pattern - DamagedState implementation
void DamagedState::damage(CRadish* radish, int damage) {
    radish->setHp(radish->getHp() - damage);
    if (radish->getHp() <= 3) {
        radish->changeState(new DeadState());
    }
    radish->updateRadishDisplay();
}

void DamagedState::clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) {
    // Click response in damaged state
}

// Refactored with State Pattern - DeadState implementation
void DeadState::damage(CRadish* radish, int damage) {
    radish->setHp(radish->getHp() - damage);
    if (radish->getHp() <= 0) {
        // Handle radish death logic
    }
    radish->updateRadishDisplay();
}

void DeadState::clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) {
    // Click response in dead state
} 