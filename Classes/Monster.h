#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "RoleBase.h"
#include "DataClass.h"
#include "MonsterObserver.h"
#include <vector>

USING_NS_CC;

// Refactored with Observer Pattern
class CMonster : public CRoleBase, public IMonsterSubject
{
public:
	CMonster();

	~CMonster();

	static void setPath(vector<Vec2> vecPath);

	virtual bool initWithData(SMonsterDt* pMonster);

	void initMonsterProperties(SMonsterDt* pMonster);
	
	void createAndSetMonsterModel(SMonsterDt* pMonster);
	
	void setHpNodePosition();
	
	void setInitialPositionAndDirection();
	
	static CMonster* createWithData(SMonsterDt* pMonster);
	
	// Remove monster
	void removeMonster();

	virtual void update(float delta);

	bool changeUpdateDir();
	
	bool hasPathProperty(const Vec2& pos);
	
	bool updateDirectionAndIndex();

	void calculateDir();

	CC_SYNTHESIZE(float, m_fSpeed, Speed);

	CC_SYNTHESIZE(float, m_fSaveSpeed, SaveSpeed);

	// Observer Pattern implementation
	virtual void addObserver(IMonsterObserver* observer) override;
	virtual void removeObserver(IMonsterObserver* observer) override;
	virtual void notifyObservers() override;

private:
	static vector<Vec2> m_vecPath;

	int m_nIndex;

	Vec2 m_vDir;

	bool m_bChangeDir;

	Size m_TiledSize;

	int m_nMoney;

	// Added for Observer Pattern - observer collection
	std::vector<IMonsterObserver*> m_observers;
};

#endif
