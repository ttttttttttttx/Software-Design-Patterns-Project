#include "ArmsDecorator.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

// װ���������෽��
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

// �ı�������۵�װ����
void AppearanceDecorator::upgrade() {
    ArmsDecorator::upgrade();
    if (armsComponent) {
        auto myArms = dynamic_cast<MyArms*>(armsComponent.get());
        if (myArms) {
            // �����������
            myArms->updateAppearance();
        }
    }
}

// �ı䱩���ʵ�װ����
void CritDecorator::upgrade() {
    ArmsDecorator::upgrade();
    // ���ӱ�����
    if (armsComponent) {
        float newCritRate = armsComponent->getCritRate() + critIncrease;
        armsComponent->setCritRate(newCritRate);
        std::cout << "Critical hit rate increased to " << newCritRate * 100 << "%." << std::endl;
    }
}

// �ı��˺���װ����
void DamageDecorator::upgrade() {
    ArmsDecorator::upgrade();
    // �����˺�
    if (armsComponent) {
        float newDamage = armsComponent->getDamage() + damageIncrease;
        armsComponent->setDamage(newDamage);
        std::cout << "Weapon damage increased to " << newDamage << "." << std::endl;
    }
}

// �ı���̺����ٵ�װ����
void RangeAndSpeedDecorator::upgrade() {
    ArmsDecorator::upgrade();
    // ������̺�����
    if (armsComponent) {
        float newRange = armsComponent->getRange() + rangeIncrease;
        float newSpeed = armsComponent->getFireRate() + speedIncrease;
        armsComponent->setRange(newRange);
        armsComponent->setFireRate(newSpeed);
        std::cout << "Weapon range increased to " << newRange << " and fire rate increased to " << newSpeed << "." << std::endl;
    }
}