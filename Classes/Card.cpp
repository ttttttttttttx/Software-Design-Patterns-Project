#include "Card.h"
#include "GameScene.h"

//----------------------------------- Money Card -----------------------------------//

MyCard* MyCard::createMoneyCard(SCardDt* pCard)
{
	MyCard* pRef = new (std::nothrow) MyCard(); // Use std::nothrow to avoid exceptions

	if (pRef)
	{
		if (pRef->Sprite::initWithSpriteFrameName(StringUtils::format(pCard->strImg.c_str(), 1)))
		{
			// Set member variables
			pRef->myArmsID = pCard->nArmsID;
			pRef->CardName = pCard->strImg;

			// Get arms data
			CArmsDtMgr* pArmsDtMgr = static_cast<CArmsDtMgr*>(CDataMgr::getInstance()->getData("ArmsMgr"));
			SArmsDt* pArmsDt = static_cast<SArmsDt*>(pArmsDtMgr->getDataByID(pRef->myArmsID));
			pRef->myArmsMoney = pArmsDt->vecMoney[0];

			return pRef;
		}

		CC_SAFE_DELETE(pRef);
	}

	return NULL;
}

/* Update card display based on available money
   myMoney: current money amount */
void MyCard::ifMoney(int myMoney)
{
	SpriteFrame* pFrame;
	// Compare money with card cost to show enabled/disabled image
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

//----------------------------------- Arms Card -----------------------------------//

bool ArmsCard::init()
{
	this->addTouch();
	Sprite* pSelect = Sprite::createWithSpriteFrameName("select_01.png");
	addChild(pSelect);
	createAllArmsCard();
	// Initially invisible until clicked
	setVisible(false);
	return true;
}

/* Create all arms cards */
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

		// Add card to container and store in vector
		cardContainers->addChild(myCard);
		allCard.push_back(myCard);
	}

	int allhigh = allCard[0]->getContentSize().height;
	int allwidth = (allCard[0]->getContentSize().width) * i;
	cardContainers->setContentSize(Size(allwidth, allhigh));
	cardContainers->setAnchorPoint(Vec2(0, 0));

	// Add card container to current layer
	addChild(cardContainers);
}

void ArmsCard::clickEventFirst(Vec2 clickPos)
{
	// Convert click position to tile position
	Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(clickPos);

	// Update all cards' display based on current money
	for (MyCard* nowCard : allCard)
	{
		nowCard->ifMoney(CGameScene::getInstance()->getUILayer()->getMoney());
	}
	
	// Adjust anchor point if cards would exceed screen boundary
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

	// Find the card that was clicked
	for (MyCard* pCard : allCard)
	{
		if (pCard->getBoundingBox().containsPoint(cardContainers->convertToNodeSpace(clickPos)))
		{
			nowCard = pCard;
			break ;
		}
	}

	if (nowCard!=NULL && CGameScene::getInstance()->getUILayer()->getMoney() >= nowCard->getMoney())// If enough money to buy the card
	{
		// Convert click position to tile position
		Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(this->getPosition());
		// If no card exists at the clicked tile, create a new card
		if (!CGameScene::getInstance()->getBuildArms()->isHaveArms(tiledPos))
		{
			// Create a new card at the clicked tile
			CGameScene::getInstance()->getBuildArms()->createArms(nowCard->getArmsID(), tiledPos);
		}
	}

	// Hide the card
	setVisible(false);
	cardContainers->setAnchorPoint(Vec2(0, 0));
}

//----------------------------------- Upgrade Card -----------------------------------//

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
	// First click, show the card
	Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(clickPos);
	Vec2 Pos = CGameScene::getInstance()->getGameMap()->getPixelByTiledPos(tiledPos);
	this->setPosition(Pos);
	this->upCard(pArms);
	nowArms = pArms;
	this->setVisible(true);
}

void UpCard::clickEventSecond(Vec2 clickPos, MyArms* pArms) {

	// Upgrade card
	if (myUpGrade->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		// If the card is fully upgraded, add money
		if (nowArms->getGrade() < nowArms->getArmsDt()->vecMoney.size()) {
			// Get the money to add
			int nMoney = nowArms->getArmsDt()->vecMoney[nowArms->getGrade()];
			// Subtract the money from the player's total money
			// Refactored with Decorator Pattern
			CGameScene::getInstance()->getUILayer()->addMoney(-nMoney);
			try {
				// Create a new decorated arms
				auto myArms = MyArms::create();

				// Decorate the arms with additional effects
				auto decoratedArms = std::make_unique<RangeAndSpeedDecorator>(
					new DamageDecorator(
						new CritDecorator(myArms, 0.02f), // 2% increase in damage
						5.0f // 5 damage points
					),
					10.0f, // 10 tile range
					0.1f  // 0.1 seconds between shots
				);
				decoratedArms->upgrade(); // Upgrade the arms

				// Add the decorated arms to the scene
				scene->addChild(decoratedArms.release());

				// Use the decorated arms
				decoratedArms->fire();
			}
			catch (const std::exception& e) {
				log("Error: %s", e.what());
			}
		}
	}

	// Sell card
	if (mySell->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		int allNum = 0;
		// Add up the total money from the card
		for (int i = 0; i < nowArms->getGrade(); i++)
		{
			allNum = allNum + nowArms->getArmsDt()->vecMoney[i];
		}
		// Add 80% of the total money to the player's total money
		CGameScene::getInstance()->getUILayer()->addMoney(allNum * 0.8);

		// Remove the card from the scene
		nowArms->removeFromParent();
		// Remove the last bullet if it exists
		if (nowArms->getLastBullet() && nowArms->getLastBullet()->getTag() == 1)
		{
			nowArms->getLastBullet()->removeFromParent();
		}
	}

	this->setVisible(false);
}

void UpCard::upCard(MyArms* pArms)
{

	// Set the card's image based on the current upgrade level
	SpriteFrame* pRander = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("range_%d.png", pArms->getArmsDt()->vecRange[pArms->getGrade() - 1]));
	myRander->setSpriteFrame(pRander);

	// Set the card's name based on the current upgrade level
	string strName;
	// If the card is not fully upgraded, set the name based on the current upgrade level
	if (pArms->getGrade() < pArms->getArmsDt()->vecMoney.size()) {
		// Store the money to add
		int nMoney = 0;
		// If the player has enough money to upgrade the card, set nMoney to the actual money to add
		if (CGameScene::getInstance()->getUILayer()->getMoney() >= pArms->getArmsDt()->vecMoney[pArms->getGrade()])
		{
			// If the player has enough money to upgrade the card, set nMoney to the actual money to add
			nMoney = pArms->getArmsDt()->vecMoney[pArms->getGrade()];
		}
		else {
			// If the player does not have enough money to upgrade the card, set nMoney to the negative value of the card's current upgrade level
			nMoney = -pArms->getArmsDt()->vecMoney[pArms->getGrade()];
		}
		// Set the card's name based on the money to add
		strName = StringUtils::format("upgrade_%d.png", nMoney);
	}
	// If the card is fully upgraded, set the name to the fully upgraded image
	else
	{
		strName = "upgrade_0_CN.png";
	}

	// Set the card's image based on the card's name
	SpriteFrame* pUpGrade = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	myUpGrade->setSpriteFrame(pUpGrade);

	// Set the card's money based on the current upgrade level
	int mymoney = 0;
	// Add up the total money from the card
	for (int i = 0; i < pArms->getGrade(); i++)
	{
		mymoney = mymoney + pArms->getArmsDt()->vecMoney[i];
	}
	// Set the card's money based on the total money
	strName = StringUtils::format("sell_%d.png", (int)(mymoney * 0.8));

	// Set the card's image based on the card's name
	SpriteFrame* pSell = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	mySell->setSpriteFrame(pSell);

}

//----------------------------------- Touch Event -----------------------------------//

void ArmsCard::addTouch() {

	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [](Touch* pTouch, Event*) {
		
		// Get the position of the touch on the game map
		Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(pTouch->getLocation());
		// Check if there is a card at the touched tile
		MyArms* pArms = CGameScene::getInstance()->getBuildArms()->isHaveArms(tiledPos);

		// If the upgrade card is visible, click the upgrade card
		if (CGameScene::getInstance()->getUpCard()->isVisible()) {
			CGameScene::getInstance()->getUpCard()->clickEventSecond(pTouch->getLocation(), pArms);
		}
		// If no card is visible and the touched tile is not occupied, click the upgrade card
		else if (pArms != NULL && !CGameScene::getInstance()->getArmsCard()->isVisible()) {
			CGameScene::getInstance()->getUpCard()->clickEventFirst(pTouch->getLocation(), pArms);
		}
		// If no card is visible and the touched tile is not occupied, click the upgrade card
		else if (!CGameScene::getInstance()->getArmsCard()->isVisible() && !CGameScene::getInstance()->getGameMap()->isInLayer("path", tiledPos))
		{
			CGameScene::getInstance()->getArmsCard()->clickEventFirst(pTouch->getLocation());
		}
		// If a card is visible, click the card
		else if (CGameScene::getInstance()->getArmsCard()->isVisible()) {
			CGameScene::getInstance()->getArmsCard()->clickEventSecond(pTouch->getLocation());
		}

		return true; 
	};

	// Add the touch listener to the current layer
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

}

