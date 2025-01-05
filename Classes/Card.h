#ifndef __CARD_H__
#define __CARD_H__
#include "cocos2d.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "Arms.h"
USING_NS_CC;

#define WINSIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()

class MyCard : public Sprite
{
public:
	static MyCard* createMoneyCard(SCardDt* pCard); // Create an instance
	void ifMoney(int nMoney);
	CC_SYNTHESIZE(int, myArmsID, ArmsID);
	CC_SYNTHESIZE(int, myArmsMoney, Money);
private:
	string CardName;  // Card image name
};

class ArmsCard : public Node
{
public:
	virtual bool init();
	void createAllArmsCard();                  // Create all cards
	void clickEventFirst(Vec2 clickPos);       // First click event
	void clickEventSecond(Vec2 clickPos);      // Second click event
	void addTouch();
	CREATE_FUNC(ArmsCard);
private:
	Sprite* cardContainers;
	std::vector<MyCard*> allCard;      // Store all card instances
}; 

class UpCard : public Node
{
public:
	virtual bool init();
	void clickEventFirst(Vec2 clickPos, MyArms* pArms);
	void clickEventSecond(Vec2 clickPos, MyArms* pArms);
	void upCard(MyArms* pArms);
	CREATE_FUNC(UpCard);
private:
	Sprite* myUpGrade;
	Sprite* mySell;
	Sprite* myRander;  // Range indicator
	MyArms* nowArms;   // Current arms
};

#endif