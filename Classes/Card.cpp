#include "Card.h"
#include "GameScene.h"

//-------------------------------------С������Ƭ---------------------------------------//

MyCard* MyCard::createMoneyCard(SCardDt* pCard)
{
	MyCard* pRef = new (std::nothrow) MyCard(); // ʹ�� std::nothrow �������׳��쳣

	if (pRef)
	{
		if (pRef->Sprite::initWithSpriteFrameName(StringUtils::format(pCard->strImg.c_str(), 1)))
		{
			// ���ó�Ա����
			pRef->myArmsID = pCard->nArmsID;
			pRef->CardName = pCard->strImg;

			// ��ȡ��������
			CArmsDtMgr* pArmsDtMgr = static_cast<CArmsDtMgr*>(CDataMgr::getInstance()->getData("ArmsMgr"));
			SArmsDt* pArmsDt = static_cast<SArmsDt*>(pArmsDtMgr->getDataByID(pRef->myArmsID));
			pRef->myArmsMoney = pArmsDt->vecMoney[0];

			return pRef;
		}

		CC_SAFE_DELETE(pRef);
	}

	return NULL;
}

/*ͨ��Ǯ��������ʾͼ������밵
nMoney�������ж���Ǯ*/
void MyCard::ifMoney(int myMoney)
{
	SpriteFrame* pFrame;
	//�Ƚϴ����Ǯ�����ڵ�Ǯ����ʾͼƬ�����Ͱ�
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

//----------------------------------------����������Ƭ------------------------------------------//

bool ArmsCard::init()
{
	this->addTouch();
	Sprite* pSelect = Sprite::createWithSpriteFrameName("select_01.png");
	addChild(pSelect);
	createAllArmsCard();
	//������һ�¾Ϳ��Կ��������
	setVisible(false);
	return true;
}

/*������Ƭ*/
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

		// ����Ƭ��ӵ���Ƭ�����У������濨Ƭ����������
		cardContainers->addChild(myCard);
		allCard.push_back(myCard);
	}

	int allhigh = allCard[0]->getContentSize().height;
	int allwidth = (allCard[0]->getContentSize().width) * i;
	cardContainers->setContentSize(Size(allwidth, allhigh));
	cardContainers->setAnchorPoint(Vec2(0, 0));

	// ����Ƭ������ӵ���ǰ����
	addChild(cardContainers);
}

void ArmsCard::clickEventFirst(Vec2 clickPos)
{
	// ���������ת��Ϊ��ͼ��Ƭ����
	Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(clickPos);

		// ���ÿ�Ƭ��Ŀ�Ƭ�����λ��
		for (MyCard* nowCard : allCard)
		{
			nowCard->ifMoney(CGameScene::getInstance()->getUILayer()->getMoney());
		}
		
		//�ǿ�Ƭ����Խ���߽�
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

	//�������п�Ƭ�ҵ�������Ŀ�Ƭ
	for (MyCard* pCard : allCard)
	{
		if (pCard->getBoundingBox().containsPoint(cardContainers->convertToNodeSpace(clickPos)))
		{
			nowCard = pCard;
			break ;
		}
	}

	if (nowCard!=NULL && CGameScene::getInstance()->getUILayer()->getMoney() >= nowCard->getMoney())//ӵ�н�Ҵ��ڿ�Ƭ������
	{
		//���������� ת��Ϊ��ͼ��Ƭ����
		Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(this->getPosition());
		//�������λ���Ѿ�����������ֱ�ӷ���
		if (!CGameScene::getInstance()->getBuildArms()->isHaveArms(tiledPos))
		{
			// ���������������ڿ�Ƭ���λ��
			CGameScene::getInstance()->getBuildArms()->createArms(nowCard->getArmsID(), tiledPos);
		}
	}

	// �ı�ɼ�ֵ��������һ�ε��
	setVisible(false);
	cardContainers->setAnchorPoint(Vec2(0, 0));
}

//-----------------------------------------����������Ƭ------------------------------------------//

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
	//��һ�ε�����������ĵط�,��ʾ��Ƭ
	Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(clickPos);
	Vec2 Pos = CGameScene::getInstance()->getGameMap()->getPixelByTiledPos(tiledPos);
	this->setPosition(Pos);
	this->upCard(pArms);
	nowArms = pArms;
	this->setVisible(true);
}

void UpCard::clickEventSecond(Vec2 clickPos, MyArms* pArms) {

	//����
	if (myUpGrade->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		//��Ƭ���Ƕ�����Ƭ
		if (nowArms->getGrade() < nowArms->getArmsDt()->vecMoney.size()) {
			//�õ�����������
			int nMoney = nowArms->getArmsDt()->vecMoney[nowArms->getGrade()];
			//ʣ���ұ����������������
			if (CGameScene::getInstance()->getUILayer()->getMoney() >= nMoney)
			{
				//����
				CGameScene::getInstance()->getUILayer()->addMoney(-nMoney);
				nowArms->upgrade();
			}
		}
	}

	//����
	if (mySell->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		int allNum = 0;
		//�������������������Ҷ���Ǯ
		for (int i = 0; i < nowArms->getGrade(); i++)
		{
			allNum = allNum + nowArms->getArmsDt()->vecMoney[i];
		}
		//��8������
		CGameScene::getInstance()->getUILayer()->addMoney(allNum * 0.8);

		//�������Ƴ���ȥ
		nowArms->removeFromParent();
		//�����һ�ε��ӵ�Ҳ�Ƴ���ȥ
		if (nowArms->getLastBullet() && nowArms->getLastBullet()->getTag() == 1)
		{
			nowArms->getLastBullet()->removeFromParent();
		}
	}

	this->setVisible(false);
}

void UpCard::upCard(MyArms* pArms)
{

	//����Ϊ��Χָʾ���ľ���֡
	SpriteFrame* pRander = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("range_%d.png", pArms->getArmsDt()->vecRange[pArms->getGrade() - 1]));
	myRander->setSpriteFrame(pRander);

	//����
	string strName;
	//�������û�дﵽ���ȼ���
	if (pArms->getGrade() < pArms->getArmsDt()->vecMoney.size()) {
		//�洢���������ķ��û򸺷���
		int nMoney = 0;
		//�����ҵ�ǰ�Ľ���Ƿ���ڻ����������������һ������ķ��á�
		if (CGameScene::getInstance()->getUILayer()->getMoney() >= pArms->getArmsDt()->vecMoney[pArms->getGrade()])
		{
			//���Ϊ�棬�� nMoney ������Ϊʵ�ʵ��������á�
			nMoney = pArms->getArmsDt()->vecMoney[pArms->getGrade()];
		}
		else {
			//���Ϊ�٣��� nMoney ������Ϊ�������õĸ�ֵ����ʾ���û���㹻�Ľ�ң���ƬӦ����ʾΪ��ɫ��
			nMoney = -pArms->getArmsDt()->vecMoney[pArms->getGrade()];
		}
		//������������ (nMoney) ����"������ť"��ͼ���ļ����ơ�
		strName = StringUtils::format("upgrade_%d.png", nMoney);
	}
	//��������Ѿ��ﵽ���ȼ���ͼ���ļ����Ʊ�����Ϊ�������ȼ����ض�ͼ��
	else
	{
		strName = "upgrade_0_CN.png";
	}

	//����������ť����֡
	SpriteFrame* pUpGrade = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	myUpGrade->setSpriteFrame(pUpGrade);

	//����
	int mymoney = 0;
	//ͨ���ۼӴӻ���������������ǰ����ķ��ã������������ܼ�ֵ
	for (int i = 0; i < pArms->getGrade(); i++)
	{
		mymoney = mymoney + pArms->getArmsDt()->vecMoney[i];
	}
	//�����ܼ�ֵ��80%���ɳ��۰�ť��ͼ���ļ�����
	strName = StringUtils::format("sell_%d.png", (int)(mymoney * 0.8));

	//����Ϊ���۰�ť�ľ���֡
	SpriteFrame* pSell = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	mySell->setSpriteFrame(pSell);

}

//-----------------------------------------������¼�----------------------------------------//

void ArmsCard::addTouch() {

	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [](Touch* pTouch, Event*) {
		
		//��ȡ�������ڵ�ͼ�ϵ���Ƭ����
		Vec2 tiledPos = CGameScene::getInstance()->getGameMap()->getTiledByPos(pTouch->getLocation());
		//�ж���Ƭλ�����Ƿ�������
		MyArms* pArms = CGameScene::getInstance()->getBuildArms()->isHaveArms(tiledPos);

		//�������Ʋ�ɼ�
		if (CGameScene::getInstance()->getUpCard()->isVisible()) {
			CGameScene::getInstance()->getUpCard()->clickEventSecond(pTouch->getLocation(), pArms);
		}
		//�������Ʋ㲻�ɼ������ǵ�������������ĵط�
		else if (pArms != NULL && !CGameScene::getInstance()->getArmsCard()->isVisible()) {
			CGameScene::getInstance()->getUpCard()->clickEventFirst(pTouch->getLocation(), pArms);
		}
		// ������Ƭ���ɼ� �Ҳ����·�� �Ҳ�����ܲ�����
		else if (!CGameScene::getInstance()->getArmsCard()->isVisible() && !CGameScene::getInstance()->getGameMap()->isInLayer("path", tiledPos))
		{
			CGameScene::getInstance()->getArmsCard()->clickEventFirst(pTouch->getLocation());
		}
		//������Ƭ�ɼ� ��������
		else if (CGameScene::getInstance()->getArmsCard()->isVisible()) {
			CGameScene::getInstance()->getArmsCard()->clickEventSecond(pTouch->getLocation());
		}

		return true; 
	};

	//���¼���������ӵ���ǰ�����ڵ㡣
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

}

