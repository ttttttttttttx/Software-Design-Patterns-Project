// Refactored with Observer Pattern - Concrete observers
#ifndef __MONSTER_OBSERVERS_H__
#define __MONSTER_OBSERVERS_H__

#include "MonsterObserver.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

// Concrete observers for different death effects
class UIObserver : public IMonsterObserver {
public:
    virtual void onMonsterDeath(CMonster* monster) override {
        // Handle money updates
        int money = monster->getMoney();
        CGameScene::getInstance()->getUILayer()->addMoney(money);
        
        // Create money animation
        Sprite* pSprite = Sprite::createWithSpriteFrameName(StringUtils::format("money%d.png", money));
        pSprite->setPosition(monster->getPosition());
        MoveBy* pMoveBy = MoveBy::create(0.5, Vec2(0, 50));
        pSprite->runAction(Sequence::createWithTwoActions(pMoveBy, RemoveSelf::create()));
        CGameScene::getInstance()->getUILayer()->addChild(pSprite);
    }
};

// Buff observer - handles buff removal on monster death
class BuffObserver : public IMonsterObserver {
public:
    virtual void onMonsterDeath(CMonster* monster) override {
        CGameScene::getInstance()->getBuffLayer()->removeBuff(monster);
    }
};

// Animation observer - handles death animation
class AnimateObserver : public IMonsterObserver {
public:
    virtual void onMonsterDeath(CMonster* monster) override {
        CGameScene::getInstance()->getMyAnimate()->createAnimate(monster->getPosition(), 3024);
    }
};

// Audio observer - handles death sound effects
class AudioObserver : public IMonsterObserver {
public:
    virtual void onMonsterDeath(CMonster* monster) override {
        experimental::AudioEngine::play2d("sound/monsterDie.mp3", false, 1.1f);
    }
};

#endif 