#include "Bullet.h"
#include "GameScene.h"




//-----------------------------------------------子弹基类------------------------------------------------//

/*初始化子弹对象
SBulletDt* pBullet：子弹指针
int nGrand：子弹等级*/
bool CBulletBase::initWithData(SBulletDt* pBullet, int nGrand)
{
	Sprite::initWithSpriteFrameName(StringUtils::format(pBullet->strImg.c_str(), nGrand));
	mySpeed = pBullet->fSpeed;
	myBuffID = pBullet->nBuffId;
	myAck = pBullet->nAck;
	myDieID = pBullet->nDieID;
	return true;
}

//-----------------------------------------------创建子弹------------------------------------------------//

/*添加子弹
nID：武器ID
pos：放置武器的位置
pNode：武器节点
fAckRange：命中范围
nGeade：等级*/
CBulletBase* CBulletLayer::addBullet(int nID, Vec2 pos, Node* pNode, float fAckRange, int nGrade)
{
	CBulletDtMgr* pBulletDtMgr = static_cast<CBulletDtMgr*>(CDataMgr::getInstance()->getData("BulletMgr"));
	SBulletDt* pBulletDt = static_cast<SBulletDt*>(pBulletDtMgr->getDataByID(nID));

	//根据建立一个子弹对象的结点
	CBulletBase* pBullet = createBullet(pBulletDt->strType, pBulletDt, nGrade);

	//给该结点赋值 位置 命中范围 
	pBullet->setPosition(pos);
	pBullet->setAckRange(fAckRange);

	//子弹是太阳和冰，播完一个动画再播下一个
	if (pBulletDt->strType == "Static")
	{
		//每0.1f更新一次
		pBullet->update(0.1f);

		//回调函数，用于在动画播放完成后从父节点中移除子弹。
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(pBulletDt->nMoveAnimateID + nGrade - 1);//添加动画

		CallFunc* pCall = CallFunc::create([=]() {
			pBullet->removeFromParent();
			});

		//使用动作序列执行动画和回调函数
		pBullet->runAction(Sequence::createWithTwoActions(pAnimate, pCall));
	}
	else
	{
		//设置子弹方向
		Vec2 Dir = (pNode->getPosition() - pos).getNormalized();
		pBullet->setDir(Dir);

		//计算夹角并旋转
		float fAngel = Dir.getAngle(Vec2(0, 1));
		pBullet->setRotation(CC_RADIANS_TO_DEGREES(fAngel));

		//连续添加动画
		CGameScene::getInstance()->getMyAnimate()->changeAction(pBullet, pBulletDt->nMoveAnimateID + nGrade - 1, true, 0.2f);//添加动画
	}

	this->addChild(pBullet);
	return pBullet;
}

class BulletFlyweight {
public:
	BulletFlyweight(const std::string& spriteFrameName, float speed, int buffID, int ack, int dieID)
		: spriteFrameName(spriteFrameName), speed(speed), buffID(buffID), ack(ack), dieID(dieID) {
	}

	const std::string& getSpriteFrameName() const { return spriteFrameName; }
	float getSpeed() const { return speed; }
	int getBuffID() const { return buffID; }
	int getAck() const { return ack; }
	int getDieID() const { return dieID; }

private:
	std::string spriteFrameName;
	float speed;
	int buffID;
	int ack;
	int dieID;
};

class BulletFlyweightFactory {
public:
	static BulletFlyweight* getFlyweight(int bulletID) {
		auto it = flyweights.find(bulletID);
		if (it != flyweights.end()) {
			return &it->second;
		}

		// 模拟从配置加载
		if (bulletID == 1) {
			flyweights[bulletID] = BulletFlyweight("common_bullet.png", 200.0f, 1, 10, 100);
		}
		else if (bulletID == 2) {
			flyweights[bulletID] = BulletFlyweight("through_bullet.png", 250.0f, 2, 15, 101);
		}

		return &flyweights[bulletID];
	}

private:
	static std::unordered_map<int, BulletFlyweight> flyweights;
};

// 创建子弹对象的函数
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
	// 如果类型不匹配，返回nullptr或者适当的默认值
	return NULL;
}

//-----------------------------------------------绿瓶子------------------------------------------------//

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
	//获取当前子弹对象的位置
	Vec2 pos = getPosition();

	//更新子弹位置
	pos.x = pos.x + mySpeed * delta * myDir.x;
	pos.y = pos.y + mySpeed * delta * myDir.y;

	//将更新后的位置应用到子弹对象上
	setPosition(pos);

}

/*普通子弹 绿瓶子 攻击怪物*/
void CBulletCommon::collisions(Node* pNode)
{
	CMonster* pMonster = static_cast<CMonster*>(pNode);
	//怪物没血了，移除怪物
	if (pMonster->Damage(myAck))
	{
		pMonster->removeMonster();
	}
	//在碰撞位置，显示子碰撞的特效
	CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
	//将子弹移除
	removeFromParent();

}

//-----------------------------------------------风扇------------------------------------------------//

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

/*更新子弹路径,让子弹不断旋转*/
void CBulletThrough::update(float delta)
{
	Vec2 pos = this->getPosition();    //获取当前子弹的位置 

	//更新子弹位置
	pos.x = pos.x + mySpeed * delta * myDir.x;
	pos.y = pos.y + mySpeed * delta * myDir.y;

	// 让子弹自身不断旋转
	this->setRotation(this->getRotation() + 500.0f * delta);
	this->setPosition(pos);
}

/*穿过子弹 风扇 攻击怪物*/
void CBulletThrough::collisions(Node* pNode)
{
	//没有发生碰撞
	if (myAckNode != pNode)
	{
		//将当前对象的 myAckNode 更新为传入的节点 pNode
		myAckNode = pNode;
		CMonster* pMonster = static_cast<CMonster*>(myAckNode);
		//在碰撞位置，显示子碰撞的特效
		CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
		//怪物没血了，移除怪物
		if (pMonster->Damage(myAck))
		{
			pMonster->removeMonster();
		}
	}
}

//-----------------------------------------------火瓶子------------------------------------------------//

/*初始化辐射子弹 火瓶子 对象的数据*/
bool CBulletRadial::initWithData(SBulletDt* pBullet, int nGrade)
{
	if (!CBulletBase::initWithData(pBullet, nGrade))
	{
		return false;
	}
	//将火瓶子的图像的锚点设为子弹的底部
	setAnchorPoint(Vec2(0.5, 0));

	setTag(1);
	scheduleUpdate();
	//创建一个新的sprite，设为不可见
	nowSprite = Sprite::create();
	nowSprite->setVisible(false);
	addChild(nowSprite);
	return true;
}

/*安全的建立一个辐射子弹*/
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

/*辐射子弹 火瓶子 更新和攻击动画*/
void CBulletRadial::update(float delta)
{
	CMonster* pMonster = CGameScene::getInstance()->getMonsterLayer()->getMonster(myAckRange, getPosition());
	//子弹攻击范围里有怪物
	if (pMonster)
	{
		if (!nowSprite->isVisible())
		{
			nowSprite->setVisible(true);
			nowSprite->setPosition(convertToNodeSpace(pMonster->getPosition()));

			//在碰撞位置，显示子碰撞的特效
			CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), myDieID);
		}


		Vec2 Pos = pMonster->getPosition() - getPosition();
		//子弹旋转
		float radian = Pos.getAngle(Vec2(0, 1));
		setRotation(CC_RADIANS_TO_DEGREES(radian));
		//子弹缩放
		float multiple = Pos.getLength() / getContentSize().height;
		setScaleY(multiple);

		//怪物没血了，移除怪物
		if (pMonster->Damage(myAck))
		{
			pMonster->removeMonster();
		}
	}
}

//-----------------------------------------------太阳和冰------------------------------------------------//

//初始化禁止子弹 太阳，冰 对象的数据
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

//更新禁止子弹 太阳，冰
void CBulletStatic::update(float delta)
{
	//获取怪物层中子弹攻击范围里的所有怪物，存放在vector数组中
	vector<CMonster*> vecMonster = CGameScene::getInstance()->getMonsterLayer()->getMonsterVec(myAckRange, this->getPosition());

	for (int i = 0; i < vecMonster.size(); i++)
	{
		//没了就移除
		if (vecMonster[i]->Damage(myAck))
		{
			vecMonster[i]->removeMonster();
		}
		else {
			//没有移除的怪物就掉血
			CGameScene::getInstance()->getBuffLayer()->addBuff(vecMonster[i], myBuffID);
		}
	}
}
