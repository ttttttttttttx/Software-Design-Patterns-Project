#ifndef __ARMS_H__
#define __ARMS_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "Monster.h"
#include "DataMgr.h"
#include "Bullet.h"
USING_NS_CC;

//-----------------------------------------------��������------------------------------------------------//

class MyArms : public Sprite
{
public:
	static MyArms* createWithData(SArmsDt* pArms);
	void fire(Node* pNode);                 // ���ڷ����ӵ�
	virtual void update(float delta);       // �麯�������ڸ�������״̬
	void upgrade();                         // ������������
	CC_SYNTHESIZE(int, myGrade,Grade);                    //�����ȼ�
	CC_SYNTHESIZE(SArmsDt*, myArmsDt, ArmsDt);            //��������
	CC_SYNTHESIZE(CBulletBase*, lastBullet,LastBullet);  //��һ�η�����ӵ�
private:
	Sprite* baseSprite;    //��������
	Sprite* nowSprite;        //��ǰ����
	bool Attack;           //����״̬
};

//-----------------------------------------------��������------------------------------------------------//

class BuildArms : public Node
{
public:
	void createArms(int nID, Vec2 tiledPos);  //ʵ�ʴ�������
	MyArms* isHaveArms(Vec2 tiledPos);   //������λ���Ƿ��������
	CREATE_FUNC(BuildArms);
};

#endif