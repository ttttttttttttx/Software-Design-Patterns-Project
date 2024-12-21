#ifndef __MONSTERLAYER_H__
#define __MONSTERLAYER_H__

#include "cocos2d.h"
#include "Monster.h"

USING_NS_CC;

// 怪物层类
class CMonsterLayer : public Node
{
public:
	// 构造函数
	CMonsterLayer();

	// 析构函数
	~CMonsterLayer();

	// 初始化方法，用于设置怪物层的初始状态
	virtual bool init();

	// 每帧更新方法，用于处理怪物层的逻辑，delta表示两帧之间的时间间隔
	virtual void update(float delta);

	// 创建怪物的方法
	void createMonster();

	// 开始生成怪物的方法
	void startCreatMonster();

	// 游戏结束的方法
	void GameOver();

	// 根据攻击范围和位置获取一个怪物实例的方法
	CMonster* getMonster(float fAckRandge, Vec2 Pos);

	// 根据攻击范围和位置获取一组怪物实例的方法
	vector<CMonster*> getMonsterVec(float fAckRandge, Vec2 Pos);

	// 创建实例的静态方法，使用宏定义实现
	CREATE_FUNC(CMonsterLayer);

	void applySpecialSkill();

private:
	// 存储怪物ID的向量
	std::vector<int> m_vecMonsterID;

	// 存储波次的向量
	std::vector<int> m_vecWave;

	// 当前怪物数量
	int m_nCurMonsterCount;

	// 当前波次
	int m_nCurWave;

	// 当前波次怪物ID
	int m_nCurWaveMonsterID;
};

// 获取屏幕大小的宏定义
#define WINSIZE Director::getInstance()->getVisibleSize()

// 获取屏幕原点位置的宏定义
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif
