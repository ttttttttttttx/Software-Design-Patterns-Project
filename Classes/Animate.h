#ifndef __ANIMATE_H__
#define __ANIMATE_H__
#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
USING_NS_CC;

#define WINSIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()

class MyAnimate : public Node
{
public:
	virtual bool init();
	Animation* seriesAction(int nID);
	void changeAction(Sprite* pSprite, int nID, bool bEver, float fDelayTime);
	Animate* getAnimate(int nID);
	void createAnimate(Vec2 pos,int nID);
	CREATE_FUNC(MyAnimate);

private:
	CAnimateDtMgr* myAnimateDtMgr;
};


#endif