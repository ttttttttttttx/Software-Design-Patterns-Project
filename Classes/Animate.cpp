#include "Animate.h"

bool MyAnimate::init()
{
	myAnimateDtMgr = static_cast<CAnimateDtMgr*>(CDataMgr::getInstance()->getData("AnimateMgr"));//拿到动画数据管理者
	return true;
}

Animation* MyAnimate::seriesAction(int nID) {

	Animation* pAnimation = Animation::create();

	SAniMateDt* pData = static_cast<SAniMateDt*>(myAnimateDtMgr->getDataByID(nID));

	for (int j = 1; j <= pData->nCount; j++)
	{
		SpriteFrame* pSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(pData->strName.c_str(), j));
		pAnimation->addSpriteFrame(pSpriteFrame);
	}
	return pAnimation;
}

void MyAnimate::changeAction(Sprite* pSprite, int nID, bool bEver, float fDelayTime)
{
	//小于3000，即怪兽和地图，不会进行升级
	if (nID<3000)
	{
		return;
	}
	//停止所有标记为 1 的动作。
	pSprite->stopAllActionsByTag(1);
	Animation* pAnimation = seriesAction(nID);

	pAnimation->setDelayPerUnit(fDelayTime);

	Animate* pAnimate = Animate::create(pAnimation);
	if (bEver)
	{
		RepeatForever* pRe = RepeatForever::create(pAnimate);
		pSprite->runAction(pRe);
	}
	else
	{
		//不需要永久播放，将动画对象的标签设置为 1
		pAnimate->setTag(1);
		pSprite->runAction(pAnimate);
	}
}

Animate* MyAnimate::getAnimate(int nID)
{
	Animation* pAnimation = seriesAction(nID);

	//设置延迟时间
	//动画播放结束后还原到初始帧
	pAnimation->setDelayPerUnit(0.1f);
	pAnimation->setRestoreOriginalFrame(true);

	Animate* pAnimate = Animate::create(pAnimation);
	//设置动画对象的标签为 1。
	pAnimate->setTag(1);

	return pAnimate;
}

void MyAnimate::createAnimate(Vec2 pos, int nID)
{
	Sprite* pSprite = Sprite::create();
	
	Animation* pAnimation = seriesAction(nID);
	pAnimation->setDelayPerUnit(0.1f);
	Animate* pAnimate = Animate::create(pAnimation);

	pSprite->setPosition(pos);

	//组合序列动作：先播放动画，然后移除精灵自身。
	pSprite->runAction(Sequence::createWithTwoActions(pAnimate,RemoveSelf::create()));
	this->addChild(pSprite);
}

