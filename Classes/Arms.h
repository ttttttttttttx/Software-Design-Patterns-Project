#ifndef __ARMS_H__
#define __ARMS_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "Monster.h"
#include "DataMgr.h"
#include "Bullet.h"
USING_NS_CC;

//-----------------------------------------------武器功能------------------------------------------------//

class MyArms : public Sprite
{
public:
	static MyArms* createWithData(SArmsDt* pArms);
	void fire(Node* pNode);                 // 用于发射子弹
	virtual void update(float delta);       // 虚函数，用于更新武器状态
	void upgrade();                         // 用于升级武器
	CC_SYNTHESIZE(int, myGrade,Grade);                    //武器等级
	CC_SYNTHESIZE(SArmsDt*, myArmsDt, ArmsDt);            //武器数据
	CC_SYNTHESIZE(CBulletBase*, lastBullet,LastBullet);  //上一次发射的子弹
private:
	Sprite* baseSprite;    //底座精灵
	Sprite* nowSprite;        //当前精灵
	bool Attack;           //攻击状态
};

//-----------------------------------------------创建武器------------------------------------------------//

class BuildArms : public Node
{
public:
	void createArms(int nID, Vec2 tiledPos);  //实际创建武器
	MyArms* isHaveArms(Vec2 tiledPos);   //检查给定位置是否存在武器
	CREATE_FUNC(BuildArms);
};

#endif