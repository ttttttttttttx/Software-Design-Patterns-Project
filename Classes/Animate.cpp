#include "Animate.h"

bool MyAnimate::init()
{
	myAnimateDtMgr = static_cast<CAnimateDtMgr*>(CDataMgr::getInstance()->getData("AnimateMgr"));//�õ��������ݹ�����
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
	//С��3000�������޺͵�ͼ�������������
	if (nID<3000)
	{
		return;
	}
	//ֹͣ���б��Ϊ 1 �Ķ�����
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
		//����Ҫ���ò��ţ�����������ı�ǩ����Ϊ 1
		pAnimate->setTag(1);
		pSprite->runAction(pAnimate);
	}
}

Animate* MyAnimate::getAnimate(int nID)
{
	Animation* pAnimation = seriesAction(nID);

	//�����ӳ�ʱ��
	//�������Ž�����ԭ����ʼ֡
	pAnimation->setDelayPerUnit(0.1f);
	pAnimation->setRestoreOriginalFrame(true);

	Animate* pAnimate = Animate::create(pAnimation);
	//���ö�������ı�ǩΪ 1��
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

	//������ж������Ȳ��Ŷ�����Ȼ���Ƴ���������
	pSprite->runAction(Sequence::createWithTwoActions(pAnimate,RemoveSelf::create()));
	this->addChild(pSprite);
}

