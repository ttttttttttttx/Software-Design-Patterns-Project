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
// ��������ӿ�
class ArmsComponent : public Node {
public:
    virtual void fire() = 0;     // �����ӵ�
    virtual void upgrade() = 0;  // ��������

    // ��ȡ/���ñ�����
    virtual float getCritRate() const = 0;
    virtual void setCritRate(float rate) = 0;

    // ��ȡ/�����˺�
    virtual float getDamage() const = 0;
    virtual void setDamage(float damage) = 0;

    // ��ȡ/�������
    virtual float getRange() const = 0;
    virtual void setRange(float range) = 0;

    // ��ȡ/��������
    virtual float getFireRate() const = 0;
    virtual void setFireRate(float rate) = 0;

    virtual ~ArmsComponent() {}
};

// ����������
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

    // ��������ط���
    float getCritRate() const override { return critRate; }
    void setCritRate(float rate) override { critRate = rate; }

    // �˺���ط���
    float getDamage() const override { return damage; }
    void setDamage(float damage) override { this->damage = damage; }

    // �����ط���
    float getRange() const override { return range; }
    void setRange(float range) override { this->range = range; }

    // ������ط���
    float getFireRate() const override { return fireRate; }
    void setFireRate(float rate) override { fireRate = rate; }

private:
    int grade;
    float critRate, damage, range, fireRate;
    Sprite* baseSprite;    
    Sprite* nowSprite;      
    bool Attack;          
};

//��������
class BuildArms : public Node
{
public:
	void createArms(int nID, Vec2 tiledPos);  //ʵ�ʴ�������
	MyArms* isHaveArms(Vec2 tiledPos);   //������λ���Ƿ��������
	CREATE_FUNC(BuildArms);
};

#endif