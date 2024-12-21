#ifndef __RADISH_H__
#define __RADISH_H__

#include "cocos2d.h"
#include "MonsterLayer.h"
USING_NS_CC;

// 萝卜类
class CRadish : public Node
{
public:
	// 构造函数
	CRadish();

	// 析构函数
	~CRadish();

	// 初始化方法，用于设置萝卜的初始状态
	virtual bool init();

	// 萝卜受到伤害的方法，返回值表示是否萝卜存活
	bool Damage(int damage);

	void addTouch();
	// 处理点击事件的方法，clickPos表示点击的位置
	void clickEvent(Vec2 clickPos);

	// 创建实例的静态方法，使用宏定义实现
	CREATE_FUNC(CRadish);

	// 获取和设置萝卜生命值的宏定义
	CC_SYNTHESIZE(int, m_nHP, Hp);

private:
	// 萝卜的生命值显示
	Sprite* m_pHp;

	// 萝卜的模型
	Sprite* m_pModel;

	// 萝卜的模型
	Sprite* myup;
};

#endif