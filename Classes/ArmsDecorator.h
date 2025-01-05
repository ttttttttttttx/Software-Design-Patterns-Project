#ifndef __ARMSDECORATOR_H__
#define __ARMSDECORATOR_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "Monster.h"
#include "DataMgr.h"
#include "Bullet.h"
#include "Arms.h"
#include <iostream>
USING_NS_CC;

// Refactored with Decorator Pattern
// Base Decorator class
class ArmsDecorator : public ArmsComponent 
{
protected:
    unique_ptr<ArmsComponent> armsComponent;

public:
    explicit ArmsDecorator(ArmsComponent* component) : armsComponent(component) {}
    void fire() override;
    void upgrade() override;
    virtual ~ArmsDecorator() = default;
};

// Decorator for appearance modifications
class AppearanceDecorator : public ArmsDecorator 
{
public:
    explicit AppearanceDecorator(ArmsComponent* component) : ArmsDecorator(component) {}
    void upgrade() override;
};

// Decorator for critical hit rate modifications
class CritDecorator : public ArmsDecorator {
public:
    explicit CritDecorator(ArmsComponent* component, float increase) : ArmsDecorator(component), critIncrease(increase) {}
    void upgrade() override;

private:
    float critIncrease;
};

// Decorator for damage modifications
class DamageDecorator : public ArmsDecorator {
public:
    explicit DamageDecorator(ArmsComponent* component, float increase) : ArmsDecorator(component), damageIncrease(increase) {}
    void upgrade() override;

private:
    float damageIncrease;
};

// Decorator for range and attack speed modifications
class RangeAndSpeedDecorator : public ArmsDecorator {
public:
    explicit RangeAndSpeedDecorator(ArmsComponent* component, float rangeIncrease, float speedIncrease)
        : ArmsDecorator(component), rangeIncrease(rangeIncrease), speedIncrease(speedIncrease) {}
    void upgrade() override;

private:
    float rangeIncrease;
    float speedIncrease;
};

#endif