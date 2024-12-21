#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "Monster.h"
USING_NS_CC;

//---------------------------------------�ӵ�����----------------------------------------//

class CBulletBase : public Sprite
{
public:
	bool initWithData(SBulletDt* pBullet, int nGrand);  //��ʼ���ӵ�����
	virtual void update(float delta) {};                   //�����ӵ������״̬
	virtual void collisions(Node* pNode) {};               //�����������ڵ����ײ
	CC_SYNTHESIZE(float, mySpeed, Speed);              //�� Speed ����һ�� float ���͵ĳ�Ա���� m_fSpeed
	CC_SYNTHESIZE(Vec2, myDir, Dir);                   //���� Vec2 m_vDir: ��ʾ�ӵ��ķ���
	CC_SYNTHESIZE(float, myAckRange, AckRange);        //���� float m_fAckRange: ��ʾ�ӵ��Ĺ�����Χ
	CC_SYNTHESIZE(int, myBuffID, BuffID);              //���� int m_nBuffID: ��ʾ�ӵ����˺�Ч��ID
	CC_SYNTHESIZE(int, myAck, Ack);                    //���� int m_nAck: ��ʾ�ӵ��Ĺ�����
	CC_SYNTHESIZE(int, myDieID, var);
protected:
	Sprite* nowSprite;
};

//---------------------------------------�����ӵ�----------------------------------------//

class CBulletLayer : public Node
{
public:
	CBulletBase* addBullet(int nID, Vec2 pos, Node* pNode, float fAckRange, int nGrade);  //����ӵ�
	CREATE_FUNC(CBulletLayer);  //���ٴ��� CBulletLayer ��
private:
	CBulletBase* CBulletLayer::createBullet(const string& type, SBulletDt* pBulletDt, int nGrand);
};

//---------------------------------------��ƿ��----------------------------------------//

class CBulletCommon : public CBulletBase
{
public:
	virtual void update(float delta);
	virtual void collisions(Node* pNode);
	static CBulletCommon* createWithData(SBulletDt* pBullet, int nGrade); //��ز���
private:
};

//---------------------------------------�糵----------------------------------------//

class CBulletThrough : public CBulletBase
{
public:
	virtual void update(float delta);
	virtual void collisions(Node* pNode);
	static CBulletThrough* createWithData(SBulletDt* pBullet, int nGrade);
	Node* myAckNode;
private:
};

//---------------------------------------��ƿ��----------------------------------------//

class CBulletRadial : public CBulletBase
{
public:
	bool initWithData(SBulletDt* pBullet, int nGrade);
	static CBulletRadial* createWithData(SBulletDt* pBullet, int nGrade);
	virtual void update(float delta);
};

//---------------------------------------̫���ͱ���----------------------------------------//

class CBulletStatic : public CBulletBase
{
public:
	static CBulletStatic* createWithData(SBulletDt* pBullet, int nGrade);
	virtual void update(float delta);
};

#endif