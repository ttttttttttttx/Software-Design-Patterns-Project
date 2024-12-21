#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "Monster.h"
USING_NS_CC;

//---------------------------------------子弹基类----------------------------------------//

class CBulletBase : public Sprite
{
public:
	bool initWithData(SBulletDt* pBullet, int nGrand);  //初始化子弹对象
	virtual void update(float delta) {};                   //更新子弹对象的状态
	virtual void collisions(Node* pNode) {};               //处理与其他节点的碰撞
	CC_SYNTHESIZE(float, mySpeed, Speed);              //由 Speed 生成一个 float 类型的成员变量 m_fSpeed
	CC_SYNTHESIZE(Vec2, myDir, Dir);                   //生成 Vec2 m_vDir: 表示子弹的方向
	CC_SYNTHESIZE(float, myAckRange, AckRange);        //生成 float m_fAckRange: 表示子弹的攻击范围
	CC_SYNTHESIZE(int, myBuffID, BuffID);              //生成 int m_nBuffID: 表示子弹的伤害效果ID
	CC_SYNTHESIZE(int, myAck, Ack);                    //生成 int m_nAck: 表示子弹的攻击力
	CC_SYNTHESIZE(int, myDieID, var);
protected:
	Sprite* nowSprite;
};

//---------------------------------------创建子弹----------------------------------------//

class CBulletLayer : public Node
{
public:
	CBulletBase* addBullet(int nID, Vec2 pos, Node* pNode, float fAckRange, int nGrade);  //添加子弹
	CREATE_FUNC(CBulletLayer);  //快速创建 CBulletLayer 类
private:
	CBulletBase* CBulletLayer::createBullet(const string& type, SBulletDt* pBulletDt, int nGrand);
};

//---------------------------------------绿瓶子----------------------------------------//

class CBulletCommon : public CBulletBase
{
public:
	virtual void update(float delta);
	virtual void collisions(Node* pNode);
	static CBulletCommon* createWithData(SBulletDt* pBullet, int nGrade); //相关参数
private:
};

//---------------------------------------风车----------------------------------------//

class CBulletThrough : public CBulletBase
{
public:
	virtual void update(float delta);
	virtual void collisions(Node* pNode);
	static CBulletThrough* createWithData(SBulletDt* pBullet, int nGrade);
	Node* myAckNode;
private:
};

//---------------------------------------火瓶子----------------------------------------//

class CBulletRadial : public CBulletBase
{
public:
	bool initWithData(SBulletDt* pBullet, int nGrade);
	static CBulletRadial* createWithData(SBulletDt* pBullet, int nGrade);
	virtual void update(float delta);
};

//---------------------------------------太阳和冰冻----------------------------------------//

class CBulletStatic : public CBulletBase
{
public:
	static CBulletStatic* createWithData(SBulletDt* pBullet, int nGrade);
	virtual void update(float delta);
};

#endif