#include "ArmsDecorator.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

// 装饰器抽象类方法
void ArmsDecorator::fire() {
    if (armsComponent) {
        armsComponent->fire();
    }
    else {
        throw std::runtime_error("ArmsComponent is not initialized.");
    }
}
void ArmsDecorator::upgrade() {
    if (armsComponent) {
        armsComponent->upgrade();
    }
    else {
        throw std::runtime_error("ArmsComponent is not initialized.");
    }
}

// 改变武器外观的装饰器
void AppearanceDecorator::upgrade() {
    ArmsDecorator::upgrade();
    if (armsComponent) {
        auto myArms = dynamic_cast<MyArms*>(armsComponent.get());
        if (myArms) {
            // 更换武器外观
            myArms->updateAppearance();
        }
    }
}

// 改变暴击率的装饰器
void CritDecorator::upgrade() {
    ArmsDecorator::upgrade();
    // 增加暴击率
    if (armsComponent) {
        float newCritRate = armsComponent->getCritRate() + critIncrease;
        armsComponent->setCritRate(newCritRate);
        std::cout << "Critical hit rate increased to " << newCritRate * 100 << "%." << std::endl;
    }
}

// 改变伤害的装饰器
void DamageDecorator::upgrade() {
    ArmsDecorator::upgrade();
    // 增加伤害
    if (armsComponent) {
        float newDamage = armsComponent->getDamage() + damageIncrease;
        armsComponent->setDamage(newDamage);
        std::cout << "Weapon damage increased to " << newDamage << "." << std::endl;
    }
}

// 改变射程和射速的装饰器
void RangeAndSpeedDecorator::upgrade() {
    ArmsDecorator::upgrade();
    // 增加射程和射速
    if (armsComponent) {
        float newRange = armsComponent->getRange() + rangeIncrease;
        float newSpeed = armsComponent->getFireRate() + speedIncrease;
        armsComponent->setRange(newRange);
        armsComponent->setFireRate(newSpeed);
        std::cout << "Weapon range increased to " << newRange << " and fire rate increased to " << newSpeed << "." << std::endl;
    }
}