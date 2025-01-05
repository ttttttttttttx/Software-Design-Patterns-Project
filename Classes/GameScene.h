//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
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
#include "ThreadPool.h"  // Include thread pool header file

USING_NS_CC;
class CGameScene : public Scene
{
public:
	~CGameScene();
	virtual bool init();
	CREATE_FUNC(CGameScene);
	static CGameScene* getInstance();
	static void deletInstance();
	void update(float delta);
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
	// Integrate thread pool in CGameScene
	ThreadPool* m_pThreadPool;  // Pointer to thread pool used to manage concurrent tasks
};

#define WINSIZE Director::getInstance()->getVisibleSize()

#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif