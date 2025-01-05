#include "Bullet.h"
#include "GameScene.h"




//-----------------------------------------------Refactored with Flyweight Pattern - Initialize bullet with shared state------------------------------------------------//

/*Initialize bullet with shared state
SBulletDt* pBullet - Bullet data pointer
int nGrand - Bullet grade*/
bool CBulletBase::initWithData(SBulletDt* pBullet, int nGrand)
{
	// Get shared state from flyweight factory
	BulletFlyweight* flyweight = BulletFlyweightFactory::getFlyweight(pBullet->nID);
	if (!flyweight) {
		return false;
	}

	// Store flyweight reference
	setFlyweight(flyweight);

	// Initialize sprite
	Sprite::initWithSpriteFrameName(StringUtils::format(pBullet->strImg.c_str(), nGrand));
	
	// Apply shared state
	flyweight->applyTo(this);
	
	return true;
}

//-----------------------------------------------Create and initialize a new bullet------------------------------------------------//

/*Create and initialize a new bullet
int nID - Bullet ID
Vec2 pos - Bullet position
Node* pNode - Bullet node
float fAckRange - Bullet attack range
int nGrade - Bullet grade*/
CBulletBase* CBulletLayer::addBullet(int nID, Vec2 pos, Node* pNode, float fAckRange, int nGrade)
{
	CBulletDtMgr* pBulletDtMgr = static_cast<CBulletDtMgr*>(CDataMgr::getInstance()->getData("BulletMgr"));
	SBulletDt* pBulletDt = static_cast<SBulletDt*>(pBulletDtMgr->getDataByID(nID));

	//Create bullet
	CBulletBase* pBullet = createBullet(pBulletDt->strType, pBulletDt, nGrade);

	//Set bullet position and attack range
	pBullet->setPosition(pos);
	pBullet->setAckRange(fAckRange);

	//Bullet animation
	if (pBulletDt->strType == "Static")
	{
		//Update every 0.1 seconds
		pBullet->update(0.1f);

		//Get bullet animation
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(pBulletDt->nMoveAnimateID + nGrade - 1);//Animation

		CallFunc* pCall = CallFunc::create([=]() {
			pBullet->removeFromParent();
			});

		//Run bullet animation
		pBullet->runAction(Sequence::createWithTwoActions(pAnimate, pCall));
	}
	else
	{
		//Bullet
		Vec2 Dir = (pNode->getPosition() - pos).getNormalized();
		pBullet->setDir(Dir);

		//Normalize direction
		float fAngel = Dir.getAngle(Vec2(0, 1));
		pBullet->setRotation(CC_RADIANS_TO_DEGREES(fAngel));

		//Animation
		CGameScene::getInstance()->getMyAnimate()->changeAction(pBullet, pBulletDt->nMoveAnimateID + nGrade - 1, true, 0.2f);//Animation
	}

	this->addChild(pBullet);
	return pBullet;
}

// Create bullet
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
	// Invalid bullet type, default to NULL
	return NULL;
}

//-----------------------------------------------Collisions------------------------------------------------//

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
	//Get current bullet position
	Vec2 pos = getPosition();

	//Update bullet position
	pos.x = pos.x + mySpeed * delta * myDir.x;
	pos.y = pos.y + mySpeed * delta * myDir.y;

	//Set bullet position
	setPosition(pos);

}

/*Collisions with monsters
Node* pNode - Collision node*/
void CBulletCommon::collisions(Node* pNode)
{
	CMonster* pMonster = static_cast<CMonster*>(pNode);
	//Check if monster is hit
	if (pMonster->Damage(myAck))
	{
		pMonster->removeMonster();
	}
	//Death animation
	CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
	//Remove bullet
	removeFromParent();

}

//-----------------------------------------------------------------------------------------------//

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

/*Bullet path, bullet rotation*/
void CBulletThrough::update(float delta)
{
	Vec2 pos = this->getPosition();    //Get current bullet position 

	//Update bullet position
	pos.x = pos.x + mySpeed * delta * myDir.x;
	pos.y = pos.y + mySpeed * delta * myDir.y;

	// Bullet rotation
	this->setRotation(this->getRotation() + 500.0f * delta);
	this->setPosition(pos);
}

/*Collisions with monsters
Node* pNode - Collision node*/
void CBulletThrough::collisions(Node* pNode)
{
	//Check if monster is hit
	if (myAckNode != pNode)
	{
		//Previous myAckNode is pNode
		myAckNode = pNode;
		CMonster* pMonster = static_cast<CMonster*>(myAckNode);
		//Death animation
		CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
		//Check if monster is hit
		if (pMonster->Damage(myAck))
		{
			pMonster->removeMonster();
		}
	}
}

//-----------------------------------------------Collisions------------------------------------------------//

/*Initialize bullet
SBulletDt* pBullet - Bullet data pointer
int nGrade - Bullet grade*/
bool CBulletRadial::initWithData(SBulletDt* pBullet, int nGrade)
{
	if (!CBulletBase::initWithData(pBullet, nGrade))
	{
		return false;
	}
	//Bullet image anchor point
	setAnchorPoint(Vec2(0.5, 0));

	setTag(1);
	scheduleUpdate();
	//One sprite is used for animation
	nowSprite = Sprite::create();
	nowSprite->setVisible(false);
	addChild(nowSprite);
	return true;
}

/*Create and initialize a new bullet*/
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

/*Collisions with monsters
Node* pNode - Collision node*/
void CBulletRadial::update(float delta)
{
	CMonster* pMonster = CGameScene::getInstance()->getMonsterLayer()->getMonster(myAckRange, getPosition());
	//Check if monster is hit
	if (pMonster)
	{
		if (!nowSprite->isVisible())
		{
			nowSprite->setVisible(true);
			nowSprite->setPosition(convertToNodeSpace(pMonster->getPosition()));

			//Death animation
			CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
		}


		Vec2 Pos = pMonster->getPosition() - getPosition();
		//Bullet rotation
		float radian = Pos.getAngle(Vec2(0, 1));
		setRotation(CC_RADIANS_TO_DEGREES(radian));
		//Bullet
		float multiple = Pos.getLength() / getContentSize().height;
		setScaleY(multiple);

		//Check if monster is hit
		if (pMonster->Damage(myAck))
		{
			pMonster->removeMonster();
		}
	}
}

//-----------------------------------------------Bullet animation------------------------------------------------//

//Initialize bullet
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

//Bullet animation
void CBulletStatic::update(float delta)
{
	//Get bullet range and vector
	vector<CMonster*> vecMonster = CGameScene::getInstance()->getMonsterLayer()->getMonsterVec(myAckRange, this->getPosition());

	for (int i = 0; i < vecMonster.size(); i++)
	{
		//Check if monster is hit
		if (vecMonster[i]->Damage(myAck))
		{
			vecMonster[i]->removeMonster();
		}
		else {
			//Add buff to monster
			CGameScene::getInstance()->getBuffLayer()->addBuff(vecMonster[i], myBuffID);
		}
	}
}
