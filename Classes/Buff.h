#ifndef __BUFF_H__
#define __BUFF_H__

#include "cocos2d.h"
#include "DataClass.h"

using namespace cocos2d;

// 存储Buff相关数据的结构体
struct SData
{
	float fTime;    // Buff的持续时间
	int nValue;     // Buff的数值
	Sprite* pSprite; // Buff的图像
};

// Buff基类
class CBuffBase : public Node
{
public:
	// 构造函数
	CBuffBase();

	// 析构函数
	~CBuffBase();

	// 初始化方法，虚函数，由子类实现具体的初始化逻辑
	virtual bool init();

	// 添加Buff节点到地图上
	void addBuffNode(Node* pNode, SBuffDt* pBuff);

	// 从地图上移除Buff节点
	void removeBuffNode(Node* pNode);

	// 每帧更新方法，虚函数，由子类实现具体的Buff逻辑
	virtual void update(float delta);

protected:
	// 存储地图上所有的Buff节点及其相关数据的Map
	std::map<Node*, SData*> m_mapBuff;
};

// 速度Buff类
class CBuffSpeed : public CBuffBase
{
public:
	// 初始化方法，用于设置Buff的初始状态
	virtual bool init();

	// 每帧更新方法，用于处理Buff的逻辑，delta表示两帧之间的时间间隔
	virtual void update(float delta);

	// 创建实例的静态方法，使用宏定义实现
	CREATE_FUNC(CBuffSpeed);
};

// 伤害Buff类
class CBuffHurt : public CBuffBase
{
public:
	// 初始化方法，用于设置Buff的初始状态
	virtual bool init();

	// 每帧更新方法，用于处理Buff的逻辑，delta表示两帧之间的时间间隔
	virtual void update(float delta);

	// 创建实例的静态方法，使用宏定义实现
	CREATE_FUNC(CBuffHurt);
};


#endif