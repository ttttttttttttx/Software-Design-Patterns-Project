#ifndef __BUFF_H__
#define __BUFF_H__

#include "cocos2d.h"
#include "DataClass.h"

using namespace cocos2d;

// �洢Buff������ݵĽṹ��
struct SData
{
	float fTime;    // Buff�ĳ���ʱ��
	int nValue;     // Buff����ֵ
	Sprite* pSprite; // Buff��ͼ��
};

// Buff����
class CBuffBase : public Node
{
public:
	// ���캯��
	CBuffBase();

	// ��������
	~CBuffBase();

	// ��ʼ���������麯����������ʵ�־���ĳ�ʼ���߼�
	virtual bool init();

	// ���Buff�ڵ㵽��ͼ��
	void addBuffNode(Node* pNode, SBuffDt* pBuff);

	// �ӵ�ͼ���Ƴ�Buff�ڵ�
	void removeBuffNode(Node* pNode);

	// ÿ֡���·������麯����������ʵ�־����Buff�߼�
	virtual void update(float delta);

protected:
	// �洢��ͼ�����е�Buff�ڵ㼰��������ݵ�Map
	std::map<Node*, SData*> m_mapBuff;
};

// �ٶ�Buff��
class CBuffSpeed : public CBuffBase
{
public:
	// ��ʼ����������������Buff�ĳ�ʼ״̬
	virtual bool init();

	// ÿ֡���·��������ڴ���Buff���߼���delta��ʾ��֮֡���ʱ����
	virtual void update(float delta);

	// ����ʵ���ľ�̬������ʹ�ú궨��ʵ��
	CREATE_FUNC(CBuffSpeed);
};

// �˺�Buff��
class CBuffHurt : public CBuffBase
{
public:
	// ��ʼ����������������Buff�ĳ�ʼ״̬
	virtual bool init();

	// ÿ֡���·��������ڴ���Buff���߼���delta��ʾ��֮֡���ʱ����
	virtual void update(float delta);

	// ����ʵ���ľ�̬������ʹ�ú궨��ʵ��
	CREATE_FUNC(CBuffHurt);
};


#endif