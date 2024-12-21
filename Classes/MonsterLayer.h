#ifndef __MONSTERLAYER_H__
#define __MONSTERLAYER_H__

#include "cocos2d.h"
#include "Monster.h"

USING_NS_CC;

// �������
class CMonsterLayer : public Node
{
public:
	// ���캯��
	CMonsterLayer();

	// ��������
	~CMonsterLayer();

	// ��ʼ���������������ù����ĳ�ʼ״̬
	virtual bool init();

	// ÿ֡���·��������ڴ���������߼���delta��ʾ��֮֡���ʱ����
	virtual void update(float delta);

	// ��������ķ���
	void createMonster();

	// ��ʼ���ɹ���ķ���
	void startCreatMonster();

	// ��Ϸ�����ķ���
	void GameOver();

	// ���ݹ�����Χ��λ�û�ȡһ������ʵ���ķ���
	CMonster* getMonster(float fAckRandge, Vec2 Pos);

	// ���ݹ�����Χ��λ�û�ȡһ�����ʵ���ķ���
	vector<CMonster*> getMonsterVec(float fAckRandge, Vec2 Pos);

	// ����ʵ���ľ�̬������ʹ�ú궨��ʵ��
	CREATE_FUNC(CMonsterLayer);

	void applySpecialSkill();

private:
	// �洢����ID������
	std::vector<int> m_vecMonsterID;

	// �洢���ε�����
	std::vector<int> m_vecWave;

	// ��ǰ��������
	int m_nCurMonsterCount;

	// ��ǰ����
	int m_nCurWave;

	// ��ǰ���ι���ID
	int m_nCurWaveMonsterID;
};

// ��ȡ��Ļ��С�ĺ궨��
#define WINSIZE Director::getInstance()->getVisibleSize()

// ��ȡ��Ļԭ��λ�õĺ궨��
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif
