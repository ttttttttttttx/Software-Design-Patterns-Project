#include "Arms.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;


/*创建武器
nID：武器ID
pos：武器位置*/
void BuildArms::createArms(int nID, Vec2 tiledPos)
{
	//将瓦片坐标转换为像素坐标，得到武器的中心位置。
	Vec2 centerPos = CGameScene::getInstance()->getGameMap()->getPixelByTiledPos(tiledPos);

	//创建一个武器实例
	CArmsDtMgr* pArmsDtMgr = static_cast<CArmsDtMgr*>(CDataMgr::getInstance()->getData("ArmsMgr"));
	SArmsDt* pArmsDt = static_cast<SArmsDt*>(pArmsDtMgr->getDataByID(nID));
	MyArms* pArms = MyArms::createWithData(pArmsDt);

	//设置武器的位置为计算得到的中心位置
	pArms->setPosition(centerPos);
	this->addChild(pArms);

	//购买了武器，扣除金币，并改变界面上的金币数
	CGameScene::getInstance()->getUILayer()->addMoney(-pArmsDt->vecMoney[0]);//扣除金币
}

/*用于检查给定的瓦片坐标上是否已经存在武器，有则返回结点
tiledPos：坐标*/
MyArms* BuildArms::isHaveArms(Vec2 tiledPos)
{
	//获取武器层的所有子节点
	Vector<Node*> vecNodes = this->getChildren();
	for (Node* pNode : vecNodes)
	{
		Vec2 nodeTile = CGameScene::getInstance()->getGameMap()->getTiledByPos(pNode->getPosition());
		//检查发现已经存在
		if (nodeTile == tiledPos)
		{
			//返回该节点
			return static_cast<MyArms*>(pNode);
		}
	}

	return NULL;
}

//初始化
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

/*武器发射子弹
Node* pNode：发射子弹的目标节点*/
void MyArms::fire(Node* pNode)
{
	//对于 火瓶、太阳、冰 持续播放动画
	if (myArmsDt->nBulletID ==6101|| myArmsDt->nBulletID ==6003|| myArmsDt->nBulletID == 6004)
	{
		CGameScene::getInstance()->getMyAnimate()->changeAction(nowSprite, myArmsDt->nAttackID + myGrade - 1, true, 0.1f);
	}
	//对于 绿瓶子、风扇 不持续播放
	else
	{
		CGameScene::getInstance()->getMyAnimate()->changeAction(nowSprite, myArmsDt->nAttackID + myGrade - 1, false, 0.1f);
	}
	
	/*在子弹层添加子弹，赋值给 m_pLastBullet
	myArmsDt->nBulletID：子弹ID
	this->getPosition()：武器的位置
	pNode：目标节点
	myArmsDt->vecRange[m_nGrade - 1] * 1.8：射程
	m_nGrade：武器等级*/
	lastBullet =CGameScene::getInstance()->getBulletLayer()->addBullet(myArmsDt->nBulletID, this->getPosition(), pNode, myArmsDt->vecRange[myGrade -1]*1.8, myGrade);
	
	//将武器设置成非攻击状态
	Attack = false;

}

/*实现了武器的每帧更新逻辑，
包括根据目标旋转、发射子弹、控制攻击状态的切换等。
delta：*/
void MyArms::update(float delta)
{
	//根据武器的射程和武器当前的位置，获取在武器射程范围内的怪物
	CMonster* pMonster = CGameScene::getInstance()->getMonsterLayer()->getMonster(myArmsDt->vecRange[myGrade - 1]*1.8, this->getPosition());
	//当所在射程中有怪物
	if (pMonster)
	{
		//武器没有处于攻击状态
		if (!Attack)
		{
			//朝怪物发射子弹
			fire(pMonster);
			//武器音效
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

			//武器处于攻击状态
			Attack = true;
			//对于 不是火瓶(在攻击的时候，只有火瓶是就那么一个子弹)
			if (myArmsDt->nBulletID != 6101)
			{
				/*延迟一定时间后将 m_bAttack 设置为 false
				* [=](float){m_bAttack = false;}：lambda 表达式捕捉当前作用域的变量，= 表示按值捕获，即复制外部变量的值
				* m_pArmsDt->vecInterval[m_nGrade-1]：延迟时间
				* fire：命名定时器，用于标识调度任务
				*/
				this->schedule([=](float) {Attack = false; }, myArmsDt->vecInterval[myGrade - 1], "fire");
			}
		}

		//武器需要旋转
		if (myArmsDt->nBulletID == 6001 || myArmsDt->nBulletID == 6101 )
		{
			Vec2 radianPos = pMonster->getPosition() - this->getPosition();
			float radian = radianPos.getAngle(Vec2(0, 1));
			//将武器的精灵旋转到该角度
			nowSprite->setRotation(CC_RADIANS_TO_DEGREES(radian));
		}
	}
	//当前射程中没有怪物
	else
	{
		//停止武器精灵上的所有动作
		nowSprite->stopAllActions();
		Attack = false;
		//取消定时器
		this->unschedule("fire");
		//如果存在最后一个子弹对象，并且该子弹为火罐
		if (lastBullet && lastBullet->getTag() == 1)
		{
			//清除旧等级武器所发射的子弹
			lastBullet->removeFromParent();
		}
	}
}

// Refactored with Decorator Pattern
//升级
void MyArms::upgrade()
{
	//停止武器的所有动作
	nowSprite->stopAllActions();
	//升级
	myGrade++;
	//获取下一个等级的武器图像的精灵帧,将武器的主精灵设置为新获取到的精灵帧,显示武器升级后的图像。
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(myArmsDt->strImg.c_str(), myGrade));
	nowSprite->setSpriteFrame(pFrame);
}