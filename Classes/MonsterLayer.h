#ifndef __MONSTER_LAYER_H__
#define __MONSTER_LAYER_H__

#include "cocos2d.h"
#include "Monster.h"
#include "MonsterObserver.h"
#include <vector>

USING_NS_CC;

// Monster Layer class manages monsters in the game
class CMonsterLayer : public Node
{
public:
	CMonsterLayer();

	~CMonsterLayer();

	virtual bool init();

	virtual void update(float delta);

	void createMonster();

	void startCreatMonster();

	void GameOver();

	CMonster* getMonster(float fAckRandge, Vec2 Pos);

	vector<CMonster*> getMonsterVec(float fAckRandge, Vec2 Pos);

	CREATE_FUNC(CMonsterLayer);

	void applySpecialSkill();

	// Added for Observer Pattern
	void setObservers(const std::vector<IMonsterObserver*>& observers) {
		m_observers = observers;
	}

	const std::vector<IMonsterObserver*>& getObservers() const {
		return m_observers;
	}

private:
	std::vector<int> m_vecMonsterID;

	std::vector<int> m_vecWave;

	int m_nCurMonsterCount;

	int m_nCurWave;

	int m_nCurWaveMonsterID;

	// Added for Observer Pattern
	std::vector<IMonsterObserver*> m_observers;
};

#define WINSIZE Director::getInstance()->getVisibleSize()

#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif
