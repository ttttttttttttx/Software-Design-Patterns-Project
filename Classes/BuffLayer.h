#ifndef __BUFFLAYER_H__
#define __BUFFLAYER_H__

#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "Buff.h"

using namespace cocos2d;

// Buff����
class CBuffLayer : public Node
{
public:
	// ���캯��
	CBuffLayer();

	// ��������
	~CBuffLayer();

	// ��ʼ����������������Buff��ĳ�ʼ״̬
	virtual bool init();

	// ��ʼ���ص���������������Buff�Ļص�����
	void inintCallBack();

	// ��ʼ���ص����������ڴ�����ͬ���͵�Buffʵ��
	void initCallbackForBuff(int buffId, CBuffBase* buffInstance);

	// ÿ֡���·��������ڴ���Buff����߼���delta��ʾ��֮֡���ʱ����
	virtual void update(float delta);

	// ���Buff��ָ���ڵ�
	void addBuff(Node* pNode, int nBuffID);

	// ��ָ���ڵ��Ƴ�Buff
	void removeBuff(Node* pNode);

	// ����ʵ���ľ�̬������ʹ�ú궨��ʵ��
	CREATE_FUNC(CBuffLayer);

private:
	// �洢Buff�ص�������Map����ΪBuff��ID��ֵΪ��Ӧ��Buffʵ��
	std::map<int, CBuffBase*> m_mapCallBack;
};

// ��ȡ��Ļ��С�ĺ궨��
#define WINSIZE Director::getInstance()->getVisibleSize()

// ��ȡ��Ļԭ��λ�õĺ궨��
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif