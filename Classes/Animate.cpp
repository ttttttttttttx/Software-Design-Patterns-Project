#include "Animate.h"

bool MyAnimate::init()
{
	myAnimateDtMgr = static_cast<CAnimateDtMgr*>(CDataMgr::getInstance()->getData("AnimateMgr")); // Get animation data manager
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
	// Skip if ID < 3000 (reserved for map and UI animations)
	if (nID<3000)
	{
		return;
	}
	// Stop all actions with tag 1
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
		// Set tag 1 for non-repeating animations
		pAnimate->setTag(1);
		pSprite->runAction(pAnimate);
	}
}

Animate* MyAnimate::getAnimate(int nID)
{
	Animation* pAnimation = seriesAction(nID);

	// Set delay time and restore to first frame when done
	pAnimation->setDelayPerUnit(0.1f);
	pAnimation->setRestoreOriginalFrame(true);

	Animate* pAnimate = Animate::create(pAnimation);
	// Set animation tag to 1
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

	// Play animation once then remove sprite
	pSprite->runAction(Sequence::createWithTwoActions(pAnimate,RemoveSelf::create()));
	this->addChild(pSprite);
}

