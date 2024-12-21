#ifndef __BUFFLAYER_H__
#define __BUFFLAYER_H__

#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "Buff.h"

using namespace cocos2d;

// Buff层类
class CBuffLayer : public Node
{
public:
	// 构造函数
	CBuffLayer();

	// 析构函数
	~CBuffLayer();

	// 初始化方法，用于设置Buff层的初始状态
	virtual bool init();

	// 初始化回调函数，用于设置Buff的回调函数
	void inintCallBack();

	// 初始化回调函数，用于创建不同类型的Buff实例
	void initCallbackForBuff(int buffId, CBuffBase* buffInstance);

	// 每帧更新方法，用于处理Buff层的逻辑，delta表示两帧之间的时间间隔
	virtual void update(float delta);

	// 添加Buff到指定节点
	void addBuff(Node* pNode, int nBuffID);

	// 从指定节点移除Buff
	void removeBuff(Node* pNode);

	// 创建实例的静态方法，使用宏定义实现
	CREATE_FUNC(CBuffLayer);

private:
	// 存储Buff回调函数的Map，键为Buff的ID，值为对应的Buff实例
	std::map<int, CBuffBase*> m_mapCallBack;
};

// 获取屏幕大小的宏定义
#define WINSIZE Director::getInstance()->getVisibleSize()

// 获取屏幕原点位置的宏定义
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif