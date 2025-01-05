#include "Card.h"
#include "GameScene.h"

//-------------------------------------小武器卡片---------------------------------------//

MyCard* MyCard::createMoneyCard(SCardDt* pCard)
{
	MyCard* pRef = new (std::nothrow) MyCard(); // 使用 std::nothrow 来避免抛出异常

	if (pRef)
	{
		if (pRef->Sprite::initWithSpriteFrameName(StringUtils::format(pCard->strImg.c_str(), 1)))
		{
			// 设置成员变量
			pRef->myArmsID = pCard->nArmsID;
			pRef->CardName = pCard->strImg;

			// 读取武器数据
			CArmsDtMgr* pArmsDtMgr = static_cast<CArmsDtMgr*>(CDataMgr::getInstance()->getData("ArmsMgr"));
			SArmsDt* pArmsDt = static_cast<SArmsDt*>(pArmsDtMgr->getDataByID(pRef->myArmsID));
			pRef->myArmsMoney = pArmsDt->vecMoney[0];

			return pRef;
		}

		CC_SAFE_DELETE(pRef);
	}

	return NULL;
}

/*通过钱够不够显示图层的亮与暗
nMoney：现在有多少钱*/
void MyCard::ifMoney(int myMoney)
{
	SpriteFrame* pFrame;
	//比较传入的钱和现在的钱，显示图片的亮和暗
	if (myMoney >= myArmsMoney)
	{
		pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(CardName.c_str(), 1));
	}
	else
	{
		pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(CardName.c_str(), 0));
	}
	setSpriteFrame(pFrame);
}

//----------------------------------------创建武器卡片------------------------------------------//

bool ArmsCard::init()
{
	this->addTouch();
	Sprite* pSelect = Sprite::createWithSpriteFrameName("select_01.png");
	addChild(pSelect);
	createAllArmsCard();
	//这样点一下就可以看到框框了
	setVisible(false);
	return true;
}

/*创建卡片*/
void ArmsCard::createAllArmsCard()
{
	cardContainers = Sprite::create();
	CLevelDtMgr* myLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	CCardDtMgr* myCardDtMgr = static_cast<CCardDtMgr*>(CDataMgr::getInstance()->getData("CardMgr"));

	int i = 0;
	for (i = 0; i < myLevelDtMgr->getCurData()->CardID.size();i++)
	{  
		int nID = myLevelDtMgr->getCurData()->CardID[i];

		MyCard* myCard = MyCard::createMoneyCard(static_cast<SCardDt*>(myCardDtMgr->getDataByID(nID)));
		myCard->setPosition(i* myCard->getContentSize().width, 0);
		myCard->setAnchorPoint(Vec2(0,0));

		// 将卡片添加到卡片容器中，并保存卡片对象到向量中
		cardContainers->addChild(myCard);
		allCard.push_back(myCard);
	}

	int allhigh = allCard[0]->getContentSize().height;
	int allwidth = (allCard[0]->getContentSize().width) * i;
	cardContainers->setContentSize(Size(allwidth, allhigh));
	cardContainers->setAnchorPoint(Vec2(0, 0));

	// 将卡片容器添加到当前层中
	addChild(cardContainers);
}

void ArmsCard::clickEventFirst(Vec2 clickPos)
{
	// 将点击坐标转换为地图瓦片坐标
	Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(clickPos);

		// 设置卡片层的卡片精灵和位置
		for (MyCard* nowCard : allCard)
		{
			nowCard->ifMoney(CGameScene::getInstance()->getUILayer()->getMoney());
		}
		
		//是卡片不会越出边界
		if (clickPos.x + (allCard[0]->getContentSize().width) * allCard.size() > WINSIZE.width)
		{
			cardContainers->setAnchorPoint(Vec2(1, 0));
		}

		Vec2 TiledPos = tiledPos;
		Vec2 Pos = CGameScene::getInstance()->getGameMap()->getPixelByTiledPos(tiledPos);
		setPosition(Pos);
		setVisible(true);
}

void ArmsCard::clickEventSecond(Vec2 clickPos) {

	MyCard* nowCard=NULL;

	//遍历所有卡片找到被点击的卡片
	for (MyCard* pCard : allCard)
	{
		if (pCard->getBoundingBox().containsPoint(cardContainers->convertToNodeSpace(clickPos)))
		{
			nowCard = pCard;
			break ;
		}
	}

	if (nowCard!=NULL && CGameScene::getInstance()->getUILayer()->getMoney() >= nowCard->getMoney())//拥有金币大于卡片所需金币
	{
		//将世界坐标 转换为地图瓦片坐标
		Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(this->getPosition());
		//如果给定位置已经存在武器，直接返回
		if (!CGameScene::getInstance()->getBuildArms()->isHaveArms(tiledPos))
		{
			// 创建武器并放置在卡片层的位置
			CGameScene::getInstance()->getBuildArms()->createArms(nowCard->getArmsID(), tiledPos);
		}
	}

	// 改变可见值，便于下一次点击
	setVisible(false);
	cardContainers->setAnchorPoint(Vec2(0, 0));
}

//-----------------------------------------升级武器卡片------------------------------------------//

bool UpCard::init()
{
	myUpGrade = Sprite::create();
	mySell = Sprite::create();
	myRander = Sprite::create();

	myUpGrade->setAnchorPoint(Vec2(0.5, -0.5));
	mySell->setAnchorPoint(Vec2(0.5, 1.5));

	this->addChild(myRander);
	this->addChild(myUpGrade);
	this->addChild(mySell);
	this->setVisible(false);
	return true;
}

void UpCard::clickEventFirst(Vec2 clickPos, MyArms* pArms)
{
	//第一次点击到有武器的地方,显示卡片
	Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(clickPos);
	Vec2 Pos = CGameScene::getInstance()->getGameMap()->getPixelByTiledPos(tiledPos);
	this->setPosition(Pos);
	this->upCard(pArms);
	nowArms = pArms;
	this->setVisible(true);
}

void UpCard::clickEventSecond(Vec2 clickPos, MyArms* pArms) {

	//升级
	if (myUpGrade->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		//卡片不是顶级卡片
		if (nowArms->getGrade() < nowArms->getArmsDt()->vecMoney.size()) {
			//拿到升级所需金币
			int nMoney = nowArms->getArmsDt()->vecMoney[nowArms->getGrade()];
			//剩余金币必须大于升级所需金币
			// 装饰器部分客户端代码
			//升级
			// Refactored with Decorator Pattern
			CGameScene::getInstance()->getUILayer()->addMoney(-nMoney);
			try {
				// 创建基础武器
				auto myArms = MyArms::create();

				// 创建装饰器并升级武器
				auto decoratedArms = std::make_unique<RangeAndSpeedDecorator>(
					new DamageDecorator(
						new CritDecorator(myArms, 0.02f), // 每次升级增加2%的暴击率
						5.0f // 每次升级增加5点伤害
					),
					10.0f, // 每次升级增加10单位射程
					0.1f  // 每次升级增加0.1的射速
				);
				decoratedArms->upgrade(); // 执行升级操作

				// 将装饰后的武器添加到场景中
				scene->addChild(decoratedArms.release());

				// 使用升级后的武器发射子弹
				decoratedArms->fire();
			}
			catch (const std::exception& e) {
				log("Error: %s", e.what());
			}
		}
	}

	//出售
	if (mySell->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		int allNum = 0;
		//计算对这个武器花费了我多少钱
		for (int i = 0; i < nowArms->getGrade(); i++)
		{
			allNum = allNum + nowArms->getArmsDt()->vecMoney[i];
		}
		//按8折卖出
		CGameScene::getInstance()->getUILayer()->addMoney(allNum * 0.8);

		//将武器移除出去
		nowArms->removeFromParent();
		//把最后一次的子弹也移除出去
		if (nowArms->getLastBullet() && nowArms->getLastBullet()->getTag() == 1)
		{
			nowArms->getLastBullet()->removeFromParent();
		}
	}

	this->setVisible(false);
}

void UpCard::upCard(MyArms* pArms)
{

	//设置为范围指示器的精灵帧
	SpriteFrame* pRander = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("range_%d.png", pArms->getArmsDt()->vecRange[pArms->getGrade() - 1]));
	myRander->setSpriteFrame(pRander);

	//升级
	string strName;
	//如果武器没有达到最大等级。
	if (pArms->getGrade() < pArms->getArmsDt()->vecMoney.size()) {
		//存储升级武器的费用或负费用
		int nMoney = 0;
		//检查玩家当前的金币是否大于或等于升级武器到下一级所需的费用。
		if (CGameScene::getInstance()->getUILayer()->getMoney() >= pArms->getArmsDt()->vecMoney[pArms->getGrade()])
		{
			//如果为真，则 nMoney 被设置为实际的升级费用。
			nMoney = pArms->getArmsDt()->vecMoney[pArms->getGrade()];
		}
		else {
			//如果为假，则 nMoney 被设置为升级费用的负值，表示玩家没有足够的金币，卡片应该显示为灰色。
			nMoney = -pArms->getArmsDt()->vecMoney[pArms->getGrade()];
		}
		//根据升级费用 (nMoney) 生成"升级按钮"的图像文件名称。
		strName = StringUtils::format("upgrade_%d.png", nMoney);
	}
	//如果武器已经达到最大等级，图像文件名称被设置为代表最大等级的特定图像。
	else
	{
		strName = "upgrade_0_CN.png";
	}

	//设置升级按钮精灵帧
	SpriteFrame* pUpGrade = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	myUpGrade->setSpriteFrame(pUpGrade);

	//出售
	int mymoney = 0;
	//通过累加从基本级别升级到当前级别的费用，计算武器的总价值
	for (int i = 0; i < pArms->getGrade(); i++)
	{
		mymoney = mymoney + pArms->getArmsDt()->vecMoney[i];
	}
	//根据总价值的80%生成出售按钮的图像文件名称
	strName = StringUtils::format("sell_%d.png", (int)(mymoney * 0.8));

	//设置为出售按钮的精灵帧
	SpriteFrame* pSell = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	mySell->setSpriteFrame(pSell);

}

//-----------------------------------------鼠标点击事件----------------------------------------//

void ArmsCard::addTouch() {

	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [](Touch* pTouch, Event*) {
		
		//获取触摸点在地图上的瓦片坐标
		Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(pTouch->getLocation());
		//判断瓦片位置上是否有武器
		MyArms* pArms = CGameScene::getInstance()->getBuildArms()->isHaveArms(tiledPos);

		//升级卡牌层可见
		if (CGameScene::getInstance()->getUpCard()->isVisible()) {
			CGameScene::getInstance()->getUpCard()->clickEventSecond(pTouch->getLocation(), pArms);
		}
		//升级卡牌层不可见，但是点击的是有武器的地方
		else if (pArms != NULL && !CGameScene::getInstance()->getArmsCard()->isVisible()) {
			CGameScene::getInstance()->getUpCard()->clickEventFirst(pTouch->getLocation(), pArms);
		}
		// 武器卡片不可见 且不点击路径 且不点击萝卜功能
		else if (!CGameScene::getInstance()->getArmsCard()->isVisible() && !CGameScene::getInstance()->getGameMap()->isInLayer("path", tiledPos))
		{
			CGameScene::getInstance()->getArmsCard()->clickEventFirst(pTouch->getLocation());
		}
		//武器卡片可见 升级武器
		else if (CGameScene::getInstance()->getArmsCard()->isVisible()) {
			CGameScene::getInstance()->getArmsCard()->clickEventSecond(pTouch->getLocation());
		}

		return true; 
	};

	//将事件监听器添加到当前场景节点。
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

}

