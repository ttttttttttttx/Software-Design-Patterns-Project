#ifndef __RADISH_STATE_H__
#define __RADISH_STATE_H__

#include "cocos2d.h"

class CRadish;  // Forward declaration

// Refactored with State Pattern
class CRadishState {
public:
    virtual ~CRadishState() {}
    virtual void damage(CRadish* radish, int damage) = 0;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) = 0;
};

// Healthy state
class HealthyState : public CRadishState {
public:
    virtual void damage(CRadish* radish, int damage) override;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) override;
};

// Damaged state
class DamagedState : public CRadishState {
public:
    virtual void damage(CRadish* radish, int damage) override;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) override;
};

// Danger state
class DangerState : public CRadishState {
public:
    virtual void damage(CRadish* radish, int damage) override;
    virtual void clickEvent(CRadish* radish, cocos2d::Vec2 clickPos) override;
};

#endif 