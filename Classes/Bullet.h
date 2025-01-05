#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "Monster.h"
#include "BulletFlyweight.h"
USING_NS_CC;

// Base bullet class - Uses Flyweight Pattern for shared states
class CBulletBase : public Sprite
{
public:
	bool initWithData(SBulletDt* pBullet, int nGrand);  // Initialize with flyweight data
	virtual void update(float delta) {};                 // Update bullet state
	virtual void collisions(Node* pNode) {};             // Handle collisions
	
	// Getters/Setters for extrinsic state
	CC_SYNTHESIZE(float, mySpeed, Speed);               // Movement speed
	CC_SYNTHESIZE(Vec2, myDir, Dir);                    // Direction vector
	CC_SYNTHESIZE(float, myAckRange, AckRange);         // Attack range
	CC_SYNTHESIZE(int, myBuffID, BuffID);               // Buff effect ID
	CC_SYNTHESIZE(int, myAck, Ack);                     // Attack power
	CC_SYNTHESIZE(int, myDieID, var);                   // Death effect ID
	
	// Add reference to flyweight
	CC_SYNTHESIZE(BulletFlyweight*, myFlyweight, Flyweight); // Shared state reference

protected:
	Sprite* nowSprite;
};

// Bullet manager layer
class CBulletLayer : public Node
{
public:
	CBulletBase* addBullet(int nID, Vec2 pos, Node* pNode, float fAckRange, int nGrade);  // Add bullet
	CREATE_FUNC(CBulletLayer);  // Create CBulletLayer
private:
	CBulletBase* CBulletLayer::createBullet(const string& type, SBulletDt* pBulletDt, int nGrand);
};

// Different bullet type implementations
class CBulletCommon : public CBulletBase
{
public:
	virtual void update(float delta);
	virtual void collisions(Node* pNode);
	static CBulletCommon* createWithData(SBulletDt* pBullet, int nGrade); //ز
private:
};

// Green bottle

class CBulletThrough : public CBulletBase
{
public:
	virtual void update(float delta);
	virtual void collisions(Node* pNode);
	static CBulletThrough* createWithData(SBulletDt* pBullet, int nGrade);
	Node* myAckNode;
private:
};

// Windmill

class CBulletRadial : public CBulletBase
{
public:
	bool initWithData(SBulletDt* pBullet, int nGrade);
	static CBulletRadial* createWithData(SBulletDt* pBullet, int nGrade);
	virtual void update(float delta);
};

// Flamejar

class CBulletStatic : public CBulletBase
{
public:
	static CBulletStatic* createWithData(SBulletDt* pBullet, int nGrade);
	virtual void update(float delta);
};

#endif