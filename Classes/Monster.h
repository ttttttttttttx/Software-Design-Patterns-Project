#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "RoleBase.h"
#include "DataClass.h"

USING_NS_CC;

// 怪物类
class CMonster : public CRoleBase
{
public:
	// 构造函数
	CMonster();

	// 析构函数
	~CMonster();

	// 设置怪物移动路径的静态方法
	static void setPath(vector<Vec2> vecPath);

	// 使用怪物数据初始化怪物实例
	virtual bool initWithData(SMonsterDt* pMonster);

	// 初始化怪物属性
	void initMonsterProperties(SMonsterDt* pMonster);
	// 创建并设置怪物的模型
	void createAndSetMonsterModel(SMonsterDt* pMonster);
	// 设置怪物血条位置
	void setHpNodePosition();
	// 设置怪物初始位置和方向
	void setInitialPositionAndDirection();
	// 根据怪物数据创建怪物实例的静态方法
	static CMonster* createWithData(SMonsterDt* pMonster);
	// 移除怪物
	void removeMonster();

	// 每帧更新方法
	virtual void update(float delta);

	// 改变更新方向
	bool changeUpdateDir();
	// 辅助方法：获取指定位置的路径属性
	bool CMonster::hasPathProperty(const Vec2& pos);
	// 辅助方法：更新怪物的方向和索引
	bool CMonster::updateDirectionAndIndex();

	// 计算方向
	void calculateDir();

	// 获取和设置怪物速度的宏定义
	CC_SYNTHESIZE(float, m_fSpeed, Speed);

	// 获取和设置怪物初始速度的宏定义
	CC_SYNTHESIZE(float, m_fSaveSpeed, SaveSpeed);

private:
	// 怪物路径
	static vector<Vec2> m_vecPath;

	// 当前路径索引
	int m_nIndex;

	// 当前方向向量
	Vec2 m_vDir;

	// 是否改变方向的标志
	bool m_bChangeDir;

	// 格子地图大小
	Size m_TiledSize;

	// 怪物的金币奖励
	int m_nMoney;
};

#endif
