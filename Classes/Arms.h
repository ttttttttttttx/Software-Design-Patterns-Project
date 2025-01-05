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
// 武器组件接口
class ArmsComponent : public Node {
public:
    virtual void fire() = 0;     // 发射子弹
    virtual void upgrade() = 0;  // 升级武器

    // 获取/设置暴击率
    virtual float getCritRate() const = 0;
    virtual void setCritRate(float rate) = 0;

    // 获取/设置伤害
    virtual float getDamage() const = 0;
    virtual void setDamage(float damage) = 0;

    // 获取/设置射程
    virtual float getRange() const = 0;
    virtual void setRange(float range) = 0;

    // 获取/设置射速
    virtual float getFireRate() const = 0;
    virtual void setFireRate(float rate) = 0;

    virtual ~ArmsComponent() {}
};

// 具体武器类
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

    // 暴击率相关方法
    float getCritRate() const override { return critRate; }
    void setCritRate(float rate) override { critRate = rate; }

    // 伤害相关方法
    float getDamage() const override { return damage; }
    void setDamage(float damage) override { this->damage = damage; }

    // 射程相关方法
    float getRange() const override { return range; }
    void setRange(float range) override { this->range = range; }

    // 射速相关方法
    float getFireRate() const override { return fireRate; }
    void setFireRate(float rate) override { fireRate = rate; }

private:
    int grade;
    float critRate, damage, range, fireRate;
    Sprite* baseSprite;    
    Sprite* nowSprite;      
    bool Attack;          
};

//创建武器
class BuildArms : public Node
{
public:
	void createArms(int nID, Vec2 tiledPos);  //实际创建武器
	MyArms* isHaveArms(Vec2 tiledPos);   //检查给定位置是否存在武器
	CREATE_FUNC(BuildArms);
};

#endif