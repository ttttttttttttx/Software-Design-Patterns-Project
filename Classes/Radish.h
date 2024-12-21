#ifndef __RADISH_H__
#define __RADISH_H__

#include "cocos2d.h"
#include "MonsterLayer.h"
USING_NS_CC;

// �ܲ���
class CRadish : public Node
{
public:
	// ���캯��
	CRadish();

	// ��������
	~CRadish();

	// ��ʼ�����������������ܲ��ĳ�ʼ״̬
	virtual bool init();

	// �ܲ��ܵ��˺��ķ���������ֵ��ʾ�Ƿ��ܲ����
	bool Damage(int damage);

	void addTouch();
	// �������¼��ķ�����clickPos��ʾ�����λ��
	void clickEvent(Vec2 clickPos);

	// ����ʵ���ľ�̬������ʹ�ú궨��ʵ��
	CREATE_FUNC(CRadish);

	// ��ȡ�������ܲ�����ֵ�ĺ궨��
	CC_SYNTHESIZE(int, m_nHP, Hp);

private:
	// �ܲ�������ֵ��ʾ
	Sprite* m_pHp;

	// �ܲ���ģ��
	Sprite* m_pModel;

	// �ܲ���ģ��
	Sprite* myup;
};

#endif