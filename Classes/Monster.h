#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "RoleBase.h"
#include "DataClass.h"

USING_NS_CC;

// ������
class CMonster : public CRoleBase
{
public:
	// ���캯��
	CMonster();

	// ��������
	~CMonster();

	// ���ù����ƶ�·���ľ�̬����
	static void setPath(vector<Vec2> vecPath);

	// ʹ�ù������ݳ�ʼ������ʵ��
	virtual bool initWithData(SMonsterDt* pMonster);

	// ��ʼ����������
	void initMonsterProperties(SMonsterDt* pMonster);
	// ���������ù����ģ��
	void createAndSetMonsterModel(SMonsterDt* pMonster);
	// ���ù���Ѫ��λ��
	void setHpNodePosition();
	// ���ù����ʼλ�úͷ���
	void setInitialPositionAndDirection();
	// ���ݹ������ݴ�������ʵ���ľ�̬����
	static CMonster* createWithData(SMonsterDt* pMonster);
	// �Ƴ�����
	void removeMonster();

	// ÿ֡���·���
	virtual void update(float delta);

	// �ı���·���
	bool changeUpdateDir();
	// ������������ȡָ��λ�õ�·������
	bool CMonster::hasPathProperty(const Vec2& pos);
	// �������������¹���ķ��������
	bool CMonster::updateDirectionAndIndex();

	// ���㷽��
	void calculateDir();

	// ��ȡ�����ù����ٶȵĺ궨��
	CC_SYNTHESIZE(float, m_fSpeed, Speed);

	// ��ȡ�����ù����ʼ�ٶȵĺ궨��
	CC_SYNTHESIZE(float, m_fSaveSpeed, SaveSpeed);

private:
	// ����·��
	static vector<Vec2> m_vecPath;

	// ��ǰ·������
	int m_nIndex;

	// ��ǰ��������
	Vec2 m_vDir;

	// �Ƿ�ı䷽��ı�־
	bool m_bChangeDir;

	// ���ӵ�ͼ��С
	Size m_TiledSize;

	// ����Ľ�ҽ���
	int m_nMoney;
};

#endif
