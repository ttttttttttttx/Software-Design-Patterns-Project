#include "Bullet.h"
#include "GameScene.h"




//-----------------------------------------------�ӵ�����------------------------------------------------//

/*��ʼ���ӵ�����
SBulletDt* pBullet���ӵ�ָ��
int nGrand���ӵ��ȼ�*/
bool CBulletBase::initWithData(SBulletDt* pBullet, int nGrand)
{
	Sprite::initWithSpriteFrameName(StringUtils::format(pBullet->strImg.c_str(), nGrand));
	mySpeed = pBullet->fSpeed;
	myBuffID = pBullet->nBuffId;
	myAck = pBullet->nAck;
	myDieID = pBullet->nDieID;
	return true;
}

//-----------------------------------------------�����ӵ�------------------------------------------------//

/*����ӵ�
nID������ID
pos������������λ��
pNode�������ڵ�
fAckRange�����з�Χ
nGeade���ȼ�*/
CBulletBase* CBulletLayer::addBullet(int nID, Vec2 pos, Node* pNode, float fAckRange, int nGrade)
{
	CBulletDtMgr* pBulletDtMgr = static_cast<CBulletDtMgr*>(CDataMgr::getInstance()->getData("BulletMgr"));
	SBulletDt* pBulletDt = static_cast<SBulletDt*>(pBulletDtMgr->getDataByID(nID));

	//���ݽ���һ���ӵ�����Ľ��
	CBulletBase* pBullet = createBullet(pBulletDt->strType, pBulletDt, nGrade);

	//���ý�㸳ֵ λ�� ���з�Χ 
	pBullet->setPosition(pos);
	pBullet->setAckRange(fAckRange);

	//�ӵ���̫���ͱ�������һ�������ٲ���һ��
	if (pBulletDt->strType == "Static"  )
	{
		//ÿ0.1f����һ��
		pBullet->update(0.1f);

        //�ص������������ڶ���������ɺ�Ӹ��ڵ����Ƴ��ӵ���
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(pBulletDt->nMoveAnimateID + nGrade - 1);//��Ӷ���
		
		CallFunc* pCall = CallFunc::create([=]() {
			pBullet->removeFromParent();
			});

		//ʹ�ö�������ִ�ж����ͻص�����
		pBullet->runAction(Sequence::createWithTwoActions(pAnimate, pCall));
	}
	else
	{
		//�����ӵ�����
		Vec2 Dir = (pNode->getPosition() - pos).getNormalized();
		pBullet->setDir(Dir);

		//����нǲ���ת
		float fAngel = Dir.getAngle(Vec2(0, 1));
		pBullet->setRotation(CC_RADIANS_TO_DEGREES(fAngel));

		//������Ӷ���
		CGameScene::getInstance()->getMyAnimate()->changeAction(pBullet, pBulletDt->nMoveAnimateID + nGrade - 1, true, 0.2f);//��Ӷ���
	}

	this->addChild(pBullet);
	return pBullet;
}

// �����ӵ�����ĺ���
CBulletBase* CBulletLayer::createBullet(const string& type, SBulletDt* pBulletDt, int nGrand) {
	if (type == "Common") {
		return CBulletCommon::createWithData(pBulletDt, nGrand);
	}
	else if (type == "Through") {
		return CBulletThrough::createWithData(pBulletDt, nGrand);
	}
	else if (type == "Radial") {
		return CBulletRadial::createWithData(pBulletDt, nGrand);
	}
	else if (type == "Static") {
		return CBulletStatic::createWithData(pBulletDt, nGrand);
	}
	// ������Ͳ�ƥ�䣬����nullptr�����ʵ���Ĭ��ֵ
	return NULL;
}

//-----------------------------------------------��ƿ��------------------------------------------------//

CBulletCommon* CBulletCommon::createWithData(SBulletDt* pBullet, int nGrade)
{
	CBulletCommon* pRef = new CBulletCommon();
	if (pRef->CBulletBase::initWithData(pBullet, nGrade))
	{
		pRef->scheduleUpdate();
		return pRef;
	}
	delete pRef;
	return NULL;
}

void CBulletCommon::update(float delta)
{
	//��ȡ��ǰ�ӵ������λ��
	Vec2 pos = getPosition();

	//�����ӵ�λ��
	pos.x = pos.x + mySpeed * delta * myDir.x;
	pos.y = pos.y + mySpeed * delta * myDir.y;

	//�����º��λ��Ӧ�õ��ӵ�������
	setPosition(pos);

}

/*��ͨ�ӵ� ��ƿ�� ��������*/
void CBulletCommon::collisions(Node* pNode)
{
	CMonster* pMonster = static_cast<CMonster*>(pNode);
	//����ûѪ�ˣ��Ƴ�����
	if (pMonster->Damage(myAck))
	{
		pMonster->removeMonster();
	}
	//����ײλ�ã���ʾ����ײ����Ч
	CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
	//���ӵ��Ƴ�
	removeFromParent();

}

//-----------------------------------------------����------------------------------------------------//

CBulletThrough* CBulletThrough::createWithData(SBulletDt* pBullet, int nGrade)
{
	CBulletThrough* pRef = new CBulletThrough();
	if (pRef->CBulletBase::initWithData(pBullet, nGrade))
	{
		pRef->scheduleUpdate();
		return pRef;
	}
	delete pRef;
	return NULL;
}

/*�����ӵ�·��,���ӵ�������ת*/
void CBulletThrough::update(float delta)
{
	Vec2 pos = this->getPosition();    //��ȡ��ǰ�ӵ���λ�� 

	//�����ӵ�λ��
	pos.x = pos.x + mySpeed * delta * myDir.x;
	pos.y = pos.y + mySpeed * delta * myDir.y;

	// ���ӵ���������ת
	this->setRotation(this->getRotation() + 500.0f * delta);
	this->setPosition(pos);  
}

/*�����ӵ� ���� ��������*/
void CBulletThrough::collisions(Node* pNode)
{
	//û�з�����ײ
	if (myAckNode !=pNode)
	{
		//����ǰ����� myAckNode ����Ϊ����Ľڵ� pNode
		myAckNode = pNode;
		CMonster* pMonster = static_cast<CMonster*>(myAckNode);
		//����ײλ�ã���ʾ����ײ����Ч
		CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
		//����ûѪ�ˣ��Ƴ�����
		if (pMonster->Damage(myAck))
		{
			pMonster->removeMonster();
		}
	}
}

//-----------------------------------------------��ƿ��------------------------------------------------//

/*��ʼ�������ӵ� ��ƿ�� ���������*/
bool CBulletRadial::initWithData(SBulletDt* pBullet, int nGrade)
{
	if (!CBulletBase::initWithData(pBullet, nGrade))
	{
		return false;
	}
	//����ƿ�ӵ�ͼ���ê����Ϊ�ӵ��ĵײ�
	setAnchorPoint(Vec2(0.5, 0));

	setTag(1);
	scheduleUpdate();
	//����һ���µ�sprite����Ϊ���ɼ�
	nowSprite = Sprite::create();
	nowSprite->setVisible(false);
	addChild(nowSprite);
	return true;
}

/*��ȫ�Ľ���һ�������ӵ�*/
CBulletRadial* CBulletRadial::createWithData(SBulletDt* pBullet, int nGrade)
{
	CBulletRadial* pRef = new CBulletRadial();
	if (pRef->initWithData(pBullet, nGrade))
	{
		return pRef;
	}
	delete pRef;
	return nullptr;
}

/*�����ӵ� ��ƿ�� ���º͹�������*/
void CBulletRadial::update(float delta)
{
	CMonster* pMonster=CGameScene::getInstance()->getMonsterLayer()->getMonster(myAckRange, getPosition());
	//�ӵ�������Χ���й���
	if (pMonster)
	{
		if (!nowSprite->isVisible())
		{
			nowSprite->setVisible(true);
			nowSprite->setPosition(convertToNodeSpace(pMonster->getPosition()));

			//����ײλ�ã���ʾ����ײ����Ч
			CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
		}

		
		Vec2 Pos = pMonster->getPosition() - getPosition();
		//�ӵ���ת
		float radian = Pos.getAngle(Vec2(0, 1));
        setRotation(CC_RADIANS_TO_DEGREES(radian));
		//�ӵ�����
		float multiple = Pos.getLength() / getContentSize().height;
		setScaleY(multiple);

		//����ûѪ�ˣ��Ƴ�����
		if (pMonster->Damage(myAck))
		{
			pMonster->removeMonster();
		}
	}
}

//-----------------------------------------------̫���ͱ�------------------------------------------------//

//��ʼ����ֹ�ӵ� ̫������ ���������
CBulletStatic* CBulletStatic::createWithData(SBulletDt* pBullet, int nGrade)
{
	CBulletStatic* pRef = new CBulletStatic();
	if (pRef->CBulletBase::initWithData(pBullet, nGrade))
	{
		pRef->scheduleUpdate();
		return pRef;
	}
	delete pRef;
	return NULL;
}

//���½�ֹ�ӵ� ̫������
void CBulletStatic::update(float delta)
{
	//��ȡ��������ӵ�������Χ������й�������vector������
	vector<CMonster*> vecMonster = CGameScene::getInstance()->getMonsterLayer()->getMonsterVec(myAckRange, this->getPosition());

	for (int i = 0; i < vecMonster.size();i++)
	{
		//û�˾��Ƴ�
		if (vecMonster[i]->Damage(myAck))
		{
			vecMonster[i]->removeMonster();
		}
		else {
		//û���Ƴ��Ĺ���͵�Ѫ
		CGameScene::getInstance()->getBuffLayer()->addBuff(vecMonster[i], myBuffID);
		}
	}
}
