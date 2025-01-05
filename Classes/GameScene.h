// Refactored with Observer Pattern - Game scene header
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameMap.h"
#include "MonsterLayer.h"
#include "Card.h"
#include "Arms.h"
#include "Bullet.h"
#include "BuffLayer.h"
#include "Radish.h"
#include "UILayer.h"
#include "Animate.h"
// Added for Observer Pattern
#include "MonsterObserver.h"
#include "MonsterObservers.h"

USING_NS_CC;

// Game scene class manages all game layers and components
class CGameScene : public Scene
{
public:
	~CGameScene();
	virtual bool init();
	CREATE_FUNC(CGameScene);
	static CGameScene* getInstance();
	static void deletInstance();
	void update(float delta);

	// Getters and setters for game components
	CC_SYNTHESIZE(CGameMap*, m_pGameMap, GameMap);
	CC_SYNTHESIZE(CMonsterLayer*, m_pMonsterLayer, MonsterLayer);
	CC_SYNTHESIZE(MyAnimate*, myAnimate, MyAnimate);
	CC_SYNTHESIZE(ArmsCard*, myCard, ArmsCard);
	CC_SYNTHESIZE(BuildArms*, myArms, BuildArms);
	CC_SYNTHESIZE(CBulletLayer*, m_pBulletLayer, BulletLayer);
	CC_SYNTHESIZE(CBuffLayer*, m_pBuffLayer, BuffLayer);
	CC_SYNTHESIZE(UpCard*, myUpCard, UpCard);
	CC_SYNTHESIZE(CRadish*, m_pRadish, Radish);
	CC_SYNTHESIZE(int, m_nMoney, Money);
	CC_SYNTHESIZE(CUILayer*, m_pUILayer, UILayer);

private:
	CGameScene();
	static CGameScene* m_spInstance;
};

// Window size macro
#define WINSIZE Director::getInstance()->getVisibleSize()

// Window origin macro
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif