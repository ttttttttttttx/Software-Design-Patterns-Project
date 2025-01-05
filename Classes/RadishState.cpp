#include "RadishState.h"
#include "Radish.h"

// HealthyState实现
void HealthyState::damage(CRadish* radish, int damage) {
    radish->setHp(radish->getHp() - damage);
    if (radish->getHp() <= 5) {
        radish->changeState(new DamagedState());
    }
    radish->updateRadishDisplay();
}

void HealthyState::clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) {
    // 健康状态下的点击响应
}

void HealthyState::enter(CRadish* radish) {
    // 进入健康状态时的处理
}

// DamagedState实现
void DamagedState::damage(CRadish* radish, int damage) {
    radish->setHp(radish->getHp() - damage);
    if (radish->getHp() <= 3) {
        radish->changeState(new DangerState());
    }
    radish->updateRadishDisplay();
}

void DamagedState::clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) {
    // 受伤状态下的点击响应
}

void DamagedState::enter(CRadish* radish) {
    // 进入受伤状态时的处理
}

// DangerState实现
void DangerState::damage(CRadish* radish, int damage) {
    radish->setHp(radish->getHp() - damage);
    if (radish->getHp() <= 0) {
        // 处理萝卜死亡逻辑
    }
    radish->updateRadishDisplay();
}

void DangerState::clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) {
    // 危险状态下的点击响应
}

void DangerState::enter(CRadish* radish) {
    // 进入危险状态时的处理
} 