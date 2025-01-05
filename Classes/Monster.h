#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "RoleBase.h"
#include "DataClass.h"
#include "MonsterObserver.h"
#include <vector>

USING_NS_CC;

// Refactored with Observer Pattern - Monster class implementation
class CMonster : public CRoleBase, public IMonsterSubject
{
public:
	// Constructor
	CMonster();

	// Destructor
	~CMonster();

	// Static method to set the monster's movement path
	static void setPath(vector<Vec2> vecPath);

	// Initialize monster instance using monster data
	virtual bool initWithData(SMonsterDt* pMonster);

	// Initialize monster properties
	void initMonsterProperties(SMonsterDt* pMonster);
	// Create and set the monster's model
	void createAndSetMonsterModel(SMonsterDt* pMonster);
	// Set the position of the monster's health bar
	void setHpNodePosition();
	// Set the monster's initial position and direction
	void setInitialPositionAndDirection();
	// Static method to create a monster instance based on monster data
	static CMonster* createWithData(SMonsterDt* pMonster);
	// Remove the monster
	void removeMonster();

	// Update method for each frame
	virtual void update(float delta);

	// Change update direction
	bool changeUpdateDir();
	// Helper method: Get path property for the specified position
	bool CMonster::hasPathProperty(const Vec2& pos);
	// Helper method: Update the monster's direction and index
	bool CMonster::updateDirectionAndIndex();

	// Calculate direction
	void calculateDir();

	// Macro to get and set the monster's speed
	CC_SYNTHESIZE(float, m_fSpeed, Speed);

	// Macro to get and set the monster's initial speed
	CC_SYNTHESIZE(float, m_fSaveSpeed, SaveSpeed);

		// Observer Pattern implementation
	virtual void addObserver(IMonsterObserver* observer) override;
	virtual void removeObserver(IMonsterObserver* observer) override;
	virtual void notifyObservers() override;


private:
	// Monster path
	static vector<Vec2> m_vecPath;

	// Current path index
	int m_nIndex;

	// Current direction vector
	Vec2 m_vDir;

	// Flag to indicate if the direction has changed
	bool m_bChangeDir;

	// Tile map size
	Size m_TiledSize;

	// Monster's coin reward
	int m_nMoney;

	
	// Observer Pattern - collection of observers
	std::vector<IMonsterObserver*> m_observers;
};

class MonsterFactory
{
public:
	// Static method to create a monster instance using monster data
	static CMonster* createMonster(SMonsterDt* pMonster);
};

#endif // MONSTER_FACTORY_H
