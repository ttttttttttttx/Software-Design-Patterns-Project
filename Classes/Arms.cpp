#include "Arms.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;


/*��������
nID������ID
pos������λ��*/
void BuildArms::createArms(int nID, Vec2 tiledPos)
{
	//����Ƭ����ת��Ϊ�������꣬�õ�����������λ�á�
	Vec2 centerPos = CGameScene::getInstance()->getGameMap()->getPixelByTiledPos(tiledPos);

	//����һ������ʵ��
	CArmsDtMgr* pArmsDtMgr = static_cast<CArmsDtMgr*>(CDataMgr::getInstance()->getData("ArmsMgr"));
	SArmsDt* pArmsDt = static_cast<SArmsDt*>(pArmsDtMgr->getDataByID(nID));
	MyArms* pArms = MyArms::createWithData(pArmsDt);

	//����������λ��Ϊ����õ�������λ��
	pArms->setPosition(centerPos);
	this->addChild(pArms);

	//�������������۳���ң����ı�����ϵĽ����
	CGameScene::getInstance()->getUILayer()->addMoney(-pArmsDt->vecMoney[0]);//�۳����
}

/*���ڼ���������Ƭ�������Ƿ��Ѿ��������������򷵻ؽ��
tiledPos������*/
MyArms* BuildArms::isHaveArms(Vec2 tiledPos)
{
	//��ȡ������������ӽڵ�
	Vector<Node*> vecNodes = this->getChildren();
	for (Node* pNode : vecNodes)
	{
		Vec2 nodeTile = CGameScene::getInstance()->getGameMap()->getTiledByPos(pNode->getPosition());
		//��鷢���Ѿ�����
		if (nodeTile == tiledPos)
		{
			//���ظýڵ�
			return static_cast<MyArms*>(pNode);
		}
	}

	return NULL;
}

//��ʼ��
MyArms* MyArms::createWithData(SArmsDt* pArms)
{
	MyArms* pRef = new MyArms();

	if (pRef)
	{
		pRef->myArmsDt = pArms;
		pRef->Attack = false;
		pRef->myGrade = 1;
		pRef->lastBullet = nullptr;
		pRef->baseSprite = Sprite::createWithSpriteFrameName(StringUtils::format(pArms->strBaseImg.c_str(), pRef->myGrade));
		pRef->nowSprite = Sprite::createWithSpriteFrameName(StringUtils::format(pArms->strImg.c_str(), pRef->myGrade));

		pRef->addChild(pRef->baseSprite);
		pRef->addChild(pRef->nowSprite);

		pRef->scheduleUpdate();

		return pRef;
	}

	return NULL;
}

/*���������ӵ�
Node* pNode�������ӵ���Ŀ��ڵ�*/
void MyArms::fire(Node* pNode)
{
	//���� ��ƿ��̫������ �������Ŷ���
	if (myArmsDt->nBulletID ==6101|| myArmsDt->nBulletID ==6003|| myArmsDt->nBulletID == 6004)
	{
		CGameScene::getInstance()->getMyAnimate()->changeAction(nowSprite, myArmsDt->nAttackID + myGrade - 1, true, 0.1f);
	}
	//���� ��ƿ�ӡ����� ����������
	else
	{
		CGameScene::getInstance()->getMyAnimate()->changeAction(nowSprite, myArmsDt->nAttackID + myGrade - 1, false, 0.1f);
	}
	
	/*���ӵ�������ӵ�����ֵ�� m_pLastBullet
	myArmsDt->nBulletID���ӵ�ID
	this->getPosition()��������λ��
	pNode��Ŀ��ڵ�
	myArmsDt->vecRange[m_nGrade - 1] * 1.8�����
	m_nGrade�������ȼ�*/
	lastBullet =CGameScene::getInstance()->getBulletLayer()->addBullet(myArmsDt->nBulletID, this->getPosition(), pNode, myArmsDt->vecRange[myGrade -1]*1.8, myGrade);
	
	//���������óɷǹ���״̬
	Attack = false;

}

/*ʵ����������ÿ֡�����߼���
��������Ŀ����ת�������ӵ������ƹ���״̬���л��ȡ�
delta��*/
void MyArms::update(float delta)
{
	//������������̺�������ǰ��λ�ã���ȡ��������̷�Χ�ڵĹ���
	CMonster* pMonster = CGameScene::getInstance()->getMonsterLayer()->getMonster(myArmsDt->vecRange[myGrade - 1]*1.8, this->getPosition());
	//������������й���
	if (pMonster)
	{
		//����û�д��ڹ���״̬
		if (!Attack)
		{
			//�����﷢���ӵ�
			fire(pMonster);
			//������Ч
			if (myArmsDt->nBulletID == 6001) {
				AudioEngine::play2d("sound/Bottle.mp3", false, 1.3f);
			}
			if (myArmsDt->nBulletID == 6002) {
				AudioEngine::play2d("sound/Fan.mp3", false, 1.3f);
			}
			if (myArmsDt->nBulletID == 6003) {
				AudioEngine::play2d("sound/Sun.mp3", false, 1.3f);
			}
			if (myArmsDt->nBulletID == 6004) {
				AudioEngine::play2d("sound/Snow.mp3", false, 1.3f);
			}
			if (myArmsDt->nBulletID == 6101) {
				AudioEngine::play2d("sound/FireBottle.mp3", false, 1.3f);
			}

			//�������ڹ���״̬
			Attack = true;
			//���� ���ǻ�ƿ(�ڹ�����ʱ��ֻ�л�ƿ�Ǿ���ôһ���ӵ�)
			if (myArmsDt->nBulletID != 6101)
			{
				/*�ӳ�һ��ʱ��� m_bAttack ����Ϊ false
				* [=](float){m_bAttack = false;}��lambda ���ʽ��׽��ǰ������ı�����= ��ʾ��ֵ���񣬼������ⲿ������ֵ
				* m_pArmsDt->vecInterval[m_nGrade-1]���ӳ�ʱ��
				* fire��������ʱ�������ڱ�ʶ��������
				*/
				this->schedule([=](float) {Attack = false; }, myArmsDt->vecInterval[myGrade - 1], "fire");
			}
		}

		//������Ҫ��ת
		if (myArmsDt->nBulletID == 6001 || myArmsDt->nBulletID == 6101 )
		{
			Vec2 radianPos = pMonster->getPosition() - this->getPosition();
			float radian = radianPos.getAngle(Vec2(0, 1));
			//�������ľ�����ת���ýǶ�
			nowSprite->setRotation(CC_RADIANS_TO_DEGREES(radian));
		}
	}
	//��ǰ�����û�й���
	else
	{
		//ֹͣ���������ϵ����ж���
		nowSprite->stopAllActions();
		Attack = false;
		//ȡ����ʱ��
		this->unschedule("fire");
		//����������һ���ӵ����󣬲��Ҹ��ӵ�Ϊ���
		if (lastBullet && lastBullet->getTag() == 1)
		{
			//����ɵȼ�������������ӵ�
			lastBullet->removeFromParent();
		}
	}
}

void MyArms::upgrade()
{
	//ֹͣ���������ж���
	nowSprite->stopAllActions();
	//����
	myGrade++;
	//��ȡ��һ���ȼ�������ͼ��ľ���֡,������������������Ϊ�»�ȡ���ľ���֡,��ʾ�����������ͼ��
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(myArmsDt->strImg.c_str(), myGrade));
	nowSprite->setSpriteFrame(pFrame);

	//̫�����µ���
	if (myArmsDt->nBulletID == 6003) {
		SpriteFrame* pFrameBase = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(myArmsDt->strBaseImg.c_str(), myGrade));
		baseSprite->setSpriteFrame(pFrameBase);
	}

	//��޸��£�����ӵ�Ҳ����
	if (lastBullet && lastBullet->getTag() == 1)
	{
		//����ɵȼ�������������ӵ�
		lastBullet->removeFromParent();
		//��������������ڷǹ���״̬
		Attack =false;
	}
}

