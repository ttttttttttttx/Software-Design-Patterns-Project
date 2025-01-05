#ifndef __ARMS_H__
#define __ARMS_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "Monster.h"
#include "DataMgr.h"
#include "Bullet.h"
#include <iostream>
USING_NS_CC;

// Refactored with Decorator Pattern
// Arms Component Interface
class ArmsComponent : public Node {
public:
    virtual void fire() = 0;     // Fire weapon
    virtual void upgrade() = 0;  // Upgrade weapon

    // Get/Set critical hit rate
    virtual float getCritRate() const = 0;
    virtual void setCritRate(float rate) = 0;

    // Get/Set damage
    virtual float getDamage() const = 0;
    virtual void setDamage(float damage) = 0;

    // Get/Set range
    virtual float getRange() const = 0;
    virtual void setRange(float range) = 0;

    // Get/Set fire rate
    virtual float getFireRate() const = 0;
    virtual void setFireRate(float rate) = 0;

    virtual ~ArmsComponent() {}
};

// Base Arms Class
class MyArms : public ArmsComponent {
public:
    CREATE_FUNC(MyArms);
    static MyArms* createWithData(SArmsDt* pArms);
    void fire(Node* pNode) override;                 
    virtual void update(float delta);       
    void upgrade() override;                         
    CC_SYNTHESIZE(int, myGrade, Grade);                   
    CC_SYNTHESIZE(SArmsDt*, myArmsDt, ArmsDt);            
    CC_SYNTHESIZE(CBulletBase*, lastBullet, LastBullet);

    // Critical hit methods
    float getCritRate() const override { return critRate; }
    void setCritRate(float rate) override { critRate = rate; }

    // Damage methods
    float getDamage() const override { return damage; }
    void setDamage(float damage) override { this->damage = damage; }

    // Range methods
    float getRange() const override { return range; }
    void setRange(float range) override { this->range = range; }

    // Fire rate methods
    float getFireRate() const override { return fireRate; }
    void setFireRate(float rate) override { fireRate = rate; }

private:
    int grade;
    float critRate, damage, range, fireRate;
    Sprite* baseSprite;    
    Sprite* nowSprite;      
    bool Attack;          
};

// Arms Builder
class BuildArms : public Node
{
public:
    void createArms(int nID, Vec2 tiledPos);  // Create arms instance
    MyArms* isHaveArms(Vec2 tiledPos);   // Check if arms exists at position
    CREATE_FUNC(BuildArms);
};

#endif