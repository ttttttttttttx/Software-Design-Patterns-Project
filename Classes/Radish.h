#ifndef __RADISH_H__
#define __RADISH_H__

#include "cocos2d.h"
#include "RadishState.h"
USING_NS_CC;

class CRadish : public Node
{
public:
	CRadish();
	~CRadish();
	bool init();
	void changeState(CRadishState* newState);
	void addTouch();
	void clickEvent(Vec2 clickPos);
	bool Damage(int damage);
	void updateRadishDisplay();
	CREATE_FUNC(CRadish);
	CC_SYNTHESIZE(int, m_nHP, Hp);

private:
	CRadishState* m_state;  
	Sprite* m_pHp;          
	Sprite* m_pModel;       
	Sprite* myup;
};
#endif
