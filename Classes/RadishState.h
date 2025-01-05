#ifndef __RADISH_STATE_H__
#define __RADISH_STATE_H__

#include "cocos2d.h"

class CRadish;  // 前向声明

class CRadishState {
public:
    virtual ~CRadishState() {}
    virtual void damage(CRadish* radish, int damage) = 0;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) = 0;
};

// 健康状态
class HealthyState : public CRadishState {
public:
    virtual void damage(CRadish* radish, int damage) override;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) override;
};

// 受伤状态
class DamagedState : public CRadishState {
public:
    virtual void damage(CRadish* radish, int damage) override;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) override;
};

// 危险状态
class DangerState : public CRadishState {
public:
    virtual void damage(CRadish* radish, int damage) override;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) override;
};

#endif 